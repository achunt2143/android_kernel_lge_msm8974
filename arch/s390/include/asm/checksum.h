<<<<<<< HEAD
#ifndef _S390_CHECKSUM_H
#define _S390_CHECKSUM_H

/*
 *  include/asm-s390/checksum.h
 *    S390 fast network checksum routines
 *    see also arch/S390/lib/checksum.c
 *
 *  S390 version
 *    Copyright (C) 1999 IBM Deutschland Entwicklung GmbH, IBM Corporation
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 *    S390 fast network checksum routines
 *
 *  S390 version
 *    Copyright IBM Corp. 1999
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *    Author(s): Ulrich Hild        (first version)
 *               Martin Schwidefsky (heavily optimized CKSM version)
 *               D.J. Barrow        (third attempt) 
 */

<<<<<<< HEAD
#include <asm/uaccess.h>

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
 * it's best to have buff aligned on a 32-bit boundary
 */
static inline __wsum
csum_partial(const void *buff, int len, __wsum sum)
{
	register unsigned long reg2 asm("2") = (unsigned long) buff;
	register unsigned long reg3 asm("3") = (unsigned long) len;

	asm volatile(
		"0:	cksm	%0,%1\n"	/* do checksum on longs */
		"	jo	0b\n"
		: "+d" (sum), "+d" (reg2), "+d" (reg3) : : "cc", "memory");
	return sum;
}

/*
 * the same as csum_partial_copy, but copies from user space.
 *
 * here even more important to align src and dst on a 32-bit (or even
 * better 64-bit) boundary
 *
 * Copy from userspace and compute checksum.  If we catch an exception
 * then zero the rest of the buffer.
 */
static inline __wsum
csum_partial_copy_from_user(const void __user *src, void *dst,
                                          int len, __wsum sum,
                                          int *err_ptr)
{
	int missing;

	missing = copy_from_user(dst, src, len);
	if (missing) {
		memset(dst + len - missing, 0, missing);
		*err_ptr = -EFAULT;
	}
		
	return csum_partial(dst, len, sum);
}


static inline __wsum
csum_partial_copy_nocheck (const void *src, void *dst, int len, __wsum sum)
{
        memcpy(dst,src,len);
	return csum_partial(dst, len, sum);
}

/*
 *      Fold a partial checksum without adding pseudo headers
=======
#ifndef _S390_CHECKSUM_H
#define _S390_CHECKSUM_H

#include <linux/instrumented.h>
#include <linux/in6.h>

static inline __wsum cksm(const void *buff, int len, __wsum sum)
{
	union register_pair rp = {
		.even = (unsigned long)buff,
		.odd = (unsigned long)len,
	};

	instrument_read(buff, len);
	asm volatile("\n"
		"0:	cksm	%[sum],%[rp]\n"
		"	jo	0b\n"
		: [sum] "+&d" (sum), [rp] "+&d" (rp.pair) : : "cc", "memory");
	return sum;
}

__wsum csum_partial(const void *buff, int len, __wsum sum);

#define _HAVE_ARCH_CSUM_AND_COPY
__wsum csum_partial_copy_nocheck(const void *src, void *dst, int len);

/*
 * Fold a partial checksum without adding pseudo headers.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static inline __sum16 csum_fold(__wsum sum)
{
	u32 csum = (__force u32) sum;

<<<<<<< HEAD
	csum += (csum >> 16) + (csum << 16);
=======
	csum += (csum >> 16) | (csum << 16);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	csum >>= 16;
	return (__force __sum16) ~csum;
}

/*
<<<<<<< HEAD
 *	This is a version of ip_compute_csum() optimized for IP headers,
 *	which always checksum on 4 octet boundaries.
 *
 */
static inline __sum16 ip_fast_csum(const void *iph, unsigned int ihl)
{
	return csum_fold(csum_partial(iph, ihl*4, 0));
}

/*
 * computes the checksum of the TCP/UDP pseudo-header
 * returns a 32-bit checksum
 */
static inline __wsum
csum_tcpudp_nofold(__be32 saddr, __be32 daddr,
                   unsigned short len, unsigned short proto,
                   __wsum sum)
{
	__u32 csum = (__force __u32)sum;

	csum += (__force __u32)saddr;
	if (csum < (__force __u32)saddr)
		csum++;

	csum += (__force __u32)daddr;
	if (csum < (__force __u32)daddr)
		csum++;

	csum += len + proto;
	if (csum < len + proto)
		csum++;

	return (__force __wsum)csum;
}

/*
 * computes the checksum of the TCP/UDP pseudo-header
 * returns a 16-bit checksum, already complemented
 */

static inline __sum16
csum_tcpudp_magic(__be32 saddr, __be32 daddr,
                  unsigned short len, unsigned short proto,
                  __wsum sum)
{
	return csum_fold(csum_tcpudp_nofold(saddr,daddr,len,proto,sum));
}

/*
 * this routine is used for miscellaneous IP-like checksums, mainly
 * in icmp.c
 */

=======
 * This is a version of ip_compute_csum() optimized for IP headers,
 * which always checksums on 4 octet boundaries.
 */
static inline __sum16 ip_fast_csum(const void *iph, unsigned int ihl)
{
	__u64 csum = 0;
	__u32 *ptr = (u32 *)iph;

	csum += *ptr++;
	csum += *ptr++;
	csum += *ptr++;
	csum += *ptr++;
	ihl -= 4;
	while (ihl--)
		csum += *ptr++;
	csum += (csum >> 32) | (csum << 32);
	return csum_fold((__force __wsum)(csum >> 32));
}

/*
 * Computes the checksum of the TCP/UDP pseudo-header.
 * Returns a 32-bit checksum.
 */
static inline __wsum csum_tcpudp_nofold(__be32 saddr, __be32 daddr, __u32 len,
					__u8 proto, __wsum sum)
{
	__u64 csum = (__force __u64)sum;

	csum += (__force __u32)saddr;
	csum += (__force __u32)daddr;
	csum += len;
	csum += proto;
	csum += (csum >> 32) | (csum << 32);
	return (__force __wsum)(csum >> 32);
}

/*
 * Computes the checksum of the TCP/UDP pseudo-header.
 * Returns a 16-bit checksum, already complemented.
 */
static inline __sum16 csum_tcpudp_magic(__be32 saddr, __be32 daddr, __u32 len,
					__u8 proto, __wsum sum)
{
	return csum_fold(csum_tcpudp_nofold(saddr, daddr, len, proto, sum));
}

/*
 * Used for miscellaneous IP-like checksums, mainly icmp.
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline __sum16 ip_compute_csum(const void *buff, int len)
{
	return csum_fold(csum_partial(buff, len, 0));
}

<<<<<<< HEAD
#endif /* _S390_CHECKSUM_H */


=======
#define _HAVE_ARCH_IPV6_CSUM
static inline __sum16 csum_ipv6_magic(const struct in6_addr *saddr,
				      const struct in6_addr *daddr,
				      __u32 len, __u8 proto, __wsum csum)
{
	__u64 sum = (__force __u64)csum;

	sum += (__force __u32)saddr->s6_addr32[0];
	sum += (__force __u32)saddr->s6_addr32[1];
	sum += (__force __u32)saddr->s6_addr32[2];
	sum += (__force __u32)saddr->s6_addr32[3];
	sum += (__force __u32)daddr->s6_addr32[0];
	sum += (__force __u32)daddr->s6_addr32[1];
	sum += (__force __u32)daddr->s6_addr32[2];
	sum += (__force __u32)daddr->s6_addr32[3];
	sum += len;
	sum += proto;
	sum += (sum >> 32) | (sum << 32);
	return csum_fold((__force __wsum)(sum >> 32));
}

#endif /* _S390_CHECKSUM_H */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
