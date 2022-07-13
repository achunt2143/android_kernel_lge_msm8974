<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * arch/x86_64/lib/csum-partial.c
 *
 * This file contains network checksum routines that are better done
 * in an architecture-specific manner due to speed.
 */
<<<<<<< HEAD
 
#include <linux/compiler.h>
#include <linux/module.h>
#include <asm/checksum.h>

static inline unsigned short from32to16(unsigned a) 
{
	unsigned short b = a >> 16; 
	asm("addw %w2,%w0\n\t"
	    "adcw $0,%w0\n" 
	    : "=r" (b)
	    : "0" (b), "r" (a));
	return b;
}

/*
 * Do a 64-bit checksum on an arbitrary memory area.
=======

#include <linux/compiler.h>
#include <linux/export.h>
#include <asm/checksum.h>
#include <asm/word-at-a-time.h>

static inline __wsum csum_finalize_sum(u64 temp64)
{
	return (__force __wsum)((temp64 + ror64(temp64, 32)) >> 32);
}

static inline unsigned long update_csum_40b(unsigned long sum, const unsigned long m[5])
{
	asm("addq %1,%0\n\t"
	     "adcq %2,%0\n\t"
	     "adcq %3,%0\n\t"
	     "adcq %4,%0\n\t"
	     "adcq %5,%0\n\t"
	     "adcq $0,%0"
		:"+r" (sum)
		:"m" (m[0]), "m" (m[1]), "m" (m[2]),
		 "m" (m[3]), "m" (m[4]));
	return sum;
}

/*
 * Do a checksum on an arbitrary memory area.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Returns a 32bit checksum.
 *
 * This isn't as time critical as it used to be because many NICs
 * do hardware checksumming these days.
<<<<<<< HEAD
 * 
 * Things tried and found to not make it faster:
 * Manual Prefetching
 * Unrolling to an 128 bytes inner loop.
 * Using interleaving with more registers to break the carry chains.
 */
static unsigned do_csum(const unsigned char *buff, unsigned len)
{
	unsigned odd, count;
	unsigned long result = 0;

	if (unlikely(len == 0))
		return result; 
	odd = 1 & (unsigned long) buff;
	if (unlikely(odd)) {
		result = *buff << 8;
		len--;
		buff++;
	}
	count = len >> 1;		/* nr of 16-bit words.. */
	if (count) {
		if (2 & (unsigned long) buff) {
			result += *(unsigned short *)buff;
			count--;
			len -= 2;
			buff += 2;
		}
		count >>= 1;		/* nr of 32-bit words.. */
		if (count) {
			unsigned long zero;
			unsigned count64;
			if (4 & (unsigned long) buff) {
				result += *(unsigned int *) buff;
				count--;
				len -= 4;
				buff += 4;
			}
			count >>= 1;	/* nr of 64-bit words.. */

			/* main loop using 64byte blocks */
			zero = 0;
			count64 = count >> 3;
			while (count64) { 
				asm("addq 0*8(%[src]),%[res]\n\t"
				    "adcq 1*8(%[src]),%[res]\n\t"
				    "adcq 2*8(%[src]),%[res]\n\t"
				    "adcq 3*8(%[src]),%[res]\n\t"
				    "adcq 4*8(%[src]),%[res]\n\t"
				    "adcq 5*8(%[src]),%[res]\n\t"
				    "adcq 6*8(%[src]),%[res]\n\t"
				    "adcq 7*8(%[src]),%[res]\n\t"
				    "adcq %[zero],%[res]"
				    : [res] "=r" (result)
				    : [src] "r" (buff), [zero] "r" (zero),
				    "[res]" (result));
				buff += 64;
				count64--;
			}

			/* last up to 7 8byte blocks */
			count %= 8; 
			while (count) { 
				asm("addq %1,%0\n\t"
				    "adcq %2,%0\n" 
					    : "=r" (result)
				    : "m" (*(unsigned long *)buff), 
				    "r" (zero),  "0" (result));
				--count; 
					buff += 8;
			}
			result = add32_with_carry(result>>32,
						  result&0xffffffff); 

			if (len & 4) {
				result += *(unsigned int *) buff;
				buff += 4;
			}
		}
		if (len & 2) {
			result += *(unsigned short *) buff;
			buff += 2;
		}
	}
	if (len & 1)
		result += *buff;
	result = add32_with_carry(result>>32, result & 0xffffffff); 
	if (unlikely(odd)) { 
		result = from32to16(result);
		result = ((result >> 8) & 0xff) | ((result & 0xff) << 8);
	}
	return result;
}

/*
 * computes the checksum of a memory block at buff, length len,
 * and adds in "sum" (32-bit)
 *
 * returns a 32-bit number suitable for feeding into itself
 * or csum_tcpudp_magic
 *
 * this function must be called with even lengths, except
 * for the last fragment, which may be odd
 *
=======
 *
 * Still, with CHECKSUM_COMPLETE this is called to compute
 * checksums on IPv6 headers (40 bytes) and other small parts.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * it's best to have buff aligned on a 64-bit boundary
 */
__wsum csum_partial(const void *buff, int len, __wsum sum)
{
<<<<<<< HEAD
	return (__force __wsum)add32_with_carry(do_csum(buff, len),
						(__force u32)sum);
}
=======
	u64 temp64 = (__force u64)sum;

	/* Do two 40-byte chunks in parallel to get better ILP */
	if (likely(len >= 80)) {
		u64 temp64_2 = 0;
		do {
			temp64 = update_csum_40b(temp64, buff);
			temp64_2 = update_csum_40b(temp64_2, buff + 40);
			buff += 80;
			len -= 80;
		} while (len >= 80);

		asm("addq %1,%0\n\t"
		    "adcq $0,%0"
		    :"+r" (temp64): "r" (temp64_2));
	}

	/*
	 * len == 40 is the hot case due to IPv6 headers, so return
	 * early for that exact case without checking the tail bytes.
	 */
	if (len >= 40) {
		temp64 = update_csum_40b(temp64, buff);
		len -= 40;
		if (!len)
			return csum_finalize_sum(temp64);
		buff += 40;
	}

	if (len & 32) {
		asm("addq 0*8(%[src]),%[res]\n\t"
		    "adcq 1*8(%[src]),%[res]\n\t"
		    "adcq 2*8(%[src]),%[res]\n\t"
		    "adcq 3*8(%[src]),%[res]\n\t"
		    "adcq $0,%[res]"
		    : [res] "+r"(temp64)
		    : [src] "r"(buff), "m"(*(const char(*)[32])buff));
		buff += 32;
	}
	if (len & 16) {
		asm("addq 0*8(%[src]),%[res]\n\t"
		    "adcq 1*8(%[src]),%[res]\n\t"
		    "adcq $0,%[res]"
		    : [res] "+r"(temp64)
		    : [src] "r"(buff), "m"(*(const char(*)[16])buff));
		buff += 16;
	}
	if (len & 8) {
		asm("addq 0*8(%[src]),%[res]\n\t"
		    "adcq $0,%[res]"
		    : [res] "+r"(temp64)
		    : [src] "r"(buff), "m"(*(const char(*)[8])buff));
		buff += 8;
	}
	if (len & 7) {
		unsigned int shift = (-len << 3) & 63;
		unsigned long trail;

		trail = (load_unaligned_zeropad(buff) << shift) >> shift;

		asm("addq %[trail],%[res]\n\t"
		    "adcq $0,%[res]"
		    : [res] "+r"(temp64)
		    : [trail] "r"(trail));
	}
	return csum_finalize_sum(temp64);
}
EXPORT_SYMBOL(csum_partial);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * this routine is used for miscellaneous IP-like checksums, mainly
 * in icmp.c
 */
__sum16 ip_compute_csum(const void *buff, int len)
{
<<<<<<< HEAD
	return csum_fold(csum_partial(buff,len,0));
}
EXPORT_SYMBOL(ip_compute_csum);

=======
	return csum_fold(csum_partial(buff, len, 0));
}
EXPORT_SYMBOL(ip_compute_csum);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
