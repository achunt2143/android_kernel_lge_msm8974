<<<<<<< HEAD
/*
 * lz4defs.h -- architecture specific defines
 *
 * Copyright (C) 2013, LG Electronics, Kyungsik Lee <kyungsik.lee@lge.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/*
 * Detects 64 bits mode
 */
#if (defined(__x86_64__) || defined(__x86_64) || defined(__amd64__) \
	|| defined(__ppc64__) || defined(__LP64__))
=======
#ifndef __LZ4DEFS_H__
#define __LZ4DEFS_H__

/*
 * lz4defs.h -- common and architecture specific defines for the kernel usage

 * LZ4 - Fast LZ compression algorithm
 * Copyright (C) 2011-2016, Yann Collet.
 * BSD 2-Clause License (http://www.opensource.org/licenses/bsd-license.php)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *	* Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer.
 *	* Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * You can contact the author at :
 *	- LZ4 homepage : http://www.lz4.org
 *	- LZ4 source repository : https://github.com/lz4/lz4
 *
 *	Changed for kernel usage by:
 *	Sven Schmidt <4sschmid@informatik.uni-hamburg.de>
 */

#include <asm/unaligned.h>

#include <linux/bitops.h>
#include <linux/string.h>	 /* memset, memcpy */

#define FORCE_INLINE __always_inline

/*-************************************
 *	Basic Types
 **************************************/
#include <linux/types.h>

typedef	uint8_t BYTE;
typedef uint16_t U16;
typedef uint32_t U32;
typedef	int32_t S32;
typedef uint64_t U64;
typedef uintptr_t uptrval;

/*-************************************
 *	Architecture specifics
 **************************************/
#if defined(CONFIG_64BIT)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define LZ4_ARCH64 1
#else
#define LZ4_ARCH64 0
#endif

<<<<<<< HEAD
/*
 * Architecture-specific macros
 */
#define ARM_EFFICIENT_UNALIGNED_ACCESS
#define BYTE	u8
typedef struct _U16_S { u16 v; } U16_S;
typedef struct _U32_S { u32 v; } U32_S;
typedef struct _U64_S { u64 v; } U64_S;
#if defined(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS)		\
	|| defined(CONFIG_ARM) && __LINUX_ARM_ARCH__ >= 6	\
	&& defined(ARM_EFFICIENT_UNALIGNED_ACCESS)

#define A16(x) (((U16_S *)(x))->v)
#define A32(x) (((U32_S *)(x))->v)
#define A64(x) (((U64_S *)(x))->v)

#define PUT4(s, d) (A32(d) = A32(s))
#define PUT8(s, d) (A64(d) = A64(s))
#define LZ4_WRITE_LITTLEENDIAN_16(p, v)	\
	do {	\
		A16(p) = v; \
		p += 2; \
	} while (0)
#else /* CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS */

#define A64(x) get_unaligned((u64 *)&(((U16_S *)(x))->v))
#define A32(x) get_unaligned((u32 *)&(((U16_S *)(x))->v))
#define A16(x) get_unaligned((u16 *)&(((U16_S *)(x))->v))

#define PUT4(s, d) \
	put_unaligned(get_unaligned((const u32 *) s), (u32 *) d)
#define PUT8(s, d) \
	put_unaligned(get_unaligned((const u64 *) s), (u64 *) d)

#define LZ4_WRITE_LITTLEENDIAN_16(p, v)	\
	do {	\
		put_unaligned(v, (u16 *)(p)); \
		p += 2; \
	} while (0)
#endif

#define COPYLENGTH 8
#define ML_BITS  4
#define ML_MASK  ((1U << ML_BITS) - 1)
#define RUN_BITS (8 - ML_BITS)
#define RUN_MASK ((1U << RUN_BITS) - 1)
#define MEMORY_USAGE	14
#define MINMATCH	4
#define SKIPSTRENGTH	6
#define LASTLITERALS	5
#define MFLIMIT		(COPYLENGTH + MINMATCH)
#define MINLENGTH	(MFLIMIT + 1)
#define MAXD_LOG	16
#define MAXD		(1 << MAXD_LOG)
#define MAXD_MASK	(u32)(MAXD - 1)
#define MAX_DISTANCE	(MAXD - 1)
#define HASH_LOG	(MAXD_LOG - 1)
#define HASHTABLESIZE	(1 << HASH_LOG)
#define MAX_NB_ATTEMPTS	256
#define OPTIMAL_ML	(int)((ML_MASK-1)+MINMATCH)
#define LZ4_64KLIMIT	((1<<16) + (MFLIMIT - 1))
#define HASHLOG64K	((MEMORY_USAGE - 2) + 1)
#define HASH64KTABLESIZE	(1U << HASHLOG64K)
#define LZ4_HASH_VALUE(p)	(((A32(p)) * 2654435761U) >> \
				((MINMATCH * 8) - (MEMORY_USAGE-2)))
#define LZ4_HASH64K_VALUE(p)	(((A32(p)) * 2654435761U) >> \
				((MINMATCH * 8) - HASHLOG64K))
#define HASH_VALUE(p)		(((A32(p)) * 2654435761U) >> \
				((MINMATCH * 8) - HASH_LOG))

#if LZ4_ARCH64/* 64-bit */
#define STEPSIZE 8

#define LZ4_COPYSTEP(s, d)	\
	do {			\
		PUT8(s, d);	\
		d += 8;		\
		s += 8;		\
	} while (0)

#define LZ4_COPYPACKET(s, d)	LZ4_COPYSTEP(s, d)

#define LZ4_SECURECOPY(s, d, e)			\
	do {					\
		if (d < e) {			\
			LZ4_WILDCOPY(s, d, e);	\
		}				\
	} while (0)
#define HTYPE u32

#ifdef __BIG_ENDIAN
#define LZ4_NBCOMMONBYTES(val) (__builtin_clzll(val) >> 3)
#else
#define LZ4_NBCOMMONBYTES(val) (__builtin_ctzll(val) >> 3)
#endif

#else	/* 32-bit */
#define STEPSIZE 4

#define LZ4_COPYSTEP(s, d)	\
	do {			\
		PUT4(s, d);	\
		d += 4;		\
		s += 4;		\
	} while (0)

#define LZ4_COPYPACKET(s, d)		\
	do {				\
		LZ4_COPYSTEP(s, d);	\
		LZ4_COPYSTEP(s, d);	\
	} while (0)

#define LZ4_SECURECOPY	LZ4_WILDCOPY
#define HTYPE const u8*

#ifdef __BIG_ENDIAN
#define LZ4_NBCOMMONBYTES(val) (__builtin_clz(val) >> 3)
#else
#define LZ4_NBCOMMONBYTES(val) (__builtin_ctz(val) >> 3)
#endif

#endif

#define LZ4_READ_LITTLEENDIAN_16(d, s, p) \
	(d = s - get_unaligned_le16(p))

#define LZ4_WILDCOPY(s, d, e)		\
	do {				\
		LZ4_COPYPACKET(s, d);	\
	} while (d < e)

#define LZ4_BLINDCOPY(s, d, l)	\
	do {	\
		u8 *e = (d) + l;	\
		LZ4_WILDCOPY(s, d, e);	\
		d = e;	\
	} while (0)
=======
#if defined(__LITTLE_ENDIAN)
#define LZ4_LITTLE_ENDIAN 1
#else
#define LZ4_LITTLE_ENDIAN 0
#endif

/*-************************************
 *	Constants
 **************************************/
#define MINMATCH 4

#define WILDCOPYLENGTH 8
#define LASTLITERALS 5
#define MFLIMIT (WILDCOPYLENGTH + MINMATCH)
/*
 * ensure it's possible to write 2 x wildcopyLength
 * without overflowing output buffer
 */
#define MATCH_SAFEGUARD_DISTANCE  ((2 * WILDCOPYLENGTH) - MINMATCH)

/* Increase this value ==> compression run slower on incompressible data */
#define LZ4_SKIPTRIGGER 6

#define HASH_UNIT sizeof(size_t)

#define KB (1 << 10)
#define MB (1 << 20)
#define GB (1U << 30)

#define MAXD_LOG 16
#define MAX_DISTANCE ((1 << MAXD_LOG) - 1)
#define STEPSIZE sizeof(size_t)

#define ML_BITS	4
#define ML_MASK	((1U << ML_BITS) - 1)
#define RUN_BITS (8 - ML_BITS)
#define RUN_MASK ((1U << RUN_BITS) - 1)

/*-************************************
 *	Reading and writing into memory
 **************************************/
static FORCE_INLINE U16 LZ4_read16(const void *ptr)
{
	return get_unaligned((const U16 *)ptr);
}

static FORCE_INLINE U32 LZ4_read32(const void *ptr)
{
	return get_unaligned((const U32 *)ptr);
}

static FORCE_INLINE size_t LZ4_read_ARCH(const void *ptr)
{
	return get_unaligned((const size_t *)ptr);
}

static FORCE_INLINE void LZ4_write16(void *memPtr, U16 value)
{
	put_unaligned(value, (U16 *)memPtr);
}

static FORCE_INLINE void LZ4_write32(void *memPtr, U32 value)
{
	put_unaligned(value, (U32 *)memPtr);
}

static FORCE_INLINE U16 LZ4_readLE16(const void *memPtr)
{
	return get_unaligned_le16(memPtr);
}

static FORCE_INLINE void LZ4_writeLE16(void *memPtr, U16 value)
{
	return put_unaligned_le16(value, memPtr);
}

/*
 * LZ4 relies on memcpy with a constant size being inlined. In freestanding
 * environments, the compiler can't assume the implementation of memcpy() is
 * standard compliant, so apply its specialized memcpy() inlining logic. When
 * possible, use __builtin_memcpy() to tell the compiler to analyze memcpy()
 * as-if it were standard compliant, so it can inline it in freestanding
 * environments. This is needed when decompressing the Linux Kernel, for example.
 */
#define LZ4_memcpy(dst, src, size) __builtin_memcpy(dst, src, size)
#define LZ4_memmove(dst, src, size) __builtin_memmove(dst, src, size)

static FORCE_INLINE void LZ4_copy8(void *dst, const void *src)
{
#if LZ4_ARCH64
	U64 a = get_unaligned((const U64 *)src);

	put_unaligned(a, (U64 *)dst);
#else
	U32 a = get_unaligned((const U32 *)src);
	U32 b = get_unaligned((const U32 *)src + 1);

	put_unaligned(a, (U32 *)dst);
	put_unaligned(b, (U32 *)dst + 1);
#endif
}

/*
 * customized variant of memcpy,
 * which can overwrite up to 7 bytes beyond dstEnd
 */
static FORCE_INLINE void LZ4_wildCopy(void *dstPtr,
	const void *srcPtr, void *dstEnd)
{
	BYTE *d = (BYTE *)dstPtr;
	const BYTE *s = (const BYTE *)srcPtr;
	BYTE *const e = (BYTE *)dstEnd;

	do {
		LZ4_copy8(d, s);
		d += 8;
		s += 8;
	} while (d < e);
}

static FORCE_INLINE unsigned int LZ4_NbCommonBytes(register size_t val)
{
#if LZ4_LITTLE_ENDIAN
	return __ffs(val) >> 3;
#else
	return (BITS_PER_LONG - 1 - __fls(val)) >> 3;
#endif
}

static FORCE_INLINE unsigned int LZ4_count(
	const BYTE *pIn,
	const BYTE *pMatch,
	const BYTE *pInLimit)
{
	const BYTE *const pStart = pIn;

	while (likely(pIn < pInLimit - (STEPSIZE - 1))) {
		size_t const diff = LZ4_read_ARCH(pMatch) ^ LZ4_read_ARCH(pIn);

		if (!diff) {
			pIn += STEPSIZE;
			pMatch += STEPSIZE;
			continue;
		}

		pIn += LZ4_NbCommonBytes(diff);

		return (unsigned int)(pIn - pStart);
	}

#if LZ4_ARCH64
	if ((pIn < (pInLimit - 3))
		&& (LZ4_read32(pMatch) == LZ4_read32(pIn))) {
		pIn += 4;
		pMatch += 4;
	}
#endif

	if ((pIn < (pInLimit - 1))
		&& (LZ4_read16(pMatch) == LZ4_read16(pIn))) {
		pIn += 2;
		pMatch += 2;
	}

	if ((pIn < pInLimit) && (*pMatch == *pIn))
		pIn++;

	return (unsigned int)(pIn - pStart);
}

typedef enum { noLimit = 0, limitedOutput = 1 } limitedOutput_directive;
typedef enum { byPtr, byU32, byU16 } tableType_t;

typedef enum { noDict = 0, withPrefix64k, usingExtDict } dict_directive;
typedef enum { noDictIssue = 0, dictSmall } dictIssue_directive;

typedef enum { endOnOutputSize = 0, endOnInputSize = 1 } endCondition_directive;
typedef enum { decode_full_block = 0, partial_decode = 1 } earlyEnd_directive;

#define LZ4_STATIC_ASSERT(c)	BUILD_BUG_ON(!(c))

#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
