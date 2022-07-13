<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __ARCH_ARM_FAULT_H
#define __ARCH_ARM_FAULT_H

/*
 * Fault status register encodings.  We steal bit 31 for our own purposes.
 */
#define FSR_LNX_PF		(1 << 31)
<<<<<<< HEAD
=======
#define FSR_CM			(1 << 13)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define FSR_WRITE		(1 << 11)
#define FSR_FS4			(1 << 10)
#define FSR_FS3_0		(15)
#define FSR_FS5_0		(0x3f)

#ifdef CONFIG_ARM_LPAE
<<<<<<< HEAD
=======
#define FSR_FS_AEA		17
#define FS_TRANS_NOLL		0x4
#define FS_PERM_NOLL		0xC
#define FS_MMU_NOLL_MASK	0x3C

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int fsr_fs(unsigned int fsr)
{
	return fsr & FSR_FS5_0;
}
#else
<<<<<<< HEAD
=======
#define FSR_FS_AEA		22
#define FS_L1_TRANS		0x5
#define FS_L2_TRANS		0x7
#define FS_L1_PERM		0xD
#define FS_L2_PERM		0xF

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int fsr_fs(unsigned int fsr)
{
	return (fsr & FSR_FS3_0) | (fsr & FSR_FS4) >> 6;
}
#endif

void do_bad_area(unsigned long addr, unsigned int fsr, struct pt_regs *regs);
<<<<<<< HEAD
unsigned long search_exception_table(unsigned long addr);
=======
void early_abt_enable(void);
asmlinkage void do_DataAbort(unsigned long addr, unsigned int fsr,
			     struct pt_regs *regs);
asmlinkage void do_PrefetchAbort(unsigned long addr, unsigned int ifsr,
				 struct pt_regs *regs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif	/* __ARCH_ARM_FAULT_H */
