<<<<<<< HEAD
/*
 *  arch/s390/lib/string.c
 *    Optimized string functions
 *
 *  S390 version
 *    Copyright (C) 2004 IBM Deutschland Entwicklung GmbH, IBM Corporation
=======
// SPDX-License-Identifier: GPL-2.0
/*
 *    Optimized string functions
 *
 *  S390 version
 *    Copyright IBM Corp. 2004
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *    Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com)
 */

#define IN_ARCH_STRING_C 1
<<<<<<< HEAD

#include <linux/types.h>
#include <linux/module.h>
=======
#ifndef __NO_FORTIFY
# define __NO_FORTIFY
#endif

#include <linux/types.h>
#include <linux/string.h>
#include <linux/export.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Helper functions to find the end of a string
 */
static inline char *__strend(const char *s)
{
<<<<<<< HEAD
	register unsigned long r0 asm("0") = 0;

	asm volatile ("0: srst  %0,%1\n"
		      "   jo    0b"
		      : "+d" (r0), "+a" (s) :  : "cc" );
	return (char *) r0;
=======
	unsigned long e = 0;

	asm volatile(
		"	lghi	0,0\n"
		"0:	srst	%[e],%[s]\n"
		"	jo	0b\n"
		: [e] "+&a" (e), [s] "+&a" (s)
		:
		: "cc", "memory", "0");
	return (char *)e;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline char *__strnend(const char *s, size_t n)
{
<<<<<<< HEAD
	register unsigned long r0 asm("0") = 0;
	const char *p = s + n;

	asm volatile ("0: srst  %0,%1\n"
		      "   jo    0b"
		      : "+d" (p), "+a" (s) : "d" (r0) : "cc" );
	return (char *) p;
=======
	const char *p = s + n;

	asm volatile(
		"	lghi	0,0\n"
		"0:	srst	%[p],%[s]\n"
		"	jo	0b\n"
		: [p] "+&d" (p), [s] "+&a" (s)
		:
		: "cc", "memory", "0");
	return (char *)p;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * strlen - Find the length of a string
 * @s: The string to be sized
 *
 * returns the length of @s
 */
<<<<<<< HEAD
size_t strlen(const char *s)
{
#if __GNUC__ < 4
	return __strend(s) - s;
#else
	return __builtin_strlen(s);
#endif
}
EXPORT_SYMBOL(strlen);
=======
#ifdef __HAVE_ARCH_STRLEN
size_t strlen(const char *s)
{
	return __strend(s) - s;
}
EXPORT_SYMBOL(strlen);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * strnlen - Find the length of a length-limited string
 * @s: The string to be sized
 * @n: The maximum number of bytes to search
 *
 * returns the minimum of the length of @s and @n
 */
<<<<<<< HEAD
size_t strnlen(const char * s, size_t n)
=======
#ifdef __HAVE_ARCH_STRNLEN
size_t strnlen(const char *s, size_t n)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return __strnend(s, n) - s;
}
EXPORT_SYMBOL(strnlen);
<<<<<<< HEAD
=======
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * strcpy - Copy a %NUL terminated string
 * @dest: Where to copy the string to
 * @src: Where to copy the string from
 *
 * returns a pointer to @dest
 */
<<<<<<< HEAD
char *strcpy(char *dest, const char *src)
{
#if __GNUC__ < 4
	register int r0 asm("0") = 0;
	char *ret = dest;

	asm volatile ("0: mvst  %0,%1\n"
		      "   jo    0b"
		      : "+&a" (dest), "+&a" (src) : "d" (r0)
		      : "cc", "memory" );
	return ret;
#else
	return __builtin_strcpy(dest, src);
#endif
}
EXPORT_SYMBOL(strcpy);

/**
 * strlcpy - Copy a %NUL terminated string into a sized buffer
 * @dest: Where to copy the string to
 * @src: Where to copy the string from
 * @size: size of destination buffer
 *
 * Compatible with *BSD: the result is always a valid
 * NUL-terminated string that fits in the buffer (unless,
 * of course, the buffer size is zero). It does not pad
 * out the result like strncpy() does.
 */
size_t strlcpy(char *dest, const char *src, size_t size)
{
	size_t ret = __strend(src) - src;

	if (size) {
		size_t len = (ret >= size) ? size-1 : ret;
		dest[len] = '\0';
		__builtin_memcpy(dest, src, len);
	}
	return ret;
}
EXPORT_SYMBOL(strlcpy);
=======
#ifdef __HAVE_ARCH_STRCPY
char *strcpy(char *dest, const char *src)
{
	char *ret = dest;

	asm volatile(
		"	lghi	0,0\n"
		"0:	mvst	%[dest],%[src]\n"
		"	jo	0b\n"
		: [dest] "+&a" (dest), [src] "+&a" (src)
		:
		: "cc", "memory", "0");
	return ret;
}
EXPORT_SYMBOL(strcpy);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * strncpy - Copy a length-limited, %NUL-terminated string
 * @dest: Where to copy the string to
 * @src: Where to copy the string from
 * @n: The maximum number of bytes to copy
 *
 * The result is not %NUL-terminated if the source exceeds
 * @n bytes.
 */
<<<<<<< HEAD
char *strncpy(char *dest, const char *src, size_t n)
{
	size_t len = __strnend(src, n) - src;
	__builtin_memset(dest + len, 0, n - len);
	__builtin_memcpy(dest, src, len);
	return dest;
}
EXPORT_SYMBOL(strncpy);
=======
#ifdef __HAVE_ARCH_STRNCPY
char *strncpy(char *dest, const char *src, size_t n)
{
	size_t len = __strnend(src, n) - src;
	memset(dest + len, 0, n - len);
	memcpy(dest, src, len);
	return dest;
}
EXPORT_SYMBOL(strncpy);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * strcat - Append one %NUL-terminated string to another
 * @dest: The string to be appended to
 * @src: The string to append to it
 *
 * returns a pointer to @dest
 */
<<<<<<< HEAD
char *strcat(char *dest, const char *src)
{
	register int r0 asm("0") = 0;
	unsigned long dummy;
	char *ret = dest;

	asm volatile ("0: srst  %0,%1\n"
		      "   jo    0b\n"
		      "1: mvst  %0,%2\n"
		      "   jo    1b"
		      : "=&a" (dummy), "+a" (dest), "+a" (src)
		      : "d" (r0), "0" (0UL) : "cc", "memory" );
	return ret;
}
EXPORT_SYMBOL(strcat);
=======
#ifdef __HAVE_ARCH_STRCAT
char *strcat(char *dest, const char *src)
{
	unsigned long dummy = 0;
	char *ret = dest;

	asm volatile(
		"	lghi	0,0\n"
		"0:	srst	%[dummy],%[dest]\n"
		"	jo	0b\n"
		"1:	mvst	%[dummy],%[src]\n"
		"	jo	1b\n"
		: [dummy] "+&a" (dummy), [dest] "+&a" (dest), [src] "+&a" (src)
		:
		: "cc", "memory", "0");
	return ret;
}
EXPORT_SYMBOL(strcat);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * strlcat - Append a length-limited, %NUL-terminated string to another
 * @dest: The string to be appended to
 * @src: The string to append to it
 * @n: The size of the destination buffer.
 */
<<<<<<< HEAD
=======
#ifdef __HAVE_ARCH_STRLCAT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
size_t strlcat(char *dest, const char *src, size_t n)
{
	size_t dsize = __strend(dest) - dest;
	size_t len = __strend(src) - src;
	size_t res = dsize + len;

	if (dsize < n) {
		dest += dsize;
		n -= dsize;
		if (len >= n)
			len = n - 1;
		dest[len] = '\0';
<<<<<<< HEAD
		__builtin_memcpy(dest, src, len);
=======
		memcpy(dest, src, len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return res;
}
EXPORT_SYMBOL(strlcat);
<<<<<<< HEAD
=======
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * strncat - Append a length-limited, %NUL-terminated string to another
 * @dest: The string to be appended to
 * @src: The string to append to it
 * @n: The maximum numbers of bytes to copy
 *
 * returns a pointer to @dest
 *
 * Note that in contrast to strncpy, strncat ensures the result is
 * terminated.
 */
<<<<<<< HEAD
=======
#ifdef __HAVE_ARCH_STRNCAT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
char *strncat(char *dest, const char *src, size_t n)
{
	size_t len = __strnend(src, n) - src;
	char *p = __strend(dest);

	p[len] = '\0';
<<<<<<< HEAD
	__builtin_memcpy(p, src, len);
	return dest;
}
EXPORT_SYMBOL(strncat);

/**
 * strcmp - Compare two strings
 * @cs: One string
 * @ct: Another string
 *
 * returns   0 if @cs and @ct are equal,
 *         < 0 if @cs is less than @ct
 *         > 0 if @cs is greater than @ct
 */
int strcmp(const char *cs, const char *ct)
{
	register int r0 asm("0") = 0;
	int ret = 0;

	asm volatile ("0: clst %2,%3\n"
		      "   jo   0b\n"
		      "   je   1f\n"
		      "   ic   %0,0(%2)\n"
		      "   ic   %1,0(%3)\n"
		      "   sr   %0,%1\n"
		      "1:"
		      : "+d" (ret), "+d" (r0), "+a" (cs), "+a" (ct)
		      : : "cc" );
	return ret;
}
EXPORT_SYMBOL(strcmp);

/**
 * strrchr - Find the last occurrence of a character in a string
 * @s: The string to be searched
 * @c: The character to search for
 */
char * strrchr(const char * s, int c)
{
       size_t len = __strend(s) - s;

       if (len)
	       do {
		       if (s[len] == (char) c)
			       return (char *) s + len;
	       } while (--len > 0);
       return NULL;
}
EXPORT_SYMBOL(strrchr);
=======
	memcpy(p, src, len);
	return dest;
}
EXPORT_SYMBOL(strncat);
#endif

/**
 * strcmp - Compare two strings
 * @s1: One string
 * @s2: Another string
 *
 * returns   0 if @s1 and @s2 are equal,
 *	   < 0 if @s1 is less than @s2
 *	   > 0 if @s1 is greater than @s2
 */
#ifdef __HAVE_ARCH_STRCMP
int strcmp(const char *s1, const char *s2)
{
	int ret = 0;

	asm volatile(
		"	lghi	0,0\n"
		"0:	clst	%[s1],%[s2]\n"
		"	jo	0b\n"
		"	je	1f\n"
		"	ic	%[ret],0(%[s1])\n"
		"	ic	0,0(%[s2])\n"
		"	sr	%[ret],0\n"
		"1:"
		: [ret] "+&d" (ret), [s1] "+&a" (s1), [s2] "+&a" (s2)
		:
		: "cc", "memory", "0");
	return ret;
}
EXPORT_SYMBOL(strcmp);
#endif

static inline int clcle(const char *s1, unsigned long l1,
			const char *s2, unsigned long l2)
{
	union register_pair r1 = { .even = (unsigned long)s1, .odd = l1, };
	union register_pair r3 = { .even = (unsigned long)s2, .odd = l2, };
	int cc;

	asm volatile(
		"0:	clcle	%[r1],%[r3],0\n"
		"	jo	0b\n"
		"	ipm	%[cc]\n"
		"	srl	%[cc],28\n"
		: [cc] "=&d" (cc), [r1] "+&d" (r1.pair), [r3] "+&d" (r3.pair)
		:
		: "cc", "memory");
	return cc;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * strstr - Find the first substring in a %NUL terminated string
 * @s1: The string to be searched
 * @s2: The string to search for
 */
<<<<<<< HEAD
char * strstr(const char * s1,const char * s2)
=======
#ifdef __HAVE_ARCH_STRSTR
char *strstr(const char *s1, const char *s2)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int l1, l2;

	l2 = __strend(s2) - s2;
	if (!l2)
		return (char *) s1;
	l1 = __strend(s1) - s1;
	while (l1-- >= l2) {
<<<<<<< HEAD
		register unsigned long r2 asm("2") = (unsigned long) s1;
		register unsigned long r3 asm("3") = (unsigned long) l2;
		register unsigned long r4 asm("4") = (unsigned long) s2;
		register unsigned long r5 asm("5") = (unsigned long) l2;
		int cc;

		asm volatile ("0: clcle %1,%3,0\n"
			      "   jo    0b\n"
			      "   ipm   %0\n"
			      "   srl   %0,28"
			      : "=&d" (cc), "+a" (r2), "+a" (r3),
			        "+a" (r4), "+a" (r5) : : "cc" );
=======
		int cc;

		cc = clcle(s1, l2, s2, l2);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!cc)
			return (char *) s1;
		s1++;
	}
	return NULL;
}
EXPORT_SYMBOL(strstr);
<<<<<<< HEAD
=======
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * memchr - Find a character in an area of memory.
 * @s: The memory area
 * @c: The byte to search for
 * @n: The size of the area.
 *
 * returns the address of the first occurrence of @c, or %NULL
 * if @c is not found
 */
<<<<<<< HEAD
void *memchr(const void *s, int c, size_t n)
{
	register int r0 asm("0") = (char) c;
	const void *ret = s + n;

	asm volatile ("0: srst  %0,%1\n"
		      "   jo    0b\n"
		      "   jl	1f\n"
		      "   la    %0,0\n"
		      "1:"
		      : "+a" (ret), "+&a" (s) : "d" (r0) : "cc" );
	return (void *) ret;
}
EXPORT_SYMBOL(memchr);

/**
 * memcmp - Compare two areas of memory
 * @cs: One area of memory
 * @ct: Another area of memory
 * @count: The size of the area.
 */
int memcmp(const void *cs, const void *ct, size_t n)
{
	register unsigned long r2 asm("2") = (unsigned long) cs;
	register unsigned long r3 asm("3") = (unsigned long) n;
	register unsigned long r4 asm("4") = (unsigned long) ct;
	register unsigned long r5 asm("5") = (unsigned long) n;
	int ret;

	asm volatile ("0: clcle %1,%3,0\n"
		      "   jo    0b\n"
		      "   ipm   %0\n"
		      "   srl   %0,28"
		      : "=&d" (ret), "+a" (r2), "+a" (r3), "+a" (r4), "+a" (r5)
		      : : "cc" );
	if (ret)
		ret = *(char *) r2 - *(char *) r4;
	return ret;
}
EXPORT_SYMBOL(memcmp);
=======
#ifdef __HAVE_ARCH_MEMCHR
void *memchr(const void *s, int c, size_t n)
{
	const void *ret = s + n;

	asm volatile(
		"	lgr	0,%[c]\n"
		"0:	srst	%[ret],%[s]\n"
		"	jo	0b\n"
		"	jl	1f\n"
		"	la	%[ret],0\n"
		"1:"
		: [ret] "+&a" (ret), [s] "+&a" (s)
		: [c] "d" (c)
		: "cc", "memory", "0");
	return (void *) ret;
}
EXPORT_SYMBOL(memchr);
#endif

/**
 * memcmp - Compare two areas of memory
 * @s1: One area of memory
 * @s2: Another area of memory
 * @n: The size of the area.
 */
#ifdef __HAVE_ARCH_MEMCMP
int memcmp(const void *s1, const void *s2, size_t n)
{
	int ret;

	ret = clcle(s1, n, s2, n);
	if (ret)
		ret = ret == 1 ? -1 : 1;
	return ret;
}
EXPORT_SYMBOL(memcmp);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * memscan - Find a character in an area of memory.
 * @s: The memory area
 * @c: The byte to search for
 * @n: The size of the area.
 *
 * returns the address of the first occurrence of @c, or 1 byte past
 * the area if @c is not found
 */
<<<<<<< HEAD
void *memscan(void *s, int c, size_t n)
{
	register int r0 asm("0") = (char) c;
	const void *ret = s + n;

	asm volatile ("0: srst  %0,%1\n"
		      "   jo    0b\n"
		      : "+a" (ret), "+&a" (s) : "d" (r0) : "cc" );
	return (void *) ret;
}
EXPORT_SYMBOL(memscan);

/**
 * memcpy - Copy one area of memory to another
 * @dest: Where to copy to
 * @src: Where to copy from
 * @n: The size of the area.
 *
 * returns a pointer to @dest
 */
void *memcpy(void *dest, const void *src, size_t n)
{
	return __builtin_memcpy(dest, src, n);
}
EXPORT_SYMBOL(memcpy);

/**
 * memset - Fill a region of memory with the given value
 * @s: Pointer to the start of the area.
 * @c: The byte to fill the area with
 * @n: The size of the area.
 *
 * returns a pointer to @s
 */
void *memset(void *s, int c, size_t n)
{
	char *xs;

	if (c == 0)
		return __builtin_memset(s, 0, n);

	xs = (char *) s;
	if (n > 0)
		do {
			*xs++ = c;
		} while (--n > 0);
	return s;
}
EXPORT_SYMBOL(memset);
=======
#ifdef __HAVE_ARCH_MEMSCAN
void *memscan(void *s, int c, size_t n)
{
	const void *ret = s + n;

	asm volatile(
		"	lgr	0,%[c]\n"
		"0:	srst	%[ret],%[s]\n"
		"	jo	0b\n"
		: [ret] "+&a" (ret), [s] "+&a" (s)
		: [c] "d" (c)
		: "cc", "memory", "0");
	return (void *)ret;
}
EXPORT_SYMBOL(memscan);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
