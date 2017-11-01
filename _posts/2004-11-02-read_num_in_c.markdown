---
layout: post
title: "Reading numbers in C"
date: 2004-11-02 22:20:19 +0800
lastedit: Tue, 11 May 2010 12:35:47 -0400
tags: code
---

Numbers can be complicated. For example, whether you accept scientific notation
as floating point numbers can cause your program have a double number of lines.

Of course, the simplest way to grab any text is using the regular expressions.
If you are using POSIX-capable systems, the natural way is to use the POSIX
regex library. As shown in the program at the bottom.

The POSIX regex functions are declared in `regex.h` and the major functions are
`regcomp()` to compile the regex, `regexec()` to perform a match, `regerror()`
to collect error messages and `regfree()` to reclaim the memory occupied by
`regcomp()`.

However, if you just need simplest number-reading function in C, probably
`strtod()` is good enough to use. For example, the following code excerpt:

```c
  char *ptr, *ptrNext, buff[BUFSIZE];
  FILE* fpMatrix=fopen("matrix.txt","r");
  ....
  for(i=0;i<m;i++) {
      if(!fgets(buff, BUFSIZE, fpMatrix)) {
          perror("Failed to finish reading the whole matrix");
          exit(1);
      };
      ptr = buff;     /* We start at the beginning of *buf */
      for(j=0;j<m;j++) {
          R[i][j]=strtod(ptr, &ptrNext);
          if(ptr==ptrNext) {
              perror("Failed to finish reading a row of matrix");
              exit(1);
          };
          ptr = ptrNext;  /* Shift to next number */
      };
  };
```

The above excerpt is to read in a m-by-m matrix with each element is a double-precision float. The file is presented in rows as lines and each element in a row separated by some word-separating characters like spaces or tab. The validity of the above excerpt is due to the use of second parameter to `strtod()` function.

[`regex-example.c`](/img/regex-example.c):

```c
/* regex-example2.c
 * Tue, 11 May 2010 12:35:47 -0400
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>

/* Example on how to use POSIX Regex calls
 * The regex calls supported in *nix can be those provided by GNU, from BSD, or
 * as in POSIX standard.
 *
 * Here the POSIX version is used and the following are the API:
 *   int regcomp(regex_t *preg, const char *regex, int cflags);
 *   int regexec(const regex_t *preg, const char *string, size_t nmatch, regmatch_t pmatch[], int eflags);
 *   size_t regerror(int errcode, const regex_t *preg, char *errbuf, size_t errbuf_size);
 *   void regfree(regex_t *preg);
 */

int main (int argc, char** argv)
{
	/* First example: Match a string multiple time */
	char* string = "   0.1234  1.2345  2.34567 	8 9   10.111213";
	char* pstr = string;
	int errcode;
	regex_t *preg = (regex_t*)malloc(sizeof(regex_t));
	regmatch_t *pmatch;

	/* Compile regex */
	if ((errcode=regcomp(preg, "([0-9]+\\.[0-9]+)",REG_EXTENDED|REG_NEWLINE))) {
		char* errbuf;
		size_t errbuf_size;

		errbuf_size = regerror(errcode, preg, NULL, 0);
		if (!(errbuf=(char*)malloc(errbuf_size))) {
			perror("malloc error!");
			exit(255);
		};
		regerror(errcode, preg, errbuf, errbuf_size);
		fprintf(stderr, "Regex compilation error: %s\n", errbuf);
		exit(1);
	};

	/* Allocate substrings buffer (1 substring) */
	pmatch = (regmatch_t*)malloc(sizeof(regmatch_t)*preg->re_nsub);

	/* Match regex to string */
	if(regexec(preg, pstr, preg->re_nsub, pmatch, 0)) {
		printf("Nothing matched with ""%s""\n",pstr);
		exit(0);
	};
	
	/* Loop to print all the matches */
	do {
		if (pmatch[0].rm_so != -1) {	/* The regex is matching part of a string */
			char *submatch;
			size_t matchlen = pmatch[0].rm_eo - pmatch[0].rm_so;
			submatch = (char*)malloc(matchlen+1);
			strncpy(submatch, pstr+pmatch[0].rm_so, matchlen+1);
			submatch[matchlen]='\0';
			printf("%s;\n", submatch);
			free(submatch);
		};
		pstr += pmatch[0].rm_eo;	/* Restart from last match */
	} while(!regexec(preg,pstr,preg->re_nsub,pmatch,0));
	regfree(preg);
	free(pmatch);

	/* Second example: Match the whole string once and extract multiple captures */
	string = "I have 20 apples and 34 oranges.";
	pstr = string;
	regcomp(preg, "([0-9]+) apples.* ([0-9]+) oranges",REG_EXTENDED|REG_NEWLINE);
	pmatch = (regmatch_t*)malloc(sizeof(regmatch_t)*(1+preg->re_nsub));
	printf("There can be %d captures\n",(int)preg->re_nsub);
	if(regexec(preg, pstr, 1+preg->re_nsub, pmatch, 0)) {
		printf("Nothing matched with ""%s""\n",pstr);
		exit(0);
	};
	if (pmatch[0].rm_so != -1) {	/* Regex matched */
		/* The whole matched part */
		char *submatch;
		size_t matchlen = pmatch[0].rm_eo - pmatch[0].rm_so;
		submatch = (char*)malloc(matchlen+1);
		strncpy(submatch, pstr+pmatch[0].rm_so, matchlen+1);
		submatch[matchlen]='\0';
		printf("Matched part = %s;\n", submatch);
		free(submatch);
		/* First capture bracket */
		matchlen = pmatch[1].rm_eo - pmatch[1].rm_so;
		submatch = (char*)malloc(matchlen+1);
		strncpy(submatch, pstr+pmatch[1].rm_so, matchlen+1);
		submatch[matchlen]='\0';
		printf("Apple = %s;\n", submatch);
		free(submatch);
		/* Second capture bracket */
		matchlen = pmatch[2].rm_eo - pmatch[2].rm_so;
		submatch = (char*)malloc(matchlen+1);
		strncpy(submatch, pstr+pmatch[2].rm_so, matchlen+1);
		submatch[matchlen]='\0';
		printf("Orange = %s;\n", submatch);
		free(submatch);
	};

	free(preg);
	exit(0);
};
```
