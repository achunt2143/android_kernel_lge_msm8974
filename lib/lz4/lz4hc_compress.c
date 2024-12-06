/*
 * LZ4 HC - High Compression Mode of LZ4
<<<<<<< HEAD
 * Copyright (C) 2011-2012, Yann Collet.
 * BSD 2-Clause License (http://www.opensource.org/licenses/bsd-license.php)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *
=======
 * Copyright (C) 2011-2015, Yann Collet.
 *
 * BSD 2 - Clause License (http://www.opensource.org/licenses/bsd - license.php)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *	* Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer.
 *	* Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
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
<<<<<<< HEAD
 *
 * You can contact the author at :
 * - LZ4 homepage : http://fastcompression.blogspot.com/p/lz4.html
 * - LZ4 source repository : http://code.google.com/p/lz4/
 *
 *  Changed for kernel use by:
 *  Chanho Min <chanho.min@lge.com>
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/lz4.h>
#include <asm/unaligned.h>
#include "lz4defs.h"

struct lz4hc_data {
	const u8 *base;
	HTYPE hashtable[HASHTABLESIZE];
	u16 chaintable[MAXD];
	const u8 *nexttoupdate;
} __attribute__((__packed__));

static inline int lz4hc_init(struct lz4hc_data *hc4, const u8 *base)
{
	memset((void *)hc4->hashtable, 0, sizeof(hc4->hashtable));
	memset(hc4->chaintable, 0xFF, sizeof(hc4->chaintable));

#if LZ4_ARCH64
	hc4->nexttoupdate = base + 1;
#else
	hc4->nexttoupdate = base;
#endif
	hc4->base = base;
	return 1;
}

/* Update chains up to ip (excluded) */
static inline void lz4hc_insert(struct lz4hc_data *hc4, const u8 *ip)
{
	u16 *chaintable = hc4->chaintable;
	HTYPE *hashtable  = hc4->hashtable;
#if LZ4_ARCH64
	const BYTE * const base = hc4->base;
#else
	const int base = 0;
#endif

	while (hc4->nexttoupdate < ip) {
		const u8 *p = hc4->nexttoupdate;
		size_t delta = p - (hashtable[HASH_VALUE(p)] + base);
		if (delta > MAX_DISTANCE)
			delta = MAX_DISTANCE;
		chaintable[(size_t)(p) & MAXD_MASK] = (u16)delta;
		hashtable[HASH_VALUE(p)] = (p) - base;
		hc4->nexttoupdate++;
	}
}

static inline size_t lz4hc_commonlength(const u8 *p1, const u8 *p2,
		const u8 *const matchlimit)
{
	const u8 *p1t = p1;

	while (p1t < matchlimit - (STEPSIZE - 1)) {
#if LZ4_ARCH64
		u64 diff = A64(p2) ^ A64(p1t);
#else
		u32 diff = A32(p2) ^ A32(p1t);
#endif
		if (!diff) {
			p1t += STEPSIZE;
			p2 += STEPSIZE;
			continue;
		}
		p1t += LZ4_NBCOMMONBYTES(diff);
		return p1t - p1;
	}
#if LZ4_ARCH64
	if ((p1t < (matchlimit-3)) && (A32(p2) == A32(p1t))) {
		p1t += 4;
		p2 += 4;
	}
#endif

	if ((p1t < (matchlimit - 1)) && (A16(p2) == A16(p1t))) {
		p1t += 2;
		p2 += 2;
	}
	if ((p1t < matchlimit) && (*p2 == *p1t))
		p1t++;
	return p1t - p1;
}

static inline int lz4hc_insertandfindbestmatch(struct lz4hc_data *hc4,
		const u8 *ip, const u8 *const matchlimit, const u8 **matchpos)
{
	u16 *const chaintable = hc4->chaintable;
	HTYPE *const hashtable = hc4->hashtable;
	const u8 *ref;
#if LZ4_ARCH64
	const BYTE * const base = hc4->base;
#else
	const int base = 0;
#endif
	int nbattempts = MAX_NB_ATTEMPTS;
	size_t repl = 0, ml = 0;
	u16 delta;

	/* HC4 match finder */
	lz4hc_insert(hc4, ip);
	ref = hashtable[HASH_VALUE(ip)] + base;

	/* potential repetition */
	if (ref >= ip-4) {
		/* confirmed */
		if (A32(ref) == A32(ip)) {
			delta = (u16)(ip-ref);
			repl = ml  = lz4hc_commonlength(ip + MINMATCH,
					ref + MINMATCH, matchlimit) + MINMATCH;
			*matchpos = ref;
		}
		ref -= (size_t)chaintable[(size_t)(ref) & MAXD_MASK];
	}

	while ((ref >= ip - MAX_DISTANCE) && nbattempts) {
		nbattempts--;
		if (*(ref + ml) == *(ip + ml)) {
			if (A32(ref) == A32(ip)) {
				size_t mlt =
					lz4hc_commonlength(ip + MINMATCH,
					ref + MINMATCH, matchlimit) + MINMATCH;
				if (mlt > ml) {
					ml = mlt;
					*matchpos = ref;
				}
			}
		}
		ref -= (size_t)chaintable[(size_t)(ref) & MAXD_MASK];
	}

	/* Complete table */
	if (repl) {
		const BYTE *ptr = ip;
		const BYTE *end;
		end = ip + repl - (MINMATCH-1);
		/* Pre-Load */
		while (ptr < end - delta) {
			chaintable[(size_t)(ptr) & MAXD_MASK] = delta;
			ptr++;
		}
		do {
			chaintable[(size_t)(ptr) & MAXD_MASK] = delta;
			/* Head of chain */
			hashtable[HASH_VALUE(ptr)] = (ptr) - base;
			ptr++;
		} while (ptr < end);
		hc4->nexttoupdate = end;
=======
 * You can contact the author at :
 *	- LZ4 homepage : http://www.lz4.org
 *	- LZ4 source repository : https://github.com/lz4/lz4
 *
 *	Changed for kernel usage by:
 *	Sven Schmidt <4sschmid@informatik.uni-hamburg.de>
 */

/*-************************************
 *	Dependencies
 **************************************/
#include <linux/lz4.h>
#include "lz4defs.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h> /* memset */

/* *************************************
 *	Local Constants and types
 ***************************************/

#define OPTIMAL_ML (int)((ML_MASK - 1) + MINMATCH)

#define HASH_FUNCTION(i)	(((i) * 2654435761U) \
	>> ((MINMATCH*8) - LZ4HC_HASH_LOG))
#define DELTANEXTU16(p)	chainTable[(U16)(p)] /* faster */

static U32 LZ4HC_hashPtr(const void *ptr)
{
	return HASH_FUNCTION(LZ4_read32(ptr));
}

/**************************************
 *	HC Compression
 **************************************/
static void LZ4HC_init(LZ4HC_CCtx_internal *hc4, const BYTE *start)
{
	memset((void *)hc4->hashTable, 0, sizeof(hc4->hashTable));
	memset(hc4->chainTable, 0xFF, sizeof(hc4->chainTable));
	hc4->nextToUpdate = 64 * KB;
	hc4->base = start - 64 * KB;
	hc4->end = start;
	hc4->dictBase = start - 64 * KB;
	hc4->dictLimit = 64 * KB;
	hc4->lowLimit = 64 * KB;
}

/* Update chains up to ip (excluded) */
static FORCE_INLINE void LZ4HC_Insert(LZ4HC_CCtx_internal *hc4,
	const BYTE *ip)
{
	U16 * const chainTable = hc4->chainTable;
	U32 * const hashTable	= hc4->hashTable;
	const BYTE * const base = hc4->base;
	U32 const target = (U32)(ip - base);
	U32 idx = hc4->nextToUpdate;

	while (idx < target) {
		U32 const h = LZ4HC_hashPtr(base + idx);
		size_t delta = idx - hashTable[h];

		if (delta > MAX_DISTANCE)
			delta = MAX_DISTANCE;

		DELTANEXTU16(idx) = (U16)delta;

		hashTable[h] = idx;
		idx++;
	}

	hc4->nextToUpdate = target;
}

static FORCE_INLINE int LZ4HC_InsertAndFindBestMatch(
	LZ4HC_CCtx_internal *hc4, /* Index table will be updated */
	const BYTE *ip,
	const BYTE * const iLimit,
	const BYTE **matchpos,
	const int maxNbAttempts)
{
	U16 * const chainTable = hc4->chainTable;
	U32 * const HashTable = hc4->hashTable;
	const BYTE * const base = hc4->base;
	const BYTE * const dictBase = hc4->dictBase;
	const U32 dictLimit = hc4->dictLimit;
	const U32 lowLimit = (hc4->lowLimit + 64 * KB > (U32)(ip - base))
		? hc4->lowLimit
		: (U32)(ip - base) - (64 * KB - 1);
	U32 matchIndex;
	int nbAttempts = maxNbAttempts;
	size_t ml = 0;

	/* HC4 match finder */
	LZ4HC_Insert(hc4, ip);
	matchIndex = HashTable[LZ4HC_hashPtr(ip)];

	while ((matchIndex >= lowLimit)
		&& (nbAttempts)) {
		nbAttempts--;
		if (matchIndex >= dictLimit) {
			const BYTE * const match = base + matchIndex;

			if (*(match + ml) == *(ip + ml)
				&& (LZ4_read32(match) == LZ4_read32(ip))) {
				size_t const mlt = LZ4_count(ip + MINMATCH,
					match + MINMATCH, iLimit) + MINMATCH;

				if (mlt > ml) {
					ml = mlt;
					*matchpos = match;
				}
			}
		} else {
			const BYTE * const match = dictBase + matchIndex;

			if (LZ4_read32(match) == LZ4_read32(ip)) {
				size_t mlt;
				const BYTE *vLimit = ip
					+ (dictLimit - matchIndex);

				if (vLimit > iLimit)
					vLimit = iLimit;
				mlt = LZ4_count(ip + MINMATCH,
					match + MINMATCH, vLimit) + MINMATCH;
				if ((ip + mlt == vLimit)
					&& (vLimit < iLimit))
					mlt += LZ4_count(ip + mlt,
						base + dictLimit,
						iLimit);
				if (mlt > ml) {
					/* virtual matchpos */
					ml = mlt;
					*matchpos = base + matchIndex;
				}
			}
		}
		matchIndex -= DELTANEXTU16(matchIndex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return (int)ml;
}

<<<<<<< HEAD
static inline int lz4hc_insertandgetwidermatch(struct lz4hc_data *hc4,
	const u8 *ip, const u8 *startlimit, const u8 *matchlimit, int longest,
	const u8 **matchpos, const u8 **startpos)
{
	u16 *const chaintable = hc4->chaintable;
	HTYPE *const hashtable = hc4->hashtable;
#if LZ4_ARCH64
	const BYTE * const base = hc4->base;
#else
	const int base = 0;
#endif
	const u8 *ref;
	int nbattempts = MAX_NB_ATTEMPTS;
	int delta = (int)(ip - startlimit);

	/* First Match */
	lz4hc_insert(hc4, ip);
	ref = hashtable[HASH_VALUE(ip)] + base;

	while ((ref >= ip - MAX_DISTANCE) && (ref >= hc4->base)
		&& (nbattempts)) {
		nbattempts--;
		if (*(startlimit + longest) == *(ref - delta + longest)) {
			if (A32(ref) == A32(ip)) {
				const u8 *reft = ref + MINMATCH;
				const u8 *ipt = ip + MINMATCH;
				const u8 *startt = ip;

				while (ipt < matchlimit-(STEPSIZE - 1)) {
					#if LZ4_ARCH64
					u64 diff = A64(reft) ^ A64(ipt);
					#else
					u32 diff = A32(reft) ^ A32(ipt);
					#endif

					if (!diff) {
						ipt += STEPSIZE;
						reft += STEPSIZE;
						continue;
					}
					ipt += LZ4_NBCOMMONBYTES(diff);
					goto _endcount;
				}
				#if LZ4_ARCH64
				if ((ipt < (matchlimit - 3))
					&& (A32(reft) == A32(ipt))) {
					ipt += 4;
					reft += 4;
				}
				ipt += 2;
				#endif
				if ((ipt < (matchlimit - 1))
					&& (A16(reft) == A16(ipt))) {
					reft += 2;
				}
				if ((ipt < matchlimit) && (*reft == *ipt))
					ipt++;
_endcount:
				reft = ref;

				while ((startt > startlimit)
					&& (reft > hc4->base)
					&& (startt[-1] == reft[-1])) {
					startt--;
					reft--;
				}

				if ((ipt - startt) > longest) {
					longest = (int)(ipt - startt);
					*matchpos = reft;
					*startpos = startt;
				}
			}
		}
		ref -= (size_t)chaintable[(size_t)(ref) & MAXD_MASK];
	}
	return longest;
}

static inline int lz4_encodesequence(const u8 **ip, u8 **op, const u8 **anchor,
		int ml, const u8 *ref)
{
	int length, len;
	u8 *token;
=======
static FORCE_INLINE int LZ4HC_InsertAndGetWiderMatch(
	LZ4HC_CCtx_internal *hc4,
	const BYTE * const ip,
	const BYTE * const iLowLimit,
	const BYTE * const iHighLimit,
	int longest,
	const BYTE **matchpos,
	const BYTE **startpos,
	const int maxNbAttempts)
{
	U16 * const chainTable = hc4->chainTable;
	U32 * const HashTable = hc4->hashTable;
	const BYTE * const base = hc4->base;
	const U32 dictLimit = hc4->dictLimit;
	const BYTE * const lowPrefixPtr = base + dictLimit;
	const U32 lowLimit = (hc4->lowLimit + 64 * KB > (U32)(ip - base))
		? hc4->lowLimit
		: (U32)(ip - base) - (64 * KB - 1);
	const BYTE * const dictBase = hc4->dictBase;
	U32 matchIndex;
	int nbAttempts = maxNbAttempts;
	int delta = (int)(ip - iLowLimit);

	/* First Match */
	LZ4HC_Insert(hc4, ip);
	matchIndex = HashTable[LZ4HC_hashPtr(ip)];

	while ((matchIndex >= lowLimit)
		&& (nbAttempts)) {
		nbAttempts--;
		if (matchIndex >= dictLimit) {
			const BYTE *matchPtr = base + matchIndex;

			if (*(iLowLimit + longest)
				== *(matchPtr - delta + longest)) {
				if (LZ4_read32(matchPtr) == LZ4_read32(ip)) {
					int mlt = MINMATCH + LZ4_count(
						ip + MINMATCH,
						matchPtr + MINMATCH,
						iHighLimit);
					int back = 0;

					while ((ip + back > iLowLimit)
						&& (matchPtr + back > lowPrefixPtr)
						&& (ip[back - 1] == matchPtr[back - 1]))
						back--;

					mlt -= back;

					if (mlt > longest) {
						longest = (int)mlt;
						*matchpos = matchPtr + back;
						*startpos = ip + back;
					}
				}
			}
		} else {
			const BYTE * const matchPtr = dictBase + matchIndex;

			if (LZ4_read32(matchPtr) == LZ4_read32(ip)) {
				size_t mlt;
				int back = 0;
				const BYTE *vLimit = ip + (dictLimit - matchIndex);

				if (vLimit > iHighLimit)
					vLimit = iHighLimit;

				mlt = LZ4_count(ip + MINMATCH,
					matchPtr + MINMATCH, vLimit) + MINMATCH;

				if ((ip + mlt == vLimit) && (vLimit < iHighLimit))
					mlt += LZ4_count(ip + mlt, base + dictLimit,
						iHighLimit);
				while ((ip + back > iLowLimit)
					&& (matchIndex + back > lowLimit)
					&& (ip[back - 1] == matchPtr[back - 1]))
					back--;

				mlt -= back;

				if ((int)mlt > longest) {
					longest = (int)mlt;
					*matchpos = base + matchIndex + back;
					*startpos = ip + back;
				}
			}
		}

		matchIndex -= DELTANEXTU16(matchIndex);
	}

	return longest;
}

static FORCE_INLINE int LZ4HC_encodeSequence(
	const BYTE **ip,
	BYTE **op,
	const BYTE **anchor,
	int matchLength,
	const BYTE * const match,
	limitedOutput_directive limitedOutputBuffer,
	BYTE *oend)
{
	int length;
	BYTE *token;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Encode Literal length */
	length = (int)(*ip - *anchor);
	token = (*op)++;
<<<<<<< HEAD
	if (length >= (int)RUN_MASK) {
		*token = (RUN_MASK << ML_BITS);
		len = length - RUN_MASK;
		for (; len > 254 ; len -= 255)
			*(*op)++ = 255;
		*(*op)++ = (u8)len;
	} else
		*token = (length << ML_BITS);

	/* Copy Literals */
	LZ4_BLINDCOPY(*anchor, *op, length);

	/* Encode Offset */
	LZ4_WRITE_LITTLEENDIAN_16(*op, (u16)(*ip - ref));

	/* Encode MatchLength */
	len = (int)(ml - MINMATCH);
	if (len >= (int)ML_MASK) {
		*token += ML_MASK;
		len -= ML_MASK;
		for (; len > 509 ; len -= 510) {
			*(*op)++ = 255;
			*(*op)++ = 255;
		}
		if (len > 254) {
			len -= 255;
			*(*op)++ = 255;
		}
		*(*op)++ = (u8)len;
	} else
		*token += len;

	/* Prepare next loop */
	*ip += ml;
=======

	if ((limitedOutputBuffer)
		&& ((*op + (length>>8)
			+ length + (2 + 1 + LASTLITERALS)) > oend)) {
		/* Check output limit */
		return 1;
	}
	if (length >= (int)RUN_MASK) {
		int len;

		*token = (RUN_MASK<<ML_BITS);
		len = length - RUN_MASK;
		for (; len > 254 ; len -= 255)
			*(*op)++ = 255;
		*(*op)++ = (BYTE)len;
	} else
		*token = (BYTE)(length<<ML_BITS);

	/* Copy Literals */
	LZ4_wildCopy(*op, *anchor, (*op) + length);
	*op += length;

	/* Encode Offset */
	LZ4_writeLE16(*op, (U16)(*ip - match));
	*op += 2;

	/* Encode MatchLength */
	length = (int)(matchLength - MINMATCH);

	if ((limitedOutputBuffer)
		&& (*op + (length>>8)
			+ (1 + LASTLITERALS) > oend)) {
		/* Check output limit */
		return 1;
	}

	if (length >= (int)ML_MASK) {
		*token += ML_MASK;
		length -= ML_MASK;

		for (; length > 509 ; length -= 510) {
			*(*op)++ = 255;
			*(*op)++ = 255;
		}

		if (length > 254) {
			length -= 255;
			*(*op)++ = 255;
		}

		*(*op)++ = (BYTE)length;
	} else
		*token += (BYTE)(length);

	/* Prepare next loop */
	*ip += matchLength;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	*anchor = *ip;

	return 0;
}

<<<<<<< HEAD
static int lz4_compresshcctx(struct lz4hc_data *ctx,
		const char *source,
		char *dest,
		int isize)
{
	const u8 *ip = (const u8 *)source;
	const u8 *anchor = ip;
	const u8 *const iend = ip + isize;
	const u8 *const mflimit = iend - MFLIMIT;
	const u8 *const matchlimit = (iend - LASTLITERALS);

	u8 *op = (u8 *)dest;

	int ml, ml2, ml3, ml0;
	const u8 *ref = NULL;
	const u8 *start2 = NULL;
	const u8 *ref2 = NULL;
	const u8 *start3 = NULL;
	const u8 *ref3 = NULL;
	const u8 *start0;
	const u8 *ref0;
	int lastrun;
=======
static int LZ4HC_compress_generic(
	LZ4HC_CCtx_internal *const ctx,
	const char * const source,
	char * const dest,
	int const inputSize,
	int const maxOutputSize,
	int compressionLevel,
	limitedOutput_directive limit
	)
{
	const BYTE *ip = (const BYTE *) source;
	const BYTE *anchor = ip;
	const BYTE * const iend = ip + inputSize;
	const BYTE * const mflimit = iend - MFLIMIT;
	const BYTE * const matchlimit = (iend - LASTLITERALS);

	BYTE *op = (BYTE *) dest;
	BYTE * const oend = op + maxOutputSize;

	unsigned int maxNbAttempts;
	int ml, ml2, ml3, ml0;
	const BYTE *ref = NULL;
	const BYTE *start2 = NULL;
	const BYTE *ref2 = NULL;
	const BYTE *start3 = NULL;
	const BYTE *ref3 = NULL;
	const BYTE *start0;
	const BYTE *ref0;

	/* init */
	if (compressionLevel > LZ4HC_MAX_CLEVEL)
		compressionLevel = LZ4HC_MAX_CLEVEL;
	if (compressionLevel < 1)
		compressionLevel = LZ4HC_DEFAULT_CLEVEL;
	maxNbAttempts = 1 << (compressionLevel - 1);
	ctx->end += inputSize;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ip++;

	/* Main Loop */
	while (ip < mflimit) {
<<<<<<< HEAD
		ml = lz4hc_insertandfindbestmatch(ctx, ip, matchlimit, (&ref));
=======
		ml = LZ4HC_InsertAndFindBestMatch(ctx, ip,
			matchlimit, (&ref), maxNbAttempts);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!ml) {
			ip++;
			continue;
		}

		/* saved, in case we would skip too much */
		start0 = ip;
		ref0 = ref;
		ml0 = ml;
<<<<<<< HEAD
_search2:
		if (ip+ml < mflimit)
			ml2 = lz4hc_insertandgetwidermatch(ctx, ip + ml - 2,
				ip + 1, matchlimit, ml, &ref2, &start2);
		else
			ml2 = ml;
		/* No better match */
		if (ml2 == ml) {
			lz4_encodesequence(&ip, &op, &anchor, ml, ref);
=======

_Search2:
		if (ip + ml < mflimit)
			ml2 = LZ4HC_InsertAndGetWiderMatch(ctx,
				ip + ml - 2, ip + 0,
				matchlimit, ml, &ref2,
				&start2, maxNbAttempts);
		else
			ml2 = ml;

		if (ml2 == ml) {
			/* No better match */
			if (LZ4HC_encodeSequence(&ip, &op,
				&anchor, ml, ref, limit, oend))
				return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			continue;
		}

		if (start0 < ip) {
<<<<<<< HEAD
			/* empirical */
			if (start2 < ip + ml0) {
=======
			if (start2 < ip + ml0) {
				/* empirical */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				ip = start0;
				ref = ref0;
				ml = ml0;
			}
		}
<<<<<<< HEAD
		/*
		 * Here, start0==ip
		 * First Match too small : removed
		 */
		if ((start2 - ip) < 3) {
			ml = ml2;
			ip = start2;
			ref = ref2;
			goto _search2;
		}

_search3:
		/*
		 * Currently we have :
		 * ml2 > ml1, and
		 * ip1+3 <= ip2 (usually < ip1+ml1)
		 */
		if ((start2 - ip) < OPTIMAL_ML) {
			int correction;
			int new_ml = ml;
=======

		/* Here, start0 == ip */
		if ((start2 - ip) < 3) {
			/* First Match too small : removed */
			ml = ml2;
			ip = start2;
			ref = ref2;
			goto _Search2;
		}

_Search3:
		/*
		* Currently we have :
		* ml2 > ml1, and
		* ip1 + 3 <= ip2 (usually < ip1 + ml1)
		*/
		if ((start2 - ip) < OPTIMAL_ML) {
			int correction;
			int new_ml = ml;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (new_ml > OPTIMAL_ML)
				new_ml = OPTIMAL_ML;
			if (ip + new_ml > start2 + ml2 - MINMATCH)
				new_ml = (int)(start2 - ip) + ml2 - MINMATCH;
<<<<<<< HEAD
			correction = new_ml - (int)(start2 - ip);
=======

			correction = new_ml - (int)(start2 - ip);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (correction > 0) {
				start2 += correction;
				ref2 += correction;
				ml2 -= correction;
			}
		}
		/*
<<<<<<< HEAD
		 * Now, we have start2 = ip+new_ml,
		 * with new_ml=min(ml, OPTIMAL_ML=18)
		 */
		if (start2 + ml2 < mflimit)
			ml3 = lz4hc_insertandgetwidermatch(ctx,
				start2 + ml2 - 3, start2, matchlimit,
				ml2, &ref3, &start3);
		else
			ml3 = ml2;

		/* No better match : 2 sequences to encode */
		if (ml3 == ml2) {
			/* ip & ref are known; Now for ml */
			if (start2 < ip+ml)
				ml = (int)(start2 - ip);

			/* Now, encode 2 sequences */
			lz4_encodesequence(&ip, &op, &anchor, ml, ref);
			ip = start2;
			lz4_encodesequence(&ip, &op, &anchor, ml2, ref2);
			continue;
		}

		/* Not enough space for match 2 : remove it */
		if (start3 < ip + ml + 3) {
			/*
			 * can write Seq1 immediately ==> Seq2 is removed,
			 * so Seq3 becomes Seq1
			 */
			if (start3 >= (ip + ml)) {
				if (start2 < ip + ml) {
					int correction =
						(int)(ip + ml - start2);
=======
		 * Now, we have start2 = ip + new_ml,
		 * with new_ml = min(ml, OPTIMAL_ML = 18)
		 */

		if (start2 + ml2 < mflimit)
			ml3 = LZ4HC_InsertAndGetWiderMatch(ctx,
				start2 + ml2 - 3, start2,
				matchlimit, ml2, &ref3, &start3,
				maxNbAttempts);
		else
			ml3 = ml2;

		if (ml3 == ml2) {
			/* No better match : 2 sequences to encode */
			/* ip & ref are known; Now for ml */
			if (start2 < ip + ml)
				ml = (int)(start2 - ip);
			/* Now, encode 2 sequences */
			if (LZ4HC_encodeSequence(&ip, &op, &anchor,
				ml, ref, limit, oend))
				return 0;
			ip = start2;
			if (LZ4HC_encodeSequence(&ip, &op, &anchor,
				ml2, ref2, limit, oend))
				return 0;
			continue;
		}

		if (start3 < ip + ml + 3) {
			/* Not enough space for match 2 : remove it */
			if (start3 >= (ip + ml)) {
				/* can write Seq1 immediately
				 * ==> Seq2 is removed,
				 * so Seq3 becomes Seq1
				 */
				if (start2 < ip + ml) {
					int correction = (int)(ip + ml - start2);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					start2 += correction;
					ref2 += correction;
					ml2 -= correction;
					if (ml2 < MINMATCH) {
						start2 = start3;
						ref2 = ref3;
						ml2 = ml3;
					}
				}

<<<<<<< HEAD
				lz4_encodesequence(&ip, &op, &anchor, ml, ref);
				ip  = start3;
				ref = ref3;
				ml  = ml3;
=======
				if (LZ4HC_encodeSequence(&ip, &op, &anchor,
					ml, ref, limit, oend))
					return 0;
				ip = start3;
				ref = ref3;
				ml = ml3;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

				start0 = start2;
				ref0 = ref2;
				ml0 = ml2;
<<<<<<< HEAD
				goto _search2;
=======
				goto _Search2;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}

			start2 = start3;
			ref2 = ref3;
			ml2 = ml3;
<<<<<<< HEAD
			goto _search3;
		}

		/*
		 * OK, now we have 3 ascending matches; let's write at least
		 * the first one ip & ref are known; Now for ml
		 */
		if (start2 < ip + ml) {
			if ((start2 - ip) < (int)ML_MASK) {
				int correction;
				if (ml > OPTIMAL_ML)
					ml = OPTIMAL_ML;
				if (ip + ml > start2 + ml2 - MINMATCH)
					ml = (int)(start2 - ip) + ml2
						- MINMATCH;
=======
			goto _Search3;
		}

		/*
		* OK, now we have 3 ascending matches;
		* let's write at least the first one
		* ip & ref are known; Now for ml
		*/
		if (start2 < ip + ml) {
			if ((start2 - ip) < (int)ML_MASK) {
				int correction;

				if (ml > OPTIMAL_ML)
					ml = OPTIMAL_ML;
				if (ip + ml > start2 + ml2 - MINMATCH)
					ml = (int)(start2 - ip) + ml2 - MINMATCH;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				correction = ml - (int)(start2 - ip);
				if (correction > 0) {
					start2 += correction;
					ref2 += correction;
					ml2 -= correction;
				}
			} else
				ml = (int)(start2 - ip);
		}
<<<<<<< HEAD
		lz4_encodesequence(&ip, &op, &anchor, ml, ref);
=======
		if (LZ4HC_encodeSequence(&ip, &op, &anchor, ml,
			ref, limit, oend))
			return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		ip = start2;
		ref = ref2;
		ml = ml2;

		start2 = start3;
		ref2 = ref3;
		ml2 = ml3;

<<<<<<< HEAD
		goto _search3;
	}

	/* Encode Last Literals */
	lastrun = (int)(iend - anchor);
	if (lastrun >= (int)RUN_MASK) {
		*op++ = (RUN_MASK << ML_BITS);
		lastrun -= RUN_MASK;
		for (; lastrun > 254 ; lastrun -= 255)
			*op++ = 255;
		*op++ = (u8) lastrun;
	} else
		*op++ = (lastrun << ML_BITS);
	memcpy(op, anchor, iend - anchor);
	op += iend - anchor;
=======
		goto _Search3;
	}

	/* Encode Last Literals */
	{
		int lastRun = (int)(iend - anchor);

		if ((limit)
			&& (((char *)op - dest) + lastRun + 1
				+ ((lastRun + 255 - RUN_MASK)/255)
					> (U32)maxOutputSize)) {
			/* Check output limit */
			return 0;
		}
		if (lastRun >= (int)RUN_MASK) {
			*op++ = (RUN_MASK<<ML_BITS);
			lastRun -= RUN_MASK;
			for (; lastRun > 254 ; lastRun -= 255)
				*op++ = 255;
			*op++ = (BYTE) lastRun;
		} else
			*op++ = (BYTE)(lastRun<<ML_BITS);
		LZ4_memcpy(op, anchor, iend - anchor);
		op += iend - anchor;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* End */
	return (int) (((char *)op) - dest);
}

<<<<<<< HEAD
int lz4hc_compress(const unsigned char *src, size_t src_len,
			unsigned char *dst, size_t *dst_len, void *wrkmem)
{
	int ret = -1;
	int out_len = 0;

	struct lz4hc_data *hc4 = (struct lz4hc_data *)wrkmem;
	lz4hc_init(hc4, (const u8 *)src);
	out_len = lz4_compresshcctx((struct lz4hc_data *)hc4, (const u8 *)src,
		(char *)dst, (int)src_len);

	if (out_len < 0)
		goto exit;

	*dst_len = out_len;
	return 0;

exit:
	return ret;
}
EXPORT_SYMBOL(lz4hc_compress);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("LZ4HC compressor");
=======
static int LZ4_compress_HC_extStateHC(
	void *state,
	const char *src,
	char *dst,
	int srcSize,
	int maxDstSize,
	int compressionLevel)
{
	LZ4HC_CCtx_internal *ctx = &((LZ4_streamHC_t *)state)->internal_donotuse;

	if (((size_t)(state)&(sizeof(void *) - 1)) != 0) {
		/* Error : state is not aligned
		 * for pointers (32 or 64 bits)
		 */
		return 0;
	}

	LZ4HC_init(ctx, (const BYTE *)src);

	if (maxDstSize < LZ4_compressBound(srcSize))
		return LZ4HC_compress_generic(ctx, src, dst,
			srcSize, maxDstSize, compressionLevel, limitedOutput);
	else
		return LZ4HC_compress_generic(ctx, src, dst,
			srcSize, maxDstSize, compressionLevel, noLimit);
}

int LZ4_compress_HC(const char *src, char *dst, int srcSize,
	int maxDstSize, int compressionLevel, void *wrkmem)
{
	return LZ4_compress_HC_extStateHC(wrkmem, src, dst,
		srcSize, maxDstSize, compressionLevel);
}
EXPORT_SYMBOL(LZ4_compress_HC);

/**************************************
 *	Streaming Functions
 **************************************/
void LZ4_resetStreamHC(LZ4_streamHC_t *LZ4_streamHCPtr, int compressionLevel)
{
	LZ4_streamHCPtr->internal_donotuse.base = NULL;
	LZ4_streamHCPtr->internal_donotuse.compressionLevel = (unsigned int)compressionLevel;
}

int LZ4_loadDictHC(LZ4_streamHC_t *LZ4_streamHCPtr,
	const char *dictionary,
	int dictSize)
{
	LZ4HC_CCtx_internal *ctxPtr = &LZ4_streamHCPtr->internal_donotuse;

	if (dictSize > 64 * KB) {
		dictionary += dictSize - 64 * KB;
		dictSize = 64 * KB;
	}
	LZ4HC_init(ctxPtr, (const BYTE *)dictionary);
	if (dictSize >= 4)
		LZ4HC_Insert(ctxPtr, (const BYTE *)dictionary + (dictSize - 3));
	ctxPtr->end = (const BYTE *)dictionary + dictSize;
	return dictSize;
}
EXPORT_SYMBOL(LZ4_loadDictHC);

/* compression */

static void LZ4HC_setExternalDict(
	LZ4HC_CCtx_internal *ctxPtr,
	const BYTE *newBlock)
{
	if (ctxPtr->end >= ctxPtr->base + 4) {
		/* Referencing remaining dictionary content */
		LZ4HC_Insert(ctxPtr, ctxPtr->end - 3);
	}

	/*
	 * Only one memory segment for extDict,
	 * so any previous extDict is lost at this stage
	 */
	ctxPtr->lowLimit	= ctxPtr->dictLimit;
	ctxPtr->dictLimit = (U32)(ctxPtr->end - ctxPtr->base);
	ctxPtr->dictBase	= ctxPtr->base;
	ctxPtr->base = newBlock - ctxPtr->dictLimit;
	ctxPtr->end	= newBlock;
	/* match referencing will resume from there */
	ctxPtr->nextToUpdate = ctxPtr->dictLimit;
}

static int LZ4_compressHC_continue_generic(
	LZ4_streamHC_t *LZ4_streamHCPtr,
	const char *source,
	char *dest,
	int inputSize,
	int maxOutputSize,
	limitedOutput_directive limit)
{
	LZ4HC_CCtx_internal *ctxPtr = &LZ4_streamHCPtr->internal_donotuse;

	/* auto - init if forgotten */
	if (ctxPtr->base == NULL)
		LZ4HC_init(ctxPtr, (const BYTE *) source);

	/* Check overflow */
	if ((size_t)(ctxPtr->end - ctxPtr->base) > 2 * GB) {
		size_t dictSize = (size_t)(ctxPtr->end - ctxPtr->base)
			- ctxPtr->dictLimit;
		if (dictSize > 64 * KB)
			dictSize = 64 * KB;
		LZ4_loadDictHC(LZ4_streamHCPtr,
			(const char *)(ctxPtr->end) - dictSize, (int)dictSize);
	}

	/* Check if blocks follow each other */
	if ((const BYTE *)source != ctxPtr->end)
		LZ4HC_setExternalDict(ctxPtr, (const BYTE *)source);

	/* Check overlapping input/dictionary space */
	{
		const BYTE *sourceEnd = (const BYTE *) source + inputSize;
		const BYTE * const dictBegin = ctxPtr->dictBase + ctxPtr->lowLimit;
		const BYTE * const dictEnd = ctxPtr->dictBase + ctxPtr->dictLimit;

		if ((sourceEnd > dictBegin)
			&& ((const BYTE *)source < dictEnd)) {
			if (sourceEnd > dictEnd)
				sourceEnd = dictEnd;
			ctxPtr->lowLimit = (U32)(sourceEnd - ctxPtr->dictBase);

			if (ctxPtr->dictLimit - ctxPtr->lowLimit < 4)
				ctxPtr->lowLimit = ctxPtr->dictLimit;
		}
	}

	return LZ4HC_compress_generic(ctxPtr, source, dest,
		inputSize, maxOutputSize, ctxPtr->compressionLevel, limit);
}

int LZ4_compress_HC_continue(
	LZ4_streamHC_t *LZ4_streamHCPtr,
	const char *source,
	char *dest,
	int inputSize,
	int maxOutputSize)
{
	if (maxOutputSize < LZ4_compressBound(inputSize))
		return LZ4_compressHC_continue_generic(LZ4_streamHCPtr,
			source, dest, inputSize, maxOutputSize, limitedOutput);
	else
		return LZ4_compressHC_continue_generic(LZ4_streamHCPtr,
			source, dest, inputSize, maxOutputSize, noLimit);
}
EXPORT_SYMBOL(LZ4_compress_HC_continue);

/* dictionary saving */

int LZ4_saveDictHC(
	LZ4_streamHC_t *LZ4_streamHCPtr,
	char *safeBuffer,
	int dictSize)
{
	LZ4HC_CCtx_internal *const streamPtr = &LZ4_streamHCPtr->internal_donotuse;
	int const prefixSize = (int)(streamPtr->end
		- (streamPtr->base + streamPtr->dictLimit));

	if (dictSize > 64 * KB)
		dictSize = 64 * KB;
	if (dictSize < 4)
		dictSize = 0;
	if (dictSize > prefixSize)
		dictSize = prefixSize;

	memmove(safeBuffer, streamPtr->end - dictSize, dictSize);

	{
		U32 const endIndex = (U32)(streamPtr->end - streamPtr->base);

		streamPtr->end = (const BYTE *)safeBuffer + dictSize;
		streamPtr->base = streamPtr->end - endIndex;
		streamPtr->dictLimit = endIndex - dictSize;
		streamPtr->lowLimit = endIndex - dictSize;

		if (streamPtr->nextToUpdate < streamPtr->dictLimit)
			streamPtr->nextToUpdate = streamPtr->dictLimit;
	}
	return dictSize;
}
EXPORT_SYMBOL(LZ4_saveDictHC);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("LZ4 HC compressor");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
