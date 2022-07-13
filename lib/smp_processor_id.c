<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * lib/smp_processor_id.c
 *
 * DEBUG_PREEMPT variant of smp_processor_id().
 */
#include <linux/export.h>
<<<<<<< HEAD
#include <linux/kallsyms.h>
#include <linux/sched.h>

notrace unsigned int debug_smp_processor_id(void)
{
	unsigned long preempt_count = preempt_count();
	int this_cpu = raw_smp_processor_id();

	if (likely(preempt_count))
=======
#include <linux/kprobes.h>
#include <linux/sched.h>

noinstr static
unsigned int check_preemption_disabled(const char *what1, const char *what2)
{
	int this_cpu = raw_smp_processor_id();

	if (likely(preempt_count()))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;

	if (irqs_disabled())
		goto out;

<<<<<<< HEAD
	/*
	 * Kernel threads bound to a single CPU can safely use
	 * smp_processor_id():
	 */
	if (cpumask_equal(tsk_cpus_allowed(current), cpumask_of(this_cpu)))
		goto out;

	/*
	 * It is valid to assume CPU-locality during early bootup:
	 */
	if (system_state != SYSTEM_RUNNING)
=======
	if (is_percpu_thread())
		goto out;

#ifdef CONFIG_SMP
	if (current->migration_disabled)
		goto out;
#endif

	/*
	 * It is valid to assume CPU-locality during early bootup:
	 */
	if (system_state < SYSTEM_SCHEDULING)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;

	/*
	 * Avoid recursion:
	 */
	preempt_disable_notrace();

<<<<<<< HEAD
	if (!printk_ratelimit())
		goto out_enable;

	printk(KERN_ERR "BUG: using smp_processor_id() in preemptible [%08x] "
			"code: %s/%d\n",
			preempt_count() - 1, current->comm, current->pid);
	print_symbol("caller is %s\n", (long)__builtin_return_address(0));
	dump_stack();

out_enable:
=======
	instrumentation_begin();
	if (!printk_ratelimit())
		goto out_enable;

	printk(KERN_ERR "BUG: using %s%s() in preemptible [%08x] code: %s/%d\n",
		what1, what2, preempt_count() - 1, current->comm, current->pid);

	printk("caller is %pS\n", __builtin_return_address(0));
	dump_stack();

out_enable:
	instrumentation_end();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	preempt_enable_no_resched_notrace();
out:
	return this_cpu;
}

<<<<<<< HEAD
EXPORT_SYMBOL(debug_smp_processor_id);

=======
noinstr unsigned int debug_smp_processor_id(void)
{
	return check_preemption_disabled("smp_processor_id", "");
}
EXPORT_SYMBOL(debug_smp_processor_id);

noinstr void __this_cpu_preempt_check(const char *op)
{
	check_preemption_disabled("__this_cpu_", op);
}
EXPORT_SYMBOL(__this_cpu_preempt_check);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
