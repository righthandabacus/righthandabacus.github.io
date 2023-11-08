---
layout: post
title: Great Expectations
date: 2023-01-30
tags: code
category: blog
---

Great expectations is a Python library for data validation. It tries to profile
a sample dataset for what are the attributes, and the statistics of the
attributes (e.g., if numeric, what are min, max, mean, variance, etc.). If you
read the documentation, it would suggest you to create a data repository with
it, then load the data and let it do the profiling. When you get a new version
of the same data (e.g., data of a different date), you can reuse the profile to
validate for any outliers or unexpected attributes.

But this design is a violation of the Unix philosophy of single responsibility:
Why it has to assume how do I store my data? In fact, you do not need to use
great expectation as your data manager. But the library's documentation didn't
make it clear.

## Work flow

Let's see how to use great expectation the minimal way. First let's check the
sample data (Taiwan Credit):

```python
import pandas as pd

# Load data as pandas DataFrame
df_credit = pd.read_csv("data/TaiwanCredit.csv", low_memory=False)
with pd.option_context('display.max_rows', 200, 'display.max_columns', 200, 'display.width', 0):
    print(pd.DataFrame({"dtype":df_credit.dtypes})
          .join(df_credit.describe().T)
         )
    print()
    print(df_credit.sample(3).T)
    print()

df_credit.info()
```

its output will be:

```
               dtype    count           mean            std           min           25%            50%            75%             max
LIMIT_BAL    float64  30000.0  167484.322667  129747.661567  10000.000000  50000.000000  140000.000000  240000.000000  1000000.000000
SEX          float64  30000.0       1.603733       0.489129      1.000000      1.000000       2.000000       2.000000        2.000000
EDUCATION    float64  30000.0       1.779867       0.728486      0.000000      1.000000       2.000000       2.000000        3.000000
MARRIAGE     float64  30000.0       1.519567       0.524175      0.000000      1.000000       2.000000       2.000000        2.000000
AGE          float64  30000.0      35.485500       9.217904     21.000000     28.000000      34.000000      41.000000       79.000000
PAY_1        float64  30000.0       0.167233       0.931323     -1.000000      0.000000       0.000000       0.000000        8.000000
PAY_2        float64  30000.0       0.118367       0.965844     -1.000000      0.000000       0.000000       0.000000        8.000000
PAY_3        float64  30000.0       0.106133       0.950876     -1.000000      0.000000       0.000000       0.000000        8.000000
PAY_4        float64  30000.0       0.069200       0.911613     -1.000000      0.000000       0.000000       0.000000        8.000000
PAY_5        float64  30000.0       0.036867       0.864561     -1.000000      0.000000       0.000000       0.000000        8.000000
PAY_6        float64  30000.0       0.035233       0.867919     -1.000000      0.000000       0.000000       0.000000        8.000000
BILL_AMT1    float64  30000.0       3.885958       1.547037     -5.219010      3.551419       4.349909       4.826671        5.984308
BILL_AMT2    float64  30000.0       3.795420       1.654534     -4.843718      3.475053       4.326357       4.806229        5.992965
BILL_AMT3    float64  30000.0       3.739384       1.696840     -5.196632      3.426064       4.302969       4.779349        6.221177
BILL_AMT4    float64  30000.0       3.670062       1.733904     -5.230452      3.366936       4.279964       4.736452        5.950164
BILL_AMT5    float64  30000.0       3.599323       1.762540     -4.910277      3.246499       4.257811       4.700630        5.967160
BILL_AMT6    float64  30000.0       3.507501       1.833062     -5.530973      3.099335       4.232285       4.691958        5.983024
PAY_AMT1     float64  30000.0       2.879374       1.411589      0.000000      3.000434       3.322426       3.699578        5.941289
PAY_AMT2     float64  30000.0       2.850124       1.423945      0.000000      2.921166       3.303196       3.699057        6.226409
PAY_AMT3     float64  30000.0       2.728754       1.454766      0.000000      2.592177       3.255514       3.653791        5.952328
PAY_AMT4     float64  30000.0       2.639519       1.475124      0.000000      2.472756       3.176381       3.603604        5.793092
PAY_AMT5     float64  30000.0       2.619748       1.495708      0.000000      2.403976       3.176381       3.605574        5.629950
PAY_AMT6     float64  30000.0       2.576596       1.532231      0.000000      2.074631       3.176381       3.602169        5.723182
FlagDefault  float64  30000.0       0.221200       0.415062      0.000000      0.000000       0.000000       0.000000        1.000000

                    28247          15710         8917
LIMIT_BAL    30000.000000  270000.000000  30000.000000
SEX              2.000000       1.000000      2.000000
EDUCATION        2.000000       1.000000      2.000000
MARRIAGE         1.000000       1.000000      2.000000
AGE             32.000000      45.000000     27.000000
PAY_1            0.000000       1.000000      0.000000
PAY_2            0.000000      -1.000000      0.000000
PAY_3            0.000000      -1.000000      0.000000
PAY_4            0.000000      -1.000000      0.000000
PAY_5            0.000000      -1.000000      0.000000
PAY_6            0.000000      -1.000000      0.000000
BILL_AMT1        3.538448       0.000000      4.419377
BILL_AMT2       -0.477121       2.445604      4.439917
BILL_AMT3       -0.477121       2.784617      4.458970
BILL_AMT4       -0.477121       2.694605      4.443185
BILL_AMT5        2.850646       2.499687      4.451832
BILL_AMT6        2.973590       0.000000      4.473136
PAY_AMT1         0.000000       2.445604      3.301247
PAY_AMT2         0.000000       2.784617      3.302331
PAY_AMT3         0.000000       2.694605      3.000434
PAY_AMT4         2.851870       2.499687      3.000434
PAY_AMT5         3.018284       0.000000      3.352375
PAY_AMT6         0.000000       2.260071      0.000000
FlagDefault      0.000000       0.000000      1.000000

<class 'pandas.core.frame.DataFrame'>
RangeIndex: 30000 entries, 0 to 29999
Data columns (total 24 columns):
 #   Column       Non-Null Count  Dtype
---  ------       --------------  -----
 0   LIMIT_BAL    30000 non-null  float64
 1   SEX          30000 non-null  float64
 2   EDUCATION    30000 non-null  float64
 3   MARRIAGE     30000 non-null  float64
 4   AGE          30000 non-null  float64
 5   PAY_1        30000 non-null  float64
 6   PAY_2        30000 non-null  float64
 7   PAY_3        30000 non-null  float64
 8   PAY_4        30000 non-null  float64
 9   PAY_5        30000 non-null  float64
 10  PAY_6        30000 non-null  float64
 11  BILL_AMT1    30000 non-null  float64
 12  BILL_AMT2    30000 non-null  float64
 13  BILL_AMT3    30000 non-null  float64
 14  BILL_AMT4    30000 non-null  float64
 15  BILL_AMT5    30000 non-null  float64
 16  BILL_AMT6    30000 non-null  float64
 17  PAY_AMT1     30000 non-null  float64
 18  PAY_AMT2     30000 non-null  float64
 19  PAY_AMT3     30000 non-null  float64
 20  PAY_AMT4     30000 non-null  float64
 21  PAY_AMT5     30000 non-null  float64
 22  PAY_AMT6     30000 non-null  float64
 23  FlagDefault  30000 non-null  float64
dtypes: float64(24)
memory usage: 5.5 MB
```

In above, you can see the basic statistics of the data and the data are all
numeric. A few samples are displayed as well.

For the great expectation part, it works as follows: There is a great
expectation object that holds the data. It remembers at least the data schema.
In below, we create one using a pandas DataFrame. Hence it holds all data with
it as well. This GE object behaves like a pandas DataFrame, indeed, and with
some additional GE-specific methods are added:

```python
#!/usr/bin/env python
# coding: utf-8

import json
import pandas as pd
import great_expectations as ge

# Load data as pandas DataFrame
df_credit = pd.read_csv("data/TaiwanCredit.csv", low_memory=False)

# Load into Great Expectation
df_ge = ge.from_pandas(df_credit)

# These are the list of functions added by great expectations
print([x for x in dir(df_ge) if not x.startswith("_") and x not in dir(df_credit)])
```

They are:

```
['add_citation',
 'attempt_allowing_relative_error',
 'batch_id',
 'batch_kwargs',
 'batch_markers',
 'batch_parameters',
 'caching',
 'column_aggregate_expectation',
 'column_map_expectation',
 'column_pair_map_expectation',
 'default_expectation_args',
 'discard_failing_expectations',
 'discard_subset_failing_expectations',
 'expect_column_bootstrapped_ks_test_p_value_to_be_greater_than',
 'expect_column_chisquare_test_p_value_to_be_greater_than',
 'expect_column_distinct_values_to_be_in_set',
 'expect_column_distinct_values_to_contain_set',
 'expect_column_distinct_values_to_equal_set',
 'expect_column_kl_divergence_to_be_less_than',
 'expect_column_max_to_be_between',
 'expect_column_mean_to_be_between',
 'expect_column_median_to_be_between',
 'expect_column_min_to_be_between',
 'expect_column_most_common_value_to_be_in_set',
 'expect_column_pair_cramers_phi_value_to_be_less_than',
 'expect_column_pair_values_A_to_be_greater_than_B',
 'expect_column_pair_values_to_be_equal',
 'expect_column_pair_values_to_be_in_set',
 'expect_column_parameterized_distribution_ks_test_p_value_to_be_greater_than',
 'expect_column_proportion_of_unique_values_to_be_between',
 'expect_column_quantile_values_to_be_between',
 'expect_column_stdev_to_be_between',
 'expect_column_sum_to_be_between',
 'expect_column_to_exist',
 'expect_column_unique_value_count_to_be_between',
 'expect_column_value_lengths_to_be_between',
 'expect_column_value_lengths_to_equal',
 'expect_column_values_to_be_between',
 'expect_column_values_to_be_dateutil_parseable',
 'expect_column_values_to_be_decreasing',
 'expect_column_values_to_be_in_set',
 'expect_column_values_to_be_in_type_list',
 'expect_column_values_to_be_increasing',
 'expect_column_values_to_be_json_parseable',
 'expect_column_values_to_be_null',
 'expect_column_values_to_be_of_type',
 'expect_column_values_to_be_unique',
 'expect_column_values_to_match_json_schema',
 'expect_column_values_to_match_regex',
 'expect_column_values_to_match_regex_list',
 'expect_column_values_to_match_strftime_format',
 'expect_column_values_to_not_be_in_set',
 'expect_column_values_to_not_be_null',
 'expect_column_values_to_not_match_regex',
 'expect_column_values_to_not_match_regex_list',
 'expect_compound_columns_to_be_unique',
 'expect_multicolumn_sum_to_equal',
 'expect_multicolumn_values_to_be_unique',
 'expect_select_column_values_to_be_unique_within_record',
 'expect_table_column_count_to_be_between',
 'expect_table_column_count_to_equal',
 'expect_table_columns_to_match_ordered_list',
 'expect_table_columns_to_match_set',
 'expect_table_row_count_to_be_between',
 'expect_table_row_count_to_equal',
 'expectation',
 'expectation_suite_name',
 'from_dataset',
 'get_binned_values',
 'get_column_count',
 'get_column_count_in_range',
 'get_column_hist',
 'get_column_max',
 'get_column_mean',
 'get_column_median',
 'get_column_min',
 'get_column_modes',
 'get_column_nonnull_count',
 'get_column_partition',
 'get_column_quantiles',
 'get_column_stdev',
 'get_column_sum',
 'get_column_unique_count',
 'get_column_value_counts',
 'get_config_value',
 'get_crosstab',
 'get_default_expectation_arguments',
 'get_evaluation_parameter',
 'get_expectation_suite',
 'get_row_count',
 'get_table_columns',
 'hashable_getters',
 'list_available_expectation_types',
 'multicolumn_map_expectation',
 'profile',
 'save_expectation_suite',
 'set_config_value',
 'set_default_expectation_argument',
 'set_evaluation_parameter',
 'test_column_aggregate_expectation_function',
 'test_column_map_expectation_function',
 'test_expectation_function',
 'validate']
```

So a lot of methods prefixed with "expect".

GE works by creating rules in the object. Creating a rule doesn't perform the
validation immediately, but the GE object will remember the rule. Examples:

```python
...
# unique values in this column
df_ge.expect_column_values_to_be_unique(column="EDUCATION")

# a column should be within a set of values
df_ge.expect_column_values_to_be_in_set(
    column = "FlagDefault", 
    value_set = [1.0, 0.0]
)
```

Instead of creating rules in code one by one, we can also ask GE to *profile*
the sample data and generate the rules by itself. The output will be
*extremely verbose* and a lot of junk rules will be created. But it will be a
good start:

```python
from great_expectations.profile.user_configurable_profiler import UserConfigurableProfiler

...

# Generate rules automatically by learning the dataset, i.e., generate a "suite"
profiler = UserConfigurableProfiler(df_ge)
# may take long time to profile
# also see: https://legacy.docs.greatexpectations.io/en/latest/autoapi/great_expectations/profile/index.html#great_expectations.profile.BasicDatasetProfiler._profile
#    suite, validation_result = BasicSuiteBuilderProfiler().profile(
#        dataset, {
#           “columns”: [“id”, “username”, “address”],
#           “excluded_expectations”: [“expect_column_mean_to_be_between”, “expect_column_median_to_be_between”, “expect_column_quantile_values_to_be_between”,],
#           “included_expectations”: [“expect_column_to_not_be_null”, “expect_column_values_to_be_in_set”,],
#        }
#    )
suite = profiler.build_suite()
# save all the learned rules in JSON format
with open("data/TaiwanCredit.suite.json", "w") as fp:
    fp.write(json.dumps(suite.to_json_dict(), indent=4, sort_keys=True))
```

This will save all the rules (206 of them in this case) into a JSON file. A
rule is in the format like the following:

```
{
  "expectation_type": "expect_column_proportion_of_unique_values_to_be_between",
  "kwargs": {
    "column": "BILL_AMT3",
    "min_value": 0.7342,
    "max_value": 0.7342
  },
  "meta": {}
}
```

The object `suite` is a `ExpectationSuite` object. It is detached from the
dataframe. You can see it has the following methods:

```
['add_citation',
 'add_expectation',
 'add_expectation_configurations',
 'append_expectation',
 'data_asset_type',
 'evaluation_parameters',
 'exclude_field_names',
 'expectation_suite_name',
 'expectations',
 'find_expectation_indexes',
 'find_expectations',
 'ge_cloud_id',
 'get',
 'get_citations',
 'get_column_expectations',
 'get_column_pair_expectations',
 'get_evaluation_parameter_dependencies',
 'get_grouped_and_ordered_expectations_by_column',
 'get_grouped_and_ordered_expectations_by_domain_type',
 'get_grouped_and_ordered_expectations_by_expectation_type',
 'get_multicolumn_expectations',
 'get_table_expectations',
 'include_field_names',
 'isEquivalentTo',
 'items',
 'keys',
 'meta',
 'name',
 'patch_expectation',
 'property_names',
 'remove_all_expectations_of_type',
 'remove_expectation',
 'render',
 'replace_expectation',
 'send_usage_event',
 'show_expectations_by_domain_type',
 'show_expectations_by_expectation_type',
 'to_dict',
 'to_json_dict',
 'to_raw_dict',
 'values']
```

You can get these with the following print statements (together with the rules count):

```python
print(suite.expectations[123])
print([x for x in dir(suite) if not x.startswith("_")])
print(len(suite.expectations))
print(len(suite.get_column_expectations()))
```

The idea of writing the rules into a JSON file is that you can edit it. Assume
you edited the rules, trimmed down some, and save it to another JSON file of
the same format, you can at anytime load the rules into a *schema* and create a
new suite. The suite has no data associated with it. But you can use it to
*validate* a data. This time, most likely not the same as you profiled:

```python
# Load expectation rules into a suite
with open("data/TaiwanCredit.suite2.json", "r") as fp:
    schema = json.loads(fp.read())

new_suite = ge.core.expectation_suite.ExpectationSuite(
    "Edited Rules",
    expectations=schema["expectations"])

# Validate data using the new rules
df_ge.validate(new_suite)
```
