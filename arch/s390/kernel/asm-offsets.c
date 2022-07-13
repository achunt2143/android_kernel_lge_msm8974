<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Generate definitions needed by assembly language modules.
 * This code generates raw asm output which is post-processed to extract
 * and format the required data.
 */

#define ASM_OFFSETS_C

#include <linux/kbuild.h>
<<<<<<< HEAD
#include <linux/sched.h>
#include <asm/cputime.h>
#include <asm/timer.h>
#include <asm/vdso.h>
#include <asm/pgtable.h>

/*
 * Make sure that the compiler is new enough. We want a compiler that
 * is known to work with the "Q" assembler constraint.
 */
#if __GNUC__ < 3 || (__GNUC__ == 3 && __GNUC_MINOR__ < 3)
#error Your compiler is too old; please use version 3.3.3 or newer
#endif

int main(void)
{
	DEFINE(__THREAD_info, offsetof(struct task_struct, stack));
	DEFINE(__THREAD_ksp, offsetof(struct task_struct, thread.ksp));
	DEFINE(__THREAD_mm_segment, offsetof(struct task_struct, thread.mm_segment));
	BLANK();
	DEFINE(__TASK_pid, offsetof(struct task_struct, pid));
	BLANK();
	DEFINE(__THREAD_per_cause, offsetof(struct task_struct, thread.per_event.cause));
	DEFINE(__THREAD_per_address, offsetof(struct task_struct, thread.per_event.address));
	DEFINE(__THREAD_per_paid, offsetof(struct task_struct, thread.per_event.paid));
	BLANK();
	DEFINE(__TI_task, offsetof(struct thread_info, task));
	DEFINE(__TI_domain, offsetof(struct thread_info, exec_domain));
	DEFINE(__TI_flags, offsetof(struct thread_info, flags));
	DEFINE(__TI_cpu, offsetof(struct thread_info, cpu));
	DEFINE(__TI_precount, offsetof(struct thread_info, preempt_count));
	DEFINE(__TI_user_timer, offsetof(struct thread_info, user_timer));
	DEFINE(__TI_system_timer, offsetof(struct thread_info, system_timer));
	DEFINE(__TI_last_break, offsetof(struct thread_info, last_break));
	BLANK();
	DEFINE(__PT_ARGS, offsetof(struct pt_regs, args));
	DEFINE(__PT_PSW, offsetof(struct pt_regs, psw));
	DEFINE(__PT_GPRS, offsetof(struct pt_regs, gprs));
	DEFINE(__PT_ORIG_GPR2, offsetof(struct pt_regs, orig_gpr2));
	DEFINE(__PT_INT_CODE, offsetof(struct pt_regs, int_code));
	DEFINE(__PT_INT_PARM_LONG, offsetof(struct pt_regs, int_parm_long));
	DEFINE(__PT_SIZE, sizeof(struct pt_regs));
	BLANK();
	DEFINE(__SF_BACKCHAIN, offsetof(struct stack_frame, back_chain));
	DEFINE(__SF_GPRS, offsetof(struct stack_frame, gprs));
	DEFINE(__SF_EMPTY, offsetof(struct stack_frame, empty1));
	BLANK();
	/* timeval/timezone offsets for use by vdso */
	DEFINE(__VDSO_UPD_COUNT, offsetof(struct vdso_data, tb_update_count));
	DEFINE(__VDSO_XTIME_STAMP, offsetof(struct vdso_data, xtime_tod_stamp));
	DEFINE(__VDSO_XTIME_SEC, offsetof(struct vdso_data, xtime_clock_sec));
	DEFINE(__VDSO_XTIME_NSEC, offsetof(struct vdso_data, xtime_clock_nsec));
	DEFINE(__VDSO_WTOM_SEC, offsetof(struct vdso_data, wtom_clock_sec));
	DEFINE(__VDSO_WTOM_NSEC, offsetof(struct vdso_data, wtom_clock_nsec));
	DEFINE(__VDSO_TIMEZONE, offsetof(struct vdso_data, tz_minuteswest));
	DEFINE(__VDSO_ECTG_OK, offsetof(struct vdso_data, ectg_available));
	DEFINE(__VDSO_NTP_MULT, offsetof(struct vdso_data, ntp_mult));
	DEFINE(__VDSO_ECTG_BASE, offsetof(struct vdso_per_cpu_data, ectg_timer_base));
	DEFINE(__VDSO_ECTG_USER, offsetof(struct vdso_per_cpu_data, ectg_user_time));
	/* constants used by the vdso */
	DEFINE(__CLOCK_REALTIME, CLOCK_REALTIME);
	DEFINE(__CLOCK_MONOTONIC, CLOCK_MONOTONIC);
	DEFINE(__CLOCK_REALTIME_RES, MONOTONIC_RES_NSEC);
	BLANK();
	/* idle data offsets */
	DEFINE(__IDLE_ENTER, offsetof(struct s390_idle_data, idle_enter));
	DEFINE(__IDLE_EXIT, offsetof(struct s390_idle_data, idle_exit));
	/* vtimer queue offsets */
	DEFINE(__VQ_IDLE_ENTER, offsetof(struct vtimer_queue, idle_enter));
	DEFINE(__VQ_IDLE_EXIT, offsetof(struct vtimer_queue, idle_exit));
	/* lowcore offsets */
	DEFINE(__LC_EXT_PARAMS, offsetof(struct _lowcore, ext_params));
	DEFINE(__LC_EXT_CPU_ADDR, offsetof(struct _lowcore, ext_cpu_addr));
	DEFINE(__LC_EXT_INT_CODE, offsetof(struct _lowcore, ext_int_code));
	DEFINE(__LC_SVC_ILC, offsetof(struct _lowcore, svc_ilc));
	DEFINE(__LC_SVC_INT_CODE, offsetof(struct _lowcore, svc_code));
	DEFINE(__LC_PGM_ILC, offsetof(struct _lowcore, pgm_ilc));
	DEFINE(__LC_PGM_INT_CODE, offsetof(struct _lowcore, pgm_code));
	DEFINE(__LC_TRANS_EXC_CODE, offsetof(struct _lowcore, trans_exc_code));
	DEFINE(__LC_PER_CAUSE, offsetof(struct _lowcore, per_perc_atmid));
	DEFINE(__LC_PER_ADDRESS, offsetof(struct _lowcore, per_address));
	DEFINE(__LC_PER_PAID, offsetof(struct _lowcore, per_access_id));
	DEFINE(__LC_AR_MODE_ID, offsetof(struct _lowcore, ar_access_id));
	DEFINE(__LC_SUBCHANNEL_ID, offsetof(struct _lowcore, subchannel_id));
	DEFINE(__LC_SUBCHANNEL_NR, offsetof(struct _lowcore, subchannel_nr));
	DEFINE(__LC_IO_INT_PARM, offsetof(struct _lowcore, io_int_parm));
	DEFINE(__LC_IO_INT_WORD, offsetof(struct _lowcore, io_int_word));
	DEFINE(__LC_STFL_FAC_LIST, offsetof(struct _lowcore, stfl_fac_list));
	DEFINE(__LC_MCCK_CODE, offsetof(struct _lowcore, mcck_interruption_code));
	DEFINE(__LC_RST_OLD_PSW, offsetof(struct _lowcore, restart_old_psw));
	DEFINE(__LC_EXT_OLD_PSW, offsetof(struct _lowcore, external_old_psw));
	DEFINE(__LC_SVC_OLD_PSW, offsetof(struct _lowcore, svc_old_psw));
	DEFINE(__LC_PGM_OLD_PSW, offsetof(struct _lowcore, program_old_psw));
	DEFINE(__LC_MCK_OLD_PSW, offsetof(struct _lowcore, mcck_old_psw));
	DEFINE(__LC_IO_OLD_PSW, offsetof(struct _lowcore, io_old_psw));
	DEFINE(__LC_RST_NEW_PSW, offsetof(struct _lowcore, restart_psw));
	DEFINE(__LC_EXT_NEW_PSW, offsetof(struct _lowcore, external_new_psw));
	DEFINE(__LC_SVC_NEW_PSW, offsetof(struct _lowcore, svc_new_psw));
	DEFINE(__LC_PGM_NEW_PSW, offsetof(struct _lowcore, program_new_psw));
	DEFINE(__LC_MCK_NEW_PSW, offsetof(struct _lowcore, mcck_new_psw));
	DEFINE(__LC_IO_NEW_PSW, offsetof(struct _lowcore, io_new_psw));
	BLANK();
	DEFINE(__LC_SAVE_AREA_SYNC, offsetof(struct _lowcore, save_area_sync));
	DEFINE(__LC_SAVE_AREA_ASYNC, offsetof(struct _lowcore, save_area_async));
	DEFINE(__LC_SAVE_AREA_RESTART, offsetof(struct _lowcore, save_area_restart));
	DEFINE(__LC_RETURN_PSW, offsetof(struct _lowcore, return_psw));
	DEFINE(__LC_RETURN_MCCK_PSW, offsetof(struct _lowcore, return_mcck_psw));
	DEFINE(__LC_SYNC_ENTER_TIMER, offsetof(struct _lowcore, sync_enter_timer));
	DEFINE(__LC_ASYNC_ENTER_TIMER, offsetof(struct _lowcore, async_enter_timer));
	DEFINE(__LC_MCCK_ENTER_TIMER, offsetof(struct _lowcore, mcck_enter_timer));
	DEFINE(__LC_EXIT_TIMER, offsetof(struct _lowcore, exit_timer));
	DEFINE(__LC_USER_TIMER, offsetof(struct _lowcore, user_timer));
	DEFINE(__LC_SYSTEM_TIMER, offsetof(struct _lowcore, system_timer));
	DEFINE(__LC_STEAL_TIMER, offsetof(struct _lowcore, steal_timer));
	DEFINE(__LC_LAST_UPDATE_TIMER, offsetof(struct _lowcore, last_update_timer));
	DEFINE(__LC_LAST_UPDATE_CLOCK, offsetof(struct _lowcore, last_update_clock));
	DEFINE(__LC_CURRENT, offsetof(struct _lowcore, current_task));
	DEFINE(__LC_CURRENT_PID, offsetof(struct _lowcore, current_pid));
	DEFINE(__LC_THREAD_INFO, offsetof(struct _lowcore, thread_info));
	DEFINE(__LC_KERNEL_STACK, offsetof(struct _lowcore, kernel_stack));
	DEFINE(__LC_ASYNC_STACK, offsetof(struct _lowcore, async_stack));
	DEFINE(__LC_PANIC_STACK, offsetof(struct _lowcore, panic_stack));
	DEFINE(__LC_RESTART_STACK, offsetof(struct _lowcore, restart_stack));
	DEFINE(__LC_RESTART_FN, offsetof(struct _lowcore, restart_fn));
	DEFINE(__LC_USER_ASCE, offsetof(struct _lowcore, user_asce));
	DEFINE(__LC_INT_CLOCK, offsetof(struct _lowcore, int_clock));
	DEFINE(__LC_MCCK_CLOCK, offsetof(struct _lowcore, mcck_clock));
	DEFINE(__LC_MACHINE_FLAGS, offsetof(struct _lowcore, machine_flags));
	DEFINE(__LC_FTRACE_FUNC, offsetof(struct _lowcore, ftrace_func));
	DEFINE(__LC_IRB, offsetof(struct _lowcore, irb));
	DEFINE(__LC_DUMP_REIPL, offsetof(struct _lowcore, ipib));
	BLANK();
	DEFINE(__LC_CPU_TIMER_SAVE_AREA, offsetof(struct _lowcore, cpu_timer_save_area));
	DEFINE(__LC_CLOCK_COMP_SAVE_AREA, offsetof(struct _lowcore, clock_comp_save_area));
	DEFINE(__LC_PSW_SAVE_AREA, offsetof(struct _lowcore, psw_save_area));
	DEFINE(__LC_PREFIX_SAVE_AREA, offsetof(struct _lowcore, prefixreg_save_area));
	DEFINE(__LC_AREGS_SAVE_AREA, offsetof(struct _lowcore, access_regs_save_area));
	DEFINE(__LC_FPREGS_SAVE_AREA, offsetof(struct _lowcore, floating_pt_save_area));
	DEFINE(__LC_GPREGS_SAVE_AREA, offsetof(struct _lowcore, gpregs_save_area));
	DEFINE(__LC_CREGS_SAVE_AREA, offsetof(struct _lowcore, cregs_save_area));
#ifdef CONFIG_32BIT
	DEFINE(SAVE_AREA_BASE, offsetof(struct _lowcore, extended_save_area_addr));
#else /* CONFIG_32BIT */
	DEFINE(__LC_EXT_PARAMS2, offsetof(struct _lowcore, ext_params2));
	DEFINE(SAVE_AREA_BASE, offsetof(struct _lowcore, floating_pt_save_area));
	DEFINE(__LC_PASTE, offsetof(struct _lowcore, paste));
	DEFINE(__LC_FP_CREG_SAVE_AREA, offsetof(struct _lowcore, fpt_creg_save_area));
	DEFINE(__LC_LAST_BREAK, offsetof(struct _lowcore, breaking_event_addr));
	DEFINE(__LC_VDSO_PER_CPU, offsetof(struct _lowcore, vdso_per_cpu_data));
	DEFINE(__LC_GMAP, offsetof(struct _lowcore, gmap));
	DEFINE(__GMAP_ASCE, offsetof(struct gmap, asce));
#endif /* CONFIG_32BIT */
=======
#include <linux/kvm_host.h>
#include <linux/sched.h>
#include <linux/purgatory.h>
#include <linux/pgtable.h>
#include <linux/ftrace.h>
#include <asm/idle.h>
#include <asm/gmap.h>
#include <asm/stacktrace.h>

int main(void)
{
	/* task struct offsets */
	OFFSET(__TASK_stack, task_struct, stack);
	OFFSET(__TASK_thread, task_struct, thread);
	OFFSET(__TASK_pid, task_struct, pid);
	BLANK();
	/* thread struct offsets */
	OFFSET(__THREAD_ksp, thread_struct, ksp);
	BLANK();
	/* thread info offsets */
	OFFSET(__TI_flags, task_struct, thread_info.flags);
	BLANK();
	/* pt_regs offsets */
	OFFSET(__PT_PSW, pt_regs, psw);
	OFFSET(__PT_GPRS, pt_regs, gprs);
	OFFSET(__PT_R0, pt_regs, gprs[0]);
	OFFSET(__PT_R1, pt_regs, gprs[1]);
	OFFSET(__PT_R2, pt_regs, gprs[2]);
	OFFSET(__PT_R3, pt_regs, gprs[3]);
	OFFSET(__PT_R4, pt_regs, gprs[4]);
	OFFSET(__PT_R5, pt_regs, gprs[5]);
	OFFSET(__PT_R6, pt_regs, gprs[6]);
	OFFSET(__PT_R7, pt_regs, gprs[7]);
	OFFSET(__PT_R8, pt_regs, gprs[8]);
	OFFSET(__PT_R9, pt_regs, gprs[9]);
	OFFSET(__PT_R10, pt_regs, gprs[10]);
	OFFSET(__PT_R11, pt_regs, gprs[11]);
	OFFSET(__PT_R12, pt_regs, gprs[12]);
	OFFSET(__PT_R13, pt_regs, gprs[13]);
	OFFSET(__PT_R14, pt_regs, gprs[14]);
	OFFSET(__PT_R15, pt_regs, gprs[15]);
	OFFSET(__PT_ORIG_GPR2, pt_regs, orig_gpr2);
	OFFSET(__PT_FLAGS, pt_regs, flags);
	OFFSET(__PT_CR1, pt_regs, cr1);
	OFFSET(__PT_LAST_BREAK, pt_regs, last_break);
	DEFINE(__PT_SIZE, sizeof(struct pt_regs));
	BLANK();
	/* stack_frame offsets */
	OFFSET(__SF_BACKCHAIN, stack_frame, back_chain);
	OFFSET(__SF_GPRS, stack_frame, gprs);
	OFFSET(__SF_EMPTY, stack_frame, empty[0]);
	OFFSET(__SF_SIE_CONTROL, stack_frame, sie_control_block);
	OFFSET(__SF_SIE_SAVEAREA, stack_frame, sie_savearea);
	OFFSET(__SF_SIE_REASON, stack_frame, sie_reason);
	OFFSET(__SF_SIE_FLAGS, stack_frame, sie_flags);
	OFFSET(__SF_SIE_CONTROL_PHYS, stack_frame, sie_control_block_phys);
	DEFINE(STACK_FRAME_OVERHEAD, sizeof(struct stack_frame));
	BLANK();
	/* idle data offsets */
	OFFSET(__CLOCK_IDLE_ENTER, s390_idle_data, clock_idle_enter);
	OFFSET(__TIMER_IDLE_ENTER, s390_idle_data, timer_idle_enter);
	OFFSET(__MT_CYCLES_ENTER, s390_idle_data, mt_cycles_enter);
	BLANK();
	/* hardware defined lowcore locations 0x000 - 0x1ff */
	OFFSET(__LC_EXT_PARAMS, lowcore, ext_params);
	OFFSET(__LC_EXT_CPU_ADDR, lowcore, ext_cpu_addr);
	OFFSET(__LC_EXT_INT_CODE, lowcore, ext_int_code);
	OFFSET(__LC_PGM_ILC, lowcore, pgm_ilc);
	OFFSET(__LC_PGM_INT_CODE, lowcore, pgm_code);
	OFFSET(__LC_DATA_EXC_CODE, lowcore, data_exc_code);
	OFFSET(__LC_MON_CLASS_NR, lowcore, mon_class_num);
	OFFSET(__LC_PER_CODE, lowcore, per_code);
	OFFSET(__LC_PER_ATMID, lowcore, per_atmid);
	OFFSET(__LC_PER_ADDRESS, lowcore, per_address);
	OFFSET(__LC_EXC_ACCESS_ID, lowcore, exc_access_id);
	OFFSET(__LC_PER_ACCESS_ID, lowcore, per_access_id);
	OFFSET(__LC_OP_ACCESS_ID, lowcore, op_access_id);
	OFFSET(__LC_AR_MODE_ID, lowcore, ar_mode_id);
	OFFSET(__LC_TRANS_EXC_CODE, lowcore, trans_exc_code);
	OFFSET(__LC_MON_CODE, lowcore, monitor_code);
	OFFSET(__LC_SUBCHANNEL_ID, lowcore, subchannel_id);
	OFFSET(__LC_SUBCHANNEL_NR, lowcore, subchannel_nr);
	OFFSET(__LC_IO_INT_PARM, lowcore, io_int_parm);
	OFFSET(__LC_IO_INT_WORD, lowcore, io_int_word);
	OFFSET(__LC_MCCK_CODE, lowcore, mcck_interruption_code);
	OFFSET(__LC_EXT_DAMAGE_CODE, lowcore, external_damage_code);
	OFFSET(__LC_MCCK_FAIL_STOR_ADDR, lowcore, failing_storage_address);
	OFFSET(__LC_PGM_LAST_BREAK, lowcore, pgm_last_break);
	OFFSET(__LC_RETURN_LPSWE, lowcore, return_lpswe);
	OFFSET(__LC_RETURN_MCCK_LPSWE, lowcore, return_mcck_lpswe);
	OFFSET(__LC_RST_OLD_PSW, lowcore, restart_old_psw);
	OFFSET(__LC_EXT_OLD_PSW, lowcore, external_old_psw);
	OFFSET(__LC_SVC_OLD_PSW, lowcore, svc_old_psw);
	OFFSET(__LC_PGM_OLD_PSW, lowcore, program_old_psw);
	OFFSET(__LC_MCK_OLD_PSW, lowcore, mcck_old_psw);
	OFFSET(__LC_IO_OLD_PSW, lowcore, io_old_psw);
	OFFSET(__LC_RST_NEW_PSW, lowcore, restart_psw);
	OFFSET(__LC_EXT_NEW_PSW, lowcore, external_new_psw);
	OFFSET(__LC_SVC_NEW_PSW, lowcore, svc_new_psw);
	OFFSET(__LC_PGM_NEW_PSW, lowcore, program_new_psw);
	OFFSET(__LC_MCK_NEW_PSW, lowcore, mcck_new_psw);
	OFFSET(__LC_IO_NEW_PSW, lowcore, io_new_psw);
	/* software defined lowcore locations 0x200 - 0xdff*/
	OFFSET(__LC_SAVE_AREA_SYNC, lowcore, save_area_sync);
	OFFSET(__LC_SAVE_AREA_ASYNC, lowcore, save_area_async);
	OFFSET(__LC_SAVE_AREA_RESTART, lowcore, save_area_restart);
	OFFSET(__LC_CPU_FLAGS, lowcore, cpu_flags);
	OFFSET(__LC_RETURN_PSW, lowcore, return_psw);
	OFFSET(__LC_RETURN_MCCK_PSW, lowcore, return_mcck_psw);
	OFFSET(__LC_SYS_ENTER_TIMER, lowcore, sys_enter_timer);
	OFFSET(__LC_MCCK_ENTER_TIMER, lowcore, mcck_enter_timer);
	OFFSET(__LC_EXIT_TIMER, lowcore, exit_timer);
	OFFSET(__LC_LAST_UPDATE_TIMER, lowcore, last_update_timer);
	OFFSET(__LC_LAST_UPDATE_CLOCK, lowcore, last_update_clock);
	OFFSET(__LC_INT_CLOCK, lowcore, int_clock);
	OFFSET(__LC_BOOT_CLOCK, lowcore, boot_clock);
	OFFSET(__LC_CURRENT, lowcore, current_task);
	OFFSET(__LC_KERNEL_STACK, lowcore, kernel_stack);
	OFFSET(__LC_ASYNC_STACK, lowcore, async_stack);
	OFFSET(__LC_NODAT_STACK, lowcore, nodat_stack);
	OFFSET(__LC_RESTART_STACK, lowcore, restart_stack);
	OFFSET(__LC_MCCK_STACK, lowcore, mcck_stack);
	OFFSET(__LC_RESTART_FN, lowcore, restart_fn);
	OFFSET(__LC_RESTART_DATA, lowcore, restart_data);
	OFFSET(__LC_RESTART_SOURCE, lowcore, restart_source);
	OFFSET(__LC_RESTART_FLAGS, lowcore, restart_flags);
	OFFSET(__LC_KERNEL_ASCE, lowcore, kernel_asce);
	OFFSET(__LC_USER_ASCE, lowcore, user_asce);
	OFFSET(__LC_LPP, lowcore, lpp);
	OFFSET(__LC_CURRENT_PID, lowcore, current_pid);
	OFFSET(__LC_GMAP, lowcore, gmap);
	OFFSET(__LC_LAST_BREAK, lowcore, last_break);
	/* software defined ABI-relevant lowcore locations 0xe00 - 0xe20 */
	OFFSET(__LC_DUMP_REIPL, lowcore, ipib);
	OFFSET(__LC_VMCORE_INFO, lowcore, vmcore_info);
	OFFSET(__LC_OS_INFO, lowcore, os_info);
	/* hardware defined lowcore locations 0x1000 - 0x18ff */
	OFFSET(__LC_MCESAD, lowcore, mcesad);
	OFFSET(__LC_EXT_PARAMS2, lowcore, ext_params2);
	OFFSET(__LC_FPREGS_SAVE_AREA, lowcore, floating_pt_save_area);
	OFFSET(__LC_GPREGS_SAVE_AREA, lowcore, gpregs_save_area);
	OFFSET(__LC_PSW_SAVE_AREA, lowcore, psw_save_area);
	OFFSET(__LC_PREFIX_SAVE_AREA, lowcore, prefixreg_save_area);
	OFFSET(__LC_FP_CREG_SAVE_AREA, lowcore, fpt_creg_save_area);
	OFFSET(__LC_TOD_PROGREG_SAVE_AREA, lowcore, tod_progreg_save_area);
	OFFSET(__LC_CPU_TIMER_SAVE_AREA, lowcore, cpu_timer_save_area);
	OFFSET(__LC_CLOCK_COMP_SAVE_AREA, lowcore, clock_comp_save_area);
	OFFSET(__LC_LAST_BREAK_SAVE_AREA, lowcore, last_break_save_area);
	OFFSET(__LC_AREGS_SAVE_AREA, lowcore, access_regs_save_area);
	OFFSET(__LC_CREGS_SAVE_AREA, lowcore, cregs_save_area);
	OFFSET(__LC_PGM_TDB, lowcore, pgm_tdb);
	BLANK();
	/* gmap/sie offsets */
	OFFSET(__GMAP_ASCE, gmap, asce);
	OFFSET(__SIE_PROG0C, kvm_s390_sie_block, prog0c);
	OFFSET(__SIE_PROG20, kvm_s390_sie_block, prog20);
	/* kexec_sha_region */
	OFFSET(__KEXEC_SHA_REGION_START, kexec_sha_region, start);
	OFFSET(__KEXEC_SHA_REGION_LEN, kexec_sha_region, len);
	DEFINE(__KEXEC_SHA_REGION_SIZE, sizeof(struct kexec_sha_region));
	/* sizeof kernel parameter area */
	DEFINE(__PARMAREA_SIZE, sizeof(struct parmarea));
	/* kernel parameter area offsets */
	DEFINE(IPL_DEVICE, PARMAREA + offsetof(struct parmarea, ipl_device));
	DEFINE(INITRD_START, PARMAREA + offsetof(struct parmarea, initrd_start));
	DEFINE(INITRD_SIZE, PARMAREA + offsetof(struct parmarea, initrd_size));
	DEFINE(OLDMEM_BASE, PARMAREA + offsetof(struct parmarea, oldmem_base));
	DEFINE(OLDMEM_SIZE, PARMAREA + offsetof(struct parmarea, oldmem_size));
	DEFINE(COMMAND_LINE, PARMAREA + offsetof(struct parmarea, command_line));
	DEFINE(MAX_COMMAND_LINE_SIZE, PARMAREA + offsetof(struct parmarea, max_command_line_size));
#ifdef CONFIG_FUNCTION_GRAPH_TRACER
	/* function graph return value tracing */
	OFFSET(__FGRAPH_RET_GPR2, fgraph_ret_regs, gpr2);
	OFFSET(__FGRAPH_RET_FP, fgraph_ret_regs, fp);
	DEFINE(__FGRAPH_RET_SIZE, sizeof(struct fgraph_ret_regs));
#endif
	OFFSET(__FTRACE_REGS_PT_REGS, ftrace_regs, regs);
	DEFINE(__FTRACE_REGS_SIZE, sizeof(struct ftrace_regs));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
