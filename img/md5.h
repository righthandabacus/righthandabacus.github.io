/* vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap: */
/**
 * @author
 *    Copyright 2006 (c) Adrian Sai-wah Tam.
 *    Released under GNU Lesser General Public Licence
 * @date
 *    Tue Apr 17 14:13:24 HKT 2007
 *
 * @brief
 *   Header file of the C++ transformation of the MD5 implementation by L. Peter Deutsch
 * @file
 */

#ifndef __MD5_H__
#define __MD5_H__

#include "md5.h"
#include <string>

using std::string;

typedef unsigned char md5_byte_t;	/**< 8-bit byte */
typedef unsigned int md5_word_t;	/**< 32-bit word */

/** @class md5 md5.h
 *  @brief Engine for calculating the 128-bit MD5 digest */
class md5 {
	protected:
		md5_word_t	count[2];	/**< message length in bits, lsw first */
		md5_word_t	abcd[4];	/**< digest buffer */
		md5_byte_t	buf[64];	/**< accumulate block */
		md5_byte_t	digestCode[16];
		void process(const md5_byte_t*data);
	public:
		md5();
		void reset();
		md5_byte_t* digest(const char* s);
		md5_byte_t* digest(const string s);
		md5_byte_t* getDigest();
		void append(const char*data,int nbytes);
		void append(const string s);
};

md5& operator<<(md5& m, const char*s);
md5& operator<<(md5& m,const string s);

/* Definition of constants */
#undef BYTE_ORDER	/* 1 = big-endian, -1 = little-endian, 0 = unknown */
#ifdef ARCH_IS_BIG_ENDIAN
#  define BYTE_ORDER (ARCH_IS_BIG_ENDIAN ? 1 : -1)
#else
#  define BYTE_ORDER 0
#endif

#define T_MASK ((md5_word_t)~0)
#define T01 /* 0xd76aa478 */ (T_MASK ^ 0x28955b87)
#define T02 /* 0xe8c7b756 */ (T_MASK ^ 0x173848a9)
#define T03    0x242070db
#define T04 /* 0xc1bdceee */ (T_MASK ^ 0x3e423111)
#define T05 /* 0xf57c0faf */ (T_MASK ^ 0x0a83f050)
#define T06    0x4787c62a
#define T07 /* 0xa8304613 */ (T_MASK ^ 0x57cfb9ec)
#define T08 /* 0xfd469501 */ (T_MASK ^ 0x02b96afe)
#define T09    0x698098d8
#define T10 /* 0x8b44f7af */ (T_MASK ^ 0x74bb0850)
#define T11 /* 0xffff5bb1 */ (T_MASK ^ 0x0000a44e)
#define T12 /* 0x895cd7be */ (T_MASK ^ 0x76a32841)
#define T13    0x6b901122
#define T14 /* 0xfd987193 */ (T_MASK ^ 0x02678e6c)
#define T15 /* 0xa679438e */ (T_MASK ^ 0x5986bc71)
#define T16    0x49b40821
#define T17 /* 0xf61e2562 */ (T_MASK ^ 0x09e1da9d)
#define T18 /* 0xc040b340 */ (T_MASK ^ 0x3fbf4cbf)
#define T19    0x265e5a51
#define T20 /* 0xe9b6c7aa */ (T_MASK ^ 0x16493855)
#define T21 /* 0xd62f105d */ (T_MASK ^ 0x29d0efa2)
#define T22    0x02441453
#define T23 /* 0xd8a1e681 */ (T_MASK ^ 0x275e197e)
#define T24 /* 0xe7d3fbc8 */ (T_MASK ^ 0x182c0437)
#define T25    0x21e1cde6
#define T26 /* 0xc33707d6 */ (T_MASK ^ 0x3cc8f829)
#define T27 /* 0xf4d50d87 */ (T_MASK ^ 0x0b2af278)
#define T28    0x455a14ed
#define T29 /* 0xa9e3e905 */ (T_MASK ^ 0x561c16fa)
#define T30 /* 0xfcefa3f8 */ (T_MASK ^ 0x03105c07)
#define T31    0x676f02d9
#define T32 /* 0x8d2a4c8a */ (T_MASK ^ 0x72d5b375)
#define T33 /* 0xfffa3942 */ (T_MASK ^ 0x0005c6bd)
#define T34 /* 0x8771f681 */ (T_MASK ^ 0x788e097e)
#define T35    0x6d9d6122
#define T36 /* 0xfde5380c */ (T_MASK ^ 0x021ac7f3)
#define T37 /* 0xa4beea44 */ (T_MASK ^ 0x5b4115bb)
#define T38    0x4bdecfa9
#define T39 /* 0xf6bb4b60 */ (T_MASK ^ 0x0944b49f)
#define T40 /* 0xbebfbc70 */ (T_MASK ^ 0x4140438f)
#define T41    0x289b7ec6
#define T42 /* 0xeaa127fa */ (T_MASK ^ 0x155ed805)
#define T43 /* 0xd4ef3085 */ (T_MASK ^ 0x2b10cf7a)
#define T44    0x04881d05
#define T45 /* 0xd9d4d039 */ (T_MASK ^ 0x262b2fc6)
#define T46 /* 0xe6db99e5 */ (T_MASK ^ 0x1924661a)
#define T47    0x1fa27cf8
#define T48 /* 0xc4ac5665 */ (T_MASK ^ 0x3b53a99a)
#define T49 /* 0xf4292244 */ (T_MASK ^ 0x0bd6ddbb)
#define T50    0x432aff97
#define T51 /* 0xab9423a7 */ (T_MASK ^ 0x546bdc58)
#define T52 /* 0xfc93a039 */ (T_MASK ^ 0x036c5fc6)
#define T53    0x655b59c3
#define T54 /* 0x8f0ccc92 */ (T_MASK ^ 0x70f3336d)
#define T55 /* 0xffeff47d */ (T_MASK ^ 0x00100b82)
#define T56 /* 0x85845dd1 */ (T_MASK ^ 0x7a7ba22e)
#define T57    0x6fa87e4f
#define T58 /* 0xfe2ce6e0 */ (T_MASK ^ 0x01d3191f)
#define T59 /* 0xa3014314 */ (T_MASK ^ 0x5cfebceb)
#define T60    0x4e0811a1
#define T61 /* 0xf7537e82 */ (T_MASK ^ 0x08ac817d)
#define T62 /* 0xbd3af235 */ (T_MASK ^ 0x42c50dca)
#define T63    0x2ad7d2bb
#define T64 /* 0xeb86d391 */ (T_MASK ^ 0x14792c6e)

#endif /*  __MD5_H__ */
