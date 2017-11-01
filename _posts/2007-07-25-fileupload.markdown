---
layout: post
title: "File Upload in AJAX"
date: 2007-07-25 06:07:23 +0800
tags: code
---

AJAX cannot do file upload, it is because due to the security concern, JavaScript is not supposed to be able to read from the user's hard disk: but the browser can.

However, it is not always true if the browser allows an extended JavaScript. Example is the Mozilla browser. We can turn on the file access support in JavaScript in Mozilla by changing the `about:config` entry `signed.applets.codebase_principal_support` to `true`.

Details is in <http://www.captain.at/ajax-file-upload.php> and there is code for that, too.

## iframe

There is an alternative way of JavaScript-initiated file upload. It is make use of an `iframe` object, which works like an embedded browser window and we can make it as the target for a file-uploading HTML form. When such form is being submitted, because its target is the iframe but not the old browser canvas, the upload job is done asynchronously in the background --- just like AJAX.

However, the submit() function call does not tell when will it be finished. Hence the simplest way to check is by polling.

Another way of doing, which I preferred, is to use the `onLoad` event of `iframe` object. However, this `onLoad` event must be enabled right before it is used. Below is an example:

HTML part (excerpt):
```html
<form id="uploadform">
File: <input type="file" name="file"/>
<input type="button" value="OK" id="submit" onclick="startUpload()" />
</form>
<iframe id="outputiframe" name="outputiframe"></iframe>
```

JavaScript part (some Prototype.js is used):
```javascript
// The ping part
function startUpload()
{
        $('outputiframe').onload = uploadResponse;
        $('uploadform').setAttribute("target","outputiframe");
        $('uploadform').setAttribute("action","upload.php");
        $('uploadform').setAttribute("method","post");
        $('uploadform').setAttribute("enctype","multipart/form-data");
        $('uploadform').submit();
}
// The pong part
function uploadResponse()
{
function submitAssgResponse()
{
        var d = Try.these(
            function() { return $('outputiframe').contentDocument; },
            function() { return $('outputiframe').contentWindow.document; },
            function() { return $('outputiframe').document; }
        );
        var responseText = d.body.innerHTML;
        // Do real things below
        doSomething(responseText);
}
```

PHP part:
```php
<?php
header("Cache-Control: no-cache, must-revalidate");  // for HTTP/1.1
header("Pragma: no-cache");                          // for HTTP/1.0

echo "<html><body>";       // required to work!

move_uploaded_file($_FILES['file']['tmp_name'],"path/to/".basename($_FILES['file']['name']));
echo Some_XML_or_JSON;

echo "</body></html>";     // required to work!
```
