/* vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap: */
/**
 * @author
 *    Copyright 2006 (c) Adrian Sai-wah Tam.
 *    Released under GNU Lesser General Public Licence
 * @date
 *    Tue Apr 17 14:13:24 HKT 2007
 *
 * @brief
 *   The C++ transformation of the MD5 implementation by L. Peter Deutsch
 * @file
 *   MD5 is specified in RFC1321 and the code by L. Peter Deutsch can be
 *   obtained from http://sourceforge.net/projects/libmd5-rfc/ at the time
 *   or writing. The original code by Deutsch carries the following copyright
 *   notice:
 *   @verbatim
	Copyright (C) 1999 Aladdin Enterprises.  All rights reserved.

	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	   claim that you wrote the original software. If you use this software
	   in a product, an acknowledgment in the product documentation would be
	   appreciated but is not required.
	2. Altered source versions must be plainly marked as such, and must not be
	   misrepresented as being the original software.
	3. This notice may not be removed or altered from any source distribution.

	L. Peter Deutsch
	ghost@aladdin.com
     @endverbatim
 */

#include "md5.h"
#include <string>

/** @brief Constructor: initialize the attributes */
md5::md5()
{
	reset();
};

/** @brief Initialize the digest buffer the set-zero the message length counters */
void md5::reset()
{
	count[0] = count[1] = 0;
	abcd[0]  = 0x67452301;
	abcd[1]  = /*0xefcdab89*/ T_MASK ^ 0x10325476;
	abcd[2]  = /*0x98badcfe*/ T_MASK ^ 0x67452301;
	abcd[3]  = 0x10325476;
};

/** @brief Give the MD5 digest of the supplied C++ string */
md5_byte_t* md5::digest(const string s)
{
	return digest(s.c_str());
};

/** @brief Give the MD5 digest of the supplied C-string */
md5_byte_t* md5::digest(const char* s)
{
	reset();
	append(s);
	return getDigest();
};

/** @brief Get the MD5 digest of the string appended so far */
md5_byte_t* md5::getDigest()
{
	static const md5_byte_t pad[64] = {
		0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
	md5_byte_t data[8];
	int i;

	/* Save the length before padding. */
	for (i = 0; i < 8; ++i)
		data[i] = (md5_byte_t)(count[i >> 2] >> ((i & 3) << 3));
	/* Pad to 56 bytes mod 64. */
	append((char*)pad, ((55 - (count[0] >> 3)) & 63) + 1);
	/* Append the length. */
	append((char*)data, 8);
	for (i = 0; i < 16; ++i)
		digestCode[i] = (md5_byte_t)(abcd[i >> 2] >> ((i & 3) << 3));
	return digestCode;
};

/** @brief Append a C++ string into the buffer for MD5 digest calculation */
void md5::append(const string s)
{
	append(s.c_str(), s.size());
};

/** @brief Append a C-string into the buffer for MD5 digest calculation */
void md5::append(const char*data, int nbytes=-1)
{
	if (nbytes==-1)
		nbytes = strlen(data);
	if (nbytes <= 0)
		return;

	int offset = (count[0] >> 3) & 63;
	md5_word_t nbits = (md5_word_t)(nbytes << 3);

	/* Update the message length. */
	count[1] += nbytes >> 29;
	count[0] += nbits;
	if (count[0] < nbits)
		count[1]++;

	/* Process an initial partial block. */
	if (offset) {
		int copy = (offset+nbytes > 64) ? (64-offset) : nbytes;
		memcpy(buf + offset, data, copy);
		if (offset + copy < 64)
			return;
		data += copy;
		nbytes -= copy;
		process(buf);
	}

	/* Process full blocks. */
	for (; nbytes >= 64; data += 64, nbytes -= 64)
		process((md5_byte_t*)data);

	/* Process a final partial block. */
	if (nbytes)
		memcpy(buf, data, nbytes);
}
 
md5& operator<<(md5& m,const string s)
{
	m.append(s);
	return m;
};

md5& operator<<(md5& m, const char*s)
{
	m.append(s);
	return m;
};

/** @brief Internal function to process a block of 64-byte data */
void md5::process(const md5_byte_t *data)
{
	md5_word_t	a=abcd[0],
			b=abcd[1],
			c=abcd[2],
			d=abcd[3];
	md5_word_t	t;
#if BYTE_ORDER > 0
	/* Define storage only for big-endian CPUs. */
	md5_word_t	X[16];
#else
	/* Define storage for little-endian or both types of CPUs. */
	md5_word_t	xbuf[16];
	const md5_word_t *X;
#endif

#if BYTE_ORDER == 0
	/* Determine dynamically whether this is a big-endian or
	 * little-endian machine, since we can use a more efficient
	 * algorithm on the latter.
	 */
	static const int w = 1;

	if (*((const md5_byte_t *)&w)) {	/* dynamic little-endian */
#endif
#if BYTE_ORDER <= 0		/* little-endian */
		/* On little-endian machines, we can process properly aligned
		 * data without copying it.
		 */
		if (!((data - (const md5_byte_t *)0) & 3)) {
			/* data are properly aligned */
			X = (const md5_word_t *)data;
		} else {
			/* not aligned */
			memcpy(xbuf, data, 64);
			X = xbuf;
		}
#endif
#if BYTE_ORDER == 0
	} else {		/* dynamic big-endian */
#endif
#if BYTE_ORDER >= 0		/* big-endian */
		/* On big-endian machines, we must arrange the bytes in the
		 * right order.
		 */
		const md5_byte_t *xp = data;
		int i;
#    if BYTE_ORDER == 0
		X = xbuf;	/* (dynamic only) */
#    else
#        define xbuf X		/* (static only) */
#    endif
		for (i = 0; i < 16; ++i, xp += 4)
			xbuf[i] = xp[0] + (xp[1] << 8) + (xp[2] << 16) + (xp[3] << 24);
#endif
#if BYTE_ORDER == 0
	};
#endif

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
	/* Round 1. */
	/* Let [abcd k s i] denote the operation
	     a = b + ((a + F(b,c,d) + X[k] + T[i]) <<< s). */
#define F(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define SET(a, b, c, d, k, s, Ti)\
		t = a + F(b,c,d) + X[k] + Ti;\
		a = ROTATE_LEFT(t, s) + b
	/* Do the following 16 operations. */
	SET(a, b, c, d,  0,  7, T01);
	SET(d, a, b, c,  1, 12, T02);
	SET(c, d, a, b,  2, 17, T03);
	SET(b, c, d, a,  3, 22, T04);
	SET(a, b, c, d,  4,  7, T05);
	SET(d, a, b, c,  5, 12, T06);
	SET(c, d, a, b,  6, 17, T07);
	SET(b, c, d, a,  7, 22, T08);
	SET(a, b, c, d,  8,  7, T09);
	SET(d, a, b, c,  9, 12, T10);
	SET(c, d, a, b, 10, 17, T11);
	SET(b, c, d, a, 11, 22, T12);
	SET(a, b, c, d, 12,  7, T13);
	SET(d, a, b, c, 13, 12, T14);
	SET(c, d, a, b, 14, 17, T15);
	SET(b, c, d, a, 15, 22, T16);
#undef SET

	/* Round 2. */
	/* Let [abcd k s i] denote the operation
	     a = b + ((a + G(b,c,d) + X[k] + T[i]) <<< s). */
#define G(x, y, z) (((x) & (z)) | ((y) & ~(z)))
#define SET(a, b, c, d, k, s, Ti)\
		t = a + G(b,c,d) + X[k] + Ti;\
		a = ROTATE_LEFT(t, s) + b
	/* Do the following 16 operations. */
	SET(a, b, c, d,  1,  5, T17);
	SET(d, a, b, c,  6,  9, T18);
	SET(c, d, a, b, 11, 14, T19);
	SET(b, c, d, a,  0, 20, T20);
	SET(a, b, c, d,  5,  5, T21);
	SET(d, a, b, c, 10,  9, T22);
	SET(c, d, a, b, 15, 14, T23);
	SET(b, c, d, a,  4, 20, T24);
	SET(a, b, c, d,  9,  5, T25);
	SET(d, a, b, c, 14,  9, T26);
	SET(c, d, a, b,  3, 14, T27);
	SET(b, c, d, a,  8, 20, T28);
	SET(a, b, c, d, 13,  5, T29);
	SET(d, a, b, c,  2,  9, T30);
	SET(c, d, a, b,  7, 14, T31);
	SET(b, c, d, a, 12, 20, T32);
#undef SET

	 /* Round 3. */
	 /* Let [abcd k s t] denote the operation
	      a = b + ((a + H(b,c,d) + X[k] + T[i]) <<< s). */
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define SET(a, b, c, d, k, s, Ti)\
		t = a + H(b,c,d) + X[k] + Ti;\
		a = ROTATE_LEFT(t, s) + b
	 /* Do the following 16 operations. */
	SET(a, b, c, d,  5,  4, T33);
	SET(d, a, b, c,  8, 11, T34);
	SET(c, d, a, b, 11, 16, T35);
	SET(b, c, d, a, 14, 23, T36);
	SET(a, b, c, d,  1,  4, T37);
	SET(d, a, b, c,  4, 11, T38);
	SET(c, d, a, b,  7, 16, T39);
	SET(b, c, d, a, 10, 23, T40);
	SET(a, b, c, d, 13,  4, T41);
	SET(d, a, b, c,  0, 11, T42);
	SET(c, d, a, b,  3, 16, T43);
	SET(b, c, d, a,  6, 23, T44);
	SET(a, b, c, d,  9,  4, T45);
	SET(d, a, b, c, 12, 11, T46);
	SET(c, d, a, b, 15, 16, T47);
	SET(b, c, d, a,  2, 23, T48);
#undef SET

	 /* Round 4. */
	 /* Let [abcd k s t] denote the operation
	      a = b + ((a + I(b,c,d) + X[k] + T[i]) <<< s). */
#define I(x, y, z) ((y) ^ ((x) | ~(z)))
#define SET(a, b, c, d, k, s, Ti)\
		t = a + I(b,c,d) + X[k] + Ti;\
		a = ROTATE_LEFT(t, s) + b
	 /* Do the following 16 operations. */
	SET(a, b, c, d,  0,  6, T49);
	SET(d, a, b, c,  7, 10, T50);
	SET(c, d, a, b, 14, 15, T51);
	SET(b, c, d, a,  5, 21, T52);
	SET(a, b, c, d, 12,  6, T53);
	SET(d, a, b, c,  3, 10, T54);
	SET(c, d, a, b, 10, 15, T55);
	SET(b, c, d, a,  1, 21, T56);
	SET(a, b, c, d,  8,  6, T57);
	SET(d, a, b, c, 15, 10, T58);
	SET(c, d, a, b,  6, 15, T59);
	SET(b, c, d, a, 13, 21, T60);
	SET(a, b, c, d,  4,  6, T61);
	SET(d, a, b, c, 11, 10, T62);
	SET(c, d, a, b,  2, 15, T63);
	SET(b, c, d, a,  9, 21, T64);
#undef SET

	 /* Then perform the following additions. (That is increment each
	    of the four registers by the value it had before this block
	    was started.) */
	abcd[0] += a;
	abcd[1] += b;
	abcd[2] += c;
	abcd[3] += d;
}
