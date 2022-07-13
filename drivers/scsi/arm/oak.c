<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Oak Generic NCR5380 driver
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

#define AUTOSENSE
/*#define PSEUDO_DMA*/

#define OAKSCSI_PUBLIC_RELEASE 1

#define priv(host)			((struct NCR5380_hostdata *)(host)->hostdata)
#define NCR5380_local_declare()		void __iomem *_base
#define NCR5380_setup(host)		_base = priv(host)->base

#define NCR5380_read(reg)		readb(_base + ((reg) << 2))
#define NCR5380_write(reg, value)	writeb(value, _base + ((reg) << 2))
#define NCR5380_intr			oakscsi_intr
#define NCR5380_queue_command		oakscsi_queue_command
#define NCR5380_proc_info		oakscsi_proc_info

#define NCR5380_implementation_fields	\
	void __iomem *base

#define BOARD_NORMAL	0
#define BOARD_NCR53C400	1
=======
#include <scsi/scsi_host.h>

#define priv(host)			((struct NCR5380_hostdata *)(host)->hostdata)

#define NCR5380_read(reg)           readb(hostdata->io + ((reg) << 2))
#define NCR5380_write(reg, value)   writeb(value, hostdata->io + ((reg) << 2))

#define NCR5380_dma_xfer_len		NCR5380_dma_xfer_none
#define NCR5380_dma_recv_setup		oakscsi_pread
#define NCR5380_dma_send_setup		oakscsi_pwrite
#define NCR5380_dma_residual		NCR5380_dma_residual_none

#define NCR5380_queue_command		oakscsi_queue_command
#define NCR5380_info			oakscsi_info

#define NCR5380_implementation_fields	/* none */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "../NCR5380.h"

#undef START_DMA_INITIATOR_RECEIVE_REG
#define START_DMA_INITIATOR_RECEIVE_REG	(128 + 7)

<<<<<<< HEAD
const char * oakscsi_info (struct Scsi_Host *spnt)
{
	return "";
}

#define STAT	((128 + 16) << 2)
#define DATA	((128 + 8) << 2)

static inline int NCR5380_pwrite(struct Scsi_Host *instance, unsigned char *addr,
              int len)
{
  void __iomem *base = priv(instance)->base;

printk("writing %p len %d\n",addr, len);
  if(!len) return -1;
=======
#define STAT	((128 + 16) << 2)
#define DATA	((128 + 8) << 2)

static inline int oakscsi_pwrite(struct NCR5380_hostdata *hostdata,
                                 unsigned char *addr, int len)
{
  u8 __iomem *base = hostdata->io;

printk("writing %p len %d\n",addr, len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

  while(1)
  {
    int status;
    while (((status = readw(base + STAT)) & 0x100)==0);
  }
<<<<<<< HEAD
}

static inline int NCR5380_pread(struct Scsi_Host *instance, unsigned char *addr,
              int len)
{
  void __iomem *base = priv(instance)->base;
=======
  return 0;
}

static inline int oakscsi_pread(struct NCR5380_hostdata *hostdata,
                                unsigned char *addr, int len)
{
  u8 __iomem *base = hostdata->io;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
printk("reading %p len %d\n", addr, len);
  while(len > 0)
  {
    unsigned int status, timeout;
    unsigned long b;
    
    timeout = 0x01FFFFFF;
    
    while (((status = readw(base + STAT)) & 0x100)==0)
    {
      timeout--;
      if(status & 0x200 || !timeout)
      {
        printk("status = %08X\n", status);
<<<<<<< HEAD
        return 1;
=======
        return -1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
      }
    }

    if(len >= 128)
    {
      readsw(base + DATA, addr, 128);
      addr += 128;
      len -= 128;
    }
    else
    {
      b = (unsigned long) readw(base + DATA);
      *addr ++ = b;
      len -= 1;
      if(len)
        *addr ++ = b>>8;
      len -= 1;
    }
  }
  return 0;
}

#undef STAT
#undef DATA

#include "../NCR5380.c"

<<<<<<< HEAD
static struct scsi_host_template oakscsi_template = {
	.module			= THIS_MODULE,
	.proc_info		= oakscsi_proc_info,
=======
static const struct scsi_host_template oakscsi_template = {
	.module			= THIS_MODULE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.name			= "Oak 16-bit SCSI",
	.info			= oakscsi_info,
	.queuecommand		= oakscsi_queue_command,
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
	.proc_name		= "oakscsi",
};

static int __devinit
oakscsi_probe(struct expansion_card *ec, const struct ecard_id *id)
{
	struct Scsi_Host *host;
	int ret = -ENOMEM;
=======
	.dma_boundary		= PAGE_SIZE - 1,
	.proc_name		= "oakscsi",
	.cmd_size		= sizeof(struct NCR5380_cmd),
	.max_sectors		= 128,
};

static int oakscsi_probe(struct expansion_card *ec, const struct ecard_id *id)
{
	struct Scsi_Host *host;
	int ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = ecard_request_resources(ec);
	if (ret)
		goto out;

	host = scsi_host_alloc(&oakscsi_template, sizeof(struct NCR5380_hostdata));
	if (!host) {
		ret = -ENOMEM;
		goto release;
	}

<<<<<<< HEAD
	priv(host)->base = ioremap(ecard_resource_start(ec, ECARD_RES_MEMC),
				   ecard_resource_len(ec, ECARD_RES_MEMC));
	if (!priv(host)->base) {
=======
	priv(host)->io = ioremap(ecard_resource_start(ec, ECARD_RES_MEMC),
	                         ecard_resource_len(ec, ECARD_RES_MEMC));
	if (!priv(host)->io) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -ENOMEM;
		goto unreg;
	}

<<<<<<< HEAD
	host->irq = IRQ_NONE;
	host->n_io_port = 255;

	NCR5380_init(host, 0);

	printk("scsi%d: at port 0x%08lx irqs disabled",
		host->host_no, host->io_port);
	printk(" options CAN_QUEUE=%d  CMD_PER_LUN=%d release=%d",
		host->can_queue, host->cmd_per_lun, OAKSCSI_PUBLIC_RELEASE);
	printk("\nscsi%d:", host->host_no);
	NCR5380_print_options(host);
	printk("\n");

	ret = scsi_add_host(host, &ec->dev);
	if (ret)
		goto out_unmap;
=======
	host->irq = NO_IRQ;

	ret = NCR5380_init(host, FLAG_DMA_FIXUP | FLAG_LATE_DMA_SETUP);
	if (ret)
		goto out_unmap;

	NCR5380_maybe_reset_bus(host);

	ret = scsi_add_host(host, &ec->dev);
	if (ret)
		goto out_exit;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	scsi_scan_host(host);
	goto out;

<<<<<<< HEAD
 out_unmap:
	iounmap(priv(host)->base);
=======
 out_exit:
	NCR5380_exit(host);
 out_unmap:
	iounmap(priv(host)->io);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 unreg:
	scsi_host_put(host);
 release:
	ecard_release_resources(ec);
 out:
	return ret;
}

<<<<<<< HEAD
static void __devexit oakscsi_remove(struct expansion_card *ec)
{
	struct Scsi_Host *host = ecard_get_drvdata(ec);
=======
static void oakscsi_remove(struct expansion_card *ec)
{
	struct Scsi_Host *host = ecard_get_drvdata(ec);
	void __iomem *base = priv(host)->io;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ecard_set_drvdata(ec, NULL);
	scsi_remove_host(host);

	NCR5380_exit(host);
<<<<<<< HEAD
	iounmap(priv(host)->base);
	scsi_host_put(host);
=======
	scsi_host_put(host);
	iounmap(base);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ecard_release_resources(ec);
}

static const struct ecard_id oakscsi_cids[] = {
	{ MANU_OAK, PROD_OAK_SCSI },
	{ 0xffff, 0xffff }
};

static struct ecard_driver oakscsi_driver = {
	.probe		= oakscsi_probe,
<<<<<<< HEAD
	.remove		= __devexit_p(oakscsi_remove),
=======
	.remove		= oakscsi_remove,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.id_table	= oakscsi_cids,
	.drv = {
		.name		= "oakscsi",
	},
};

static int __init oakscsi_init(void)
{
	return ecard_register_driver(&oakscsi_driver);
}

static void __exit oakscsi_exit(void)
{
	ecard_remove_driver(&oakscsi_driver);
}

module_init(oakscsi_init);
module_exit(oakscsi_exit);

MODULE_AUTHOR("Russell King");
MODULE_DESCRIPTION("Oak SCSI driver");
MODULE_LICENSE("GPL");

