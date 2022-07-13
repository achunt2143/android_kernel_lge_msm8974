<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  arch/m68k/mvme16x/config.c
 *
 *  Copyright (C) 1995 Richard Hirst [richard@sleepie.demon.co.uk]
 *
 * Based on:
 *
 *  linux/amiga/config.c
 *
 *  Copyright (C) 1993 Hamish Macdonald
<<<<<<< HEAD
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file README.legal in the main directory of this archive
 * for more details.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/seq_file.h>
#include <linux/tty.h>
<<<<<<< HEAD
=======
#include <linux/clocksource.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/console.h>
#include <linux/linkage.h>
#include <linux/init.h>
#include <linux/major.h>
<<<<<<< HEAD
#include <linux/genhd.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/rtc.h>
#include <linux/interrupt.h>
#include <linux/module.h>

#include <asm/bootinfo.h>
<<<<<<< HEAD
#include <asm/pgtable.h>
#include <asm/setup.h>
#include <asm/irq.h>
#include <asm/traps.h>
#include <asm/rtc.h>
#include <asm/machdep.h>
#include <asm/mvme16xhw.h>
=======
#include <asm/bootinfo-vme.h>
#include <asm/byteorder.h>
#include <asm/setup.h>
#include <asm/irq.h>
#include <asm/traps.h>
#include <asm/machdep.h>
#include <asm/mvme16xhw.h>
#include <asm/config.h>

#include "mvme16x.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern t_bdid mvme_bdid;

static MK48T08ptr_t volatile rtc = (MK48T08ptr_t)MVME_RTC_BASE;

static void mvme16x_get_model(char *model);
<<<<<<< HEAD
extern void mvme16x_sched_init(irq_handler_t handler);
extern unsigned long mvme16x_gettimeoffset (void);
extern int mvme16x_hwclk (int, struct rtc_time *);
extern int mvme16x_set_clock_mmss (unsigned long);
=======
extern void mvme16x_sched_init(void);
extern int mvme16x_hwclk (int, struct rtc_time *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern void mvme16x_reset (void);

int bcd2int (unsigned char b);

<<<<<<< HEAD
/* Save tick handler routine pointer, will point to xtime_update() in
 * kernel/time/timekeeping.c, called via mvme16x_process_int() */

static irq_handler_t tick_handler;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

unsigned short mvme16x_config;
EXPORT_SYMBOL(mvme16x_config);


<<<<<<< HEAD
int mvme16x_parse_bootinfo(const struct bi_record *bi)
{
	if (bi->tag == BI_VME_TYPE || bi->tag == BI_VME_BRDINFO)
=======
int __init mvme16x_parse_bootinfo(const struct bi_record *bi)
{
	uint16_t tag = be16_to_cpu(bi->tag);
	if (tag == BI_VME_TYPE || tag == BI_VME_BRDINFO)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	else
		return 1;
}

void mvme16x_reset(void)
{
<<<<<<< HEAD
	printk ("\r\n\nCalled mvme16x_reset\r\n"
			"\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r");
=======
	pr_info("\r\n\nCalled mvme16x_reset\r\n"
		"\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* The string of returns is to delay the reset until the whole
	 * message is output.  Assert reset bit in GCSR */
	*(volatile char *)0xfff40107 = 0x80;
}

static void mvme16x_get_model(char *model)
{
    p_bdid p = &mvme_bdid;
    char suf[4];

    suf[1] = p->brdsuffix[0];
    suf[2] = p->brdsuffix[1];
    suf[3] = '\0';
    suf[0] = suf[1] ? '-' : '\0';

<<<<<<< HEAD
    sprintf(model, "Motorola MVME%x%s", p->brdno, suf);
=======
    sprintf(model, "Motorola MVME%x%s", be16_to_cpu(p->brdno), suf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}


static void mvme16x_get_hardware_list(struct seq_file *m)
{
<<<<<<< HEAD
    p_bdid p = &mvme_bdid;

    if (p->brdno == 0x0162 || p->brdno == 0x0172)
=======
    uint16_t brdno = be16_to_cpu(mvme_bdid.brdno);

    if (brdno == 0x0162 || brdno == 0x0172)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
    {
	unsigned char rev = *(unsigned char *)MVME162_VERSION_REG;

	seq_printf (m, "VMEchip2        %spresent\n",
			rev & MVME16x_CONFIG_NO_VMECHIP2 ? "NOT " : "");
	seq_printf (m, "SCSI interface  %spresent\n",
			rev & MVME16x_CONFIG_NO_SCSICHIP ? "NOT " : "");
	seq_printf (m, "Ethernet i/f    %spresent\n",
			rev & MVME16x_CONFIG_NO_ETHERNET ? "NOT " : "");
    }
}

/*
 * This function is called during kernel startup to initialize
 * the mvme16x IRQ handling routines.  Should probably ensure
 * that the base vectors for the VMEChip2 and PCCChip2 are valid.
 */

static void __init mvme16x_init_IRQ (void)
{
	m68k_setup_user_interrupt(VEC_USER, 192);
}

<<<<<<< HEAD
#define pcc2chip	((volatile u_char *)0xfff42000)
#define PccSCCMICR	0x1d
#define PccSCCTICR	0x1e
#define PccSCCRICR	0x1f
#define PccTPIACKR	0x25
=======
#define PCC2CHIP   (0xfff42000)
#define PCCSCCMICR (PCC2CHIP + 0x1d)
#define PCCSCCTICR (PCC2CHIP + 0x1e)
#define PCCSCCRICR (PCC2CHIP + 0x1f)
#define PCCTPIACKR (PCC2CHIP + 0x25)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_EARLY_PRINTK

/**** cd2401 registers ****/
#define CD2401_ADDR	(0xfff45000)

#define CyGFRCR         (0x81)
#define CyCCR		(0x13)
#define      CyCLR_CHAN		(0x40)
#define      CyINIT_CHAN	(0x20)
#define      CyCHIP_RESET	(0x10)
#define      CyENB_XMTR		(0x08)
#define      CyDIS_XMTR		(0x04)
#define      CyENB_RCVR		(0x02)
#define      CyDIS_RCVR		(0x01)
#define CyCAR		(0xee)
#define CyIER		(0x11)
#define      CyMdmCh		(0x80)
#define      CyRxExc		(0x20)
#define      CyRxData		(0x08)
#define      CyTxMpty		(0x02)
#define      CyTxRdy		(0x01)
#define CyLICR		(0x26)
#define CyRISR		(0x89)
#define      CyTIMEOUT		(0x80)
#define      CySPECHAR		(0x70)
#define      CyOVERRUN		(0x08)
#define      CyPARITY		(0x04)
#define      CyFRAME		(0x02)
#define      CyBREAK		(0x01)
#define CyREOIR		(0x84)
#define CyTEOIR		(0x85)
#define CyMEOIR		(0x86)
#define      CyNOTRANS		(0x08)
#define CyRFOC		(0x30)
#define CyRDR		(0xf8)
#define CyTDR		(0xf8)
#define CyMISR		(0x8b)
#define CyRISR		(0x89)
#define CyTISR		(0x8a)
#define CyMSVR1		(0xde)
#define CyMSVR2		(0xdf)
#define      CyDSR		(0x80)
#define      CyDCD		(0x40)
#define      CyCTS		(0x20)
#define      CyDTR		(0x02)
#define      CyRTS		(0x01)
#define CyRTPRL		(0x25)
#define CyRTPRH		(0x24)
#define CyCOR1		(0x10)
#define      CyPARITY_NONE	(0x00)
#define      CyPARITY_E		(0x40)
#define      CyPARITY_O		(0xC0)
#define      Cy_5_BITS		(0x04)
#define      Cy_6_BITS		(0x05)
#define      Cy_7_BITS		(0x06)
#define      Cy_8_BITS		(0x07)
#define CyCOR2		(0x17)
#define      CyETC		(0x20)
#define      CyCtsAE		(0x02)
#define CyCOR3		(0x16)
#define      Cy_1_STOP		(0x02)
#define      Cy_2_STOP		(0x04)
#define CyCOR4		(0x15)
#define      CyREC_FIFO		(0x0F)  /* Receive FIFO threshold */
#define CyCOR5		(0x14)
#define CyCOR6		(0x18)
#define CyCOR7		(0x07)
#define CyRBPR		(0xcb)
#define CyRCOR		(0xc8)
#define CyTBPR		(0xc3)
#define CyTCOR		(0xc0)
#define CySCHR1		(0x1f)
#define CySCHR2 	(0x1e)
#define CyTPR		(0xda)
#define CyPILR1		(0xe3)
#define CyPILR2		(0xe0)
#define CyPILR3		(0xe1)
#define CyCMR		(0x1b)
#define      CyASYNC		(0x02)
#define CyLICR          (0x26)
#define CyLIVR          (0x09)
#define CySCRL		(0x23)
#define CySCRH		(0x22)
#define CyTFTC		(0x80)

<<<<<<< HEAD
static void cons_write(struct console *co, const char *str, unsigned count)
{
	volatile unsigned char *base_addr = (u_char *)CD2401_ADDR;
	volatile u_char sink;
=======
void mvme16x_cons_write(struct console *co, const char *str, unsigned count)
{
	volatile unsigned char *base_addr = (u_char *)CD2401_ADDR;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u_char ier;
	int port;
	u_char do_lf = 0;
	int i = 0;

	/* Ensure transmitter is enabled! */

	port = 0;
	base_addr[CyCAR] = (u_char)port;
	while (base_addr[CyCCR])
		;
	base_addr[CyCCR] = CyENB_XMTR;

	ier = base_addr[CyIER];
	base_addr[CyIER] = CyTxMpty;

	while (1) {
<<<<<<< HEAD
		if (pcc2chip[PccSCCTICR] & 0x20)
		{
			/* We have a Tx int. Acknowledge it */
			sink = pcc2chip[PccTPIACKR];
=======
		if (in_8(PCCSCCTICR) & 0x20)
		{
			/* We have a Tx int. Acknowledge it */
			in_8(PCCTPIACKR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if ((base_addr[CyLICR] >> 2) == port) {
				if (i == count) {
					/* Last char of string is now output */
					base_addr[CyTEOIR] = CyNOTRANS;
					break;
				}
				if (do_lf) {
					base_addr[CyTDR] = '\n';
					str++;
					i++;
					do_lf = 0;
				}
				else if (*str == '\n') {
					base_addr[CyTDR] = '\r';
					do_lf = 1;
				}
				else {
					base_addr[CyTDR] = *str++;
					i++;
				}
				base_addr[CyTEOIR] = 0;
			}
			else
				base_addr[CyTEOIR] = CyNOTRANS;
		}
	}

	base_addr[CyIER] = ier;
}

<<<<<<< HEAD
static struct console cons_info =
{
	.name	= "sercon",
	.write	= cons_write,
	.flags	= CON_PRINTBUFFER | CON_BOOT,
	.index	= -1,
};

static void __init mvme16x_early_console(void)
{
	register_console(&cons_info);

	printk(KERN_INFO "MVME16x: early console registered\n");
}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

void __init config_mvme16x(void)
{
    p_bdid p = &mvme_bdid;
    char id[40];
<<<<<<< HEAD

    mach_max_dma_address = 0xffffffff;
    mach_sched_init      = mvme16x_sched_init;
    mach_init_IRQ        = mvme16x_init_IRQ;
    mach_gettimeoffset   = mvme16x_gettimeoffset;
    mach_hwclk           = mvme16x_hwclk;
    mach_set_clock_mmss	 = mvme16x_set_clock_mmss;
=======
    uint16_t brdno = be16_to_cpu(p->brdno);

    mach_sched_init      = mvme16x_sched_init;
    mach_init_IRQ        = mvme16x_init_IRQ;
    mach_hwclk           = mvme16x_hwclk;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
    mach_reset		 = mvme16x_reset;
    mach_get_model       = mvme16x_get_model;
    mach_get_hardware_list = mvme16x_get_hardware_list;

    /* Report board revision */

    if (strncmp("BDID", p->bdid, 4))
    {
<<<<<<< HEAD
	printk ("\n\nBug call .BRD_ID returned garbage - giving up\n\n");
=======
	pr_crit("Bug call .BRD_ID returned garbage - giving up\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	while (1)
		;
    }
    /* Board type is only set by newer versions of vmelilo/tftplilo */
    if (vme_brdtype == 0)
<<<<<<< HEAD
	vme_brdtype = p->brdno;

    mvme16x_get_model(id);
    printk ("\nBRD_ID: %s   BUG %x.%x %02x/%02x/%02x\n", id, p->rev>>4,
					p->rev&0xf, p->yr, p->mth, p->day);
    if (p->brdno == 0x0162 || p->brdno == 0x172)
=======
	vme_brdtype = brdno;

    mvme16x_get_model(id);
    pr_info("BRD_ID: %s   BUG %x.%x %02x/%02x/%02x\n", id, p->rev >> 4,
	    p->rev & 0xf, p->yr, p->mth, p->day);
    if (brdno == 0x0162 || brdno == 0x172)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
    {
	unsigned char rev = *(unsigned char *)MVME162_VERSION_REG;

	mvme16x_config = rev | MVME16x_CONFIG_GOT_SCCA;

<<<<<<< HEAD
	printk ("MVME%x Hardware status:\n", p->brdno);
	printk ("    CPU Type           68%s040\n",
			rev & MVME16x_CONFIG_GOT_FPU ? "" : "LC");
	printk ("    CPU clock          %dMHz\n",
			rev & MVME16x_CONFIG_SPEED_32 ? 32 : 25);
	printk ("    VMEchip2           %spresent\n",
			rev & MVME16x_CONFIG_NO_VMECHIP2 ? "NOT " : "");
	printk ("    SCSI interface     %spresent\n",
			rev & MVME16x_CONFIG_NO_SCSICHIP ? "NOT " : "");
	printk ("    Ethernet interface %spresent\n",
			rev & MVME16x_CONFIG_NO_ETHERNET ? "NOT " : "");
=======
	pr_info("MVME%x Hardware status:\n", brdno);
	pr_info("    CPU Type           68%s040\n",
		rev & MVME16x_CONFIG_GOT_FPU ? "" : "LC");
	pr_info("    CPU clock          %dMHz\n",
		rev & MVME16x_CONFIG_SPEED_32 ? 32 : 25);
	pr_info("    VMEchip2           %spresent\n",
		rev & MVME16x_CONFIG_NO_VMECHIP2 ? "NOT " : "");
	pr_info("    SCSI interface     %spresent\n",
		rev & MVME16x_CONFIG_NO_SCSICHIP ? "NOT " : "");
	pr_info("    Ethernet interface %spresent\n",
		rev & MVME16x_CONFIG_NO_ETHERNET ? "NOT " : "");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
    }
    else
    {
	mvme16x_config = MVME16x_CONFIG_GOT_LP | MVME16x_CONFIG_GOT_CD2401;
<<<<<<< HEAD

	/* Dont allow any interrupts from the CD2401 until the interrupt */
	/* handlers are installed					 */

	pcc2chip[PccSCCMICR] = 0x10;
	pcc2chip[PccSCCTICR] = 0x10;
	pcc2chip[PccSCCRICR] = 0x10;
#ifdef CONFIG_EARLY_PRINTK
	mvme16x_early_console();
#endif
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
    }
}

static irqreturn_t mvme16x_abort_int (int irq, void *dev_id)
{
<<<<<<< HEAD
	p_bdid p = &mvme_bdid;
	unsigned long *new = (unsigned long *)vectors;
	unsigned long *old = (unsigned long *)0xffe00000;
	volatile unsigned char uc, *ucp;

	if (p->brdno == 0x0162 || p->brdno == 0x172)
=======
	unsigned long *new = (unsigned long *)vectors;
	unsigned long *old = (unsigned long *)0xffe00000;
	volatile unsigned char uc, *ucp;
	uint16_t brdno = be16_to_cpu(mvme_bdid.brdno);

	if (brdno == 0x0162 || brdno == 0x172)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{
		ucp = (volatile unsigned char *)0xfff42043;
		uc = *ucp | 8;
		*ucp = uc;
	}
	else
	{
		*(volatile unsigned long *)0xfff40074 = 0x40000000;
	}
	*(new+4) = *(old+4);		/* Illegal instruction */
	*(new+9) = *(old+9);		/* Trace */
	*(new+47) = *(old+47);		/* Trap #15 */

<<<<<<< HEAD
	if (p->brdno == 0x0162 || p->brdno == 0x172)
=======
	if (brdno == 0x0162 || brdno == 0x172)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		*(new+0x5e) = *(old+0x5e);	/* ABORT switch */
	else
		*(new+0x6e) = *(old+0x6e);	/* ABORT switch */
	return IRQ_HANDLED;
}

<<<<<<< HEAD
static irqreturn_t mvme16x_timer_int (int irq, void *dev_id)
{
    *(volatile unsigned char *)0xfff4201b |= 8;
    return tick_handler(irq, dev_id);
}

void mvme16x_sched_init (irq_handler_t timer_routine)
{
    p_bdid p = &mvme_bdid;
    int irq;

    tick_handler = timer_routine;
    /* Using PCCchip2 or MC2 chip tick timer 1 */
    *(volatile unsigned long *)0xfff42008 = 0;
    *(volatile unsigned long *)0xfff42004 = 10000;	/* 10ms */
    *(volatile unsigned char *)0xfff42017 |= 3;
    *(volatile unsigned char *)0xfff4201b = 0x16;
    if (request_irq(MVME16x_IRQ_TIMER, mvme16x_timer_int, 0,
				"timer", mvme16x_timer_int))
	panic ("Couldn't register timer int");

    if (p->brdno == 0x0162 || p->brdno == 0x172)
=======
static u64 mvme16x_read_clk(struct clocksource *cs);

static struct clocksource mvme16x_clk = {
	.name   = "pcc",
	.rating = 250,
	.read   = mvme16x_read_clk,
	.mask   = CLOCKSOURCE_MASK(32),
	.flags  = CLOCK_SOURCE_IS_CONTINUOUS,
};

static u32 clk_total;

#define PCC_TIMER_CLOCK_FREQ 1000000
#define PCC_TIMER_CYCLES     (PCC_TIMER_CLOCK_FREQ / HZ)

#define PCCTCMP1             (PCC2CHIP + 0x04)
#define PCCTCNT1             (PCC2CHIP + 0x08)
#define PCCTOVR1             (PCC2CHIP + 0x17)
#define PCCTIC1              (PCC2CHIP + 0x1b)

#define PCCTOVR1_TIC_EN      0x01
#define PCCTOVR1_COC_EN      0x02
#define PCCTOVR1_OVR_CLR     0x04

#define PCCTIC1_INT_LEVEL    6
#define PCCTIC1_INT_CLR      0x08
#define PCCTIC1_INT_EN       0x10

static irqreturn_t mvme16x_timer_int (int irq, void *dev_id)
{
	unsigned long flags;

	local_irq_save(flags);
	out_8(PCCTOVR1, PCCTOVR1_OVR_CLR | PCCTOVR1_TIC_EN | PCCTOVR1_COC_EN);
	out_8(PCCTIC1, PCCTIC1_INT_EN | PCCTIC1_INT_CLR | PCCTIC1_INT_LEVEL);
	clk_total += PCC_TIMER_CYCLES;
	legacy_timer_tick(1);
	local_irq_restore(flags);

	return IRQ_HANDLED;
}

void mvme16x_sched_init(void)
{
    uint16_t brdno = be16_to_cpu(mvme_bdid.brdno);
    int irq;

    /* Using PCCchip2 or MC2 chip tick timer 1 */
    if (request_irq(MVME16x_IRQ_TIMER, mvme16x_timer_int, IRQF_TIMER, "timer",
                    NULL))
	panic ("Couldn't register timer int");

    out_be32(PCCTCNT1, 0);
    out_be32(PCCTCMP1, PCC_TIMER_CYCLES);
    out_8(PCCTOVR1, PCCTOVR1_OVR_CLR | PCCTOVR1_TIC_EN | PCCTOVR1_COC_EN);
    out_8(PCCTIC1, PCCTIC1_INT_EN | PCCTIC1_INT_CLR | PCCTIC1_INT_LEVEL);

    clocksource_register_hz(&mvme16x_clk, PCC_TIMER_CLOCK_FREQ);

    if (brdno == 0x0162 || brdno == 0x172)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	irq = MVME162_IRQ_ABORT;
    else
        irq = MVME167_IRQ_ABORT;
    if (request_irq(irq, mvme16x_abort_int, 0,
				"abort", mvme16x_abort_int))
	panic ("Couldn't register abort int");
}

<<<<<<< HEAD

/* This is always executed with interrupts disabled.  */
unsigned long mvme16x_gettimeoffset (void)
{
    return (*(volatile unsigned long *)0xfff42008);
=======
static u64 mvme16x_read_clk(struct clocksource *cs)
{
	unsigned long flags;
	u8 overflow, tmp;
	u32 ticks;

	local_irq_save(flags);
	tmp = in_8(PCCTOVR1) >> 4;
	ticks = in_be32(PCCTCNT1);
	overflow = in_8(PCCTOVR1) >> 4;
	if (overflow != tmp)
		ticks = in_be32(PCCTCNT1);
	ticks += overflow * PCC_TIMER_CYCLES;
	ticks += clk_total;
	local_irq_restore(flags);

	return ticks;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int bcd2int (unsigned char b)
{
	return ((b>>4)*10 + (b&15));
}

int mvme16x_hwclk(int op, struct rtc_time *t)
{
<<<<<<< HEAD
#warning check me!
	if (!op) {
		rtc->ctrl = RTC_READ;
		t->tm_year = bcd2int (rtc->bcd_year);
		t->tm_mon  = bcd2int (rtc->bcd_mth);
=======
	if (!op) {
		rtc->ctrl = RTC_READ;
		t->tm_year = bcd2int (rtc->bcd_year);
		t->tm_mon  = bcd2int(rtc->bcd_mth) - 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		t->tm_mday = bcd2int (rtc->bcd_dom);
		t->tm_hour = bcd2int (rtc->bcd_hr);
		t->tm_min  = bcd2int (rtc->bcd_min);
		t->tm_sec  = bcd2int (rtc->bcd_sec);
		rtc->ctrl = 0;
<<<<<<< HEAD
	}
	return 0;
}

int mvme16x_set_clock_mmss (unsigned long nowtime)
{
	return 0;
}

=======
		if (t->tm_year < 70)
			t->tm_year += 100;
	} else {
		/* FIXME Setting the time is not yet supported */
		return -EOPNOTSUPP;
	}
	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
