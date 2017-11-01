/* vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap: */
/**
 * @author
 *    Copyright 2007 (c) Adrian Sai-wah Tam.
 *    Released under GNU Lesser General Public Licence
 * @date
 *    Tue Apr 17 14:13:24 HKT 2007
 *
 * @file
 * @brief
 *   Header file to patricia_cpp.cc
 */

#ifndef __PATRICIA_H__
#define __PATRICIA_H__

#include <stdlib.h>

/** @class patricia patricia.h
 *  @brief
 *     A Patricia tree node
 */
class patricia {
	protected:
		char*		str;		///< Key of this node
		int		len;		///< Length of key in bits
		void*		data;		///< Data associated with this key
		patricia*	left;		///< Left branch (1 bit) pointer
		patricia*	right;		///< Right branch (0 bit) pointer
	/* Internal functions */
	protected:
		patricia*	_search(char* key, const patricia*last=NULL) const;
		void		_freekey(patricia* node);
		void		_clone(const patricia* target);
		static int	_bitncmp(const char*s1,const char*s2,int n);
		static int	_extractbit(const char*s,int n);
		static int	_getmatchlen(const char*s1,const char*s2);
		static int	_bitlen(const char* const s);
		static int	_charcmp(const char* s, const char* t);
	public:
		patricia();
		patricia(char* key, void* data);
		patricia(const patricia* node);
		patricia*	maxmatch(char* key, const patricia*last=NULL) const;
		void		add(char* key, void* data);
		void		del(char* key, patricia*papa=NULL);
		inline char*	getkey() const { return str; };
		inline void*	getdata() const { return data; };
#ifdef UNITTEST
		int		validate() const;
		void		printcompact() const;
		void		print(int n=0) const;
	friend	int main();
#endif
};

#ifdef UNITTEST
#include <stdio.h>
#include <string.h>
#include <assert.h>
#endif

#endif
