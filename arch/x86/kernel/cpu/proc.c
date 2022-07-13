<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/smp.h>
#include <linux/timex.h>
#include <linux/string.h>
#include <linux/seq_file.h>
#include <linux/cpufreq.h>
<<<<<<< HEAD
=======
#include <asm/prctl.h>
#include <linux/proc_fs.h>

#include "cpu.h"

#ifdef CONFIG_X86_VMX_FEATURE_NAMES
extern const char * const x86_vmx_flags[NVMXINTS*32];
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 *	Get CPU information for use by the procfs.
 */
static void show_cpuinfo_core(struct seq_file *m, struct cpuinfo_x86 *c,
			      unsigned int cpu)
{
#ifdef CONFIG_SMP
<<<<<<< HEAD
	if (c->x86_max_cores * smp_num_siblings > 1) {
		seq_printf(m, "physical id\t: %d\n", c->phys_proc_id);
		seq_printf(m, "siblings\t: %d\n",
			   cpumask_weight(cpu_core_mask(cpu)));
		seq_printf(m, "core id\t\t: %d\n", c->cpu_core_id);
		seq_printf(m, "cpu cores\t: %d\n", c->booted_cores);
		seq_printf(m, "apicid\t\t: %d\n", c->apicid);
		seq_printf(m, "initial apicid\t: %d\n", c->initial_apicid);
	}
=======
	seq_printf(m, "physical id\t: %d\n", c->topo.pkg_id);
	seq_printf(m, "siblings\t: %d\n",
		   cpumask_weight(topology_core_cpumask(cpu)));
	seq_printf(m, "core id\t\t: %d\n", c->topo.core_id);
	seq_printf(m, "cpu cores\t: %d\n", c->booted_cores);
	seq_printf(m, "apicid\t\t: %d\n", c->topo.apicid);
	seq_printf(m, "initial apicid\t: %d\n", c->topo.initial_apicid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
}

#ifdef CONFIG_X86_32
static void show_cpuinfo_misc(struct seq_file *m, struct cpuinfo_x86 *c)
{
<<<<<<< HEAD
	/*
	 * We use exception 16 if we have hardware math and we've either seen
	 * it or the CPU claims it is internal
	 */
	int fpu_exception = c->hard_math && (ignore_fpu_irq || cpu_has_fpu);
	seq_printf(m,
		   "fdiv_bug\t: %s\n"
		   "hlt_bug\t\t: %s\n"
=======
	seq_printf(m,
		   "fdiv_bug\t: %s\n"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		   "f00f_bug\t: %s\n"
		   "coma_bug\t: %s\n"
		   "fpu\t\t: %s\n"
		   "fpu_exception\t: %s\n"
		   "cpuid level\t: %d\n"
<<<<<<< HEAD
		   "wp\t\t: %s\n",
		   c->fdiv_bug ? "yes" : "no",
		   c->hlt_works_ok ? "no" : "yes",
		   c->f00f_bug ? "yes" : "no",
		   c->coma_bug ? "yes" : "no",
		   c->hard_math ? "yes" : "no",
		   fpu_exception ? "yes" : "no",
		   c->cpuid_level,
		   c->wp_works_ok ? "yes" : "no");
=======
		   "wp\t\t: yes\n",
		   boot_cpu_has_bug(X86_BUG_FDIV) ? "yes" : "no",
		   boot_cpu_has_bug(X86_BUG_F00F) ? "yes" : "no",
		   boot_cpu_has_bug(X86_BUG_COMA) ? "yes" : "no",
		   boot_cpu_has(X86_FEATURE_FPU) ? "yes" : "no",
		   boot_cpu_has(X86_FEATURE_FPU) ? "yes" : "no",
		   c->cpuid_level);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#else
static void show_cpuinfo_misc(struct seq_file *m, struct cpuinfo_x86 *c)
{
	seq_printf(m,
		   "fpu\t\t: yes\n"
		   "fpu_exception\t: yes\n"
		   "cpuid level\t: %d\n"
		   "wp\t\t: yes\n",
		   c->cpuid_level);
}
#endif

static int show_cpuinfo(struct seq_file *m, void *v)
{
	struct cpuinfo_x86 *c = v;
	unsigned int cpu;
	int i;

	cpu = c->cpu_index;
	seq_printf(m, "processor\t: %u\n"
		   "vendor_id\t: %s\n"
		   "cpu family\t: %d\n"
		   "model\t\t: %u\n"
		   "model name\t: %s\n",
		   cpu,
		   c->x86_vendor_id[0] ? c->x86_vendor_id : "unknown",
		   c->x86,
		   c->x86_model,
		   c->x86_model_id[0] ? c->x86_model_id : "unknown");

<<<<<<< HEAD
	if (c->x86_mask || c->cpuid_level >= 0)
		seq_printf(m, "stepping\t: %d\n", c->x86_mask);
	else
		seq_printf(m, "stepping\t: unknown\n");
=======
	if (c->x86_stepping || c->cpuid_level >= 0)
		seq_printf(m, "stepping\t: %d\n", c->x86_stepping);
	else
		seq_puts(m, "stepping\t: unknown\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (c->microcode)
		seq_printf(m, "microcode\t: 0x%x\n", c->microcode);

	if (cpu_has(c, X86_FEATURE_TSC)) {
<<<<<<< HEAD
		unsigned int freq = cpufreq_quick_get(cpu);

		if (!freq)
			freq = cpu_khz;
		seq_printf(m, "cpu MHz\t\t: %u.%03u\n",
			   freq / 1000, (freq % 1000));
	}

	/* Cache size */
	if (c->x86_cache_size >= 0)
		seq_printf(m, "cache size\t: %d KB\n", c->x86_cache_size);
=======
		unsigned int freq = arch_freq_get_on_cpu(cpu);

		seq_printf(m, "cpu MHz\t\t: %u.%03u\n", freq / 1000, (freq % 1000));
	}

	/* Cache size */
	if (c->x86_cache_size)
		seq_printf(m, "cache size\t: %u KB\n", c->x86_cache_size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	show_cpuinfo_core(m, c, cpu);
	show_cpuinfo_misc(m, c);

<<<<<<< HEAD
	seq_printf(m, "flags\t\t:");
=======
	seq_puts(m, "flags\t\t:");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (i = 0; i < 32*NCAPINTS; i++)
		if (cpu_has(c, i) && x86_cap_flags[i] != NULL)
			seq_printf(m, " %s", x86_cap_flags[i]);

<<<<<<< HEAD
=======
#ifdef CONFIG_X86_VMX_FEATURE_NAMES
	if (cpu_has(c, X86_FEATURE_VMX) && c->vmx_capability[0]) {
		seq_puts(m, "\nvmx flags\t:");
		for (i = 0; i < 32*NVMXINTS; i++) {
			if (test_bit(i, (unsigned long *)c->vmx_capability) &&
			    x86_vmx_flags[i] != NULL)
				seq_printf(m, " %s", x86_vmx_flags[i]);
		}
	}
#endif

	seq_puts(m, "\nbugs\t\t:");
	for (i = 0; i < 32*NBUGINTS; i++) {
		unsigned int bug_bit = 32*NCAPINTS + i;

		if (cpu_has_bug(c, bug_bit) && x86_bug_flags[i])
			seq_printf(m, " %s", x86_bug_flags[i]);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	seq_printf(m, "\nbogomips\t: %lu.%02lu\n",
		   c->loops_per_jiffy/(500000/HZ),
		   (c->loops_per_jiffy/(5000/HZ)) % 100);

#ifdef CONFIG_X86_64
	if (c->x86_tlbsize > 0)
		seq_printf(m, "TLB size\t: %d 4K pages\n", c->x86_tlbsize);
#endif
	seq_printf(m, "clflush size\t: %u\n", c->x86_clflush_size);
	seq_printf(m, "cache_alignment\t: %d\n", c->x86_cache_alignment);
	seq_printf(m, "address sizes\t: %u bits physical, %u bits virtual\n",
		   c->x86_phys_bits, c->x86_virt_bits);

<<<<<<< HEAD
	seq_printf(m, "power management:");
=======
	seq_puts(m, "power management:");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (i = 0; i < 32; i++) {
		if (c->x86_power & (1 << i)) {
			if (i < ARRAY_SIZE(x86_power_flags) &&
			    x86_power_flags[i])
				seq_printf(m, "%s%s",
					   x86_power_flags[i][0] ? " " : "",
					   x86_power_flags[i]);
			else
				seq_printf(m, " [%d]", i);
		}
	}

<<<<<<< HEAD
	seq_printf(m, "\n\n");
=======
	seq_puts(m, "\n\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static void *c_start(struct seq_file *m, loff_t *pos)
{
<<<<<<< HEAD
	if (*pos == 0)	/* just in case, cpu 0 is not the first */
		*pos = cpumask_first(cpu_online_mask);
	else
		*pos = cpumask_next(*pos - 1, cpu_online_mask);
=======
	*pos = cpumask_next(*pos - 1, cpu_online_mask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if ((*pos) < nr_cpu_ids)
		return &cpu_data(*pos);
	return NULL;
}

static void *c_next(struct seq_file *m, void *v, loff_t *pos)
{
	(*pos)++;
	return c_start(m, pos);
}

static void c_stop(struct seq_file *m, void *v)
{
}

const struct seq_operations cpuinfo_op = {
	.start	= c_start,
	.next	= c_next,
	.stop	= c_stop,
	.show	= show_cpuinfo,
};
<<<<<<< HEAD
=======

#ifdef CONFIG_X86_USER_SHADOW_STACK
static void dump_x86_features(struct seq_file *m, unsigned long features)
{
	if (features & ARCH_SHSTK_SHSTK)
		seq_puts(m, "shstk ");
	if (features & ARCH_SHSTK_WRSS)
		seq_puts(m, "wrss ");
}

void arch_proc_pid_thread_features(struct seq_file *m, struct task_struct *task)
{
	seq_puts(m, "x86_Thread_features:\t");
	dump_x86_features(m, task->thread.features);
	seq_putc(m, '\n');

	seq_puts(m, "x86_Thread_features_locked:\t");
	dump_x86_features(m, task->thread.features_locked);
	seq_putc(m, '\n');
}
#endif /* CONFIG_X86_USER_SHADOW_STACK */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
