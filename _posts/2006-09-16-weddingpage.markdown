---
layout: post
title: "How I created my wedding page"
date: 2006-09-16 10:09:02 +0800
tags: code
---

It is just a small page, but used some AJAX techniques and libraries to develop. The structure is as follows.

## moo.fx and accordion
From a higher-level point of view, it is three pages combined into one. The way
I combined them is to use [moo.fx](http://moofx.mad4milk.net) --- it is
something like simplified version of scriptaculous. But what I needed is the
*Accordion* effect. I created an accordion using three `menuitem` div's and
three `box` div's. This is my JavaScript

```javascript
function init(){
        var stretchers = document.getElementsByClassName('box');
        var toggles = document.getElementsByClassName('menuItem');
        var myAccordion = new fx.Accordion(
                toggles, stretchers, {'opacity':false, 'height':true, 'duration':600}
        );
        //hash functions
        var found = false;
        toggles.each(function(h3, i){
                var div = Element.find(h3, 'nextSibling');
                if (window.location.href.indexOf(h3.title) > 0) {
                        myAccordion.showThisHideOpen(div);
                        found = true;
                }
        });
        if (!found) myAccordion.showThisHideOpen(stretchers[0]);
};

init();
```
and this is my HTML
```html
<div id="menu">
    <div id="menuSep">|</div><div class="menuItem"><a href="#">gallery</a></div>
    <div id="menuSep">|</div><div class="menuItem"><a href="#">info</a></div>
    <div id="menuSep">|</div><div class="menuItem"><a href="#">guest book</a></div>
    <div id="menuSep">|</div>
</div>
<div id="content">
    <div class="box">
    ...
    </div>
    <div class="box">
    ...
    </div>
    <div class="box">
    ...
    </div>
</div>
```
The anchor tag around `menuItem` is required because of IE. Otherwise IE users cannot catch the onClick event and the Accordion would not work. The effect of Accordion is that, the n-th `menuItem` will correspond to the n-th `box`. If a `menuItem` is clicked, only the corresponding `box` will be shown and every other `box`es would be closed (hidden). The flipping of `box` is an animation (page scrolling up or down), which is provided by the moo.fx package.

## Slide show
The slide show is done by yet another package, called [mooShow](http://www.mooshow.eatpixels.com/). It is based on moo.fx and doing a picture slideshow. However, I modified it moderately to add some more effects, namely:
  * pictures are automatically changed periodically
  * when pictures are changed manually or automatically, it is done by fade-out, then resize the frame, then fade-in

The first modification is to call the `nextImage()` function periodically, when the `autoscroll` option is set to `yes`. The key function is this one:
```javascript
function autoScroll(showName,period) {
    // Auto-scrolling function
    if (mooShows[showName].timeoutID) {
        mooShows[showName].nextImage();
    };
    mooShows[showName].timeoutID = setTimeout("autoScroll('"+showName+"',"+period+")", period);
}
```
The above function is to set a timer to call the `nextImage()` function every `period` milliseconds. Once it is called for the first time, it will change the picture periodically. But how can I call it the first time? Add some code! Firstly, I added the `scroll()` function in to mooshow object as follows:
```javascript
scroll: function() {
    if (this.autoscroll == 'yes') {
        autoScroll(this.id,this.autoscrollperiod);
    };
},
```
then I changed `create_mooshow()` function and add the following line at the very end:
  mooShows[showName].scroll();
and it is all set.

The way mooShow get its options is to call `eval(xxx.innerHTML)`, so that the options can be put inside the HTML area, this is a neat and fabulous technique.

The second modification, the fade-out effect is done by extending the mooshow class. I first rename the `loadImage()` function to `loadRealImage()` function and use the following `loadImage()` function:
```javascript
loadImage: function() {
    // Fade out the current image, then call loadRealImage
    showName = this.id;     // get showname

    // Fade out the current one
    this.fader = new fx.Opacity(showName+'_image', {duration: mooShows[showName].fadeSpeed, onComplete:function() {
        mooShows[showName].loadRealImage();
    }});
    this.fader.toggle();
},
```
I don't directly modify the `loadImage()` function is because I need to wait for the fade-out finish before loading the image and do a fade-in. Otherwise, you will never see the fade-out effect because the resizing of image frame would be done immediately because of the asynchronous dynamics of moo.fx. Hence the way I do this is to keep the `loadImage()` function intact but renamed it, and after I finished the fading out, I start to do what shall be done originally.

## Guest book
The final part of the web page is the guest book. It is something done in a very ad-hoc manner. Let's look at the code of `guestbook.js` first:
```javascript
var gStart;     // integer

function gSubmit() {
        // Collect the fields in table "input" and submit and call gInit();
        var params = { name:$F("name"), email:$F("email"), msg:$F("text") };
        var qryStr = $H(params).toQueryString();
        var ajaxObj = new Ajax.Request("/~adrian/wedding/gSave.php", {method: 'post', parameters: qryStr, onComplete: gInit });
};

function gScrollUp() {
        gStart--;
        gLoad();
};

function gScrollDown() {
        gStart++;
        gLoad();
};

function gLoad() {
        // First, rectify the value of gStart
        var gMessages = document.getElementsByClassName("message" , $("messages"));
        if (gStart < 0) {
                gStart = 0;     // Show at most from 0th message
        };
        if (gStart > gMessages.length - 1 ) {
                gStart = gMessages.length - 1;  // Show at least one message
        };
        // Hide some of the messages, but show some
        for(i=0;i<gMessages.length;i++){
                if (i<gStart) {
                        (gMessages[i]).style.display = "none";
                } else {
                        (gMessages[i]).style.display = "block";
                };
        };
};

function gInit() {
        // Load the messages from the server
        var ajaxObj = new Ajax.Updater("gMessages", "/~adrian/wedding/gLoad.php", {method:'get'});
        // And adjust the display
        gStart = 0;
};
```
and the HTML part is as follows:
```html
<div id="guestbook">
    <!-- Input box -->
    <div id="signTips" onclick="inputbox.toggle();"><a href="#">Sign our guest book!</a></div>
    <div id="input">
    <table>
        <tr><th><label for="name">Name</label></th><td><input type="input" id="name"/></td></tr>
        <tr><th><label for="email">Email</label></th><td><input type="input" id="email"/> (for us to keep contact with you)</td></tr>
        <tr><th><label for="message">Message</label></th><td><textarea id="text" rows="7"></textarea></td></tr>
        <tr id="submit"><th>&nbsp;</th><td><a href="#" class="button" onClick="gSubmit(); inputbox.hide();">Submit</a></td></tr>
    </table>
    </div>
    <!-- Message list -->
    <div id="messagearea">
        <div id="up" onClick="gScrollUp();"></div>
        <div id="down" onClick="gScrollDown();"></div>
        <div id="gMessages"></div>
    </div>
</div>
```

The `signTips` div is to allow people to hide or show the input form (the table). It is, again, using moo.fx to do the hide/show effect. The code is very brief:
```javascript
var inputbox;
addLoadEvent(function() {
        inputbox = new fx.Combo('input', {height: true, opacity: true, duration: 500});
        inputbox.hide();
});
```

The table is to let people to input the data. When you click on the submit anchor, the form would be submitted through AJAX. The `gMessages` div is to hold the messages. From the JavaScript, we can see that the submit function is just to collect all the fields and make it up as a query string format and then submit it using an AJAX object. Finally, reload the messages.

The messages are reloaded by using the `Ajax.Updater`, which the server-side PHP will construct the HTML for us. The interesting part is that, I don't want my page to be messed up by the scroll bar the the right. Hence I want to use the CSS to prevent scrolling. I created both an `up` and a `down` button for people to scroll my messages. It is done by manipulating the `display` CSS options of individual messages.

My guest book needs some PHP backend. I used a directory to hold all guestbook messages (as plain text files). The format is newline-delimited, where the first to fourth lines are respectively the guest's name, email, timestamp, IP address. The rest is the message text. The PHP script I used to retrieve all the messages in reversed chronological order is this one:
```php
<?php
header ("Cache-Control: no-cache, must-revalidate");  // for HTTP/1.1
header ("Pragma: no-cache");                          // for HTTP/1.0

$filenames = array();
$dir = 'guestbook';

// Get all files
if ($handle = opendir($dir)) {
        while (($file = readdir($handle)) !== false) {
                if (is_file("$dir/$file") !== true) {
                        continue;
                };
                // We found a file, put it into filenames list
                $filenames[] = "$dir/$file";
        };
        closedir($handle);
};

// Sort in reverse chronological order, then output
rsort($filenames);
foreach($filenames as $filename) {
        if (($fp = @fopen($filename, "r")) !== false) {
                // Read the file into arrays
                $buffer = array();
                $i=0;
                while (!feof($fp)) {
                        if ($i>4) {
                                $buffer[4] .= "<br/>".trim(fgets($fp));
                        } else {
                                $buffer[$i] = trim(fgets($fp));
                        };
                        $i++;
                };
                fclose($fp);

                // and print an HTML
                echo ""."<div class=\"message\">\n".
                        "<div class=\"name\">".$buffer[0]."</div>\n".
                        "<div class=\"date\">".$buffer[2]."</div>\n".
                        "<div class=\"text\">".$buffer[4]."</div>\n".
                        "</div>\n";
        }
}
?>
```
and the script to save a new message is the following:
```php
<?php
$allowedTags='<a><br><b><i><img><li><ol><p><strong><u><ul>';
$dir = 'guestbook';

$name = strip_tags(stripslashes($_POST['name']),$allowedTags);
$email = strip_tags(stripslashes($_POST['email']),$allowedTags);
$filename = date('U');
$date = date('r');
$ip = $_SERVER['REMOTE_ADDR'];
$message = strip_tags(stripslashes($_POST['msg']),$allowedTags);

// Write to file
if (($fp = @fopen("$dir/$filename.txt", "w")) !== false) {
        fwrite($fp,"$name\n$email\n$date\n$ip\n$message");
        fclose($fp);
}
?>
```

## CSS
Microsoft's browser sucks!

In Mozilla/Firefox, it is really standards-compliant and I finish my CSS setting correctly and nicely. However, IE cannot correctly show the effect I want. Hence I have to do some browser-detection. The way I did this is to use a PHP script to detect which and which and then return the right CSS text. The code is as follows:
```php
<?php
header( "Content-type: text/css" );

$fp = null;
if (strpos($_SERVER['HTTP_USER_AGENT'],'MSIE')===false) {
        // Mozilla
        $fp = fopen("wedding.moz.css", "rb");
} else {
        // Microsuck
        $fp = fopen("wedding.ie.css", "rb");
};

// Output
while (!feof($fp)) {
        echo fread($fp, 8192);
};
fclose($fp);
?>
```

