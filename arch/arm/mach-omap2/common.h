/*
 * Header for code common to all OMAP2+ machines.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * You should have received a copy of the  GNU General Public License along
 * with this program; if not, write  to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __ARCH_ARM_MACH_OMAP2PLUS_COMMON_H
#define __ARCH_ARM_MACH_OMAP2PLUS_COMMON_H
#ifndef __ASSEMBLER__

<<<<<<< HEAD
#include <linux/delay.h>
#include <plat/common.h>
#include <asm/proc-fns.h>

#ifdef CONFIG_SOC_OMAP2420
extern void omap242x_map_common_io(void);
#else
static inline void omap242x_map_common_io(void)
=======
#include <linux/irq.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/mfd/twl.h>
#include <linux/platform_data/i2c-omap.h>
#include <linux/reboot.h>
#include <linux/irqchip/irq-omap-intc.h>

#include <asm/proc-fns.h>
#include <asm/hardware/cache-l2x0.h>

#include "i2c.h"

#define OMAP_INTC_START		NR_IRQS

extern int (*omap_pm_soc_init)(void);
int omap_pm_nop_init(void);

#if defined(CONFIG_PM) && defined(CONFIG_ARCH_OMAP3)
int omap3_pm_init(void);
#else
static inline int omap3_pm_init(void)
{
	return 0;
}
#endif

#if defined(CONFIG_PM) && (defined(CONFIG_ARCH_OMAP4) || defined(CONFIG_SOC_OMAP5) || defined(CONFIG_SOC_DRA7XX))
int omap4_pm_init(void);
int omap4_pm_init_early(void);
#else
static inline int omap4_pm_init(void)
{
	return 0;
}

static inline int omap4_pm_init_early(void)
{
	return 0;
}
#endif

#if defined(CONFIG_PM) && (defined(CONFIG_SOC_AM33XX) || \
	defined(CONFIG_SOC_AM43XX))
int amx3_common_pm_init(void);
#else
static inline int amx3_common_pm_init(void)
{
	return 0;
}
#endif

#ifdef CONFIG_CACHE_L2X0
int omap_l2_cache_init(void);
#define OMAP_L2C_AUX_CTRL	(L2C_AUX_CTRL_SHARED_OVERRIDE | \
				 L310_AUX_CTRL_DATA_PREFETCH | \
				 L310_AUX_CTRL_INSTR_PREFETCH)
void omap4_l2c310_write_sec(unsigned long val, unsigned reg);
#else
static inline int omap_l2_cache_init(void)
{
	return 0;
}

#define OMAP_L2C_AUX_CTRL	0
#define omap4_l2c310_write_sec	NULL
#endif

#ifdef CONFIG_SOC_HAS_REALTIME_COUNTER
extern void omap5_realtime_timer_init(void);
#else
static inline void omap5_realtime_timer_init(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
}
#endif

<<<<<<< HEAD
#ifdef CONFIG_SOC_OMAP2430
extern void omap243x_map_common_io(void);
#else
static inline void omap243x_map_common_io(void)
=======
void omap2420_init_early(void);
void omap2430_init_early(void);
void omap3430_init_early(void);
void omap3630_init_early(void);
void am33xx_init_early(void);
void am35xx_init_early(void);
void ti814x_init_early(void);
void ti816x_init_early(void);
void am43xx_init_early(void);
void am43xx_init_late(void);
void omap4430_init_early(void);
void omap5_init_early(void);
void omap3_init_late(void);
void omap4430_init_late(void);
void ti81xx_init_late(void);
void am33xx_init_late(void);
void omap5_init_late(void);
void dra7xx_init_early(void);
void dra7xx_init_late(void);

#ifdef CONFIG_SOC_BUS
void omap_soc_device_init(void);
#else
static inline void omap_soc_device_init(void)
{
}
#endif

#if defined(CONFIG_SOC_OMAP2420) || defined(CONFIG_SOC_OMAP2430)
void omap2xxx_restart(enum reboot_mode mode, const char *cmd);
#else
static inline void omap2xxx_restart(enum reboot_mode mode, const char *cmd)
{
}
#endif

#ifdef CONFIG_SOC_AM33XX
void am33xx_restart(enum reboot_mode mode, const char *cmd);
#else
static inline void am33xx_restart(enum reboot_mode mode, const char *cmd)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
}
#endif

#ifdef CONFIG_ARCH_OMAP3
<<<<<<< HEAD
extern void omap34xx_map_common_io(void);
#else
static inline void omap34xx_map_common_io(void)
=======
void omap3xxx_restart(enum reboot_mode mode, const char *cmd);
#else
static inline void omap3xxx_restart(enum reboot_mode mode, const char *cmd)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
}
#endif

<<<<<<< HEAD
#ifdef CONFIG_SOC_OMAPTI81XX
extern void omapti81xx_map_common_io(void);
#else
static inline void omapti81xx_map_common_io(void)
=======
#ifdef CONFIG_SOC_TI81XX
void ti81xx_restart(enum reboot_mode mode, const char *cmd);
#else
static inline void ti81xx_restart(enum reboot_mode mode, const char *cmd)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
}
#endif

<<<<<<< HEAD
#ifdef CONFIG_SOC_OMAPAM33XX
extern void omapam33xx_map_common_io(void);
#else
static inline void omapam33xx_map_common_io(void)
=======
#if defined(CONFIG_ARCH_OMAP4) || defined(CONFIG_SOC_OMAP5) || \
	defined(CONFIG_SOC_DRA7XX) || defined(CONFIG_SOC_AM43XX)
void omap44xx_restart(enum reboot_mode mode, const char *cmd);
#else
static inline void omap44xx_restart(enum reboot_mode mode, const char *cmd)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
}
#endif

<<<<<<< HEAD
#ifdef CONFIG_ARCH_OMAP4
extern void omap44xx_map_common_io(void);
#else
static inline void omap44xx_map_common_io(void)
{
}
#endif

extern void omap2_init_common_infrastructure(void);

extern struct sys_timer omap2_timer;
extern struct sys_timer omap3_timer;
extern struct sys_timer omap3_secure_timer;
extern struct sys_timer omap4_timer;

void omap2420_init_early(void);
void omap2430_init_early(void);
void omap3430_init_early(void);
void omap35xx_init_early(void);
void omap3630_init_early(void);
void omap3_init_early(void);	/* Do not use this one */
void am35xx_init_early(void);
void ti81xx_init_early(void);
void omap4430_init_early(void);
void omap_prcm_restart(char, const char *);

/*
 * IO bases for various OMAP processors
 * Except the tap base, rest all the io bases
 * listed are physical addresses.
 */
struct omap_globals {
	u32		class;		/* OMAP class to detect */
	void __iomem	*tap;		/* Control module ID code */
	void __iomem	*sdrc;           /* SDRAM Controller */
	void __iomem	*sms;            /* SDRAM Memory Scheduler */
	void __iomem	*ctrl;           /* System Control Module */
	void __iomem	*ctrl_pad;	/* PAD Control Module */
	void __iomem	*prm;            /* Power and Reset Management */
	void __iomem	*cm;             /* Clock Management */
	void __iomem	*cm2;
};

void omap2_set_globals_242x(void);
void omap2_set_globals_243x(void);
void omap2_set_globals_3xxx(void);
void omap2_set_globals_443x(void);
void omap2_set_globals_ti81xx(void);
void omap2_set_globals_am33xx(void);

/* These get called from omap2_set_globals_xxxx(), do not call these */
void omap2_set_globals_tap(struct omap_globals *);
void omap2_set_globals_sdrc(struct omap_globals *);
void omap2_set_globals_control(struct omap_globals *);
void omap2_set_globals_prcm(struct omap_globals *);

void omap242x_map_io(void);
void omap243x_map_io(void);
void omap3_map_io(void);
void am33xx_map_io(void);
void omap4_map_io(void);
void ti81xx_map_io(void);
void omap_barriers_init(void);

extern void __init omap_init_consistent_dma_size(void);
=======
#ifdef CONFIG_OMAP_INTERCONNECT_BARRIER
void omap_barrier_reserve_memblock(void);
void omap_barriers_init(void);
#else
static inline void omap_barrier_reserve_memblock(void)
{
}
#endif

/* This gets called from mach-omap2/io.c, do not call this */
void __init omap2_set_globals_tap(u32 class, void __iomem *tap);

void __init omap242x_map_io(void);
void __init omap243x_map_io(void);
void __init omap3_map_io(void);
void __init am33xx_map_io(void);
void __init omap4_map_io(void);
void __init omap5_map_io(void);
void __init dra7xx_map_io(void);
void __init ti81xx_map_io(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * omap_test_timeout - busy-loop, testing a condition
 * @cond: condition to test until it evaluates to true
 * @timeout: maximum number of microseconds in the timeout
 * @index: loop index (integer)
 *
 * Loop waiting for @cond to become true or until at least @timeout
 * microseconds have passed.  To use, define some integer @index in the
 * calling code.  After running, if @index == @timeout, then the loop has
 * timed out.
 */
#define omap_test_timeout(cond, timeout, index)			\
({								\
	for (index = 0; index < timeout; index++) {		\
		if (cond)					\
			break;					\
		udelay(1);					\
	}							\
})

<<<<<<< HEAD
extern struct device *omap2_get_mpuss_device(void);
extern struct device *omap2_get_iva_device(void);
extern struct device *omap2_get_l3_device(void);
extern struct device *omap4_get_dsp_device(void);

void omap2_init_irq(void);
void omap3_init_irq(void);
void ti81xx_init_irq(void);
extern int omap_irq_pending(void);
void omap_intc_save_context(void);
void omap_intc_restore_context(void);
void omap3_intc_suspend(void);
void omap3_intc_prepare_idle(void);
void omap3_intc_resume_idle(void);
void omap2_intc_handle_irq(struct pt_regs *regs);
void omap3_intc_handle_irq(struct pt_regs *regs);
=======
void omap_gic_of_init(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_CACHE_L2X0
extern void __iomem *omap4_get_l2cache_base(void);
#endif

struct device_node;
<<<<<<< HEAD
#ifdef CONFIG_OF
int __init omap_intc_of_init(struct device_node *node,
			     struct device_node *parent);
#else
int __init omap_intc_of_init(struct device_node *node,
			     struct device_node *parent)
{
	return 0;
}
#endif
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_SMP
extern void __iomem *omap4_get_scu_base(void);
#else
static inline void __iomem *omap4_get_scu_base(void)
{
	return NULL;
}
#endif

<<<<<<< HEAD
extern void __init gic_init_irq(void);
extern void omap_smc1(u32 fn, u32 arg);
extern void __iomem *omap4_get_sar_ram_base(void);
extern void omap_do_wfi(void);

#ifdef CONFIG_SMP
/* Needed for secondary core boot */
extern void omap_secondary_startup(void);
extern u32 omap_modify_auxcoreboot0(u32 set_mask, u32 clear_mask);
extern void omap_auxcoreboot_addr(u32 cpu_addr);
extern u32 omap_read_auxcoreboot0(void);
#endif

#if defined(CONFIG_SMP) && defined(CONFIG_PM)
extern int omap4_mpuss_init(void);
extern int omap4_enter_lowpower(unsigned int cpu, unsigned int power_state);
extern int omap4_finish_suspend(unsigned long cpu_state);
extern void omap4_cpu_resume(void);
extern int omap4_hotplug_cpu(unsigned int cpu, unsigned int power_state);
extern u32 omap4_mpuss_read_prev_context_state(void);
#else
static inline int omap4_enter_lowpower(unsigned int cpu,
					unsigned int power_state)
=======
extern void gic_dist_disable(void);
extern void gic_dist_enable(void);
extern bool gic_dist_disabled(void);
extern void gic_timer_retrigger(void);
extern void _omap_smc1(u32 fn, u32 arg);
extern void omap4_sar_ram_init(void);
extern void __iomem *omap4_get_sar_ram_base(void);
extern void omap4_mpuss_early_init(void);
extern void omap_do_wfi(void);
extern void omap_interconnect_sync(void);

#ifdef CONFIG_SMP
/* Needed for secondary core boot */
extern u32 omap_modify_auxcoreboot0(u32 set_mask, u32 clear_mask);
extern void omap_auxcoreboot_addr(u32 cpu_addr);
extern u32 omap_read_auxcoreboot0(void);

extern void omap4_cpu_die(unsigned int cpu);
extern int omap4_cpu_kill(unsigned int cpu);

extern const struct smp_operations omap4_smp_ops;
#endif

extern u32 omap4_get_cpu1_ns_pa_addr(void);

#if defined(CONFIG_SMP) && defined(CONFIG_PM)
extern int omap4_mpuss_init(void);
extern int omap4_enter_lowpower(unsigned int cpu, unsigned int power_state,
				bool rcuidle);
extern int omap4_hotplug_cpu(unsigned int cpu, unsigned int power_state);
#else
static inline int omap4_enter_lowpower(unsigned int cpu,
					unsigned int power_state,
					bool rcuidle)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	cpu_do_idle();
	return 0;
}

static inline int omap4_hotplug_cpu(unsigned int cpu, unsigned int power_state)
{
	cpu_do_idle();
	return 0;
}

static inline int omap4_mpuss_init(void)
{
	return 0;
}

<<<<<<< HEAD
=======
#endif

#ifdef CONFIG_ARCH_OMAP4
void omap4_secondary_startup(void);
void omap4460_secondary_startup(void);
int omap4_finish_suspend(unsigned long cpu_state);
void omap4_cpu_resume(void);
#else
static inline void omap4_secondary_startup(void)
{
}

static inline void omap4460_secondary_startup(void)
{
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int omap4_finish_suspend(unsigned long cpu_state)
{
	return 0;
}
<<<<<<< HEAD

static inline void omap4_cpu_resume(void)
{}

static inline u32 omap4_mpuss_read_prev_context_state(void)
=======
static inline void omap4_cpu_resume(void)
{
}
#endif

#if defined(CONFIG_SOC_OMAP5) || defined(CONFIG_SOC_DRA7XX)
void omap5_secondary_startup(void);
void omap5_secondary_hyp_startup(void);
#else
static inline void omap5_secondary_startup(void)
{
}

static inline void omap5_secondary_hyp_startup(void)
{
}
#endif

struct omap_system_dma_plat_info;

void pdata_quirks_init(const struct of_device_id *);
void omap_auxdata_legacy_init(struct device *dev);
void omap_pcs_legacy_init(int irq, void (*rearm)(void));
extern struct omap_system_dma_plat_info dma_plat_info;

struct omap_sdrc_params;
extern void omap_sdrc_init(struct omap_sdrc_params *sdrc_cs0,
				      struct omap_sdrc_params *sdrc_cs1);
extern void omap_reserve(void);

struct omap_hwmod;
extern int omap_dss_reset(struct omap_hwmod *);

/* SoC specific clock initializer */
int omap_clk_init(void);

#if IS_ENABLED(CONFIG_OMAP_IOMMU)
int omap_iommu_set_pwrdm_constraint(struct platform_device *pdev, bool request,
				    u8 *pwrst);
#else
static inline int omap_iommu_set_pwrdm_constraint(struct platform_device *pdev,
						  bool request, u8 *pwrst)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return 0;
}
#endif

<<<<<<< HEAD
struct omap_sdrc_params;
extern void omap_sdrc_init(struct omap_sdrc_params *sdrc_cs0,
				      struct omap_sdrc_params *sdrc_cs1);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __ASSEMBLER__ */
#endif /* __ARCH_ARM_MACH_OMAP2PLUS_COMMON_H */
