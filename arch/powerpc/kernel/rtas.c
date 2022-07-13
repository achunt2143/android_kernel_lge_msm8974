<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *
 * Procedures for interfacing to the RTAS on CHRP machines.
 *
 * Peter Bergner, IBM	March 2001.
 * Copyright (C) 2001 IBM.
<<<<<<< HEAD
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 */

#include <stdarg.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/export.h>
#include <linux/init.h>
#include <linux/capability.h>
#include <linux/delay.h>
#include <linux/cpu.h>
#include <linux/smp.h>
#include <linux/completion.h>
#include <linux/cpumask.h>
#include <linux/memblock.h>
#include <linux/slab.h>
#include <linux/reboot.h>

#include <asm/prom.h>
#include <asm/rtas.h>
#include <asm/hvcall.h>
#include <asm/machdep.h>
#include <asm/firmware.h>
#include <asm/page.h>
#include <asm/param.h>
#include <asm/delay.h>
#include <asm/uaccess.h>
#include <asm/udbg.h>
#include <asm/syscalls.h>
#include <asm/smp.h>
#include <linux/atomic.h>
#include <asm/time.h>
#include <asm/mmu.h>
#include <asm/topology.h>
#include <asm/pSeries_reconfig.h>

struct rtas_t rtas = {
	.lock = __ARCH_SPIN_LOCK_UNLOCKED
};
EXPORT_SYMBOL(rtas);

DEFINE_SPINLOCK(rtas_data_buf_lock);
EXPORT_SYMBOL(rtas_data_buf_lock);

char rtas_data_buf[RTAS_DATA_BUF_SIZE] __cacheline_aligned;
EXPORT_SYMBOL(rtas_data_buf);
=======
 */

#define pr_fmt(fmt)	"rtas: " fmt

#include <linux/bsearch.h>
#include <linux/capability.h>
#include <linux/delay.h>
#include <linux/export.h>
#include <linux/init.h>
#include <linux/kconfig.h>
#include <linux/kernel.h>
#include <linux/lockdep.h>
#include <linux/memblock.h>
#include <linux/mutex.h>
#include <linux/of.h>
#include <linux/of_fdt.h>
#include <linux/reboot.h>
#include <linux/sched.h>
#include <linux/security.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/stdarg.h>
#include <linux/syscalls.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/xarray.h>

#include <asm/delay.h>
#include <asm/firmware.h>
#include <asm/interrupt.h>
#include <asm/machdep.h>
#include <asm/mmu.h>
#include <asm/page.h>
#include <asm/rtas-work-area.h>
#include <asm/rtas.h>
#include <asm/time.h>
#include <asm/trace.h>
#include <asm/udbg.h>

struct rtas_filter {
	/* Indexes into the args buffer, -1 if not used */
	const int buf_idx1;
	const int size_idx1;
	const int buf_idx2;
	const int size_idx2;
	/*
	 * Assumed buffer size per the spec if the function does not
	 * have a size parameter, e.g. ibm,errinjct. 0 if unused.
	 */
	const int fixed_size;
};

/**
 * struct rtas_function - Descriptor for RTAS functions.
 *
 * @token: Value of @name if it exists under the /rtas node.
 * @name: Function name.
 * @filter: If non-NULL, invoking this function via the rtas syscall is
 *          generally allowed, and @filter describes constraints on the
 *          arguments. See also @banned_for_syscall_on_le.
 * @banned_for_syscall_on_le: Set when call via sys_rtas is generally allowed
 *                            but specifically restricted on ppc64le. Such
 *                            functions are believed to have no users on
 *                            ppc64le, and we want to keep it that way. It does
 *                            not make sense for this to be set when @filter
 *                            is NULL.
 * @lock: Pointer to an optional dedicated per-function mutex. This
 *        should be set for functions that require multiple calls in
 *        sequence to complete a single operation, and such sequences
 *        will disrupt each other if allowed to interleave. Users of
 *        this function are required to hold the associated lock for
 *        the duration of the call sequence. Add an explanatory
 *        comment to the function table entry if setting this member.
 */
struct rtas_function {
	s32 token;
	const bool banned_for_syscall_on_le:1;
	const char * const name;
	const struct rtas_filter *filter;
	struct mutex *lock;
};

/*
 * Per-function locks for sequence-based RTAS functions.
 */
static DEFINE_MUTEX(rtas_ibm_activate_firmware_lock);
static DEFINE_MUTEX(rtas_ibm_get_dynamic_sensor_state_lock);
static DEFINE_MUTEX(rtas_ibm_get_indices_lock);
static DEFINE_MUTEX(rtas_ibm_lpar_perftools_lock);
static DEFINE_MUTEX(rtas_ibm_physical_attestation_lock);
static DEFINE_MUTEX(rtas_ibm_set_dynamic_indicator_lock);
DEFINE_MUTEX(rtas_ibm_get_vpd_lock);

static struct rtas_function rtas_function_table[] __ro_after_init = {
	[RTAS_FNIDX__CHECK_EXCEPTION] = {
		.name = "check-exception",
	},
	[RTAS_FNIDX__DISPLAY_CHARACTER] = {
		.name = "display-character",
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = -1, .size_idx1 = -1,
			.buf_idx2 = -1, .size_idx2 = -1,
		},
	},
	[RTAS_FNIDX__EVENT_SCAN] = {
		.name = "event-scan",
	},
	[RTAS_FNIDX__FREEZE_TIME_BASE] = {
		.name = "freeze-time-base",
	},
	[RTAS_FNIDX__GET_POWER_LEVEL] = {
		.name = "get-power-level",
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = -1, .size_idx1 = -1,
			.buf_idx2 = -1, .size_idx2 = -1,
		},
	},
	[RTAS_FNIDX__GET_SENSOR_STATE] = {
		.name = "get-sensor-state",
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = -1, .size_idx1 = -1,
			.buf_idx2 = -1, .size_idx2 = -1,
		},
	},
	[RTAS_FNIDX__GET_TERM_CHAR] = {
		.name = "get-term-char",
	},
	[RTAS_FNIDX__GET_TIME_OF_DAY] = {
		.name = "get-time-of-day",
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = -1, .size_idx1 = -1,
			.buf_idx2 = -1, .size_idx2 = -1,
		},
	},
	[RTAS_FNIDX__IBM_ACTIVATE_FIRMWARE] = {
		.name = "ibm,activate-firmware",
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = -1, .size_idx1 = -1,
			.buf_idx2 = -1, .size_idx2 = -1,
		},
		/*
		 * PAPR+ as of v2.13 doesn't explicitly impose any
		 * restriction, but this typically requires multiple
		 * calls before success, and there's no reason to
		 * allow sequences to interleave.
		 */
		.lock = &rtas_ibm_activate_firmware_lock,
	},
	[RTAS_FNIDX__IBM_CBE_START_PTCAL] = {
		.name = "ibm,cbe-start-ptcal",
	},
	[RTAS_FNIDX__IBM_CBE_STOP_PTCAL] = {
		.name = "ibm,cbe-stop-ptcal",
	},
	[RTAS_FNIDX__IBM_CHANGE_MSI] = {
		.name = "ibm,change-msi",
	},
	[RTAS_FNIDX__IBM_CLOSE_ERRINJCT] = {
		.name = "ibm,close-errinjct",
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = -1, .size_idx1 = -1,
			.buf_idx2 = -1, .size_idx2 = -1,
		},
	},
	[RTAS_FNIDX__IBM_CONFIGURE_BRIDGE] = {
		.name = "ibm,configure-bridge",
	},
	[RTAS_FNIDX__IBM_CONFIGURE_CONNECTOR] = {
		.name = "ibm,configure-connector",
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = 0, .size_idx1 = -1,
			.buf_idx2 = 1, .size_idx2 = -1,
			.fixed_size = 4096,
		},
	},
	[RTAS_FNIDX__IBM_CONFIGURE_KERNEL_DUMP] = {
		.name = "ibm,configure-kernel-dump",
	},
	[RTAS_FNIDX__IBM_CONFIGURE_PE] = {
		.name = "ibm,configure-pe",
	},
	[RTAS_FNIDX__IBM_CREATE_PE_DMA_WINDOW] = {
		.name = "ibm,create-pe-dma-window",
	},
	[RTAS_FNIDX__IBM_DISPLAY_MESSAGE] = {
		.name = "ibm,display-message",
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = 0, .size_idx1 = -1,
			.buf_idx2 = -1, .size_idx2 = -1,
		},
	},
	[RTAS_FNIDX__IBM_ERRINJCT] = {
		.name = "ibm,errinjct",
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = 2, .size_idx1 = -1,
			.buf_idx2 = -1, .size_idx2 = -1,
			.fixed_size = 1024,
		},
	},
	[RTAS_FNIDX__IBM_EXTI2C] = {
		.name = "ibm,exti2c",
	},
	[RTAS_FNIDX__IBM_GET_CONFIG_ADDR_INFO] = {
		.name = "ibm,get-config-addr-info",
	},
	[RTAS_FNIDX__IBM_GET_CONFIG_ADDR_INFO2] = {
		.name = "ibm,get-config-addr-info2",
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = -1, .size_idx1 = -1,
			.buf_idx2 = -1, .size_idx2 = -1,
		},
	},
	[RTAS_FNIDX__IBM_GET_DYNAMIC_SENSOR_STATE] = {
		.name = "ibm,get-dynamic-sensor-state",
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = 1, .size_idx1 = -1,
			.buf_idx2 = -1, .size_idx2 = -1,
		},
		/*
		 * PAPR+ v2.13 R1–7.3.19–3 is explicit that the OS
		 * must not call ibm,get-dynamic-sensor-state with
		 * different inputs until a non-retry status has been
		 * returned.
		 */
		.lock = &rtas_ibm_get_dynamic_sensor_state_lock,
	},
	[RTAS_FNIDX__IBM_GET_INDICES] = {
		.name = "ibm,get-indices",
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = 2, .size_idx1 = 3,
			.buf_idx2 = -1, .size_idx2 = -1,
		},
		/*
		 * PAPR+ v2.13 R1–7.3.17–2 says that the OS must not
		 * interleave ibm,get-indices call sequences with
		 * different inputs.
		 */
		.lock = &rtas_ibm_get_indices_lock,
	},
	[RTAS_FNIDX__IBM_GET_RIO_TOPOLOGY] = {
		.name = "ibm,get-rio-topology",
	},
	[RTAS_FNIDX__IBM_GET_SYSTEM_PARAMETER] = {
		.name = "ibm,get-system-parameter",
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = 1, .size_idx1 = 2,
			.buf_idx2 = -1, .size_idx2 = -1,
		},
	},
	[RTAS_FNIDX__IBM_GET_VPD] = {
		.name = "ibm,get-vpd",
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = 0, .size_idx1 = -1,
			.buf_idx2 = 1, .size_idx2 = 2,
		},
		/*
		 * PAPR+ v2.13 R1–7.3.20–4 indicates that sequences
		 * should not be allowed to interleave.
		 */
		.lock = &rtas_ibm_get_vpd_lock,
	},
	[RTAS_FNIDX__IBM_GET_XIVE] = {
		.name = "ibm,get-xive",
	},
	[RTAS_FNIDX__IBM_INT_OFF] = {
		.name = "ibm,int-off",
	},
	[RTAS_FNIDX__IBM_INT_ON] = {
		.name = "ibm,int-on",
	},
	[RTAS_FNIDX__IBM_IO_QUIESCE_ACK] = {
		.name = "ibm,io-quiesce-ack",
	},
	[RTAS_FNIDX__IBM_LPAR_PERFTOOLS] = {
		.name = "ibm,lpar-perftools",
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = 2, .size_idx1 = 3,
			.buf_idx2 = -1, .size_idx2 = -1,
		},
		/*
		 * PAPR+ v2.13 R1–7.3.26–6 says the OS should allow
		 * only one call sequence in progress at a time.
		 */
		.lock = &rtas_ibm_lpar_perftools_lock,
	},
	[RTAS_FNIDX__IBM_MANAGE_FLASH_IMAGE] = {
		.name = "ibm,manage-flash-image",
	},
	[RTAS_FNIDX__IBM_MANAGE_STORAGE_PRESERVATION] = {
		.name = "ibm,manage-storage-preservation",
	},
	[RTAS_FNIDX__IBM_NMI_INTERLOCK] = {
		.name = "ibm,nmi-interlock",
	},
	[RTAS_FNIDX__IBM_NMI_REGISTER] = {
		.name = "ibm,nmi-register",
	},
	[RTAS_FNIDX__IBM_OPEN_ERRINJCT] = {
		.name = "ibm,open-errinjct",
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = -1, .size_idx1 = -1,
			.buf_idx2 = -1, .size_idx2 = -1,
		},
	},
	[RTAS_FNIDX__IBM_OPEN_SRIOV_ALLOW_UNFREEZE] = {
		.name = "ibm,open-sriov-allow-unfreeze",
	},
	[RTAS_FNIDX__IBM_OPEN_SRIOV_MAP_PE_NUMBER] = {
		.name = "ibm,open-sriov-map-pe-number",
	},
	[RTAS_FNIDX__IBM_OS_TERM] = {
		.name = "ibm,os-term",
	},
	[RTAS_FNIDX__IBM_PARTNER_CONTROL] = {
		.name = "ibm,partner-control",
	},
	[RTAS_FNIDX__IBM_PHYSICAL_ATTESTATION] = {
		.name = "ibm,physical-attestation",
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = 0, .size_idx1 = 1,
			.buf_idx2 = -1, .size_idx2 = -1,
		},
		/*
		 * This follows a sequence-based pattern similar to
		 * ibm,get-vpd et al. Since PAPR+ restricts
		 * interleaving call sequences for other functions of
		 * this style, assume the restriction applies here,
		 * even though it's not explicit in the spec.
		 */
		.lock = &rtas_ibm_physical_attestation_lock,
	},
	[RTAS_FNIDX__IBM_PLATFORM_DUMP] = {
		.name = "ibm,platform-dump",
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = 4, .size_idx1 = 5,
			.buf_idx2 = -1, .size_idx2 = -1,
		},
		/*
		 * PAPR+ v2.13 7.3.3.4.1 indicates that concurrent
		 * sequences of ibm,platform-dump are allowed if they
		 * are operating on different dump tags. So leave the
		 * lock pointer unset for now. This may need
		 * reconsideration if kernel-internal users appear.
		 */
	},
	[RTAS_FNIDX__IBM_POWER_OFF_UPS] = {
		.name = "ibm,power-off-ups",
	},
	[RTAS_FNIDX__IBM_QUERY_INTERRUPT_SOURCE_NUMBER] = {
		.name = "ibm,query-interrupt-source-number",
	},
	[RTAS_FNIDX__IBM_QUERY_PE_DMA_WINDOW] = {
		.name = "ibm,query-pe-dma-window",
	},
	[RTAS_FNIDX__IBM_READ_PCI_CONFIG] = {
		.name = "ibm,read-pci-config",
	},
	[RTAS_FNIDX__IBM_READ_SLOT_RESET_STATE] = {
		.name = "ibm,read-slot-reset-state",
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = -1, .size_idx1 = -1,
			.buf_idx2 = -1, .size_idx2 = -1,
		},
	},
	[RTAS_FNIDX__IBM_READ_SLOT_RESET_STATE2] = {
		.name = "ibm,read-slot-reset-state2",
	},
	[RTAS_FNIDX__IBM_REMOVE_PE_DMA_WINDOW] = {
		.name = "ibm,remove-pe-dma-window",
	},
	[RTAS_FNIDX__IBM_RESET_PE_DMA_WINDOW] = {
		/*
		 * Note: PAPR+ v2.13 7.3.31.4.1 spells this as
		 * "ibm,reset-pe-dma-windows" (plural), but RTAS
		 * implementations use the singular form in practice.
		 */
		.name = "ibm,reset-pe-dma-window",
	},
	[RTAS_FNIDX__IBM_SCAN_LOG_DUMP] = {
		.name = "ibm,scan-log-dump",
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = 0, .size_idx1 = 1,
			.buf_idx2 = -1, .size_idx2 = -1,
		},
	},
	[RTAS_FNIDX__IBM_SET_DYNAMIC_INDICATOR] = {
		.name = "ibm,set-dynamic-indicator",
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = 2, .size_idx1 = -1,
			.buf_idx2 = -1, .size_idx2 = -1,
		},
		/*
		 * PAPR+ v2.13 R1–7.3.18–3 says the OS must not call
		 * this function with different inputs until a
		 * non-retry status has been returned.
		 */
		.lock = &rtas_ibm_set_dynamic_indicator_lock,
	},
	[RTAS_FNIDX__IBM_SET_EEH_OPTION] = {
		.name = "ibm,set-eeh-option",
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = -1, .size_idx1 = -1,
			.buf_idx2 = -1, .size_idx2 = -1,
		},
	},
	[RTAS_FNIDX__IBM_SET_SLOT_RESET] = {
		.name = "ibm,set-slot-reset",
	},
	[RTAS_FNIDX__IBM_SET_SYSTEM_PARAMETER] = {
		.name = "ibm,set-system-parameter",
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = 1, .size_idx1 = -1,
			.buf_idx2 = -1, .size_idx2 = -1,
		},
	},
	[RTAS_FNIDX__IBM_SET_XIVE] = {
		.name = "ibm,set-xive",
	},
	[RTAS_FNIDX__IBM_SLOT_ERROR_DETAIL] = {
		.name = "ibm,slot-error-detail",
	},
	[RTAS_FNIDX__IBM_SUSPEND_ME] = {
		.name = "ibm,suspend-me",
		.banned_for_syscall_on_le = true,
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = -1, .size_idx1 = -1,
			.buf_idx2 = -1, .size_idx2 = -1,
		},
	},
	[RTAS_FNIDX__IBM_TUNE_DMA_PARMS] = {
		.name = "ibm,tune-dma-parms",
	},
	[RTAS_FNIDX__IBM_UPDATE_FLASH_64_AND_REBOOT] = {
		.name = "ibm,update-flash-64-and-reboot",
	},
	[RTAS_FNIDX__IBM_UPDATE_NODES] = {
		.name = "ibm,update-nodes",
		.banned_for_syscall_on_le = true,
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = 0, .size_idx1 = -1,
			.buf_idx2 = -1, .size_idx2 = -1,
			.fixed_size = 4096,
		},
	},
	[RTAS_FNIDX__IBM_UPDATE_PROPERTIES] = {
		.name = "ibm,update-properties",
		.banned_for_syscall_on_le = true,
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = 0, .size_idx1 = -1,
			.buf_idx2 = -1, .size_idx2 = -1,
			.fixed_size = 4096,
		},
	},
	[RTAS_FNIDX__IBM_VALIDATE_FLASH_IMAGE] = {
		.name = "ibm,validate-flash-image",
	},
	[RTAS_FNIDX__IBM_WRITE_PCI_CONFIG] = {
		.name = "ibm,write-pci-config",
	},
	[RTAS_FNIDX__NVRAM_FETCH] = {
		.name = "nvram-fetch",
	},
	[RTAS_FNIDX__NVRAM_STORE] = {
		.name = "nvram-store",
	},
	[RTAS_FNIDX__POWER_OFF] = {
		.name = "power-off",
	},
	[RTAS_FNIDX__PUT_TERM_CHAR] = {
		.name = "put-term-char",
	},
	[RTAS_FNIDX__QUERY_CPU_STOPPED_STATE] = {
		.name = "query-cpu-stopped-state",
	},
	[RTAS_FNIDX__READ_PCI_CONFIG] = {
		.name = "read-pci-config",
	},
	[RTAS_FNIDX__RTAS_LAST_ERROR] = {
		.name = "rtas-last-error",
	},
	[RTAS_FNIDX__SET_INDICATOR] = {
		.name = "set-indicator",
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = -1, .size_idx1 = -1,
			.buf_idx2 = -1, .size_idx2 = -1,
		},
	},
	[RTAS_FNIDX__SET_POWER_LEVEL] = {
		.name = "set-power-level",
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = -1, .size_idx1 = -1,
			.buf_idx2 = -1, .size_idx2 = -1,
		},
	},
	[RTAS_FNIDX__SET_TIME_FOR_POWER_ON] = {
		.name = "set-time-for-power-on",
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = -1, .size_idx1 = -1,
			.buf_idx2 = -1, .size_idx2 = -1,
		},
	},
	[RTAS_FNIDX__SET_TIME_OF_DAY] = {
		.name = "set-time-of-day",
		.filter = &(const struct rtas_filter) {
			.buf_idx1 = -1, .size_idx1 = -1,
			.buf_idx2 = -1, .size_idx2 = -1,
		},
	},
	[RTAS_FNIDX__START_CPU] = {
		.name = "start-cpu",
	},
	[RTAS_FNIDX__STOP_SELF] = {
		.name = "stop-self",
	},
	[RTAS_FNIDX__SYSTEM_REBOOT] = {
		.name = "system-reboot",
	},
	[RTAS_FNIDX__THAW_TIME_BASE] = {
		.name = "thaw-time-base",
	},
	[RTAS_FNIDX__WRITE_PCI_CONFIG] = {
		.name = "write-pci-config",
	},
};

#define for_each_rtas_function(funcp)                                       \
	for (funcp = &rtas_function_table[0];                               \
	     funcp < &rtas_function_table[ARRAY_SIZE(rtas_function_table)]; \
	     ++funcp)

/*
 * Nearly all RTAS calls need to be serialized. All uses of the
 * default rtas_args block must hold rtas_lock.
 *
 * Exceptions to the RTAS serialization requirement (e.g. stop-self)
 * must use a separate rtas_args structure.
 */
static DEFINE_RAW_SPINLOCK(rtas_lock);
static struct rtas_args rtas_args;

/**
 * rtas_function_token() - RTAS function token lookup.
 * @handle: Function handle, e.g. RTAS_FN_EVENT_SCAN.
 *
 * Context: Any context.
 * Return: the token value for the function if implemented by this platform,
 *         otherwise RTAS_UNKNOWN_SERVICE.
 */
s32 rtas_function_token(const rtas_fn_handle_t handle)
{
	const size_t index = handle.index;
	const bool out_of_bounds = index >= ARRAY_SIZE(rtas_function_table);

	if (WARN_ONCE(out_of_bounds, "invalid function index %zu", index))
		return RTAS_UNKNOWN_SERVICE;
	/*
	 * Various drivers attempt token lookups on non-RTAS
	 * platforms.
	 */
	if (!rtas.dev)
		return RTAS_UNKNOWN_SERVICE;

	return rtas_function_table[index].token;
}
EXPORT_SYMBOL_GPL(rtas_function_token);

static int rtas_function_cmp(const void *a, const void *b)
{
	const struct rtas_function *f1 = a;
	const struct rtas_function *f2 = b;

	return strcmp(f1->name, f2->name);
}

/*
 * Boot-time initialization of the function table needs the lookup to
 * return a non-const-qualified object. Use rtas_name_to_function()
 * in all other contexts.
 */
static struct rtas_function *__rtas_name_to_function(const char *name)
{
	const struct rtas_function key = {
		.name = name,
	};
	struct rtas_function *found;

	found = bsearch(&key, rtas_function_table, ARRAY_SIZE(rtas_function_table),
			sizeof(rtas_function_table[0]), rtas_function_cmp);

	return found;
}

static const struct rtas_function *rtas_name_to_function(const char *name)
{
	return __rtas_name_to_function(name);
}

static DEFINE_XARRAY(rtas_token_to_function_xarray);

static int __init rtas_token_to_function_xarray_init(void)
{
	const struct rtas_function *func;
	int err = 0;

	for_each_rtas_function(func) {
		const s32 token = func->token;

		if (token == RTAS_UNKNOWN_SERVICE)
			continue;

		err = xa_err(xa_store(&rtas_token_to_function_xarray,
				      token, (void *)func, GFP_KERNEL));
		if (err)
			break;
	}

	return err;
}
arch_initcall(rtas_token_to_function_xarray_init);

/*
 * For use by sys_rtas(), where the token value is provided by user
 * space and we don't want to warn on failed lookups.
 */
static const struct rtas_function *rtas_token_to_function_untrusted(s32 token)
{
	return xa_load(&rtas_token_to_function_xarray, token);
}

/*
 * Reverse lookup for deriving the function descriptor from a
 * known-good token value in contexts where the former is not already
 * available. @token must be valid, e.g. derived from the result of a
 * prior lookup against the function table.
 */
static const struct rtas_function *rtas_token_to_function(s32 token)
{
	const struct rtas_function *func;

	if (WARN_ONCE(token < 0, "invalid token %d", token))
		return NULL;

	func = rtas_token_to_function_untrusted(token);
	if (func)
		return func;
	/*
	 * Fall back to linear scan in case the reverse mapping hasn't
	 * been initialized yet.
	 */
	if (xa_empty(&rtas_token_to_function_xarray)) {
		for_each_rtas_function(func) {
			if (func->token == token)
				return func;
		}
	}

	WARN_ONCE(true, "unexpected failed lookup for token %d", token);
	return NULL;
}

/* This is here deliberately so it's only used in this file */
void enter_rtas(unsigned long);

static void __do_enter_rtas(struct rtas_args *args)
{
	enter_rtas(__pa(args));
	srr_regs_clobbered(); /* rtas uses SRRs, invalidate */
}

static void __do_enter_rtas_trace(struct rtas_args *args)
{
	const struct rtas_function *func = rtas_token_to_function(be32_to_cpu(args->token));

	/*
	 * If there is a per-function lock, it must be held by the
	 * caller.
	 */
	if (func->lock)
		lockdep_assert_held(func->lock);

	if (args == &rtas_args)
		lockdep_assert_held(&rtas_lock);

	trace_rtas_input(args, func->name);
	trace_rtas_ll_entry(args);

	__do_enter_rtas(args);

	trace_rtas_ll_exit(args);
	trace_rtas_output(args, func->name);
}

static void do_enter_rtas(struct rtas_args *args)
{
	const unsigned long msr = mfmsr();
	/*
	 * Situations where we want to skip any active tracepoints for
	 * safety reasons:
	 *
	 * 1. The last code executed on an offline CPU as it stops,
	 *    i.e. we're about to call stop-self. The tracepoints'
	 *    function name lookup uses xarray, which uses RCU, which
	 *    isn't valid to call on an offline CPU.  Any events
	 *    emitted on an offline CPU will be discarded anyway.
	 *
	 * 2. In real mode, as when invoking ibm,nmi-interlock from
	 *    the pseries MCE handler. We cannot count on trace
	 *    buffers or the entries in rtas_token_to_function_xarray
	 *    to be contained in the RMO.
	 */
	const unsigned long mask = MSR_IR | MSR_DR;
	const bool can_trace = likely(cpu_online(raw_smp_processor_id()) &&
				      (msr & mask) == mask);
	/*
	 * Make sure MSR[RI] is currently enabled as it will be forced later
	 * in enter_rtas.
	 */
	BUG_ON(!(msr & MSR_RI));

	BUG_ON(!irqs_disabled());

	hard_irq_disable(); /* Ensure MSR[EE] is disabled on PPC64 */

	if (can_trace)
		__do_enter_rtas_trace(args);
	else
		__do_enter_rtas(args);
}

struct rtas_t rtas;

DEFINE_SPINLOCK(rtas_data_buf_lock);
EXPORT_SYMBOL_GPL(rtas_data_buf_lock);

char rtas_data_buf[RTAS_DATA_BUF_SIZE] __aligned(SZ_4K);
EXPORT_SYMBOL_GPL(rtas_data_buf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

unsigned long rtas_rmo_buf;

/*
 * If non-NULL, this gets called when the kernel terminates.
 * This is done like this so rtas_flash can be a module.
 */
void (*rtas_flash_term_hook)(int);
<<<<<<< HEAD
EXPORT_SYMBOL(rtas_flash_term_hook);

/* RTAS use home made raw locking instead of spin_lock_irqsave
 * because those can be called from within really nasty contexts
 * such as having the timebase stopped which would lockup with
 * normal locks and spinlock debugging enabled
 */
static unsigned long lock_rtas(void)
{
	unsigned long flags;

	local_irq_save(flags);
	preempt_disable();
	arch_spin_lock_flags(&rtas.lock, flags);
	return flags;
}

static void unlock_rtas(unsigned long flags)
{
	arch_spin_unlock(&rtas.lock);
	local_irq_restore(flags);
	preempt_enable();
}
=======
EXPORT_SYMBOL_GPL(rtas_flash_term_hook);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * call_rtas_display_status and call_rtas_display_status_delay
 * are designed only for very early low-level debugging, which
 * is why the token is hard-coded to 10.
 */
<<<<<<< HEAD
static void call_rtas_display_status(char c)
{
	struct rtas_args *args = &rtas.args;
	unsigned long s;

	if (!rtas.base)
		return;
	s = lock_rtas();

	args->token = 10;
	args->nargs = 1;
	args->nret  = 1;
	args->rets  = (rtas_arg_t *)&(args->args[1]);
	args->args[0] = (unsigned char)c;

	enter_rtas(__pa(args));

	unlock_rtas(s);
=======
static void call_rtas_display_status(unsigned char c)
{
	unsigned long flags;

	if (!rtas.base)
		return;

	raw_spin_lock_irqsave(&rtas_lock, flags);
	rtas_call_unlocked(&rtas_args, 10, 1, 1, NULL, c);
	raw_spin_unlock_irqrestore(&rtas_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void call_rtas_display_status_delay(char c)
{
	static int pending_newline = 0;  /* did last write end with unprinted newline? */
	static int width = 16;

<<<<<<< HEAD
	if (c == '\n') {	
=======
	if (c == '\n') {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		while (width-- > 0)
			call_rtas_display_status(' ');
		width = 16;
		mdelay(500);
		pending_newline = 1;
	} else {
		if (pending_newline) {
			call_rtas_display_status('\r');
			call_rtas_display_status('\n');
<<<<<<< HEAD
		} 
=======
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		pending_newline = 0;
		if (width--) {
			call_rtas_display_status(c);
			udelay(10000);
		}
	}
}

void __init udbg_init_rtas_panel(void)
{
	udbg_putc = call_rtas_display_status_delay;
}

#ifdef CONFIG_UDBG_RTAS_CONSOLE

/* If you think you're dying before early_init_dt_scan_rtas() does its
 * work, you can hard code the token values for your firmware here and
 * hardcode rtas.base/entry etc.
 */
static unsigned int rtas_putchar_token = RTAS_UNKNOWN_SERVICE;
static unsigned int rtas_getchar_token = RTAS_UNKNOWN_SERVICE;

static void udbg_rtascon_putc(char c)
{
	int tries;

	if (!rtas.base)
		return;

	/* Add CRs before LFs */
	if (c == '\n')
		udbg_rtascon_putc('\r');

	/* if there is more than one character to be displayed, wait a bit */
	for (tries = 0; tries < 16; tries++) {
		if (rtas_call(rtas_putchar_token, 1, 1, NULL, c) == 0)
			break;
		udelay(1000);
	}
}

static int udbg_rtascon_getc_poll(void)
{
	int c;

	if (!rtas.base)
		return -1;

	if (rtas_call(rtas_getchar_token, 0, 2, &c))
		return -1;

	return c;
}

static int udbg_rtascon_getc(void)
{
	int c;

	while ((c = udbg_rtascon_getc_poll()) == -1)
		;

	return c;
}


void __init udbg_init_rtas_console(void)
{
	udbg_putc = udbg_rtascon_putc;
	udbg_getc = udbg_rtascon_getc;
	udbg_getc_poll = udbg_rtascon_getc_poll;
}
#endif /* CONFIG_UDBG_RTAS_CONSOLE */

void rtas_progress(char *s, unsigned short hex)
{
	struct device_node *root;
	int width;
<<<<<<< HEAD
	const int *p;
=======
	const __be32 *p;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char *os;
	static int display_character, set_indicator;
	static int display_width, display_lines, form_feed;
	static const int *row_width;
	static DEFINE_SPINLOCK(progress_lock);
	static int current_line;
	static int pending_newline = 0;  /* did last write end with unprinted newline? */

	if (!rtas.base)
		return;

	if (display_width == 0) {
		display_width = 0x10;
		if ((root = of_find_node_by_path("/rtas"))) {
			if ((p = of_get_property(root,
					"ibm,display-line-length", NULL)))
<<<<<<< HEAD
				display_width = *p;
			if ((p = of_get_property(root,
					"ibm,form-feed", NULL)))
				form_feed = *p;
			if ((p = of_get_property(root,
					"ibm,display-number-of-lines", NULL)))
				display_lines = *p;
=======
				display_width = be32_to_cpu(*p);
			if ((p = of_get_property(root,
					"ibm,form-feed", NULL)))
				form_feed = be32_to_cpu(*p);
			if ((p = of_get_property(root,
					"ibm,display-number-of-lines", NULL)))
				display_lines = be32_to_cpu(*p);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			row_width = of_get_property(root,
					"ibm,display-truncation-length", NULL);
			of_node_put(root);
		}
<<<<<<< HEAD
		display_character = rtas_token("display-character");
		set_indicator = rtas_token("set-indicator");
=======
		display_character = rtas_function_token(RTAS_FN_DISPLAY_CHARACTER);
		set_indicator = rtas_function_token(RTAS_FN_SET_INDICATOR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (display_character == RTAS_UNKNOWN_SERVICE) {
		/* use hex display if available */
		if (set_indicator != RTAS_UNKNOWN_SERVICE)
			rtas_call(set_indicator, 3, 1, NULL, 6, 0, hex);
		return;
	}

	spin_lock(&progress_lock);

	/*
	 * Last write ended with newline, but we didn't print it since
	 * it would just clear the bottom line of output. Print it now
	 * instead.
	 *
	 * If no newline is pending and form feed is supported, clear the
	 * display with a form feed; otherwise, print a CR to start output
	 * at the beginning of the line.
	 */
	if (pending_newline) {
		rtas_call(display_character, 1, 1, NULL, '\r');
		rtas_call(display_character, 1, 1, NULL, '\n');
		pending_newline = 0;
	} else {
		current_line = 0;
		if (form_feed)
			rtas_call(display_character, 1, 1, NULL,
				  (char)form_feed);
		else
			rtas_call(display_character, 1, 1, NULL, '\r');
	}
<<<<<<< HEAD
 
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (row_width)
		width = row_width[current_line];
	else
		width = display_width;
	os = s;
	while (*os) {
		if (*os == '\n' || *os == '\r') {
			/* If newline is the last character, save it
			 * until next call to avoid bumping up the
			 * display output.
			 */
			if (*os == '\n' && !os[1]) {
				pending_newline = 1;
				current_line++;
				if (current_line > display_lines-1)
					current_line = display_lines-1;
				spin_unlock(&progress_lock);
				return;
			}
<<<<<<< HEAD
 
			/* RTAS wants CR-LF, not just LF */
 
=======

			/* RTAS wants CR-LF, not just LF */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (*os == '\n') {
				rtas_call(display_character, 1, 1, NULL, '\r');
				rtas_call(display_character, 1, 1, NULL, '\n');
			} else {
				/* CR might be used to re-draw a line, so we'll
				 * leave it alone and not add LF.
				 */
				rtas_call(display_character, 1, 1, NULL, *os);
			}
<<<<<<< HEAD
 
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (row_width)
				width = row_width[current_line];
			else
				width = display_width;
		} else {
			width--;
			rtas_call(display_character, 1, 1, NULL, *os);
		}
<<<<<<< HEAD
 
		os++;
 
=======

		os++;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* if we overwrite the screen length */
		if (width <= 0)
			while ((*os != 0) && (*os != '\n') && (*os != '\r'))
				os++;
	}
<<<<<<< HEAD
 
	spin_unlock(&progress_lock);
}
EXPORT_SYMBOL(rtas_progress);		/* needed by rtas_flash module */

int rtas_token(const char *service)
{
	const int *tokp;
	if (rtas.dev == NULL)
		return RTAS_UNKNOWN_SERVICE;
	tokp = of_get_property(rtas.dev, service, NULL);
	return tokp ? *tokp : RTAS_UNKNOWN_SERVICE;
}
EXPORT_SYMBOL(rtas_token);

int rtas_service_present(const char *service)
{
	return rtas_token(service) != RTAS_UNKNOWN_SERVICE;
}
EXPORT_SYMBOL(rtas_service_present);

#ifdef CONFIG_RTAS_ERROR_LOGGING
=======

	spin_unlock(&progress_lock);
}
EXPORT_SYMBOL_GPL(rtas_progress);		/* needed by rtas_flash module */

int rtas_token(const char *service)
{
	const struct rtas_function *func;
	const __be32 *tokp;

	if (rtas.dev == NULL)
		return RTAS_UNKNOWN_SERVICE;

	func = rtas_name_to_function(service);
	if (func)
		return func->token;
	/*
	 * The caller is looking up a name that is not known to be an
	 * RTAS function. Either it's a function that needs to be
	 * added to the table, or they're misusing rtas_token() to
	 * access non-function properties of the /rtas node. Warn and
	 * fall back to the legacy behavior.
	 */
	WARN_ONCE(1, "unknown function `%s`, should it be added to rtas_function_table?\n",
		  service);

	tokp = of_get_property(rtas.dev, service, NULL);
	return tokp ? be32_to_cpu(*tokp) : RTAS_UNKNOWN_SERVICE;
}
EXPORT_SYMBOL_GPL(rtas_token);

#ifdef CONFIG_RTAS_ERROR_LOGGING

static u32 rtas_error_log_max __ro_after_init = RTAS_ERROR_LOG_MAX;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Return the firmware-specified size of the error log buffer
 *  for all rtas calls that require an error buffer argument.
 *  This includes 'check-exception' and 'rtas-last-error'.
 */
int rtas_get_error_log_max(void)
{
<<<<<<< HEAD
	static int rtas_error_log_max;
	if (rtas_error_log_max)
		return rtas_error_log_max;

	rtas_error_log_max = rtas_token ("rtas-error-log-max");
	if ((rtas_error_log_max == RTAS_UNKNOWN_SERVICE) ||
	    (rtas_error_log_max > RTAS_ERROR_LOG_MAX)) {
		printk (KERN_WARNING "RTAS: bad log buffer size %d\n",
			rtas_error_log_max);
		rtas_error_log_max = RTAS_ERROR_LOG_MAX;
	}
	return rtas_error_log_max;
}
EXPORT_SYMBOL(rtas_get_error_log_max);


static char rtas_err_buf[RTAS_ERROR_LOG_MAX];
static int rtas_last_error_token;
=======
	return rtas_error_log_max;
}

static void __init init_error_log_max(void)
{
	static const char propname[] __initconst = "rtas-error-log-max";
	u32 max;

	if (of_property_read_u32(rtas.dev, propname, &max)) {
		pr_warn("%s not found, using default of %u\n",
			propname, RTAS_ERROR_LOG_MAX);
		max = RTAS_ERROR_LOG_MAX;
	}

	if (max > RTAS_ERROR_LOG_MAX) {
		pr_warn("%s = %u, clamping max error log size to %u\n",
			propname, max, RTAS_ERROR_LOG_MAX);
		max = RTAS_ERROR_LOG_MAX;
	}

	rtas_error_log_max = max;
}


static char rtas_err_buf[RTAS_ERROR_LOG_MAX];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/** Return a copy of the detailed error text associated with the
 *  most recent failed call to rtas.  Because the error text
 *  might go stale if there are any other intervening rtas calls,
 *  this routine must be called atomically with whatever produced
<<<<<<< HEAD
 *  the error (i.e. with rtas.lock still held from the previous call).
 */
static char *__fetch_rtas_last_error(char *altbuf)
{
=======
 *  the error (i.e. with rtas_lock still held from the previous call).
 */
static char *__fetch_rtas_last_error(char *altbuf)
{
	const s32 token = rtas_function_token(RTAS_FN_RTAS_LAST_ERROR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct rtas_args err_args, save_args;
	u32 bufsz;
	char *buf = NULL;

<<<<<<< HEAD
	if (rtas_last_error_token == -1)
=======
	lockdep_assert_held(&rtas_lock);

	if (token == -1)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return NULL;

	bufsz = rtas_get_error_log_max();

<<<<<<< HEAD
	err_args.token = rtas_last_error_token;
	err_args.nargs = 2;
	err_args.nret = 1;
	err_args.args[0] = (rtas_arg_t)__pa(rtas_err_buf);
	err_args.args[1] = bufsz;
	err_args.args[2] = 0;

	save_args = rtas.args;
	rtas.args = err_args;

	enter_rtas(__pa(&rtas.args));

	err_args = rtas.args;
	rtas.args = save_args;
=======
	err_args.token = cpu_to_be32(token);
	err_args.nargs = cpu_to_be32(2);
	err_args.nret = cpu_to_be32(1);
	err_args.args[0] = cpu_to_be32(__pa(rtas_err_buf));
	err_args.args[1] = cpu_to_be32(bufsz);
	err_args.args[2] = 0;

	save_args = rtas_args;
	rtas_args = err_args;

	do_enter_rtas(&rtas_args);

	err_args = rtas_args;
	rtas_args = save_args;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Log the error in the unlikely case that there was one. */
	if (unlikely(err_args.args[2] == 0)) {
		if (altbuf) {
			buf = altbuf;
		} else {
			buf = rtas_err_buf;
<<<<<<< HEAD
			if (mem_init_done)
				buf = kmalloc(RTAS_ERROR_LOG_MAX, GFP_ATOMIC);
		}
		if (buf)
			memcpy(buf, rtas_err_buf, RTAS_ERROR_LOG_MAX);
=======
			if (slab_is_available())
				buf = kmalloc(RTAS_ERROR_LOG_MAX, GFP_ATOMIC);
		}
		if (buf)
			memmove(buf, rtas_err_buf, RTAS_ERROR_LOG_MAX);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return buf;
}

#define get_errorlog_buffer()	kmalloc(RTAS_ERROR_LOG_MAX, GFP_KERNEL)

#else /* CONFIG_RTAS_ERROR_LOGGING */
#define __fetch_rtas_last_error(x)	NULL
#define get_errorlog_buffer()		NULL
<<<<<<< HEAD
#endif

int rtas_call(int token, int nargs, int nret, int *outputs, ...)
{
	va_list list;
	int i;
	unsigned long s;
	struct rtas_args *rtas_args;
=======
static void __init init_error_log_max(void) {}
#endif


static void
va_rtas_call_unlocked(struct rtas_args *args, int token, int nargs, int nret,
		      va_list list)
{
	int i;

	args->token = cpu_to_be32(token);
	args->nargs = cpu_to_be32(nargs);
	args->nret  = cpu_to_be32(nret);
	args->rets  = &(args->args[nargs]);

	for (i = 0; i < nargs; ++i)
		args->args[i] = cpu_to_be32(va_arg(list, __u32));

	for (i = 0; i < nret; ++i)
		args->rets[i] = 0;

	do_enter_rtas(args);
}

/**
 * rtas_call_unlocked() - Invoke an RTAS firmware function without synchronization.
 * @args: RTAS parameter block to be used for the call, must obey RTAS addressing
 *        constraints.
 * @token: Identifies the function being invoked.
 * @nargs: Number of input parameters. Does not include token.
 * @nret: Number of output parameters, including the call status.
 * @....: List of @nargs input parameters.
 *
 * Invokes the RTAS function indicated by @token, which the caller
 * should obtain via rtas_function_token().
 *
 * This function is similar to rtas_call(), but must be used with a
 * limited set of RTAS calls specifically exempted from the general
 * requirement that only one RTAS call may be in progress at any
 * time. Examples include stop-self and ibm,nmi-interlock.
 */
void rtas_call_unlocked(struct rtas_args *args, int token, int nargs, int nret, ...)
{
	va_list list;

	va_start(list, nret);
	va_rtas_call_unlocked(args, token, nargs, nret, list);
	va_end(list);
}

static bool token_is_restricted_errinjct(s32 token)
{
	return token == rtas_function_token(RTAS_FN_IBM_OPEN_ERRINJCT) ||
	       token == rtas_function_token(RTAS_FN_IBM_ERRINJCT);
}

/**
 * rtas_call() - Invoke an RTAS firmware function.
 * @token: Identifies the function being invoked.
 * @nargs: Number of input parameters. Does not include token.
 * @nret: Number of output parameters, including the call status.
 * @outputs: Array of @nret output words.
 * @....: List of @nargs input parameters.
 *
 * Invokes the RTAS function indicated by @token, which the caller
 * should obtain via rtas_function_token().
 *
 * The @nargs and @nret arguments must match the number of input and
 * output parameters specified for the RTAS function.
 *
 * rtas_call() returns RTAS status codes, not conventional Linux errno
 * values. Callers must translate any failure to an appropriate errno
 * in syscall context. Most callers of RTAS functions that can return
 * -2 or 990x should use rtas_busy_delay() to correctly handle those
 * statuses before calling again.
 *
 * The return value descriptions are adapted from 7.2.8 [RTAS] Return
 * Codes of the PAPR and CHRP specifications.
 *
 * Context: Process context preferably, interrupt context if
 *          necessary.  Acquires an internal spinlock and may perform
 *          GFP_ATOMIC slab allocation in error path. Unsafe for NMI
 *          context.
 * Return:
 * *                          0 - RTAS function call succeeded.
 * *                         -1 - RTAS function encountered a hardware or
 *                                platform error, or the token is invalid,
 *                                or the function is restricted by kernel policy.
 * *                         -2 - Specs say "A necessary hardware device was busy,
 *                                and the requested function could not be
 *                                performed. The operation should be retried at
 *                                a later time." This is misleading, at least with
 *                                respect to current RTAS implementations. What it
 *                                usually means in practice is that the function
 *                                could not be completed while meeting RTAS's
 *                                deadline for returning control to the OS (250us
 *                                for PAPR/PowerVM, typically), but the call may be
 *                                immediately reattempted to resume work on it.
 * *                         -3 - Parameter error.
 * *                         -7 - Unexpected state change.
 * *                9000...9899 - Vendor-specific success codes.
 * *                9900...9905 - Advisory extended delay. Caller should try
 *                                again after ~10^x ms has elapsed, where x is
 *                                the last digit of the status [0-5]. Again going
 *                                beyond the PAPR text, 990x on PowerVM indicates
 *                                contention for RTAS-internal resources. Other
 *                                RTAS call sequences in progress should be
 *                                allowed to complete before reattempting the
 *                                call.
 * *                      -9000 - Multi-level isolation error.
 * *              -9999...-9004 - Vendor-specific error codes.
 * * Additional negative values - Function-specific error.
 * * Additional positive values - Function-specific success.
 */
int rtas_call(int token, int nargs, int nret, int *outputs, ...)
{
	struct pin_cookie cookie;
	va_list list;
	int i;
	unsigned long flags;
	struct rtas_args *args;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char *buff_copy = NULL;
	int ret;

	if (!rtas.entry || token == RTAS_UNKNOWN_SERVICE)
		return -1;

<<<<<<< HEAD
	s = lock_rtas();
	rtas_args = &rtas.args;

	rtas_args->token = token;
	rtas_args->nargs = nargs;
	rtas_args->nret  = nret;
	rtas_args->rets  = (rtas_arg_t *)&(rtas_args->args[nargs]);
	va_start(list, outputs);
	for (i = 0; i < nargs; ++i)
		rtas_args->args[i] = va_arg(list, rtas_arg_t);
	va_end(list);

	for (i = 0; i < nret; ++i)
		rtas_args->rets[i] = 0;

	enter_rtas(__pa(rtas_args));

	/* A -1 return code indicates that the last command couldn't
	   be completed due to a hardware error. */
	if (rtas_args->rets[0] == -1)
=======
	if (token_is_restricted_errinjct(token)) {
		/*
		 * It would be nicer to not discard the error value
		 * from security_locked_down(), but callers expect an
		 * RTAS status, not an errno.
		 */
		if (security_locked_down(LOCKDOWN_RTAS_ERROR_INJECTION))
			return -1;
	}

	if ((mfmsr() & (MSR_IR|MSR_DR)) != (MSR_IR|MSR_DR)) {
		WARN_ON_ONCE(1);
		return -1;
	}

	raw_spin_lock_irqsave(&rtas_lock, flags);
	cookie = lockdep_pin_lock(&rtas_lock);

	/* We use the global rtas args buffer */
	args = &rtas_args;

	va_start(list, outputs);
	va_rtas_call_unlocked(args, token, nargs, nret, list);
	va_end(list);

	/* A -1 return code indicates that the last command couldn't
	   be completed due to a hardware error. */
	if (be32_to_cpu(args->rets[0]) == -1)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		buff_copy = __fetch_rtas_last_error(NULL);

	if (nret > 1 && outputs != NULL)
		for (i = 0; i < nret-1; ++i)
<<<<<<< HEAD
			outputs[i] = rtas_args->rets[i+1];
	ret = (nret > 0)? rtas_args->rets[0]: 0;

	unlock_rtas(s);

	if (buff_copy) {
		log_error(buff_copy, ERR_TYPE_RTAS_LOG, 0);
		if (mem_init_done)
=======
			outputs[i] = be32_to_cpu(args->rets[i + 1]);
	ret = (nret > 0) ? be32_to_cpu(args->rets[0]) : 0;

	lockdep_unpin_lock(&rtas_lock, cookie);
	raw_spin_unlock_irqrestore(&rtas_lock, flags);

	if (buff_copy) {
		log_error(buff_copy, ERR_TYPE_RTAS_LOG, 0);
		if (slab_is_available())
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			kfree(buff_copy);
	}
	return ret;
}
<<<<<<< HEAD
EXPORT_SYMBOL(rtas_call);

/* For RTAS_BUSY (-2), delay for 1 millisecond.  For an extended busy status
 * code of 990n, perform the hinted delay of 10^n (last digit) milliseconds.
=======
EXPORT_SYMBOL_GPL(rtas_call);

/**
 * rtas_busy_delay_time() - From an RTAS status value, calculate the
 *                          suggested delay time in milliseconds.
 *
 * @status: a value returned from rtas_call() or similar APIs which return
 *          the status of a RTAS function call.
 *
 * Context: Any context.
 *
 * Return:
 * * 100000 - If @status is 9905.
 * * 10000  - If @status is 9904.
 * * 1000   - If @status is 9903.
 * * 100    - If @status is 9902.
 * * 10     - If @status is 9901.
 * * 1      - If @status is either 9900 or -2. This is "wrong" for -2, but
 *            some callers depend on this behavior, and the worst outcome
 *            is that they will delay for longer than necessary.
 * * 0      - If @status is not a busy or extended delay value.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
unsigned int rtas_busy_delay_time(int status)
{
	int order;
	unsigned int ms = 0;

	if (status == RTAS_BUSY) {
		ms = 1;
<<<<<<< HEAD
	} else if (status >= 9900 && status <= 9905) {
		order = status - 9900;
=======
	} else if (status >= RTAS_EXTENDED_DELAY_MIN &&
		   status <= RTAS_EXTENDED_DELAY_MAX) {
		order = status - RTAS_EXTENDED_DELAY_MIN;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		for (ms = 1; order > 0; order--)
			ms *= 10;
	}

	return ms;
}
<<<<<<< HEAD
EXPORT_SYMBOL(rtas_busy_delay_time);

/* For an RTAS busy status code, perform the hinted delay. */
unsigned int rtas_busy_delay(int status)
{
	unsigned int ms;

	might_sleep();
	ms = rtas_busy_delay_time(status);
	if (ms && need_resched())
		msleep(ms);

	return ms;
}
EXPORT_SYMBOL(rtas_busy_delay);

static int rtas_error_rc(int rtas_rc)
=======

/*
 * Early boot fallback for rtas_busy_delay().
 */
static bool __init rtas_busy_delay_early(int status)
{
	static size_t successive_ext_delays __initdata;
	bool retry;

	switch (status) {
	case RTAS_EXTENDED_DELAY_MIN...RTAS_EXTENDED_DELAY_MAX:
		/*
		 * In the unlikely case that we receive an extended
		 * delay status in early boot, the OS is probably not
		 * the cause, and there's nothing we can do to clear
		 * the condition. Best we can do is delay for a bit
		 * and hope it's transient. Lie to the caller if it
		 * seems like we're stuck in a retry loop.
		 */
		mdelay(1);
		retry = true;
		successive_ext_delays += 1;
		if (successive_ext_delays > 1000) {
			pr_err("too many extended delays, giving up\n");
			dump_stack();
			retry = false;
			successive_ext_delays = 0;
		}
		break;
	case RTAS_BUSY:
		retry = true;
		successive_ext_delays = 0;
		break;
	default:
		retry = false;
		successive_ext_delays = 0;
		break;
	}

	return retry;
}

/**
 * rtas_busy_delay() - helper for RTAS busy and extended delay statuses
 *
 * @status: a value returned from rtas_call() or similar APIs which return
 *          the status of a RTAS function call.
 *
 * Context: Process context. May sleep or schedule.
 *
 * Return:
 * * true  - @status is RTAS_BUSY or an extended delay hint. The
 *           caller may assume that the CPU has been yielded if necessary,
 *           and that an appropriate delay for @status has elapsed.
 *           Generally the caller should reattempt the RTAS call which
 *           yielded @status.
 *
 * * false - @status is not @RTAS_BUSY nor an extended delay hint. The
 *           caller is responsible for handling @status.
 */
bool __ref rtas_busy_delay(int status)
{
	unsigned int ms;
	bool ret;

	/*
	 * Can't do timed sleeps before timekeeping is up.
	 */
	if (system_state < SYSTEM_SCHEDULING)
		return rtas_busy_delay_early(status);

	switch (status) {
	case RTAS_EXTENDED_DELAY_MIN...RTAS_EXTENDED_DELAY_MAX:
		ret = true;
		ms = rtas_busy_delay_time(status);
		/*
		 * The extended delay hint can be as high as 100 seconds.
		 * Surely any function returning such a status is either
		 * buggy or isn't going to be significantly slowed by us
		 * polling at 1HZ. Clamp the sleep time to one second.
		 */
		ms = clamp(ms, 1U, 1000U);
		/*
		 * The delay hint is an order-of-magnitude suggestion, not
		 * a minimum. It is fine, possibly even advantageous, for
		 * us to pause for less time than hinted. For small values,
		 * use usleep_range() to ensure we don't sleep much longer
		 * than actually needed.
		 *
		 * See Documentation/timers/timers-howto.rst for
		 * explanation of the threshold used here. In effect we use
		 * usleep_range() for 9900 and 9901, msleep() for
		 * 9902-9905.
		 */
		if (ms <= 20)
			usleep_range(ms * 100, ms * 1000);
		else
			msleep(ms);
		break;
	case RTAS_BUSY:
		ret = true;
		/*
		 * We should call again immediately if there's no other
		 * work to do.
		 */
		cond_resched();
		break;
	default:
		ret = false;
		/*
		 * Not a busy or extended delay status; the caller should
		 * handle @status itself. Ensure we warn on misuses in
		 * atomic context regardless.
		 */
		might_sleep();
		break;
	}

	return ret;
}
EXPORT_SYMBOL_GPL(rtas_busy_delay);

int rtas_error_rc(int rtas_rc)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int rc;

	switch (rtas_rc) {
<<<<<<< HEAD
		case -1: 		/* Hardware Error */
			rc = -EIO;
			break;
		case -3:		/* Bad indicator/domain/etc */
			rc = -EINVAL;
			break;
		case -9000:		/* Isolation error */
			rc = -EFAULT;
			break;
		case -9001:		/* Outstanding TCE/PTE */
			rc = -EEXIST;
			break;
		case -9002:		/* No usable slot */
			rc = -ENODEV;
			break;
		default:
			printk(KERN_ERR "%s: unexpected RTAS error %d\n",
					__func__, rtas_rc);
			rc = -ERANGE;
			break;
	}
	return rc;
}

int rtas_get_power_level(int powerdomain, int *level)
{
	int token = rtas_token("get-power-level");
=======
	case RTAS_HARDWARE_ERROR:	/* Hardware Error */
		rc = -EIO;
		break;
	case RTAS_INVALID_PARAMETER:	/* Bad indicator/domain/etc */
		rc = -EINVAL;
		break;
	case -9000:			/* Isolation error */
		rc = -EFAULT;
		break;
	case -9001:			/* Outstanding TCE/PTE */
		rc = -EEXIST;
		break;
	case -9002:			/* No usable slot */
		rc = -ENODEV;
		break;
	default:
		pr_err("%s: unexpected error %d\n", __func__, rtas_rc);
		rc = -ERANGE;
		break;
	}
	return rc;
}
EXPORT_SYMBOL_GPL(rtas_error_rc);

int rtas_get_power_level(int powerdomain, int *level)
{
	int token = rtas_function_token(RTAS_FN_GET_POWER_LEVEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int rc;

	if (token == RTAS_UNKNOWN_SERVICE)
		return -ENOENT;

	while ((rc = rtas_call(token, 1, 2, level, powerdomain)) == RTAS_BUSY)
		udelay(1);

	if (rc < 0)
		return rtas_error_rc(rc);
	return rc;
}
<<<<<<< HEAD
EXPORT_SYMBOL(rtas_get_power_level);

int rtas_set_power_level(int powerdomain, int level, int *setlevel)
{
	int token = rtas_token("set-power-level");
=======
EXPORT_SYMBOL_GPL(rtas_get_power_level);

int rtas_set_power_level(int powerdomain, int level, int *setlevel)
{
	int token = rtas_function_token(RTAS_FN_SET_POWER_LEVEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int rc;

	if (token == RTAS_UNKNOWN_SERVICE)
		return -ENOENT;

	do {
		rc = rtas_call(token, 2, 2, setlevel, powerdomain, level);
	} while (rtas_busy_delay(rc));

	if (rc < 0)
		return rtas_error_rc(rc);
	return rc;
}
<<<<<<< HEAD
EXPORT_SYMBOL(rtas_set_power_level);

int rtas_get_sensor(int sensor, int index, int *state)
{
	int token = rtas_token("get-sensor-state");
=======
EXPORT_SYMBOL_GPL(rtas_set_power_level);

int rtas_get_sensor(int sensor, int index, int *state)
{
	int token = rtas_function_token(RTAS_FN_GET_SENSOR_STATE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int rc;

	if (token == RTAS_UNKNOWN_SERVICE)
		return -ENOENT;

	do {
		rc = rtas_call(token, 2, 2, state, sensor, index);
	} while (rtas_busy_delay(rc));

	if (rc < 0)
		return rtas_error_rc(rc);
	return rc;
}
<<<<<<< HEAD
EXPORT_SYMBOL(rtas_get_sensor);

int rtas_get_sensor_fast(int sensor, int index, int *state)
{
	int token = rtas_token("get-sensor-state");
=======
EXPORT_SYMBOL_GPL(rtas_get_sensor);

int rtas_get_sensor_fast(int sensor, int index, int *state)
{
	int token = rtas_function_token(RTAS_FN_GET_SENSOR_STATE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int rc;

	if (token == RTAS_UNKNOWN_SERVICE)
		return -ENOENT;

	rc = rtas_call(token, 2, 2, state, sensor, index);
	WARN_ON(rc == RTAS_BUSY || (rc >= RTAS_EXTENDED_DELAY_MIN &&
				    rc <= RTAS_EXTENDED_DELAY_MAX));

	if (rc < 0)
		return rtas_error_rc(rc);
	return rc;
}

bool rtas_indicator_present(int token, int *maxindex)
{
	int proplen, count, i;
	const struct indicator_elem {
<<<<<<< HEAD
		u32 token;
		u32 maxindex;
=======
		__be32 token;
		__be32 maxindex;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} *indicators;

	indicators = of_get_property(rtas.dev, "rtas-indicators", &proplen);
	if (!indicators)
		return false;

	count = proplen / sizeof(struct indicator_elem);

	for (i = 0; i < count; i++) {
<<<<<<< HEAD
		if (indicators[i].token != token)
			continue;
		if (maxindex)
			*maxindex = indicators[i].maxindex;
=======
		if (__be32_to_cpu(indicators[i].token) != token)
			continue;
		if (maxindex)
			*maxindex = __be32_to_cpu(indicators[i].maxindex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return true;
	}

	return false;
}
<<<<<<< HEAD
EXPORT_SYMBOL(rtas_indicator_present);

int rtas_set_indicator(int indicator, int index, int new_value)
{
	int token = rtas_token("set-indicator");
=======

int rtas_set_indicator(int indicator, int index, int new_value)
{
	int token = rtas_function_token(RTAS_FN_SET_INDICATOR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int rc;

	if (token == RTAS_UNKNOWN_SERVICE)
		return -ENOENT;

	do {
		rc = rtas_call(token, 3, 1, NULL, indicator, index, new_value);
	} while (rtas_busy_delay(rc));

	if (rc < 0)
		return rtas_error_rc(rc);
	return rc;
}
<<<<<<< HEAD
EXPORT_SYMBOL(rtas_set_indicator);
=======
EXPORT_SYMBOL_GPL(rtas_set_indicator);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Ignoring RTAS extended delay
 */
int rtas_set_indicator_fast(int indicator, int index, int new_value)
{
<<<<<<< HEAD
	int rc;
	int token = rtas_token("set-indicator");
=======
	int token = rtas_function_token(RTAS_FN_SET_INDICATOR);
	int rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (token == RTAS_UNKNOWN_SERVICE)
		return -ENOENT;

	rc = rtas_call(token, 3, 1, NULL, indicator, index, new_value);

<<<<<<< HEAD
	WARN_ON(rc == -2 || (rc >= 9900 && rc <= 9905));
=======
	WARN_ON(rc == RTAS_BUSY || (rc >= RTAS_EXTENDED_DELAY_MIN &&
				    rc <= RTAS_EXTENDED_DELAY_MAX));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (rc < 0)
		return rtas_error_rc(rc);

	return rc;
}

<<<<<<< HEAD
void rtas_restart(char *cmd)
{
	if (rtas_flash_term_hook)
		rtas_flash_term_hook(SYS_RESTART);
	printk("RTAS system-reboot returned %d\n",
	       rtas_call(rtas_token("system-reboot"), 0, 1, NULL));
=======
/**
 * rtas_ibm_suspend_me() - Call ibm,suspend-me to suspend the LPAR.
 *
 * @fw_status: RTAS call status will be placed here if not NULL.
 *
 * rtas_ibm_suspend_me() should be called only on a CPU which has
 * received H_CONTINUE from the H_JOIN hcall. All other active CPUs
 * should be waiting to return from H_JOIN.
 *
 * rtas_ibm_suspend_me() may suspend execution of the OS
 * indefinitely. Callers should take appropriate measures upon return, such as
 * resetting watchdog facilities.
 *
 * Callers may choose to retry this call if @fw_status is
 * %RTAS_THREADS_ACTIVE.
 *
 * Return:
 * 0          - The partition has resumed from suspend, possibly after
 *              migration to a different host.
 * -ECANCELED - The operation was aborted.
 * -EAGAIN    - There were other CPUs not in H_JOIN at the time of the call.
 * -EBUSY     - Some other condition prevented the suspend from succeeding.
 * -EIO       - Hardware/platform error.
 */
int rtas_ibm_suspend_me(int *fw_status)
{
	int token = rtas_function_token(RTAS_FN_IBM_SUSPEND_ME);
	int fwrc;
	int ret;

	fwrc = rtas_call(token, 0, 1, NULL);

	switch (fwrc) {
	case 0:
		ret = 0;
		break;
	case RTAS_SUSPEND_ABORTED:
		ret = -ECANCELED;
		break;
	case RTAS_THREADS_ACTIVE:
		ret = -EAGAIN;
		break;
	case RTAS_NOT_SUSPENDABLE:
	case RTAS_OUTSTANDING_COPROC:
		ret = -EBUSY;
		break;
	case -1:
	default:
		ret = -EIO;
		break;
	}

	if (fw_status)
		*fw_status = fwrc;

	return ret;
}

void __noreturn rtas_restart(char *cmd)
{
	if (rtas_flash_term_hook)
		rtas_flash_term_hook(SYS_RESTART);
	pr_emerg("system-reboot returned %d\n",
		 rtas_call(rtas_function_token(RTAS_FN_SYSTEM_REBOOT), 0, 1, NULL));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (;;);
}

void rtas_power_off(void)
{
	if (rtas_flash_term_hook)
		rtas_flash_term_hook(SYS_POWER_OFF);
	/* allow power on only with power button press */
<<<<<<< HEAD
	printk("RTAS power-off returned %d\n",
	       rtas_call(rtas_token("power-off"), 2, 1, NULL, -1, -1));
	for (;;);
}

void rtas_halt(void)
=======
	pr_emerg("power-off returned %d\n",
		 rtas_call(rtas_function_token(RTAS_FN_POWER_OFF), 2, 1, NULL, -1, -1));
	for (;;);
}

void __noreturn rtas_halt(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (rtas_flash_term_hook)
		rtas_flash_term_hook(SYS_HALT);
	/* allow power on only with power button press */
<<<<<<< HEAD
	printk("RTAS power-off returned %d\n",
	       rtas_call(rtas_token("power-off"), 2, 1, NULL, -1, -1));
=======
	pr_emerg("power-off returned %d\n",
		 rtas_call(rtas_function_token(RTAS_FN_POWER_OFF), 2, 1, NULL, -1, -1));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (;;);
}

/* Must be in the RMO region, so we place it here */
static char rtas_os_term_buf[2048];
<<<<<<< HEAD

void rtas_os_term(char *str)
{
=======
static bool ibm_extended_os_term;

void rtas_os_term(char *str)
{
	s32 token = rtas_function_token(RTAS_FN_IBM_OS_TERM);
	static struct rtas_args args;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int status;

	/*
	 * Firmware with the ibm,extended-os-term property is guaranteed
	 * to always return from an ibm,os-term call. Earlier versions without
	 * this property may terminate the partition which we want to avoid
	 * since it interferes with panic_timeout.
	 */
<<<<<<< HEAD
	if (RTAS_UNKNOWN_SERVICE == rtas_token("ibm,os-term") ||
	    RTAS_UNKNOWN_SERVICE == rtas_token("ibm,extended-os-term"))
=======

	if (token == RTAS_UNKNOWN_SERVICE || !ibm_extended_os_term)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;

	snprintf(rtas_os_term_buf, 2048, "OS panic: %s", str);

<<<<<<< HEAD
	do {
		status = rtas_call(rtas_token("ibm,os-term"), 1, 1, NULL,
				   __pa(rtas_os_term_buf));
	} while (rtas_busy_delay(status));

	if (status != 0)
		printk(KERN_EMERG "ibm,os-term call failed %d\n", status);
}

static int ibm_suspend_me_token = RTAS_UNKNOWN_SERVICE;
#ifdef CONFIG_PPC_PSERIES
static int __rtas_suspend_last_cpu(struct rtas_suspend_me_data *data, int wake_when_done)
{
	u16 slb_size = mmu_slb_size;
	int rc = H_MULTI_THREADS_ACTIVE;
	int cpu;

	slb_set_size(SLB_MIN_SIZE);
	printk(KERN_DEBUG "calling ibm,suspend-me on cpu %i\n", smp_processor_id());

	while (rc == H_MULTI_THREADS_ACTIVE && !atomic_read(&data->done) &&
	       !atomic_read(&data->error))
		rc = rtas_call(data->token, 0, 1, NULL);

	if (rc || atomic_read(&data->error)) {
		printk(KERN_DEBUG "ibm,suspend-me returned %d\n", rc);
		slb_set_size(slb_size);
	}

	if (atomic_read(&data->error))
		rc = atomic_read(&data->error);

	atomic_set(&data->error, rc);
	pSeries_coalesce_init();

	if (wake_when_done) {
		atomic_set(&data->done, 1);

		for_each_online_cpu(cpu)
			plpar_hcall_norets(H_PROD, get_hard_smp_processor_id(cpu));
	}

	if (atomic_dec_return(&data->working) == 0)
		complete(data->complete);

	return rc;
}

int rtas_suspend_last_cpu(struct rtas_suspend_me_data *data)
{
	atomic_inc(&data->working);
	return __rtas_suspend_last_cpu(data, 0);
}

static int __rtas_suspend_cpu(struct rtas_suspend_me_data *data, int wake_when_done)
{
	long rc = H_SUCCESS;
	unsigned long msr_save;
	int cpu;

	atomic_inc(&data->working);

	/* really need to ensure MSR.EE is off for H_JOIN */
	msr_save = mfmsr();
	mtmsr(msr_save & ~(MSR_EE));

	while (rc == H_SUCCESS && !atomic_read(&data->done) && !atomic_read(&data->error))
		rc = plpar_hcall_norets(H_JOIN);

	mtmsr(msr_save);

	if (rc == H_SUCCESS) {
		/* This cpu was prodded and the suspend is complete. */
		goto out;
	} else if (rc == H_CONTINUE) {
		/* All other cpus are in H_JOIN, this cpu does
		 * the suspend.
		 */
		return __rtas_suspend_last_cpu(data, wake_when_done);
	} else {
		printk(KERN_ERR "H_JOIN on cpu %i failed with rc = %ld\n",
		       smp_processor_id(), rc);
		atomic_set(&data->error, rc);
	}

	if (wake_when_done) {
		atomic_set(&data->done, 1);

		/* This cpu did the suspend or got an error; in either case,
		 * we need to prod all other other cpus out of join state.
		 * Extra prods are harmless.
		 */
		for_each_online_cpu(cpu)
			plpar_hcall_norets(H_PROD, get_hard_smp_processor_id(cpu));
	}
out:
	if (atomic_dec_return(&data->working) == 0)
		complete(data->complete);
	return rc;
}

int rtas_suspend_cpu(struct rtas_suspend_me_data *data)
{
	return __rtas_suspend_cpu(data, 0);
}

static void rtas_percpu_suspend_me(void *info)
{
	__rtas_suspend_cpu((struct rtas_suspend_me_data *)info, 1);
}

enum rtas_cpu_state {
	DOWN,
	UP,
};

#ifndef CONFIG_SMP
static int rtas_cpu_state_change_mask(enum rtas_cpu_state state,
				cpumask_var_t cpus)
{
	if (!cpumask_empty(cpus)) {
		cpumask_clear(cpus);
		return -EINVAL;
	} else
		return 0;
}
#else
/* On return cpumask will be altered to indicate CPUs changed.
 * CPUs with states changed will be set in the mask,
 * CPUs with status unchanged will be unset in the mask. */
static int rtas_cpu_state_change_mask(enum rtas_cpu_state state,
				cpumask_var_t cpus)
{
	int cpu;
	int cpuret = 0;
	int ret = 0;

	if (cpumask_empty(cpus))
		return 0;

	for_each_cpu(cpu, cpus) {
		switch (state) {
		case DOWN:
			cpuret = cpu_down(cpu);
			break;
		case UP:
			cpuret = cpu_up(cpu);
			break;
		}
		if (cpuret) {
			pr_debug("%s: cpu_%s for cpu#%d returned %d.\n",
					__func__,
					((state == UP) ? "up" : "down"),
					cpu, cpuret);
			if (!ret)
				ret = cpuret;
			if (state == UP) {
				/* clear bits for unchanged cpus, return */
				cpumask_shift_right(cpus, cpus, cpu);
				cpumask_shift_left(cpus, cpus, cpu);
				break;
			} else {
				/* clear bit for unchanged cpu, continue */
				cpumask_clear_cpu(cpu, cpus);
			}
		}
	}

	return ret;
}
#endif

int rtas_online_cpus_mask(cpumask_var_t cpus)
{
	int ret;

	ret = rtas_cpu_state_change_mask(UP, cpus);

	if (ret) {
		cpumask_var_t tmp_mask;

		if (!alloc_cpumask_var(&tmp_mask, GFP_TEMPORARY))
			return ret;

		/* Use tmp_mask to preserve cpus mask from first failure */
		cpumask_copy(tmp_mask, cpus);
		rtas_offline_cpus_mask(tmp_mask);
		free_cpumask_var(tmp_mask);
	}

	return ret;
}
EXPORT_SYMBOL(rtas_online_cpus_mask);

int rtas_offline_cpus_mask(cpumask_var_t cpus)
{
	return rtas_cpu_state_change_mask(DOWN, cpus);
}
EXPORT_SYMBOL(rtas_offline_cpus_mask);

int rtas_ibm_suspend_me(struct rtas_args *args)
{
	long state;
	long rc;
	unsigned long retbuf[PLPAR_HCALL_BUFSIZE];
	struct rtas_suspend_me_data data;
	DECLARE_COMPLETION_ONSTACK(done);
	cpumask_var_t offline_mask;
	int cpuret;

	if (!rtas_service_present("ibm,suspend-me"))
		return -ENOSYS;

	/* Make sure the state is valid */
	rc = plpar_hcall(H_VASI_STATE, retbuf,
			 ((u64)args->args[0] << 32) | args->args[1]);

	state = retbuf[0];

	if (rc) {
		printk(KERN_ERR "rtas_ibm_suspend_me: vasi_state returned %ld\n",rc);
		return rc;
	} else if (state == H_VASI_ENABLED) {
		args->args[args->nargs] = RTAS_NOT_SUSPENDABLE;
		return 0;
	} else if (state != H_VASI_SUSPENDING) {
		printk(KERN_ERR "rtas_ibm_suspend_me: vasi_state returned state %ld\n",
		       state);
		args->args[args->nargs] = -1;
		return 0;
	}

	if (!alloc_cpumask_var(&offline_mask, GFP_TEMPORARY))
		return -ENOMEM;

	atomic_set(&data.working, 0);
	atomic_set(&data.done, 0);
	atomic_set(&data.error, 0);
	data.token = rtas_token("ibm,suspend-me");
	data.complete = &done;

	/* All present CPUs must be online */
	cpumask_andnot(offline_mask, cpu_present_mask, cpu_online_mask);
	cpuret = rtas_online_cpus_mask(offline_mask);
	if (cpuret) {
		pr_err("%s: Could not bring present CPUs online.\n", __func__);
		atomic_set(&data.error, cpuret);
		goto out;
	}

	stop_topology_update();

	/* Call function on all CPUs.  One of us will make the
	 * rtas call
	 */
	if (on_each_cpu(rtas_percpu_suspend_me, &data, 0))
		atomic_set(&data.error, -EINVAL);

	wait_for_completion(&done);

	if (atomic_read(&data.error) != 0)
		printk(KERN_ERR "Error doing global join\n");

	start_topology_update();

	/* Take down CPUs not online prior to suspend */
	cpuret = rtas_offline_cpus_mask(offline_mask);
	if (cpuret)
		pr_warn("%s: Could not restore CPUs to offline state.\n",
				__func__);

out:
	free_cpumask_var(offline_mask);
	return atomic_read(&data.error);
}
#else /* CONFIG_PPC_PSERIES */
int rtas_ibm_suspend_me(struct rtas_args *args)
{
	return -ENOSYS;
}
#endif

/**
 * Find a specific pseries error log in an RTAS extended event log.
 * @log: RTAS error/event log
 * @section_id: two character section identifier
 *
 * Returns a pointer to the specified errorlog or NULL if not found.
 */
struct pseries_errorlog *get_pseries_errorlog(struct rtas_error_log *log,
					      uint16_t section_id)
=======
	/*
	 * Keep calling as long as RTAS returns a "try again" status,
	 * but don't use rtas_busy_delay(), which potentially
	 * schedules.
	 */
	do {
		rtas_call_unlocked(&args, token, 1, 1, NULL, __pa(rtas_os_term_buf));
		status = be32_to_cpu(args.rets[0]);
	} while (rtas_busy_delay_time(status));

	if (status != 0)
		pr_emerg("ibm,os-term call failed %d\n", status);
}

/**
 * rtas_activate_firmware() - Activate a new version of firmware.
 *
 * Context: This function may sleep.
 *
 * Activate a new version of partition firmware. The OS must call this
 * after resuming from a partition hibernation or migration in order
 * to maintain the ability to perform live firmware updates. It's not
 * catastrophic for this method to be absent or to fail; just log the
 * condition in that case.
 */
void rtas_activate_firmware(void)
{
	int token = rtas_function_token(RTAS_FN_IBM_ACTIVATE_FIRMWARE);
	int fwrc;

	if (token == RTAS_UNKNOWN_SERVICE) {
		pr_notice("ibm,activate-firmware method unavailable\n");
		return;
	}

	mutex_lock(&rtas_ibm_activate_firmware_lock);

	do {
		fwrc = rtas_call(token, 0, 1, NULL);
	} while (rtas_busy_delay(fwrc));

	mutex_unlock(&rtas_ibm_activate_firmware_lock);

	if (fwrc)
		pr_err("ibm,activate-firmware failed (%i)\n", fwrc);
}

/**
 * get_pseries_errorlog() - Find a specific pseries error log in an RTAS
 *                          extended event log.
 * @log: RTAS error/event log
 * @section_id: two character section identifier
 *
 * Return: A pointer to the specified errorlog or NULL if not found.
 */
noinstr struct pseries_errorlog *get_pseries_errorlog(struct rtas_error_log *log,
						      uint16_t section_id)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct rtas_ext_event_log_v6 *ext_log =
		(struct rtas_ext_event_log_v6 *)log->buffer;
	struct pseries_errorlog *sect;
	unsigned char *p, *log_end;
<<<<<<< HEAD

	/* Check that we understand the format */
	if (log->extended_log_length < sizeof(struct rtas_ext_event_log_v6) ||
	    ext_log->log_format != RTAS_V6EXT_LOG_FORMAT_EVENT_LOG ||
	    ext_log->company_id != RTAS_V6EXT_COMPANY_ID_IBM)
		return NULL;

	log_end = log->buffer + log->extended_log_length;
=======
	uint32_t ext_log_length = rtas_error_extended_log_length(log);
	uint8_t log_format = rtas_ext_event_log_format(ext_log);
	uint32_t company_id = rtas_ext_event_company_id(ext_log);

	/* Check that we understand the format */
	if (ext_log_length < sizeof(struct rtas_ext_event_log_v6) ||
	    log_format != RTAS_V6EXT_LOG_FORMAT_EVENT_LOG ||
	    company_id != RTAS_V6EXT_COMPANY_ID_IBM)
		return NULL;

	log_end = log->buffer + ext_log_length;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	p = ext_log->vendor_log;

	while (p < log_end) {
		sect = (struct pseries_errorlog *)p;
<<<<<<< HEAD
		if (sect->id == section_id)
			return sect;
		p += sect->length;
=======
		if (pseries_errorlog_id(sect) == section_id)
			return sect;
		p += pseries_errorlog_length(sect);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return NULL;
}

<<<<<<< HEAD
asmlinkage int ppc_rtas(struct rtas_args __user *uargs)
{
	struct rtas_args args;
	unsigned long flags;
	char *buff_copy, *errbuf = NULL;
	int nargs;
	int rc;
=======
/*
 * The sys_rtas syscall, as originally designed, allows root to pass
 * arbitrary physical addresses to RTAS calls. A number of RTAS calls
 * can be abused to write to arbitrary memory and do other things that
 * are potentially harmful to system integrity, and thus should only
 * be used inside the kernel and not exposed to userspace.
 *
 * All known legitimate users of the sys_rtas syscall will only ever
 * pass addresses that fall within the RMO buffer, and use a known
 * subset of RTAS calls.
 *
 * Accordingly, we filter RTAS requests to check that the call is
 * permitted, and that provided pointers fall within the RMO buffer.
 * If a function is allowed to be invoked via the syscall, then its
 * entry in the rtas_functions table points to a rtas_filter that
 * describes its constraints, with the indexes of the parameters which
 * are expected to contain addresses and sizes of buffers allocated
 * inside the RMO buffer.
 */

static bool in_rmo_buf(u32 base, u32 end)
{
	return base >= rtas_rmo_buf &&
		base < (rtas_rmo_buf + RTAS_USER_REGION_SIZE) &&
		base <= end &&
		end >= rtas_rmo_buf &&
		end < (rtas_rmo_buf + RTAS_USER_REGION_SIZE);
}

static bool block_rtas_call(const struct rtas_function *func, int nargs,
			    struct rtas_args *args)
{
	const struct rtas_filter *f;
	const bool is_platform_dump =
		func == &rtas_function_table[RTAS_FNIDX__IBM_PLATFORM_DUMP];
	const bool is_config_conn =
		func == &rtas_function_table[RTAS_FNIDX__IBM_CONFIGURE_CONNECTOR];
	u32 base, size, end;

	/*
	 * Only functions with filters attached are allowed.
	 */
	f = func->filter;
	if (!f)
		goto err;
	/*
	 * And some functions aren't allowed on LE.
	 */
	if (IS_ENABLED(CONFIG_CPU_LITTLE_ENDIAN) && func->banned_for_syscall_on_le)
		goto err;

	if (f->buf_idx1 != -1) {
		base = be32_to_cpu(args->args[f->buf_idx1]);
		if (f->size_idx1 != -1)
			size = be32_to_cpu(args->args[f->size_idx1]);
		else if (f->fixed_size)
			size = f->fixed_size;
		else
			size = 1;

		end = base + size - 1;

		/*
		 * Special case for ibm,platform-dump - NULL buffer
		 * address is used to indicate end of dump processing
		 */
		if (is_platform_dump && base == 0)
			return false;

		if (!in_rmo_buf(base, end))
			goto err;
	}

	if (f->buf_idx2 != -1) {
		base = be32_to_cpu(args->args[f->buf_idx2]);
		if (f->size_idx2 != -1)
			size = be32_to_cpu(args->args[f->size_idx2]);
		else if (f->fixed_size)
			size = f->fixed_size;
		else
			size = 1;
		end = base + size - 1;

		/*
		 * Special case for ibm,configure-connector where the
		 * address can be 0
		 */
		if (is_config_conn && base == 0)
			return false;

		if (!in_rmo_buf(base, end))
			goto err;
	}

	return false;
err:
	pr_err_ratelimited("sys_rtas: RTAS call blocked - exploit attempt?\n");
	pr_err_ratelimited("sys_rtas: %s nargs=%d (called by %s)\n",
			   func->name, nargs, current->comm);
	return true;
}

/* We assume to be passed big endian arguments */
SYSCALL_DEFINE1(rtas, struct rtas_args __user *, uargs)
{
	const struct rtas_function *func;
	struct pin_cookie cookie;
	struct rtas_args args;
	unsigned long flags;
	char *buff_copy, *errbuf = NULL;
	int nargs, nret, token;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	if (!rtas.entry)
		return -EINVAL;

	if (copy_from_user(&args, uargs, 3 * sizeof(u32)) != 0)
		return -EFAULT;

<<<<<<< HEAD
	nargs = args.nargs;
	if (nargs > ARRAY_SIZE(args.args)
	    || args.nret > ARRAY_SIZE(args.args)
	    || nargs + args.nret > ARRAY_SIZE(args.args))
=======
	nargs = be32_to_cpu(args.nargs);
	nret  = be32_to_cpu(args.nret);
	token = be32_to_cpu(args.token);

	if (nargs >= ARRAY_SIZE(args.args)
	    || nret > ARRAY_SIZE(args.args)
	    || nargs + nret > ARRAY_SIZE(args.args))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;

	/* Copy in args. */
	if (copy_from_user(args.args, uargs->args,
			   nargs * sizeof(rtas_arg_t)) != 0)
		return -EFAULT;

<<<<<<< HEAD
	if (args.token == RTAS_UNKNOWN_SERVICE)
		return -EINVAL;

	args.rets = &args.args[nargs];
	memset(args.rets, 0, args.nret * sizeof(rtas_arg_t));

	/* Need to handle ibm,suspend_me call specially */
	if (args.token == ibm_suspend_me_token) {
		rc = rtas_ibm_suspend_me(&args);
		if (rc)
=======
	/*
	 * If this token doesn't correspond to a function the kernel
	 * understands, you're not allowed to call it.
	 */
	func = rtas_token_to_function_untrusted(token);
	if (!func)
		return -EINVAL;

	args.rets = &args.args[nargs];
	memset(args.rets, 0, nret * sizeof(rtas_arg_t));

	if (block_rtas_call(func, nargs, &args))
		return -EINVAL;

	if (token_is_restricted_errinjct(token)) {
		int err;

		err = security_locked_down(LOCKDOWN_RTAS_ERROR_INJECTION);
		if (err)
			return err;
	}

	/* Need to handle ibm,suspend_me call specially */
	if (token == rtas_function_token(RTAS_FN_IBM_SUSPEND_ME)) {

		/*
		 * rtas_ibm_suspend_me assumes the streamid handle is in cpu
		 * endian, or at least the hcall within it requires it.
		 */
		int rc = 0;
		u64 handle = ((u64)be32_to_cpu(args.args[0]) << 32)
		              | be32_to_cpu(args.args[1]);
		rc = rtas_syscall_dispatch_ibm_suspend_me(handle);
		if (rc == -EAGAIN)
			args.rets[0] = cpu_to_be32(RTAS_NOT_SUSPENDABLE);
		else if (rc == -EIO)
			args.rets[0] = cpu_to_be32(-1);
		else if (rc)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return rc;
		goto copy_return;
	}

	buff_copy = get_errorlog_buffer();

<<<<<<< HEAD
	flags = lock_rtas();

	rtas.args = args;
	enter_rtas(__pa(&rtas.args));
	args = rtas.args;

	/* A -1 return code indicates that the last command couldn't
	   be completed due to a hardware error. */
	if (args.rets[0] == -1)
		errbuf = __fetch_rtas_last_error(buff_copy);

	unlock_rtas(flags);
=======
	/*
	 * If this function has a mutex assigned to it, we must
	 * acquire it to avoid interleaving with any kernel-based uses
	 * of the same function. Kernel-based sequences acquire the
	 * appropriate mutex explicitly.
	 */
	if (func->lock)
		mutex_lock(func->lock);

	raw_spin_lock_irqsave(&rtas_lock, flags);
	cookie = lockdep_pin_lock(&rtas_lock);

	rtas_args = args;
	do_enter_rtas(&rtas_args);
	args = rtas_args;

	/* A -1 return code indicates that the last command couldn't
	   be completed due to a hardware error. */
	if (be32_to_cpu(args.rets[0]) == -1)
		errbuf = __fetch_rtas_last_error(buff_copy);

	lockdep_unpin_lock(&rtas_lock, cookie);
	raw_spin_unlock_irqrestore(&rtas_lock, flags);

	if (func->lock)
		mutex_unlock(func->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (buff_copy) {
		if (errbuf)
			log_error(errbuf, ERR_TYPE_RTAS_LOG, 0);
		kfree(buff_copy);
	}

 copy_return:
	/* Copy out args. */
	if (copy_to_user(uargs->args + nargs,
			 args.args + nargs,
<<<<<<< HEAD
			 args.nret * sizeof(rtas_arg_t)) != 0)
=======
			 nret * sizeof(rtas_arg_t)) != 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EFAULT;

	return 0;
}

<<<<<<< HEAD
/*
 * Call early during boot, before mem init or bootmem, to retrieve the RTAS
 * informations from the device-tree and allocate the RMO buffer for userland
=======
static void __init rtas_function_table_init(void)
{
	struct property *prop;

	for (size_t i = 0; i < ARRAY_SIZE(rtas_function_table); ++i) {
		struct rtas_function *curr = &rtas_function_table[i];
		struct rtas_function *prior;
		int cmp;

		curr->token = RTAS_UNKNOWN_SERVICE;

		if (i == 0)
			continue;
		/*
		 * Ensure table is sorted correctly for binary search
		 * on function names.
		 */
		prior = &rtas_function_table[i - 1];

		cmp = strcmp(prior->name, curr->name);
		if (cmp < 0)
			continue;

		if (cmp == 0) {
			pr_err("'%s' has duplicate function table entries\n",
			       curr->name);
		} else {
			pr_err("function table unsorted: '%s' wrongly precedes '%s'\n",
			       prior->name, curr->name);
		}
	}

	for_each_property_of_node(rtas.dev, prop) {
		struct rtas_function *func;

		if (prop->length != sizeof(u32))
			continue;

		func = __rtas_name_to_function(prop->name);
		if (!func)
			continue;

		func->token = be32_to_cpup((__be32 *)prop->value);

		pr_debug("function %s has token %u\n", func->name, func->token);
	}
}

/*
 * Call early during boot, before mem init, to retrieve the RTAS
 * information from the device-tree and allocate the RMO buffer for userland
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * accesses.
 */
void __init rtas_initialize(void)
{
	unsigned long rtas_region = RTAS_INSTANTIATE_MAX;
<<<<<<< HEAD
=======
	u32 base, size, entry;
	int no_base, no_size, no_entry;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Get RTAS dev node and fill up our "rtas" structure with infos
	 * about it.
	 */
	rtas.dev = of_find_node_by_name(NULL, "rtas");
<<<<<<< HEAD
	if (rtas.dev) {
		const u32 *basep, *entryp, *sizep;

		basep = of_get_property(rtas.dev, "linux,rtas-base", NULL);
		sizep = of_get_property(rtas.dev, "rtas-size", NULL);
		if (basep != NULL && sizep != NULL) {
			rtas.base = *basep;
			rtas.size = *sizep;
			entryp = of_get_property(rtas.dev,
					"linux,rtas-entry", NULL);
			if (entryp == NULL) /* Ugh */
				rtas.entry = rtas.base;
			else
				rtas.entry = *entryp;
		} else
			rtas.dev = NULL;
	}
	if (!rtas.dev)
		return;

=======
	if (!rtas.dev)
		return;

	no_base = of_property_read_u32(rtas.dev, "linux,rtas-base", &base);
	no_size = of_property_read_u32(rtas.dev, "rtas-size", &size);
	if (no_base || no_size) {
		of_node_put(rtas.dev);
		rtas.dev = NULL;
		return;
	}

	rtas.base = base;
	rtas.size = size;
	no_entry = of_property_read_u32(rtas.dev, "linux,rtas-entry", &entry);
	rtas.entry = no_entry ? rtas.base : entry;

	init_error_log_max();

	/* Must be called before any function token lookups */
	rtas_function_table_init();

	/*
	 * Discover this now to avoid a device tree lookup in the
	 * panic path.
	 */
	ibm_extended_os_term = of_property_read_bool(rtas.dev, "ibm,extended-os-term");

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* If RTAS was found, allocate the RMO buffer for it and look for
	 * the stop-self token if any
	 */
#ifdef CONFIG_PPC64
<<<<<<< HEAD
	if (machine_is(pseries) && firmware_has_feature(FW_FEATURE_LPAR)) {
		rtas_region = min(ppc64_rma_size, RTAS_INSTANTIATE_MAX);
		ibm_suspend_me_token = rtas_token("ibm,suspend-me");
	}
#endif
	rtas_rmo_buf = memblock_alloc_base(RTAS_RMOBUF_MAX, PAGE_SIZE, rtas_region);

#ifdef CONFIG_RTAS_ERROR_LOGGING
	rtas_last_error_token = rtas_token("rtas-last-error");
#endif
=======
	if (firmware_has_feature(FW_FEATURE_LPAR))
		rtas_region = min(ppc64_rma_size, RTAS_INSTANTIATE_MAX);
#endif
	rtas_rmo_buf = memblock_phys_alloc_range(RTAS_USER_REGION_SIZE, PAGE_SIZE,
						 0, rtas_region);
	if (!rtas_rmo_buf)
		panic("ERROR: RTAS: Failed to allocate %lx bytes below %pa\n",
		      PAGE_SIZE, &rtas_region);

	rtas_work_area_reserve_arena(rtas_region);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int __init early_init_dt_scan_rtas(unsigned long node,
		const char *uname, int depth, void *data)
{
<<<<<<< HEAD
	u32 *basep, *entryp, *sizep;
=======
	const u32 *basep, *entryp, *sizep;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (depth != 1 || strcmp(uname, "rtas") != 0)
		return 0;

	basep  = of_get_flat_dt_prop(node, "linux,rtas-base", NULL);
	entryp = of_get_flat_dt_prop(node, "linux,rtas-entry", NULL);
	sizep  = of_get_flat_dt_prop(node, "rtas-size", NULL);

<<<<<<< HEAD
=======
#ifdef CONFIG_PPC64
	/* need this feature to decide the crashkernel offset */
	if (of_get_flat_dt_prop(node, "ibm,hypertas-functions", NULL))
		powerpc_firmware_features |= FW_FEATURE_LPAR;
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (basep && entryp && sizep) {
		rtas.base = *basep;
		rtas.entry = *entryp;
		rtas.size = *sizep;
	}

#ifdef CONFIG_UDBG_RTAS_CONSOLE
	basep = of_get_flat_dt_prop(node, "put-term-char", NULL);
	if (basep)
		rtas_putchar_token = *basep;

	basep = of_get_flat_dt_prop(node, "get-term-char", NULL);
	if (basep)
		rtas_getchar_token = *basep;

	if (rtas_putchar_token != RTAS_UNKNOWN_SERVICE &&
	    rtas_getchar_token != RTAS_UNKNOWN_SERVICE)
		udbg_init_rtas_console();

#endif

	/* break now */
	return 1;
}

<<<<<<< HEAD
static arch_spinlock_t timebase_lock;
static u64 timebase = 0;

void __cpuinit rtas_give_timebase(void)
{
	unsigned long flags;

	local_irq_save(flags);
	hard_irq_disable();
	arch_spin_lock(&timebase_lock);
	rtas_call(rtas_token("freeze-time-base"), 0, 1, NULL);
	timebase = get_tb();
	arch_spin_unlock(&timebase_lock);

	while (timebase)
		barrier();
	rtas_call(rtas_token("thaw-time-base"), 0, 1, NULL);
	local_irq_restore(flags);
}

void __cpuinit rtas_take_timebase(void)
{
	while (!timebase)
		barrier();
	arch_spin_lock(&timebase_lock);
	set_tb(timebase >> 32, timebase & 0xffffffff);
	timebase = 0;
	arch_spin_unlock(&timebase_lock);
=======
static DEFINE_RAW_SPINLOCK(timebase_lock);
static u64 timebase = 0;

void rtas_give_timebase(void)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&timebase_lock, flags);
	hard_irq_disable();
	rtas_call(rtas_function_token(RTAS_FN_FREEZE_TIME_BASE), 0, 1, NULL);
	timebase = get_tb();
	raw_spin_unlock(&timebase_lock);

	while (timebase)
		barrier();
	rtas_call(rtas_function_token(RTAS_FN_THAW_TIME_BASE), 0, 1, NULL);
	local_irq_restore(flags);
}

void rtas_take_timebase(void)
{
	while (!timebase)
		barrier();
	raw_spin_lock(&timebase_lock);
	set_tb(timebase >> 32, timebase & 0xffffffff);
	timebase = 0;
	raw_spin_unlock(&timebase_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
