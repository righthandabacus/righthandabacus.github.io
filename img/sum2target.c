/* sum2target.c - Thu, 23 Feb 2012 00:13:00 -0500
 *
 * Given an array of sorted integers, find four elements of them that can sum up to S
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
#define _debug(a,b) debug(a,b)
#define _printf(...) printf(__VA_ARGS__)
#else
#define _debug(a,b)
#define _printf(...)
#endif

int CURSORCOUNT;
/* Sum of the elements in a that is pointed by the cursors */
int sum(int* a, int* cursor, int numCursor)
{
	int i;
	int s = 0;
	for (i = 0; i<numCursor; ++i) {
		s += a[cursor[i]];
	};
	return s;
}

void debug(int* a, int* cursor)
{
	int i,s;
	s=0;
	for (i=0; i<CURSORCOUNT-1; ++i) {
		printf("%d+",a[cursor[i]]);
		s += a[cursor[i]];
	}
	s += a[cursor[CURSORCOUNT-1]];
	printf("%d = %d\n", a[cursor[CURSORCOUNT-1]], s);
}

/* Binary search from a[begin] to a[end], inclusive, for target */
int search(int* a, int begin, int end, int target)
{
	int i;
	/* Sanity check goes first */
	if (begin > end) return -255;
	/* Simple case: just one item */
	if (begin == end) return (a[begin] == target)? begin : -1;
	/* Recursive search */
	i = (begin+end)/2;
	if (a[i] == target) {
		return i;
	} else if (a[i] > target) {
		return search(a, begin, i-1, target);
	} else /* if (a[i] < target) */ {
		return search(a, i+1, end, target);
	}
}

/* Recursive binary search from a[0] to a[n], inclusive, for k items sum up to target */
int recurSearch(int* a, int n, int* cursor, int k, int target)
{ 
	int s, ret;
	int myMin, myMax, i;
	char* bitmap;
	/* Sanity checks */
	if (n < 0) return -255;   /* array must of positive length */
	if (n < k-1) return -255; /* array length must not shorter than cursor count */
	/* Terminal case: Only one cursor provided, reduced to simple binary search */
	if (k == 1) {
		ret = search(a, 0, n, target);
		if (ret >= 0 && ret <= n) {
			cursor[0] = ret;
			return 0; /* Solution found */
		} else {
			return -1; /* Solution not found */
		}
	}
	/* General case: More than one cursor */
	ret = -1; /* Assume solution not found */
	bitmap = (char*)malloc(n+1); /* create bitmap */
	bzero(bitmap, n+1); /* initialize bitmap */
	for (i=0; i<k; ++i) cursor[i]=i; /* initialize cursors */
	s = sum(a, cursor, k);
	if (s == target) {
		ret = 0; /* Too easy: Solution found */
	} else if (s < target && n > k-1) {
		/* Some cursor have to move up */
		s = sum(a, cursor, k-1);
		myMin = k;
		myMax = n;
		/* Loop-based binary search for the marginal position between too big and too small */
		_printf("Binary search started on cursor %d\n",k-1);
		while (myMin <= myMax) {
			cursor[k-1] = (myMin + myMax)/2;
			_debug(a, cursor);
			/* The easy edge cases */
			if (a[cursor[k-1]] > target - s) { /* too big to be a solution */
				myMax = cursor[k-1]-1;
				continue;
			} else if (a[cursor[k-1]] == target - s) { /* Solution found by just moving the last cursor */
				ret = 0;
				break;
			};
			/* The case to recur */
			myMin = cursor[k-1] + 1; /* Next try have to move up the lowerbound */
			bitmap[cursor[k-1]] = 1; /* Mark this position in the bitmap for future reference */
			ret = recurSearch(a, cursor[k-1]-1, cursor, k-1, target-a[cursor[k-1]]);
			if (ret == 0) { /* solution found in recursive search */
				break;
			};
		};
		_printf("Binary search ended on cursor %d\n",k-1);
		/* Perform linear search if solution is not yet found */
		if (sum(a, cursor, k) != target) {
			_printf("Linear search started on cursor %d\n",k-1);
			for (; cursor[k-1] >= k; --cursor[k-1]) {
				if (bitmap[cursor[k-1]] == 1) continue; /* Skip those already examined */
				_debug(a, cursor);
				ret = recurSearch(a, cursor[k-1]-1, cursor, k-1, target-a[cursor[k-1]]);
				if (ret == 0) break; /* solution found in recursive search */
			};
			_printf("Linear search ended on cursor %d\n",k-1);
		};
	};
	free(bitmap);
	return ret;
}

/* Print array */
void UT_printarray(int* a, int N)
{
	int i;
	printf("[");
	for (i=0; i<N-1; ++i) {
		printf("%d, ", a[i]);
	}
	printf("%d]", a[N-1]);
}
/* Print solutions */
void UT_printsol(int* a, int* cursor, int k)
{
	int i;
	for (i=0; i<k-1; ++i) {
		printf("%d+", a[cursor[i]]);
	}
	printf("%d", a[cursor[k-1]]);
}

/* Unit test: Fixed array, fixed number of cursors */
int unittest1()
{
	int a[] = {0,8,12,13,16,23,26,30,37,39,55,59,61,62,63,70,89};
	int N = 17;
	int S = 8+23+59+70;
	int four = 4;
	int cursor[four];
	int ret;
	/* Recursive binary search */
	ret = recurSearch(a, N-1, cursor, four, S);
	printf("Array: "); UT_printarray(a, N); printf("\n");
	if (ret == 0) {
		printf("Solution found: "); UT_printsol(a, cursor, four); printf(" = %d\n", S);
		return 0;
	} else {
		printf("No solution found.\n");
		return -1;
	};
}

/* Helper function for qsort() call */
int qsortcmp(const void*a, const void*b)
{
	return (*(int*)a - *(int*)b);
}

/* Tell if any item in the sorted cursor is repeated */
int nonUniq(int* cursor, int cSize)
{
	int i;
	for (i = 1; i < cSize; ++i) {
		if (cursor[i] == cursor[i-1]) return i;
	};
	return 0;
}


/* Unit test: Random array, random number of cursors */
int unittest2()
{
	int aSize = 1 + (random() % 50);  /* array */
	int *a = (int*)malloc(aSize * sizeof(int));
	int cSize = 1 + (random() % aSize); /* cursors */
	int *cursor = (int*)malloc((cSize+2) * sizeof(int));
	int i, j, S, ret;
	/* Fill in the array, and sort it */
	for (i=0; i<aSize; ++i) {
		a[i] = random() % 100;
	}
	qsort(a, aSize, sizeof(int), qsortcmp);
	/* Fill in the cursor */
	for (i=0; i<cSize; ++i) {
		cursor[i] = random() % aSize;
	};
	qsort(cursor, cSize, sizeof(int), qsortcmp);
	/* Ensure the cursors are distinct */
	for (;;) {
		i = nonUniq(cursor, cSize);
		if (i == 0) break;
		cursor[i] = random() % aSize;
		qsort(cursor, cSize, sizeof(int), qsortcmp);
	};
	/* Compute the target */
	S = sum(a, cursor, cSize);
	/* Print the target */
	printf("Array[%d]: ", aSize); UT_printarray(a, aSize); printf("\n");
	printf("Goal[%d]: ", cSize); UT_printsol(a, cursor, cSize); printf(" = %d\n", S);
	/* Reset the cursor */
	j = (aSize-cSize)/2;
	cursor[0] = -1;
	for (i=1; i<=cSize; cursor[i++] = j++);
	cursor[cSize+1] = aSize;
	/* Recursive binary search */
	CURSORCOUNT = cSize;
	ret = recurSearch(a, aSize-1, cursor, cSize, S);
	if (ret == 0) {
		printf("Solution found: "); UT_printsol(a, cursor, cSize); printf(" = %d\n", S);
		return 0;
	} else {
		printf("No solution found.\n");
		return -1;
	};
	free(a);
	free(cursor);
}

int main(int argc, char** argv)
{
	int n, N;
	if (argc == 2) {
		N = atoi(argv[1]);
	} else {
		N = 10;
	};

	printf("===== Unit test 1 =====\n");
	if (unittest1()) return -1;

	/*srandomdev();*/ /* Seed random generator by hardware entropy */
	srandom(1);
	for (n = 1; n <= N; ++n) {
		printf("===== Unit test 2 : trial %d =====\n", n);
		if (unittest2()) return -1;
	}
	return 0;
}
