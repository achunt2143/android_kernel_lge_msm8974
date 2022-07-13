<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* drivers/nubus/proc.c: Proc FS interface for NuBus.

   By David Huggins-Daines <dhd@debian.org>

   Much code and many ideas from drivers/pci/proc.c:
   Copyright (c) 1997, 1998 Martin Mares <mj@atrey.karlin.mff.cuni.cz>

   This is initially based on the Zorro and PCI interfaces.  However,
   it works somewhat differently.  The intent is to provide a
   structure in /proc analogous to the structure of the NuBus ROM
   resources.

<<<<<<< HEAD
   Therefore each NuBus device is in fact a directory, which may in
   turn contain subdirectories.  The "files" correspond to NuBus
   resource records.  For those types of records which we know how to
   convert to formats that are meaningful to userspace (mostly just
   icons) these files will provide "cooked" data.  Otherwise they will
   simply provide raw access (read-only of course) to the ROM.  */
=======
   Therefore each board function gets a directory, which may in turn
   contain subdirectories.  Each slot resource is a file.  Unrecognized
   resources are empty files, since every resource ID requires a special
   case (e.g. if the resource ID implies a directory or block, then its
   value has to be interpreted as a slot ROM pointer etc.).
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/nubus.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
<<<<<<< HEAD
#include <linux/init.h>
#include <linux/module.h>

#include <asm/uaccess.h>
#include <asm/byteorder.h>

static int
nubus_devices_proc_show(struct seq_file *m, void *v)
{
	struct nubus_dev *dev = nubus_devices;

	while (dev) {
		seq_printf(m, "%x\t%04x %04x %04x %04x",
			      dev->board->slot,
			      dev->category,
			      dev->type,
			      dev->dr_sw,
			      dev->dr_hw);
		seq_printf(m, "\t%08lx\n", dev->board->slot_addr);
		dev = dev->next;
	}
	return 0;
}

static int nubus_devices_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, nubus_devices_proc_show, NULL);
}

static const struct file_operations nubus_devices_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= nubus_devices_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static struct proc_dir_entry *proc_bus_nubus_dir;

static void nubus_proc_subdir(struct nubus_dev* dev,
			      struct proc_dir_entry* parent,
			      struct nubus_dir* dir)
{
	struct nubus_dirent ent;

	/* Some of these are directories, others aren't */
	while (nubus_readdir(dir, &ent) != -1) {
		char name[8];
		struct proc_dir_entry* e;
		
		sprintf(name, "%x", ent.type);
		e = create_proc_entry(name, S_IFREG | S_IRUGO |
				      S_IWUSR, parent);
		if (!e) return;
	}
}

/* Can't do this recursively since the root directory is structured
   somewhat differently from the subdirectories */
static void nubus_proc_populate(struct nubus_dev* dev,
				struct proc_dir_entry* parent,
				struct nubus_dir* root)
{
	struct nubus_dirent ent;

	/* We know these are all directories (board resource + one or
	   more functional resources) */
	while (nubus_readdir(root, &ent) != -1) {
		char name[8];
		struct proc_dir_entry* e;
		struct nubus_dir dir;
		
		sprintf(name, "%x", ent.type);
		e = proc_mkdir(name, parent);
		if (!e) return;

		/* And descend */
		if (nubus_get_subdir(&ent, &dir) == -1) {
			/* This shouldn't happen */
			printk(KERN_ERR "NuBus root directory node %x:%x has no subdir!\n",
			       dev->board->slot, ent.type);
			continue;
		} else {
			nubus_proc_subdir(dev, e, &dir);
		}
	}
}

int nubus_proc_attach_device(struct nubus_dev *dev)
{
	struct proc_dir_entry *e;
	struct nubus_dir root;
	char name[8];

	if (dev == NULL) {
		printk(KERN_ERR
		       "NULL pointer in nubus_proc_attach_device, shoot the programmer!\n");
		return -1;
	}
		
	if (dev->board == NULL) {
		printk(KERN_ERR
		       "NULL pointer in nubus_proc_attach_device, shoot the programmer!\n");
		printk("dev = %p, dev->board = %p\n", dev, dev->board);
		return -1;
	}
		
	/* Create a directory */
	sprintf(name, "%x", dev->board->slot);
	e = dev->procdir = proc_mkdir(name, proc_bus_nubus_dir);
	if (!e)
		return -ENOMEM;

	/* Now recursively populate it with files */
	nubus_get_root_dir(dev->board, &root);
	nubus_proc_populate(dev, e, &root);

	return 0;
}
EXPORT_SYMBOL(nubus_proc_attach_device);

/* FIXME: this is certainly broken! */
int nubus_proc_detach_device(struct nubus_dev *dev)
{
	struct proc_dir_entry *e;

	if ((e = dev->procdir)) {
		if (atomic_read(&e->count))
			return -EBUSY;
		remove_proc_entry(e->name, proc_bus_nubus_dir);
		dev->procdir = NULL;
	}
	return 0;
}
EXPORT_SYMBOL(nubus_proc_detach_device);

void __init proc_bus_nubus_add_devices(void)
{
	struct nubus_dev *dev;
	
	for(dev = nubus_devices; dev; dev = dev->next)
		nubus_proc_attach_device(dev);
}

void __init nubus_proc_init(void)
{
	if (!MACH_IS_MAC)
		return;
	proc_bus_nubus_dir = proc_mkdir("bus/nubus", NULL);
	proc_create("devices", 0, proc_bus_nubus_dir, &nubus_devices_proc_fops);
	proc_bus_nubus_add_devices();
=======
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <asm/byteorder.h>

/*
 * /proc/bus/nubus/devices stuff
 */

static int
nubus_devices_proc_show(struct seq_file *m, void *v)
{
	struct nubus_rsrc *fres;

	for_each_func_rsrc(fres)
		seq_printf(m, "%x\t%04x %04x %04x %04x\t%08lx\n",
			   fres->board->slot, fres->category, fres->type,
			   fres->dr_sw, fres->dr_hw, fres->board->slot_addr);
	return 0;
}

static struct proc_dir_entry *proc_bus_nubus_dir;

/*
 * /proc/bus/nubus/x/ stuff
 */

struct proc_dir_entry *nubus_proc_add_board(struct nubus_board *board)
{
	char name[2];

	if (!proc_bus_nubus_dir || !nubus_populate_procfs)
		return NULL;
	snprintf(name, sizeof(name), "%x", board->slot);
	return proc_mkdir(name, proc_bus_nubus_dir);
}

/* The PDE private data for any directory under /proc/bus/nubus/x/
 * is the bytelanes value for the board in slot x.
 */

struct proc_dir_entry *nubus_proc_add_rsrc_dir(struct proc_dir_entry *procdir,
					       const struct nubus_dirent *ent,
					       struct nubus_board *board)
{
	char name[9];
	int lanes = board->lanes;

	if (!procdir || !nubus_populate_procfs)
		return NULL;
	snprintf(name, sizeof(name), "%x", ent->type);
	remove_proc_subtree(name, procdir);
	return proc_mkdir_data(name, 0555, procdir, (void *)lanes);
}

/* The PDE private data for a file under /proc/bus/nubus/x/ is a pointer to
 * an instance of the following structure, which gives the location and size
 * of the resource data in the slot ROM. For slot resources which hold only a
 * small integer, this integer value is stored directly and size is set to 0.
 * A NULL private data pointer indicates an unrecognized resource.
 */

struct nubus_proc_pde_data {
	unsigned char *res_ptr;
	unsigned int res_size;
};

static struct nubus_proc_pde_data *
nubus_proc_alloc_pde_data(unsigned char *ptr, unsigned int size)
{
	struct nubus_proc_pde_data *pded;

	pded = kmalloc(sizeof(*pded), GFP_KERNEL);
	if (!pded)
		return NULL;

	pded->res_ptr = ptr;
	pded->res_size = size;
	return pded;
}

static int nubus_proc_rsrc_show(struct seq_file *m, void *v)
{
	struct inode *inode = m->private;
	struct nubus_proc_pde_data *pded;

	pded = pde_data(inode);
	if (!pded)
		return 0;

	if (pded->res_size > m->size)
		return -EFBIG;

	if (pded->res_size) {
		int lanes = (int)proc_get_parent_data(inode);
		struct nubus_dirent ent;

		if (!lanes)
			return 0;

		ent.mask = lanes;
		ent.base = pded->res_ptr;
		ent.data = 0;
		nubus_seq_write_rsrc_mem(m, &ent, pded->res_size);
	} else {
		unsigned int data = (unsigned int)pded->res_ptr;

		seq_putc(m, data >> 16);
		seq_putc(m, data >> 8);
		seq_putc(m, data >> 0);
	}
	return 0;
}

static int nubus_rsrc_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, nubus_proc_rsrc_show, inode);
}

static const struct proc_ops nubus_rsrc_proc_ops = {
	.proc_open	= nubus_rsrc_proc_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_release	= single_release,
};

void nubus_proc_add_rsrc_mem(struct proc_dir_entry *procdir,
			     const struct nubus_dirent *ent,
			     unsigned int size)
{
	char name[9];
	struct nubus_proc_pde_data *pded;

	if (!procdir || !nubus_populate_procfs)
		return;

	snprintf(name, sizeof(name), "%x", ent->type);
	if (size)
		pded = nubus_proc_alloc_pde_data(nubus_dirptr(ent), size);
	else
		pded = NULL;
	remove_proc_subtree(name, procdir);
	proc_create_data(name, S_IFREG | 0444, procdir,
			 &nubus_rsrc_proc_ops, pded);
}

void nubus_proc_add_rsrc(struct proc_dir_entry *procdir,
			 const struct nubus_dirent *ent)
{
	char name[9];
	unsigned char *data = (unsigned char *)ent->data;

	if (!procdir || !nubus_populate_procfs)
		return;

	snprintf(name, sizeof(name), "%x", ent->type);
	remove_proc_subtree(name, procdir);
	proc_create_data(name, S_IFREG | 0444, procdir,
			 &nubus_rsrc_proc_ops,
			 nubus_proc_alloc_pde_data(data, 0));
}

/*
 * /proc/nubus stuff
 */

void __init nubus_proc_init(void)
{
	proc_create_single("nubus", 0, NULL, nubus_proc_show);
	proc_bus_nubus_dir = proc_mkdir("bus/nubus", NULL);
	if (!proc_bus_nubus_dir)
		return;
	proc_create_single("devices", 0, proc_bus_nubus_dir,
			nubus_devices_proc_show);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
