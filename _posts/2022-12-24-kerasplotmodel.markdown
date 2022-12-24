---
layout: post
title: "Fixing Keras plot_model bug with expand_nested=True on functional models"
date: Sat Dec 24 10:58:16 EST 2022
tags: code
highlightjs:
  - diff
---

Keras has a bug on its function `plot_model()`. It works fine for simple models
but when you have *nested* models, i.e., a model that depends on some
submodels, the plot would be wrong. Specifically, it would be wrong if the
submodel is not single-input, single-output (i.e., it must be created using
functional API). It would be misleading when you depend on the plot to check
the architecture.

To see what's wrong, we can try out this test code. Adapted from
<https://github.com/tensorflow/tensorflow/issues/42101> and it requires
Graphviz installation to work:

```python
import tensorflow as tf

def test1():
    inn = tf.keras.Input((1,),name="in1")
    d1 = tf.keras.layers.Dense(1, name="d1")(inn)
    d2 = tf.keras.layers.Dense(1, name="d2")(inn)
    m1 = tf.keras.Model(inputs=inn, outputs=[d1,d2], name="model1")

    in2_1 = tf.keras.Input((1,),name="in1")
    in2_2 = tf.keras.Input((1,),name="in2")
    m2 = tf.keras.Model(inputs=[in2_1,in2_2],outputs=[in2_1 + in2_2],name="model2")

    # combined model
    in0 = tf.keras.Input((1,),name="in0")
    m = tf.keras.Model(inputs=in0, outputs=m2(m1(in0)))

    tf.keras.utils.plot_model(m, to_file="model1.png", show_shapes=True, expand_nested=True)

def test2():
    inn = tf.keras.Input((1,),name="in1")
    d1 = tf.keras.layers.Dense(1, name="d1")(inn)
    d2 = tf.keras.layers.Dense(1, name="d2")(inn)
    m1 = tf.keras.Model(inputs=inn, outputs=[d1,d2], name="model1")

    d3 = tf.keras.layers.Dense(1, name="d1")
    d4 = tf.keras.layers.Dense(1, name="d2")

    # combined model
    in0 = tf.keras.Input((1,),name="in0")
    out1, out2 = m1(in0)
    add = tf.keras.layers.Add()
    m = tf.keras.Model(inputs=in0, outputs=add([d3(out1),d4(out2)]))

    tf.keras.utils.plot_model(m, to_file="model2.png", show_shapes=True, expand_nested=True)

test1()
test2()
```

What it gives you are the following:

![](/img/keras-models-bad.png)

Which, in fact, they should be like this:

![](/img/keras-models-good.png)

To know what's wrong, let's do this:

```
>>> import tensorflow as tf
>>> tf.keras.utils.plot_model.__code__.co_filename
'/opt/homebrew/lib/python3.10/site-packages/keras/utils/vis_utils.py'
```

So we know the `plot_model()` function is using Keras' code (depends on
environment, not always the case, as there is a copy of the same code in
TensorFlow as well). In that code, we can see `plot_model()` create the picture
using `model_to_dot()`, which is to using pydot to generate Graphviz language
representation.

The problem is with `model_to_dot()`.  We see the following over there:

```python
def model_to_dot(...):
    ...
    sub_n_first_node = {}
    sub_n_last_node = {}

    ...
    # Create graph nodes.
    for i, layer in enumerate(layers):
        ...
        if expand_nested and isinstance(layer, functional.Functional):
            submodel_not_wrapper = model_to_dot(...)
            # sub_n : submodel_not_wrapper
            sub_n_nodes = submodel_not_wrapper.get_nodes()
            sub_n_first_node[layer.name] = sub_n_nodes[0]
            sub_n_last_node[layer.name] = sub_n_nodes[-1]
            dot.add_subgraph(submodel_not_wrapper)

        ...

    # Connect nodes with edges.
    for i, layer in enumerate(layers):
        ...
        layer_id = str(id(layer))
        for i, node in enumerate(layer._inbound_nodes):
            node_key = layer.name + "_ib-" + str(i)
            if node_key in model._network_nodes:
                for inbound_layer in tf.nest.flatten(node.inbound_layers):
                    inbound_layer_id = str(id(inbound_layer))
                    if not expand_nested:
                        ...
                    else:
                        # if inbound_layer is not Model or wrapped Model
                        if not isinstance(
                            inbound_layer, functional.Functional
                        ) and not is_wrapped_model(inbound_layer):
                            # if current layer is not Model or wrapped Model
                            if not isinstance(
                                layer, functional.Functional
                            ) and not is_wrapped_model(layer):
                                assert dot.get_node(inbound_layer_id)
                                assert dot.get_node(layer_id)
                                add_edge(dot, inbound_layer_id, layer_id)
                            # if current layer is Model
                            elif isinstance(layer, functional.Functional):
                                add_edge(
                                    dot,
                                    inbound_layer_id,
                                    sub_n_first_node[layer.name].get_name(),
                                )
                            # if current layer is wrapped Model
                            elif is_wrapped_model(layer):
                                add_edge(dot, inbound_layer_id, layer_id)
                                name = sub_w_first_node[
                                    layer.layer.name
                                ].get_name()
                                add_edge(dot, layer_id, name)
                        # if inbound_layer is Model
                        elif isinstance(inbound_layer, functional.Functional):
                            name = sub_n_last_node[
                                inbound_layer.name
                            ].get_name()
                            if isinstance(layer, functional.Functional):
                                output_name = sub_n_first_node[
                                    layer.name
                                ].get_name()
                                add_edge(dot, name, output_name)
                            else:
                                add_edge(dot, name, layer_id)
                        # if inbound_layer is wrapped Model
                        elif is_wrapped_model(inbound_layer):
                            inbound_layer_name = inbound_layer.layer.name
                            add_edge(
                                dot,
                                sub_w_last_node[inbound_layer_name].get_name(),
                                layer_id,
                            )
    return dot
```

In Keras, a submodel can be a *wrapped model* (i.e., the `TimeDistributed`
layer and `Bidirectional` layer, which are related to recurrent architectures)
or a *functional* model.
Functional model can be complicated but in the above, we remember only the
first and last node from a functional model and connect the input and output to
them. What if the input/output are not the first or last? What if there are
multiple input or multiple output? Those are the cases that the test code above
failed.

Fixing this is not difficult but we need a new approach of identifying what are
the nodes connect into or out of the submodel.

A TensorFlow/Keras model is manipulating *tensors*. A tensor is an object with
a name. However, a tensor does not remember the layer it is associated with.
Only the layer remembers the tensor. In a model, we can find out its input and
output tensors. However, these tensors would not allow you to trace the
connections.

So the solution is to use the name of tensors. It is required that layers
immediately used by a model carry unique names. Layers of different models can
share the name, but when we combined them into a bigger model, the model name
will be prefixed to it like a file system path. With the name of a model, and
the name of a layer, we can identify which layer in a bigger model produces a
particular tensor.

In code, it means, first, remember all output tensors and associate with the
layer id (which are the node names in Graphviz); and return the assocation as
an attribute in the Graphviz graph:

```python
def model_to_dot(...):
    ...
    tensors_map = {}
    ...

    for i, layer in enumerate(layers):
        ...

        layer_id = str(id(layer))
        try:
            if isinstance(layer.output, list):
                for tensor in layer.output:
                    tensors_map[tensor.name] = layer_id
            else:
                tensors_map[layer.output.name] = layer_id
        except AttributeError:
            pass  # layer.output not exists

    ...
    dot._tensors_map = tensors_map
    return dot
```

and then, when we obtained a subgraph (i.e., from a submodel), we merge the
name-layer map:

```python
        if isinstance(layer, Wrapper):
            if expand_nested and isinstance(layer.layer, functional.Functional):
                submodel_wrapper = model_to_dot(layer.layer, ...)
                # sub_w : submodel_wrapper
                sub_w_nodes = submodel_wrapper.get_nodes()
                sub_w_first_node[layer.layer.name] = sub_w_nodes[0]
                sub_w_last_node[layer.layer.name] = sub_w_nodes[-1]
                dot.add_subgraph(submodel_wrapper)
                for key, val in submodel_wrapper._tensors_map.items():
                    tensors_map[key] = val
                    tensors_map[f"{layer.name}/{key}"] = val
            else:
                layer_name = f"{layer_name}({layer.layer.name})"
                child_class_name = layer.layer.__class__.__name__
                class_name = f"{class_name}({child_class_name})"

        if expand_nested and isinstance(layer, functional.Functional):
            submodel_not_wrapper = model_to_dot(layer, ...)
            # sub_n : submodel_not_wrapper
            dot.add_subgraph(submodel_not_wrapper)
            for key, val in submodel_not_wrapper._tensors_map.items():
                tensors_map[key] = val
                tensors_map[f"{layer.name}/{key}"] = val
```

This way, we get all the nodes ready. Now the edges.

```python
    # Connect nodes with edges.
    for i, layer in enumerate(layers):
        ...
        layer_id = str(id(layer))
        if not expand_nested:
            ...
        elif isinstance(layer, functional.Functional):
            # if layer is Model, we connect based on input tensors
            sources, targets = [], []
            for node in layer.inbound_nodes:
                for tensor in tf.nest.flatten(node.input_tensors):
                    sources.append(tensors_map[tensor.name])
            for tensor in tf.nest.flatten(layer.input):
                targets.append(str(id(tensor.node.layer)))
            while len(targets) < len(sources):
                targets.append(layer_id)
            for s, t in zip(sources, targets):
                add_edge(dot, s, t)
            if is_wrapped_model(layer):
                name = sub_w_first_node[layer.layer.name].get_name()
                add_edge(dot, layer_id, name)
        else:
            for i, node in enumerate(layer._inbound_nodes):
                node_key = layer.name + "_ib-" + str(i)
                if node_key not in model._network_nodes:
                    continue
                for inbound_layer in tf.nest.flatten(node.inbound_layers):
                    inbound_layer_id = str(id(inbound_layer))
                    if not expand_nested:
                        ...
                    else:
                        # if inbound_layer is not Model or wrapped Model
                        if not isinstance(
                            inbound_layer, functional.Functional
                        ) and not is_wrapped_model(inbound_layer):
                            ...
                        # if inbound_layer is Model
                        elif isinstance(inbound_layer, functional.Functional):
                            sources = []
                            for tensor in tf.nest.flatten(layer.input):
                                source = tensors_map[tensor.name]
                                add_edge(dot, source, layer_id)
                        # if inbound_layer is wrapped Model
                        elif is_wrapped_model(inbound_layer):
                            ...
```

Basically, we handle the case of connecting from or to a functional submodel
differently in the sense that we will figure out the tensor connections, by
name, using the name-layer map we built earlier.

The complete code is as follows,

{% raw %}
```python
@keras_export("keras.utils.model_to_dot")
def model_to_dot(
    model,
    show_shapes=False,
    show_dtype=False,
    show_layer_names=True,
    rankdir="TB",
    expand_nested=False,
    dpi=96,
    subgraph=False,
    layer_range=None,
    show_layer_activations=False,
):
    """Convert a Keras model to dot format.

    Args:
      model: A Keras model instance.
      show_shapes: whether to display shape information.
      show_dtype: whether to display layer dtypes.
      show_layer_names: whether to display layer names.
      rankdir: `rankdir` argument passed to PyDot,
          a string specifying the format of the plot:
          'TB' creates a vertical plot;
          'LR' creates a horizontal plot.
      expand_nested: whether to expand nested models into clusters.
      dpi: Dots per inch.
      subgraph: whether to return a `pydot.Cluster` instance.
      layer_range: input of `list` containing two `str` items, which is the
          starting layer name and ending layer name (both inclusive) indicating
          the range of layers for which the `pydot.Dot` will be generated. It
          also accepts regex patterns instead of exact name. In such case, start
          predicate will be the first element it matches to `layer_range[0]`
          and the end predicate will be the last element it matches to
          `layer_range[1]`. By default `None` which considers all layers of
          model. Note that you must pass range such that the resultant subgraph
          must be complete.
      show_layer_activations: Display layer activations (only for layers that
          have an `activation` property).

    Returns:
      A `pydot.Dot` instance representing the Keras model or
      a `pydot.Cluster` instance representing nested model if
      `subgraph=True`.

    Raises:
      ValueError: if `model_to_dot` is called before the model is built.
      ImportError: if pydot is not available.
    """

    if not model.built:
        raise ValueError(
            "This model has not yet been built. "
            "Build the model first by calling `build()` or by calling "
            "the model on a batch of data."
        )

    from keras.engine import functional
    from keras.engine import sequential
    from keras.layers import Wrapper

    if not check_pydot():
        raise ImportError(
            "You must install pydot (`pip install pydot`) for "
            "model_to_dot to work."
        )

    if subgraph:
        dot = pydot.Cluster(style="dashed", graph_name=model.name)
        dot.set("label", model.name)
        dot.set("labeljust", "l")
    else:
        dot = pydot.Dot()
        dot.set("rankdir", rankdir)
        dot.set("concentrate", True)
        dot.set("dpi", dpi)
        dot.set_node_defaults(shape="record")

    if layer_range is not None:
        if len(layer_range) != 2:
            raise ValueError(
                "layer_range must be of shape (2,). Received: "
                f"layer_range = {layer_range} of length {len(layer_range)}"
            )
        if not isinstance(layer_range[0], str) or not isinstance(
            layer_range[1], str
        ):
            raise ValueError(
                "layer_range should contain string type only. "
                f"Received: {layer_range}"
            )
        layer_range = layer_utils.get_layer_index_bound_by_layer_name(
            model, layer_range
        )
        if layer_range[0] < 0 or layer_range[1] > len(model.layers):
            raise ValueError(
                "Both values in layer_range should be in range (0, "
                f"{len(model.layers)}. Received: {layer_range}"
            )

    tensors_map = {}
    sub_w_first_node = {}
    sub_w_last_node = {}

    layers = model.layers
    if not model._is_graph_network:
        node = pydot.Node(str(id(model)), label=model.name)
        dot.add_node(node)
        return dot
    elif isinstance(model, sequential.Sequential):
        if not model.built:
            model.build()
        layers = super(sequential.Sequential, model).layers

    # Create graph nodes.
    for i, layer in enumerate(layers):
        if (layer_range) and (i < layer_range[0] or i >= layer_range[1]):
            continue

        layer_id = str(id(layer))
        try:
            if isinstance(layer.output, list):
                for tensor in layer.output:
                    tensors_map[tensor.name] = layer_id
            else:
                tensors_map[layer.output.name] = layer_id
        except AttributeError:
            pass  # layer.output not exists

        # Append a wrapped layer's label to node's label, if it exists.
        layer_name = layer.name
        class_name = layer.__class__.__name__

        if isinstance(layer, Wrapper):
            if expand_nested and isinstance(layer.layer, functional.Functional):
                submodel_wrapper = model_to_dot(
                    layer.layer,
                    show_shapes,
                    show_dtype,
                    show_layer_names,
                    rankdir,
                    expand_nested,
                    subgraph=True,
                    show_layer_activations=show_layer_activations,
                )
                # sub_w : submodel_wrapper
                sub_w_nodes = submodel_wrapper.get_nodes()
                sub_w_first_node[layer.layer.name] = sub_w_nodes[0]
                sub_w_last_node[layer.layer.name] = sub_w_nodes[-1]
                dot.add_subgraph(submodel_wrapper)
                for key, val in submodel_wrapper._tensors_map.items():
                    tensors_map[key] = val
                    tensors_map[f"{layer.name}/{key}"] = val
            else:
                layer_name = f"{layer_name}({layer.layer.name})"
                child_class_name = layer.layer.__class__.__name__
                class_name = f"{class_name}({child_class_name})"

        if expand_nested and isinstance(layer, functional.Functional):
            submodel_not_wrapper = model_to_dot(
                layer,
                show_shapes,
                show_dtype,
                show_layer_names,
                rankdir,
                expand_nested,
                subgraph=True,
                show_layer_activations=show_layer_activations,
            )
            # sub_n : submodel_not_wrapper
            dot.add_subgraph(submodel_not_wrapper)
            for key, val in submodel_not_wrapper._tensors_map.items():
                tensors_map[key] = val
                tensors_map[f"{layer.name}/{key}"] = val

        # Create node's label.
        label = class_name

        # Rebuild the label as a table including the layer's activation.
        if (
            show_layer_activations
            and hasattr(layer, "activation")
            and layer.activation is not None
        ):
            if hasattr(layer.activation, "name"):
                activation_name = layer.activation.name
            elif hasattr(layer.activation, "__name__"):
                activation_name = layer.activation.__name__
            else:
                activation_name = str(layer.activation)
            label = "{%s|%s}" % (label, activation_name)

        # Rebuild the label as a table including the layer's name.
        if show_layer_names:
            if not layer.trainable:
                label = f"{layer_name}*|{label}"
            else:
                label = f"{layer_name}|{label}"

        # Rebuild the label as a table including the layer's dtype.
        if show_dtype:

            def format_dtype(dtype):
                if dtype is None:
                    return "?"
                else:
                    return str(dtype)

            label = f"{label}|{format_dtype(layer.dtype)}"

        # Rebuild the label as a table including input/output shapes.
        if show_shapes:

            def format_shape(shape):
                return (
                    str(shape)
                    .replace(str(None), "None")
                    .replace("{", "/{")
                    .replace("}", "/}")
                )

            try:
                outputlabels = format_shape(layer.output_shape)
            except AttributeError:
                outputlabels = "?"
            if hasattr(layer, "input_shape"):
                inputlabels = format_shape(layer.input_shape)
            elif hasattr(layer, "input_shapes"):
                inputlabels = ", ".join(
                    [format_shape(ishape) for ishape in layer.input_shapes]
                )
            else:
                inputlabels = "?"
            label = "{%s}|{input:|output:}|{{%s}|{%s}}" % (
                label,
                inputlabels,
                outputlabels,
            )
        if not expand_nested or not isinstance(layer, functional.Functional):
            node = pydot.Node(layer_id, label=label)
            dot.add_node(node)

    # Connect nodes with edges.
    for i, layer in enumerate(layers):
        if (layer_range) and (i <= layer_range[0] or i >= layer_range[1]):
            continue
        layer_id = str(id(layer))
        if not expand_nested:
            for i, node in enumerate(layer._inbound_nodes):
                node_key = layer.name + "_ib-" + str(i)
                if node_key not in model._network_nodes:
                    continue
                for inbound_layer in tf.nest.flatten(node.inbound_layers):
                    inbound_layer_id = str(id(inbound_layer))
                    assert dot.get_node(inbound_layer_id)
                    assert dot.get_node(layer_id)
                    add_edge(dot, inbound_layer_id, layer_id)
        elif isinstance(layer, functional.Functional):
            # if layer is Model, we connect based on input tensors
            sources, targets = [], []
            for node in layer.inbound_nodes:
                for tensor in tf.nest.flatten(node.input_tensors):
                    sources.append(tensors_map[tensor.name])
            for tensor in tf.nest.flatten(layer.input):
                targets.append(str(id(tensor.node.layer)))
            while len(targets) < len(sources):
                targets.append(layer_id)
            for s, t in zip(sources, targets):
                add_edge(dot, s, t)
            if is_wrapped_model(layer):
                name = sub_w_first_node[layer.layer.name].get_name()
                add_edge(dot, layer_id, name)
        else:
            for i, node in enumerate(layer._inbound_nodes):
                node_key = layer.name + "_ib-" + str(i)
                if node_key not in model._network_nodes:
                    continue
                for inbound_layer in tf.nest.flatten(node.inbound_layers):
                    inbound_layer_id = str(id(inbound_layer))
                    if not expand_nested:
                        assert dot.get_node(inbound_layer_id)
                        assert dot.get_node(layer_id)
                        add_edge(dot, inbound_layer_id, layer_id)
                    else:
                        # if inbound_layer is not Model or wrapped Model
                        if not isinstance(
                            inbound_layer, functional.Functional
                        ) and not is_wrapped_model(inbound_layer):
                            # if current layer is not Model or wrapped Model
                            if not is_wrapped_model(layer):
                                assert dot.get_node(inbound_layer_id)
                                assert dot.get_node(layer_id)
                                add_edge(dot, inbound_layer_id, layer_id)
                            # if current layer is wrapped Model
                            elif is_wrapped_model(layer):
                                add_edge(dot, inbound_layer_id, layer_id)
                                name = sub_w_first_node[
                                    layer.layer.name
                                ].get_name()
                                add_edge(dot, layer_id, name)
                        # if inbound_layer is Model
                        elif isinstance(inbound_layer, functional.Functional):
                            sources = []
                            for tensor in tf.nest.flatten(layer.input):
                                source = tensors_map[tensor.name]
                                add_edge(dot, source, layer_id)
                        # if inbound_layer is wrapped Model
                        elif is_wrapped_model(inbound_layer):
                            inbound_layer_name = inbound_layer.layer.name
                            add_edge(
                                dot,
                                sub_w_last_node[inbound_layer_name].get_name(),
                                layer_id,
                            )
    dot._tensors_map = tensors_map
    return dot
```
{% endraw %}

and the diff:

```diff
--- vis_utils.py.orig	2022-12-14 20:21:50
+++ vis_utils.py.new	2022-12-24 12:34:56
@@ -173,8 +173,7 @@
                 f"{len(model.layers)}. Received: {layer_range}"
             )

-    sub_n_first_node = {}
-    sub_n_last_node = {}
+    tensors_map = {}
     sub_w_first_node = {}
     sub_w_last_node = {}

@@ -194,6 +193,14 @@
             continue

         layer_id = str(id(layer))
+        try:
+            if isinstance(layer.output, list):
+                for tensor in layer.output:
+                    tensors_map[tensor.name] = layer_id
+            else:
+                tensors_map[layer.output.name] = layer_id
+        except AttributeError:
+            pass  # layer.output not exists

         # Append a wrapped layer's label to node's label, if it exists.
         layer_name = layer.name
@@ -209,12 +216,16 @@
                     rankdir,
                     expand_nested,
                     subgraph=True,
+                    show_layer_activations=show_layer_activations,
                 )
                 # sub_w : submodel_wrapper
                 sub_w_nodes = submodel_wrapper.get_nodes()
                 sub_w_first_node[layer.layer.name] = sub_w_nodes[0]
                 sub_w_last_node[layer.layer.name] = sub_w_nodes[-1]
                 dot.add_subgraph(submodel_wrapper)
+                for key, val in submodel_wrapper._tensors_map.items():
+                    tensors_map[key] = val
+                    tensors_map[f"{layer.name}/{key}"] = val
             else:
                 layer_name = f"{layer_name}({layer.layer.name})"
                 child_class_name = layer.layer.__class__.__name__
@@ -229,12 +240,13 @@
                 rankdir,
                 expand_nested,
                 subgraph=True,
+                show_layer_activations=show_layer_activations,
             )
             # sub_n : submodel_not_wrapper
-            sub_n_nodes = submodel_not_wrapper.get_nodes()
-            sub_n_first_node[layer.name] = sub_n_nodes[0]
-            sub_n_last_node[layer.name] = sub_n_nodes[-1]
             dot.add_subgraph(submodel_not_wrapper)
+            for key, val in submodel_not_wrapper._tensors_map.items():
+                tensors_map[key] = val
+                tensors_map[f"{layer.name}/{key}"] = val

         # Create node's label.
         label = class_name
@@ -255,7 +267,10 @@

         # Rebuild the label as a table including the layer's name.
         if show_layer_names:
-            label = f"{layer_name}|{label}"
+            if not layer.trainable:
+                label = f"{layer_name}*|{label}"
+            else:
+                label = f"{layer_name}|{label}"

         # Rebuild the label as a table including the layer's dtype.
         if show_dtype:
@@ -305,11 +320,38 @@
         if (layer_range) and (i <= layer_range[0] or i >= layer_range[1]):
             continue
         layer_id = str(id(layer))
-        for i, node in enumerate(layer._inbound_nodes):
-            node_key = layer.name + "_ib-" + str(i)
-            if node_key in model._network_nodes:
+        if not expand_nested:
+            for i, node in enumerate(layer._inbound_nodes):
+                node_key = layer.name + "_ib-" + str(i)
+                if node_key not in model._network_nodes:
+                    continue
                 for inbound_layer in tf.nest.flatten(node.inbound_layers):
                     inbound_layer_id = str(id(inbound_layer))
+                    assert dot.get_node(inbound_layer_id)
+                    assert dot.get_node(layer_id)
+                    add_edge(dot, inbound_layer_id, layer_id)
+        elif isinstance(layer, functional.Functional):
+            # if layer is Model, we connect based on input tensors
+            sources, targets = [], []
+            for node in layer.inbound_nodes:
+                for tensor in tf.nest.flatten(node.input_tensors):
+                    sources.append(tensors_map[tensor.name])
+            for tensor in tf.nest.flatten(layer.input):
+                targets.append(str(id(tensor.node.layer)))
+            while len(targets) < len(sources):
+                targets.append(layer_id)
+            for s, t in zip(sources, targets):
+                add_edge(dot, s, t)
+            if is_wrapped_model(layer):
+                name = sub_w_first_node[layer.layer.name].get_name()
+                add_edge(dot, layer_id, name)
+        else:
+            for i, node in enumerate(layer._inbound_nodes):
+                node_key = layer.name + "_ib-" + str(i)
+                if node_key not in model._network_nodes:
+                    continue
+                for inbound_layer in tf.nest.flatten(node.inbound_layers):
+                    inbound_layer_id = str(id(inbound_layer))
                     if not expand_nested:
                         assert dot.get_node(inbound_layer_id)
                         assert dot.get_node(layer_id)
@@ -320,19 +362,10 @@
                             inbound_layer, functional.Functional
                         ) and not is_wrapped_model(inbound_layer):
                             # if current layer is not Model or wrapped Model
-                            if not isinstance(
-                                layer, functional.Functional
-                            ) and not is_wrapped_model(layer):
+                            if not is_wrapped_model(layer):
                                 assert dot.get_node(inbound_layer_id)
                                 assert dot.get_node(layer_id)
                                 add_edge(dot, inbound_layer_id, layer_id)
-                            # if current layer is Model
-                            elif isinstance(layer, functional.Functional):
-                                add_edge(
-                                    dot,
-                                    inbound_layer_id,
-                                    sub_n_first_node[layer.name].get_name(),
-                                )
                             # if current layer is wrapped Model
                             elif is_wrapped_model(layer):
                                 add_edge(dot, inbound_layer_id, layer_id)
@@ -342,16 +375,10 @@
                                 add_edge(dot, layer_id, name)
                         # if inbound_layer is Model
                         elif isinstance(inbound_layer, functional.Functional):
-                            name = sub_n_last_node[
-                                inbound_layer.name
-                            ].get_name()
-                            if isinstance(layer, functional.Functional):
-                                output_name = sub_n_first_node[
-                                    layer.name
-                                ].get_name()
-                                add_edge(dot, name, output_name)
-                            else:
-                                add_edge(dot, name, layer_id)
+                            sources = []
+                            for tensor in tf.nest.flatten(layer.input):
+                                source = tensors_map[tensor.name]
+                                add_edge(dot, source, layer_id)
                         # if inbound_layer is wrapped Model
                         elif is_wrapped_model(inbound_layer):
                             inbound_layer_name = inbound_layer.layer.name
@@ -360,6 +387,7 @@
                                 sub_w_last_node[inbound_layer_name].get_name(),
                                 layer_id,
                             )
+    dot._tensors_map = tensors_map
     return dot


```

Note that I modified the function a bit to print an asterisk for the
non-trainable layers. I found this useful.
