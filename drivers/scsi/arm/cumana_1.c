<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Generic Generic NCR5380 driver
 *
 * Copyright 1995-2002, Russell King
 */
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/signal.h>
#include <linux/ioport.h>
#include <linux/delay.h>
=======
#include <linux/ioport.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/blkdev.h>
#include <linux/init.h>

#include <asm/ecard.h>
#include <asm/io.h>

<<<<<<< HEAD
#include "../scsi.h"
#include <scsi/scsi_host.h>

#include <scsi/scsicam.h>

#define AUTOSENSE
#define PSEUDO_DMA

#define CUMANASCSI_PUBLIC_RELEASE 1

#define priv(host)			((struct NCR5380_hostdata *)(host)->hostdata)
#define NCR5380_local_declare()		struct Scsi_Host *_instance
#define NCR5380_setup(instance)		_instance = instance
#define NCR5380_read(reg)		cumanascsi_read(_instance, reg)
#define NCR5380_write(reg, value)	cumanascsi_write(_instance, reg, value)
#define NCR5380_intr			cumanascsi_intr
#define NCR5380_queue_command		cumanascsi_queue_command
#define NCR5380_proc_info		cumanascsi_proc_info

#define NCR5380_implementation_fields	\
	unsigned ctrl;			\
	void __iomem *base;		\
	void __iomem *dma

#define BOARD_NORMAL	0
#define BOARD_NCR53C400	1

#include "../NCR5380.h"

void cumanascsi_setup(char *str, int *ints)
{
}

const char *cumanascsi_info(struct Scsi_Host *spnt)
{
	return "";
}

=======
#include <scsi/scsi_host.h>

#define priv(host)			((struct NCR5380_hostdata *)(host)->hostdata)
#define NCR5380_read(reg)		cumanascsi_read(hostdata, reg)
#define NCR5380_write(reg, value)	cumanascsi_write(hostdata, reg, value)

#define NCR5380_dma_xfer_len		cumanascsi_dma_xfer_len
#define NCR5380_dma_recv_setup		cumanascsi_pread
#define NCR5380_dma_send_setup		cumanascsi_pwrite
#define NCR5380_dma_residual		NCR5380_dma_residual_none

#define NCR5380_intr			cumanascsi_intr
#define NCR5380_queue_command		cumanascsi_queue_command
#define NCR5380_info			cumanascsi_info

#define NCR5380_implementation_fields	\
	unsigned ctrl

struct NCR5380_hostdata;
static u8 cumanascsi_read(struct NCR5380_hostdata *, unsigned int);
static void cumanascsi_write(struct NCR5380_hostdata *, unsigned int, u8);

#include "../NCR5380.h"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define CTRL	0x16fc
#define STAT	0x2004
#define L(v)	(((v)<<16)|((v) & 0x0000ffff))
#define H(v)	(((v)>>16)|((v) & 0xffff0000))

<<<<<<< HEAD
static inline int
NCR5380_pwrite(struct Scsi_Host *host, unsigned char *addr, int len)
{
  unsigned long *laddr;
  void __iomem *dma = priv(host)->dma + 0x2000;

  if(!len) return 0;

  writeb(0x02, priv(host)->base + CTRL);
=======
static inline int cumanascsi_pwrite(struct NCR5380_hostdata *hostdata,
                                    unsigned char *addr, int len)
{
  unsigned long *laddr;
  u8 __iomem *base = hostdata->io;
  u8 __iomem *dma = hostdata->pdma_io + 0x2000;

  if(!len) return 0;

  writeb(0x02, base + CTRL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
  laddr = (unsigned long *)addr;
  while(len >= 32)
  {
    unsigned int status;
    unsigned long v;
<<<<<<< HEAD
    status = readb(priv(host)->base + STAT);
=======
    status = readb(base + STAT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
    if(status & 0x80)
      goto end;
    if(!(status & 0x40))
      continue;
    v=*laddr++; writew(L(v), dma); writew(H(v), dma);
    v=*laddr++; writew(L(v), dma); writew(H(v), dma);
    v=*laddr++; writew(L(v), dma); writew(H(v), dma);
    v=*laddr++; writew(L(v), dma); writew(H(v), dma);
    v=*laddr++; writew(L(v), dma); writew(H(v), dma);
    v=*laddr++; writew(L(v), dma); writew(H(v), dma);
    v=*laddr++; writew(L(v), dma); writew(H(v), dma);
    v=*laddr++; writew(L(v), dma); writew(H(v), dma);
    len -= 32;
    if(len == 0)
      break;
  }

  addr = (unsigned char *)laddr;
<<<<<<< HEAD
  writeb(0x12, priv(host)->base + CTRL);
=======
  writeb(0x12, base + CTRL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

  while(len > 0)
  {
    unsigned int status;
<<<<<<< HEAD
    status = readb(priv(host)->base + STAT);
=======
    status = readb(base + STAT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
    if(status & 0x80)
      goto end;
    if(status & 0x40)
    {
      writeb(*addr++, dma);
      if(--len == 0)
        break;
    }

<<<<<<< HEAD
    status = readb(priv(host)->base + STAT);
=======
    status = readb(base + STAT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
    if(status & 0x80)
      goto end;
    if(status & 0x40)
    {
      writeb(*addr++, dma);
      if(--len == 0)
        break;
    }
  }
end:
<<<<<<< HEAD
  writeb(priv(host)->ctrl | 0x40, priv(host)->base + CTRL);
  return len;
}

static inline int
NCR5380_pread(struct Scsi_Host *host, unsigned char *addr, int len)
{
  unsigned long *laddr;
  void __iomem *dma = priv(host)->dma + 0x2000;

  if(!len) return 0;

  writeb(0x00, priv(host)->base + CTRL);
=======
  writeb(hostdata->ctrl | 0x40, base + CTRL);

	if (len)
		return -1;
	return 0;
}

static inline int cumanascsi_pread(struct NCR5380_hostdata *hostdata,
                                   unsigned char *addr, int len)
{
  unsigned long *laddr;
  u8 __iomem *base = hostdata->io;
  u8 __iomem *dma = hostdata->pdma_io + 0x2000;

  if(!len) return 0;

  writeb(0x00, base + CTRL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
  laddr = (unsigned long *)addr;
  while(len >= 32)
  {
    unsigned int status;
<<<<<<< HEAD
    status = readb(priv(host)->base + STAT);
=======
    status = readb(base + STAT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
    if(status & 0x80)
      goto end;
    if(!(status & 0x40))
      continue;
    *laddr++ = readw(dma) | (readw(dma) << 16);
    *laddr++ = readw(dma) | (readw(dma) << 16);
    *laddr++ = readw(dma) | (readw(dma) << 16);
    *laddr++ = readw(dma) | (readw(dma) << 16);
    *laddr++ = readw(dma) | (readw(dma) << 16);
    *laddr++ = readw(dma) | (readw(dma) << 16);
    *laddr++ = readw(dma) | (readw(dma) << 16);
    *laddr++ = readw(dma) | (readw(dma) << 16);
    len -= 32;
    if(len == 0)
      break;
  }

  addr = (unsigned char *)laddr;
<<<<<<< HEAD
  writeb(0x10, priv(host)->base + CTRL);
=======
  writeb(0x10, base + CTRL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

  while(len > 0)
  {
    unsigned int status;
<<<<<<< HEAD
    status = readb(priv(host)->base + STAT);
=======
    status = readb(base + STAT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
    if(status & 0x80)
      goto end;
    if(status & 0x40)
    {
      *addr++ = readb(dma);
      if(--len == 0)
        break;
    }

<<<<<<< HEAD
    status = readb(priv(host)->base + STAT);
=======
    status = readb(base + STAT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
    if(status & 0x80)
      goto end;
    if(status & 0x40)
    {
      *addr++ = readb(dma);
      if(--len == 0)
        break;
    }
  }
end:
<<<<<<< HEAD
  writeb(priv(host)->ctrl | 0x40, priv(host)->base + CTRL);
  return len;
}

static unsigned char cumanascsi_read(struct Scsi_Host *host, unsigned int reg)
{
	void __iomem *base = priv(host)->base;
	unsigned char val;
=======
  writeb(hostdata->ctrl | 0x40, base + CTRL);

	if (len)
		return -1;
	return 0;
}

static int cumanascsi_dma_xfer_len(struct NCR5380_hostdata *hostdata,
                                   struct scsi_cmnd *cmd)
{
	return cmd->transfersize;
}

static u8 cumanascsi_read(struct NCR5380_hostdata *hostdata,
                          unsigned int reg)
{
	u8 __iomem *base = hostdata->io;
	u8 val;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	writeb(0, base + CTRL);

	val = readb(base + 0x2100 + (reg << 2));

<<<<<<< HEAD
	priv(host)->ctrl = 0x40;
=======
	hostdata->ctrl = 0x40;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	writeb(0x40, base + CTRL);

	return val;
}

<<<<<<< HEAD
static void cumanascsi_write(struct Scsi_Host *host, unsigned int reg, unsigned int value)
{
	void __iomem *base = priv(host)->base;
=======
static void cumanascsi_write(struct NCR5380_hostdata *hostdata,
                             unsigned int reg, u8 value)
{
	u8 __iomem *base = hostdata->io;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	writeb(0, base + CTRL);

	writeb(value, base + 0x2100 + (reg << 2));

<<<<<<< HEAD
	priv(host)->ctrl = 0x40;
=======
	hostdata->ctrl = 0x40;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	writeb(0x40, base + CTRL);
}

#include "../NCR5380.c"

<<<<<<< HEAD
static struct scsi_host_template cumanascsi_template = {
=======
static const struct scsi_host_template cumanascsi_template = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.module			= THIS_MODULE,
	.name			= "Cumana 16-bit SCSI",
	.info			= cumanascsi_info,
	.queuecommand		= cumanascsi_queue_command,
	.eh_abort_handler	= NCR5380_abort,
<<<<<<< HEAD
	.eh_bus_reset_handler	= NCR5380_bus_reset,
=======
	.eh_host_reset_handler	= NCR5380_host_reset,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.can_queue		= 16,
	.this_id		= 7,
	.sg_tablesize		= SG_ALL,
	.cmd_per_lun		= 2,
<<<<<<< HEAD
	.use_clustering		= DISABLE_CLUSTERING,
	.proc_name		= "CumanaSCSI-1",
};

static int __devinit
cumanascsi1_probe(struct expansion_card *ec, const struct ecard_id *id)
=======
	.proc_name		= "CumanaSCSI-1",
	.cmd_size		= sizeof(struct NCR5380_cmd),
	.max_sectors		= 128,
	.dma_boundary		= PAGE_SIZE - 1,
};

static int cumanascsi1_probe(struct expansion_card *ec,
			     const struct ecard_id *id)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct Scsi_Host *host;
	int ret;

	ret = ecard_request_resources(ec);
	if (ret)
		goto out;

	host = scsi_host_alloc(&cumanascsi_template, sizeof(struct NCR5380_hostdata));
	if (!host) {
		ret = -ENOMEM;
		goto out_release;
	}

<<<<<<< HEAD
	priv(host)->base = ioremap(ecard_resource_start(ec, ECARD_RES_IOCSLOW),
				   ecard_resource_len(ec, ECARD_RES_IOCSLOW));
	priv(host)->dma = ioremap(ecard_resource_start(ec, ECARD_RES_MEMC),
				  ecard_resource_len(ec, ECARD_RES_MEMC));
	if (!priv(host)->base || !priv(host)->dma) {
=======
	priv(host)->io = ioremap(ecard_resource_start(ec, ECARD_RES_IOCSLOW),
	                         ecard_resource_len(ec, ECARD_RES_IOCSLOW));
	priv(host)->pdma_io = ioremap(ecard_resource_start(ec, ECARD_RES_MEMC),
	                              ecard_resource_len(ec, ECARD_RES_MEMC));
	if (!priv(host)->io || !priv(host)->pdma_io) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -ENOMEM;
		goto out_unmap;
	}

	host->irq = ec->irq;

<<<<<<< HEAD
	NCR5380_init(host, 0);

        priv(host)->ctrl = 0;
        writeb(0, priv(host)->base + CTRL);

	host->n_io_port = 255;
	if (!(request_region(host->io_port, host->n_io_port, "CumanaSCSI-1"))) {
		ret = -EBUSY;
		goto out_unmap;
	}

	ret = request_irq(host->irq, cumanascsi_intr, IRQF_DISABLED,
=======
	ret = NCR5380_init(host, FLAG_DMA_FIXUP | FLAG_LATE_DMA_SETUP);
	if (ret)
		goto out_unmap;

	NCR5380_maybe_reset_bus(host);

        priv(host)->ctrl = 0;
        writeb(0, priv(host)->io + CTRL);

	ret = request_irq(host->irq, cumanascsi_intr, 0,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			  "CumanaSCSI-1", host);
	if (ret) {
		printk("scsi%d: IRQ%d not free: %d\n",
		    host->host_no, host->irq, ret);
<<<<<<< HEAD
		goto out_unmap;
	}

	printk("scsi%d: at port 0x%08lx irq %d",
		host->host_no, host->io_port, host->irq);
	printk(" options CAN_QUEUE=%d CMD_PER_LUN=%d release=%d",
		host->can_queue, host->cmd_per_lun, CUMANASCSI_PUBLIC_RELEASE);
	printk("\nscsi%d:", host->host_no);
	NCR5380_print_options(host);
	printk("\n");

=======
		goto out_exit;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = scsi_add_host(host, &ec->dev);
	if (ret)
		goto out_free_irq;

	scsi_scan_host(host);
	goto out;

 out_free_irq:
	free_irq(host->irq, host);
<<<<<<< HEAD
 out_unmap:
	iounmap(priv(host)->base);
	iounmap(priv(host)->dma);
=======
 out_exit:
	NCR5380_exit(host);
 out_unmap:
	iounmap(priv(host)->io);
	iounmap(priv(host)->pdma_io);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	scsi_host_put(host);
 out_release:
	ecard_release_resources(ec);
 out:
	return ret;
}

<<<<<<< HEAD
static void __devexit cumanascsi1_remove(struct expansion_card *ec)
{
	struct Scsi_Host *host = ecard_get_drvdata(ec);
=======
static void cumanascsi1_remove(struct expansion_card *ec)
{
	struct Scsi_Host *host = ecard_get_drvdata(ec);
	void __iomem *base = priv(host)->io;
	void __iomem *dma = priv(host)->pdma_io;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ecard_set_drvdata(ec, NULL);

	scsi_remove_host(host);
	free_irq(host->irq, host);
	NCR5380_exit(host);
<<<<<<< HEAD
	iounmap(priv(host)->base);
	iounmap(priv(host)->dma);
	scsi_host_put(host);
=======
	scsi_host_put(host);
	iounmap(base);
	iounmap(dma);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ecard_release_resources(ec);
}

static const struct ecard_id cumanascsi1_cids[] = {
	{ MANU_CUMANA, PROD_CUMANA_SCSI_1 },
	{ 0xffff, 0xffff }
};

static struct ecard_driver cumanascsi1_driver = {
	.probe		= cumanascsi1_probe,
<<<<<<< HEAD
	.remove		= __devexit_p(cumanascsi1_remove),
=======
	.remove		= cumanascsi1_remove,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.id_table	= cumanascsi1_cids,
	.drv = {
		.name		= "cumanascsi1",
	},
};

static int __init cumanascsi_init(void)
{
	return ecard_register_driver(&cumanascsi1_driver);
}

static void __exit cumanascsi_exit(void)
{
	ecard_remove_driver(&cumanascsi1_driver);
}

module_init(cumanascsi_init);
module_exit(cumanascsi_exit);

MODULE_DESCRIPTION("Cumana SCSI-1 driver for Acorn machines");
MODULE_LICENSE("GPL");
