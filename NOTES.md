notes on learning Jekyll blogging
=================================

Jekyll : static site generator

GitHub pages : web server. Support regular HTML content, and Jekyll

Post is a superset of a page in many ways. It's just that categories, tags, and
time-related data is post-specific, therefore we don't process all pages.
Instead, we process just posts when considering categories, tags, and dates.


misc functionalities for Jekyll
-------------------------------

- Default theme of Jekyll: minima
    - Location: /Library/Ruby/Gems/2.0.0/gems/minima-1.2.0/
- CDN for loading Javascripts: https://cdnjs.com/libraries
- Jekyll output should be full of unnecessary spaces due to liquid template
    - Compress HTML output by liquid: https://github.com/penibelst/jekyll-compress-html
    - A 159-line solution to leverage the string manipulation function in liquid
    - To include in the base layout file
- Tags and categories
    - single page for tags list or categories list to hold everything is possible
    - separated pages for each tag/category will need plugin (which is not
      supported by GitHub pages) or manual intervention
    - Manual method:
        - minddust
        - http://blog.lanyonm.org/articles/2013/11/21/alphabetize-jekyll-page-tags-pure-liquid.html
        - https://christianspecht.de/2014/10/25/separate-pages-per-tag-category-with-jekyll-without-plugins/, for latter
    - Other reference implementation:
        - http://codinfox.github.io/dev/2015/03/06/use-tags-and-categories-in-your-jekyll-based-github-pages/ (single page)
        - https://blog.meinside.pe.kr/Adding-tag-cloud-and-archives-page-to-Jekyll/
- Tag icon before tag:

  ``` html
  <a href="/blogs/tags/#my_tag" class="post-tag">my_tag</a>
  ```

  ``` css
  .post-tag:before {
     content: "\f02b";
     font-family: FontAwesome;
     padding-right: .5em;
  }
  ```
- Jekyll process conversion before liquify, thus this needed in case quoting
  liquid-parsable code:

  {% raw %}
  ```c
  int func() {{ return }}
  ```
  {% endraw %}
- Post vs page
    - http://stackoverflow.com/questions/15095625/what-is-the-differences-between-post-and-page
    - Post must be in `_post/`, page can be anywhere
    - Post expects date and title in file name
    - Post has title, url, date, id, categories, next, previous, tags, content
      attributes
    - Post collection is `site.posts`, and url will be generated using permalink
      configured in config yaml
    - Pages will be relative to root directory as url
    - Use of categories in pages: http://stackoverflow.com/questions/17118551/generating-a-list-of-pages-not-posts-in-a-given-category
    - Post can be compared (because of date attribute)
- Sidenote CSS: https://edwardtufte.github.io/tufte-css/
- Multi-blog with Jekyll
    - References:
       - https://www.garron.me/en/blog/multi-blog-site-jekyll.html
       - 


Checklist of stuff to support
=============================

- [X] MathJax (SVG or MathML?)
- [X] Tag page
- [-] Category page
- [-] Search box
- [X] Disqus
- [X] prev/next buttons at two sides (ref: http://scribble.muan.co/2013/05/06/scribble-the-jekyll-theme/)
- [X] Font style (ref: http://demo.studiopress.com/scribble/colors/blue)
- [X] Another (smaller) font style to check out: http://jwillmer.de/blog/tutorial/how-to-install-jekyll-and-pages-gem-on-windows-10-x46
    - Resources:
        - font awesome: http://fontawesome.io/examples/
        - google font for web: https://fonts.google.com/
- [ ] Javascript integrity
    - example:

          <link href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css"
                rel="stylesheet"
                integrity="sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u"
                crossorigin="anonymous">

    - see more: https://www.srihash.org/
- [X] Add /favicon.ico
- [X] CSS for related posts
- [ ] Review colour consistency in CSS


Font choices
============

Display fonts good for title
- Abril Fatface (missing slanted "l")
- Chonburi (missing double "f")
- Sansita One (1 style, Omnibus-Type), missing slanted "l"
- Yatra One

- Gabriela (missing slanted "l")
- Stardos Stencil (missing double "f")
- Cormorant SC (small cap, missing double "f")

- Coiny
- Unica One (1 style, Eduardo Tunni)

Body fonts good for text:
- EB Garamond (serif, 1 style)
- Cormorant Garamond (serif, 10 styles)
- Cormorant (serif, 10 styles) *
- Halent (serif, 5 styles)

- Marcellus (sans, 1 style)


Decision
--------

headline text: Lora;
body text: Cormorant


Colour functions
================
body background
header & footer background
body text
quoted text
header & footer text
link
link hover background
header link hover
header menu
header menu hover
header menu select
pager
pager hover
page title text
page subtitle text
post title text
post subtitle text
post meta
post excerpt more
post related
post related excerpt title
post related excerpt meta (date)
post related excerpt body



How to set up  GHP locally with Jekyll
======================================
Set up git repo:

    $ git init my-jekyll-site-project
    $ cd my-jekyll-site-project
    $ git checkout -b gh-pages

We need branch gh-pages in a project repo for GitHub pages.

Upload to github:

    $ git remote add origin <remote repo url>
    $ git remove -v   # verify remote
    $ git push origin master

Install jekyll:

    $ cat > Gemfile
    source 'https://rubygems.org'
    gem 'github-pages', group: :jekyll_plugins
    ^D
    $ bundle install

Optionally, generate Jekyl site files:

    $ bundle exec jekyll new . --force

then remove the following line from Gemfile

    $ "jekyll", "3.2.1"

and uncomment this line:

    $ gem "github-pages", group :jekyll_plugins

Then edit Jekyll template site

Local testing:

    $ bundle exec jekyll serve

which sets up a webserver at localhost


Hosting Github pages
--------------------

1. create a repository named <username>.github.io
2. push in something
3. page available as http(s)://<username>.github.io
   OR, for project page, http(s)://<username>.github.io/<projectname>
4. Mine::
     repo url = https://github.com/righthandabacus/righthandabacus.github.io.git

Github will not process through Jekyll if file ".nojekyll" exists

* Github help on pages: https://help.github.com/categories/github-pages-basics/
* custom domain: https://help.github.com/articles/setting-up-a-custom-domain-with-github-pages/


Update of dependencies
----------------------
Run update command, any of these two:
     $ bundle update github-pages
or
     $ gem update github-pages


GitHub pages versions
---------------------
Ref: https://pages.github.com/versions/ last check 2017-06-28

	jekyll/3.4.3
	activesupport/4.2.8
	github-pages-health-check/1.3.4
	github-pages/140
	html-pipeline/2.6.0
	jekyll-avatar/0.4.2
	jekyll-coffeescript/1.0.1
	jekyll-default-layout/0.1.4
	jekyll-feed/0.9.2
	jekyll-gist/1.4.0
	jekyll-github-metadata/2.3.1
	jekyll-mentions/1.2.0
	jekyll-optional-front-matter/0.1.2
	jekyll-paginate/1.1.0
	jekyll-readme-index/0.1.0
	jekyll-redirect-from/0.12.1
	jekyll-relative-links/0.4.0
	jekyll-sass-converter/1.5.0
	jekyll-seo-tag/2.2.3
	jekyll-sitemap/1.0.0
	jekyll-swiss/0.4.0
	jekyll-theme-architect/0.0.4
	jekyll-theme-cayman/0.0.4
	jekyll-theme-dinky/0.0.4
	jekyll-theme-hacker/0.0.4
	jekyll-theme-leap-day/0.0.4
	jekyll-theme-merlot/0.0.4
	jekyll-theme-midnight/0.0.4
	jekyll-theme-minimal/0.0.4
	jekyll-theme-modernist/0.0.4
	jekyll-theme-primer/0.2.1
	jekyll-theme-slate/0.0.4
	jekyll-theme-tactile/0.0.4
	jekyll-theme-time-machine/0.0.4
	jekyll-titles-from-headings/0.1.5
	jemoji/0.7.0
	kramdown/1.11.1
	liquid/3.0.6
	listen/3.0.6
	minima/1.1.0
	nokogiri/1.8.0
	rouge/1.11.1
	ruby/2.4.0
	safe_yaml/1.0.4
	sass/3.4.24

DNS point to github: Set up CNAME file with the DNS as the only line


Configurating Jekyll
--------------------

### File structures

`_config.yml`
  : holds config for Jekyll site

`_drafts/`
  : unpublished blog posts

`_includes/`
  : parts of a page to be included throughout the site

`_layouts/`
  : templates for the content, all repeating code are in a layout

`_posts/`
  : the blog posts, written in Markdown

`_data/`
  : contains YAML, JSON, CSV files, to be used throughout Jekyll site

`_site/`
  : generated static content

`.jekyll-metadata`
  : used by Jekyll's incremental build to keep track of the files which
    have changed

all other files
  : all other files are processed and output to `_site` on a build


## Default `_config.yml` settings

	# Where things are
	source:       .
	destination:  ./_site
	plugins_dir:  _plugins
	layouts_dir:  _layouts
	data_dir:     _data
	includes_dir: _includes
	collections:
	  posts:
	    output:   true
	
	# Handling Reading
	safe:         false
	include:      [".htaccess"]
	exclude:      []
	keep_files:   [".git", ".svn"]
	encoding:     "utf-8"
	markdown_ext: "markdown,mkdown,mkdn,mkd,md"
	
	# Filtering Content
	show_drafts: null
	limit_posts: 0
	future:      false
	unpublished: false
	
	# Plugins
	whitelist: []
	gems:      []
	
	# Conversion
	markdown:    kramdown
	highlighter: rouge
	lsi:         false
	excerpt_separator: "\n\n"
	incremental: false
	
	# Serving
	detach:  false
	port:    4000
	host:    127.0.0.1
	baseurl: "" # does not include hostname
	show_dir_listing: false
	
	# Outputting
	permalink:     date
	paginate_path: /page:num
	timezone:      null
	
	quiet:    false
	verbose:  false
	defaults: []
	
	liquid:
	  error_mode: warn
	
	# Markdown Processors
	rdiscount:
	  extensions: []
	
	redcarpet:
	  extensions: []
	
	kramdown:
	  auto_ids:       true
	  footnote_nr:    1
	  entity_output:  as_char
	  toc_levels:     1..6
	  smart_quotes:   lsquo,rsquo,ldquo,rdquo
	  input:          GFM
	  hard_wrap:      false
	  footnote_nr:    1


Create new site
---------------
Run command in directory:

    $ jekyll new . --force

Serve site locally (http://127.0.0.1:4000)

    $ jekyll serve --profile

Note: in `_config.yml`, "url:" should not be specified to make sure
relative links are used. `_config.yml` is read only once at jekyll serve.
Further change requires restart.


Jekyll page structure
=====================

Front matters
-------------

Front matters are between two triple dashed lines at the top of the
file. In between are YAML to set variables. Example:

    ---
    hello_text: "Hello there!"
    ---
    <!doctype html>
	<html lang="en">
	  <head>
	    <meta charset="utf-8">
	    <title>About</title>
	  </head>
	  <body>
	    <h1>About page</h1>
	    {{ page.hello_text }}
	  </body>
	</html>

Liquid is the language used to generate pages. Variable name inside
double curly braces, ``{{ var }}``, outputs the content. Logic
statements should be surrounded by curly vbraces and percentage sign,
e.g., ``{% if statement %}``

Predefined global variables:
  * layout: if set, specifies the layout file to use
  * permalink: make the post URL something other than site-wide style
    (default /year/month/day/title.html)
  * published: set to false to make it not show up in generated site

Predefined variables for posts:
  * date: Override the date setting from the name of the post. In format
    of YYYY-MM-DD HH:MM:SS +/-TTTT
  * category, or categories: specify one or more categories that the
    post belongs to. In case of categories, they can be specified as
    YAML list or comma-separated string
  * tags: Similar to categories, in YAML list or comma-separated string

See more variables: https://jekyllrb.com/docs/variables/


Posts
-----

Posts should be put under `_posts` folder, in Markdown or HTML. All posts
must have YAML front matter so they can be converted into static site.
Post files should be named as YYYY-MM-DD-title.md

Creating index of posts (usually in a template):

	<ul>
	  {% for post in site.posts %}
	    <li>
	      <a href="{{ post.url }}">{{ post.title }}</a>
	      {{ post.excerpt }}
	    </li>
	  {% endfor %}
	</ul>

Excerpt of a post is from the beginning of the content to first
occurrence of `excerpt_separator`. This is referenced as post.excerpt.
The excerpt is automatically wrapped in `<p>`. To remove the wrap, use
the following instead:

    {{ post.excerpt | remove: '<p>' | remove: '</p>' }}

Other liquid command: ``| strip_html`` to remove all HTML tags in the
output.

To include syntax highlighting of code snippets, use the following:

	{% highlight ruby %}
	def show
	  @widget = Widget(params[:id])
	  respond_to do |format|
	    format.html # show.html.erb
	    format.json { render json: @widget }
	  end
	end
	{% endhighlight %}


Collections
-----------

New type of document that behave like pages or posts but in different
namespace.

See https://jekyllrb.com/docs/collections/ for detail


Using data files
----------------

In `_data/my_members.csv`:

	name,github
	Eric Mill,konklone
	Parker Moore,parkr
	Liu Fengyun,liufengyun

In template:

	<ul>
	{% for member in site.data.my_members %}
	  <li>
	    <a href="https://github.com/{{ member.github }}">
	      {{ member.name }}
	    </a>
	  </li>
	{% endfor %}
	</ul>


Blog architectures to study
===========================
http://codinfox.github.io/blog/
https://blog.brandonparsons.me/
https://christianspecht.de/2014/10/25/separate-pages-per-tag-category-with-jekyll-without-plugins/
https://www.farnamstreetblog.com/2014/08/12-books-every-investor-should-read/

Nice and clean templates/themes:
  - scribble, https://github.com/muan/scribble
  - minimal, https://github.com/orderedlist/minimal
  - lanyon, https://github.com/poole/lanyon/
  - Tragelogue, https://github.com/SalGnt/Travelogue
  - simplygrey, https://github.com/liamsymonds/simplygrey-jekyll
  - beautiful jekyll, https://github.com/daattali/beautiful-jekyll

Tricks
======
Use CSS to specify different font for CJK vs latin characters
  - <https://blog.yorkxin.org/2012/06/17/assign-fonts-for-specific-characters>
  - <http://wcc723.github.io/sass/2014/02/21/font-code-range>
  - use `@font-face` and `unicode-range`

Chinese page setting on web:
  - <https://wanderer.tw/簡單做好中文排版/>

Free weeb design ebooks:
  - <https://speckyboy.com/free-web-design-ebooks-2014/>

You might not need JS:
  - <http://youmightnotneedjs.com/>
  - <https://robots.thoughtbot.com/you-don-t-need-javascript-for-that>
  - <https://github.com/you-dont-need/You-Dont-Need-Javascript>
  - CSS3 crossfading images: <http://css3.bradshawenterprises.com/cfimg/>

Better tag cloud using heavier JS+CSS:
  - <http://mistic100.github.io/jQCloud/demo.html>

Highlight.js CDN: For uncommon languages, need to load another js
  - <https://cdnjs.com/libraries/highlight.js>
