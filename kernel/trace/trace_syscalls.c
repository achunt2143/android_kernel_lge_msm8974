<<<<<<< HEAD
#include <trace/syscall.h>
#include <trace/events/syscalls.h>
=======
// SPDX-License-Identifier: GPL-2.0
#include <trace/syscall.h>
#include <trace/events/syscalls.h>
#include <linux/syscalls.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/module.h>	/* for MODULE_NAME_LEN via KSYM_SYMBOL_LEN */
#include <linux/ftrace.h>
#include <linux/perf_event.h>
<<<<<<< HEAD
=======
#include <linux/xarray.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/syscall.h>

#include "trace_output.h"
#include "trace.h"

static DEFINE_MUTEX(syscall_trace_lock);
<<<<<<< HEAD
static int sys_refcount_enter;
static int sys_refcount_exit;
static DECLARE_BITMAP(enabled_enter_syscalls, NR_syscalls);
static DECLARE_BITMAP(enabled_exit_syscalls, NR_syscalls);

static int syscall_enter_register(struct ftrace_event_call *event,
				 enum trace_reg type, void *data);
static int syscall_exit_register(struct ftrace_event_call *event,
				 enum trace_reg type, void *data);

static int syscall_enter_define_fields(struct ftrace_event_call *call);
static int syscall_exit_define_fields(struct ftrace_event_call *call);

static struct list_head *
syscall_get_enter_fields(struct ftrace_event_call *call)
=======

static int syscall_enter_register(struct trace_event_call *event,
				 enum trace_reg type, void *data);
static int syscall_exit_register(struct trace_event_call *event,
				 enum trace_reg type, void *data);

static struct list_head *
syscall_get_enter_fields(struct trace_event_call *call)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct syscall_metadata *entry = call->data;

	return &entry->enter_fields;
}

<<<<<<< HEAD
struct trace_event_functions enter_syscall_print_funcs = {
	.trace		= print_syscall_enter,
};

struct trace_event_functions exit_syscall_print_funcs = {
	.trace		= print_syscall_exit,
};

struct ftrace_event_class event_class_syscall_enter = {
	.system		= "syscalls",
	.reg		= syscall_enter_register,
	.define_fields	= syscall_enter_define_fields,
	.get_fields	= syscall_get_enter_fields,
	.raw_init	= init_syscall_trace,
};

struct ftrace_event_class event_class_syscall_exit = {
	.system		= "syscalls",
	.reg		= syscall_exit_register,
	.define_fields	= syscall_exit_define_fields,
	.fields		= LIST_HEAD_INIT(event_class_syscall_exit.fields),
	.raw_init	= init_syscall_trace,
};

extern struct syscall_metadata *__start_syscalls_metadata[];
extern struct syscall_metadata *__stop_syscalls_metadata[];

=======
extern struct syscall_metadata *__start_syscalls_metadata[];
extern struct syscall_metadata *__stop_syscalls_metadata[];

static DEFINE_XARRAY(syscalls_metadata_sparse);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct syscall_metadata **syscalls_metadata;

#ifndef ARCH_HAS_SYSCALL_MATCH_SYM_NAME
static inline bool arch_syscall_match_sym_name(const char *sym, const char *name)
{
	/*
	 * Only compare after the "sys" prefix. Archs that use
	 * syscall wrappers may have syscalls symbols aliases prefixed
<<<<<<< HEAD
	 * with "SyS" instead of "sys", leading to an unwanted
=======
	 * with ".SyS" or ".sys" instead of "sys", leading to an unwanted
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * mismatch.
	 */
	return !strcmp(sym + 3, name + 3);
}
#endif

<<<<<<< HEAD
=======
#ifdef ARCH_TRACE_IGNORE_COMPAT_SYSCALLS
/*
 * Some architectures that allow for 32bit applications
 * to run on a 64bit kernel, do not map the syscalls for
 * the 32bit tasks the same as they do for 64bit tasks.
 *
 *     *cough*x86*cough*
 *
 * In such a case, instead of reporting the wrong syscalls,
 * simply ignore them.
 *
 * For an arch to ignore the compat syscalls it needs to
 * define ARCH_TRACE_IGNORE_COMPAT_SYSCALLS as well as
 * define the function arch_trace_is_compat_syscall() to let
 * the tracing system know that it should ignore it.
 */
static int
trace_get_syscall_nr(struct task_struct *task, struct pt_regs *regs)
{
	if (unlikely(arch_trace_is_compat_syscall(regs)))
		return -1;

	return syscall_get_nr(task, regs);
}
#else
static inline int
trace_get_syscall_nr(struct task_struct *task, struct pt_regs *regs)
{
	return syscall_get_nr(task, regs);
}
#endif /* ARCH_TRACE_IGNORE_COMPAT_SYSCALLS */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static __init struct syscall_metadata *
find_syscall_meta(unsigned long syscall)
{
	struct syscall_metadata **start;
	struct syscall_metadata **stop;
	char str[KSYM_SYMBOL_LEN];


	start = __start_syscalls_metadata;
	stop = __stop_syscalls_metadata;
	kallsyms_lookup(syscall, NULL, NULL, NULL, str);

	if (arch_syscall_match_sym_name(str, "sys_ni_syscall"))
		return NULL;

	for ( ; start < stop; start++) {
		if ((*start)->name && arch_syscall_match_sym_name(str, (*start)->name))
			return *start;
	}
	return NULL;
}

static struct syscall_metadata *syscall_nr_to_meta(int nr)
{
<<<<<<< HEAD
=======
	if (IS_ENABLED(CONFIG_HAVE_SPARSE_SYSCALL_NR))
		return xa_load(&syscalls_metadata_sparse, (unsigned long)nr);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!syscalls_metadata || nr >= NR_syscalls || nr < 0)
		return NULL;

	return syscalls_metadata[nr];
}

<<<<<<< HEAD
enum print_line_t
print_syscall_enter(struct trace_iterator *iter, int flags,
		    struct trace_event *event)
{
=======
const char *get_syscall_name(int syscall)
{
	struct syscall_metadata *entry;

	entry = syscall_nr_to_meta(syscall);
	if (!entry)
		return NULL;

	return entry->name;
}

static enum print_line_t
print_syscall_enter(struct trace_iterator *iter, int flags,
		    struct trace_event *event)
{
	struct trace_array *tr = iter->tr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct trace_seq *s = &iter->seq;
	struct trace_entry *ent = iter->ent;
	struct syscall_trace_enter *trace;
	struct syscall_metadata *entry;
<<<<<<< HEAD
	int i, ret, syscall;
=======
	int i, syscall;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	trace = (typeof(trace))ent;
	syscall = trace->nr;
	entry = syscall_nr_to_meta(syscall);

	if (!entry)
		goto end;

	if (entry->enter_event->event.type != ent->type) {
		WARN_ON_ONCE(1);
		goto end;
	}

<<<<<<< HEAD
	ret = trace_seq_printf(s, "%s(", entry->name);
	if (!ret)
		return TRACE_TYPE_PARTIAL_LINE;

	for (i = 0; i < entry->nb_args; i++) {
		/* parameter types */
		if (trace_flags & TRACE_ITER_VERBOSE) {
			ret = trace_seq_printf(s, "%s ", entry->types[i]);
			if (!ret)
				return TRACE_TYPE_PARTIAL_LINE;
		}
		/* parameter values */
		ret = trace_seq_printf(s, "%s: %lx%s", entry->args[i],
				       trace->args[i],
				       i == entry->nb_args - 1 ? "" : ", ");
		if (!ret)
			return TRACE_TYPE_PARTIAL_LINE;
	}

	ret = trace_seq_putc(s, ')');
	if (!ret)
		return TRACE_TYPE_PARTIAL_LINE;

end:
	ret =  trace_seq_putc(s, '\n');
	if (!ret)
		return TRACE_TYPE_PARTIAL_LINE;

	return TRACE_TYPE_HANDLED;
}

enum print_line_t
=======
	trace_seq_printf(s, "%s(", entry->name);

	for (i = 0; i < entry->nb_args; i++) {

		if (trace_seq_has_overflowed(s))
			goto end;

		/* parameter types */
		if (tr && tr->trace_flags & TRACE_ITER_VERBOSE)
			trace_seq_printf(s, "%s ", entry->types[i]);

		/* parameter values */
		trace_seq_printf(s, "%s: %lx%s", entry->args[i],
				 trace->args[i],
				 i == entry->nb_args - 1 ? "" : ", ");
	}

	trace_seq_putc(s, ')');
end:
	trace_seq_putc(s, '\n');

	return trace_handle_return(s);
}

static enum print_line_t
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
print_syscall_exit(struct trace_iterator *iter, int flags,
		   struct trace_event *event)
{
	struct trace_seq *s = &iter->seq;
	struct trace_entry *ent = iter->ent;
	struct syscall_trace_exit *trace;
	int syscall;
	struct syscall_metadata *entry;
<<<<<<< HEAD
	int ret;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	trace = (typeof(trace))ent;
	syscall = trace->nr;
	entry = syscall_nr_to_meta(syscall);

	if (!entry) {
<<<<<<< HEAD
		trace_seq_printf(s, "\n");
		return TRACE_TYPE_HANDLED;
=======
		trace_seq_putc(s, '\n');
		goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (entry->exit_event->event.type != ent->type) {
		WARN_ON_ONCE(1);
		return TRACE_TYPE_UNHANDLED;
	}

<<<<<<< HEAD
	ret = trace_seq_printf(s, "%s -> 0x%lx\n", entry->name,
				trace->ret);
	if (!ret)
		return TRACE_TYPE_PARTIAL_LINE;

	return TRACE_TYPE_HANDLED;
}

extern char *__bad_type_size(void);

#define SYSCALL_FIELD(type, name)					\
	sizeof(type) != sizeof(trace.name) ?				\
		__bad_type_size() :					\
		#type, #name, offsetof(typeof(trace), name),		\
		sizeof(trace.name), is_signed_type(type)

static
int  __set_enter_print_fmt(struct syscall_metadata *entry, char *buf, int len)
=======
	trace_seq_printf(s, "%s -> 0x%lx\n", entry->name,
				trace->ret);

 out:
	return trace_handle_return(s);
}

#define SYSCALL_FIELD(_type, _name) {					\
	.type = #_type, .name = #_name,					\
	.size = sizeof(_type), .align = __alignof__(_type),		\
	.is_signed = is_signed_type(_type), .filter_type = FILTER_OTHER }

static int __init
__set_enter_print_fmt(struct syscall_metadata *entry, char *buf, int len)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int i;
	int pos = 0;

	/* When len=0, we just calculate the needed length */
#define LEN_OR_ZERO (len ? len - pos : 0)

	pos += snprintf(buf + pos, LEN_OR_ZERO, "\"");
	for (i = 0; i < entry->nb_args; i++) {
		pos += snprintf(buf + pos, LEN_OR_ZERO, "%s: 0x%%0%zulx%s",
				entry->args[i], sizeof(unsigned long),
				i == entry->nb_args - 1 ? "" : ", ");
	}
	pos += snprintf(buf + pos, LEN_OR_ZERO, "\"");

	for (i = 0; i < entry->nb_args; i++) {
		pos += snprintf(buf + pos, LEN_OR_ZERO,
				", ((unsigned long)(REC->%s))", entry->args[i]);
	}

#undef LEN_OR_ZERO

	/* return the length of print_fmt */
	return pos;
}

<<<<<<< HEAD
static int set_syscall_print_fmt(struct ftrace_event_call *call)
=======
static int __init set_syscall_print_fmt(struct trace_event_call *call)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	char *print_fmt;
	int len;
	struct syscall_metadata *entry = call->data;

	if (entry->enter_event != call) {
		call->print_fmt = "\"0x%lx\", REC->ret";
		return 0;
	}

	/* First: called with 0 length to calculate the needed length */
	len = __set_enter_print_fmt(entry, NULL, 0);

	print_fmt = kmalloc(len + 1, GFP_KERNEL);
	if (!print_fmt)
		return -ENOMEM;

	/* Second: actually write the @print_fmt */
	__set_enter_print_fmt(entry, print_fmt, len + 1);
	call->print_fmt = print_fmt;

	return 0;
}

<<<<<<< HEAD
static void free_syscall_print_fmt(struct ftrace_event_call *call)
=======
static void __init free_syscall_print_fmt(struct trace_event_call *call)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct syscall_metadata *entry = call->data;

	if (entry->enter_event == call)
		kfree(call->print_fmt);
}

<<<<<<< HEAD
static int syscall_enter_define_fields(struct ftrace_event_call *call)
{
	struct syscall_trace_enter trace;
	struct syscall_metadata *meta = call->data;
	int ret;
	int i;
	int offset = offsetof(typeof(trace), args);

	ret = trace_define_field(call, SYSCALL_FIELD(int, nr), FILTER_OTHER);
	if (ret)
		return ret;
=======
static int __init syscall_enter_define_fields(struct trace_event_call *call)
{
	struct syscall_trace_enter trace;
	struct syscall_metadata *meta = call->data;
	int offset = offsetof(typeof(trace), args);
	int ret = 0;
	int i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (i = 0; i < meta->nb_args; i++) {
		ret = trace_define_field(call, meta->types[i],
					 meta->args[i], offset,
					 sizeof(unsigned long), 0,
					 FILTER_OTHER);
<<<<<<< HEAD
=======
		if (ret)
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		offset += sizeof(unsigned long);
	}

	return ret;
}

<<<<<<< HEAD
static int syscall_exit_define_fields(struct ftrace_event_call *call)
{
	struct syscall_trace_exit trace;
	int ret;

	ret = trace_define_field(call, SYSCALL_FIELD(int, nr), FILTER_OTHER);
	if (ret)
		return ret;

	ret = trace_define_field(call, SYSCALL_FIELD(long, ret),
				 FILTER_OTHER);

	return ret;
}

void ftrace_syscall_enter(void *ignore, struct pt_regs *regs, long id)
{
	struct syscall_trace_enter *entry;
	struct syscall_metadata *sys_data;
	struct ring_buffer_event *event;
	struct ring_buffer *buffer;
	int size;
	int syscall_nr;
	unsigned long irq_flags;
	int pc;

	syscall_nr = syscall_get_nr(current, regs);
	if (syscall_nr < 0 || syscall_nr >= NR_syscalls)
		return;
	if (!test_bit(syscall_nr, enabled_enter_syscalls))
=======
static void ftrace_syscall_enter(void *data, struct pt_regs *regs, long id)
{
	struct trace_array *tr = data;
	struct trace_event_file *trace_file;
	struct syscall_trace_enter *entry;
	struct syscall_metadata *sys_data;
	struct trace_event_buffer fbuffer;
	unsigned long args[6];
	int syscall_nr;
	int size;

	syscall_nr = trace_get_syscall_nr(current, regs);
	if (syscall_nr < 0 || syscall_nr >= NR_syscalls)
		return;

	/* Here we're inside tp handler's rcu_read_lock_sched (__DO_TRACE) */
	trace_file = rcu_dereference_sched(tr->enter_syscall_files[syscall_nr]);
	if (!trace_file)
		return;

	if (trace_trigger_soft_disabled(trace_file))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;

	sys_data = syscall_nr_to_meta(syscall_nr);
	if (!sys_data)
		return;

	size = sizeof(*entry) + sizeof(unsigned long) * sys_data->nb_args;

<<<<<<< HEAD
	local_save_flags(irq_flags);
	pc = preempt_count();

	event = trace_current_buffer_lock_reserve(&buffer,
			sys_data->enter_event->event.type, size, irq_flags, pc);
	if (!event)
		return;

	entry = ring_buffer_event_data(event);
	entry->nr = syscall_nr;
	syscall_get_arguments(current, regs, 0, sys_data->nb_args, entry->args);

	if (!filter_current_check_discard(buffer, sys_data->enter_event,
					  entry, event))
		trace_current_buffer_unlock_commit(buffer, event,
						   irq_flags, pc);
}

void ftrace_syscall_exit(void *ignore, struct pt_regs *regs, long ret)
{
	struct syscall_trace_exit *entry;
	struct syscall_metadata *sys_data;
	struct ring_buffer_event *event;
	struct ring_buffer *buffer;
	int syscall_nr;
	unsigned long irq_flags;
	int pc;

	syscall_nr = syscall_get_nr(current, regs);
	if (syscall_nr < 0 || syscall_nr >= NR_syscalls)
		return;
	if (!test_bit(syscall_nr, enabled_exit_syscalls))
=======
	entry = trace_event_buffer_reserve(&fbuffer, trace_file, size);
	if (!entry)
		return;

	entry = ring_buffer_event_data(fbuffer.event);
	entry->nr = syscall_nr;
	syscall_get_arguments(current, regs, args);
	memcpy(entry->args, args, sizeof(unsigned long) * sys_data->nb_args);

	trace_event_buffer_commit(&fbuffer);
}

static void ftrace_syscall_exit(void *data, struct pt_regs *regs, long ret)
{
	struct trace_array *tr = data;
	struct trace_event_file *trace_file;
	struct syscall_trace_exit *entry;
	struct syscall_metadata *sys_data;
	struct trace_event_buffer fbuffer;
	int syscall_nr;

	syscall_nr = trace_get_syscall_nr(current, regs);
	if (syscall_nr < 0 || syscall_nr >= NR_syscalls)
		return;

	/* Here we're inside tp handler's rcu_read_lock_sched (__DO_TRACE()) */
	trace_file = rcu_dereference_sched(tr->exit_syscall_files[syscall_nr]);
	if (!trace_file)
		return;

	if (trace_trigger_soft_disabled(trace_file))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;

	sys_data = syscall_nr_to_meta(syscall_nr);
	if (!sys_data)
		return;

<<<<<<< HEAD
	local_save_flags(irq_flags);
	pc = preempt_count();

	event = trace_current_buffer_lock_reserve(&buffer,
			sys_data->exit_event->event.type, sizeof(*entry),
			irq_flags, pc);
	if (!event)
		return;

	entry = ring_buffer_event_data(event);
	entry->nr = syscall_nr;
	entry->ret = syscall_get_return_value(current, regs);

	if (!filter_current_check_discard(buffer, sys_data->exit_event,
					  entry, event))
		trace_current_buffer_unlock_commit(buffer, event,
						   irq_flags, pc);
}

int reg_event_syscall_enter(struct ftrace_event_call *call)
{
=======
	entry = trace_event_buffer_reserve(&fbuffer, trace_file, sizeof(*entry));
	if (!entry)
		return;

	entry = ring_buffer_event_data(fbuffer.event);
	entry->nr = syscall_nr;
	entry->ret = syscall_get_return_value(current, regs);

	trace_event_buffer_commit(&fbuffer);
}

static int reg_event_syscall_enter(struct trace_event_file *file,
				   struct trace_event_call *call)
{
	struct trace_array *tr = file->tr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret = 0;
	int num;

	num = ((struct syscall_metadata *)call->data)->syscall_nr;
	if (WARN_ON_ONCE(num < 0 || num >= NR_syscalls))
		return -ENOSYS;
	mutex_lock(&syscall_trace_lock);
<<<<<<< HEAD
	if (!sys_refcount_enter)
		ret = register_trace_sys_enter(ftrace_syscall_enter, NULL);
	if (!ret) {
		set_bit(num, enabled_enter_syscalls);
		sys_refcount_enter++;
=======
	if (!tr->sys_refcount_enter)
		ret = register_trace_sys_enter(ftrace_syscall_enter, tr);
	if (!ret) {
		rcu_assign_pointer(tr->enter_syscall_files[num], file);
		tr->sys_refcount_enter++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	mutex_unlock(&syscall_trace_lock);
	return ret;
}

<<<<<<< HEAD
void unreg_event_syscall_enter(struct ftrace_event_call *call)
{
=======
static void unreg_event_syscall_enter(struct trace_event_file *file,
				      struct trace_event_call *call)
{
	struct trace_array *tr = file->tr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int num;

	num = ((struct syscall_metadata *)call->data)->syscall_nr;
	if (WARN_ON_ONCE(num < 0 || num >= NR_syscalls))
		return;
	mutex_lock(&syscall_trace_lock);
<<<<<<< HEAD
	sys_refcount_enter--;
	clear_bit(num, enabled_enter_syscalls);
	if (!sys_refcount_enter)
		unregister_trace_sys_enter(ftrace_syscall_enter, NULL);
	mutex_unlock(&syscall_trace_lock);
}

int reg_event_syscall_exit(struct ftrace_event_call *call)
{
=======
	tr->sys_refcount_enter--;
	RCU_INIT_POINTER(tr->enter_syscall_files[num], NULL);
	if (!tr->sys_refcount_enter)
		unregister_trace_sys_enter(ftrace_syscall_enter, tr);
	mutex_unlock(&syscall_trace_lock);
}

static int reg_event_syscall_exit(struct trace_event_file *file,
				  struct trace_event_call *call)
{
	struct trace_array *tr = file->tr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret = 0;
	int num;

	num = ((struct syscall_metadata *)call->data)->syscall_nr;
	if (WARN_ON_ONCE(num < 0 || num >= NR_syscalls))
		return -ENOSYS;
	mutex_lock(&syscall_trace_lock);
<<<<<<< HEAD
	if (!sys_refcount_exit)
		ret = register_trace_sys_exit(ftrace_syscall_exit, NULL);
	if (!ret) {
		set_bit(num, enabled_exit_syscalls);
		sys_refcount_exit++;
=======
	if (!tr->sys_refcount_exit)
		ret = register_trace_sys_exit(ftrace_syscall_exit, tr);
	if (!ret) {
		rcu_assign_pointer(tr->exit_syscall_files[num], file);
		tr->sys_refcount_exit++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	mutex_unlock(&syscall_trace_lock);
	return ret;
}

<<<<<<< HEAD
void unreg_event_syscall_exit(struct ftrace_event_call *call)
{
=======
static void unreg_event_syscall_exit(struct trace_event_file *file,
				     struct trace_event_call *call)
{
	struct trace_array *tr = file->tr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int num;

	num = ((struct syscall_metadata *)call->data)->syscall_nr;
	if (WARN_ON_ONCE(num < 0 || num >= NR_syscalls))
		return;
	mutex_lock(&syscall_trace_lock);
<<<<<<< HEAD
	sys_refcount_exit--;
	clear_bit(num, enabled_exit_syscalls);
	if (!sys_refcount_exit)
		unregister_trace_sys_exit(ftrace_syscall_exit, NULL);
	mutex_unlock(&syscall_trace_lock);
}

int init_syscall_trace(struct ftrace_event_call *call)
=======
	tr->sys_refcount_exit--;
	RCU_INIT_POINTER(tr->exit_syscall_files[num], NULL);
	if (!tr->sys_refcount_exit)
		unregister_trace_sys_exit(ftrace_syscall_exit, tr);
	mutex_unlock(&syscall_trace_lock);
}

static int __init init_syscall_trace(struct trace_event_call *call)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int id;
	int num;

	num = ((struct syscall_metadata *)call->data)->syscall_nr;
	if (num < 0 || num >= NR_syscalls) {
		pr_debug("syscall %s metadata not mapped, disabling ftrace event\n",
				((struct syscall_metadata *)call->data)->name);
		return -ENOSYS;
	}

	if (set_syscall_print_fmt(call) < 0)
		return -ENOMEM;

	id = trace_event_raw_init(call);

	if (id < 0) {
		free_syscall_print_fmt(call);
		return id;
	}

	return id;
}

<<<<<<< HEAD
=======
static struct trace_event_fields __refdata syscall_enter_fields_array[] = {
	SYSCALL_FIELD(int, __syscall_nr),
	{ .type = TRACE_FUNCTION_TYPE,
	  .define_fields = syscall_enter_define_fields },
	{}
};

struct trace_event_functions enter_syscall_print_funcs = {
	.trace		= print_syscall_enter,
};

struct trace_event_functions exit_syscall_print_funcs = {
	.trace		= print_syscall_exit,
};

struct trace_event_class __refdata event_class_syscall_enter = {
	.system		= "syscalls",
	.reg		= syscall_enter_register,
	.fields_array	= syscall_enter_fields_array,
	.get_fields	= syscall_get_enter_fields,
	.raw_init	= init_syscall_trace,
};

struct trace_event_class __refdata event_class_syscall_exit = {
	.system		= "syscalls",
	.reg		= syscall_exit_register,
	.fields_array	= (struct trace_event_fields[]){
		SYSCALL_FIELD(int, __syscall_nr),
		SYSCALL_FIELD(long, ret),
		{}
	},
	.fields		= LIST_HEAD_INIT(event_class_syscall_exit.fields),
	.raw_init	= init_syscall_trace,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
unsigned long __init __weak arch_syscall_addr(int nr)
{
	return (unsigned long)sys_call_table[nr];
}

<<<<<<< HEAD
int __init init_ftrace_syscalls(void)
=======
void __init init_ftrace_syscalls(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct syscall_metadata *meta;
	unsigned long addr;
	int i;
<<<<<<< HEAD

	syscalls_metadata = kcalloc(NR_syscalls, sizeof(*syscalls_metadata),
				    GFP_KERNEL);
	if (!syscalls_metadata) {
		WARN_ON(1);
		return -ENOMEM;
=======
	void *ret;

	if (!IS_ENABLED(CONFIG_HAVE_SPARSE_SYSCALL_NR)) {
		syscalls_metadata = kcalloc(NR_syscalls,
					sizeof(*syscalls_metadata),
					GFP_KERNEL);
		if (!syscalls_metadata) {
			WARN_ON(1);
			return;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	for (i = 0; i < NR_syscalls; i++) {
		addr = arch_syscall_addr(i);
		meta = find_syscall_meta(addr);
		if (!meta)
			continue;

		meta->syscall_nr = i;
<<<<<<< HEAD
		syscalls_metadata[i] = meta;
	}

	return 0;
}
core_initcall(init_ftrace_syscalls);
=======

		if (!IS_ENABLED(CONFIG_HAVE_SPARSE_SYSCALL_NR)) {
			syscalls_metadata[i] = meta;
		} else {
			ret = xa_store(&syscalls_metadata_sparse, i, meta,
					GFP_KERNEL);
			WARN(xa_is_err(ret),
				"Syscall memory allocation failed\n");
		}

	}
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_PERF_EVENTS

static DECLARE_BITMAP(enabled_perf_enter_syscalls, NR_syscalls);
static DECLARE_BITMAP(enabled_perf_exit_syscalls, NR_syscalls);
static int sys_perf_refcount_enter;
static int sys_perf_refcount_exit;

<<<<<<< HEAD
=======
static int perf_call_bpf_enter(struct trace_event_call *call, struct pt_regs *regs,
			       struct syscall_metadata *sys_data,
			       struct syscall_trace_enter *rec)
{
	struct syscall_tp_t {
		struct trace_entry ent;
		int syscall_nr;
		unsigned long args[SYSCALL_DEFINE_MAXARGS];
	} __aligned(8) param;
	int i;

	BUILD_BUG_ON(sizeof(param.ent) < sizeof(void *));

	/* bpf prog requires 'regs' to be the first member in the ctx (a.k.a. &param) */
	*(struct pt_regs **)&param = regs;
	param.syscall_nr = rec->nr;
	for (i = 0; i < sys_data->nb_args; i++)
		param.args[i] = rec->args[i];
	return trace_call_bpf(call, &param);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void perf_syscall_enter(void *ignore, struct pt_regs *regs, long id)
{
	struct syscall_metadata *sys_data;
	struct syscall_trace_enter *rec;
	struct hlist_head *head;
<<<<<<< HEAD
=======
	unsigned long args[6];
	bool valid_prog_array;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int syscall_nr;
	int rctx;
	int size;

<<<<<<< HEAD
	syscall_nr = syscall_get_nr(current, regs);
=======
	syscall_nr = trace_get_syscall_nr(current, regs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (syscall_nr < 0 || syscall_nr >= NR_syscalls)
		return;
	if (!test_bit(syscall_nr, enabled_perf_enter_syscalls))
		return;

	sys_data = syscall_nr_to_meta(syscall_nr);
	if (!sys_data)
		return;

<<<<<<< HEAD
=======
	head = this_cpu_ptr(sys_data->enter_event->perf_events);
	valid_prog_array = bpf_prog_array_valid(sys_data->enter_event);
	if (!valid_prog_array && hlist_empty(head))
		return;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* get the size after alignment with the u32 buffer size field */
	size = sizeof(unsigned long) * sys_data->nb_args + sizeof(*rec);
	size = ALIGN(size + sizeof(u32), sizeof(u64));
	size -= sizeof(u32);

<<<<<<< HEAD
	if (WARN_ONCE(size > PERF_MAX_TRACE_SIZE,
		      "perf buffer not large enough"))
		return;

	rec = (struct syscall_trace_enter *)perf_trace_buf_prepare(size,
				sys_data->enter_event->event.type, regs, &rctx);
=======
	rec = perf_trace_buf_alloc(size, NULL, &rctx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!rec)
		return;

	rec->nr = syscall_nr;
<<<<<<< HEAD
	syscall_get_arguments(current, regs, 0, sys_data->nb_args,
			       (unsigned long *)&rec->args);

	head = this_cpu_ptr(sys_data->enter_event->perf_events);
	perf_trace_buf_submit(rec, size, rctx, 0, 1, regs, head);
}

int perf_sysenter_enable(struct ftrace_event_call *call)
=======
	syscall_get_arguments(current, regs, args);
	memcpy(&rec->args, args, sizeof(unsigned long) * sys_data->nb_args);

	if ((valid_prog_array &&
	     !perf_call_bpf_enter(sys_data->enter_event, regs, sys_data, rec)) ||
	    hlist_empty(head)) {
		perf_swevent_put_recursion_context(rctx);
		return;
	}

	perf_trace_buf_submit(rec, size, rctx,
			      sys_data->enter_event->event.type, 1, regs,
			      head, NULL);
}

static int perf_sysenter_enable(struct trace_event_call *call)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int ret = 0;
	int num;

	num = ((struct syscall_metadata *)call->data)->syscall_nr;

	mutex_lock(&syscall_trace_lock);
	if (!sys_perf_refcount_enter)
		ret = register_trace_sys_enter(perf_syscall_enter, NULL);
	if (ret) {
<<<<<<< HEAD
		pr_info("event trace: Could not activate"
				"syscall entry trace point");
=======
		pr_info("event trace: Could not activate syscall entry trace point");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		set_bit(num, enabled_perf_enter_syscalls);
		sys_perf_refcount_enter++;
	}
	mutex_unlock(&syscall_trace_lock);
	return ret;
}

<<<<<<< HEAD
void perf_sysenter_disable(struct ftrace_event_call *call)
=======
static void perf_sysenter_disable(struct trace_event_call *call)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int num;

	num = ((struct syscall_metadata *)call->data)->syscall_nr;

	mutex_lock(&syscall_trace_lock);
	sys_perf_refcount_enter--;
	clear_bit(num, enabled_perf_enter_syscalls);
	if (!sys_perf_refcount_enter)
		unregister_trace_sys_enter(perf_syscall_enter, NULL);
	mutex_unlock(&syscall_trace_lock);
}

<<<<<<< HEAD
=======
static int perf_call_bpf_exit(struct trace_event_call *call, struct pt_regs *regs,
			      struct syscall_trace_exit *rec)
{
	struct syscall_tp_t {
		struct trace_entry ent;
		int syscall_nr;
		unsigned long ret;
	} __aligned(8) param;

	/* bpf prog requires 'regs' to be the first member in the ctx (a.k.a. &param) */
	*(struct pt_regs **)&param = regs;
	param.syscall_nr = rec->nr;
	param.ret = rec->ret;
	return trace_call_bpf(call, &param);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void perf_syscall_exit(void *ignore, struct pt_regs *regs, long ret)
{
	struct syscall_metadata *sys_data;
	struct syscall_trace_exit *rec;
	struct hlist_head *head;
<<<<<<< HEAD
=======
	bool valid_prog_array;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int syscall_nr;
	int rctx;
	int size;

<<<<<<< HEAD
	syscall_nr = syscall_get_nr(current, regs);
=======
	syscall_nr = trace_get_syscall_nr(current, regs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (syscall_nr < 0 || syscall_nr >= NR_syscalls)
		return;
	if (!test_bit(syscall_nr, enabled_perf_exit_syscalls))
		return;

	sys_data = syscall_nr_to_meta(syscall_nr);
	if (!sys_data)
		return;

<<<<<<< HEAD
=======
	head = this_cpu_ptr(sys_data->exit_event->perf_events);
	valid_prog_array = bpf_prog_array_valid(sys_data->exit_event);
	if (!valid_prog_array && hlist_empty(head))
		return;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* We can probably do that at build time */
	size = ALIGN(sizeof(*rec) + sizeof(u32), sizeof(u64));
	size -= sizeof(u32);

<<<<<<< HEAD
	/*
	 * Impossible, but be paranoid with the future
	 * How to put this check outside runtime?
	 */
	if (WARN_ONCE(size > PERF_MAX_TRACE_SIZE,
		"exit event has grown above perf buffer size"))
		return;

	rec = (struct syscall_trace_exit *)perf_trace_buf_prepare(size,
				sys_data->exit_event->event.type, regs, &rctx);
=======
	rec = perf_trace_buf_alloc(size, NULL, &rctx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!rec)
		return;

	rec->nr = syscall_nr;
	rec->ret = syscall_get_return_value(current, regs);

<<<<<<< HEAD
	head = this_cpu_ptr(sys_data->exit_event->perf_events);
	perf_trace_buf_submit(rec, size, rctx, 0, 1, regs, head);
}

int perf_sysexit_enable(struct ftrace_event_call *call)
=======
	if ((valid_prog_array &&
	     !perf_call_bpf_exit(sys_data->exit_event, regs, rec)) ||
	    hlist_empty(head)) {
		perf_swevent_put_recursion_context(rctx);
		return;
	}

	perf_trace_buf_submit(rec, size, rctx, sys_data->exit_event->event.type,
			      1, regs, head, NULL);
}

static int perf_sysexit_enable(struct trace_event_call *call)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int ret = 0;
	int num;

	num = ((struct syscall_metadata *)call->data)->syscall_nr;

	mutex_lock(&syscall_trace_lock);
	if (!sys_perf_refcount_exit)
		ret = register_trace_sys_exit(perf_syscall_exit, NULL);
	if (ret) {
<<<<<<< HEAD
		pr_info("event trace: Could not activate"
				"syscall exit trace point");
=======
		pr_info("event trace: Could not activate syscall exit trace point");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		set_bit(num, enabled_perf_exit_syscalls);
		sys_perf_refcount_exit++;
	}
	mutex_unlock(&syscall_trace_lock);
	return ret;
}

<<<<<<< HEAD
void perf_sysexit_disable(struct ftrace_event_call *call)
=======
static void perf_sysexit_disable(struct trace_event_call *call)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int num;

	num = ((struct syscall_metadata *)call->data)->syscall_nr;

	mutex_lock(&syscall_trace_lock);
	sys_perf_refcount_exit--;
	clear_bit(num, enabled_perf_exit_syscalls);
	if (!sys_perf_refcount_exit)
		unregister_trace_sys_exit(perf_syscall_exit, NULL);
	mutex_unlock(&syscall_trace_lock);
}

#endif /* CONFIG_PERF_EVENTS */

<<<<<<< HEAD
static int syscall_enter_register(struct ftrace_event_call *event,
				 enum trace_reg type, void *data)
{
	switch (type) {
	case TRACE_REG_REGISTER:
		return reg_event_syscall_enter(event);
	case TRACE_REG_UNREGISTER:
		unreg_event_syscall_enter(event);
=======
static int syscall_enter_register(struct trace_event_call *event,
				 enum trace_reg type, void *data)
{
	struct trace_event_file *file = data;

	switch (type) {
	case TRACE_REG_REGISTER:
		return reg_event_syscall_enter(file, event);
	case TRACE_REG_UNREGISTER:
		unreg_event_syscall_enter(file, event);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

#ifdef CONFIG_PERF_EVENTS
	case TRACE_REG_PERF_REGISTER:
		return perf_sysenter_enable(event);
	case TRACE_REG_PERF_UNREGISTER:
		perf_sysenter_disable(event);
		return 0;
	case TRACE_REG_PERF_OPEN:
	case TRACE_REG_PERF_CLOSE:
	case TRACE_REG_PERF_ADD:
	case TRACE_REG_PERF_DEL:
		return 0;
#endif
	}
	return 0;
}

<<<<<<< HEAD
static int syscall_exit_register(struct ftrace_event_call *event,
				 enum trace_reg type, void *data)
{
	switch (type) {
	case TRACE_REG_REGISTER:
		return reg_event_syscall_exit(event);
	case TRACE_REG_UNREGISTER:
		unreg_event_syscall_exit(event);
=======
static int syscall_exit_register(struct trace_event_call *event,
				 enum trace_reg type, void *data)
{
	struct trace_event_file *file = data;

	switch (type) {
	case TRACE_REG_REGISTER:
		return reg_event_syscall_exit(file, event);
	case TRACE_REG_UNREGISTER:
		unreg_event_syscall_exit(file, event);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

#ifdef CONFIG_PERF_EVENTS
	case TRACE_REG_PERF_REGISTER:
		return perf_sysexit_enable(event);
	case TRACE_REG_PERF_UNREGISTER:
		perf_sysexit_disable(event);
		return 0;
	case TRACE_REG_PERF_OPEN:
	case TRACE_REG_PERF_CLOSE:
	case TRACE_REG_PERF_ADD:
	case TRACE_REG_PERF_DEL:
		return 0;
#endif
	}
	return 0;
}
