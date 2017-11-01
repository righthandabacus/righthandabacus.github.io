<?php
/**
 * Content Plugin: displays a number of recent entries from either the
 * changelog or the filesystem
 * modified from the blog plugin by Esther Brunner and bin/wantedpages.php
 *
 * @license    GPL 2 (http://www.gnu.org/licenses/gpl.html)
 * @author     Adrian Sai-wah Tam <adrian [at] ieaa [dot] org>
 */

if(!defined('DOKU_INC')) define('DOKU_INC',realpath(dirname(__FILE__).'/../../').'/');
if(!defined('DOKU_PLUGIN')) define('DOKU_PLUGIN',DOKU_INC.'lib/plugins/');
require_once(DOKU_PLUGIN.'syntax.php');

define ('DW_DIR_CONTINUE',1);
define ('DW_DIR_NS',2);
define ('DW_DIR_PAGE',3);

// Reverse sort the pages by date (recents first)
function _content_cmp_pages($a,$b) {
    if ($a[date] == $b[date]) {
        return 0;
    }
    return ($a[date] < $b[date]) ? 1 : -1;
}

/**
 * All DokuWiki plugins to extend the parser/rendering mechanism
 * need to inherit from this class
 */
class syntax_plugin_content extends DokuWiki_Syntax_Plugin {
    /**
     * return some info
     */
    function getInfo(){
        return array(
            'author' => 'Adrian Sai-wha Tam',
            'email'  => 'adrian@ieaa.org',
            'date'   => '2005-09-27',
            'name'   => 'Content Plugin',
            'desc'   => 'Displays a number of recent entries from a given namespace, from filesystem or changelog',
            'url'    => 'http://aipl.ie.cuhk.edu.hk/~adrian/doku.php/project:dokuwiki:content',
        );
    }

    function getType(){ return 'substition'; }
    function getPType(){ return 'block'; }
    function getSort(){ return 307; }
    function connectTo($mode) { $this->Lexer->addSpecialPattern('\{\{content>.+?\}\}',$mode,'plugin_content'); }

    /**
     * Handle the match, syntax: content>[namespace][?num[&options[&options..]]]
     */
    function handle($match, $state, $pos, &$handler){
        global $conf;
        $match = substr($match,10,-2); // strip {{content> from start and }} from end
        list($ns,$options) = explode('?',$match,2);
        // Processing the options
        $options = explode('&',$options);
        $showpage = $conf['content']['showpage'];
        $scan = $conf['content']['scanmode'];
        $recursive = (in_array("recursive",$options))?true:false;
        $showpage = (in_array("showpage",$options))?true:$showpage;
        $showpage = (in_array("noshowpage",$options))?false:$showpage;
        $scan = (in_array("filesystem",$options))?true:$scan;
        $scan = (in_array("changelog",$options))?false:$scan;
        $num = $options[0];     // Suppose the first option is the number
        if (!is_integer($num)){
            if (is_integer($ns)){
                $num = $ns;
                $ns  = '';
            } else {
                $num = 5;
            }
        }
        return array($ns,$num,$recursive,$showpage,$scan);
    }

    /**
     * Create output
     */
    function render($mode, &$renderer, $data) {
        global $conf;
        global $ID;
        global $filechain;
        global $conf;

        if($mode == 'xhtml'){
            if (!isset($filechain)) $filechain[] = $ID;

            // list here pages which should not be included in the blog
            $exclude = array('start','sidebar');
            $exclude = array_merge($filechain,$exclude);

            $comments   = 'Comments'; // @todo: localize!

            // prevent caching to ensure the included page is always fresh
            // $renderer->info['cache'] = FALSE; // without caching it's to slow

            $num        = $data[1];
            $recents = ($data[4])?  // Scan filesystem or changelog?
                          $this->_get_pages($data[0],$data[2]) :
                          getRecents(0,$num,false,$data[0],$data[2]);
            if ($num > count($recents)) $num = count($recents);

            if (! $data[3]) {    // If showpage is not set, the pages are expressed as lists
                $renderer->doc .= '<ul>';
            };

            foreach($recents as $recent){
                $id = $recent['id'];
                if (in_array($id, $exclude)) continue;
                $filechain[] = $id;

                $date     = date($conf['dformat'],$recent['date']);
                if ($recent['user']){       // Available only if we obtained the list from changelog
                    $userInfo = auth_getUserData($recent['user']);
                    $nametag  = $userInfo['name'];
                } else {                    // If it is from filesystem scan, we should know the filename
                    $nametag  = $recent['file'];
                }
                $content  = p_cached_xhtml(wikiFN($id));
                if ($data[3]) {    // showpage ?
                    $patterns = array('!<div class="toc">.*?(</div>\n</div>)!s', // remove toc
                                      '!<h1>(.*?)</h1>!s',                       // link h1 (permalink)
                                      '#<!-- SECTION \[(\d*-\d*)\] -->#e',       // remove section edit buttons
                                      '!<div class="category">.*?</div>!s');     // remove category tags
                    $replace  = array('',
                                      '<h1>'.$renderer->internallink($id,"\\1",'',true).'</h1>',
                                      '',
                                      '');
                    $content  = preg_replace($patterns,$replace,$content);
                    $parts = preg_split('/<hr.*>/i', $content);
                    if (count($parts)>1) $content = $parts[0].'</div>';
                    $renderer->doc .= '<div class="include">';
                    $renderer->doc .= $content;
                    $renderer->doc .= '</div>';
                    $renderer->doc .= '<div class="meta">' . $nametag . ' &middot; ' . $date;
                    $dFN = wikiFN($this->_addDiscussionNS($id));
                    if ($dFN && @file_exists($dFN) && $data[3]){
                        $text = io_readFile($dFN);
                        $noc  = count(explode('----',$text)) - 1;
                        $renderer->doc .= ' &middot; '.
                            $renderer->internallink($id.'#discussion',$comments.' ('.$noc.')','',true);
                    }
                    $renderer->doc .= '</div><br /><br />';
                } else {    // no showpage
                    if (preg_match('!<h1>.*</h1>!',$content) == 0) {
                        $content = $renderer->internallink($id,$id,"",true);
                    } else {
                        $content  = preg_replace('!.*?<h1>(.*?)</h1>.*!s',
                                                 $renderer->internallink($id,"\\1",'',true),
                                                 $content);
                    };
                    $renderer->doc .= "<li>$content<br/>&nbsp;&nbsp;&nbsp;&nbsp;".$recent['file']." &middot $date</li>";
                };
            }
            if (! $data[3]) {    // If showpage is not set, the pages are expressed as lists
                $renderer->doc .= '</ul>';
            };
            return true;
        }
        return false;
    }

    /**
     * Convert a normal page ID to a discussion page ID.
     *
     * @author Dave Lawson <dlawson@masterytech.com>
     * @author Esther Brunner <esther@kaffeehaus.ch>
     */
    function _addDiscussionNS($id) {
        global $conf;
        $dNS = $conf['discussion']['namespace'];
        if (!$dNS) return false;
        if ($this->_isDiscussionID($ID)) return $id;
        $dID = getNS($id);
        return $dID.($dID ? ':' : '').$dNS.':'.noNS($id);;
    }

    /**
     * Is specified page ID in a discussion namespace?
     *
     * @author Dave Lawson <dlawson@masterytech.com>
     */
    function _isDiscussionID($id) {
        global $conf;
        $dNS = $conf['discussion']['namespace'];
        $pID = noNS($id);
        return strpos($id, $dNS.':'.$pID) !== false;
    }

    // Check for the type of directory entry
    // copied from bin/wantedpages.php
    function _dir_filter($entry, $basepath) {
        if ($entry == '.' || $entry == '..' ) {
            return DW_DIR_CONTINUE;
        }
        if ( is_dir($basepath . '/' . $entry) ) {
            if ( strpos($entry, '_') === 0 ) {
                return DW_DIR_CONTINUE;
            }
            return DW_DIR_NS;
        }
        if ( preg_match('/\.txt$/',$entry) ) {
            return DW_DIR_PAGE;
        }
        return DW_DIR_CONTINUE;
    }

    // Get a list of pages, without sorting
    // copied from bin/wantedpages.php with modification
    function _get_pages($dir,$recursive) {
        global $conf;
        static $trunclen = NULL;
        $fullpath = $conf['datadir'].'/'.$dir;
        if ( !$trunclen ) {
            global $conf;
            $trunclen = strlen($conf['datadir'].':');
        };
        if ( !is_dir($fullpath) ) {
            echo "Unable to read directory $dir\n";
            exit(1);
        };
        $pages = array();
        $dh = opendir($fullpath);
        while ( FALSE !== ( $entry = readdir($dh) ) ) {
            $status = $this->_dir_filter($entry, $fullpath);
            if ( $status == DW_DIR_CONTINUE ) {
                continue;
            } else if ( $status == DW_DIR_NS ) {
                if ($recursive) {
                  $pages = array_merge($pages, $this->_get_pages($dir . '/' . $entry,$recursive));
                };
            } else {
                $page = array(
                    'id'=>$this->pathID($dir . '/' . $entry),
                    'file'=>$dir . '/' . $entry,
                    'date'=>filemtime($fullpath . '/' . $entry),
                    );
                $pages[] = $page;
            }
        }
        closedir($dh);
        usort($pages,"_content_cmp_pages");
        return $pages;
    }

    // copied from inc/search.php
    function pathID($path,$keeptxt=false){
        $id = utf8_decodeFN($path);
        $id = str_replace('/',':',$id);
        if(!$keeptxt) $id = preg_replace('#\.txt$#','',$id);
        $id = preg_replace('#^:+#','',$id);
        $id = preg_replace('#:+$#','',$id);
        return $id;
    }
}

//Setup VIM: ex: et ts=4 enc=utf-8 :
