<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *          mxser.c  -- MOXA Smartio/Industio family multiport serial driver.
 *
 *      Copyright (C) 1999-2006  Moxa Technologies (support@moxa.com).
 *	Copyright (C) 2006-2008  Jiri Slaby <jirislaby@gmail.com>
 *
 *      This code is loosely based on the 1.8 moxa driver which is based on
 *	Linux serial driver, written by Linus Torvalds, Theodore T'so and
 *	others.
 *
<<<<<<< HEAD
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	Fed through a cleanup, indent and remove of non 2.6 code by Alan Cox
 *	<alan@lxorguk.ukuu.org.uk>. The original 1.8 code is available on
 *	www.moxa.com.
 *	- Fixed x86_64 cleanness
 */

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/interrupt.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial.h>
#include <linux/serial_reg.h>
#include <linux/major.h>
#include <linux/string.h>
#include <linux/fcntl.h>
#include <linux/ptrace.h>
#include <linux/ioport.h>
#include <linux/mm.h>
#include <linux/delay.h>
#include <linux/pci.h>
#include <linux/bitops.h>
#include <linux/slab.h>
#include <linux/ratelimit.h>

#include <asm/io.h>
#include <asm/irq.h>
<<<<<<< HEAD
#include <asm/uaccess.h>

#include "mxser.h"

#define	MXSER_VERSION	"2.0.5"		/* 1.14 */
=======
#include <linux/uaccess.h>

/*
 *	Semi-public control interfaces
 */

/*
 *	MOXA ioctls
 */

#define MOXA			0x400
#define MOXA_SET_OP_MODE	(MOXA + 66)
#define MOXA_GET_OP_MODE	(MOXA + 67)

#define RS232_MODE		0
#define RS485_2WIRE_MODE	1
#define RS422_MODE		2
#define RS485_4WIRE_MODE	3
#define OP_MODE_MASK		3

/* --------------------------------------------------- */

/*
 * Follow just what Moxa Must chip defines.
 *
 * When LCR register (offset 0x03) is written the following value, the Must chip
 * will enter enhanced mode. And a write to EFR (offset 0x02) bit 6,7 will
 * change bank.
 */
#define MOXA_MUST_ENTER_ENHANCED	0xBF

/* when enhanced mode is enabled, access to general bank register */
#define MOXA_MUST_GDL_REGISTER		0x07
#define MOXA_MUST_GDL_MASK		0x7F
#define MOXA_MUST_GDL_HAS_BAD_DATA	0x80

#define MOXA_MUST_LSR_RERR		0x80	/* error in receive FIFO */
/* enhanced register bank select and enhanced mode setting register */
/* This works only when LCR register equals to 0xBF */
#define MOXA_MUST_EFR_REGISTER		0x02
#define MOXA_MUST_EFR_EFRB_ENABLE	0x10 /* enhanced mode enable */
/* enhanced register bank set 0, 1, 2 */
#define MOXA_MUST_EFR_BANK0		0x00
#define MOXA_MUST_EFR_BANK1		0x40
#define MOXA_MUST_EFR_BANK2		0x80
#define MOXA_MUST_EFR_BANK3		0xC0
#define MOXA_MUST_EFR_BANK_MASK		0xC0

/* set XON1 value register, when LCR=0xBF and change to bank0 */
#define MOXA_MUST_XON1_REGISTER		0x04

/* set XON2 value register, when LCR=0xBF and change to bank0 */
#define MOXA_MUST_XON2_REGISTER		0x05

/* set XOFF1 value register, when LCR=0xBF and change to bank0 */
#define MOXA_MUST_XOFF1_REGISTER	0x06

/* set XOFF2 value register, when LCR=0xBF and change to bank0 */
#define MOXA_MUST_XOFF2_REGISTER	0x07

#define MOXA_MUST_RBRTL_REGISTER	0x04
#define MOXA_MUST_RBRTH_REGISTER	0x05
#define MOXA_MUST_RBRTI_REGISTER	0x06
#define MOXA_MUST_THRTL_REGISTER	0x07
#define MOXA_MUST_ENUM_REGISTER		0x04
#define MOXA_MUST_HWID_REGISTER		0x05
#define MOXA_MUST_ECR_REGISTER		0x06
#define MOXA_MUST_CSR_REGISTER		0x07

#define MOXA_MUST_FCR_GDA_MODE_ENABLE	0x20 /* good data mode enable */
#define MOXA_MUST_FCR_GDA_ONLY_ENABLE	0x10 /* only good data put into RxFIFO */

#define MOXA_MUST_IER_ECTSI		0x80 /* enable CTS interrupt */
#define MOXA_MUST_IER_ERTSI		0x40 /* enable RTS interrupt */
#define MOXA_MUST_IER_XINT		0x20 /* enable Xon/Xoff interrupt */
#define MOXA_MUST_IER_EGDAI		0x10 /* enable GDA interrupt */

#define MOXA_MUST_RECV_ISR		(UART_IER_RDI | MOXA_MUST_IER_EGDAI)

/* GDA interrupt pending */
#define MOXA_MUST_IIR_GDA		0x1C
#define MOXA_MUST_IIR_RDA		0x04
#define MOXA_MUST_IIR_RTO		0x0C
#define MOXA_MUST_IIR_LSR		0x06

/* received Xon/Xoff or specical interrupt pending */
#define MOXA_MUST_IIR_XSC		0x10

/* RTS/CTS change state interrupt pending */
#define MOXA_MUST_IIR_RTSCTS		0x20
#define MOXA_MUST_IIR_MASK		0x3E

#define MOXA_MUST_MCR_XON_FLAG		0x40
#define MOXA_MUST_MCR_XON_ANY		0x80
#define MOXA_MUST_MCR_TX_XON		0x08

#define MOXA_MUST_EFR_SF_MASK		0x0F /* software flow control on chip mask value */
#define MOXA_MUST_EFR_SF_TX1		0x08 /* send Xon1/Xoff1 */
#define MOXA_MUST_EFR_SF_TX2		0x04 /* send Xon2/Xoff2 */
#define MOXA_MUST_EFR_SF_TX12		0x0C /* send Xon1,Xon2/Xoff1,Xoff2 */
#define MOXA_MUST_EFR_SF_TX_NO		0x00 /* don't send Xon/Xoff */
#define MOXA_MUST_EFR_SF_TX_MASK	0x0C /* Tx software flow control mask */
#define MOXA_MUST_EFR_SF_RX_NO		0x00 /* don't receive Xon/Xoff */
#define MOXA_MUST_EFR_SF_RX1		0x02 /* receive Xon1/Xoff1 */
#define MOXA_MUST_EFR_SF_RX2		0x01 /* receive Xon2/Xoff2 */
#define MOXA_MUST_EFR_SF_RX12		0x03 /* receive Xon1,Xon2/Xoff1,Xoff2 */
#define MOXA_MUST_EFR_SF_RX_MASK	0x03 /* Rx software flow control mask */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define	MXSERMAJOR	 174

#define MXSER_BOARDS		4	/* Max. boards */
#define MXSER_PORTS_PER_BOARD	8	/* Max. ports per board */
#define MXSER_PORTS		(MXSER_BOARDS * MXSER_PORTS_PER_BOARD)
#define MXSER_ISR_PASS_LIMIT	100

<<<<<<< HEAD
/*CheckIsMoxaMust return value*/
#define MOXA_OTHER_UART		0x00
#define MOXA_MUST_MU150_HWID	0x01
#define MOXA_MUST_MU860_HWID	0x02

#define WAKEUP_CHARS		256

#define UART_MCR_AFE		0x20
#define UART_LSR_SPECIAL	0x1E

#define PCI_DEVICE_ID_POS104UL	0x1044
#define PCI_DEVICE_ID_CB108	0x1080
#define PCI_DEVICE_ID_CP102UF	0x1023
#define PCI_DEVICE_ID_CP112UL	0x1120
#define PCI_DEVICE_ID_CB114	0x1142
#define PCI_DEVICE_ID_CP114UL	0x1143
#define PCI_DEVICE_ID_CB134I	0x1341
#define PCI_DEVICE_ID_CP138U	0x1380


#define C168_ASIC_ID    1
#define C104_ASIC_ID    2
#define C102_ASIC_ID	0xB
#define CI132_ASIC_ID	4
#define CI134_ASIC_ID	3
#define CI104J_ASIC_ID  5

#define MXSER_HIGHBAUD	1
#define MXSER_HAS2	2

/* This is only for PCI */
static const struct {
	int type;
	int tx_fifo;
	int rx_fifo;
	int xmit_fifo_size;
	int rx_high_water;
	int rx_trigger;
	int rx_low_water;
	long max_baud;
} Gpci_uart_info[] = {
	{MOXA_OTHER_UART, 16, 16, 16, 14, 14, 1, 921600L},
	{MOXA_MUST_MU150_HWID, 64, 64, 64, 48, 48, 16, 230400L},
	{MOXA_MUST_MU860_HWID, 128, 128, 128, 96, 96, 32, 921600L}
};
#define UART_INFO_NUM	ARRAY_SIZE(Gpci_uart_info)

struct mxser_cardinfo {
	char *name;
	unsigned int nports;
	unsigned int flags;
};

static const struct mxser_cardinfo mxser_cards[] = {
/* 0*/	{ "C168 series",	8, },
	{ "C104 series",	4, },
	{ "CI-104J series",	4, },
	{ "C168H/PCI series",	8, },
	{ "C104H/PCI series",	4, },
/* 5*/	{ "C102 series",	4, MXSER_HAS2 },	/* C102-ISA */
	{ "CI-132 series",	4, MXSER_HAS2 },
	{ "CI-134 series",	4, },
	{ "CP-132 series",	2, },
	{ "CP-114 series",	4, },
/*10*/	{ "CT-114 series",	4, },
	{ "CP-102 series",	2, MXSER_HIGHBAUD },
	{ "CP-104U series",	4, },
	{ "CP-168U series",	8, },
	{ "CP-132U series",	2, },
/*15*/	{ "CP-134U series",	4, },
	{ "CP-104JU series",	4, },
	{ "Moxa UC7000 Serial",	8, },		/* RC7000 */
	{ "CP-118U series",	8, },
	{ "CP-102UL series",	2, },
/*20*/	{ "CP-102U series",	2, },
	{ "CP-118EL series",	8, },
	{ "CP-168EL series",	8, },
	{ "CP-104EL series",	4, },
	{ "CB-108 series",	8, },
/*25*/	{ "CB-114 series",	4, },
	{ "CB-134I series",	4, },
	{ "CP-138U series",	8, },
	{ "POS-104UL series",	4, },
	{ "CP-114UL series",	4, },
/*30*/	{ "CP-102UF series",	2, },
	{ "CP-112UL series",	2, },
};

/* driver_data correspond to the lines in the structure above
   see also ISA probe function before you change something */
static struct pci_device_id mxser_pcibrds[] = {
	{ PCI_VDEVICE(MOXA, PCI_DEVICE_ID_MOXA_C168),	.driver_data = 3 },
	{ PCI_VDEVICE(MOXA, PCI_DEVICE_ID_MOXA_C104),	.driver_data = 4 },
	{ PCI_VDEVICE(MOXA, PCI_DEVICE_ID_MOXA_CP132),	.driver_data = 8 },
	{ PCI_VDEVICE(MOXA, PCI_DEVICE_ID_MOXA_CP114),	.driver_data = 9 },
	{ PCI_VDEVICE(MOXA, PCI_DEVICE_ID_MOXA_CT114),	.driver_data = 10 },
	{ PCI_VDEVICE(MOXA, PCI_DEVICE_ID_MOXA_CP102),	.driver_data = 11 },
	{ PCI_VDEVICE(MOXA, PCI_DEVICE_ID_MOXA_CP104U),	.driver_data = 12 },
	{ PCI_VDEVICE(MOXA, PCI_DEVICE_ID_MOXA_CP168U),	.driver_data = 13 },
	{ PCI_VDEVICE(MOXA, PCI_DEVICE_ID_MOXA_CP132U),	.driver_data = 14 },
	{ PCI_VDEVICE(MOXA, PCI_DEVICE_ID_MOXA_CP134U),	.driver_data = 15 },
	{ PCI_VDEVICE(MOXA, PCI_DEVICE_ID_MOXA_CP104JU),.driver_data = 16 },
	{ PCI_VDEVICE(MOXA, PCI_DEVICE_ID_MOXA_RC7000),	.driver_data = 17 },
	{ PCI_VDEVICE(MOXA, PCI_DEVICE_ID_MOXA_CP118U),	.driver_data = 18 },
	{ PCI_VDEVICE(MOXA, PCI_DEVICE_ID_MOXA_CP102UL),.driver_data = 19 },
	{ PCI_VDEVICE(MOXA, PCI_DEVICE_ID_MOXA_CP102U),	.driver_data = 20 },
	{ PCI_VDEVICE(MOXA, PCI_DEVICE_ID_MOXA_CP118EL),.driver_data = 21 },
	{ PCI_VDEVICE(MOXA, PCI_DEVICE_ID_MOXA_CP168EL),.driver_data = 22 },
	{ PCI_VDEVICE(MOXA, PCI_DEVICE_ID_MOXA_CP104EL),.driver_data = 23 },
	{ PCI_VDEVICE(MOXA, PCI_DEVICE_ID_CB108),	.driver_data = 24 },
	{ PCI_VDEVICE(MOXA, PCI_DEVICE_ID_CB114),	.driver_data = 25 },
	{ PCI_VDEVICE(MOXA, PCI_DEVICE_ID_CB134I),	.driver_data = 26 },
	{ PCI_VDEVICE(MOXA, PCI_DEVICE_ID_CP138U),	.driver_data = 27 },
	{ PCI_VDEVICE(MOXA, PCI_DEVICE_ID_POS104UL),	.driver_data = 28 },
	{ PCI_VDEVICE(MOXA, PCI_DEVICE_ID_CP114UL),	.driver_data = 29 },
	{ PCI_VDEVICE(MOXA, PCI_DEVICE_ID_CP102UF),	.driver_data = 30 },
	{ PCI_VDEVICE(MOXA, PCI_DEVICE_ID_CP112UL),	.driver_data = 31 },
=======
#define WAKEUP_CHARS		256

#define MXSER_BAUD_BASE		921600
#define MXSER_CUSTOM_DIVISOR	(MXSER_BAUD_BASE * 16)

#define PCI_DEVICE_ID_MOXA_RC7000	0x0001
#define PCI_DEVICE_ID_MOXA_CP102	0x1020
#define PCI_DEVICE_ID_MOXA_CP102UL	0x1021
#define PCI_DEVICE_ID_MOXA_CP102U	0x1022
#define PCI_DEVICE_ID_MOXA_CP102UF	0x1023
#define PCI_DEVICE_ID_MOXA_C104		0x1040
#define PCI_DEVICE_ID_MOXA_CP104U	0x1041
#define PCI_DEVICE_ID_MOXA_CP104JU	0x1042
#define PCI_DEVICE_ID_MOXA_CP104EL	0x1043
#define PCI_DEVICE_ID_MOXA_POS104UL	0x1044
#define PCI_DEVICE_ID_MOXA_CB108	0x1080
#define PCI_DEVICE_ID_MOXA_CP112UL	0x1120
#define PCI_DEVICE_ID_MOXA_CT114	0x1140
#define PCI_DEVICE_ID_MOXA_CP114	0x1141
#define PCI_DEVICE_ID_MOXA_CB114	0x1142
#define PCI_DEVICE_ID_MOXA_CP114UL	0x1143
#define PCI_DEVICE_ID_MOXA_CP118U	0x1180
#define PCI_DEVICE_ID_MOXA_CP118EL	0x1181
#define PCI_DEVICE_ID_MOXA_CP132	0x1320
#define PCI_DEVICE_ID_MOXA_CP132U	0x1321
#define PCI_DEVICE_ID_MOXA_CP134U	0x1340
#define PCI_DEVICE_ID_MOXA_CB134I	0x1341
#define PCI_DEVICE_ID_MOXA_CP138U	0x1380
#define PCI_DEVICE_ID_MOXA_C168		0x1680
#define PCI_DEVICE_ID_MOXA_CP168U	0x1681
#define PCI_DEVICE_ID_MOXA_CP168EL	0x1682

#define MXSER_NPORTS(ddata)		((ddata) & 0xffU)
#define MXSER_HIGHBAUD			0x0100

enum mxser_must_hwid {
	MOXA_OTHER_UART		= 0x00,
	MOXA_MUST_MU150_HWID	= 0x01,
	MOXA_MUST_MU860_HWID	= 0x02,
};

static const struct {
	u8 type;
	u8 fifo_size;
	u8 rx_high_water;
	u8 rx_low_water;
	speed_t max_baud;
} Gpci_uart_info[] = {
	{ MOXA_OTHER_UART,	 16, 14,  1, 921600 },
	{ MOXA_MUST_MU150_HWID,	 64, 48, 16, 230400 },
	{ MOXA_MUST_MU860_HWID, 128, 96, 32, 921600 }
};
#define UART_INFO_NUM	ARRAY_SIZE(Gpci_uart_info)


/* driver_data correspond to the lines in the structure above
   see also ISA probe function before you change something */
static const struct pci_device_id mxser_pcibrds[] = {
	{ PCI_DEVICE_DATA(MOXA, C168,		8) },
	{ PCI_DEVICE_DATA(MOXA, C104,		4) },
	{ PCI_DEVICE_DATA(MOXA, CP132,		2) },
	{ PCI_DEVICE_DATA(MOXA, CP114,		4) },
	{ PCI_DEVICE_DATA(MOXA, CT114,		4) },
	{ PCI_DEVICE_DATA(MOXA, CP102,		2 | MXSER_HIGHBAUD) },
	{ PCI_DEVICE_DATA(MOXA, CP104U,		4) },
	{ PCI_DEVICE_DATA(MOXA, CP168U,		8) },
	{ PCI_DEVICE_DATA(MOXA, CP132U,		2) },
	{ PCI_DEVICE_DATA(MOXA, CP134U,		4) },
	{ PCI_DEVICE_DATA(MOXA, CP104JU,	4) },
	{ PCI_DEVICE_DATA(MOXA, RC7000,		8) }, /* RC7000 */
	{ PCI_DEVICE_DATA(MOXA, CP118U,		8) },
	{ PCI_DEVICE_DATA(MOXA, CP102UL,	2) },
	{ PCI_DEVICE_DATA(MOXA, CP102U,		2) },
	{ PCI_DEVICE_DATA(MOXA, CP118EL,	8) },
	{ PCI_DEVICE_DATA(MOXA, CP168EL,	8) },
	{ PCI_DEVICE_DATA(MOXA, CP104EL,	4) },
	{ PCI_DEVICE_DATA(MOXA, CB108,		8) },
	{ PCI_DEVICE_DATA(MOXA, CB114,		4) },
	{ PCI_DEVICE_DATA(MOXA, CB134I,		4) },
	{ PCI_DEVICE_DATA(MOXA, CP138U,		8) },
	{ PCI_DEVICE_DATA(MOXA, POS104UL,	4) },
	{ PCI_DEVICE_DATA(MOXA, CP114UL,	4) },
	{ PCI_DEVICE_DATA(MOXA, CP102UF,	2) },
	{ PCI_DEVICE_DATA(MOXA, CP112UL,	2) },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ }
};
MODULE_DEVICE_TABLE(pci, mxser_pcibrds);

<<<<<<< HEAD
static unsigned long ioaddr[MXSER_BOARDS];
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int ttymajor = MXSERMAJOR;

/* Variables for insmod */

MODULE_AUTHOR("Casper Yang");
MODULE_DESCRIPTION("MOXA Smartio/Industio Family Multiport Board Device Driver");
<<<<<<< HEAD
module_param_array(ioaddr, ulong, NULL, 0);
MODULE_PARM_DESC(ioaddr, "ISA io addresses to look for a moxa board");
module_param(ttymajor, int, 0);
MODULE_LICENSE("GPL");

struct mxser_log {
	int tick;
	unsigned long rxcnt[MXSER_PORTS];
	unsigned long txcnt[MXSER_PORTS];
};

struct mxser_mon {
	unsigned long rxcnt;
	unsigned long txcnt;
	unsigned long up_rxcnt;
	unsigned long up_txcnt;
	int modem_status;
	unsigned char hold_reason;
};

struct mxser_mon_ext {
	unsigned long rx_cnt[32];
	unsigned long tx_cnt[32];
	unsigned long up_rxcnt[32];
	unsigned long up_txcnt[32];
	int modem_status[32];

	long baudrate[32];
	int databits[32];
	int stopbits[32];
	int parity[32];
	int flowctrl[32];
	int fifo[32];
	int iftype[32];
};

=======
module_param(ttymajor, int, 0);
MODULE_LICENSE("GPL");

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct mxser_board;

struct mxser_port {
	struct tty_port port;
	struct mxser_board *board;

	unsigned long ioaddr;
	unsigned long opmode_ioaddr;
<<<<<<< HEAD
	int max_baud;

	int rx_high_water;
	int rx_trigger;		/* Rx fifo trigger level */
	int rx_low_water;
	int baud_base;		/* max. speed */
	int type;		/* UART type */

	int x_char;		/* xon/xoff character */
	int IER;		/* Interrupt Enable Register */
	int MCR;		/* Modem control register */

	unsigned char stop_rx;
	unsigned char ldisc_stop_rx;

	int custom_divisor;
	unsigned char err_shadow;

	struct async_icount icount; /* kernel counters for 4 input interrupts */
	int timeout;

	int read_status_mask;
	int ignore_status_mask;
	int xmit_fifo_size;
	int xmit_head;
	int xmit_tail;
	int xmit_cnt;

	struct ktermios normal_termios;

	struct mxser_mon mon_data;
=======

	u8 rx_high_water;
	u8 rx_low_water;
	int type;		/* UART type */

	u8 x_char;		/* xon/xoff character */
	u8 IER;			/* Interrupt Enable Register */
	u8 MCR;			/* Modem control register */
	u8 FCR;			/* FIFO control register */

	struct async_icount icount; /* kernel counters for 4 input interrupts */
	unsigned int timeout;

	u8 read_status_mask;
	u8 ignore_status_mask;
	u8 xmit_fifo_size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spinlock_t slock;
};

struct mxser_board {
	unsigned int idx;
<<<<<<< HEAD
	int irq;
	const struct mxser_cardinfo *info;
	unsigned long vector;
	unsigned long vector_mask;

	int chip_flag;
	int uart_type;

	struct mxser_port ports[MXSER_PORTS_PER_BOARD];
};

struct mxser_mstatus {
	tcflag_t cflag;
	int cts;
	int dsr;
	int ri;
	int dcd;
};

static struct mxser_board mxser_boards[MXSER_BOARDS];
static struct tty_driver *mxvar_sdriver;
static struct mxser_log mxvar_log;
static int mxser_set_baud_method[MXSER_PORTS + 1];

static void mxser_enable_must_enchance_mode(unsigned long baseio)
{
	u8 oldlcr;
	u8 efr;

	oldlcr = inb(baseio + UART_LCR);
	outb(MOXA_MUST_ENTER_ENCHANCE, baseio + UART_LCR);

	efr = inb(baseio + MOXA_MUST_EFR_REGISTER);
	efr |= MOXA_MUST_EFR_EFRB_ENABLE;

	outb(efr, baseio + MOXA_MUST_EFR_REGISTER);
	outb(oldlcr, baseio + UART_LCR);
}

#ifdef	CONFIG_PCI
static void mxser_disable_must_enchance_mode(unsigned long baseio)
{
	u8 oldlcr;
	u8 efr;

	oldlcr = inb(baseio + UART_LCR);
	outb(MOXA_MUST_ENTER_ENCHANCE, baseio + UART_LCR);

	efr = inb(baseio + MOXA_MUST_EFR_REGISTER);
	efr &= ~MOXA_MUST_EFR_EFRB_ENABLE;

	outb(efr, baseio + MOXA_MUST_EFR_REGISTER);
	outb(oldlcr, baseio + UART_LCR);
}
#endif

static void mxser_set_must_xon1_value(unsigned long baseio, u8 value)
{
	u8 oldlcr;
	u8 efr;

	oldlcr = inb(baseio + UART_LCR);
	outb(MOXA_MUST_ENTER_ENCHANCE, baseio + UART_LCR);

	efr = inb(baseio + MOXA_MUST_EFR_REGISTER);
	efr &= ~MOXA_MUST_EFR_BANK_MASK;
	efr |= MOXA_MUST_EFR_BANK0;

	outb(efr, baseio + MOXA_MUST_EFR_REGISTER);
=======
	unsigned short nports;
	int irq;
	unsigned long vector;

	enum mxser_must_hwid must_hwid;
	speed_t max_baud;

	struct mxser_port ports[] __counted_by(nports);
};

static DECLARE_BITMAP(mxser_boards, MXSER_BOARDS);
static struct tty_driver *mxvar_sdriver;

static u8 __mxser_must_set_EFR(unsigned long baseio, u8 clear, u8 set,
		bool restore_LCR)
{
	u8 oldlcr, efr;

	oldlcr = inb(baseio + UART_LCR);
	outb(MOXA_MUST_ENTER_ENHANCED, baseio + UART_LCR);

	efr = inb(baseio + MOXA_MUST_EFR_REGISTER);
	efr &= ~clear;
	efr |= set;

	outb(efr, baseio + MOXA_MUST_EFR_REGISTER);

	if (restore_LCR)
		outb(oldlcr, baseio + UART_LCR);

	return oldlcr;
}

static u8 mxser_must_select_bank(unsigned long baseio, u8 bank)
{
	return __mxser_must_set_EFR(baseio, MOXA_MUST_EFR_BANK_MASK, bank,
			false);
}

static void mxser_set_must_xon1_value(unsigned long baseio, u8 value)
{
	u8 oldlcr = mxser_must_select_bank(baseio, MOXA_MUST_EFR_BANK0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	outb(value, baseio + MOXA_MUST_XON1_REGISTER);
	outb(oldlcr, baseio + UART_LCR);
}

static void mxser_set_must_xoff1_value(unsigned long baseio, u8 value)
{
<<<<<<< HEAD
	u8 oldlcr;
	u8 efr;

	oldlcr = inb(baseio + UART_LCR);
	outb(MOXA_MUST_ENTER_ENCHANCE, baseio + UART_LCR);

	efr = inb(baseio + MOXA_MUST_EFR_REGISTER);
	efr &= ~MOXA_MUST_EFR_BANK_MASK;
	efr |= MOXA_MUST_EFR_BANK0;

	outb(efr, baseio + MOXA_MUST_EFR_REGISTER);
=======
	u8 oldlcr = mxser_must_select_bank(baseio, MOXA_MUST_EFR_BANK0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	outb(value, baseio + MOXA_MUST_XOFF1_REGISTER);
	outb(oldlcr, baseio + UART_LCR);
}

static void mxser_set_must_fifo_value(struct mxser_port *info)
{
<<<<<<< HEAD
	u8 oldlcr;
	u8 efr;

	oldlcr = inb(info->ioaddr + UART_LCR);
	outb(MOXA_MUST_ENTER_ENCHANCE, info->ioaddr + UART_LCR);

	efr = inb(info->ioaddr + MOXA_MUST_EFR_REGISTER);
	efr &= ~MOXA_MUST_EFR_BANK_MASK;
	efr |= MOXA_MUST_EFR_BANK1;

	outb(efr, info->ioaddr + MOXA_MUST_EFR_REGISTER);
	outb((u8)info->rx_high_water, info->ioaddr + MOXA_MUST_RBRTH_REGISTER);
	outb((u8)info->rx_trigger, info->ioaddr + MOXA_MUST_RBRTI_REGISTER);
	outb((u8)info->rx_low_water, info->ioaddr + MOXA_MUST_RBRTL_REGISTER);
=======
	u8 oldlcr = mxser_must_select_bank(info->ioaddr, MOXA_MUST_EFR_BANK1);
	outb(info->rx_high_water, info->ioaddr + MOXA_MUST_RBRTH_REGISTER);
	outb(info->rx_high_water, info->ioaddr + MOXA_MUST_RBRTI_REGISTER);
	outb(info->rx_low_water, info->ioaddr + MOXA_MUST_RBRTL_REGISTER);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	outb(oldlcr, info->ioaddr + UART_LCR);
}

static void mxser_set_must_enum_value(unsigned long baseio, u8 value)
{
<<<<<<< HEAD
	u8 oldlcr;
	u8 efr;

	oldlcr = inb(baseio + UART_LCR);
	outb(MOXA_MUST_ENTER_ENCHANCE, baseio + UART_LCR);

	efr = inb(baseio + MOXA_MUST_EFR_REGISTER);
	efr &= ~MOXA_MUST_EFR_BANK_MASK;
	efr |= MOXA_MUST_EFR_BANK2;

	outb(efr, baseio + MOXA_MUST_EFR_REGISTER);
=======
	u8 oldlcr = mxser_must_select_bank(baseio, MOXA_MUST_EFR_BANK2);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	outb(value, baseio + MOXA_MUST_ENUM_REGISTER);
	outb(oldlcr, baseio + UART_LCR);
}

<<<<<<< HEAD
#ifdef CONFIG_PCI
static void mxser_get_must_hardware_id(unsigned long baseio, u8 *pId)
{
	u8 oldlcr;
	u8 efr;

	oldlcr = inb(baseio + UART_LCR);
	outb(MOXA_MUST_ENTER_ENCHANCE, baseio + UART_LCR);

	efr = inb(baseio + MOXA_MUST_EFR_REGISTER);
	efr &= ~MOXA_MUST_EFR_BANK_MASK;
	efr |= MOXA_MUST_EFR_BANK2;

	outb(efr, baseio + MOXA_MUST_EFR_REGISTER);
	*pId = inb(baseio + MOXA_MUST_HWID_REGISTER);
	outb(oldlcr, baseio + UART_LCR);
}
#endif

static void SET_MOXA_MUST_NO_SOFTWARE_FLOW_CONTROL(unsigned long baseio)
{
	u8 oldlcr;
	u8 efr;

	oldlcr = inb(baseio + UART_LCR);
	outb(MOXA_MUST_ENTER_ENCHANCE, baseio + UART_LCR);

	efr = inb(baseio + MOXA_MUST_EFR_REGISTER);
	efr &= ~MOXA_MUST_EFR_SF_MASK;

	outb(efr, baseio + MOXA_MUST_EFR_REGISTER);
	outb(oldlcr, baseio + UART_LCR);
}

static void mxser_enable_must_tx_software_flow_control(unsigned long baseio)
{
	u8 oldlcr;
	u8 efr;

	oldlcr = inb(baseio + UART_LCR);
	outb(MOXA_MUST_ENTER_ENCHANCE, baseio + UART_LCR);

	efr = inb(baseio + MOXA_MUST_EFR_REGISTER);
	efr &= ~MOXA_MUST_EFR_SF_TX_MASK;
	efr |= MOXA_MUST_EFR_SF_TX1;

	outb(efr, baseio + MOXA_MUST_EFR_REGISTER);
	outb(oldlcr, baseio + UART_LCR);
}

static void mxser_disable_must_tx_software_flow_control(unsigned long baseio)
{
	u8 oldlcr;
	u8 efr;

	oldlcr = inb(baseio + UART_LCR);
	outb(MOXA_MUST_ENTER_ENCHANCE, baseio + UART_LCR);

	efr = inb(baseio + MOXA_MUST_EFR_REGISTER);
	efr &= ~MOXA_MUST_EFR_SF_TX_MASK;

	outb(efr, baseio + MOXA_MUST_EFR_REGISTER);
	outb(oldlcr, baseio + UART_LCR);
}

static void mxser_enable_must_rx_software_flow_control(unsigned long baseio)
{
	u8 oldlcr;
	u8 efr;

	oldlcr = inb(baseio + UART_LCR);
	outb(MOXA_MUST_ENTER_ENCHANCE, baseio + UART_LCR);

	efr = inb(baseio + MOXA_MUST_EFR_REGISTER);
	efr &= ~MOXA_MUST_EFR_SF_RX_MASK;
	efr |= MOXA_MUST_EFR_SF_RX1;

	outb(efr, baseio + MOXA_MUST_EFR_REGISTER);
	outb(oldlcr, baseio + UART_LCR);
}

static void mxser_disable_must_rx_software_flow_control(unsigned long baseio)
{
	u8 oldlcr;
	u8 efr;

	oldlcr = inb(baseio + UART_LCR);
	outb(MOXA_MUST_ENTER_ENCHANCE, baseio + UART_LCR);

	efr = inb(baseio + MOXA_MUST_EFR_REGISTER);
	efr &= ~MOXA_MUST_EFR_SF_RX_MASK;

	outb(efr, baseio + MOXA_MUST_EFR_REGISTER);
	outb(oldlcr, baseio + UART_LCR);
}

#ifdef CONFIG_PCI
static int __devinit CheckIsMoxaMust(unsigned long io)
=======
static u8 mxser_get_must_hardware_id(unsigned long baseio)
{
	u8 oldlcr = mxser_must_select_bank(baseio, MOXA_MUST_EFR_BANK2);
	u8 id = inb(baseio + MOXA_MUST_HWID_REGISTER);
	outb(oldlcr, baseio + UART_LCR);

	return id;
}

static void mxser_must_set_EFR(unsigned long baseio, u8 clear, u8 set)
{
	__mxser_must_set_EFR(baseio, clear, set, true);
}

static void mxser_must_set_enhance_mode(unsigned long baseio, bool enable)
{
	mxser_must_set_EFR(baseio,
			enable ? 0 : MOXA_MUST_EFR_EFRB_ENABLE,
			enable ? MOXA_MUST_EFR_EFRB_ENABLE : 0);
}

static void mxser_must_no_sw_flow_control(unsigned long baseio)
{
	mxser_must_set_EFR(baseio, MOXA_MUST_EFR_SF_MASK, 0);
}

static void mxser_must_set_tx_sw_flow_control(unsigned long baseio, bool enable)
{
	mxser_must_set_EFR(baseio, MOXA_MUST_EFR_SF_TX_MASK,
			enable ? MOXA_MUST_EFR_SF_TX1 : 0);
}

static void mxser_must_set_rx_sw_flow_control(unsigned long baseio, bool enable)
{
	mxser_must_set_EFR(baseio, MOXA_MUST_EFR_SF_RX_MASK,
			enable ? MOXA_MUST_EFR_SF_RX1 : 0);
}

static enum mxser_must_hwid mxser_must_get_hwid(unsigned long io)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u8 oldmcr, hwid;
	int i;

	outb(0, io + UART_LCR);
<<<<<<< HEAD
	mxser_disable_must_enchance_mode(io);
	oldmcr = inb(io + UART_MCR);
	outb(0, io + UART_MCR);
	mxser_set_must_xon1_value(io, 0x11);
	if ((hwid = inb(io + UART_MCR)) != 0) {
=======
	mxser_must_set_enhance_mode(io, false);
	oldmcr = inb(io + UART_MCR);
	outb(0, io + UART_MCR);
	mxser_set_must_xon1_value(io, 0x11);
	if (inb(io + UART_MCR) != 0) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		outb(oldmcr, io + UART_MCR);
		return MOXA_OTHER_UART;
	}

<<<<<<< HEAD
	mxser_get_must_hardware_id(io, &hwid);
	for (i = 1; i < UART_INFO_NUM; i++) { /* 0 = OTHER_UART */
		if (hwid == Gpci_uart_info[i].type)
			return (int)hwid;
	}
	return MOXA_OTHER_UART;
}
#endif

static void process_txrx_fifo(struct mxser_port *info)
{
	int i;

	if ((info->type == PORT_16450) || (info->type == PORT_8250)) {
		info->rx_trigger = 1;
		info->rx_high_water = 1;
		info->rx_low_water = 1;
		info->xmit_fifo_size = 1;
	} else
		for (i = 0; i < UART_INFO_NUM; i++)
			if (info->board->chip_flag == Gpci_uart_info[i].type) {
				info->rx_trigger = Gpci_uart_info[i].rx_trigger;
				info->rx_low_water = Gpci_uart_info[i].rx_low_water;
				info->rx_high_water = Gpci_uart_info[i].rx_high_water;
				info->xmit_fifo_size = Gpci_uart_info[i].xmit_fifo_size;
				break;
			}
}

static unsigned char mxser_get_msr(int baseaddr, int mode, int port)
{
	static unsigned char mxser_msr[MXSER_PORTS + 1];
	unsigned char status = 0;

	status = inb(baseaddr + UART_MSR);

	mxser_msr[port] &= 0x0F;
	mxser_msr[port] |= status;
	status = mxser_msr[port];
	if (mode)
		mxser_msr[port] = 0;

	return status;
}

static int mxser_carrier_raised(struct tty_port *port)
{
	struct mxser_port *mp = container_of(port, struct mxser_port, port);
	return (inb(mp->ioaddr + UART_MSR) & UART_MSR_DCD)?1:0;
}

static void mxser_dtr_rts(struct tty_port *port, int on)
{
	struct mxser_port *mp = container_of(port, struct mxser_port, port);
	unsigned long flags;

	spin_lock_irqsave(&mp->slock, flags);
	if (on)
		outb(inb(mp->ioaddr + UART_MCR) |
			UART_MCR_DTR | UART_MCR_RTS, mp->ioaddr + UART_MCR);
	else
		outb(inb(mp->ioaddr + UART_MCR)&~(UART_MCR_DTR | UART_MCR_RTS),
			mp->ioaddr + UART_MCR);
	spin_unlock_irqrestore(&mp->slock, flags);
}

static int mxser_set_baud(struct tty_struct *tty, long newspd)
{
	struct mxser_port *info = tty->driver_data;
	int quot = 0, baud;
	unsigned char cval;

	if (!info->ioaddr)
		return -1;

	if (newspd > info->max_baud)
		return -1;

	if (newspd == 134) {
		quot = 2 * info->baud_base / 269;
		tty_encode_baud_rate(tty, 134, 134);
	} else if (newspd) {
		quot = info->baud_base / newspd;
		if (quot == 0)
			quot = 1;
		baud = info->baud_base/quot;
=======
	hwid = mxser_get_must_hardware_id(io);
	for (i = 1; i < UART_INFO_NUM; i++) /* 0 = OTHER_UART */
		if (hwid == Gpci_uart_info[i].type)
			return hwid;

	return MOXA_OTHER_UART;
}

static bool mxser_16550A_or_MUST(struct mxser_port *info)
{
	return info->type == PORT_16550A || info->board->must_hwid;
}

static void mxser_process_txrx_fifo(struct mxser_port *info)
{
	unsigned int i;

	if (info->type == PORT_16450 || info->type == PORT_8250) {
		info->rx_high_water = 1;
		info->rx_low_water = 1;
		info->xmit_fifo_size = 1;
		return;
	}

	for (i = 0; i < UART_INFO_NUM; i++)
		if (info->board->must_hwid == Gpci_uart_info[i].type) {
			info->rx_low_water = Gpci_uart_info[i].rx_low_water;
			info->rx_high_water = Gpci_uart_info[i].rx_high_water;
			info->xmit_fifo_size = Gpci_uart_info[i].fifo_size;
			break;
		}
}

static void __mxser_start_tx(struct mxser_port *info)
{
	outb(info->IER & ~UART_IER_THRI, info->ioaddr + UART_IER);
	info->IER |= UART_IER_THRI;
	outb(info->IER, info->ioaddr + UART_IER);
}

static void mxser_start_tx(struct mxser_port *info)
{
	unsigned long flags;

	spin_lock_irqsave(&info->slock, flags);
	__mxser_start_tx(info);
	spin_unlock_irqrestore(&info->slock, flags);
}

static void __mxser_stop_tx(struct mxser_port *info)
{
	info->IER &= ~UART_IER_THRI;
	outb(info->IER, info->ioaddr + UART_IER);
}

static bool mxser_carrier_raised(struct tty_port *port)
{
	struct mxser_port *mp = container_of(port, struct mxser_port, port);

	return inb(mp->ioaddr + UART_MSR) & UART_MSR_DCD;
}

static void mxser_dtr_rts(struct tty_port *port, bool active)
{
	struct mxser_port *mp = container_of(port, struct mxser_port, port);
	unsigned long flags;
	u8 mcr;

	spin_lock_irqsave(&mp->slock, flags);
	mcr = inb(mp->ioaddr + UART_MCR);
	if (active)
		mcr |= UART_MCR_DTR | UART_MCR_RTS;
	else
		mcr &= ~(UART_MCR_DTR | UART_MCR_RTS);
	outb(mcr, mp->ioaddr + UART_MCR);
	spin_unlock_irqrestore(&mp->slock, flags);
}

static int mxser_set_baud(struct tty_struct *tty, speed_t newspd)
{
	struct mxser_port *info = tty->driver_data;
	unsigned int quot = 0, baud;
	unsigned char cval;
	u64 timeout;

	if (newspd > info->board->max_baud)
		return -1;

	if (newspd == 134) {
		quot = 2 * MXSER_BAUD_BASE / 269;
		tty_encode_baud_rate(tty, 134, 134);
	} else if (newspd) {
		quot = MXSER_BAUD_BASE / newspd;
		if (quot == 0)
			quot = 1;
		baud = MXSER_BAUD_BASE / quot;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		tty_encode_baud_rate(tty, baud, baud);
	} else {
		quot = 0;
	}

<<<<<<< HEAD
	info->timeout = ((info->xmit_fifo_size * HZ * 10 * quot) / info->baud_base);
	info->timeout += HZ / 50;	/* Add .02 seconds of slop */
=======
	/*
	 * worst case (128 * 1000 * 10 * 18432) needs 35 bits, so divide in the
	 * u64 domain
	 */
	timeout = (u64)info->xmit_fifo_size * HZ * 10 * quot;
	do_div(timeout, MXSER_BAUD_BASE);
	info->timeout = timeout + HZ / 50; /* Add .02 seconds of slop */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (quot) {
		info->MCR |= UART_MCR_DTR;
		outb(info->MCR, info->ioaddr + UART_MCR);
	} else {
		info->MCR &= ~UART_MCR_DTR;
		outb(info->MCR, info->ioaddr + UART_MCR);
		return 0;
	}

	cval = inb(info->ioaddr + UART_LCR);

	outb(cval | UART_LCR_DLAB, info->ioaddr + UART_LCR);	/* set DLAB */

	outb(quot & 0xff, info->ioaddr + UART_DLL);	/* LS of divisor */
	outb(quot >> 8, info->ioaddr + UART_DLM);	/* MS of divisor */
	outb(cval, info->ioaddr + UART_LCR);	/* reset DLAB */

<<<<<<< HEAD
#ifdef BOTHER
	if (C_BAUD(tty) == BOTHER) {
		quot = info->baud_base % newspd;
=======
	if (C_BAUD(tty) == BOTHER) {
		quot = MXSER_BAUD_BASE % newspd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		quot *= 8;
		if (quot % newspd > newspd / 2) {
			quot /= newspd;
			quot++;
		} else
			quot /= newspd;

		mxser_set_must_enum_value(info->ioaddr, quot);
<<<<<<< HEAD
	} else
#endif
		mxser_set_must_enum_value(info->ioaddr, 0);
=======
	} else {
		mxser_set_must_enum_value(info->ioaddr, 0);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
=======
static void mxser_handle_cts(struct tty_struct *tty, struct mxser_port *info,
		u8 msr)
{
	bool cts = msr & UART_MSR_CTS;

	if (tty->hw_stopped) {
		if (cts) {
			tty->hw_stopped = false;

			if (!mxser_16550A_or_MUST(info))
				__mxser_start_tx(info);
			tty_wakeup(tty);
		}
		return;
	} else if (cts)
		return;

	tty->hw_stopped = true;
	if (!mxser_16550A_or_MUST(info))
		__mxser_stop_tx(info);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * This routine is called to set the UART divisor registers to match
 * the specified baud rate for a serial port.
 */
<<<<<<< HEAD
static int mxser_change_speed(struct tty_struct *tty,
					struct ktermios *old_termios)
{
	struct mxser_port *info = tty->driver_data;
	unsigned cflag, cval, fcr;
	int ret = 0;
	unsigned char status;

	cflag = tty->termios->c_cflag;
	if (!info->ioaddr)
		return ret;

	if (mxser_set_baud_method[tty->index] == 0)
		mxser_set_baud(tty, tty_get_baud_rate(tty));

	/* byte size and parity */
	switch (cflag & CSIZE) {
	case CS5:
		cval = 0x00;
		break;
	case CS6:
		cval = 0x01;
		break;
	case CS7:
		cval = 0x02;
		break;
	case CS8:
		cval = 0x03;
		break;
	default:
		cval = 0x00;
		break;		/* too keep GCC shut... */
	}
	if (cflag & CSTOPB)
		cval |= 0x04;
=======
static void mxser_change_speed(struct tty_struct *tty,
			       const struct ktermios *old_termios)
{
	struct mxser_port *info = tty->driver_data;
	unsigned cflag, cval;

	cflag = tty->termios.c_cflag;

	if (mxser_set_baud(tty, tty_get_baud_rate(tty))) {
		/* Use previous rate on a failure */
		if (old_termios) {
			speed_t baud = tty_termios_baud_rate(old_termios);
			tty_encode_baud_rate(tty, baud, baud);
		}
	}

	/* byte size and parity */
	cval = UART_LCR_WLEN(tty_get_char_size(tty->termios.c_cflag));

	if (cflag & CSTOPB)
		cval |= UART_LCR_STOP;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (cflag & PARENB)
		cval |= UART_LCR_PARITY;
	if (!(cflag & PARODD))
		cval |= UART_LCR_EPAR;
	if (cflag & CMSPAR)
		cval |= UART_LCR_SPAR;

<<<<<<< HEAD
	if ((info->type == PORT_8250) || (info->type == PORT_16450)) {
		if (info->board->chip_flag) {
			fcr = UART_FCR_ENABLE_FIFO;
			fcr |= MOXA_MUST_FCR_GDA_MODE_ENABLE;
			mxser_set_must_fifo_value(info);
		} else
			fcr = 0;
	} else {
		fcr = UART_FCR_ENABLE_FIFO;
		if (info->board->chip_flag) {
			fcr |= MOXA_MUST_FCR_GDA_MODE_ENABLE;
			mxser_set_must_fifo_value(info);
		} else {
			switch (info->rx_trigger) {
			case 1:
				fcr |= UART_FCR_TRIGGER_1;
				break;
			case 4:
				fcr |= UART_FCR_TRIGGER_4;
				break;
			case 8:
				fcr |= UART_FCR_TRIGGER_8;
				break;
			default:
				fcr |= UART_FCR_TRIGGER_14;
				break;
			}
=======
	info->FCR = 0;
	if (info->board->must_hwid) {
		info->FCR |= UART_FCR_ENABLE_FIFO |
			MOXA_MUST_FCR_GDA_MODE_ENABLE;
		mxser_set_must_fifo_value(info);
	} else if (info->type != PORT_8250 && info->type != PORT_16450) {
		info->FCR |= UART_FCR_ENABLE_FIFO;
		switch (info->rx_high_water) {
		case 1:
			info->FCR |= UART_FCR_TRIGGER_1;
			break;
		case 4:
			info->FCR |= UART_FCR_TRIGGER_4;
			break;
		case 8:
			info->FCR |= UART_FCR_TRIGGER_8;
			break;
		default:
			info->FCR |= UART_FCR_TRIGGER_14;
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	/* CTS flow control flag and modem status interrupts */
	info->IER &= ~UART_IER_MSI;
	info->MCR &= ~UART_MCR_AFE;
<<<<<<< HEAD
	if (cflag & CRTSCTS) {
		info->port.flags |= ASYNC_CTS_FLOW;
		info->IER |= UART_IER_MSI;
		if ((info->type == PORT_16550A) || (info->board->chip_flag)) {
			info->MCR |= UART_MCR_AFE;
		} else {
			status = inb(info->ioaddr + UART_MSR);
			if (tty->hw_stopped) {
				if (status & UART_MSR_CTS) {
					tty->hw_stopped = 0;
					if (info->type != PORT_16550A &&
							!info->board->chip_flag) {
						outb(info->IER & ~UART_IER_THRI,
							info->ioaddr +
							UART_IER);
						info->IER |= UART_IER_THRI;
						outb(info->IER, info->ioaddr +
								UART_IER);
					}
					tty_wakeup(tty);
				}
			} else {
				if (!(status & UART_MSR_CTS)) {
					tty->hw_stopped = 1;
					if ((info->type != PORT_16550A) &&
							(!info->board->chip_flag)) {
						info->IER &= ~UART_IER_THRI;
						outb(info->IER, info->ioaddr +
								UART_IER);
					}
				}
			}
		}
	} else {
		info->port.flags &= ~ASYNC_CTS_FLOW;
	}
	outb(info->MCR, info->ioaddr + UART_MCR);
	if (cflag & CLOCAL) {
		info->port.flags &= ~ASYNC_CHECK_CD;
	} else {
		info->port.flags |= ASYNC_CHECK_CD;
		info->IER |= UART_IER_MSI;
	}
=======
	tty_port_set_cts_flow(&info->port, cflag & CRTSCTS);
	if (cflag & CRTSCTS) {
		info->IER |= UART_IER_MSI;
		if (mxser_16550A_or_MUST(info)) {
			info->MCR |= UART_MCR_AFE;
		} else {
			mxser_handle_cts(tty, info,
					inb(info->ioaddr + UART_MSR));
		}
	}
	outb(info->MCR, info->ioaddr + UART_MCR);
	tty_port_set_check_carrier(&info->port, ~cflag & CLOCAL);
	if (~cflag & CLOCAL)
		info->IER |= UART_IER_MSI;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	outb(info->IER, info->ioaddr + UART_IER);

	/*
	 * Set up parity check flag
	 */
	info->read_status_mask = UART_LSR_OE | UART_LSR_THRE | UART_LSR_DR;
	if (I_INPCK(tty))
		info->read_status_mask |= UART_LSR_FE | UART_LSR_PE;
	if (I_BRKINT(tty) || I_PARMRK(tty))
		info->read_status_mask |= UART_LSR_BI;

	info->ignore_status_mask = 0;

	if (I_IGNBRK(tty)) {
		info->ignore_status_mask |= UART_LSR_BI;
		info->read_status_mask |= UART_LSR_BI;
		/*
		 * If we're ignore parity and break indicators, ignore
		 * overruns too.  (For real raw support).
		 */
		if (I_IGNPAR(tty)) {
			info->ignore_status_mask |=
						UART_LSR_OE |
						UART_LSR_PE |
						UART_LSR_FE;
			info->read_status_mask |=
						UART_LSR_OE |
						UART_LSR_PE |
						UART_LSR_FE;
		}
	}
<<<<<<< HEAD
	if (info->board->chip_flag) {
		mxser_set_must_xon1_value(info->ioaddr, START_CHAR(tty));
		mxser_set_must_xoff1_value(info->ioaddr, STOP_CHAR(tty));
		if (I_IXON(tty)) {
			mxser_enable_must_rx_software_flow_control(
					info->ioaddr);
		} else {
			mxser_disable_must_rx_software_flow_control(
					info->ioaddr);
		}
		if (I_IXOFF(tty)) {
			mxser_enable_must_tx_software_flow_control(
					info->ioaddr);
		} else {
			mxser_disable_must_tx_software_flow_control(
					info->ioaddr);
		}
	}


	outb(fcr, info->ioaddr + UART_FCR);	/* set fcr */
	outb(cval, info->ioaddr + UART_LCR);

	return ret;
}

static void mxser_check_modem_status(struct tty_struct *tty,
				struct mxser_port *port, int status)
{
	/* update input line counters */
	if (status & UART_MSR_TERI)
		port->icount.rng++;
	if (status & UART_MSR_DDSR)
		port->icount.dsr++;
	if (status & UART_MSR_DDCD)
		port->icount.dcd++;
	if (status & UART_MSR_DCTS)
		port->icount.cts++;
	port->mon_data.modem_status = status;
	wake_up_interruptible(&port->port.delta_msr_wait);

	if ((port->port.flags & ASYNC_CHECK_CD) && (status & UART_MSR_DDCD)) {
		if (status & UART_MSR_DCD)
			wake_up_interruptible(&port->port.open_wait);
	}

	if (port->port.flags & ASYNC_CTS_FLOW) {
		if (tty->hw_stopped) {
			if (status & UART_MSR_CTS) {
				tty->hw_stopped = 0;

				if ((port->type != PORT_16550A) &&
						(!port->board->chip_flag)) {
					outb(port->IER & ~UART_IER_THRI,
						port->ioaddr + UART_IER);
					port->IER |= UART_IER_THRI;
					outb(port->IER, port->ioaddr +
							UART_IER);
				}
				tty_wakeup(tty);
			}
		} else {
			if (!(status & UART_MSR_CTS)) {
				tty->hw_stopped = 1;
				if (port->type != PORT_16550A &&
						!port->board->chip_flag) {
					port->IER &= ~UART_IER_THRI;
					outb(port->IER, port->ioaddr +
							UART_IER);
				}
			}
		}
	}
=======
	if (info->board->must_hwid) {
		mxser_set_must_xon1_value(info->ioaddr, START_CHAR(tty));
		mxser_set_must_xoff1_value(info->ioaddr, STOP_CHAR(tty));
		mxser_must_set_rx_sw_flow_control(info->ioaddr, I_IXON(tty));
		mxser_must_set_tx_sw_flow_control(info->ioaddr, I_IXOFF(tty));
	}


	outb(info->FCR, info->ioaddr + UART_FCR);
	outb(cval, info->ioaddr + UART_LCR);
}

static u8 mxser_check_modem_status(struct tty_struct *tty,
				struct mxser_port *port)
{
	u8 msr = inb(port->ioaddr + UART_MSR);

	if (!(msr & UART_MSR_ANY_DELTA))
		return msr;

	/* update input line counters */
	if (msr & UART_MSR_TERI)
		port->icount.rng++;
	if (msr & UART_MSR_DDSR)
		port->icount.dsr++;
	if (msr & UART_MSR_DDCD)
		port->icount.dcd++;
	if (msr & UART_MSR_DCTS)
		port->icount.cts++;
	wake_up_interruptible(&port->port.delta_msr_wait);

	if (tty_port_check_carrier(&port->port) && (msr & UART_MSR_DDCD)) {
		if (msr & UART_MSR_DCD)
			wake_up_interruptible(&port->port.open_wait);
	}

	if (tty_port_cts_enabled(&port->port))
		mxser_handle_cts(tty, port, msr);

	return msr;
}

static void mxser_disable_and_clear_FIFO(struct mxser_port *info)
{
	u8 fcr = UART_FCR_CLEAR_RCVR | UART_FCR_CLEAR_XMIT;

	if (info->board->must_hwid)
		fcr |= MOXA_MUST_FCR_GDA_MODE_ENABLE;

	outb(fcr, info->ioaddr + UART_FCR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int mxser_activate(struct tty_port *port, struct tty_struct *tty)
{
	struct mxser_port *info = container_of(port, struct mxser_port, port);
<<<<<<< HEAD
	unsigned long page;
	unsigned long flags;

	page = __get_free_page(GFP_KERNEL);
	if (!page)
		return -ENOMEM;

	spin_lock_irqsave(&info->slock, flags);

	if (!info->ioaddr || !info->type) {
		set_bit(TTY_IO_ERROR, &tty->flags);
		free_page(page);
		spin_unlock_irqrestore(&info->slock, flags);
		return 0;
	}
	info->port.xmit_buf = (unsigned char *) page;
=======
	unsigned long flags;
	int ret;

	ret = tty_port_alloc_xmit_buf(port);
	if (ret < 0)
		return ret;

	spin_lock_irqsave(&info->slock, flags);

	if (!info->type) {
		set_bit(TTY_IO_ERROR, &tty->flags);
		spin_unlock_irqrestore(&info->slock, flags);
		ret = 0;
		goto err_free_xmit;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Clear the FIFO buffers and disable them
	 * (they will be reenabled in mxser_change_speed())
	 */
<<<<<<< HEAD
	if (info->board->chip_flag)
		outb((UART_FCR_CLEAR_RCVR |
			UART_FCR_CLEAR_XMIT |
			MOXA_MUST_FCR_GDA_MODE_ENABLE), info->ioaddr + UART_FCR);
	else
		outb((UART_FCR_CLEAR_RCVR | UART_FCR_CLEAR_XMIT),
			info->ioaddr + UART_FCR);
=======
	mxser_disable_and_clear_FIFO(info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * At this point there's no way the LSR could still be 0xFF;
	 * if it is, then bail out, because there's likely no UART
	 * here.
	 */
	if (inb(info->ioaddr + UART_LSR) == 0xff) {
		spin_unlock_irqrestore(&info->slock, flags);
		if (capable(CAP_SYS_ADMIN)) {
			set_bit(TTY_IO_ERROR, &tty->flags);
			return 0;
<<<<<<< HEAD
		} else
			return -ENODEV;
=======
		}

		ret = -ENODEV;
		goto err_free_xmit;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * Clear the interrupt registers.
	 */
	(void) inb(info->ioaddr + UART_LSR);
	(void) inb(info->ioaddr + UART_RX);
	(void) inb(info->ioaddr + UART_IIR);
	(void) inb(info->ioaddr + UART_MSR);

	/*
	 * Now, initialize the UART
	 */
	outb(UART_LCR_WLEN8, info->ioaddr + UART_LCR);	/* reset DLAB */
	info->MCR = UART_MCR_DTR | UART_MCR_RTS;
	outb(info->MCR, info->ioaddr + UART_MCR);

	/*
	 * Finally, enable interrupts
	 */
	info->IER = UART_IER_MSI | UART_IER_RLSI | UART_IER_RDI;

<<<<<<< HEAD
	if (info->board->chip_flag)
=======
	if (info->board->must_hwid)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		info->IER |= MOXA_MUST_IER_EGDAI;
	outb(info->IER, info->ioaddr + UART_IER);	/* enable interrupts */

	/*
	 * And clear the interrupt registers again for luck.
	 */
	(void) inb(info->ioaddr + UART_LSR);
	(void) inb(info->ioaddr + UART_RX);
	(void) inb(info->ioaddr + UART_IIR);
	(void) inb(info->ioaddr + UART_MSR);

	clear_bit(TTY_IO_ERROR, &tty->flags);
<<<<<<< HEAD
	info->xmit_cnt = info->xmit_head = info->xmit_tail = 0;
=======
	kfifo_reset(&port->xmit_fifo);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * and set the speed of the serial port
	 */
	mxser_change_speed(tty, NULL);
	spin_unlock_irqrestore(&info->slock, flags);

	return 0;
<<<<<<< HEAD
=======
err_free_xmit:
	tty_port_free_xmit_buf(port);
	return ret;
}

/*
 * To stop accepting input, we disable the receive line status interrupts, and
 * tell the interrupt driver to stop checking the data ready bit in the line
 * status register.
 */
static void mxser_stop_rx(struct mxser_port *info)
{
	info->IER &= ~UART_IER_RLSI;
	if (info->board->must_hwid)
		info->IER &= ~MOXA_MUST_RECV_ISR;

	outb(info->IER, info->ioaddr + UART_IER);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * This routine will shutdown a serial port
 */
static void mxser_shutdown_port(struct tty_port *port)
{
	struct mxser_port *info = container_of(port, struct mxser_port, port);
	unsigned long flags;

	spin_lock_irqsave(&info->slock, flags);

<<<<<<< HEAD
=======
	mxser_stop_rx(info);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * clear delta_msr_wait queue to avoid mem leaks: we may free the irq
	 * here so the queue might never be waken up
	 */
	wake_up_interruptible(&info->port.delta_msr_wait);

<<<<<<< HEAD
	/*
	 * Free the xmit buffer, if necessary
	 */
	if (info->port.xmit_buf) {
		free_page((unsigned long) info->port.xmit_buf);
		info->port.xmit_buf = NULL;
	}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	info->IER = 0;
	outb(0x00, info->ioaddr + UART_IER);

	/* clear Rx/Tx FIFO's */
<<<<<<< HEAD
	if (info->board->chip_flag)
		outb(UART_FCR_CLEAR_RCVR | UART_FCR_CLEAR_XMIT |
				MOXA_MUST_FCR_GDA_MODE_ENABLE,
				info->ioaddr + UART_FCR);
	else
		outb(UART_FCR_CLEAR_RCVR | UART_FCR_CLEAR_XMIT,
			info->ioaddr + UART_FCR);
=======
	mxser_disable_and_clear_FIFO(info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* read data port to reset things */
	(void) inb(info->ioaddr + UART_RX);


<<<<<<< HEAD
	if (info->board->chip_flag)
		SET_MOXA_MUST_NO_SOFTWARE_FLOW_CONTROL(info->ioaddr);

	spin_unlock_irqrestore(&info->slock, flags);
=======
	if (info->board->must_hwid)
		mxser_must_no_sw_flow_control(info->ioaddr);

	spin_unlock_irqrestore(&info->slock, flags);

	/* make sure ISR is not running while we free the buffer */
	synchronize_irq(info->board->irq);

	tty_port_free_xmit_buf(port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * This routine is called whenever a serial port is opened.  It
 * enables interrupts for a serial port, linking in its async structure into
 * the IRQ chain.   It also performs the serial-specific
 * initialization for the tty structure.
 */
static int mxser_open(struct tty_struct *tty, struct file *filp)
{
<<<<<<< HEAD
	struct mxser_port *info;
	int line;

	line = tty->index;
	if (line == MXSER_PORTS)
		return 0;
	info = &mxser_boards[line / MXSER_PORTS_PER_BOARD].ports[line % MXSER_PORTS_PER_BOARD];
	if (!info->ioaddr)
		return -ENODEV;

	tty->driver_data = info;
	return tty_port_open(&info->port, tty, filp);
=======
	struct tty_port *tport = tty->port;
	struct mxser_port *port = container_of(tport, struct mxser_port, port);

	tty->driver_data = port;

	return tty_port_open(tport, tty, filp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void mxser_flush_buffer(struct tty_struct *tty)
{
	struct mxser_port *info = tty->driver_data;
<<<<<<< HEAD
	char fcr;
	unsigned long flags;


	spin_lock_irqsave(&info->slock, flags);
	info->xmit_cnt = info->xmit_head = info->xmit_tail = 0;

	fcr = inb(info->ioaddr + UART_FCR);
	outb((fcr | UART_FCR_CLEAR_RCVR | UART_FCR_CLEAR_XMIT),
		info->ioaddr + UART_FCR);
	outb(fcr, info->ioaddr + UART_FCR);
=======
	unsigned long flags;

	spin_lock_irqsave(&info->slock, flags);
	kfifo_reset(&info->port.xmit_fifo);

	outb(info->FCR | UART_FCR_CLEAR_RCVR | UART_FCR_CLEAR_XMIT,
		info->ioaddr + UART_FCR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_unlock_irqrestore(&info->slock, flags);

	tty_wakeup(tty);
}

<<<<<<< HEAD

static void mxser_close_port(struct tty_port *port)
{
	struct mxser_port *info = container_of(port, struct mxser_port, port);
	unsigned long timeout;
	/*
	 * At this point we stop accepting input.  To do this, we
	 * disable the receive line status interrupts, and tell the
	 * interrupt driver to stop checking the data ready bit in the
	 * line status register.
	 */
	info->IER &= ~UART_IER_RLSI;
	if (info->board->chip_flag)
		info->IER &= ~MOXA_MUST_RECV_ISR;

	outb(info->IER, info->ioaddr + UART_IER);
	/*
	 * Before we drop DTR, make sure the UART transmitter
	 * has completely drained; this is especially
	 * important if there is a transmit FIFO!
	 */
	timeout = jiffies + HZ;
	while (!(inb(info->ioaddr + UART_LSR) & UART_LSR_TEMT)) {
		schedule_timeout_interruptible(5);
		if (time_after(jiffies, timeout))
			break;
	}
}

/*
 * This routine is called when the serial port gets closed.  First, we
 * wait for the last remaining data to be sent.  Then, we unlink its
 * async structure from the interrupt chain if necessary, and we free
 * that IRQ if nothing is left in the chain.
 */
static void mxser_close(struct tty_struct *tty, struct file *filp)
{
	struct mxser_port *info = tty->driver_data;
	struct tty_port *port = &info->port;

	if (tty->index == MXSER_PORTS || info == NULL)
		return;
	if (tty_port_close_start(port, tty, filp) == 0)
		return;
	mutex_lock(&port->mutex);
	mxser_close_port(port);
	mxser_flush_buffer(tty);
	mxser_shutdown_port(port);
	clear_bit(ASYNCB_INITIALIZED, &port->flags);
	mutex_unlock(&port->mutex);
	/* Right now the tty_port set is done outside of the close_end helper
	   as we don't yet have everyone using refcounts */	
	tty_port_close_end(port, tty);
	tty_port_tty_set(port, NULL);
}

static int mxser_write(struct tty_struct *tty, const unsigned char *buf, int count)
{
	int c, total = 0;
	struct mxser_port *info = tty->driver_data;
	unsigned long flags;

	if (!info->port.xmit_buf)
		return 0;

	while (1) {
		c = min_t(int, count, min(SERIAL_XMIT_SIZE - info->xmit_cnt - 1,
					  SERIAL_XMIT_SIZE - info->xmit_head));
		if (c <= 0)
			break;

		memcpy(info->port.xmit_buf + info->xmit_head, buf, c);
		spin_lock_irqsave(&info->slock, flags);
		info->xmit_head = (info->xmit_head + c) &
				  (SERIAL_XMIT_SIZE - 1);
		info->xmit_cnt += c;
		spin_unlock_irqrestore(&info->slock, flags);

		buf += c;
		count -= c;
		total += c;
	}

	if (info->xmit_cnt && !tty->stopped) {
		if (!tty->hw_stopped ||
				(info->type == PORT_16550A) ||
				(info->board->chip_flag)) {
			spin_lock_irqsave(&info->slock, flags);
			outb(info->IER & ~UART_IER_THRI, info->ioaddr +
					UART_IER);
			info->IER |= UART_IER_THRI;
			outb(info->IER, info->ioaddr + UART_IER);
			spin_unlock_irqrestore(&info->slock, flags);
		}
	}
	return total;
}

static int mxser_put_char(struct tty_struct *tty, unsigned char ch)
{
	struct mxser_port *info = tty->driver_data;
	unsigned long flags;

	if (!info->port.xmit_buf)
		return 0;

	if (info->xmit_cnt >= SERIAL_XMIT_SIZE - 1)
		return 0;

	spin_lock_irqsave(&info->slock, flags);
	info->port.xmit_buf[info->xmit_head++] = ch;
	info->xmit_head &= SERIAL_XMIT_SIZE - 1;
	info->xmit_cnt++;
	spin_unlock_irqrestore(&info->slock, flags);
	if (!tty->stopped) {
		if (!tty->hw_stopped ||
				(info->type == PORT_16550A) ||
				info->board->chip_flag) {
			spin_lock_irqsave(&info->slock, flags);
			outb(info->IER & ~UART_IER_THRI, info->ioaddr + UART_IER);
			info->IER |= UART_IER_THRI;
			outb(info->IER, info->ioaddr + UART_IER);
			spin_unlock_irqrestore(&info->slock, flags);
		}
	}
	return 1;
=======
static void mxser_close(struct tty_struct *tty, struct file *filp)
{
	tty_port_close(tty->port, tty, filp);
}

static ssize_t mxser_write(struct tty_struct *tty, const u8 *buf, size_t count)
{
	struct mxser_port *info = tty->driver_data;
	unsigned long flags;
	size_t written;
	bool is_empty;

	spin_lock_irqsave(&info->slock, flags);
	written = kfifo_in(&info->port.xmit_fifo, buf, count);
	is_empty = kfifo_is_empty(&info->port.xmit_fifo);
	spin_unlock_irqrestore(&info->slock, flags);

	if (!is_empty && !tty->flow.stopped)
		if (!tty->hw_stopped || mxser_16550A_or_MUST(info))
			mxser_start_tx(info);

	return written;
}

static int mxser_put_char(struct tty_struct *tty, u8 ch)
{
	struct mxser_port *info = tty->driver_data;
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&info->slock, flags);
	ret = kfifo_put(&info->port.xmit_fifo, ch);
	spin_unlock_irqrestore(&info->slock, flags);

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}


static void mxser_flush_chars(struct tty_struct *tty)
{
	struct mxser_port *info = tty->driver_data;
<<<<<<< HEAD
	unsigned long flags;

	if (info->xmit_cnt <= 0 || tty->stopped || !info->port.xmit_buf ||
			(tty->hw_stopped && info->type != PORT_16550A &&
			 !info->board->chip_flag))
		return;

	spin_lock_irqsave(&info->slock, flags);

	outb(info->IER & ~UART_IER_THRI, info->ioaddr + UART_IER);
	info->IER |= UART_IER_THRI;
	outb(info->IER, info->ioaddr + UART_IER);

	spin_unlock_irqrestore(&info->slock, flags);
}

static int mxser_write_room(struct tty_struct *tty)
{
	struct mxser_port *info = tty->driver_data;
	int ret;

	ret = SERIAL_XMIT_SIZE - info->xmit_cnt - 1;
	return ret < 0 ? 0 : ret;
}

static int mxser_chars_in_buffer(struct tty_struct *tty)
{
	struct mxser_port *info = tty->driver_data;
	return info->xmit_cnt;
=======

	if (kfifo_is_empty(&info->port.xmit_fifo) || tty->flow.stopped ||
			(tty->hw_stopped && !mxser_16550A_or_MUST(info)))
		return;

	mxser_start_tx(info);
}

static unsigned int mxser_write_room(struct tty_struct *tty)
{
	struct mxser_port *info = tty->driver_data;

	return kfifo_avail(&info->port.xmit_fifo);
}

static unsigned int mxser_chars_in_buffer(struct tty_struct *tty)
{
	struct mxser_port *info = tty->driver_data;

	return kfifo_len(&info->port.xmit_fifo);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * ------------------------------------------------------------
 * friends of mxser_ioctl()
 * ------------------------------------------------------------
 */
static int mxser_get_serial_info(struct tty_struct *tty,
<<<<<<< HEAD
		struct serial_struct __user *retinfo)
{
	struct mxser_port *info = tty->driver_data;
	struct serial_struct tmp = {
		.type = info->type,
		.line = tty->index,
		.port = info->ioaddr,
		.irq = info->board->irq,
		.flags = info->port.flags,
		.baud_base = info->baud_base,
		.close_delay = info->port.close_delay,
		.closing_wait = info->port.closing_wait,
		.custom_divisor = info->custom_divisor,
		.hub6 = 0
	};
	if (copy_to_user(retinfo, &tmp, sizeof(*retinfo)))
		return -EFAULT;
=======
		struct serial_struct *ss)
{
	struct mxser_port *info = tty->driver_data;
	struct tty_port *port = &info->port;
	unsigned int closing_wait, close_delay;

	mutex_lock(&port->mutex);

	close_delay = jiffies_to_msecs(info->port.close_delay) / 10;
	closing_wait = info->port.closing_wait;
	if (closing_wait != ASYNC_CLOSING_WAIT_NONE)
		closing_wait = jiffies_to_msecs(closing_wait) / 10;

	ss->type = info->type;
	ss->line = tty->index;
	ss->port = info->ioaddr;
	ss->irq = info->board->irq;
	ss->flags = info->port.flags;
	ss->baud_base = MXSER_BAUD_BASE;
	ss->close_delay = close_delay;
	ss->closing_wait = closing_wait;
	ss->custom_divisor = MXSER_CUSTOM_DIVISOR,
	mutex_unlock(&port->mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int mxser_set_serial_info(struct tty_struct *tty,
<<<<<<< HEAD
		struct serial_struct __user *new_info)
{
	struct mxser_port *info = tty->driver_data;
	struct tty_port *port = &info->port;
	struct serial_struct new_serial;
	speed_t baud;
	unsigned long sl_flags;
	unsigned int flags;
	int retval = 0;

	if (!new_info || !info->ioaddr)
		return -ENODEV;
	if (copy_from_user(&new_serial, new_info, sizeof(new_serial)))
		return -EFAULT;

	if (new_serial.irq != info->board->irq ||
			new_serial.port != info->ioaddr)
		return -EINVAL;

	flags = port->flags & ASYNC_SPD_MASK;

	if (!capable(CAP_SYS_ADMIN)) {
		if ((new_serial.baud_base != info->baud_base) ||
				(new_serial.close_delay != info->port.close_delay) ||
				((new_serial.flags & ~ASYNC_USR_MASK) != (info->port.flags & ~ASYNC_USR_MASK)))
			return -EPERM;
		info->port.flags = ((info->port.flags & ~ASYNC_USR_MASK) |
				(new_serial.flags & ASYNC_USR_MASK));
=======
		struct serial_struct *ss)
{
	struct mxser_port *info = tty->driver_data;
	struct tty_port *port = &info->port;
	speed_t baud;
	unsigned long sl_flags;
	unsigned int old_speed, close_delay, closing_wait;
	int retval = 0;

	if (tty_io_error(tty))
		return -EIO;

	mutex_lock(&port->mutex);

	if (ss->irq != info->board->irq ||
			ss->port != info->ioaddr) {
		mutex_unlock(&port->mutex);
		return -EINVAL;
	}

	old_speed = port->flags & ASYNC_SPD_MASK;

	close_delay = msecs_to_jiffies(ss->close_delay * 10);
	closing_wait = ss->closing_wait;
	if (closing_wait != ASYNC_CLOSING_WAIT_NONE)
		closing_wait = msecs_to_jiffies(closing_wait * 10);

	if (!capable(CAP_SYS_ADMIN)) {
		if ((ss->baud_base != MXSER_BAUD_BASE) ||
				(close_delay != port->close_delay) ||
				(closing_wait != port->closing_wait) ||
				((ss->flags & ~ASYNC_USR_MASK) != (port->flags & ~ASYNC_USR_MASK))) {
			mutex_unlock(&port->mutex);
			return -EPERM;
		}
		port->flags = (port->flags & ~ASYNC_USR_MASK) |
				(ss->flags & ASYNC_USR_MASK);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		/*
		 * OK, past this point, all the error checking has been done.
		 * At this point, we start making changes.....
		 */
		port->flags = ((port->flags & ~ASYNC_FLAGS) |
<<<<<<< HEAD
				(new_serial.flags & ASYNC_FLAGS));
		port->close_delay = new_serial.close_delay * HZ / 100;
		port->closing_wait = new_serial.closing_wait * HZ / 100;
		tty->low_latency = (port->flags & ASYNC_LOW_LATENCY) ? 1 : 0;
		if ((port->flags & ASYNC_SPD_MASK) == ASYNC_SPD_CUST &&
				(new_serial.baud_base != info->baud_base ||
				new_serial.custom_divisor !=
				info->custom_divisor)) {
			if (new_serial.custom_divisor == 0)
				return -EINVAL;
			baud = new_serial.baud_base / new_serial.custom_divisor;
			tty_encode_baud_rate(tty, baud, baud);
		}
	}

	info->type = new_serial.type;

	process_txrx_fifo(info);

	if (test_bit(ASYNCB_INITIALIZED, &port->flags)) {
		if (flags != (port->flags & ASYNC_SPD_MASK)) {
=======
				(ss->flags & ASYNC_FLAGS));
		port->close_delay = close_delay;
		port->closing_wait = closing_wait;
		if ((port->flags & ASYNC_SPD_MASK) == ASYNC_SPD_CUST &&
				(ss->baud_base != MXSER_BAUD_BASE ||
				ss->custom_divisor !=
				MXSER_CUSTOM_DIVISOR)) {
			if (ss->custom_divisor == 0) {
				mutex_unlock(&port->mutex);
				return -EINVAL;
			}
			baud = ss->baud_base / ss->custom_divisor;
			tty_encode_baud_rate(tty, baud, baud);
		}

		info->type = ss->type;

		mxser_process_txrx_fifo(info);
	}

	if (tty_port_initialized(port)) {
		if (old_speed != (port->flags & ASYNC_SPD_MASK)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			spin_lock_irqsave(&info->slock, sl_flags);
			mxser_change_speed(tty, NULL);
			spin_unlock_irqrestore(&info->slock, sl_flags);
		}
	} else {
		retval = mxser_activate(port, tty);
		if (retval == 0)
<<<<<<< HEAD
			set_bit(ASYNCB_INITIALIZED, &port->flags);
	}
=======
			tty_port_set_initialized(port, true);
	}
	mutex_unlock(&port->mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return retval;
}

/*
 * mxser_get_lsr_info - get line status register info
 *
 * Purpose: Let user call ioctl() to get info when the UART physically
 *	    is emptied.  On bus types like RS485, the transmitter must
 *	    release the bus after transmitting. This must be done when
 *	    the transmit shift register is empty, not be done when the
 *	    transmit holding register is empty.  This functionality
 *	    allows an RS485 driver to be written in user space.
 */
static int mxser_get_lsr_info(struct mxser_port *info,
		unsigned int __user *value)
{
	unsigned char status;
	unsigned int result;
	unsigned long flags;

	spin_lock_irqsave(&info->slock, flags);
	status = inb(info->ioaddr + UART_LSR);
	spin_unlock_irqrestore(&info->slock, flags);
	result = ((status & UART_LSR_TEMT) ? TIOCSER_TEMT : 0);
	return put_user(result, value);
}

static int mxser_tiocmget(struct tty_struct *tty)
{
	struct mxser_port *info = tty->driver_data;
<<<<<<< HEAD
	unsigned char control, status;
	unsigned long flags;


	if (tty->index == MXSER_PORTS)
		return -ENOIOCTLCMD;
	if (test_bit(TTY_IO_ERROR, &tty->flags))
		return -EIO;

	control = info->MCR;

	spin_lock_irqsave(&info->slock, flags);
	status = inb(info->ioaddr + UART_MSR);
	if (status & UART_MSR_ANY_DELTA)
		mxser_check_modem_status(tty, info, status);
	spin_unlock_irqrestore(&info->slock, flags);
	return ((control & UART_MCR_RTS) ? TIOCM_RTS : 0) |
		    ((control & UART_MCR_DTR) ? TIOCM_DTR : 0) |
		    ((status & UART_MSR_DCD) ? TIOCM_CAR : 0) |
		    ((status & UART_MSR_RI) ? TIOCM_RNG : 0) |
		    ((status & UART_MSR_DSR) ? TIOCM_DSR : 0) |
		    ((status & UART_MSR_CTS) ? TIOCM_CTS : 0);
=======
	unsigned char control;
	unsigned long flags;
	u8 msr;

	if (tty_io_error(tty))
		return -EIO;

	spin_lock_irqsave(&info->slock, flags);
	control = info->MCR;
	msr = mxser_check_modem_status(tty, info);
	spin_unlock_irqrestore(&info->slock, flags);

	return ((control & UART_MCR_RTS) ? TIOCM_RTS : 0) |
		    ((control & UART_MCR_DTR) ? TIOCM_DTR : 0) |
		    ((msr & UART_MSR_DCD) ? TIOCM_CAR : 0) |
		    ((msr & UART_MSR_RI) ? TIOCM_RNG : 0) |
		    ((msr & UART_MSR_DSR) ? TIOCM_DSR : 0) |
		    ((msr & UART_MSR_CTS) ? TIOCM_CTS : 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int mxser_tiocmset(struct tty_struct *tty,
		unsigned int set, unsigned int clear)
{
	struct mxser_port *info = tty->driver_data;
	unsigned long flags;

<<<<<<< HEAD

	if (tty->index == MXSER_PORTS)
		return -ENOIOCTLCMD;
	if (test_bit(TTY_IO_ERROR, &tty->flags))
=======
	if (tty_io_error(tty))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EIO;

	spin_lock_irqsave(&info->slock, flags);

	if (set & TIOCM_RTS)
		info->MCR |= UART_MCR_RTS;
	if (set & TIOCM_DTR)
		info->MCR |= UART_MCR_DTR;

	if (clear & TIOCM_RTS)
		info->MCR &= ~UART_MCR_RTS;
	if (clear & TIOCM_DTR)
		info->MCR &= ~UART_MCR_DTR;

	outb(info->MCR, info->ioaddr + UART_MCR);
	spin_unlock_irqrestore(&info->slock, flags);
	return 0;
}

<<<<<<< HEAD
static int __init mxser_program_mode(int port)
{
	int id, i, j, n;

	outb(0, port);
	outb(0, port);
	outb(0, port);
	(void)inb(port);
	(void)inb(port);
	outb(0, port);
	(void)inb(port);

	id = inb(port + 1) & 0x1F;
	if ((id != C168_ASIC_ID) &&
			(id != C104_ASIC_ID) &&
			(id != C102_ASIC_ID) &&
			(id != CI132_ASIC_ID) &&
			(id != CI134_ASIC_ID) &&
			(id != CI104J_ASIC_ID))
		return -1;
	for (i = 0, j = 0; i < 4; i++) {
		n = inb(port + 2);
		if (n == 'M') {
			j = 1;
		} else if ((j == 1) && (n == 1)) {
			j = 2;
			break;
		} else
			j = 0;
	}
	if (j != 2)
		id = -2;
	return id;
}

static void __init mxser_normal_mode(int port)
{
	int i, n;

	outb(0xA5, port + 1);
	outb(0x80, port + 3);
	outb(12, port + 0);	/* 9600 bps */
	outb(0, port + 1);
	outb(0x03, port + 3);	/* 8 data bits */
	outb(0x13, port + 4);	/* loop back mode */
	for (i = 0; i < 16; i++) {
		n = inb(port + 5);
		if ((n & 0x61) == 0x60)
			break;
		if ((n & 1) == 1)
			(void)inb(port);
	}
	outb(0x00, port + 4);
}

#define CHIP_SK 	0x01	/* Serial Data Clock  in Eprom */
#define CHIP_DO 	0x02	/* Serial Data Output in Eprom */
#define CHIP_CS 	0x04	/* Serial Chip Select in Eprom */
#define CHIP_DI 	0x08	/* Serial Data Input  in Eprom */
#define EN_CCMD 	0x000	/* Chip's command register     */
#define EN0_RSARLO	0x008	/* Remote start address reg 0  */
#define EN0_RSARHI	0x009	/* Remote start address reg 1  */
#define EN0_RCNTLO	0x00A	/* Remote byte count reg WR    */
#define EN0_RCNTHI	0x00B	/* Remote byte count reg WR    */
#define EN0_DCFG	0x00E	/* Data configuration reg WR   */
#define EN0_PORT	0x010	/* Rcv missed frame error counter RD */
#define ENC_PAGE0	0x000	/* Select page 0 of chip registers   */
#define ENC_PAGE3	0x0C0	/* Select page 3 of chip registers   */
static int __init mxser_read_register(int port, unsigned short *regs)
{
	int i, k, value, id;
	unsigned int j;

	id = mxser_program_mode(port);
	if (id < 0)
		return id;
	for (i = 0; i < 14; i++) {
		k = (i & 0x3F) | 0x180;
		for (j = 0x100; j > 0; j >>= 1) {
			outb(CHIP_CS, port);
			if (k & j) {
				outb(CHIP_CS | CHIP_DO, port);
				outb(CHIP_CS | CHIP_DO | CHIP_SK, port);	/* A? bit of read */
			} else {
				outb(CHIP_CS, port);
				outb(CHIP_CS | CHIP_SK, port);	/* A? bit of read */
			}
		}
		(void)inb(port);
		value = 0;
		for (k = 0, j = 0x8000; k < 16; k++, j >>= 1) {
			outb(CHIP_CS, port);
			outb(CHIP_CS | CHIP_SK, port);
			if (inb(port) & CHIP_DI)
				value |= j;
		}
		regs[i] = value;
		outb(0, port);
	}
	mxser_normal_mode(port);
	return id;
}

static int mxser_ioctl_special(unsigned int cmd, void __user *argp)
{
	struct mxser_port *ip;
	struct tty_port *port;
	struct tty_struct *tty;
	int result, status;
	unsigned int i, j;
	int ret = 0;

	switch (cmd) {
	case MOXA_GET_MAJOR:
		printk_ratelimited(KERN_WARNING "mxser: '%s' uses deprecated ioctl "
					"%x (GET_MAJOR), fix your userspace\n",
					current->comm, cmd);
		return put_user(ttymajor, (int __user *)argp);

	case MOXA_CHKPORTENABLE:
		result = 0;
		for (i = 0; i < MXSER_BOARDS; i++)
			for (j = 0; j < MXSER_PORTS_PER_BOARD; j++)
				if (mxser_boards[i].ports[j].ioaddr)
					result |= (1 << i);
		return put_user(result, (unsigned long __user *)argp);
	case MOXA_GETDATACOUNT:
		/* The receive side is locked by port->slock but it isn't
		   clear that an exact snapshot is worth copying here */
		if (copy_to_user(argp, &mxvar_log, sizeof(mxvar_log)))
			ret = -EFAULT;
		return ret;
	case MOXA_GETMSTATUS: {
		struct mxser_mstatus ms, __user *msu = argp;
		for (i = 0; i < MXSER_BOARDS; i++)
			for (j = 0; j < MXSER_PORTS_PER_BOARD; j++) {
				ip = &mxser_boards[i].ports[j];
				port = &ip->port;
				memset(&ms, 0, sizeof(ms));

				mutex_lock(&port->mutex);
				if (!ip->ioaddr)
					goto copy;
				
				tty = tty_port_tty_get(port);

				if (!tty || !tty->termios)
					ms.cflag = ip->normal_termios.c_cflag;
				else
					ms.cflag = tty->termios->c_cflag;
				tty_kref_put(tty);
				spin_lock_irq(&ip->slock);
				status = inb(ip->ioaddr + UART_MSR);
				spin_unlock_irq(&ip->slock);
				if (status & UART_MSR_DCD)
					ms.dcd = 1;
				if (status & UART_MSR_DSR)
					ms.dsr = 1;
				if (status & UART_MSR_CTS)
					ms.cts = 1;
			copy:
				mutex_unlock(&port->mutex);
				if (copy_to_user(msu, &ms, sizeof(ms)))
					return -EFAULT;
				msu++;
			}
		return 0;
	}
	case MOXA_ASPP_MON_EXT: {
		struct mxser_mon_ext *me; /* it's 2k, stack unfriendly */
		unsigned int cflag, iflag, p;
		u8 opmode;

		me = kzalloc(sizeof(*me), GFP_KERNEL);
		if (!me)
			return -ENOMEM;

		for (i = 0, p = 0; i < MXSER_BOARDS; i++) {
			for (j = 0; j < MXSER_PORTS_PER_BOARD; j++, p++) {
				if (p >= ARRAY_SIZE(me->rx_cnt)) {
					i = MXSER_BOARDS;
					break;
				}
				ip = &mxser_boards[i].ports[j];
				port = &ip->port;

				mutex_lock(&port->mutex);
				if (!ip->ioaddr) {
					mutex_unlock(&port->mutex);
					continue;
				}

				spin_lock_irq(&ip->slock);
				status = mxser_get_msr(ip->ioaddr, 0, p);

				if (status & UART_MSR_TERI)
					ip->icount.rng++;
				if (status & UART_MSR_DDSR)
					ip->icount.dsr++;
				if (status & UART_MSR_DDCD)
					ip->icount.dcd++;
				if (status & UART_MSR_DCTS)
					ip->icount.cts++;

				ip->mon_data.modem_status = status;
				me->rx_cnt[p] = ip->mon_data.rxcnt;
				me->tx_cnt[p] = ip->mon_data.txcnt;
				me->up_rxcnt[p] = ip->mon_data.up_rxcnt;
				me->up_txcnt[p] = ip->mon_data.up_txcnt;
				me->modem_status[p] =
					ip->mon_data.modem_status;
				spin_unlock_irq(&ip->slock);

				tty = tty_port_tty_get(&ip->port);

				if (!tty || !tty->termios) {
					cflag = ip->normal_termios.c_cflag;
					iflag = ip->normal_termios.c_iflag;
					me->baudrate[p] = tty_termios_baud_rate(&ip->normal_termios);
				} else {
					cflag = tty->termios->c_cflag;
					iflag = tty->termios->c_iflag;
					me->baudrate[p] = tty_get_baud_rate(tty);
				}
				tty_kref_put(tty);

				me->databits[p] = cflag & CSIZE;
				me->stopbits[p] = cflag & CSTOPB;
				me->parity[p] = cflag & (PARENB | PARODD |
						CMSPAR);

				if (cflag & CRTSCTS)
					me->flowctrl[p] |= 0x03;

				if (iflag & (IXON | IXOFF))
					me->flowctrl[p] |= 0x0C;

				if (ip->type == PORT_16550A)
					me->fifo[p] = 1;

				opmode = inb(ip->opmode_ioaddr)>>((p % 4) * 2);
				opmode &= OP_MODE_MASK;
				me->iftype[p] = opmode;
				mutex_unlock(&port->mutex);
			}
		}
		if (copy_to_user(argp, me, sizeof(*me)))
			ret = -EFAULT;
		kfree(me);
		return ret;
	}
	default:
		return -ENOIOCTLCMD;
	}
	return 0;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int mxser_cflags_changed(struct mxser_port *info, unsigned long arg,
		struct async_icount *cprev)
{
	struct async_icount cnow;
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&info->slock, flags);
	cnow = info->icount;	/* atomic copy */
	spin_unlock_irqrestore(&info->slock, flags);

	ret =	((arg & TIOCM_RNG) && (cnow.rng != cprev->rng)) ||
		((arg & TIOCM_DSR) && (cnow.dsr != cprev->dsr)) ||
		((arg & TIOCM_CD)  && (cnow.dcd != cprev->dcd)) ||
		((arg & TIOCM_CTS) && (cnow.cts != cprev->cts));

	*cprev = cnow;

	return ret;
}

<<<<<<< HEAD
=======
/* We should likely switch to TIOCGRS485/TIOCSRS485. */
static int mxser_ioctl_op_mode(struct mxser_port *port, int index, bool set,
		int __user *u_opmode)
{
	int opmode, p = index % 4;
	int shiftbit = p * 2;
	u8 val;

	if (port->board->must_hwid != MOXA_MUST_MU860_HWID)
		return -EFAULT;

	if (set) {
		if (get_user(opmode, u_opmode))
			return -EFAULT;

		if (opmode & ~OP_MODE_MASK)
			return -EINVAL;

		spin_lock_irq(&port->slock);
		val = inb(port->opmode_ioaddr);
		val &= ~(OP_MODE_MASK << shiftbit);
		val |= (opmode << shiftbit);
		outb(val, port->opmode_ioaddr);
		spin_unlock_irq(&port->slock);

		return 0;
	}

	spin_lock_irq(&port->slock);
	opmode = inb(port->opmode_ioaddr) >> shiftbit;
	spin_unlock_irq(&port->slock);

	return put_user(opmode & OP_MODE_MASK, u_opmode);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int mxser_ioctl(struct tty_struct *tty,
		unsigned int cmd, unsigned long arg)
{
	struct mxser_port *info = tty->driver_data;
<<<<<<< HEAD
	struct tty_port *port = &info->port;
	struct async_icount cnow;
	unsigned long flags;
	void __user *argp = (void __user *)arg;
	int retval;

	if (tty->index == MXSER_PORTS)
		return mxser_ioctl_special(cmd, argp);

	if (cmd == MOXA_SET_OP_MODE || cmd == MOXA_GET_OP_MODE) {
		int p;
		unsigned long opmode;
		static unsigned char ModeMask[] = { 0xfc, 0xf3, 0xcf, 0x3f };
		int shiftbit;
		unsigned char val, mask;

		p = tty->index % 4;
		if (cmd == MOXA_SET_OP_MODE) {
			if (get_user(opmode, (int __user *) argp))
				return -EFAULT;
			if (opmode != RS232_MODE &&
					opmode != RS485_2WIRE_MODE &&
					opmode != RS422_MODE &&
					opmode != RS485_4WIRE_MODE)
				return -EFAULT;
			mask = ModeMask[p];
			shiftbit = p * 2;
			spin_lock_irq(&info->slock);
			val = inb(info->opmode_ioaddr);
			val &= mask;
			val |= (opmode << shiftbit);
			outb(val, info->opmode_ioaddr);
			spin_unlock_irq(&info->slock);
		} else {
			shiftbit = p * 2;
			spin_lock_irq(&info->slock);
			opmode = inb(info->opmode_ioaddr) >> shiftbit;
			spin_unlock_irq(&info->slock);
			opmode &= OP_MODE_MASK;
			if (put_user(opmode, (int __user *)argp))
				return -EFAULT;
		}
		return 0;
	}

	if (cmd != TIOCGSERIAL && cmd != TIOCMIWAIT &&
			test_bit(TTY_IO_ERROR, &tty->flags))
		return -EIO;

	switch (cmd) {
	case TIOCGSERIAL:
		mutex_lock(&port->mutex);
		retval = mxser_get_serial_info(tty, argp);
		mutex_unlock(&port->mutex);
		return retval;
	case TIOCSSERIAL:
		mutex_lock(&port->mutex);
		retval = mxser_set_serial_info(tty, argp);
		mutex_unlock(&port->mutex);
		return retval;
=======
	struct async_icount cnow;
	unsigned long flags;
	void __user *argp = (void __user *)arg;

	if (cmd == MOXA_SET_OP_MODE || cmd == MOXA_GET_OP_MODE)
		return mxser_ioctl_op_mode(info, tty->index,
				cmd == MOXA_SET_OP_MODE, argp);

	if (cmd != TIOCMIWAIT && tty_io_error(tty))
		return -EIO;

	switch (cmd) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case TIOCSERGETLSR:	/* Get line status register */
		return  mxser_get_lsr_info(info, argp);
		/*
		 * Wait for any of the 4 modem inputs (DCD,RI,DSR,CTS) to change
		 * - mask passed in arg for lines of interest
		 *   (use |'ed TIOCM_RNG/DSR/CD/CTS for masking)
		 * Caller should use TIOCGICOUNT to see which one it was
		 */
	case TIOCMIWAIT:
		spin_lock_irqsave(&info->slock, flags);
		cnow = info->icount;	/* note the counters on entry */
		spin_unlock_irqrestore(&info->slock, flags);

		return wait_event_interruptible(info->port.delta_msr_wait,
				mxser_cflags_changed(info, arg, &cnow));
<<<<<<< HEAD
	case MOXA_HighSpeedOn:
		return put_user(info->baud_base != 115200 ? 1 : 0, (int __user *)argp);
	case MOXA_SDS_RSTICOUNTER:
		spin_lock_irq(&info->slock);
		info->mon_data.rxcnt = 0;
		info->mon_data.txcnt = 0;
		spin_unlock_irq(&info->slock);
		return 0;

	case MOXA_ASPP_OQUEUE:{
		int len, lsr;

		len = mxser_chars_in_buffer(tty);
		spin_lock_irq(&info->slock);
		lsr = inb(info->ioaddr + UART_LSR) & UART_LSR_THRE;
		spin_unlock_irq(&info->slock);
		len += (lsr ? 0 : 1);

		return put_user(len, (int __user *)argp);
	}
	case MOXA_ASPP_MON: {
		int mcr, status;

		spin_lock_irq(&info->slock);
		status = mxser_get_msr(info->ioaddr, 1, tty->index);
		mxser_check_modem_status(tty, info, status);

		mcr = inb(info->ioaddr + UART_MCR);
		spin_unlock_irq(&info->slock);

		if (mcr & MOXA_MUST_MCR_XON_FLAG)
			info->mon_data.hold_reason &= ~NPPI_NOTIFY_XOFFHOLD;
		else
			info->mon_data.hold_reason |= NPPI_NOTIFY_XOFFHOLD;

		if (mcr & MOXA_MUST_MCR_TX_XON)
			info->mon_data.hold_reason &= ~NPPI_NOTIFY_XOFFXENT;
		else
			info->mon_data.hold_reason |= NPPI_NOTIFY_XOFFXENT;

		if (tty->hw_stopped)
			info->mon_data.hold_reason |= NPPI_NOTIFY_CTSHOLD;
		else
			info->mon_data.hold_reason &= ~NPPI_NOTIFY_CTSHOLD;

		if (copy_to_user(argp, &info->mon_data,
				sizeof(struct mxser_mon)))
			return -EFAULT;

		return 0;
	}
	case MOXA_ASPP_LSTATUS: {
		if (put_user(info->err_shadow, (unsigned char __user *)argp))
			return -EFAULT;

		info->err_shadow = 0;
		return 0;
	}
	case MOXA_SET_BAUD_METHOD: {
		int method;

		if (get_user(method, (int __user *)argp))
			return -EFAULT;
		mxser_set_baud_method[tty->index] = method;
		return put_user(method, (int __user *)argp);
	}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		return -ENOIOCTLCMD;
	}
	return 0;
}

	/*
	 * Get counter of input serial line interrupts (DCD,RI,DSR,CTS)
	 * Return: write counters to the user passed counter struct
	 * NB: both 1->0 and 0->1 transitions are counted except for
	 *     RI where only 0->1 is counted.
	 */

static int mxser_get_icount(struct tty_struct *tty,
		struct serial_icounter_struct *icount)

{
	struct mxser_port *info = tty->driver_data;
	struct async_icount cnow;
	unsigned long flags;

	spin_lock_irqsave(&info->slock, flags);
	cnow = info->icount;
	spin_unlock_irqrestore(&info->slock, flags);

	icount->frame = cnow.frame;
	icount->brk = cnow.brk;
	icount->overrun = cnow.overrun;
	icount->buf_overrun = cnow.buf_overrun;
	icount->parity = cnow.parity;
	icount->rx = cnow.rx;
	icount->tx = cnow.tx;
	icount->cts = cnow.cts;
	icount->dsr = cnow.dsr;
	icount->rng = cnow.rng;
	icount->dcd = cnow.dcd;
	return 0;
}

<<<<<<< HEAD
static void mxser_stoprx(struct tty_struct *tty)
{
	struct mxser_port *info = tty->driver_data;

	info->ldisc_stop_rx = 1;
	if (I_IXOFF(tty)) {
		if (info->board->chip_flag) {
=======
/*
 * This routine is called by the upper-layer tty layer to signal that
 * incoming characters should be throttled.
 */
static void mxser_throttle(struct tty_struct *tty)
{
	struct mxser_port *info = tty->driver_data;

	if (I_IXOFF(tty)) {
		if (info->board->must_hwid) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			info->IER &= ~MOXA_MUST_RECV_ISR;
			outb(info->IER, info->ioaddr + UART_IER);
		} else {
			info->x_char = STOP_CHAR(tty);
			outb(0, info->ioaddr + UART_IER);
			info->IER |= UART_IER_THRI;
			outb(info->IER, info->ioaddr + UART_IER);
		}
	}

<<<<<<< HEAD
	if (tty->termios->c_cflag & CRTSCTS) {
=======
	if (C_CRTSCTS(tty)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		info->MCR &= ~UART_MCR_RTS;
		outb(info->MCR, info->ioaddr + UART_MCR);
	}
}

<<<<<<< HEAD
/*
 * This routine is called by the upper-layer tty layer to signal that
 * incoming characters should be throttled.
 */
static void mxser_throttle(struct tty_struct *tty)
{
	mxser_stoprx(tty);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void mxser_unthrottle(struct tty_struct *tty)
{
	struct mxser_port *info = tty->driver_data;

	/* startrx */
<<<<<<< HEAD
	info->ldisc_stop_rx = 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (I_IXOFF(tty)) {
		if (info->x_char)
			info->x_char = 0;
		else {
<<<<<<< HEAD
			if (info->board->chip_flag) {
=======
			if (info->board->must_hwid) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				info->IER |= MOXA_MUST_RECV_ISR;
				outb(info->IER, info->ioaddr + UART_IER);
			} else {
				info->x_char = START_CHAR(tty);
				outb(0, info->ioaddr + UART_IER);
				info->IER |= UART_IER_THRI;
				outb(info->IER, info->ioaddr + UART_IER);
			}
		}
	}

<<<<<<< HEAD
	if (tty->termios->c_cflag & CRTSCTS) {
=======
	if (C_CRTSCTS(tty)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		info->MCR |= UART_MCR_RTS;
		outb(info->MCR, info->ioaddr + UART_MCR);
	}
}

/*
 * mxser_stop() and mxser_start()
 *
<<<<<<< HEAD
 * This routines are called before setting or resetting tty->stopped.
=======
 * This routines are called before setting or resetting tty->flow.stopped.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * They enable or disable transmitter interrupts, as necessary.
 */
static void mxser_stop(struct tty_struct *tty)
{
	struct mxser_port *info = tty->driver_data;
	unsigned long flags;

	spin_lock_irqsave(&info->slock, flags);
<<<<<<< HEAD
	if (info->IER & UART_IER_THRI) {
		info->IER &= ~UART_IER_THRI;
		outb(info->IER, info->ioaddr + UART_IER);
	}
=======
	if (info->IER & UART_IER_THRI)
		__mxser_stop_tx(info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock_irqrestore(&info->slock, flags);
}

static void mxser_start(struct tty_struct *tty)
{
	struct mxser_port *info = tty->driver_data;
	unsigned long flags;

	spin_lock_irqsave(&info->slock, flags);
<<<<<<< HEAD
	if (info->xmit_cnt && info->port.xmit_buf) {
		outb(info->IER & ~UART_IER_THRI, info->ioaddr + UART_IER);
		info->IER |= UART_IER_THRI;
		outb(info->IER, info->ioaddr + UART_IER);
	}
	spin_unlock_irqrestore(&info->slock, flags);
}

static void mxser_set_termios(struct tty_struct *tty, struct ktermios *old_termios)
=======
	if (!kfifo_is_empty(&info->port.xmit_fifo))
		__mxser_start_tx(info);
	spin_unlock_irqrestore(&info->slock, flags);
}

static void mxser_set_termios(struct tty_struct *tty,
			      const struct ktermios *old_termios)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct mxser_port *info = tty->driver_data;
	unsigned long flags;

	spin_lock_irqsave(&info->slock, flags);
	mxser_change_speed(tty, old_termios);
	spin_unlock_irqrestore(&info->slock, flags);

<<<<<<< HEAD
	if ((old_termios->c_cflag & CRTSCTS) &&
			!(tty->termios->c_cflag & CRTSCTS)) {
		tty->hw_stopped = 0;
=======
	if ((old_termios->c_cflag & CRTSCTS) && !C_CRTSCTS(tty)) {
		tty->hw_stopped = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mxser_start(tty);
	}

	/* Handle sw stopped */
<<<<<<< HEAD
	if ((old_termios->c_iflag & IXON) &&
			!(tty->termios->c_iflag & IXON)) {
		tty->stopped = 0;

		if (info->board->chip_flag) {
			spin_lock_irqsave(&info->slock, flags);
			mxser_disable_must_rx_software_flow_control(
					info->ioaddr);
=======
	if ((old_termios->c_iflag & IXON) && !I_IXON(tty)) {
		tty->flow.stopped = 0;

		if (info->board->must_hwid) {
			spin_lock_irqsave(&info->slock, flags);
			mxser_must_set_rx_sw_flow_control(info->ioaddr, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			spin_unlock_irqrestore(&info->slock, flags);
		}

		mxser_start(tty);
	}
}

<<<<<<< HEAD
=======
static bool mxser_tx_empty(struct mxser_port *info)
{
	unsigned long flags;
	u8 lsr;

	spin_lock_irqsave(&info->slock, flags);
	lsr = inb(info->ioaddr + UART_LSR);
	spin_unlock_irqrestore(&info->slock, flags);

	return !(lsr & UART_LSR_TEMT);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * mxser_wait_until_sent() --- wait until the transmitter is empty
 */
static void mxser_wait_until_sent(struct tty_struct *tty, int timeout)
{
	struct mxser_port *info = tty->driver_data;
<<<<<<< HEAD
	unsigned long orig_jiffies, char_time;
	unsigned long flags;
	int lsr;
=======
	unsigned long expire, char_time;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (info->type == PORT_UNKNOWN)
		return;

	if (info->xmit_fifo_size == 0)
		return;		/* Just in case.... */

<<<<<<< HEAD
	orig_jiffies = jiffies;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Set the check interval to be 1/5 of the estimated time to
	 * send a single character, and make it at least 1.  The check
	 * interval should also be less than the timeout.
	 *
	 * Note: we have to use pretty tight timings here to satisfy
	 * the NIST-PCTS.
	 */
	char_time = (info->timeout - HZ / 50) / info->xmit_fifo_size;
	char_time = char_time / 5;
	if (char_time == 0)
		char_time = 1;
	if (timeout && timeout < char_time)
		char_time = timeout;
<<<<<<< HEAD
=======

	char_time = jiffies_to_msecs(char_time);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * If the transmitter hasn't cleared in twice the approximate
	 * amount of time to send the entire FIFO, it probably won't
	 * ever clear.  This assumes the UART isn't doing flow
	 * control, which is currently the case.  Hence, if it ever
	 * takes longer than info->timeout, this is probably due to a
	 * UART bug of some kind.  So, we clamp the timeout parameter at
	 * 2*info->timeout.
	 */
	if (!timeout || timeout > 2 * info->timeout)
		timeout = 2 * info->timeout;

<<<<<<< HEAD
	spin_lock_irqsave(&info->slock, flags);
	while (!((lsr = inb(info->ioaddr + UART_LSR)) & UART_LSR_TEMT)) {
		spin_unlock_irqrestore(&info->slock, flags);
		schedule_timeout_interruptible(char_time);
		spin_lock_irqsave(&info->slock, flags);
		if (signal_pending(current))
			break;
		if (timeout && time_after(jiffies, orig_jiffies + timeout))
			break;
	}
	spin_unlock_irqrestore(&info->slock, flags);
	set_current_state(TASK_RUNNING);
=======
	expire = jiffies + timeout;

	while (mxser_tx_empty(info)) {
		msleep_interruptible(char_time);
		if (signal_pending(current))
			break;
		if (time_after(jiffies, expire))
			break;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * This routine is called by tty_hangup() when a hangup is signaled.
 */
static void mxser_hangup(struct tty_struct *tty)
{
	struct mxser_port *info = tty->driver_data;

	mxser_flush_buffer(tty);
	tty_port_hangup(&info->port);
}

/*
 * mxser_rs_break() --- routine which turns the break handling on or off
 */
static int mxser_rs_break(struct tty_struct *tty, int break_state)
{
	struct mxser_port *info = tty->driver_data;
	unsigned long flags;
<<<<<<< HEAD

	spin_lock_irqsave(&info->slock, flags);
	if (break_state == -1)
		outb(inb(info->ioaddr + UART_LCR) | UART_LCR_SBC,
			info->ioaddr + UART_LCR);
	else
		outb(inb(info->ioaddr + UART_LCR) & ~UART_LCR_SBC,
			info->ioaddr + UART_LCR);
	spin_unlock_irqrestore(&info->slock, flags);
	return 0;
}

static void mxser_receive_chars(struct tty_struct *tty,
				struct mxser_port *port, int *status)
{
	unsigned char ch, gdl;
	int ignored = 0;
	int cnt = 0;
	int recv_room;
	int max = 256;

	recv_room = tty->receive_room;
	if (recv_room == 0 && !port->ldisc_stop_rx)
		mxser_stoprx(tty);
	if (port->board->chip_flag != MOXA_OTHER_UART) {

		if (*status & UART_LSR_SPECIAL)
			goto intr_old;
		if (port->board->chip_flag == MOXA_MUST_MU860_HWID &&
				(*status & MOXA_MUST_LSR_RERR))
			goto intr_old;
		if (*status & MOXA_MUST_LSR_RERR)
			goto intr_old;

		gdl = inb(port->ioaddr + MOXA_MUST_GDL_REGISTER);

		if (port->board->chip_flag == MOXA_MUST_MU150_HWID)
			gdl &= MOXA_MUST_GDL_MASK;
		if (gdl >= recv_room) {
			if (!port->ldisc_stop_rx)
				mxser_stoprx(tty);
		}
		while (gdl--) {
			ch = inb(port->ioaddr + UART_RX);
			tty_insert_flip_char(tty, ch, 0);
			cnt++;
		}
		goto end_intr;
	}
intr_old:
=======
	u8 lcr;

	spin_lock_irqsave(&info->slock, flags);
	lcr = inb(info->ioaddr + UART_LCR);
	if (break_state == -1)
		lcr |= UART_LCR_SBC;
	else
		lcr &= ~UART_LCR_SBC;
	outb(lcr, info->ioaddr + UART_LCR);
	spin_unlock_irqrestore(&info->slock, flags);

	return 0;
}

static bool mxser_receive_chars_new(struct mxser_port *port, u8 status)
{
	enum mxser_must_hwid hwid = port->board->must_hwid;
	u8 gdl;

	if (hwid == MOXA_OTHER_UART)
		return false;
	if (status & (UART_LSR_BRK_ERROR_BITS | MOXA_MUST_LSR_RERR))
		return false;

	gdl = inb(port->ioaddr + MOXA_MUST_GDL_REGISTER);
	if (hwid == MOXA_MUST_MU150_HWID)
		gdl &= MOXA_MUST_GDL_MASK;

	while (gdl--) {
		u8 ch = inb(port->ioaddr + UART_RX);
		if (!tty_insert_flip_char(&port->port, ch, 0))
			port->icount.buf_overrun++;
	}

	return true;
}

static u8 mxser_receive_chars_old(struct tty_struct *tty,
		                struct mxser_port *port, u8 status)
{
	enum mxser_must_hwid hwid = port->board->must_hwid;
	int ignored = 0;
	int max = 256;
	u8 ch;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	do {
		if (max-- < 0)
			break;

		ch = inb(port->ioaddr + UART_RX);
<<<<<<< HEAD
		if (port->board->chip_flag && (*status & UART_LSR_OE))
			outb(0x23, port->ioaddr + UART_FCR);
		*status &= port->read_status_mask;
		if (*status & port->ignore_status_mask) {
			if (++ignored > 100)
				break;
		} else {
			char flag = 0;
			if (*status & UART_LSR_SPECIAL) {
				if (*status & UART_LSR_BI) {
=======
		if (hwid && (status & UART_LSR_OE))
			outb(port->FCR | UART_FCR_CLEAR_RCVR,
					port->ioaddr + UART_FCR);
		status &= port->read_status_mask;
		if (status & port->ignore_status_mask) {
			if (++ignored > 100)
				break;
		} else {
			u8 flag = 0;
			if (status & UART_LSR_BRK_ERROR_BITS) {
				if (status & UART_LSR_BI) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					flag = TTY_BREAK;
					port->icount.brk++;

					if (port->port.flags & ASYNC_SAK)
						do_SAK(tty);
<<<<<<< HEAD
				} else if (*status & UART_LSR_PE) {
					flag = TTY_PARITY;
					port->icount.parity++;
				} else if (*status & UART_LSR_FE) {
					flag = TTY_FRAME;
					port->icount.frame++;
				} else if (*status & UART_LSR_OE) {
					flag = TTY_OVERRUN;
					port->icount.overrun++;
				} else
					flag = TTY_BREAK;
			}
			tty_insert_flip_char(tty, ch, flag);
			cnt++;
			if (cnt >= recv_room) {
				if (!port->ldisc_stop_rx)
					mxser_stoprx(tty);
				break;
			}

		}

		if (port->board->chip_flag)
			break;

		*status = inb(port->ioaddr + UART_LSR);
	} while (*status & UART_LSR_DR);

end_intr:
	mxvar_log.rxcnt[tty->index] += cnt;
	port->mon_data.rxcnt += cnt;
	port->mon_data.up_rxcnt += cnt;

	/*
	 * We are called from an interrupt context with &port->slock
	 * being held. Drop it temporarily in order to prevent
	 * recursive locking.
	 */
	spin_unlock(&port->slock);
	tty_flip_buffer_push(tty);
	spin_lock(&port->slock);
=======
				} else if (status & UART_LSR_PE) {
					flag = TTY_PARITY;
					port->icount.parity++;
				} else if (status & UART_LSR_FE) {
					flag = TTY_FRAME;
					port->icount.frame++;
				} else if (status & UART_LSR_OE) {
					flag = TTY_OVERRUN;
					port->icount.overrun++;
				}
			}
			if (!tty_insert_flip_char(&port->port, ch, flag)) {
				port->icount.buf_overrun++;
				break;
			}
		}

		if (hwid)
			break;

		status = inb(port->ioaddr + UART_LSR);
	} while (status & UART_LSR_DR);

	return status;
}

static u8 mxser_receive_chars(struct tty_struct *tty,
		struct mxser_port *port, u8 status)
{
	if (!mxser_receive_chars_new(port, status))
		status = mxser_receive_chars_old(tty, port, status);

	tty_flip_buffer_push(&port->port);

	return status;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void mxser_transmit_chars(struct tty_struct *tty, struct mxser_port *port)
{
<<<<<<< HEAD
	int count, cnt;
=======
	int count;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (port->x_char) {
		outb(port->x_char, port->ioaddr + UART_TX);
		port->x_char = 0;
<<<<<<< HEAD
		mxvar_log.txcnt[tty->index]++;
		port->mon_data.txcnt++;
		port->mon_data.up_txcnt++;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		port->icount.tx++;
		return;
	}

<<<<<<< HEAD
	if (port->port.xmit_buf == NULL)
		return;

	if (port->xmit_cnt <= 0 || tty->stopped ||
			(tty->hw_stopped &&
			(port->type != PORT_16550A) &&
			(!port->board->chip_flag))) {
		port->IER &= ~UART_IER_THRI;
		outb(port->IER, port->ioaddr + UART_IER);
		return;
	}

	cnt = port->xmit_cnt;
	count = port->xmit_fifo_size;
	do {
		outb(port->port.xmit_buf[port->xmit_tail++],
			port->ioaddr + UART_TX);
		port->xmit_tail = port->xmit_tail & (SERIAL_XMIT_SIZE - 1);
		if (--port->xmit_cnt <= 0)
			break;
	} while (--count > 0);
	mxvar_log.txcnt[tty->index] += (cnt - port->xmit_cnt);

	port->mon_data.txcnt += (cnt - port->xmit_cnt);
	port->mon_data.up_txcnt += (cnt - port->xmit_cnt);
	port->icount.tx += (cnt - port->xmit_cnt);

	if (port->xmit_cnt < WAKEUP_CHARS)
		tty_wakeup(tty);

	if (port->xmit_cnt <= 0) {
		port->IER &= ~UART_IER_THRI;
		outb(port->IER, port->ioaddr + UART_IER);
	}
=======
	if (kfifo_is_empty(&port->port.xmit_fifo) || tty->flow.stopped ||
			(tty->hw_stopped && !mxser_16550A_or_MUST(port))) {
		__mxser_stop_tx(port);
		return;
	}

	count = port->xmit_fifo_size;
	do {
		u8 c;

		if (!kfifo_get(&port->port.xmit_fifo, &c))
			break;

		outb(c, port->ioaddr + UART_TX);
		port->icount.tx++;
	} while (--count > 0);

	if (kfifo_len(&port->port.xmit_fifo) < WAKEUP_CHARS)
		tty_wakeup(tty);

	if (kfifo_is_empty(&port->port.xmit_fifo))
		__mxser_stop_tx(port);
}

static bool mxser_port_isr(struct mxser_port *port)
{
	struct tty_struct *tty;
	u8 iir, status;
	bool error = false;

	iir = inb(port->ioaddr + UART_IIR);
	if (iir & UART_IIR_NO_INT)
		return true;

	iir &= MOXA_MUST_IIR_MASK;
	tty = tty_port_tty_get(&port->port);
	if (!tty) {
		status = inb(port->ioaddr + UART_LSR);
		outb(port->FCR | UART_FCR_CLEAR_RCVR | UART_FCR_CLEAR_XMIT,
				port->ioaddr + UART_FCR);
		inb(port->ioaddr + UART_MSR);

		error = true;
		goto put_tty;
	}

	status = inb(port->ioaddr + UART_LSR);

	if (port->board->must_hwid) {
		if (iir == MOXA_MUST_IIR_GDA ||
		    iir == MOXA_MUST_IIR_RDA ||
		    iir == MOXA_MUST_IIR_RTO ||
		    iir == MOXA_MUST_IIR_LSR)
			status = mxser_receive_chars(tty, port, status);
	} else {
		status &= port->read_status_mask;
		if (status & UART_LSR_DR)
			status = mxser_receive_chars(tty, port, status);
	}

	mxser_check_modem_status(tty, port);

	if (port->board->must_hwid) {
		if (iir == 0x02 && (status & UART_LSR_THRE))
			mxser_transmit_chars(tty, port);
	} else {
		if (status & UART_LSR_THRE)
			mxser_transmit_chars(tty, port);
	}

put_tty:
	tty_kref_put(tty);

	return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * This is the serial driver's generic interrupt routine
 */
static irqreturn_t mxser_interrupt(int irq, void *dev_id)
{
<<<<<<< HEAD
	int status, iir, i;
	struct mxser_board *brd = NULL;
	struct mxser_port *port;
	int max, irqbits, bits, msr;
	unsigned int int_cnt, pass_counter = 0;
	int handled = IRQ_NONE;
	struct tty_struct *tty;

	for (i = 0; i < MXSER_BOARDS; i++)
		if (dev_id == &mxser_boards[i]) {
			brd = dev_id;
			break;
		}

	if (i == MXSER_BOARDS)
		goto irq_stop;
	if (brd == NULL)
		goto irq_stop;
	max = brd->info->nports;
	while (pass_counter++ < MXSER_ISR_PASS_LIMIT) {
		irqbits = inb(brd->vector) & brd->vector_mask;
		if (irqbits == brd->vector_mask)
=======
	struct mxser_board *brd = dev_id;
	struct mxser_port *port;
	unsigned int int_cnt, pass_counter = 0;
	unsigned int i, max = brd->nports;
	int handled = IRQ_NONE;
	u8 irqbits, bits, mask = BIT(max) - 1;

	while (pass_counter++ < MXSER_ISR_PASS_LIMIT) {
		irqbits = inb(brd->vector) & mask;
		if (irqbits == mask)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;

		handled = IRQ_HANDLED;
		for (i = 0, bits = 1; i < max; i++, irqbits |= bits, bits <<= 1) {
<<<<<<< HEAD
			if (irqbits == brd->vector_mask)
=======
			if (irqbits == mask)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				break;
			if (bits & irqbits)
				continue;
			port = &brd->ports[i];

			int_cnt = 0;
			spin_lock(&port->slock);
			do {
<<<<<<< HEAD
				iir = inb(port->ioaddr + UART_IIR);
				if (iir & UART_IIR_NO_INT)
					break;
				iir &= MOXA_MUST_IIR_MASK;
				tty = tty_port_tty_get(&port->port);
				if (!tty ||
						(port->port.flags & ASYNC_CLOSING) ||
						!(port->port.flags &
							ASYNC_INITIALIZED)) {
					status = inb(port->ioaddr + UART_LSR);
					outb(0x27, port->ioaddr + UART_FCR);
					inb(port->ioaddr + UART_MSR);
					tty_kref_put(tty);
					break;
				}

				status = inb(port->ioaddr + UART_LSR);

				if (status & UART_LSR_PE)
					port->err_shadow |= NPPI_NOTIFY_PARITY;
				if (status & UART_LSR_FE)
					port->err_shadow |= NPPI_NOTIFY_FRAMING;
				if (status & UART_LSR_OE)
					port->err_shadow |=
						NPPI_NOTIFY_HW_OVERRUN;
				if (status & UART_LSR_BI)
					port->err_shadow |= NPPI_NOTIFY_BREAK;

				if (port->board->chip_flag) {
					if (iir == MOXA_MUST_IIR_GDA ||
					    iir == MOXA_MUST_IIR_RDA ||
					    iir == MOXA_MUST_IIR_RTO ||
					    iir == MOXA_MUST_IIR_LSR)
						mxser_receive_chars(tty, port,
								&status);

				} else {
					status &= port->read_status_mask;
					if (status & UART_LSR_DR)
						mxser_receive_chars(tty, port,
								&status);
				}
				msr = inb(port->ioaddr + UART_MSR);
				if (msr & UART_MSR_ANY_DELTA)
					mxser_check_modem_status(tty, port, msr);

				if (port->board->chip_flag) {
					if (iir == 0x02 && (status &
								UART_LSR_THRE))
						mxser_transmit_chars(tty, port);
				} else {
					if (status & UART_LSR_THRE)
						mxser_transmit_chars(tty, port);
				}
				tty_kref_put(tty);
=======
				if (mxser_port_isr(port))
					break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			} while (int_cnt++ < MXSER_ISR_PASS_LIMIT);
			spin_unlock(&port->slock);
		}
	}

<<<<<<< HEAD
irq_stop:
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return handled;
}

static const struct tty_operations mxser_ops = {
	.open = mxser_open,
	.close = mxser_close,
	.write = mxser_write,
	.put_char = mxser_put_char,
	.flush_chars = mxser_flush_chars,
	.write_room = mxser_write_room,
	.chars_in_buffer = mxser_chars_in_buffer,
	.flush_buffer = mxser_flush_buffer,
	.ioctl = mxser_ioctl,
	.throttle = mxser_throttle,
	.unthrottle = mxser_unthrottle,
	.set_termios = mxser_set_termios,
	.stop = mxser_stop,
	.start = mxser_start,
	.hangup = mxser_hangup,
	.break_ctl = mxser_rs_break,
	.wait_until_sent = mxser_wait_until_sent,
	.tiocmget = mxser_tiocmget,
	.tiocmset = mxser_tiocmset,
<<<<<<< HEAD
	.get_icount = mxser_get_icount,
};

struct tty_port_operations mxser_port_ops = {
=======
	.set_serial = mxser_set_serial_info,
	.get_serial = mxser_get_serial_info,
	.get_icount = mxser_get_icount,
};

static const struct tty_port_operations mxser_port_ops = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.carrier_raised = mxser_carrier_raised,
	.dtr_rts = mxser_dtr_rts,
	.activate = mxser_activate,
	.shutdown = mxser_shutdown_port,
};

/*
 * The MOXA Smartio/Industio serial driver boot-time initialization code!
 */

<<<<<<< HEAD
static void mxser_release_ISA_res(struct mxser_board *brd)
{
	free_irq(brd->irq, brd);
	release_region(brd->ports[0].ioaddr, 8 * brd->info->nports);
	release_region(brd->vector, 1);
}

static int __devinit mxser_initbrd(struct mxser_board *brd,
		struct pci_dev *pdev)
{
	struct mxser_port *info;
	unsigned int i;
	int retval;

	printk(KERN_INFO "mxser: max. baud rate = %d bps\n",
			brd->ports[0].max_baud);

	for (i = 0; i < brd->info->nports; i++) {
		info = &brd->ports[i];
		tty_port_init(&info->port);
		info->port.ops = &mxser_port_ops;
		info->board = brd;
		info->stop_rx = 0;
		info->ldisc_stop_rx = 0;

		/* Enhance mode enabled here */
		if (brd->chip_flag != MOXA_OTHER_UART)
			mxser_enable_must_enchance_mode(info->ioaddr);

		info->port.flags = ASYNC_SHARE_IRQ;
		info->type = brd->uart_type;

		process_txrx_fifo(info);

		info->custom_divisor = info->baud_base * 16;
		info->port.close_delay = 5 * HZ / 10;
		info->port.closing_wait = 30 * HZ;
		info->normal_termios = mxvar_sdriver->init_termios;
		memset(&info->mon_data, 0, sizeof(struct mxser_mon));
		info->err_shadow = 0;
=======
static void mxser_initbrd(struct mxser_board *brd, bool high_baud)
{
	struct mxser_port *info;
	unsigned int i;
	bool is_mu860;

	brd->must_hwid = mxser_must_get_hwid(brd->ports[0].ioaddr);
	is_mu860 = brd->must_hwid == MOXA_MUST_MU860_HWID;

	for (i = 0; i < UART_INFO_NUM; i++) {
		if (Gpci_uart_info[i].type == brd->must_hwid) {
			brd->max_baud = Gpci_uart_info[i].max_baud;

			/* exception....CP-102 */
			if (high_baud)
				brd->max_baud = 921600;
			break;
		}
	}

	if (is_mu860) {
		/* set to RS232 mode by default */
		outb(0, brd->vector + 4);
		outb(0, brd->vector + 0x0c);
	}

	for (i = 0; i < brd->nports; i++) {
		info = &brd->ports[i];
		if (is_mu860) {
			if (i < 4)
				info->opmode_ioaddr = brd->vector + 4;
			else
				info->opmode_ioaddr = brd->vector + 0x0c;
		}
		tty_port_init(&info->port);
		info->port.ops = &mxser_port_ops;
		info->board = brd;

		/* Enhance mode enabled here */
		if (brd->must_hwid != MOXA_OTHER_UART)
			mxser_must_set_enhance_mode(info->ioaddr, true);

		info->type = PORT_16550A;

		mxser_process_txrx_fifo(info);

		info->port.close_delay = 5 * HZ / 10;
		info->port.closing_wait = 30 * HZ;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		spin_lock_init(&info->slock);

		/* before set INT ISR, disable all int */
		outb(inb(info->ioaddr + UART_IER) & 0xf0,
			info->ioaddr + UART_IER);
	}
<<<<<<< HEAD

	retval = request_irq(brd->irq, mxser_interrupt, IRQF_SHARED, "mxser",
			brd);
	if (retval)
		printk(KERN_ERR "Board %s: Request irq failed, IRQ (%d) may "
			"conflict with another device.\n",
			brd->info->name, brd->irq);

	return retval;
}

static int __init mxser_get_ISA_conf(int cap, struct mxser_board *brd)
{
	int id, i, bits;
	unsigned short regs[16], irq;
	unsigned char scratch, scratch2;

	brd->chip_flag = MOXA_OTHER_UART;

	id = mxser_read_register(cap, regs);
	switch (id) {
	case C168_ASIC_ID:
		brd->info = &mxser_cards[0];
		break;
	case C104_ASIC_ID:
		brd->info = &mxser_cards[1];
		break;
	case CI104J_ASIC_ID:
		brd->info = &mxser_cards[2];
		break;
	case C102_ASIC_ID:
		brd->info = &mxser_cards[5];
		break;
	case CI132_ASIC_ID:
		brd->info = &mxser_cards[6];
		break;
	case CI134_ASIC_ID:
		brd->info = &mxser_cards[7];
		break;
	default:
		return 0;
	}

	irq = 0;
	/* some ISA cards have 2 ports, but we want to see them as 4-port (why?)
	   Flag-hack checks if configuration should be read as 2-port here. */
	if (brd->info->nports == 2 || (brd->info->flags & MXSER_HAS2)) {
		irq = regs[9] & 0xF000;
		irq = irq | (irq >> 4);
		if (irq != (regs[9] & 0xFF00))
			goto err_irqconflict;
	} else if (brd->info->nports == 4) {
		irq = regs[9] & 0xF000;
		irq = irq | (irq >> 4);
		irq = irq | (irq >> 8);
		if (irq != regs[9])
			goto err_irqconflict;
	} else if (brd->info->nports == 8) {
		irq = regs[9] & 0xF000;
		irq = irq | (irq >> 4);
		irq = irq | (irq >> 8);
		if ((irq != regs[9]) || (irq != regs[10]))
			goto err_irqconflict;
	}

	if (!irq) {
		printk(KERN_ERR "mxser: interrupt number unset\n");
		return -EIO;
	}
	brd->irq = ((int)(irq & 0xF000) >> 12);
	for (i = 0; i < 8; i++)
		brd->ports[i].ioaddr = (int) regs[i + 1] & 0xFFF8;
	if ((regs[12] & 0x80) == 0) {
		printk(KERN_ERR "mxser: invalid interrupt vector\n");
		return -EIO;
	}
	brd->vector = (int)regs[11];	/* interrupt vector */
	if (id == 1)
		brd->vector_mask = 0x00FF;
	else
		brd->vector_mask = 0x000F;
	for (i = 7, bits = 0x0100; i >= 0; i--, bits <<= 1) {
		if (regs[12] & bits) {
			brd->ports[i].baud_base = 921600;
			brd->ports[i].max_baud = 921600;
		} else {
			brd->ports[i].baud_base = 115200;
			brd->ports[i].max_baud = 115200;
		}
	}
	scratch2 = inb(cap + UART_LCR) & (~UART_LCR_DLAB);
	outb(scratch2 | UART_LCR_DLAB, cap + UART_LCR);
	outb(0, cap + UART_EFR);	/* EFR is the same as FCR */
	outb(scratch2, cap + UART_LCR);
	outb(UART_FCR_ENABLE_FIFO, cap + UART_FCR);
	scratch = inb(cap + UART_IIR);

	if (scratch & 0xC0)
		brd->uart_type = PORT_16550A;
	else
		brd->uart_type = PORT_16450;
	if (!request_region(brd->ports[0].ioaddr, 8 * brd->info->nports,
			"mxser(IO)")) {
		printk(KERN_ERR "mxser: can't request ports I/O region: "
				"0x%.8lx-0x%.8lx\n",
				brd->ports[0].ioaddr, brd->ports[0].ioaddr +
				8 * brd->info->nports - 1);
		return -EIO;
	}
	if (!request_region(brd->vector, 1, "mxser(vector)")) {
		release_region(brd->ports[0].ioaddr, 8 * brd->info->nports);
		printk(KERN_ERR "mxser: can't request interrupt vector region: "
				"0x%.8lx-0x%.8lx\n",
				brd->ports[0].ioaddr, brd->ports[0].ioaddr +
				8 * brd->info->nports - 1);
		return -EIO;
	}
	return brd->info->nports;

err_irqconflict:
	printk(KERN_ERR "mxser: invalid interrupt number\n");
	return -EIO;
}

static int __devinit mxser_probe(struct pci_dev *pdev,
		const struct pci_device_id *ent)
{
#ifdef CONFIG_PCI
	struct mxser_board *brd;
	unsigned int i, j;
	unsigned long ioaddress;
	int retval = -EINVAL;

	for (i = 0; i < MXSER_BOARDS; i++)
		if (mxser_boards[i].info == NULL)
			break;

=======
}

static int mxser_probe(struct pci_dev *pdev,
		const struct pci_device_id *ent)
{
	struct mxser_board *brd;
	unsigned int i, base;
	unsigned long ioaddress;
	unsigned short nports = MXSER_NPORTS(ent->driver_data);
	struct device *tty_dev;
	int retval = -EINVAL;

	i = find_first_zero_bit(mxser_boards, MXSER_BOARDS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (i >= MXSER_BOARDS) {
		dev_err(&pdev->dev, "too many boards found (maximum %d), board "
				"not configured\n", MXSER_BOARDS);
		goto err;
	}

<<<<<<< HEAD
	brd = &mxser_boards[i];
	brd->idx = i * MXSER_PORTS_PER_BOARD;
	dev_info(&pdev->dev, "found MOXA %s board (BusNo=%d, DevNo=%d)\n",
		mxser_cards[ent->driver_data].name,
		pdev->bus->number, PCI_SLOT(pdev->devfn));

	retval = pci_enable_device(pdev);
	if (retval) {
		dev_err(&pdev->dev, "PCI enable failed\n");
		goto err;
=======
	brd = devm_kzalloc(&pdev->dev, struct_size(brd, ports, nports),
			GFP_KERNEL);
	if (!brd)
		goto err;

	brd->idx = i;
	__set_bit(brd->idx, mxser_boards);
	base = i * MXSER_PORTS_PER_BOARD;

	retval = pcim_enable_device(pdev);
	if (retval) {
		dev_err(&pdev->dev, "PCI enable failed\n");
		goto err_zero;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* io address */
	ioaddress = pci_resource_start(pdev, 2);
	retval = pci_request_region(pdev, 2, "mxser(IO)");
	if (retval)
<<<<<<< HEAD
		goto err_dis;

	brd->info = &mxser_cards[ent->driver_data];
	for (i = 0; i < brd->info->nports; i++)
=======
		goto err_zero;

	brd->nports = nports;
	for (i = 0; i < nports; i++)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		brd->ports[i].ioaddr = ioaddress + 8 * i;

	/* vector */
	ioaddress = pci_resource_start(pdev, 3);
	retval = pci_request_region(pdev, 3, "mxser(vector)");
	if (retval)
		goto err_zero;
	brd->vector = ioaddress;

	/* irq */
	brd->irq = pdev->irq;

<<<<<<< HEAD
	brd->chip_flag = CheckIsMoxaMust(brd->ports[0].ioaddr);
	brd->uart_type = PORT_16550A;
	brd->vector_mask = 0;

	for (i = 0; i < brd->info->nports; i++) {
		for (j = 0; j < UART_INFO_NUM; j++) {
			if (Gpci_uart_info[j].type == brd->chip_flag) {
				brd->ports[i].max_baud =
					Gpci_uart_info[j].max_baud;

				/* exception....CP-102 */
				if (brd->info->flags & MXSER_HIGHBAUD)
					brd->ports[i].max_baud = 921600;
				break;
			}
		}
	}

	if (brd->chip_flag == MOXA_MUST_MU860_HWID) {
		for (i = 0; i < brd->info->nports; i++) {
			if (i < 4)
				brd->ports[i].opmode_ioaddr = ioaddress + 4;
			else
				brd->ports[i].opmode_ioaddr = ioaddress + 0x0c;
		}
		outb(0, ioaddress + 4);	/* default set to RS232 mode */
		outb(0, ioaddress + 0x0c);	/* default set to RS232 mode */
	}

	for (i = 0; i < brd->info->nports; i++) {
		brd->vector_mask |= (1 << i);
		brd->ports[i].baud_base = 921600;
	}

	/* mxser_initbrd will hook ISR. */
	retval = mxser_initbrd(brd, pdev);
	if (retval)
		goto err_rel3;

	for (i = 0; i < brd->info->nports; i++)
		tty_register_device(mxvar_sdriver, brd->idx + i, &pdev->dev);

	pci_set_drvdata(pdev, brd);

	return 0;
err_rel3:
	pci_release_region(pdev, 3);
err_zero:
	brd->info = NULL;
	pci_release_region(pdev, 2);
err_dis:
	pci_disable_device(pdev);
err:
	return retval;
#else
	return -ENODEV;
#endif
}

static void __devexit mxser_remove(struct pci_dev *pdev)
{
#ifdef CONFIG_PCI
	struct mxser_board *brd = pci_get_drvdata(pdev);
	unsigned int i;

	for (i = 0; i < brd->info->nports; i++)
		tty_unregister_device(mxvar_sdriver, brd->idx + i);

	free_irq(pdev->irq, brd);
	pci_release_region(pdev, 2);
	pci_release_region(pdev, 3);
	pci_disable_device(pdev);
	brd->info = NULL;
#endif
=======
	mxser_initbrd(brd, ent->driver_data & MXSER_HIGHBAUD);

	retval = devm_request_irq(&pdev->dev, brd->irq, mxser_interrupt,
			IRQF_SHARED, "mxser", brd);
	if (retval) {
		dev_err(&pdev->dev, "request irq failed");
		goto err_relbrd;
	}

	for (i = 0; i < nports; i++) {
		tty_dev = tty_port_register_device(&brd->ports[i].port,
				mxvar_sdriver, base + i, &pdev->dev);
		if (IS_ERR(tty_dev)) {
			retval = PTR_ERR(tty_dev);
			for (; i > 0; i--)
				tty_unregister_device(mxvar_sdriver,
					base + i - 1);
			goto err_relbrd;
		}
	}

	pci_set_drvdata(pdev, brd);

	return 0;
err_relbrd:
	for (i = 0; i < nports; i++)
		tty_port_destroy(&brd->ports[i].port);
err_zero:
	__clear_bit(brd->idx, mxser_boards);
err:
	return retval;
}

static void mxser_remove(struct pci_dev *pdev)
{
	struct mxser_board *brd = pci_get_drvdata(pdev);
	unsigned int i, base = brd->idx * MXSER_PORTS_PER_BOARD;

	for (i = 0; i < brd->nports; i++) {
		tty_unregister_device(mxvar_sdriver, base + i);
		tty_port_destroy(&brd->ports[i].port);
	}

	__clear_bit(brd->idx, mxser_boards);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct pci_driver mxser_driver = {
	.name = "mxser",
	.id_table = mxser_pcibrds,
	.probe = mxser_probe,
<<<<<<< HEAD
	.remove = __devexit_p(mxser_remove)
=======
	.remove = mxser_remove
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int __init mxser_module_init(void)
{
<<<<<<< HEAD
	struct mxser_board *brd;
	unsigned int b, i, m;
	int retval;

	mxvar_sdriver = alloc_tty_driver(MXSER_PORTS + 1);
	if (!mxvar_sdriver)
		return -ENOMEM;

	printk(KERN_INFO "MOXA Smartio/Industio family driver version %s\n",
		MXSER_VERSION);
=======
	int retval;

	mxvar_sdriver = tty_alloc_driver(MXSER_PORTS, TTY_DRIVER_REAL_RAW |
			TTY_DRIVER_DYNAMIC_DEV);
	if (IS_ERR(mxvar_sdriver))
		return PTR_ERR(mxvar_sdriver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Initialize the tty_driver structure */
	mxvar_sdriver->name = "ttyMI";
	mxvar_sdriver->major = ttymajor;
	mxvar_sdriver->minor_start = 0;
	mxvar_sdriver->type = TTY_DRIVER_TYPE_SERIAL;
	mxvar_sdriver->subtype = SERIAL_TYPE_NORMAL;
	mxvar_sdriver->init_termios = tty_std_termios;
	mxvar_sdriver->init_termios.c_cflag = B9600|CS8|CREAD|HUPCL|CLOCAL;
<<<<<<< HEAD
	mxvar_sdriver->flags = TTY_DRIVER_REAL_RAW|TTY_DRIVER_DYNAMIC_DEV;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	tty_set_operations(mxvar_sdriver, &mxser_ops);

	retval = tty_register_driver(mxvar_sdriver);
	if (retval) {
		printk(KERN_ERR "Couldn't install MOXA Smartio/Industio family "
				"tty driver !\n");
		goto err_put;
	}

<<<<<<< HEAD
	/* Start finding ISA boards here */
	for (m = 0, b = 0; b < MXSER_BOARDS; b++) {
		if (!ioaddr[b])
			continue;

		brd = &mxser_boards[m];
		retval = mxser_get_ISA_conf(ioaddr[b], brd);
		if (retval <= 0) {
			brd->info = NULL;
			continue;
		}

		printk(KERN_INFO "mxser: found MOXA %s board (CAP=0x%lx)\n",
				brd->info->name, ioaddr[b]);

		/* mxser_initbrd will hook ISR. */
		if (mxser_initbrd(brd, NULL) < 0) {
			brd->info = NULL;
			continue;
		}

		brd->idx = m * MXSER_PORTS_PER_BOARD;
		for (i = 0; i < brd->info->nports; i++)
			tty_register_device(mxvar_sdriver, brd->idx + i, NULL);

		m++;
	}

	retval = pci_register_driver(&mxser_driver);
	if (retval) {
		printk(KERN_ERR "mxser: can't register pci driver\n");
		if (!m) {
			retval = -ENODEV;
			goto err_unr;
		} /* else: we have some ISA cards under control */
=======
	retval = pci_register_driver(&mxser_driver);
	if (retval) {
		printk(KERN_ERR "mxser: can't register pci driver\n");
		goto err_unr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
err_unr:
	tty_unregister_driver(mxvar_sdriver);
err_put:
<<<<<<< HEAD
	put_tty_driver(mxvar_sdriver);
=======
	tty_driver_kref_put(mxvar_sdriver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return retval;
}

static void __exit mxser_module_exit(void)
{
<<<<<<< HEAD
	unsigned int i, j;

	pci_unregister_driver(&mxser_driver);

	for (i = 0; i < MXSER_BOARDS; i++) /* ISA remains */
		if (mxser_boards[i].info != NULL)
			for (j = 0; j < mxser_boards[i].info->nports; j++)
				tty_unregister_device(mxvar_sdriver,
						mxser_boards[i].idx + j);
	tty_unregister_driver(mxvar_sdriver);
	put_tty_driver(mxvar_sdriver);

	for (i = 0; i < MXSER_BOARDS; i++)
		if (mxser_boards[i].info != NULL)
			mxser_release_ISA_res(&mxser_boards[i]);
=======
	pci_unregister_driver(&mxser_driver);
	tty_unregister_driver(mxvar_sdriver);
	tty_driver_kref_put(mxvar_sdriver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(mxser_module_init);
module_exit(mxser_module_exit);
