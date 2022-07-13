<<<<<<< HEAD
/*
 *  arch/s390/kernel/cpcmd.c
 *
 *  S390 version
 *    Copyright IBM Corp. 1999,2007
=======
// SPDX-License-Identifier: GPL-2.0
/*
 *  S390 version
 *    Copyright IBM Corp. 1999, 2007
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *    Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com),
 *               Christian Borntraeger (cborntra@de.ibm.com),
 */

#define KMSG_COMPONENT "cpcmd"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt

#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/module.h>
=======
#include <linux/export.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/stddef.h>
#include <linux/string.h>
<<<<<<< HEAD
#include <asm/ebcdic.h>
#include <asm/cpcmd.h>
#include <asm/io.h>
=======
#include <linux/mm.h>
#include <linux/io.h>
#include <asm/diag.h>
#include <asm/ebcdic.h>
#include <asm/cpcmd.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static DEFINE_SPINLOCK(cpcmd_lock);
static char cpcmd_buf[241];

static int diag8_noresponse(int cmdlen)
{
<<<<<<< HEAD
	register unsigned long reg2 asm ("2") = (addr_t) cpcmd_buf;
	register unsigned long reg3 asm ("3") = cmdlen;

	asm volatile(
#ifndef CONFIG_64BIT
		"	diag	%1,%0,0x8\n"
#else /* CONFIG_64BIT */
		"	sam31\n"
		"	diag	%1,%0,0x8\n"
		"	sam64\n"
#endif /* CONFIG_64BIT */
		: "+d" (reg3) : "d" (reg2) : "cc");
	return reg3;
=======
	asm volatile(
		"	diag	%[rx],%[ry],0x8\n"
		: [ry] "+&d" (cmdlen)
		: [rx] "d" (__pa(cpcmd_buf))
		: "cc");
	return cmdlen;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int diag8_response(int cmdlen, char *response, int *rlen)
{
<<<<<<< HEAD
	register unsigned long reg2 asm ("2") = (addr_t) cpcmd_buf;
	register unsigned long reg3 asm ("3") = (addr_t) response;
	register unsigned long reg4 asm ("4") = cmdlen | 0x40000000L;
	register unsigned long reg5 asm ("5") = *rlen;

	asm volatile(
#ifndef CONFIG_64BIT
		"	diag	%2,%0,0x8\n"
		"	brc	8,1f\n"
		"	ar	%1,%4\n"
#else /* CONFIG_64BIT */
		"	sam31\n"
		"	diag	%2,%0,0x8\n"
		"	sam64\n"
		"	brc	8,1f\n"
		"	agr	%1,%4\n"
#endif /* CONFIG_64BIT */
		"1:\n"
		: "+d" (reg4), "+d" (reg5)
		: "d" (reg2), "d" (reg3), "d" (*rlen) : "cc");
	*rlen = reg5;
	return reg4;
=======
	union register_pair rx, ry;
	int cc;

	rx.even = __pa(cpcmd_buf);
	rx.odd	= __pa(response);
	ry.even = cmdlen | 0x40000000L;
	ry.odd	= *rlen;
	asm volatile(
		"	diag	%[rx],%[ry],0x8\n"
		"	ipm	%[cc]\n"
		"	srl	%[cc],28\n"
		: [cc] "=&d" (cc), [ry] "+&d" (ry.pair)
		: [rx] "d" (rx.pair)
		: "cc");
	if (cc)
		*rlen += ry.odd;
	else
		*rlen = ry.odd;
	return ry.even;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * __cpcmd has some restrictions over cpcmd
<<<<<<< HEAD
 *  - the response buffer must reside below 2GB (if any)
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *  - __cpcmd is unlocked and therefore not SMP-safe
 */
int  __cpcmd(const char *cmd, char *response, int rlen, int *response_code)
{
	int cmdlen;
	int rc;
	int response_len;

	cmdlen = strlen(cmd);
	BUG_ON(cmdlen > 240);
	memcpy(cpcmd_buf, cmd, cmdlen);
	ASCEBC(cpcmd_buf, cmdlen);

<<<<<<< HEAD
=======
	diag_stat_inc(DIAG_STAT_X008);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (response) {
		memset(response, 0, rlen);
		response_len = rlen;
		rc = diag8_response(cmdlen, response, &rlen);
		EBCASC(response, response_len);
        } else {
		rc = diag8_noresponse(cmdlen);
        }
	if (response_code)
		*response_code = rc;
	return rlen;
}
EXPORT_SYMBOL(__cpcmd);

int cpcmd(const char *cmd, char *response, int rlen, int *response_code)
{
<<<<<<< HEAD
	char *lowbuf;
	int len;
	unsigned long flags;

	if ((virt_to_phys(response) != (unsigned long) response) ||
			(((unsigned long)response + rlen) >> 31)) {
		lowbuf = kmalloc(rlen, GFP_KERNEL | GFP_DMA);
		if (!lowbuf) {
			pr_warning("The cpcmd kernel function failed to "
				   "allocate a response buffer\n");
=======
	unsigned long flags;
	char *lowbuf;
	int len;

	if (is_vmalloc_or_module_addr(response)) {
		lowbuf = kmalloc(rlen, GFP_KERNEL);
		if (!lowbuf) {
			pr_warn("The cpcmd kernel function failed to allocate a response buffer\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -ENOMEM;
		}
		spin_lock_irqsave(&cpcmd_lock, flags);
		len = __cpcmd(cmd, lowbuf, rlen, response_code);
		spin_unlock_irqrestore(&cpcmd_lock, flags);
		memcpy(response, lowbuf, rlen);
		kfree(lowbuf);
	} else {
		spin_lock_irqsave(&cpcmd_lock, flags);
		len = __cpcmd(cmd, response, rlen, response_code);
		spin_unlock_irqrestore(&cpcmd_lock, flags);
	}
	return len;
}
EXPORT_SYMBOL(cpcmd);
