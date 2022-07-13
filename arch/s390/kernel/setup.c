<<<<<<< HEAD
/*
 *  arch/s390/kernel/setup.c
 *
 *  S390 version
 *    Copyright (C) IBM Corp. 1999,2012
=======
// SPDX-License-Identifier: GPL-2.0
/*
 *  S390 version
 *    Copyright IBM Corp. 1999, 2012
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *    Author(s): Hartmut Penner (hp@de.ibm.com),
 *               Martin Schwidefsky (schwidefsky@de.ibm.com)
 *
 *  Derived from "arch/i386/kernel/setup.c"
 *    Copyright (C) 1995, Linus Torvalds
 */

/*
 * This file handles the architecture-dependent parts of initialization
 */

#define KMSG_COMPONENT "setup"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt

#include <linux/errno.h>
<<<<<<< HEAD
#include <linux/module.h>
#include <linux/sched.h>
=======
#include <linux/export.h>
#include <linux/sched.h>
#include <linux/sched/task.h>
#include <linux/cpu.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/memblock.h>
#include <linux/mm.h>
#include <linux/stddef.h>
#include <linux/unistd.h>
#include <linux/ptrace.h>
<<<<<<< HEAD
=======
#include <linux/random.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/user.h>
#include <linux/tty.h>
#include <linux/ioport.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/initrd.h>
<<<<<<< HEAD
#include <linux/bootmem.h>
#include <linux/root_dev.h>
#include <linux/console.h>
#include <linux/kernel_stat.h>
=======
#include <linux/root_dev.h>
#include <linux/console.h>
#include <linux/kernel_stat.h>
#include <linux/dma-map-ops.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/device.h>
#include <linux/notifier.h>
#include <linux/pfn.h>
#include <linux/ctype.h>
#include <linux/reboot.h>
#include <linux/topology.h>
<<<<<<< HEAD
#include <linux/ftrace.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kexec.h>
#include <linux/crash_dump.h>
#include <linux/memory.h>
#include <linux/compat.h>
<<<<<<< HEAD

#include <asm/ipl.h>
#include <asm/uaccess.h>
=======
#include <linux/start_kernel.h>
#include <linux/hugetlb.h>
#include <linux/kmemleak.h>

#include <asm/archrandom.h>
#include <asm/boot_data.h>
#include <asm/ipl.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/facility.h>
#include <asm/smp.h>
#include <asm/mmu_context.h>
#include <asm/cpcmd.h>
<<<<<<< HEAD
#include <asm/lowcore.h>
=======
#include <asm/abs_lowcore.h>
#include <asm/nmi.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/irq.h>
#include <asm/page.h>
#include <asm/ptrace.h>
#include <asm/sections.h>
#include <asm/ebcdic.h>
<<<<<<< HEAD
#include <asm/kvm_virtio.h>
#include <asm/diag.h>
#include <asm/os_info.h>
#include "entry.h"

long psw_kernel_bits	= PSW_DEFAULT_KEY | PSW_MASK_BASE | PSW_ASC_PRIMARY |
			  PSW_MASK_EA | PSW_MASK_BA;
long psw_user_bits	= PSW_MASK_DAT | PSW_MASK_IO | PSW_MASK_EXT |
			  PSW_DEFAULT_KEY | PSW_MASK_BASE | PSW_MASK_MCHECK |
			  PSW_MASK_PSTATE | PSW_ASC_HOME;

/*
 * User copy operations.
 */
struct uaccess_ops uaccess;
EXPORT_SYMBOL(uaccess);

=======
#include <asm/diag.h>
#include <asm/os_info.h>
#include <asm/sclp.h>
#include <asm/stacktrace.h>
#include <asm/sysinfo.h>
#include <asm/numa.h>
#include <asm/alternative.h>
#include <asm/nospec-branch.h>
#include <asm/physmem_info.h>
#include <asm/maccess.h>
#include <asm/uv.h>
#include <asm/asm-offsets.h>
#include "entry.h"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Machine setup..
 */
unsigned int console_mode = 0;
EXPORT_SYMBOL(console_mode);

unsigned int console_devno = -1;
EXPORT_SYMBOL(console_devno);

unsigned int console_irq = -1;
EXPORT_SYMBOL(console_irq);

<<<<<<< HEAD
unsigned long elf_hwcap = 0;
char elf_platform[ELF_PLATFORM_SIZE];

struct mem_chunk __initdata memory_chunk[MEMORY_CHUNKS];

int __initdata memory_end_set;
unsigned long __initdata memory_end;
=======
/*
 * Some code and data needs to stay below 2 GB, even when the kernel would be
 * relocated above 2 GB, because it has to use 31 bit addresses.
 * Such code and data is part of the .amode31 section.
 */
char __amode31_ref *__samode31 = _samode31;
char __amode31_ref *__eamode31 = _eamode31;
char __amode31_ref *__stext_amode31 = _stext_amode31;
char __amode31_ref *__etext_amode31 = _etext_amode31;
struct exception_table_entry __amode31_ref *__start_amode31_ex_table = _start_amode31_ex_table;
struct exception_table_entry __amode31_ref *__stop_amode31_ex_table = _stop_amode31_ex_table;

/*
 * Control registers CR2, CR5 and CR15 are initialized with addresses
 * of tables that must be placed below 2G which is handled by the AMODE31
 * sections.
 * Because the AMODE31 sections are relocated below 2G at startup,
 * the content of control registers CR2, CR5 and CR15 must be updated
 * with new addresses after the relocation. The initial initialization of
 * control registers occurs in head64.S and then gets updated again after AMODE31
 * relocation. We must access the relevant AMODE31 tables indirectly via
 * pointers placed in the .amode31.refs linker section. Those pointers get
 * updated automatically during AMODE31 relocation and always contain a valid
 * address within AMODE31 sections.
 */

static __amode31_data u32 __ctl_duct_amode31[16] __aligned(64);

static __amode31_data u64 __ctl_aste_amode31[8] __aligned(64) = {
	[1] = 0xffffffffffffffff
};

static __amode31_data u32 __ctl_duald_amode31[32] __aligned(128) = {
	0x80000000, 0, 0, 0,
	0x80000000, 0, 0, 0,
	0x80000000, 0, 0, 0,
	0x80000000, 0, 0, 0,
	0x80000000, 0, 0, 0,
	0x80000000, 0, 0, 0,
	0x80000000, 0, 0, 0,
	0x80000000, 0, 0, 0
};

static __amode31_data u32 __ctl_linkage_stack_amode31[8] __aligned(64) = {
	0, 0, 0x89000000, 0,
	0, 0, 0x8a000000, 0
};

static u64 __amode31_ref *__ctl_aste = __ctl_aste_amode31;
static u32 __amode31_ref *__ctl_duald = __ctl_duald_amode31;
static u32 __amode31_ref *__ctl_linkage_stack = __ctl_linkage_stack_amode31;
static u32 __amode31_ref *__ctl_duct = __ctl_duct_amode31;

unsigned long __bootdata_preserved(max_mappable);
unsigned long __bootdata(ident_map_size);
struct physmem_info __bootdata(physmem_info);

unsigned long __bootdata_preserved(__kaslr_offset);
int __bootdata_preserved(__kaslr_enabled);
unsigned int __bootdata_preserved(zlib_dfltcc_support);
EXPORT_SYMBOL(zlib_dfltcc_support);
u64 __bootdata_preserved(stfle_fac_list[16]);
EXPORT_SYMBOL(stfle_fac_list);
u64 __bootdata_preserved(alt_stfle_fac_list[16]);
struct oldmem_data __bootdata_preserved(oldmem_data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

unsigned long VMALLOC_START;
EXPORT_SYMBOL(VMALLOC_START);

unsigned long VMALLOC_END;
EXPORT_SYMBOL(VMALLOC_END);

struct page *vmemmap;
EXPORT_SYMBOL(vmemmap);
<<<<<<< HEAD

/* An array with a pointer to the lowcore of every CPU. */
struct _lowcore *lowcore_ptr[NR_CPUS];
EXPORT_SYMBOL(lowcore_ptr);

=======
unsigned long vmemmap_size;

unsigned long MODULES_VADDR;
unsigned long MODULES_END;

/* An array with a pointer to the lowcore of every CPU. */
struct lowcore *lowcore_ptr[NR_CPUS];
EXPORT_SYMBOL(lowcore_ptr);

DEFINE_STATIC_KEY_FALSE(cpu_has_bear);

/*
 * The Write Back bit position in the physaddr is given by the SLPC PCI.
 * Leaving the mask zero always uses write through which is safe
 */
unsigned long mio_wb_bit_mask __ro_after_init;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * This is set up by the setup-routine at boot-time
 * for S390 need to find out, what we have to setup
 * using address 0x10400 ...
 */

#include <asm/setup.h>

/*
 * condev= and conmode= setup parameter.
 */

static int __init condev_setup(char *str)
{
	int vdev;

	vdev = simple_strtoul(str, &str, 0);
	if (vdev >= 0 && vdev < 65536) {
		console_devno = vdev;
		console_irq = -1;
	}
	return 1;
}

__setup("condev=", condev_setup);

static void __init set_preferred_console(void)
{
<<<<<<< HEAD
	if (MACHINE_IS_KVM)
		add_preferred_console("hvc", 0, NULL);
	else if (CONSOLE_IS_3215 || CONSOLE_IS_SCLP)
		add_preferred_console("ttyS", 0, NULL);
	else if (CONSOLE_IS_3270)
		add_preferred_console("tty3270", 0, NULL);
=======
	if (CONSOLE_IS_3215 || CONSOLE_IS_SCLP)
		add_preferred_console("ttyS", 0, NULL);
	else if (CONSOLE_IS_3270)
		add_preferred_console("tty3270", 0, NULL);
	else if (CONSOLE_IS_VT220)
		add_preferred_console("ttysclp", 0, NULL);
	else if (CONSOLE_IS_HVC)
		add_preferred_console("hvc", 0, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int __init conmode_setup(char *str)
{
#if defined(CONFIG_SCLP_CONSOLE) || defined(CONFIG_SCLP_VT220_CONSOLE)
<<<<<<< HEAD
	if (strncmp(str, "hwc", 4) == 0 || strncmp(str, "sclp", 5) == 0)
                SET_CONSOLE_SCLP;
#endif
#if defined(CONFIG_TN3215_CONSOLE)
	if (strncmp(str, "3215", 5) == 0)
		SET_CONSOLE_3215;
#endif
#if defined(CONFIG_TN3270_CONSOLE)
	if (strncmp(str, "3270", 5) == 0)
=======
	if (!strcmp(str, "hwc") || !strcmp(str, "sclp"))
                SET_CONSOLE_SCLP;
#endif
#if defined(CONFIG_TN3215_CONSOLE)
	if (!strcmp(str, "3215"))
		SET_CONSOLE_3215;
#endif
#if defined(CONFIG_TN3270_CONSOLE)
	if (!strcmp(str, "3270"))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		SET_CONSOLE_3270;
#endif
	set_preferred_console();
        return 1;
}

__setup("conmode=", conmode_setup);

static void __init conmode_default(void)
{
	char query_buffer[1024];
	char *ptr;

        if (MACHINE_IS_VM) {
		cpcmd("QUERY CONSOLE", query_buffer, 1024, NULL);
		console_devno = simple_strtoul(query_buffer + 5, NULL, 16);
		ptr = strstr(query_buffer, "SUBCHANNEL =");
		console_irq = simple_strtoul(ptr + 13, NULL, 16);
		cpcmd("QUERY TERM", query_buffer, 1024, NULL);
		ptr = strstr(query_buffer, "CONMODE");
		/*
		 * Set the conmode to 3215 so that the device recognition 
		 * will set the cu_type of the console to 3215. If the
		 * conmode is 3270 and we don't set it back then both
		 * 3215 and the 3270 driver will try to access the console
		 * device (3215 as console and 3270 as normal tty).
		 */
		cpcmd("TERM CONMODE 3215", NULL, 0, NULL);
		if (ptr == NULL) {
#if defined(CONFIG_SCLP_CONSOLE) || defined(CONFIG_SCLP_VT220_CONSOLE)
			SET_CONSOLE_SCLP;
#endif
			return;
		}
<<<<<<< HEAD
		if (strncmp(ptr + 8, "3270", 4) == 0) {
=======
		if (str_has_prefix(ptr + 8, "3270")) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#if defined(CONFIG_TN3270_CONSOLE)
			SET_CONSOLE_3270;
#elif defined(CONFIG_TN3215_CONSOLE)
			SET_CONSOLE_3215;
#elif defined(CONFIG_SCLP_CONSOLE) || defined(CONFIG_SCLP_VT220_CONSOLE)
			SET_CONSOLE_SCLP;
#endif
<<<<<<< HEAD
		} else if (strncmp(ptr + 8, "3215", 4) == 0) {
=======
		} else if (str_has_prefix(ptr + 8, "3215")) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#if defined(CONFIG_TN3215_CONSOLE)
			SET_CONSOLE_3215;
#elif defined(CONFIG_TN3270_CONSOLE)
			SET_CONSOLE_3270;
#elif defined(CONFIG_SCLP_CONSOLE) || defined(CONFIG_SCLP_VT220_CONSOLE)
			SET_CONSOLE_SCLP;
#endif
		}
<<<<<<< HEAD
=======
	} else if (MACHINE_IS_KVM) {
		if (sclp.has_vt220 && IS_ENABLED(CONFIG_SCLP_VT220_CONSOLE))
			SET_CONSOLE_VT220;
		else if (sclp.has_linemode && IS_ENABLED(CONFIG_SCLP_CONSOLE))
			SET_CONSOLE_SCLP;
		else
			SET_CONSOLE_HVC;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
#if defined(CONFIG_SCLP_CONSOLE) || defined(CONFIG_SCLP_VT220_CONSOLE)
		SET_CONSOLE_SCLP;
#endif
	}
}

<<<<<<< HEAD
#ifdef CONFIG_ZFCPDUMP
static void __init setup_zfcpdump(unsigned int console_devno)
{
	static char str[41];

	if (ipl_info.type != IPL_TYPE_FCP_DUMP)
		return;
	if (OLDMEM_BASE)
		return;
	if (console_devno != -1)
		sprintf(str, " cio_ignore=all,!0.0.%04x,!0.0.%04x",
			ipl_info.data.fcp.dev_id.devno, console_devno);
	else
		sprintf(str, " cio_ignore=all,!0.0.%04x",
			ipl_info.data.fcp.dev_id.devno);
	strcat(boot_command_line, str);
	console_loglevel = 2;
}
#else
static inline void setup_zfcpdump(unsigned int console_devno) {}
#endif /* CONFIG_ZFCPDUMP */
=======
#ifdef CONFIG_CRASH_DUMP
static void __init setup_zfcpdump(void)
{
	if (!is_ipl_type_dump())
		return;
	if (oldmem_data.start)
		return;
	strlcat(boot_command_line, " cio_ignore=all,!ipldev,!condev", COMMAND_LINE_SIZE);
	console_loglevel = 2;
}
#else
static inline void setup_zfcpdump(void) {}
#endif /* CONFIG_CRASH_DUMP */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

 /*
 * Reboot, halt and power_off stubs. They just call _machine_restart,
 * _machine_halt or _machine_power_off. 
 */

void machine_restart(char *command)
{
	if ((!in_interrupt() && !in_atomic()) || oops_in_progress)
		/*
		 * Only unblank the console if we are called in enabled
		 * context or a bust_spinlocks cleared the way for us.
		 */
		console_unblank();
	_machine_restart(command);
}

void machine_halt(void)
{
	if (!in_interrupt() || oops_in_progress)
		/*
		 * Only unblank the console if we are called in enabled
		 * context or a bust_spinlocks cleared the way for us.
		 */
		console_unblank();
	_machine_halt();
}

void machine_power_off(void)
{
	if (!in_interrupt() || oops_in_progress)
		/*
		 * Only unblank the console if we are called in enabled
		 * context or a bust_spinlocks cleared the way for us.
		 */
		console_unblank();
	_machine_power_off();
}

/*
 * Dummy power off function.
 */
void (*pm_power_off)(void) = machine_power_off;
<<<<<<< HEAD

static int __init early_parse_mem(char *p)
{
	memory_end = memparse(p, &p);
	memory_end_set = 1;
	return 0;
}
early_param("mem", early_parse_mem);

static int __init parse_vmalloc(char *arg)
{
	if (!arg)
		return -EINVAL;
	VMALLOC_END = (memparse(arg, &arg) + PAGE_SIZE - 1) & PAGE_MASK;
	return 0;
}
early_param("vmalloc", parse_vmalloc);

unsigned int user_mode = HOME_SPACE_MODE;
EXPORT_SYMBOL_GPL(user_mode);

static int set_amode_primary(void)
{
	psw_kernel_bits = (psw_kernel_bits & ~PSW_MASK_ASC) | PSW_ASC_HOME;
	psw_user_bits = (psw_user_bits & ~PSW_MASK_ASC) | PSW_ASC_PRIMARY;
#ifdef CONFIG_COMPAT
	psw32_user_bits =
		(psw32_user_bits & ~PSW32_MASK_ASC) | PSW32_ASC_PRIMARY;
#endif

	if (MACHINE_HAS_MVCOS) {
		memcpy(&uaccess, &uaccess_mvcos_switch, sizeof(uaccess));
		return 1;
	} else {
		memcpy(&uaccess, &uaccess_pt, sizeof(uaccess));
		return 0;
	}
}

/*
 * Switch kernel/user addressing modes?
 */
static int __init early_parse_switch_amode(char *p)
{
	user_mode = PRIMARY_SPACE_MODE;
	return 0;
}
early_param("switch_amode", early_parse_switch_amode);

static int __init early_parse_user_mode(char *p)
{
	if (p && strcmp(p, "primary") == 0)
		user_mode = PRIMARY_SPACE_MODE;
	else if (!p || strcmp(p, "home") == 0)
		user_mode = HOME_SPACE_MODE;
	else
		return 1;
	return 0;
}
early_param("user_mode", early_parse_user_mode);

static void setup_addressing_mode(void)
{
	if (user_mode == PRIMARY_SPACE_MODE) {
		if (set_amode_primary())
			pr_info("Address spaces switched, "
				"mvcos available\n");
		else
			pr_info("Address spaces switched, "
				"mvcos not available\n");
	}
}

void *restart_stack __attribute__((__section__(".data")));

static void __init setup_lowcore(void)
{
	struct _lowcore *lc;
=======
EXPORT_SYMBOL_GPL(pm_power_off);

void *restart_stack;

unsigned long stack_alloc(void)
{
#ifdef CONFIG_VMAP_STACK
	void *ret;

	ret = __vmalloc_node(THREAD_SIZE, THREAD_SIZE, THREADINFO_GFP,
			     NUMA_NO_NODE, __builtin_return_address(0));
	kmemleak_not_leak(ret);
	return (unsigned long)ret;
#else
	return __get_free_pages(GFP_KERNEL, THREAD_SIZE_ORDER);
#endif
}

void stack_free(unsigned long stack)
{
#ifdef CONFIG_VMAP_STACK
	vfree((void *) stack);
#else
	free_pages(stack, THREAD_SIZE_ORDER);
#endif
}

static unsigned long __init stack_alloc_early(void)
{
	unsigned long stack;

	stack = (unsigned long)memblock_alloc(THREAD_SIZE, THREAD_SIZE);
	if (!stack) {
		panic("%s: Failed to allocate %lu bytes align=0x%lx\n",
		      __func__, THREAD_SIZE, THREAD_SIZE);
	}
	return stack;
}

static void __init setup_lowcore(void)
{
	struct lowcore *lc, *abs_lc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Setup lowcore for boot cpu
	 */
<<<<<<< HEAD
	BUILD_BUG_ON(sizeof(struct _lowcore) != LC_PAGES * 4096);
	lc = __alloc_bootmem_low(LC_PAGES * PAGE_SIZE, LC_PAGES * PAGE_SIZE, 0);
	lc->restart_psw.mask = psw_kernel_bits;
	lc->restart_psw.addr =
		PSW_ADDR_AMODE | (unsigned long) restart_int_handler;
	lc->external_new_psw.mask = psw_kernel_bits |
		PSW_MASK_DAT | PSW_MASK_MCHECK;
	lc->external_new_psw.addr =
		PSW_ADDR_AMODE | (unsigned long) ext_int_handler;
	lc->svc_new_psw.mask = psw_kernel_bits |
		PSW_MASK_DAT | PSW_MASK_IO | PSW_MASK_EXT | PSW_MASK_MCHECK;
	lc->svc_new_psw.addr = PSW_ADDR_AMODE | (unsigned long) system_call;
	lc->program_new_psw.mask = psw_kernel_bits |
		PSW_MASK_DAT | PSW_MASK_MCHECK;
	lc->program_new_psw.addr =
		PSW_ADDR_AMODE | (unsigned long) pgm_check_handler;
	lc->mcck_new_psw.mask = psw_kernel_bits;
	lc->mcck_new_psw.addr =
		PSW_ADDR_AMODE | (unsigned long) mcck_int_handler;
	lc->io_new_psw.mask = psw_kernel_bits |
		PSW_MASK_DAT | PSW_MASK_MCHECK;
	lc->io_new_psw.addr = PSW_ADDR_AMODE | (unsigned long) io_int_handler;
	lc->clock_comparator = -1ULL;
	lc->kernel_stack = ((unsigned long) &init_thread_union) + THREAD_SIZE;
	lc->async_stack = (unsigned long)
		__alloc_bootmem(ASYNC_SIZE, ASYNC_SIZE, 0) + ASYNC_SIZE;
	lc->panic_stack = (unsigned long)
		__alloc_bootmem(PAGE_SIZE, PAGE_SIZE, 0) + PAGE_SIZE;
	lc->current_task = (unsigned long) init_thread_union.thread_info.task;
	lc->thread_info = (unsigned long) &init_thread_union;
	lc->machine_flags = S390_lowcore.machine_flags;
	lc->stfl_fac_list = S390_lowcore.stfl_fac_list;
	memcpy(lc->stfle_fac_list, S390_lowcore.stfle_fac_list,
	       MAX_FACILITY_BIT/8);
#ifndef CONFIG_64BIT
	if (MACHINE_HAS_IEEE) {
		lc->extended_save_area_addr = (__u32)
			__alloc_bootmem_low(PAGE_SIZE, PAGE_SIZE, 0);
		/* enable extended save area */
		__ctl_set_bit(14, 29);
	}
#else
	lc->vdso_per_cpu_data = (unsigned long) &lc->paste[0];
#endif
	lc->sync_enter_timer = S390_lowcore.sync_enter_timer;
	lc->async_enter_timer = S390_lowcore.async_enter_timer;
=======
	BUILD_BUG_ON(sizeof(struct lowcore) != LC_PAGES * PAGE_SIZE);
	lc = memblock_alloc_low(sizeof(*lc), sizeof(*lc));
	if (!lc)
		panic("%s: Failed to allocate %zu bytes align=%zx\n",
		      __func__, sizeof(*lc), sizeof(*lc));

	lc->restart_psw.mask = PSW_KERNEL_BITS & ~PSW_MASK_DAT;
	lc->restart_psw.addr = __pa(restart_int_handler);
	lc->external_new_psw.mask = PSW_KERNEL_BITS;
	lc->external_new_psw.addr = (unsigned long) ext_int_handler;
	lc->svc_new_psw.mask = PSW_KERNEL_BITS;
	lc->svc_new_psw.addr = (unsigned long) system_call;
	lc->program_new_psw.mask = PSW_KERNEL_BITS;
	lc->program_new_psw.addr = (unsigned long) pgm_check_handler;
	lc->mcck_new_psw.mask = PSW_KERNEL_BITS;
	lc->mcck_new_psw.addr = (unsigned long) mcck_int_handler;
	lc->io_new_psw.mask = PSW_KERNEL_BITS;
	lc->io_new_psw.addr = (unsigned long) io_int_handler;
	lc->clock_comparator = clock_comparator_max;
	lc->current_task = (unsigned long)&init_task;
	lc->lpp = LPP_MAGIC;
	lc->machine_flags = S390_lowcore.machine_flags;
	lc->preempt_count = S390_lowcore.preempt_count;
	nmi_alloc_mcesa_early(&lc->mcesad);
	lc->sys_enter_timer = S390_lowcore.sys_enter_timer;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	lc->exit_timer = S390_lowcore.exit_timer;
	lc->user_timer = S390_lowcore.user_timer;
	lc->system_timer = S390_lowcore.system_timer;
	lc->steal_timer = S390_lowcore.steal_timer;
	lc->last_update_timer = S390_lowcore.last_update_timer;
	lc->last_update_clock = S390_lowcore.last_update_clock;
<<<<<<< HEAD
	lc->ftrace_func = S390_lowcore.ftrace_func;

	restart_stack = __alloc_bootmem(ASYNC_SIZE, ASYNC_SIZE, 0);
	restart_stack += ASYNC_SIZE;

	/*
	 * Set up PSW restart to call ipl.c:do_restart(). Copy the relevant
	 * restart data to the absolute zero lowcore. This is necesary if
=======
	/*
	 * Allocate the global restart stack which is the same for
	 * all CPUs in case *one* of them does a PSW restart.
	 */
	restart_stack = (void *)(stack_alloc_early() + STACK_INIT_OFFSET);
	lc->mcck_stack = stack_alloc_early() + STACK_INIT_OFFSET;
	lc->async_stack = stack_alloc_early() + STACK_INIT_OFFSET;
	lc->nodat_stack = stack_alloc_early() + STACK_INIT_OFFSET;
	lc->kernel_stack = S390_lowcore.kernel_stack;
	/*
	 * Set up PSW restart to call ipl.c:do_restart(). Copy the relevant
	 * restart data to the absolute zero lowcore. This is necessary if
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * PSW restart is done on an offline CPU that has lowcore zero.
	 */
	lc->restart_stack = (unsigned long) restart_stack;
	lc->restart_fn = (unsigned long) do_restart;
	lc->restart_data = 0;
<<<<<<< HEAD
	lc->restart_source = -1UL;
	memcpy(&S390_lowcore.restart_stack, &lc->restart_stack,
	       4*sizeof(unsigned long));
	copy_to_absolute_zero(&S390_lowcore.restart_psw,
			      &lc->restart_psw, sizeof(psw_t));

	set_prefix((u32)(unsigned long) lc);
	lowcore_ptr[0] = lc;
=======
	lc->restart_source = -1U;
	lc->spinlock_lockval = arch_spin_lockval(0);
	lc->spinlock_index = 0;
	arch_spin_lock_setup(0);
	lc->return_lpswe = gen_lpswe(__LC_RETURN_PSW);
	lc->return_mcck_lpswe = gen_lpswe(__LC_RETURN_MCCK_PSW);
	lc->preempt_count = PREEMPT_DISABLED;
	lc->kernel_asce = S390_lowcore.kernel_asce;
	lc->user_asce = S390_lowcore.user_asce;

	system_ctlreg_init_save_area(lc);
	abs_lc = get_abs_lowcore();
	abs_lc->restart_stack = lc->restart_stack;
	abs_lc->restart_fn = lc->restart_fn;
	abs_lc->restart_data = lc->restart_data;
	abs_lc->restart_source = lc->restart_source;
	abs_lc->restart_psw = lc->restart_psw;
	abs_lc->restart_flags = RESTART_FLAG_CTLREGS;
	abs_lc->program_new_psw = lc->program_new_psw;
	abs_lc->mcesad = lc->mcesad;
	put_abs_lowcore(abs_lc);

	set_prefix(__pa(lc));
	lowcore_ptr[0] = lc;
	if (abs_lowcore_map(0, lowcore_ptr[0], false))
		panic("Couldn't setup absolute lowcore");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct resource code_resource = {
	.name  = "Kernel code",
<<<<<<< HEAD
	.flags = IORESOURCE_BUSY | IORESOURCE_MEM,
=======
	.flags = IORESOURCE_BUSY | IORESOURCE_SYSTEM_RAM,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct resource data_resource = {
	.name = "Kernel data",
<<<<<<< HEAD
	.flags = IORESOURCE_BUSY | IORESOURCE_MEM,
=======
	.flags = IORESOURCE_BUSY | IORESOURCE_SYSTEM_RAM,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct resource bss_resource = {
	.name = "Kernel bss",
<<<<<<< HEAD
	.flags = IORESOURCE_BUSY | IORESOURCE_MEM,
=======
	.flags = IORESOURCE_BUSY | IORESOURCE_SYSTEM_RAM,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct resource __initdata *standard_resources[] = {
	&code_resource,
	&data_resource,
	&bss_resource,
};

static void __init setup_resources(void)
{
	struct resource *res, *std_res, *sub_res;
<<<<<<< HEAD
	int i, j;

	code_resource.start = (unsigned long) &_text;
	code_resource.end = (unsigned long) &_etext - 1;
	data_resource.start = (unsigned long) &_etext;
	data_resource.end = (unsigned long) &_edata - 1;
	bss_resource.start = (unsigned long) &__bss_start;
	bss_resource.end = (unsigned long) &__bss_stop - 1;

	for (i = 0; i < MEMORY_CHUNKS; i++) {
		if (!memory_chunk[i].size)
			continue;
		if (memory_chunk[i].type == CHUNK_OLDMEM ||
		    memory_chunk[i].type == CHUNK_CRASHK)
			continue;
		res = alloc_bootmem_low(sizeof(*res));
		res->flags = IORESOURCE_BUSY | IORESOURCE_MEM;
		switch (memory_chunk[i].type) {
		case CHUNK_READ_WRITE:
		case CHUNK_CRASHK:
			res->name = "System RAM";
			break;
		case CHUNK_READ_ONLY:
			res->name = "System ROM";
			res->flags |= IORESOURCE_READONLY;
			break;
		default:
			res->name = "reserved";
		}
		res->start = memory_chunk[i].addr;
		res->end = res->start + memory_chunk[i].size - 1;
=======
	phys_addr_t start, end;
	int j;
	u64 i;

	code_resource.start = __pa_symbol(_text);
	code_resource.end = __pa_symbol(_etext) - 1;
	data_resource.start = __pa_symbol(_etext);
	data_resource.end = __pa_symbol(_edata) - 1;
	bss_resource.start = __pa_symbol(__bss_start);
	bss_resource.end = __pa_symbol(__bss_stop) - 1;

	for_each_mem_range(i, &start, &end) {
		res = memblock_alloc(sizeof(*res), 8);
		if (!res)
			panic("%s: Failed to allocate %zu bytes align=0x%x\n",
			      __func__, sizeof(*res), 8);
		res->flags = IORESOURCE_BUSY | IORESOURCE_SYSTEM_RAM;

		res->name = "System RAM";
		res->start = start;
		/*
		 * In memblock, end points to the first byte after the
		 * range while in resources, end points to the last byte in
		 * the range.
		 */
		res->end = end - 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		request_resource(&iomem_resource, res);

		for (j = 0; j < ARRAY_SIZE(standard_resources); j++) {
			std_res = standard_resources[j];
			if (std_res->start < res->start ||
			    std_res->start > res->end)
				continue;
			if (std_res->end > res->end) {
<<<<<<< HEAD
				sub_res = alloc_bootmem_low(sizeof(*sub_res));
=======
				sub_res = memblock_alloc(sizeof(*sub_res), 8);
				if (!sub_res)
					panic("%s: Failed to allocate %zu bytes align=0x%x\n",
					      __func__, sizeof(*sub_res), 8);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				*sub_res = *std_res;
				sub_res->end = res->end;
				std_res->start = res->end + 1;
				request_resource(res, sub_res);
			} else {
				request_resource(res, std_res);
			}
		}
	}
<<<<<<< HEAD
}

unsigned long real_memory_size;
EXPORT_SYMBOL_GPL(real_memory_size);

static void __init setup_memory_end(void)
{
	unsigned long vmax, vmalloc_size, tmp;
	int i;


#ifdef CONFIG_ZFCPDUMP
	if (ipl_info.type == IPL_TYPE_FCP_DUMP && !OLDMEM_BASE) {
		memory_end = ZFCPDUMP_HSA_SIZE;
		memory_end_set = 1;
	}
#endif
	real_memory_size = 0;
	memory_end &= PAGE_MASK;

	/*
	 * Make sure all chunks are MAX_ORDER aligned so we don't need the
	 * extra checks that HOLES_IN_ZONE would require.
	 */
	for (i = 0; i < MEMORY_CHUNKS; i++) {
		unsigned long start, end;
		struct mem_chunk *chunk;
		unsigned long align;

		chunk = &memory_chunk[i];
		align = 1UL << (MAX_ORDER + PAGE_SHIFT - 1);
		start = (chunk->addr + align - 1) & ~(align - 1);
		end = (chunk->addr + chunk->size) & ~(align - 1);
		if (start >= end)
			memset(chunk, 0, sizeof(*chunk));
		else {
			chunk->addr = start;
			chunk->size = end - start;
		}
		real_memory_size = max(real_memory_size,
				       chunk->addr + chunk->size);
	}

	/* Choose kernel address space layout: 2, 3, or 4 levels. */
#ifdef CONFIG_64BIT
	vmalloc_size = VMALLOC_END ?: 128UL << 30;
	tmp = (memory_end ?: real_memory_size) / PAGE_SIZE;
	tmp = tmp * (sizeof(struct page) + PAGE_SIZE) + vmalloc_size;
	if (tmp <= (1UL << 42))
		vmax = 1UL << 42;	/* 3-level kernel page table */
	else
		vmax = 1UL << 53;	/* 4-level kernel page table */
#else
	vmalloc_size = VMALLOC_END ?: 96UL << 20;
	vmax = 1UL << 31;		/* 2-level kernel page table */
#endif
	/* vmalloc area is at the end of the kernel address space. */
	VMALLOC_END = vmax;
	VMALLOC_START = vmax - vmalloc_size;

	/* Split remaining virtual space between 1:1 mapping & vmemmap array */
	tmp = VMALLOC_START / (PAGE_SIZE + sizeof(struct page));
	tmp = VMALLOC_START - tmp * sizeof(struct page);
	tmp &= ~((vmax >> 11) - 1);	/* align to page table level */
	tmp = min(tmp, 1UL << MAX_PHYSMEM_BITS);
	vmemmap = (struct page *) tmp;

	/* Take care that memory_end is set and <= vmemmap */
	memory_end = min(memory_end ?: real_memory_size, tmp);

	/* Fixup memory chunk array to fit into 0..memory_end */
	for (i = 0; i < MEMORY_CHUNKS; i++) {
		struct mem_chunk *chunk = &memory_chunk[i];

		if (chunk->addr >= memory_end) {
			memset(chunk, 0, sizeof(*chunk));
			continue;
		}
		if (chunk->addr + chunk->size > memory_end)
			chunk->size = memory_end - chunk->addr;
	}
}

static void __init setup_vmcoreinfo(void)
{
#ifdef CONFIG_KEXEC
	unsigned long ptr = paddr_vmcoreinfo_note();

	copy_to_absolute_zero(&S390_lowcore.vmcore_info, &ptr, sizeof(ptr));
#endif
=======
#ifdef CONFIG_CRASH_DUMP
	/*
	 * Re-add removed crash kernel memory as reserved memory. This makes
	 * sure it will be mapped with the identity mapping and struct pages
	 * will be created, so it can be resized later on.
	 * However add it later since the crash kernel resource should not be
	 * part of the System RAM resource.
	 */
	if (crashk_res.end) {
		memblock_add_node(crashk_res.start, resource_size(&crashk_res),
				  0, MEMBLOCK_NONE);
		memblock_reserve(crashk_res.start, resource_size(&crashk_res));
		insert_resource(&iomem_resource, &crashk_res);
	}
#endif
}

static void __init setup_memory_end(void)
{
	max_pfn = max_low_pfn = PFN_DOWN(ident_map_size);
	pr_notice("The maximum memory size is %luMB\n", ident_map_size >> 20);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifdef CONFIG_CRASH_DUMP

/*
<<<<<<< HEAD
 * Find suitable location for crashkernel memory
 */
static unsigned long __init find_crash_base(unsigned long crash_size,
					    char **msg)
{
	unsigned long crash_base;
	struct mem_chunk *chunk;
	int i;

	if (memory_chunk[0].size < crash_size) {
		*msg = "first memory chunk must be at least crashkernel size";
		return 0;
	}
	if (OLDMEM_BASE && crash_size == OLDMEM_SIZE)
		return OLDMEM_BASE;

	for (i = MEMORY_CHUNKS - 1; i >= 0; i--) {
		chunk = &memory_chunk[i];
		if (chunk->size == 0)
			continue;
		if (chunk->type != CHUNK_READ_WRITE)
			continue;
		if (chunk->size < crash_size)
			continue;
		crash_base = (chunk->addr + chunk->size) - crash_size;
		if (crash_base < crash_size)
			continue;
		if (crash_base < ZFCPDUMP_HSA_SIZE_MAX)
			continue;
		if (crash_base < (unsigned long) INITRD_START + INITRD_SIZE)
			continue;
		return crash_base;
	}
	*msg = "no suitable area found";
	return 0;
}

/*
 * Check if crash_base and crash_size is valid
 */
static int __init verify_crash_base(unsigned long crash_base,
				    unsigned long crash_size,
				    char **msg)
{
	struct mem_chunk *chunk;
	int i;

	/*
	 * Because we do the swap to zero, we must have at least 'crash_size'
	 * bytes free space before crash_base
	 */
	if (crash_size > crash_base) {
		*msg = "crashkernel offset must be greater than size";
		return -EINVAL;
	}

	/* First memory chunk must be at least crash_size */
	if (memory_chunk[0].size < crash_size) {
		*msg = "first memory chunk must be at least crashkernel size";
		return -EINVAL;
	}
	/* Check if we fit into the respective memory chunk */
	for (i = 0; i < MEMORY_CHUNKS; i++) {
		chunk = &memory_chunk[i];
		if (chunk->size == 0)
			continue;
		if (crash_base < chunk->addr)
			continue;
		if (crash_base >= chunk->addr + chunk->size)
			continue;
		/* we have found the memory chunk */
		if (crash_base + crash_size > chunk->addr + chunk->size) {
			*msg = "selected memory chunk is too small for "
				"crashkernel memory";
			return -EINVAL;
		}
		return 0;
	}
	*msg = "invalid memory range specified";
	return -EINVAL;
}

/*
 * Reserve kdump memory by creating a memory hole in the mem_chunk array
 */
static void __init reserve_kdump_bootmem(unsigned long addr, unsigned long size,
					 int type)
{
	create_mem_hole(memory_chunk, addr, size, type);
}

/*
 * When kdump is enabled, we have to ensure that no memory from
 * the area [0 - crashkernel memory size] and
 * [crashk_res.start - crashk_res.end] is set offline.
=======
 * When kdump is enabled, we have to ensure that no memory from the area
 * [0 - crashkernel memory size] is set offline - it will be exchanged with
 * the crashkernel memory region when kdump is triggered. The crashkernel
 * memory region can never get offlined (pages are unmovable).
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static int kdump_mem_notifier(struct notifier_block *nb,
			      unsigned long action, void *data)
{
	struct memory_notify *arg = data;

<<<<<<< HEAD
	if (arg->start_pfn < PFN_DOWN(resource_size(&crashk_res)))
		return NOTIFY_BAD;
	if (arg->start_pfn > PFN_DOWN(crashk_res.end))
		return NOTIFY_OK;
	if (arg->start_pfn + arg->nr_pages - 1 < PFN_DOWN(crashk_res.start))
		return NOTIFY_OK;
	return NOTIFY_BAD;
=======
	if (action != MEM_GOING_OFFLINE)
		return NOTIFY_OK;
	if (arg->start_pfn < PFN_DOWN(resource_size(&crashk_res)))
		return NOTIFY_BAD;
	return NOTIFY_OK;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct notifier_block kdump_mem_nb = {
	.notifier_call = kdump_mem_notifier,
};

#endif

/*
<<<<<<< HEAD
 * Make sure that oldmem, where the dump is stored, is protected
 */
static void reserve_oldmem(void)
{
#ifdef CONFIG_CRASH_DUMP
	if (!OLDMEM_BASE)
		return;

	reserve_kdump_bootmem(OLDMEM_BASE, OLDMEM_SIZE, CHUNK_OLDMEM);
	reserve_kdump_bootmem(OLDMEM_SIZE, memory_end - OLDMEM_SIZE,
			      CHUNK_OLDMEM);
	if (OLDMEM_BASE + OLDMEM_SIZE == real_memory_size)
		saved_max_pfn = PFN_DOWN(OLDMEM_BASE) - 1;
	else
		saved_max_pfn = PFN_DOWN(real_memory_size) - 1;
#endif
=======
 * Reserve page tables created by decompressor
 */
static void __init reserve_pgtables(void)
{
	unsigned long start, end;
	struct reserved_range *range;

	for_each_physmem_reserved_type_range(RR_VMEM, range, &start, &end)
		memblock_reserve(start, end - start);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Reserve memory for kdump kernel to be loaded with kexec
 */
static void __init reserve_crashkernel(void)
{
#ifdef CONFIG_CRASH_DUMP
	unsigned long long crash_base, crash_size;
<<<<<<< HEAD
	char *msg = NULL;
	int rc;

	rc = parse_crashkernel(boot_command_line, memory_end, &crash_size,
			       &crash_base);
	if (rc || crash_size == 0)
		return;
	crash_base = ALIGN(crash_base, KEXEC_CRASH_MEM_ALIGN);
	crash_size = ALIGN(crash_size, KEXEC_CRASH_MEM_ALIGN);
	if (register_memory_notifier(&kdump_mem_nb))
		return;
	if (!crash_base)
		crash_base = find_crash_base(crash_size, &msg);
	if (!crash_base) {
		pr_info("crashkernel reservation failed: %s\n", msg);
		unregister_memory_notifier(&kdump_mem_nb);
		return;
	}
	if (verify_crash_base(crash_base, crash_size, &msg)) {
		pr_info("crashkernel reservation failed: %s\n", msg);
		unregister_memory_notifier(&kdump_mem_nb);
		return;
	}
	if (!OLDMEM_BASE && MACHINE_IS_VM)
		diag10_range(PFN_DOWN(crash_base), PFN_DOWN(crash_size));
	crashk_res.start = crash_base;
	crashk_res.end = crash_base + crash_size - 1;
	insert_resource(&iomem_resource, &crashk_res);
	reserve_kdump_bootmem(crash_base, crash_size, CHUNK_CRASHK);
	pr_info("Reserving %lluMB of memory at %lluMB "
		"for crashkernel (System RAM: %luMB)\n",
		crash_size >> 20, crash_base >> 20, memory_end >> 20);
=======
	phys_addr_t low, high;
	int rc;

	rc = parse_crashkernel(boot_command_line, ident_map_size,
			       &crash_size, &crash_base, NULL, NULL);

	crash_base = ALIGN(crash_base, KEXEC_CRASH_MEM_ALIGN);
	crash_size = ALIGN(crash_size, KEXEC_CRASH_MEM_ALIGN);
	if (rc || crash_size == 0)
		return;

	if (memblock.memory.regions[0].size < crash_size) {
		pr_info("crashkernel reservation failed: %s\n",
			"first memory chunk must be at least crashkernel size");
		return;
	}

	low = crash_base ?: oldmem_data.start;
	high = low + crash_size;
	if (low >= oldmem_data.start && high <= oldmem_data.start + oldmem_data.size) {
		/* The crashkernel fits into OLDMEM, reuse OLDMEM */
		crash_base = low;
	} else {
		/* Find suitable area in free memory */
		low = max_t(unsigned long, crash_size, sclp.hsa_size);
		high = crash_base ? crash_base + crash_size : ULONG_MAX;

		if (crash_base && crash_base < low) {
			pr_info("crashkernel reservation failed: %s\n",
				"crash_base too low");
			return;
		}
		low = crash_base ?: low;
		crash_base = memblock_phys_alloc_range(crash_size,
						       KEXEC_CRASH_MEM_ALIGN,
						       low, high);
	}

	if (!crash_base) {
		pr_info("crashkernel reservation failed: %s\n",
			"no suitable area found");
		return;
	}

	if (register_memory_notifier(&kdump_mem_nb)) {
		memblock_phys_free(crash_base, crash_size);
		return;
	}

	if (!oldmem_data.start && MACHINE_IS_VM)
		diag10_range(PFN_DOWN(crash_base), PFN_DOWN(crash_size));
	crashk_res.start = crash_base;
	crashk_res.end = crash_base + crash_size - 1;
	memblock_remove(crash_base, crash_size);
	pr_info("Reserving %lluMB of memory at %lluMB "
		"for crashkernel (System RAM: %luMB)\n",
		crash_size >> 20, crash_base >> 20,
		(unsigned long)memblock.memory.total_size >> 20);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	os_info_crashkernel_add(crash_base, crash_size);
#endif
}

<<<<<<< HEAD
static void __init setup_memory(void)
{
        unsigned long bootmap_size;
	unsigned long start_pfn, end_pfn;
	int i;

	/*
	 * partially used pages are not usable - thus
	 * we are rounding upwards:
	 */
	start_pfn = PFN_UP(__pa(&_end));
	end_pfn = max_pfn = PFN_DOWN(memory_end);

#ifdef CONFIG_BLK_DEV_INITRD
	/*
	 * Move the initrd in case the bitmap of the bootmem allocater
	 * would overwrite it.
	 */

	if (INITRD_START && INITRD_SIZE) {
		unsigned long bmap_size;
		unsigned long start;

		bmap_size = bootmem_bootmap_pages(end_pfn - start_pfn + 1);
		bmap_size = PFN_PHYS(bmap_size);

		if (PFN_PHYS(start_pfn) + bmap_size > INITRD_START) {
			start = PFN_PHYS(start_pfn) + bmap_size + PAGE_SIZE;

#ifdef CONFIG_CRASH_DUMP
			if (OLDMEM_BASE) {
				/* Move initrd behind kdump oldmem */
				if (start + INITRD_SIZE > OLDMEM_BASE &&
				    start < OLDMEM_BASE + OLDMEM_SIZE)
					start = OLDMEM_BASE + OLDMEM_SIZE;
			}
#endif
			if (start + INITRD_SIZE > memory_end) {
				pr_err("initrd extends beyond end of "
				       "memory (0x%08lx > 0x%08lx) "
				       "disabling initrd\n",
				       start + INITRD_SIZE, memory_end);
				INITRD_START = INITRD_SIZE = 0;
			} else {
				pr_info("Moving initrd (0x%08lx -> "
					"0x%08lx, size: %ld)\n",
					INITRD_START, start, INITRD_SIZE);
				memmove((void *) start, (void *) INITRD_START,
					INITRD_SIZE);
				INITRD_START = start;
			}
		}
	}
#endif

	/*
	 * Initialize the boot-time allocator
	 */
	bootmap_size = init_bootmem(start_pfn, end_pfn);

	/*
	 * Register RAM areas with the bootmem allocator.
	 */

	for (i = 0; i < MEMORY_CHUNKS && memory_chunk[i].size > 0; i++) {
		unsigned long start_chunk, end_chunk, pfn;

		if (memory_chunk[i].type != CHUNK_READ_WRITE &&
		    memory_chunk[i].type != CHUNK_CRASHK)
			continue;
		start_chunk = PFN_DOWN(memory_chunk[i].addr);
		end_chunk = start_chunk + PFN_DOWN(memory_chunk[i].size);
		end_chunk = min(end_chunk, end_pfn);
		if (start_chunk >= end_chunk)
			continue;
		memblock_add_node(PFN_PHYS(start_chunk),
				  PFN_PHYS(end_chunk - start_chunk), 0);
		pfn = max(start_chunk, start_pfn);
		for (; pfn < end_chunk; pfn++)
			page_set_storage_key(PFN_PHYS(pfn),
					     PAGE_DEFAULT_KEY, 0);
	}

	psw_set_key(PAGE_DEFAULT_KEY);

	free_bootmem_with_active_regions(0, max_pfn);

	/*
	 * Reserve memory used for lowcore/command line/kernel image.
	 */
	reserve_bootmem(0, (unsigned long)_ehead, BOOTMEM_DEFAULT);
	reserve_bootmem((unsigned long)_stext,
			PFN_PHYS(start_pfn) - (unsigned long)_stext,
			BOOTMEM_DEFAULT);
	/*
	 * Reserve the bootmem bitmap itself as well. We do this in two
	 * steps (first step was init_bootmem()) because this catches
	 * the (very unlikely) case of us accidentally initializing the
	 * bootmem allocator with an invalid RAM area.
	 */
	reserve_bootmem(start_pfn << PAGE_SHIFT, bootmap_size,
			BOOTMEM_DEFAULT);

#ifdef CONFIG_CRASH_DUMP
	if (crashk_res.start)
		reserve_bootmem(crashk_res.start,
				crashk_res.end - crashk_res.start + 1,
				BOOTMEM_DEFAULT);
	if (is_kdump_kernel())
		reserve_bootmem(elfcorehdr_addr - OLDMEM_BASE,
				PAGE_ALIGN(elfcorehdr_size), BOOTMEM_DEFAULT);
#endif
#ifdef CONFIG_BLK_DEV_INITRD
	if (INITRD_START && INITRD_SIZE) {
		if (INITRD_START + INITRD_SIZE <= memory_end) {
			reserve_bootmem(INITRD_START, INITRD_SIZE,
					BOOTMEM_DEFAULT);
			initrd_start = INITRD_START;
			initrd_end = initrd_start + INITRD_SIZE;
		} else {
			pr_err("initrd extends beyond end of "
			       "memory (0x%08lx > 0x%08lx) "
			       "disabling initrd\n",
			       initrd_start + INITRD_SIZE, memory_end);
			initrd_start = initrd_end = 0;
		}
	}
#endif
}

/*
 * Setup hardware capabilities.
 */
static void __init setup_hwcaps(void)
{
	static const int stfl_bits[6] = { 0, 2, 7, 17, 19, 21 };
	struct cpuid cpu_id;
	int i;

	/*
	 * The store facility list bits numbers as found in the principles
	 * of operation are numbered with bit 1UL<<31 as number 0 to
	 * bit 1UL<<0 as number 31.
	 *   Bit 0: instructions named N3, "backported" to esa-mode
	 *   Bit 2: z/Architecture mode is active
	 *   Bit 7: the store-facility-list-extended facility is installed
	 *   Bit 17: the message-security assist is installed
	 *   Bit 19: the long-displacement facility is installed
	 *   Bit 21: the extended-immediate facility is installed
	 *   Bit 22: extended-translation facility 3 is installed
	 *   Bit 30: extended-translation facility 3 enhancement facility
	 * These get translated to:
	 *   HWCAP_S390_ESAN3 bit 0, HWCAP_S390_ZARCH bit 1,
	 *   HWCAP_S390_STFLE bit 2, HWCAP_S390_MSA bit 3,
	 *   HWCAP_S390_LDISP bit 4, HWCAP_S390_EIMM bit 5 and
	 *   HWCAP_S390_ETF3EH bit 8 (22 && 30).
	 */
	for (i = 0; i < 6; i++)
		if (test_facility(stfl_bits[i]))
			elf_hwcap |= 1UL << i;

	if (test_facility(22) && test_facility(30))
		elf_hwcap |= HWCAP_S390_ETF3EH;

	/*
	 * Check for additional facilities with store-facility-list-extended.
	 * stfle stores doublewords (8 byte) with bit 1ULL<<63 as bit 0
	 * and 1ULL<<0 as bit 63. Bits 0-31 contain the same information
	 * as stored by stfl, bits 32-xxx contain additional facilities.
	 * How many facility words are stored depends on the number of
	 * doublewords passed to the instruction. The additional facilities
	 * are:
	 *   Bit 42: decimal floating point facility is installed
	 *   Bit 44: perform floating point operation facility is installed
	 * translated to:
	 *   HWCAP_S390_DFP bit 6 (42 && 44).
	 */
	if ((elf_hwcap & (1UL << 2)) && test_facility(42) && test_facility(44))
		elf_hwcap |= HWCAP_S390_DFP;

	/*
	 * Huge page support HWCAP_S390_HPAGE is bit 7.
	 */
	if (MACHINE_HAS_HPAGE)
		elf_hwcap |= HWCAP_S390_HPAGE;

	/*
	 * 64-bit register support for 31-bit processes
	 * HWCAP_S390_HIGH_GPRS is bit 9.
	 */
	elf_hwcap |= HWCAP_S390_HIGH_GPRS;

	get_cpu_id(&cpu_id);
	switch (cpu_id.machine) {
	case 0x9672:
#if !defined(CONFIG_64BIT)
	default:	/* Use "g5" as default for 31 bit kernels. */
#endif
		strcpy(elf_platform, "g5");
		break;
	case 0x2064:
	case 0x2066:
#if defined(CONFIG_64BIT)
	default:	/* Use "z900" as default for 64 bit kernels. */
#endif
		strcpy(elf_platform, "z900");
		break;
	case 0x2084:
	case 0x2086:
		strcpy(elf_platform, "z990");
		break;
	case 0x2094:
	case 0x2096:
		strcpy(elf_platform, "z9-109");
		break;
	case 0x2097:
	case 0x2098:
		strcpy(elf_platform, "z10");
		break;
	case 0x2817:
	case 0x2818:
		strcpy(elf_platform, "z196");
		break;
=======
/*
 * Reserve the initrd from being used by memblock
 */
static void __init reserve_initrd(void)
{
	unsigned long addr, size;

	if (!IS_ENABLED(CONFIG_BLK_DEV_INITRD) || !get_physmem_reserved(RR_INITRD, &addr, &size))
		return;
	initrd_start = (unsigned long)__va(addr);
	initrd_end = initrd_start + size;
	memblock_reserve(addr, size);
}

/*
 * Reserve the memory area used to pass the certificate lists
 */
static void __init reserve_certificate_list(void)
{
	if (ipl_cert_list_addr)
		memblock_reserve(ipl_cert_list_addr, ipl_cert_list_size);
}

static void __init reserve_physmem_info(void)
{
	unsigned long addr, size;

	if (get_physmem_reserved(RR_MEM_DETECT_EXTENDED, &addr, &size))
		memblock_reserve(addr, size);
}

static void __init free_physmem_info(void)
{
	unsigned long addr, size;

	if (get_physmem_reserved(RR_MEM_DETECT_EXTENDED, &addr, &size))
		memblock_phys_free(addr, size);
}

static void __init memblock_add_physmem_info(void)
{
	unsigned long start, end;
	int i;

	pr_debug("physmem info source: %s (%hhd)\n",
		 get_physmem_info_source(), physmem_info.info_source);
	/* keep memblock lists close to the kernel */
	memblock_set_bottom_up(true);
	for_each_physmem_usable_range(i, &start, &end)
		memblock_add(start, end - start);
	for_each_physmem_online_range(i, &start, &end)
		memblock_physmem_add(start, end - start);
	memblock_set_bottom_up(false);
	memblock_set_node(0, ULONG_MAX, &memblock.memory, 0);
}

/*
 * Reserve memory used for lowcore/command line/kernel image.
 */
static void __init reserve_kernel(void)
{
	memblock_reserve(0, STARTUP_NORMAL_OFFSET);
	memblock_reserve(OLDMEM_BASE, sizeof(unsigned long));
	memblock_reserve(OLDMEM_SIZE, sizeof(unsigned long));
	memblock_reserve(physmem_info.reserved[RR_AMODE31].start, __eamode31 - __samode31);
	memblock_reserve(__pa(sclp_early_sccb), EXT_SCCB_READ_SCP);
	memblock_reserve(__pa(_stext), _end - _stext);
}

static void __init setup_memory(void)
{
	phys_addr_t start, end;
	u64 i;

	/*
	 * Init storage key for present memory
	 */
	for_each_mem_range(i, &start, &end)
		storage_key_init_range(start, end);

	psw_set_key(PAGE_DEFAULT_KEY);
}

static void __init relocate_amode31_section(void)
{
	unsigned long amode31_size = __eamode31 - __samode31;
	long amode31_offset, *ptr;

	amode31_offset = physmem_info.reserved[RR_AMODE31].start - (unsigned long)__samode31;
	pr_info("Relocating AMODE31 section of size 0x%08lx\n", amode31_size);

	/* Move original AMODE31 section to the new one */
	memmove((void *)physmem_info.reserved[RR_AMODE31].start, __samode31, amode31_size);
	/* Zero out the old AMODE31 section to catch invalid accesses within it */
	memset(__samode31, 0, amode31_size);

	/* Update all AMODE31 region references */
	for (ptr = _start_amode31_refs; ptr != _end_amode31_refs; ptr++)
		*ptr += amode31_offset;
}

/* This must be called after AMODE31 relocation */
static void __init setup_cr(void)
{
	union ctlreg2 cr2;
	union ctlreg5 cr5;
	union ctlreg15 cr15;

	__ctl_duct[1] = (unsigned long)__ctl_aste;
	__ctl_duct[2] = (unsigned long)__ctl_aste;
	__ctl_duct[4] = (unsigned long)__ctl_duald;

	/* Update control registers CR2, CR5 and CR15 */
	local_ctl_store(2, &cr2.reg);
	local_ctl_store(5, &cr5.reg);
	local_ctl_store(15, &cr15.reg);
	cr2.ducto = (unsigned long)__ctl_duct >> 6;
	cr5.pasteo = (unsigned long)__ctl_duct >> 6;
	cr15.lsea = (unsigned long)__ctl_linkage_stack >> 3;
	system_ctl_load(2, &cr2.reg);
	system_ctl_load(5, &cr5.reg);
	system_ctl_load(15, &cr15.reg);
}

/*
 * Add system information as device randomness
 */
static void __init setup_randomness(void)
{
	struct sysinfo_3_2_2 *vmms;

	vmms = memblock_alloc(PAGE_SIZE, PAGE_SIZE);
	if (!vmms)
		panic("Failed to allocate memory for sysinfo structure\n");
	if (stsi(vmms, 3, 2, 2) == 0 && vmms->count)
		add_device_randomness(&vmms->vm, sizeof(vmms->vm[0]) * vmms->count);
	memblock_free(vmms, PAGE_SIZE);

	if (cpacf_query_func(CPACF_PRNO, CPACF_PRNO_TRNG))
		static_branch_enable(&s390_arch_random_available);
}

/*
 * Issue diagnose 318 to set the control program name and
 * version codes.
 */
static void __init setup_control_program_code(void)
{
	union diag318_info diag318_info = {
		.cpnc = CPNC_LINUX,
		.cpvc = 0,
	};

	if (!sclp.has_diag318)
		return;

	diag_stat_inc(DIAG_STAT_X318);
	asm volatile("diag %0,0,0x318\n" : : "d" (diag318_info.val));
}

/*
 * Print the component list from the IPL report
 */
static void __init log_component_list(void)
{
	struct ipl_rb_component_entry *ptr, *end;
	char *str;

	if (!early_ipl_comp_list_addr)
		return;
	if (ipl_block.hdr.flags & IPL_PL_FLAG_SIPL)
		pr_info("Linux is running with Secure-IPL enabled\n");
	else
		pr_info("Linux is running with Secure-IPL disabled\n");
	ptr = __va(early_ipl_comp_list_addr);
	end = (void *) ptr + early_ipl_comp_list_size;
	pr_info("The IPL report contains the following components:\n");
	while (ptr < end) {
		if (ptr->flags & IPL_RB_COMPONENT_FLAG_SIGNED) {
			if (ptr->flags & IPL_RB_COMPONENT_FLAG_VERIFIED)
				str = "signed, verified";
			else
				str = "signed, verification failed";
		} else {
			str = "not signed";
		}
		pr_info("%016llx - %016llx (%s)\n",
			ptr->addr, ptr->addr + ptr->len, str);
		ptr++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/*
 * Setup function called from init/main.c just after the banner
 * was printed.
 */

void __init setup_arch(char **cmdline_p)
{
        /*
         * print what head.S has found out about the machine
         */
<<<<<<< HEAD
#ifndef CONFIG_64BIT
	if (MACHINE_IS_VM)
		pr_info("Linux is running as a z/VM "
			"guest operating system in 31-bit mode\n");
	else if (MACHINE_IS_LPAR)
		pr_info("Linux is running natively in 31-bit mode\n");
	if (MACHINE_HAS_IEEE)
		pr_info("The hardware system has IEEE compatible "
			"floating point units\n");
	else
		pr_info("The hardware system has no IEEE compatible "
			"floating point units\n");
#else /* CONFIG_64BIT */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (MACHINE_IS_VM)
		pr_info("Linux is running as a z/VM "
			"guest operating system in 64-bit mode\n");
	else if (MACHINE_IS_KVM)
		pr_info("Linux is running under KVM in 64-bit mode\n");
	else if (MACHINE_IS_LPAR)
		pr_info("Linux is running natively in 64-bit mode\n");
<<<<<<< HEAD
#endif /* CONFIG_64BIT */
=======
	else
		pr_info("Linux is running as a guest in 64-bit mode\n");

	log_component_list();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Have one command line that is parsed and saved in /proc/cmdline */
	/* boot_command_line has been already set up in early.c */
	*cmdline_p = boot_command_line;

        ROOT_DEV = Root_RAM0;

<<<<<<< HEAD
	init_mm.start_code = PAGE_OFFSET;
	init_mm.end_code = (unsigned long) &_etext;
	init_mm.end_data = (unsigned long) &_edata;
	init_mm.brk = (unsigned long) &_end;

	if (MACHINE_HAS_MVCOS)
		memcpy(&uaccess, &uaccess_mvcos, sizeof(uaccess));
	else
		memcpy(&uaccess, &uaccess_std, sizeof(uaccess));

	parse_early_param();

	os_info_init();
	setup_ipl();
	setup_memory_end();
	setup_addressing_mode();
	reserve_oldmem();
	reserve_crashkernel();
	setup_memory();
	setup_resources();
	setup_vmcoreinfo();
	setup_lowcore();

        cpu_init();
	s390_init_cpu_topology();

	/*
	 * Setup capabilities (ELF_HWCAP & ELF_PLATFORM).
	 */
	setup_hwcaps();

	/*
	 * Create kernel page tables and switch to virtual addressing.
	 */
        paging_init();

=======
	setup_initial_init_mm(_text, _etext, _edata, _end);

	if (IS_ENABLED(CONFIG_EXPOLINE_AUTO))
		nospec_auto_detect();

	jump_label_init();
	parse_early_param();
#ifdef CONFIG_CRASH_DUMP
	/* Deactivate elfcorehdr= kernel parameter */
	elfcorehdr_addr = ELFCORE_ADDR_MAX;
#endif

	os_info_init();
	setup_ipl();
	setup_control_program_code();

	/* Do some memory reservations *before* memory is added to memblock */
	reserve_pgtables();
	reserve_kernel();
	reserve_initrd();
	reserve_certificate_list();
	reserve_physmem_info();
	memblock_set_current_limit(ident_map_size);
	memblock_allow_resize();

	/* Get information about *all* installed memory */
	memblock_add_physmem_info();

	free_physmem_info();
	setup_memory_end();
	memblock_dump_all();
	setup_memory();

	relocate_amode31_section();
	setup_cr();
	setup_uv();
	dma_contiguous_reserve(ident_map_size);
	vmcp_cma_reserve();
	if (MACHINE_HAS_EDAT2)
		hugetlb_cma_reserve(PUD_SHIFT - PAGE_SHIFT);

	reserve_crashkernel();
#ifdef CONFIG_CRASH_DUMP
	/*
	 * Be aware that smp_save_dump_secondary_cpus() triggers a system reset.
	 * Therefore CPU and device initialization should be done afterwards.
	 */
	smp_save_dump_secondary_cpus();
#endif

	setup_resources();
	setup_lowcore();
	smp_fill_possible_mask();
	cpu_detect_mhz_feature();
        cpu_init();
	numa_setup();
	smp_detect_cpus();
	topology_init_early();

	if (test_facility(193))
		static_branch_enable(&cpu_has_bear);

	/*
	 * Create kernel page tables.
	 */
        paging_init();

	/*
	 * After paging_init created the kernel page table, the new PSWs
	 * in lowcore can now run with DAT enabled.
	 */
#ifdef CONFIG_CRASH_DUMP
	smp_save_dump_ipl_cpu();
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
        /* Setup default console */
	conmode_default();
	set_preferred_console();

<<<<<<< HEAD
	/* Setup zfcpdump support */
	setup_zfcpdump(console_devno);
=======
	apply_alternative_instructions();
	if (IS_ENABLED(CONFIG_EXPOLINE))
		nospec_init_branches();

	/* Setup zfcp/nvme dump support */
	setup_zfcpdump();

	/* Add system specific data to the random pool */
	setup_randomness();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
