<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Generate definitions needed by assembly language modules.
 * This code generates raw asm output which is post-processed to extract
 * and format the required data.
 */
#define COMPILE_OFFSETS

#include <linux/crypto.h>
<<<<<<< HEAD
=======
#include <crypto/aria.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/sched.h>
#include <linux/stddef.h>
#include <linux/hardirq.h>
#include <linux/suspend.h>
#include <linux/kbuild.h>
#include <asm/processor.h>
#include <asm/thread_info.h>
#include <asm/sigframe.h>
#include <asm/bootparam.h>
#include <asm/suspend.h>
<<<<<<< HEAD
=======
#include <asm/tlbflush.h>
#include <asm/tdx.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_XEN
#include <xen/interface/xen.h>
#endif

#ifdef CONFIG_X86_32
# include "asm-offsets_32.c"
#else
# include "asm-offsets_64.c"
#endif

<<<<<<< HEAD
void common(void) {
	BLANK();
	OFFSET(TI_flags, thread_info, flags);
	OFFSET(TI_status, thread_info, status);
	OFFSET(TI_addr_limit, thread_info, addr_limit);
	OFFSET(TI_preempt_count, thread_info, preempt_count);

	BLANK();
	OFFSET(crypto_tfm_ctx_offset, crypto_tfm, __crt_ctx);
=======
static void __used common(void)
{
	BLANK();
	OFFSET(TASK_threadsp, task_struct, thread.sp);
#ifdef CONFIG_STACKPROTECTOR
	OFFSET(TASK_stack_canary, task_struct, stack_canary);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	BLANK();
	OFFSET(pbe_address, pbe, address);
	OFFSET(pbe_orig_address, pbe, orig_address);
	OFFSET(pbe_next, pbe, next);

<<<<<<< HEAD
#ifdef CONFIG_PARAVIRT
	BLANK();
	OFFSET(PARAVIRT_enabled, pv_info, paravirt_enabled);
	OFFSET(PARAVIRT_PATCH_pv_cpu_ops, paravirt_patch_template, pv_cpu_ops);
	OFFSET(PARAVIRT_PATCH_pv_irq_ops, paravirt_patch_template, pv_irq_ops);
	OFFSET(PV_IRQ_irq_disable, pv_irq_ops, irq_disable);
	OFFSET(PV_IRQ_irq_enable, pv_irq_ops, irq_enable);
	OFFSET(PV_CPU_iret, pv_cpu_ops, iret);
	OFFSET(PV_CPU_irq_enable_sysexit, pv_cpu_ops, irq_enable_sysexit);
	OFFSET(PV_CPU_read_cr0, pv_cpu_ops, read_cr0);
	OFFSET(PV_MMU_read_cr2, pv_mmu_ops, read_cr2);
=======
#if defined(CONFIG_X86_32) || defined(CONFIG_IA32_EMULATION)
	BLANK();
	OFFSET(IA32_SIGCONTEXT_ax, sigcontext_32, ax);
	OFFSET(IA32_SIGCONTEXT_bx, sigcontext_32, bx);
	OFFSET(IA32_SIGCONTEXT_cx, sigcontext_32, cx);
	OFFSET(IA32_SIGCONTEXT_dx, sigcontext_32, dx);
	OFFSET(IA32_SIGCONTEXT_si, sigcontext_32, si);
	OFFSET(IA32_SIGCONTEXT_di, sigcontext_32, di);
	OFFSET(IA32_SIGCONTEXT_bp, sigcontext_32, bp);
	OFFSET(IA32_SIGCONTEXT_sp, sigcontext_32, sp);
	OFFSET(IA32_SIGCONTEXT_ip, sigcontext_32, ip);

	BLANK();
	OFFSET(IA32_RT_SIGFRAME_sigcontext, rt_sigframe_ia32, uc.uc_mcontext);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#ifdef CONFIG_XEN
	BLANK();
	OFFSET(XEN_vcpu_info_mask, vcpu_info, evtchn_upcall_mask);
	OFFSET(XEN_vcpu_info_pending, vcpu_info, evtchn_upcall_pending);
<<<<<<< HEAD
#endif

	BLANK();
	OFFSET(BP_scratch, boot_params, scratch);
=======
	OFFSET(XEN_vcpu_info_arch_cr2, vcpu_info, arch.cr2);
#endif

	BLANK();
	OFFSET(TDX_MODULE_rcx, tdx_module_args, rcx);
	OFFSET(TDX_MODULE_rdx, tdx_module_args, rdx);
	OFFSET(TDX_MODULE_r8,  tdx_module_args, r8);
	OFFSET(TDX_MODULE_r9,  tdx_module_args, r9);
	OFFSET(TDX_MODULE_r10, tdx_module_args, r10);
	OFFSET(TDX_MODULE_r11, tdx_module_args, r11);
	OFFSET(TDX_MODULE_r12, tdx_module_args, r12);
	OFFSET(TDX_MODULE_r13, tdx_module_args, r13);
	OFFSET(TDX_MODULE_r14, tdx_module_args, r14);
	OFFSET(TDX_MODULE_r15, tdx_module_args, r15);
	OFFSET(TDX_MODULE_rbx, tdx_module_args, rbx);
	OFFSET(TDX_MODULE_rdi, tdx_module_args, rdi);
	OFFSET(TDX_MODULE_rsi, tdx_module_args, rsi);

	BLANK();
	OFFSET(BP_scratch, boot_params, scratch);
	OFFSET(BP_secure_boot, boot_params, secure_boot);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	OFFSET(BP_loadflags, boot_params, hdr.loadflags);
	OFFSET(BP_hardware_subarch, boot_params, hdr.hardware_subarch);
	OFFSET(BP_version, boot_params, hdr.version);
	OFFSET(BP_kernel_alignment, boot_params, hdr.kernel_alignment);
<<<<<<< HEAD
	OFFSET(BP_pref_address, boot_params, hdr.pref_address);
	OFFSET(BP_code32_start, boot_params, hdr.code32_start);
=======
	OFFSET(BP_init_size, boot_params, hdr.init_size);
	OFFSET(BP_pref_address, boot_params, hdr.pref_address);

	BLANK();
	DEFINE(PTREGS_SIZE, sizeof(struct pt_regs));

	/* TLB state for the entry code */
	OFFSET(TLB_STATE_user_pcid_flush_mask, tlb_state, user_pcid_flush_mask);

	/* Layout info for cpu_entry_area */
	OFFSET(CPU_ENTRY_AREA_entry_stack, cpu_entry_area, entry_stack_page);
	DEFINE(SIZEOF_entry_stack, sizeof(struct entry_stack));
	DEFINE(MASK_entry_stack, (~(sizeof(struct entry_stack) - 1)));

	/* Offset for fields in tss_struct */
	OFFSET(TSS_sp0, tss_struct, x86_tss.sp0);
	OFFSET(TSS_sp1, tss_struct, x86_tss.sp1);
	OFFSET(TSS_sp2, tss_struct, x86_tss.sp2);
	OFFSET(X86_top_of_stack, pcpu_hot, top_of_stack);
	OFFSET(X86_current_task, pcpu_hot, current_task);
#ifdef CONFIG_MITIGATION_CALL_DEPTH_TRACKING
	OFFSET(X86_call_depth, pcpu_hot, call_depth);
#endif
#if IS_ENABLED(CONFIG_CRYPTO_ARIA_AESNI_AVX_X86_64)
	/* Offset for fields in aria_ctx */
	BLANK();
	OFFSET(ARIA_CTX_enc_key, aria_ctx, enc_key);
	OFFSET(ARIA_CTX_dec_key, aria_ctx, dec_key);
	OFFSET(ARIA_CTX_rounds, aria_ctx, rounds);
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
