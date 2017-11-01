---
layout: post
title: "Spamassassin"
date: 2007-08-09 05:25:29 +0800
tags: system
---

Setup spamassassin in Debian is easy: just apt-get. Exim users might need sa-exim for rejecting mails at the gateway.

The SARE (Spam Assassin Rules Emporium, <http://www.rulesemporium.com/>) has a bunch of rule for download. Put them into `/etc/spamassassin/` and restart the spamd.

## Writing rules

Customized rules can be written and put to either `/etc/spamassassin` for global use or put to `~/.spamassassin/`. Detailed configuration please consult the manpage of `spamd`.

The syntax for the rules is as follows:

    body     NEW_RULE   /regex/
    score    NEW_RULE   0.1
    describe NEW_RULE   This is a rule

It checks the body for a regex match. If found, it add the score to the mail (negative means non-spam) and the describe line is for information only and appears in the verbose report. The regex is in Perl's dialect and flags are allowed.

Instead of checking body, one can also check the header. In the header, you can check for the `Subject`, `From` or `To`, for example:

    header ANOTHER_RULE   Subject =~ /regex/i
    score  ANOTHER_RULE   0.1

The header field to check is put before `=~`. It can be anything that can happen in the header, including obscured ones. The stuff behind `=~` is the regex to match. The name for the header field is case insensitive, which means the rule

    header YET_ANOTHER_RULE   Foobar =~ /regex/i

matches any one below:

    foobar: something with that regex
    FooBar: something with that regex
    fOObaR: something with that regex

To match anything in the header, use `ALL`, such as:

    header SHOWING_ALL  ALL =~ /regex/

To search for links in particular, we can use the `uri` rules: which only matches with links' URI:

    uri  EXAMPLE   /regex\.com/

To search for the body of email without any preprocessing, use `rawbody`:

     rawbody RAWBODY_EXAMPLE /\<\-\-! created with spamware 1\.0 \-\-\>/

To combine rules for forming a new rule, it is called `meta` rules: It uses a boolean for the combination:

     header  __BANK_SEND  From =~ /cs\@bank\.com/
     body    __FOR_LOAN   /(loan|money)/
     meta    BANK_LOAN  (__BANK_SEND && __FOR_LOAN)
     score   BANK_LOAN  -1.0

Alternatively, `meta` can also check if more than a number of rules matched:

     body    __FOR_LOAN   /loan/
     body    __MONEY      /money/
     body    __INTEREST   /interest/
     body    __CREDIT     /credit/
     meta    LOAN_MAIL    ( (__FOR_LOAN + __MONEY + __INTEREST + __CREDIT)>2 )
     describe LOAN_MAIL   Matched more than 2 items
     score   LOAN_MAIL    1.0

Or you can do a weighted measure:

     meta    LOAN_MAIL    ( (0.5*__FOR_LOAN + 0.5*__MONEY + 0.3*__INTEREST + 0.8*__CREDIT)>1.3 )

