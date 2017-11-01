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
