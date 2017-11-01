---
layout: post
title: "DokuWiki plug-in: Multi-line lists"
date: 2007-06-18 22:37:54 +0800
tags: code
---

DokuWiki supports lists using the following syntax:

```markdown
  * item1
  * item2
     * subitem 2.1
  * item 3
```

However, when you have a very long list item, you will found a nightmare! In the
editing window, the items will wrap and you lost all the visual hierarchy.
Although you can still make the list as you want to but it is simply not as
*intuitive* as it should be.

I made a plug-in, by referencing the creole plug-in, to do the above things,
which now supports the syntax as follows:

```markdown
  * item1
    and its continuation
  * item 2
     * subitem 2.1
       and its continuation
```

Those "continuation" will be shown as one single line, but you can make it
visually better when editing the source. All continuation needs at least three
spaces from the begin of the line and the actually number of spaces are not
important to the outcome. The number of continuation lines is unlimited given
that all lines begins with at least three spaces.

## Code

To use, create a directory `lib/plugins/mllist/` and put the [following
file](/img/mllist.txt) as `syntax.php` to use.

```php
<?php
/**
 * Multiline List Plugin
 * 
 * @license    GPL 2 (http://www.gnu.org/licenses/gpl.html)
 * @author     Adrian Sai-wah Tam <adrian.sw.tam@gmail.com>
 */
 
// must be run within Dokuwiki
if(!defined('DOKU_INC')) die();

if(!defined('DOKU_PLUGIN')) define('DOKU_PLUGIN',DOKU_INC.'lib/plugins/');
require_once(DOKU_PLUGIN.'syntax.php');
 
/**
 * All DokuWiki plugins to extend the parser/rendering mechanism
 * need to inherit from this class
 */
class syntax_plugin_mllist extends DokuWiki_Syntax_Plugin {
 
  function getInfo(){
    return array(
      'author' => 'Adrian Sai-wah Tam',
      'email'  => 'adrian.sw.tam@gmail.com',
      'date'   => '2007-06-06',
      'name'   => 'Multiline list plugin',
      'desc'   => 'Allows a list item to break into multiple lines with indentation on non-bullet lines',
      'url'    => 'http://aipl.ie.cuhk.edu.hk/~adrian/doku.php/software/mllist'
    );
  }

  function getType(){ return 'container'; }
  function getPType(){ return 'block'; }
  function getSort(){ return 9; }
  
  function getAllowedTypes(){
    return array('formatting', 'substition', 'disabled', 'protected');
  }
  
  function connectTo($mode){
    $this->Lexer->addEntryPattern('\n {2,}[\-\*]',$mode,'plugin_mllist');
    $this->Lexer->addEntryPattern('\n\t{1,}[\-\*]',$mode,'plugin_mllist');
    $this->Lexer->addPattern('\n {2,}[\-\*]','plugin_mllist');
    $this->Lexer->addPattern('\n\t{1,}[\-\*]','plugin_mllist');
    // Continuation lines need at least three spaces for indentation
    $this->Lexer->addPattern('\n {2,}(?=\s)','plugin_mllist');
    $this->Lexer->addPattern('\n\t{1,}(?=\s)','plugin_mllist');
  }
  
  function postConnect(){
    $this->Lexer->addExitPattern('\n','plugin_mllist');
  }
  
  function handle($match, $state, $pos, &$handler){
    switch ($state){
      case DOKU_LEXER_ENTER:
        $ReWriter = & new Doku_Handler_List($handler->CallWriter);
        $handler->CallWriter = & $ReWriter;
        $handler->_addCall('list_open', array($match), $pos);
        break;
      case DOKU_LEXER_EXIT:
        $handler->_addCall('list_close', array(), $pos);
        $handler->CallWriter->process();
        $ReWriter = & $handler->CallWriter;
        $handler->CallWriter = & $ReWriter->CallWriter;
        break;
      case DOKU_LEXER_MATCHED:
        if (preg_match("/^\s+$/",$match)) break;
            // Captures the continuation case
        $handler->_addCall('list_item', array($match), $pos);
        break;
      case DOKU_LEXER_UNMATCHED:
        $handler->_addCall('cdata', array($match), $pos);
        break;
    }
    return true;
  }
  
  function render($mode, &$renderer, $data){
    return true;
  }
}
 
//Setup VIM: ex: et ts=4 enc=utf-8 :
```

## Change History

* 2007-06-10:
  * Initial release.

