/* vim:set ts=8 sw=8 filetype=cpp noai nosi cin nu noet nolbr sm sbr=">" ff=unix syntax=cpp noic nowrap: */
/**
 * @author
 *    Copyright 2007 (c) Adrian Sai-wah Tam.
 *    Released under GNU Lesser General Public Licence
 * @date
 *    Tue Apr 17 14:13:24 HKT 2007
 *
 * @brief
 *   Implementation of Patricia tree in C++
 *
 * @file
 * The idea of Patricia tree implemented here was from:\n
 *    Donald R. Morrison. "PATRICIA -- practical algorithm to retrieve
 *    information coded in alphanumeric". Journal of the ACM, 15(4):514-534,
 *    October 1968\n
 * This implementation is standalone in the sense that dependence to external
 * libraries are kept at minimal
 */

#include "patricia.h"

/** @brief Default constructor: Set null everything */
patricia::patricia()
{
	right = left = NULL;
	str = NULL;
	data = NULL;
	len = 0;
};

/** @brief Create a new Patricia tree node (_key,_data) with initialization */
patricia::patricia(char* _key, void* _data)
{
	str = _key;
	len = _bitlen(_key);
	data = _data;
	left = right = NULL;
};

/** @brief Copy construcutor */
patricia::patricia(const patricia* node)
{
	str = node->str;
	len = node->len;
	data = node->data;
	left = node->left;
	right = node->right;
};

/** @brief Compare bit vectors s1 and s2 for at most n bits
 *  @return
 *        a boolean value, indicates whether they match (==0) or not (!=0)
 */
int patricia::_bitncmp(const char*s1,const char*s2,int n)
{
	char mask = (char)(-1);				/* Assumed signed, two's complement representation */
	while((unsigned)n>=8*sizeof(char)) {		/* If we have to check for more than a char */
		if (*s1==*s2) {				/* use character equality to check */
			if (*s1 == '\0') return 0;	/* return equal if we exhausted the string */
			if (n-=8*sizeof(char)) {	/* shift for 8*sizeof(char) bits */
				s1++; s2++;
				continue;		/* continue if we still have bits to match */
			};
			return 0;			/* return equal if we exhaused n */
		};
		return (int)*s1 ^ (int)*s2;		/* return XOR of two char if they don't match */
	};
	return (((int)*s1 ^ (int)*s2) & (int)(mask<<(8*sizeof(char)-n)));
							/* partial char: compare the first n MSBs */
}

/** @brief Extract the n-th bit from the bit vector s. MSB is defined as the 1st bit. */
int patricia::_extractbit(const char*s,int n)
{
	n--;
	s += n/(8*sizeof(char));		/* Shift to the correct byte */
	n = n%(8*sizeof(char))+1;		/* Shift to the correct byte */
	return (unsigned)(*s) & (0x01<<(8*sizeof(char)-n));
}

/** @brief Compare bit vectors s1 and s2 for the highest number of equal bits */
int patricia::_getmatchlen(const char*s1,const char*s2)
{
	char c;
	int n;
	for (n=0; *s1==*s2; n+=8*sizeof(char)) {	/* Count for full chars */
		s1++; s2++;				/* Shift back the last unmatched byte */
	};
	c = *s1 ^ *s2;					/* Count for partial char, assumed 1 char = 1 byte */
	if (c & '\x80') n += 0;				/* n += highest asserted bit of the XOR */
	else if (c & '\x40') n+= 1;
	else if (c & '\x20') n+= 2;
	else if (c & '\x10') n+= 3;
	else if (c & '\x08') n+= 4;
	else if (c & '\x04') n+= 5;
	else if (c & '\x02') n+= 6;
	else if (c & '\x01') n+= 7;
	return n;
}

/** @brief Measure the length of a string in number of bits */
int patricia::_bitlen(const char* const s)
{
	int i;
	for(i=0;s[i];i++);
	return i*8*sizeof(char);
};

/** @brief Compare two strings to see if they are equal */
int patricia::_charcmp(const char* s, const char* t)
{
	for(;*s && *s==*t; s++, t++);
	return *s ^ *t;
};

/** @brief
 *    Search for a string in the Patricia tree and return the maximal match.
 *  @param last
 *     Denotes the last terminiating node ever traversed. Defaulted to NULL and
 *     it should not be set or used by the external caller of this function.
 *  @param _key
 *     The string to perform maximal matching. It can be arbitrary long and
 *     this function will check for the dictionary word that matches from the
 *     left.
 *  @return
 *     The most matching terminating node in the Patricia tree. If the first
 *     bit of the string doesn't fit the root node (for example, the tree
 *     is empty), then a null pointer is returned.
 */
patricia* patricia::maxmatch(char* _key, const patricia* last) const
{
	if ( _bitncmp(_key,str,len) ) {
		/* If I am not a substring of the _key */
		return (patricia*)last;
	} else if ( _bitlen(_key) == len ) {
		/* If I am exactly the _key */
		return (patricia*)this;
	} else {
		/* So, I am a substring of the _key.
		 * Recurrsively call on the suitable branch, and
		 * replacing the last terminating node with myself
		 * when appropriate */
		return (_extractbit(_key,len+1)?left:right)->
			maxmatch(_key,(_bitlen(str)==len)?this:last);
	};
};


/** @brief
 *    Search for a string in the Patricia tree and return the closest node.
 *  @param last
 *     Denotes the last terminiating node ever traversed. Defaulted to NULL and
 *     it should not be set or used by the external caller of this function.
 *  @param _key
 *     The string to search on the tree. It can be arbitrary long and
 *     this function will check for the node that best matches from the
 *     left.
 *  @return
 *     The most matching node in the Patricia tree. If the first bit of the
 *     string doesn't fit the root node (for example, the tree is empty),
 *     then a null pointer is returned.
 */
patricia* patricia::_search(char* _key, const patricia* last) const
{
	if ( _bitncmp(_key,str,len) ) {
		/* If I am not a substring of the _key */
		return (patricia*)last;
	} else if ( _bitlen(_key) == len ) {
		/* If I am exactly the _key */
		return (patricia*)this;
	} else {
		/* So, I am a substring of the _key.
		 * Recurrsively call on the suitable branch. */
		return (_extractbit(_key,len+1)?left:right)->_search(_key,this);
	};
};

/** @brief
 *   Given a Patricia tree, put the _data into the tree with the specified _key
 */
void patricia::add(char* _key, void* _data)
{
	/* In case of empty tree: Create and finish */
	if (str==NULL) {
		str = _key;
		data = _data;
		len = _bitlen(str);
		return;
	};
	/* In case we need to change the root node for addition */
	if (_bitncmp(str,_key,len)) {
		patricia* myself = new patricia(this);			/* Clone myself */
		if (_bitncmp(str,_key,_bitlen(str))) {
			/* New node is not a substring of the current root node */
			data = NULL;					/* Make root node a forking node */
			len = _getmatchlen(_key,str);
			if (_extractbit(_key,len+1)) {		/* put in two children to the root */
				left = new patricia(_key,_data);
				right = myself;
			} else {
				right = new patricia(_key,_data);
				left = myself;
			};
		} else {
			/* New node is a substring of the current root node */
			str = _key;				/* Update myself */
			data = _data;
			len = _bitlen(str);
			left = right = NULL;
			if (_extractbit(str,len+1)) {	/* put in two children to the root */
				left = myself;
			} else {
				right = myself;
			};
		};
		return;
	};
	/* In case of _key crash */
	if (_charcmp(str,_key)==0) {
		if (data != _data) {
			if (data) free(data);
			data = _data;
		};
		return;
	};
	/* In case the new node is my immediate child for sure */
	if (left==NULL && _extractbit(_key,len+1)) {
		left = new patricia(_key,_data);
		return;
	};
	if (right==NULL && _extractbit(_key,len+1)==0) {
		right = new patricia(_key,_data);
		return;
	};
	/* Otherwise, go down one level and call recurrsively */
	return (_extractbit(_key,len+1)?left:right)->add(_key,data);
};

/** @brief Given a Patricia tree, free the key string node->str
 *  @param node
 *     A pointer to a Patricia node for which it is going to be deleted and its
 *     key is going to be freed.
 */
void patricia::_freekey(patricia* node)
{
	if (this==NULL || node==NULL) return;
	if (this == node) {
		free(str);
		return;
	};
	if (str == node->str) {		/* Replace every reference to the string */
		str = ((str==left->str)?right:left)->str;
	};
	(_extractbit(node->str,len+1)?left:right)->_freekey(node);
};

/** @brief Make myself an exact duplicate of the specified Paticia node */
void patricia::_clone(const patricia* t)
{
	str = t->str;
	len = t->len;
	data = t->data;
	left = t->left;
	right = t->right;
};

/** @brief
 *    Given a Patricia tree, remove the node as indicated by the key.
 *  @param papa
 *    Indicates the parent node of the current node and it defaults to NULL.
 *    This is a variable used internally and callers should not set or use it.
 *  @param _key
 *    The _key that identifies the node to delete.
 */
void patricia::del(char* _key, patricia* papa)
{
	static patricia* root = this;		/* Remember who is the root node */
	if ( _bitncmp(_key,str,len) ) {
		return;		/* The _key doesn't exist in this tree */
	} else if ( _bitlen(_key) != len ) {
		/* my str is a substring of _key on first len bits */
		return (_extractbit(_key,len+1)?left:right)->del(_key,this);
	} else {	/* I am holding the _key exactly */
		if (left==NULL && right==NULL) {
			/* If I have no children... */
			if (data) free(data);	/* clear myself */
			root->_freekey(this);
			if (papa==NULL) {
				/* I am root node */
				str = NULL;
				return;
			} else if (_bitlen(papa->str) == papa->len) {
				/* My parent is a terminating node, unlink myself from my parent */
				if (papa->left == this) papa->left = NULL;
				else papa->right = NULL;
			} else {
				/* My parent is not a terminating node, replace my parent with sibling */
				patricia* sibling = (papa->left==this)? (papa->right) : (papa->left);
				papa->_clone(sibling);
				delete(sibling);
			};
			delete(this);
		} else if (left==NULL || right==NULL) {
			/* If I get only one children, lift it up to my position */
			if (data) free(data);
			root->_freekey(this);
			patricia* child = (left)?left:right;
			_clone(child);
			delete(child);
		} else {
			/* I have two children, make myself a non-terminating node */
			if (data) free(data);
			root->_freekey(this);
			str = left->str;
		};
		return;
	};
};

/* ------------------------------------------------------------------------ */
#ifdef UNITTEST

/* Check if a tree is well-formed */
int patricia::validate() const
{
	if (str==NULL) return 0;
	if (left) {
		if (_bitncmp(str,left->str,len)) {
			printf("Left child (%s for %d) not a substring of myself: %s (%d)\n",left->str, left->len, str,len);
			return 1;	/* Left child is not a substring of myself */
		} else if (_extractbit(left->str,len+1)==0) {
			printf("Right child located at left: %s (%d)\n",str,len);
			return 1;	/* Right (zero) child located at left */
		} else if (left->validate()) {
			return 1;	/* Check recurrsively on left branch */
		};
	};
	if (right) {
		if (_bitncmp(str,right->str,len)) {
			printf("Right child not a substring of myself: %s (%d)\n",str,len);
			return 1;	/* Right child is not a substring of myself */
		} else if (_extractbit(right->str,len+1)) {
			printf("Left child located at right: %s (%d)\n",str,len);
			return 1;	/* Left (one) child located at right */
		} else if (right->validate()) {
			return 1;	/* Check recurrsively on right branch */
		};
	};
	return 0;
};

/* Walk through the Patricia tree, and print in braces */
void patricia::printcompact() const
{
	if (str==NULL) return;
	if (_bitlen(str) == len) {
		printf("%s{",str);
	} else {
		printf("(%d){",len);
	};
	if (right) right->printcompact();
	printf("}{");
	if (left) left->printcompact();
	printf("}");
};

/* Walk through the Patricia tree, and print in a tree form */
void patricia::print(int n) const
{
	int i;
	if (str==NULL) return;
	for(i=0;i<n;i++) { printf(" "); };
	if (_bitlen(str) == len) {
		printf("%s\n",str);
	} else {
		printf("%s:(%d)\n",str,len);
	};
	if (right) right->print(n+4);
	if (left) left->print(n+4);
};

int main()
{
	char* s;
	patricia* tree=new patricia;
	assert(   0==tree->_bitncmp("\xFF\xFF","\xFF\xFF",16));
	assert(   2==tree->_bitncmp("\xFF\xFF","\xFF\xFD",16));
	assert(   0==tree->_bitncmp("\xFF\xFF","\xFF\xFE",15));
	assert(   0==tree->_bitncmp("\xFF\xFF","\xFF\xF0",12));
	assert(  12==tree->_bitncmp("\xFF\xFF","\xFF\xF1",14));
	assert(0x80==tree->_extractbit("\xA5\xA5",1));
	assert(0x00==tree->_extractbit("\xA5\xA5",2));
	assert(0x01==tree->_extractbit("\xA5\xA5",16));
	assert(0x00==tree->_extractbit("\xA5\xA5",15));
	assert(   0==tree->_getmatchlen("\x01\x01","\x81\x01"));
	assert(   1==tree->_getmatchlen("\x01\x01","\x71\x01"));
	assert(   4==tree->_getmatchlen("\x01\x01","\x09\x01"));
	assert(   8==tree->_getmatchlen("\x01\x01","\x01\xFF"));
	assert(  14==tree->_getmatchlen("\x01\x01","\x01\x02"));
	s = (char*)malloc(sizeof(char)*12); strcpy(s,"america");     tree->add(s,NULL); assert(0==tree->validate());
	s = (char*)malloc(sizeof(char)*12); strcpy(s,"asia");        tree->add(s,NULL); assert(0==tree->validate());
	s = (char*)malloc(sizeof(char)*12); strcpy(s,"asiapacific"); tree->add(s,NULL); assert(0==tree->validate());
	s = (char*)malloc(sizeof(char)*12); strcpy(s,"europe");      tree->add(s,NULL); assert(0==tree->validate());
	s = (char*)malloc(sizeof(char)*12); strcpy(s,"european");    tree->add(s,NULL); assert(0==tree->validate());
	s = (char*)malloc(sizeof(char)*12); strcpy(s,"africa");      tree->add(s,NULL); assert(0==tree->validate());
	s = (char*)malloc(sizeof(char)*12); strcpy(s,"australia");   tree->add(s,NULL); assert(0==tree->validate());
	s = (char*)malloc(sizeof(char)*12); strcpy(s,"europa");      tree->add(s,NULL); assert(0==tree->validate());
	s = (char*)malloc(sizeof(char)*12); strcpy(s,"antartica");   tree->add(s,NULL); assert(0==tree->validate());
	assert(0==strcmp("asia",tree->maxmatch("asian")->getkey()));
	tree->del("europa");      assert(0==tree->validate());
	tree->del("africa");      assert(0==tree->validate());
	tree->del("asian");       assert(0==tree->validate());
	tree->del("asia");        assert(0==tree->validate());
	tree->del("asiapacific"); assert(0==tree->validate());
	tree->del("america");     assert(0==tree->validate());
	tree->del("europe");      assert(0==tree->validate());
	tree->del("australia");   assert(0==tree->validate());
	tree->del("antartica");   assert(0==tree->validate());
	tree->del("european");    assert(0==tree->validate());
	printf("OK\n");
	for (int i=0; i<1e5; i++) {
		s = (char*)malloc(sizeof(char)*12); strcpy(s,"america");     tree->add(s,NULL); tree->validate();
		s = (char*)malloc(sizeof(char)*12); strcpy(s,"asia");        tree->add(s,NULL); tree->validate();
		s = (char*)malloc(sizeof(char)*12); strcpy(s,"asiapacific"); tree->add(s,NULL); tree->validate();
		s = (char*)malloc(sizeof(char)*12); strcpy(s,"europe");      tree->add(s,NULL); tree->validate();
		s = (char*)malloc(sizeof(char)*12); strcpy(s,"european");    tree->add(s,NULL); tree->validate();
		s = (char*)malloc(sizeof(char)*12); strcpy(s,"africa");      tree->add(s,NULL); tree->validate();
		s = (char*)malloc(sizeof(char)*12); strcpy(s,"australia");   tree->add(s,NULL); tree->validate();
		s = (char*)malloc(sizeof(char)*12); strcpy(s,"europa");      tree->add(s,NULL); tree->validate();
		s = (char*)malloc(sizeof(char)*12); strcpy(s,"antartica");   tree->add(s,NULL); tree->validate();
		tree->maxmatch("asian");
		tree->del("europa");      tree->validate();
		tree->del("africa");      tree->validate();
		tree->del("asian");       tree->validate();
		tree->del("asia");        tree->validate();
		tree->del("asiapacific"); tree->validate();
		tree->del("america");     tree->validate();
		tree->del("europe");      tree->validate();
		tree->del("australia");   tree->validate();
		tree->del("antartica");   tree->validate();
		tree->del("european");    tree->validate();
	};
	return 0;
};
#endif

