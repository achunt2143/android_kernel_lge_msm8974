<<<<<<< HEAD
#ifndef __OPENRISC_VMLINUX_H_
#define __OPENRISC_VMLINUX_H_

extern char _stext, _etext, _edata, _end;
#ifdef CONFIG_BLK_DEV_INITRD
extern char __initrd_start, __initrd_end;
extern char __initramfs_start;
#endif

extern u32 __dtb_start[];

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __OPENRISC_VMLINUX_H_
#define __OPENRISC_VMLINUX_H_

#ifdef CONFIG_BLK_DEV_INITRD
extern char __initrd_start, __initrd_end;
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
