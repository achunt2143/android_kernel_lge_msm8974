<<<<<<< HEAD
/*
 *	pci_syscall.c
 *
 * For architectures where we want to allow direct access
 * to the PCI config stuff - it would probably be preferable
 * on PCs too, but there people just do it by hand with the
 * magic northbridge registers..
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * For architectures where we want to allow direct access to the PCI config
 * stuff - it would probably be preferable on PCs too, but there people
 * just do it by hand with the magic northbridge registers.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/errno.h>
#include <linux/pci.h>
<<<<<<< HEAD
#include <linux/syscalls.h>
#include <asm/uaccess.h>
=======
#include <linux/security.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "pci.h"

SYSCALL_DEFINE5(pciconfig_read, unsigned long, bus, unsigned long, dfn,
		unsigned long, off, unsigned long, len, void __user *, buf)
{
	struct pci_dev *dev;
	u8 byte;
	u16 word;
	u32 dword;
<<<<<<< HEAD
	long err;
	long cfg_ret;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	err = -ENODEV;
	dev = pci_get_bus_and_slot(bus, dfn);
=======
	int err, cfg_ret;

	err = -EPERM;
	dev = NULL;
	if (!capable(CAP_SYS_ADMIN))
		goto error;

	err = -ENODEV;
	dev = pci_get_domain_bus_and_slot(0, bus, dfn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!dev)
		goto error;

	switch (len) {
	case 1:
		cfg_ret = pci_user_read_config_byte(dev, off, &byte);
		break;
	case 2:
		cfg_ret = pci_user_read_config_word(dev, off, &word);
		break;
	case 4:
		cfg_ret = pci_user_read_config_dword(dev, off, &dword);
		break;
	default:
		err = -EINVAL;
		goto error;
<<<<<<< HEAD
	};

	err = -EIO;
	if (cfg_ret != PCIBIOS_SUCCESSFUL)
=======
	}

	err = -EIO;
	if (cfg_ret)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto error;

	switch (len) {
	case 1:
<<<<<<< HEAD
		err = put_user(byte, (unsigned char __user *)buf);
		break;
	case 2:
		err = put_user(word, (unsigned short __user *)buf);
		break;
	case 4:
		err = put_user(dword, (unsigned int __user *)buf);
=======
		err = put_user(byte, (u8 __user *)buf);
		break;
	case 2:
		err = put_user(word, (u16 __user *)buf);
		break;
	case 4:
		err = put_user(dword, (u32 __user *)buf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}
	pci_dev_put(dev);
	return err;

error:
	/* ??? XFree86 doesn't even check the return value.  They
	   just look for 0xffffffff in the output, since that's what
	   they get instead of a machine check on x86.  */
	switch (len) {
	case 1:
<<<<<<< HEAD
		put_user(-1, (unsigned char __user *)buf);
		break;
	case 2:
		put_user(-1, (unsigned short __user *)buf);
		break;
	case 4:
		put_user(-1, (unsigned int __user *)buf);
=======
		put_user(-1, (u8 __user *)buf);
		break;
	case 2:
		put_user(-1, (u16 __user *)buf);
		break;
	case 4:
		put_user(-1, (u32 __user *)buf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}
	pci_dev_put(dev);
	return err;
}

SYSCALL_DEFINE5(pciconfig_write, unsigned long, bus, unsigned long, dfn,
		unsigned long, off, unsigned long, len, void __user *, buf)
{
	struct pci_dev *dev;
	u8 byte;
	u16 word;
	u32 dword;
	int err = 0;

<<<<<<< HEAD
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	dev = pci_get_bus_and_slot(bus, dfn);
	if (!dev)
		return -ENODEV;

	switch(len) {
=======
	if (!capable(CAP_SYS_ADMIN) ||
	    security_locked_down(LOCKDOWN_PCI_ACCESS))
		return -EPERM;

	dev = pci_get_domain_bus_and_slot(0, bus, dfn);
	if (!dev)
		return -ENODEV;

	switch (len) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case 1:
		err = get_user(byte, (u8 __user *)buf);
		if (err)
			break;
		err = pci_user_write_config_byte(dev, off, byte);
<<<<<<< HEAD
		if (err != PCIBIOS_SUCCESSFUL)
=======
		if (err)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			err = -EIO;
		break;

	case 2:
		err = get_user(word, (u16 __user *)buf);
		if (err)
			break;
		err = pci_user_write_config_word(dev, off, word);
<<<<<<< HEAD
		if (err != PCIBIOS_SUCCESSFUL)
=======
		if (err)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			err = -EIO;
		break;

	case 4:
		err = get_user(dword, (u32 __user *)buf);
		if (err)
			break;
		err = pci_user_write_config_dword(dev, off, dword);
<<<<<<< HEAD
		if (err != PCIBIOS_SUCCESSFUL)
=======
		if (err)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			err = -EIO;
		break;

	default:
		err = -EINVAL;
		break;
	}
	pci_dev_put(dev);
	return err;
}
