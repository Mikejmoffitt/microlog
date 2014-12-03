Microlog
========

What is it
----------
Microlog is a barebones static site generator that aims to replace the use of
blogging software as a means of publishing project logs. Many un-needed 
features of other popular software will not be implemented, like social network
integration, an online editor, comments, the concept of authenticating, or
any front-face interactivity other than hyperlinks.

Why would you do that
---------------------
There are a lot of similar things like this but I wanted to have a go at
making my own. Plus, this way it can work in whatever screwy way I think of
instead of, I don't know, something else.

Is it hard to set up
--------------------
No. Microlog is just a program that needs to be run when the site is updated.
The only sever software needed is a super simple HTTP server. Don't worry about
security or a database or auth or anything like that - security is just a huge
illusion anyway. The only authentication you care about is SSHing into the 
machine running it (it could be a potato, just about anything can handle this).

Okay how does it work
--------------------
Running microlog generates a site\ directory. Inside is an index, a pages/
directory, an articles/ directory, and a res/ directory. These are generated 
from what you put in the source/ directory.

An index2, index3, etc. may also be generated. These refer to pages of
navigation through the site index should the number of articles grow too large.
The max number of articles per page may be specified when microlog is run.

Inside source/articles are articles. They are numbered starting at 0, and 
increment by one each time a new one is added. For creating these I suggest
any old text editor. 

There is also source/pages. These are like articles, except they have their
own index that is fixed at the top of the page - things like "about" and 
"downloads", etc. This feature is not yet implemented.

Finally, there's source/resources. This is exactly what it sounds like, it just
lets you reference things by their names and it'll drop them into res/ in
the final site output. No need to specify the directory.

This sounds dumb
----------------
This tool isn't made to be versatile, or to fit the uses of others. If you 
don't like it, then that's fine, it's not for you. It's just for generating my
extremely simple website. If the lack of modern web 2.0 HD super plus turbo
features scares you off, then that's great.

On the bright side, it'll produce pretty fast pages that'll look just fine in
text-only browsers!

Tell me more about articles
---------------------------
Articles are written in plaintext, though HTML formatting may be used. Use of
deprecated tags like <u> and </u> is fine. Styles may be defined in styles.css,
which is in the site's output directory.

There's no need to put any of the HTML boilerplate. In fact, don't, it might
break something. All that is needed at the top is the first few lines to be
formatted like this:

[Article Name]
[Date written]

The first two lines are used for generating the index. The max length for the
title is 80 characters. The date is pretty strictly in this format:

    11/20/1992

That's my birthday, though. Put whatever date you want in there.
After that, type away. Feel free to use inline HTML bold and italic lines 
for emphasis, etc.

What sort of editing stuff can I do
-----------------------------------
While you can put HTML tags in there on your own, there are a few shorthands
available. A pound sign #, followed by one of a few specific characters, stands
in for some tags. A second pound sign signifies the appropriate closing.

If the parser comes across \#i(name)# it'll replace it with appropriate <img>
tags in the output html. This is just shorthand for <img src="res/name" />. Alt
text isn't included, if that's a big deal then you're already writing a
mouthful so you might as well write the whole \<img> tag structure.

HTML5 BELLS AND WHISTLES
------------------------
\#v(identifier)# generates an embedded mp4 video.
\#a(identifier)# generates an embedded audio playback control.

\#m(identifier)# is like a, but it does not link to res/. You can hard link with
this one.

Web 2.0!

How can I integrate my bootstrap theme
--------------------------------------
Please leave!
