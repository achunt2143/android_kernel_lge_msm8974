<<<<<<< HEAD
/*
 *  include/asm-s390/string.h
 *
 *  S390 version
 *    Copyright (C) 1999 IBM Deutschland Entwicklung GmbH, IBM Corporation
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 *  S390 version
 *    Copyright IBM Corp. 1999
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *    Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com),
 */

#ifndef _S390_STRING_H_
#define _S390_STRING_H_

<<<<<<< HEAD
#ifdef __KERNEL__

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _LINUX_TYPES_H
#include <linux/types.h>
#endif

<<<<<<< HEAD
#define __HAVE_ARCH_MEMCHR	/* inline & arch function */
#define __HAVE_ARCH_MEMCMP	/* arch function */
#define __HAVE_ARCH_MEMCPY	/* gcc builtin & arch function */
#define __HAVE_ARCH_MEMSCAN	/* inline & arch function */
#define __HAVE_ARCH_MEMSET	/* gcc builtin & arch function */
=======
#define __HAVE_ARCH_MEMCPY	/* gcc builtin & arch function */
#define __HAVE_ARCH_MEMMOVE	/* gcc builtin & arch function */
#define __HAVE_ARCH_MEMSET	/* gcc builtin & arch function */
#define __HAVE_ARCH_MEMSET16	/* arch function */
#define __HAVE_ARCH_MEMSET32	/* arch function */
#define __HAVE_ARCH_MEMSET64	/* arch function */

void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
void *memmove(void *dest, const void *src, size_t n);

#ifndef CONFIG_KASAN
#define __HAVE_ARCH_MEMCHR	/* inline & arch function */
#define __HAVE_ARCH_MEMCMP	/* arch function */
#define __HAVE_ARCH_MEMSCAN	/* inline & arch function */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define __HAVE_ARCH_STRCAT	/* inline & arch function */
#define __HAVE_ARCH_STRCMP	/* arch function */
#define __HAVE_ARCH_STRCPY	/* inline & arch function */
#define __HAVE_ARCH_STRLCAT	/* arch function */
<<<<<<< HEAD
#define __HAVE_ARCH_STRLCPY	/* arch function */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define __HAVE_ARCH_STRLEN	/* inline & arch function */
#define __HAVE_ARCH_STRNCAT	/* arch function */
#define __HAVE_ARCH_STRNCPY	/* arch function */
#define __HAVE_ARCH_STRNLEN	/* inline & arch function */
<<<<<<< HEAD
#define __HAVE_ARCH_STRRCHR	/* arch function */
#define __HAVE_ARCH_STRSTR	/* arch function */

/* Prototypes for non-inlined arch strings functions. */
extern int memcmp(const void *, const void *, size_t);
extern void *memcpy(void *, const void *, size_t);
extern void *memset(void *, int, size_t);
extern int strcmp(const char *,const char *);
extern size_t strlcat(char *, const char *, size_t);
extern size_t strlcpy(char *, const char *, size_t);
extern char *strncat(char *, const char *, size_t);
extern char *strncpy(char *, const char *, size_t);
extern char *strrchr(const char *, int);
extern char *strstr(const char *, const char *);

#undef __HAVE_ARCH_MEMMOVE
#undef __HAVE_ARCH_STRCHR
#undef __HAVE_ARCH_STRNCHR
#undef __HAVE_ARCH_STRNCMP
#undef __HAVE_ARCH_STRNICMP
=======
#define __HAVE_ARCH_STRSTR	/* arch function */

/* Prototypes for non-inlined arch strings functions. */
int memcmp(const void *s1, const void *s2, size_t n);
int strcmp(const char *s1, const char *s2);
size_t strlcat(char *dest, const char *src, size_t n);
char *strncat(char *dest, const char *src, size_t n);
char *strncpy(char *dest, const char *src, size_t n);
char *strstr(const char *s1, const char *s2);
#endif /* !CONFIG_KASAN */

#undef __HAVE_ARCH_STRCHR
#undef __HAVE_ARCH_STRNCHR
#undef __HAVE_ARCH_STRNCMP
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#undef __HAVE_ARCH_STRPBRK
#undef __HAVE_ARCH_STRSEP
#undef __HAVE_ARCH_STRSPN

<<<<<<< HEAD
#if !defined(IN_ARCH_STRING_C)

static inline void *memchr(const void * s, int c, size_t n)
{
	register int r0 asm("0") = (char) c;
	const void *ret = s + n;

	asm volatile(
		"0:	srst	%0,%1\n"
		"	jo	0b\n"
		"	jl	1f\n"
		"	la	%0,0\n"
		"1:"
		: "+a" (ret), "+&a" (s) : "d" (r0) : "cc");
	return (void *) ret;
}

static inline void *memscan(void *s, int c, size_t n)
{
	register int r0 asm("0") = (char) c;
	const void *ret = s + n;

	asm volatile(
		"0:	srst	%0,%1\n"
		"	jo	0b\n"
		: "+a" (ret), "+&a" (s) : "d" (r0) : "cc");
	return (void *) ret;
}

static inline char *strcat(char *dst, const char *src)
{
	register int r0 asm("0") = 0;
	unsigned long dummy;
	char *ret = dst;

	asm volatile(
		"0:	srst	%0,%1\n"
		"	jo	0b\n"
		"1:	mvst	%0,%2\n"
		"	jo	1b"
		: "=&a" (dummy), "+a" (dst), "+a" (src)
		: "d" (r0), "0" (0) : "cc", "memory" );
	return ret;
}

static inline char *strcpy(char *dst, const char *src)
{
#if __GNUC__ < 4
	register int r0 asm("0") = 0;
	char *ret = dst;

	asm volatile(
		"0:	mvst	%0,%1\n"
		"	jo	0b"
		: "+&a" (dst), "+&a" (src) : "d" (r0)
		: "cc", "memory");
	return ret;
#else
	return __builtin_strcpy(dst, src);
#endif
}

static inline size_t strlen(const char *s)
{
#if __GNUC__ < 4
	register unsigned long r0 asm("0") = 0;
	const char *tmp = s;

	asm volatile(
		"0:	srst	%0,%1\n"
		"	jo	0b"
		: "+d" (r0), "+a" (tmp) :  : "cc");
	return r0 - (unsigned long) s;
#else
	return __builtin_strlen(s);
#endif
}

static inline size_t strnlen(const char * s, size_t n)
{
	register int r0 asm("0") = 0;
=======
#if defined(CONFIG_KASAN) && !defined(__SANITIZE_ADDRESS__)

#define strlen(s) __strlen(s)

#define __no_sanitize_prefix_strfunc(x) __##x

#ifndef __NO_FORTIFY
#define __NO_FORTIFY /* FORTIFY_SOURCE uses __builtin_memcpy, etc. */
#endif

#else
#define __no_sanitize_prefix_strfunc(x) x
#endif /* defined(CONFIG_KASAN) && !defined(__SANITIZE_ADDRESS__) */

void *__memcpy(void *dest, const void *src, size_t n);
void *__memset(void *s, int c, size_t n);
void *__memmove(void *dest, const void *src, size_t n);
void *__memset16(uint16_t *s, uint16_t v, size_t count);
void *__memset32(uint32_t *s, uint32_t v, size_t count);
void *__memset64(uint64_t *s, uint64_t v, size_t count);

static inline void *memset16(uint16_t *s, uint16_t v, size_t count)
{
	return __memset16(s, v, count * sizeof(v));
}

static inline void *memset32(uint32_t *s, uint32_t v, size_t count)
{
	return __memset32(s, v, count * sizeof(v));
}

static inline void *memset64(uint64_t *s, uint64_t v, size_t count)
{
	return __memset64(s, v, count * sizeof(v));
}

#if !defined(IN_ARCH_STRING_C) && (!defined(CONFIG_FORTIFY_SOURCE) || defined(__NO_FORTIFY))

#ifdef __HAVE_ARCH_MEMCHR
static inline void *memchr(const void * s, int c, size_t n)
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
#endif

#ifdef __HAVE_ARCH_MEMSCAN
static inline void *memscan(void *s, int c, size_t n)
{
	const void *ret = s + n;

	asm volatile(
		"	lgr	0,%[c]\n"
		"0:	srst	%[ret],%[s]\n"
		"	jo	0b\n"
		: [ret] "+&a" (ret), [s] "+&a" (s)
		: [c] "d" (c)
		: "cc", "memory", "0");
	return (void *) ret;
}
#endif

#ifdef __HAVE_ARCH_STRCAT
static inline char *strcat(char *dst, const char *src)
{
	unsigned long dummy = 0;
	char *ret = dst;

	asm volatile(
		"	lghi	0,0\n"
		"0:	srst	%[dummy],%[dst]\n"
		"	jo	0b\n"
		"1:	mvst	%[dummy],%[src]\n"
		"	jo	1b"
		: [dummy] "+&a" (dummy), [dst] "+&a" (dst), [src] "+&a" (src)
		:
		: "cc", "memory", "0");
	return ret;
}
#endif

#ifdef __HAVE_ARCH_STRCPY
static inline char *strcpy(char *dst, const char *src)
{
	char *ret = dst;

	asm volatile(
		"	lghi	0,0\n"
		"0:	mvst	%[dst],%[src]\n"
		"	jo	0b"
		: [dst] "+&a" (dst), [src] "+&a" (src)
		:
		: "cc", "memory", "0");
	return ret;
}
#endif

#if defined(__HAVE_ARCH_STRLEN) || (defined(CONFIG_KASAN) && !defined(__SANITIZE_ADDRESS__))
static inline size_t __no_sanitize_prefix_strfunc(strlen)(const char *s)
{
	unsigned long end = 0;
	const char *tmp = s;

	asm volatile(
		"	lghi	0,0\n"
		"0:	srst	%[end],%[tmp]\n"
		"	jo	0b"
		: [end] "+&a" (end), [tmp] "+&a" (tmp)
		:
		: "cc", "memory", "0");
	return end - (unsigned long)s;
}
#endif

#ifdef __HAVE_ARCH_STRNLEN
static inline size_t strnlen(const char * s, size_t n)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	const char *tmp = s;
	const char *end = s + n;

	asm volatile(
<<<<<<< HEAD
		"0:	srst	%0,%1\n"
		"	jo	0b"
		: "+a" (end), "+a" (tmp) : "d" (r0)  : "cc");
	return end - s;
}
=======
		"	lghi	0,0\n"
		"0:	srst	%[end],%[tmp]\n"
		"	jo	0b"
		: [end] "+&a" (end), [tmp] "+&a" (tmp)
		:
		: "cc", "memory", "0");
	return end - s;
}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else /* IN_ARCH_STRING_C */
void *memchr(const void * s, int c, size_t n);
void *memscan(void *s, int c, size_t n);
char *strcat(char *dst, const char *src);
char *strcpy(char *dst, const char *src);
size_t strlen(const char *s);
size_t strnlen(const char * s, size_t n);
#endif /* !IN_ARCH_STRING_C */

<<<<<<< HEAD
#endif /* __KERNEL__ */

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __S390_STRING_H_ */
