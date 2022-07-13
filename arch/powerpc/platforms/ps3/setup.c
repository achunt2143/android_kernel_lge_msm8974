<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  PS3 platform setup routines.
 *
 *  Copyright (C) 2006 Sony Computer Entertainment Inc.
 *  Copyright 2006 Sony Corp.
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/root_dev.h>
#include <linux/console.h>
#include <linux/export.h>
<<<<<<< HEAD
#include <linux/bootmem.h>
=======
#include <linux/memblock.h>
#include <linux/of.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <asm/machdep.h>
#include <asm/firmware.h>
#include <asm/time.h>
#include <asm/iommu.h>
#include <asm/udbg.h>
<<<<<<< HEAD
#include <asm/prom.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/lv1call.h>
#include <asm/ps3gpu.h>

#include "platform.h"

#if defined(DEBUG)
#define DBG udbg_printf
#else
#define DBG pr_debug
#endif

/* mutex synchronizing GPU accesses and video mode changes */
DEFINE_MUTEX(ps3_gpu_mutex);
EXPORT_SYMBOL_GPL(ps3_gpu_mutex);

static union ps3_firmware_version ps3_firmware_version;
<<<<<<< HEAD
=======
static char ps3_firmware_version_str[16];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void ps3_get_firmware_version(union ps3_firmware_version *v)
{
	*v = ps3_firmware_version;
}
EXPORT_SYMBOL_GPL(ps3_get_firmware_version);

int ps3_compare_firmware_version(u16 major, u16 minor, u16 rev)
{
	union ps3_firmware_version x;

	x.pad = 0;
	x.major = major;
	x.minor = minor;
	x.rev = rev;

	return (ps3_firmware_version.raw > x.raw) -
	       (ps3_firmware_version.raw < x.raw);
}
EXPORT_SYMBOL_GPL(ps3_compare_firmware_version);

static void ps3_power_save(void)
{
	/*
	 * lv1_pause() puts the PPE thread into inactive state until an
	 * irq on an unmasked plug exists. MSR[EE] has no effect.
	 * flags: 0 = wake on DEC interrupt, 1 = ignore DEC interrupt.
	 */

	lv1_pause(0);
}

<<<<<<< HEAD
static void ps3_restart(char *cmd)
=======
static void __noreturn ps3_restart(char *cmd)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	DBG("%s:%d cmd '%s'\n", __func__, __LINE__, cmd);

	smp_send_stop();
	ps3_sys_manager_restart(); /* never returns */
}

static void ps3_power_off(void)
{
	DBG("%s:%d\n", __func__, __LINE__);

	smp_send_stop();
	ps3_sys_manager_power_off(); /* never returns */
}

<<<<<<< HEAD
static void ps3_halt(void)
=======
static void __noreturn ps3_halt(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	DBG("%s:%d\n", __func__, __LINE__);

	smp_send_stop();
	ps3_sys_manager_halt(); /* never returns */
}

static void ps3_panic(char *str)
{
	DBG("%s:%d %s\n", __func__, __LINE__, str);

	smp_send_stop();
	printk("\n");
	printk("   System does not reboot automatically.\n");
	printk("   Please press POWER button.\n");
	printk("\n");
<<<<<<< HEAD
=======
	panic_flush_kmsg_end();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	while(1)
		lv1_pause(1);
}

#if defined(CONFIG_FB_PS3) || defined(CONFIG_FB_PS3_MODULE) || \
    defined(CONFIG_PS3_FLASH) || defined(CONFIG_PS3_FLASH_MODULE)
static void __init prealloc(struct ps3_prealloc *p)
{
	if (!p->size)
		return;

<<<<<<< HEAD
	p->address = __alloc_bootmem(p->size, p->align, __pa(MAX_DMA_ADDRESS));
	if (!p->address) {
		printk(KERN_ERR "%s: Cannot allocate %s\n", __func__,
		       p->name);
		return;
	}
=======
	p->address = memblock_alloc(p->size, p->align);
	if (!p->address)
		panic("%s: Failed to allocate %lu bytes align=0x%lx\n",
		      __func__, p->size, p->align);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	printk(KERN_INFO "%s: %lu bytes at %p\n", p->name, p->size,
	       p->address);
}
#endif

#if defined(CONFIG_FB_PS3) || defined(CONFIG_FB_PS3_MODULE)
struct ps3_prealloc ps3fb_videomemory = {
	.name = "ps3fb videomemory",
	.size = CONFIG_FB_PS3_DEFAULT_SIZE_M*1024*1024,
	.align = 1024*1024		/* the GPU requires 1 MiB alignment */
};
EXPORT_SYMBOL_GPL(ps3fb_videomemory);
#define prealloc_ps3fb_videomemory()	prealloc(&ps3fb_videomemory)

static int __init early_parse_ps3fb(char *p)
{
	if (!p)
		return 1;

<<<<<<< HEAD
	ps3fb_videomemory.size = _ALIGN_UP(memparse(p, &p),
=======
	ps3fb_videomemory.size = ALIGN(memparse(p, &p),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					   ps3fb_videomemory.align);
	return 0;
}
early_param("ps3fb", early_parse_ps3fb);
#else
#define prealloc_ps3fb_videomemory()	do { } while (0)
#endif

#if defined(CONFIG_PS3_FLASH) || defined(CONFIG_PS3_FLASH_MODULE)
struct ps3_prealloc ps3flash_bounce_buffer = {
	.name = "ps3flash bounce buffer",
	.size = 256*1024,
	.align = 256*1024
};
EXPORT_SYMBOL_GPL(ps3flash_bounce_buffer);
#define prealloc_ps3flash_bounce_buffer()	prealloc(&ps3flash_bounce_buffer)

static int __init early_parse_ps3flash(char *p)
{
	if (!p)
		return 1;

	if (!strcmp(p, "off"))
		ps3flash_bounce_buffer.size = 0;

	return 0;
}
early_param("ps3flash", early_parse_ps3flash);
#else
#define prealloc_ps3flash_bounce_buffer()	do { } while (0)
#endif

<<<<<<< HEAD
static int ps3_set_dabr(unsigned long dabr)
{
	enum {DABR_USER = 1, DABR_KERNEL = 2,};

	return lv1_set_dabr(dabr, DABR_KERNEL | DABR_USER) ? -1 : 0;
}

=======
static int ps3_set_dabr(unsigned long dabr, unsigned long dabrx)
{
	/* Have to set at least one bit in the DABRX */
	if (dabrx == 0 && dabr == 0)
		dabrx = DABRX_USER;
	/* hypervisor only allows us to set BTI, Kernel and user */
	dabrx &= DABRX_BTI | DABRX_KERNEL | DABRX_USER;

	return lv1_set_dabr(dabr, dabrx) ? -1 : 0;
}

static ssize_t ps3_fw_version_show(struct kobject *kobj,
	struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%s", ps3_firmware_version_str);
}

static int __init ps3_setup_sysfs(void)
{
	static struct kobj_attribute attr = __ATTR(fw-version, S_IRUGO,
		ps3_fw_version_show, NULL);
	static struct kobject *kobj;
	int result;

	kobj = kobject_create_and_add("ps3", firmware_kobj);

	if (!kobj) {
		pr_warn("%s:%d: kobject_create_and_add failed.\n", __func__,
			__LINE__);
		return -ENOMEM;
	}

	result = sysfs_create_file(kobj, &attr.attr);

	if (result) {
		pr_warn("%s:%d: sysfs_create_file failed.\n", __func__,
			__LINE__);
		kobject_put(kobj);
		return -ENOMEM;
	}

	return 0;
}
core_initcall(ps3_setup_sysfs);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void __init ps3_setup_arch(void)
{
	u64 tmp;

	DBG(" -> %s:%d\n", __func__, __LINE__);

	lv1_get_version_info(&ps3_firmware_version.raw, &tmp);

<<<<<<< HEAD
	printk(KERN_INFO "PS3 firmware version %u.%u.%u\n",
	       ps3_firmware_version.major, ps3_firmware_version.minor,
	       ps3_firmware_version.rev);
=======
	snprintf(ps3_firmware_version_str, sizeof(ps3_firmware_version_str),
		"%u.%u.%u", ps3_firmware_version.major,
		ps3_firmware_version.minor, ps3_firmware_version.rev);

	printk(KERN_INFO "PS3 firmware version %s\n", ps3_firmware_version_str);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ps3_spu_set_platform();

#ifdef CONFIG_SMP
	smp_init_ps3();
#endif

<<<<<<< HEAD
#ifdef CONFIG_DUMMY_CONSOLE
	conswitchp = &dummy_con;
#endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	prealloc_ps3fb_videomemory();
	prealloc_ps3flash_bounce_buffer();

	ppc_md.power_save = ps3_power_save;
	ps3_os_area_init();

	DBG(" <- %s:%d\n", __func__, __LINE__);
}

static void __init ps3_progress(char *s, unsigned short hex)
{
	printk("*** %04x : %s\n", hex, s ? s : "");
}

<<<<<<< HEAD
static int __init ps3_probe(void)
{
	unsigned long htab_size;
	unsigned long dt_root;

	DBG(" -> %s:%d\n", __func__, __LINE__);

	dt_root = of_get_flat_dt_root();
	if (!of_flat_dt_is_compatible(dt_root, "sony,ps3"))
		return 0;

	powerpc_firmware_features |= FW_FEATURE_PS3_POSSIBLE;

	ps3_os_area_save_params();
	ps3_mm_init();
	ps3_mm_vas_create(&htab_size);
	ps3_hpte_init(htab_size);
=======
void __init ps3_early_mm_init(void)
{
	unsigned long htab_size;

	ps3_mm_init();
	ps3_mm_vas_create(&htab_size);
	ps3_hpte_init(htab_size);
}

static int __init ps3_probe(void)
{
	DBG(" -> %s:%d\n", __func__, __LINE__);

	ps3_os_area_save_params();

	pm_power_off = ps3_power_off;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	DBG(" <- %s:%d\n", __func__, __LINE__);
	return 1;
}

<<<<<<< HEAD
#if defined(CONFIG_KEXEC)
=======
#if defined(CONFIG_KEXEC_CORE)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void ps3_kexec_cpu_down(int crash_shutdown, int secondary)
{
	int cpu = smp_processor_id();

	DBG(" -> %s:%d: (%d)\n", __func__, __LINE__, cpu);

	ps3_smp_cleanup_cpu(cpu);
	ps3_shutdown_IRQ(cpu);

	DBG(" <- %s:%d\n", __func__, __LINE__);
}
#endif

define_machine(ps3) {
	.name				= "PS3",
<<<<<<< HEAD
=======
	.compatible			= "sony,ps3",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.probe				= ps3_probe,
	.setup_arch			= ps3_setup_arch,
	.init_IRQ			= ps3_init_IRQ,
	.panic				= ps3_panic,
	.get_boot_time			= ps3_get_boot_time,
	.set_dabr			= ps3_set_dabr,
	.calibrate_decr			= ps3_calibrate_decr,
	.progress			= ps3_progress,
	.restart			= ps3_restart,
<<<<<<< HEAD
	.power_off			= ps3_power_off,
	.halt				= ps3_halt,
#if defined(CONFIG_KEXEC)
=======
	.halt				= ps3_halt,
#if defined(CONFIG_KEXEC_CORE)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.kexec_cpu_down			= ps3_kexec_cpu_down,
#endif
};
