<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Kprobes-based tracing events
 *
 * Created by Masami Hiramatsu <mhiramat@redhat.com>
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/kprobes.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/smp.h>
#include <linux/debugfs.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <linux/ptrace.h>
#include <linux/perf_event.h>
#include <linux/stringify.h>
#include <linux/limits.h>
#include <asm/bitsperlong.h>

#include "trace.h"
#include "trace_output.h"

#define MAX_TRACE_ARGS 128
#define MAX_ARGSTR_LEN 63
#define MAX_EVENT_NAME_LEN 64
#define MAX_STRING_SIZE PATH_MAX
#define KPROBE_EVENT_SYSTEM "kprobes"

/* Reserved field names */
#define FIELD_STRING_IP "__probe_ip"
#define FIELD_STRING_RETIP "__probe_ret_ip"
#define FIELD_STRING_FUNC "__probe_func"

const char *reserved_field_names[] = {
	"common_type",
	"common_flags",
	"common_preempt_count",
	"common_pid",
	"common_tgid",
	FIELD_STRING_IP,
	FIELD_STRING_RETIP,
	FIELD_STRING_FUNC,
};

/* Printing function type */
typedef int (*print_type_func_t)(struct trace_seq *, const char *, void *,
				 void *);
#define PRINT_TYPE_FUNC_NAME(type)	print_type_##type
#define PRINT_TYPE_FMT_NAME(type)	print_type_format_##type

/* Printing  in basic type function template */
#define DEFINE_BASIC_PRINT_TYPE_FUNC(type, fmt, cast)			\
static __kprobes int PRINT_TYPE_FUNC_NAME(type)(struct trace_seq *s,	\
						const char *name,	\
						void *data, void *ent)\
{									\
	return trace_seq_printf(s, " %s=" fmt, name, (cast)*(type *)data);\
}									\
static const char PRINT_TYPE_FMT_NAME(type)[] = fmt;

DEFINE_BASIC_PRINT_TYPE_FUNC(u8, "%x", unsigned int)
DEFINE_BASIC_PRINT_TYPE_FUNC(u16, "%x", unsigned int)
DEFINE_BASIC_PRINT_TYPE_FUNC(u32, "%lx", unsigned long)
DEFINE_BASIC_PRINT_TYPE_FUNC(u64, "%llx", unsigned long long)
DEFINE_BASIC_PRINT_TYPE_FUNC(s8, "%d", int)
DEFINE_BASIC_PRINT_TYPE_FUNC(s16, "%d", int)
DEFINE_BASIC_PRINT_TYPE_FUNC(s32, "%ld", long)
DEFINE_BASIC_PRINT_TYPE_FUNC(s64, "%lld", long long)

/* data_rloc: data relative location, compatible with u32 */
#define make_data_rloc(len, roffs)	\
	(((u32)(len) << 16) | ((u32)(roffs) & 0xffff))
#define get_rloc_len(dl)	((u32)(dl) >> 16)
#define get_rloc_offs(dl)	((u32)(dl) & 0xffff)

static inline void *get_rloc_data(u32 *dl)
{
	return (u8 *)dl + get_rloc_offs(*dl);
}

/* For data_loc conversion */
static inline void *get_loc_data(u32 *dl, void *ent)
{
	return (u8 *)ent + get_rloc_offs(*dl);
}

/*
 * Convert data_rloc to data_loc:
 *  data_rloc stores the offset from data_rloc itself, but data_loc
 *  stores the offset from event entry.
 */
#define convert_rloc_to_loc(dl, offs)	((u32)(dl) + (offs))

/* For defining macros, define string/string_size types */
typedef u32 string;
typedef u32 string_size;

/* Print type function for string type */
static __kprobes int PRINT_TYPE_FUNC_NAME(string)(struct trace_seq *s,
						  const char *name,
						  void *data, void *ent)
{
	int len = *(u32 *)data >> 16;

	if (!len)
		return trace_seq_printf(s, " %s=(fault)", name);
	else
		return trace_seq_printf(s, " %s=\"%s\"", name,
					(const char *)get_loc_data(data, ent));
}
static const char PRINT_TYPE_FMT_NAME(string)[] = "\\\"%s\\\"";

/* Data fetch function type */
typedef	void (*fetch_func_t)(struct pt_regs *, void *, void *);

struct fetch_param {
	fetch_func_t	fn;
	void *data;
};

static __kprobes void call_fetch(struct fetch_param *fprm,
				 struct pt_regs *regs, void *dest)
{
	return fprm->fn(regs, fprm->data, dest);
}

#define FETCH_FUNC_NAME(method, type)	fetch_##method##_##type
/*
 * Define macro for basic types - we don't need to define s* types, because
 * we have to care only about bitwidth at recording time.
 */
#define DEFINE_BASIC_FETCH_FUNCS(method) \
DEFINE_FETCH_##method(u8)		\
DEFINE_FETCH_##method(u16)		\
DEFINE_FETCH_##method(u32)		\
DEFINE_FETCH_##method(u64)

#define CHECK_FETCH_FUNCS(method, fn)			\
	(((FETCH_FUNC_NAME(method, u8) == fn) ||	\
	  (FETCH_FUNC_NAME(method, u16) == fn) ||	\
	  (FETCH_FUNC_NAME(method, u32) == fn) ||	\
	  (FETCH_FUNC_NAME(method, u64) == fn) ||	\
	  (FETCH_FUNC_NAME(method, string) == fn) ||	\
	  (FETCH_FUNC_NAME(method, string_size) == fn)) \
	 && (fn != NULL))

/* Data fetch function templates */
#define DEFINE_FETCH_reg(type)						\
static __kprobes void FETCH_FUNC_NAME(reg, type)(struct pt_regs *regs,	\
					void *offset, void *dest)	\
{									\
	*(type *)dest = (type)regs_get_register(regs,			\
				(unsigned int)((unsigned long)offset));	\
}
DEFINE_BASIC_FETCH_FUNCS(reg)
/* No string on the register */
#define fetch_reg_string NULL
#define fetch_reg_string_size NULL

#define DEFINE_FETCH_stack(type)					\
static __kprobes void FETCH_FUNC_NAME(stack, type)(struct pt_regs *regs,\
					  void *offset, void *dest)	\
{									\
	*(type *)dest = (type)regs_get_kernel_stack_nth(regs,		\
				(unsigned int)((unsigned long)offset));	\
}
DEFINE_BASIC_FETCH_FUNCS(stack)
/* No string on the stack entry */
#define fetch_stack_string NULL
#define fetch_stack_string_size NULL

#define DEFINE_FETCH_retval(type)					\
static __kprobes void FETCH_FUNC_NAME(retval, type)(struct pt_regs *regs,\
					  void *dummy, void *dest)	\
{									\
	*(type *)dest = (type)regs_return_value(regs);			\
}
DEFINE_BASIC_FETCH_FUNCS(retval)
/* No string on the retval */
#define fetch_retval_string NULL
#define fetch_retval_string_size NULL

#define DEFINE_FETCH_memory(type)					\
static __kprobes void FETCH_FUNC_NAME(memory, type)(struct pt_regs *regs,\
					  void *addr, void *dest)	\
{									\
	type retval;							\
	if (probe_kernel_address(addr, retval))				\
		*(type *)dest = 0;					\
	else								\
		*(type *)dest = retval;					\
}
DEFINE_BASIC_FETCH_FUNCS(memory)
/*
 * Fetch a null-terminated string. Caller MUST set *(u32 *)dest with max
 * length and relative data location.
 */
static __kprobes void FETCH_FUNC_NAME(memory, string)(struct pt_regs *regs,
						      void *addr, void *dest)
{
	long ret;
	int maxlen = get_rloc_len(*(u32 *)dest);
	u8 *dst = get_rloc_data(dest);
	u8 *src = addr;
	mm_segment_t old_fs = get_fs();
	if (!maxlen)
		return;
	/*
	 * Try to get string again, since the string can be changed while
	 * probing.
	 */
	set_fs(KERNEL_DS);
	pagefault_disable();
	do
		ret = __copy_from_user_inatomic(dst++, src++, 1);
	while (dst[-1] && ret == 0 && src - (u8 *)addr < maxlen);
	dst[-1] = '\0';
	pagefault_enable();
	set_fs(old_fs);

	if (ret < 0) {	/* Failed to fetch string */
		((u8 *)get_rloc_data(dest))[0] = '\0';
		*(u32 *)dest = make_data_rloc(0, get_rloc_offs(*(u32 *)dest));
	} else
		*(u32 *)dest = make_data_rloc(src - (u8 *)addr,
					      get_rloc_offs(*(u32 *)dest));
}
/* Return the length of string -- including null terminal byte */
static __kprobes void FETCH_FUNC_NAME(memory, string_size)(struct pt_regs *regs,
							void *addr, void *dest)
{
	int ret, len = 0;
	u8 c;
	mm_segment_t old_fs = get_fs();

	set_fs(KERNEL_DS);
	pagefault_disable();
	do {
		ret = __copy_from_user_inatomic(&c, (u8 *)addr + len, 1);
		len++;
	} while (c && ret == 0 && len < MAX_STRING_SIZE);
	pagefault_enable();
	set_fs(old_fs);

	if (ret < 0)	/* Failed to check the length */
		*(u32 *)dest = 0;
	else
		*(u32 *)dest = len;
}

/* Memory fetching by symbol */
struct symbol_cache {
	char *symbol;
	long offset;
	unsigned long addr;
};

static unsigned long update_symbol_cache(struct symbol_cache *sc)
{
	sc->addr = (unsigned long)kallsyms_lookup_name(sc->symbol);
	if (sc->addr)
		sc->addr += sc->offset;
	return sc->addr;
}

static void free_symbol_cache(struct symbol_cache *sc)
{
	kfree(sc->symbol);
	kfree(sc);
}

static struct symbol_cache *alloc_symbol_cache(const char *sym, long offset)
{
	struct symbol_cache *sc;

	if (!sym || strlen(sym) == 0)
		return NULL;
	sc = kzalloc(sizeof(struct symbol_cache), GFP_KERNEL);
	if (!sc)
		return NULL;

	sc->symbol = kstrdup(sym, GFP_KERNEL);
	if (!sc->symbol) {
		kfree(sc);
		return NULL;
	}
	sc->offset = offset;

	update_symbol_cache(sc);
	return sc;
}

#define DEFINE_FETCH_symbol(type)					\
static __kprobes void FETCH_FUNC_NAME(symbol, type)(struct pt_regs *regs,\
					  void *data, void *dest)	\
{									\
	struct symbol_cache *sc = data;					\
	if (sc->addr)							\
		fetch_memory_##type(regs, (void *)sc->addr, dest);	\
	else								\
		*(type *)dest = 0;					\
}
DEFINE_BASIC_FETCH_FUNCS(symbol)
DEFINE_FETCH_symbol(string)
DEFINE_FETCH_symbol(string_size)

/* Dereference memory access function */
struct deref_fetch_param {
	struct fetch_param orig;
	long offset;
};

#define DEFINE_FETCH_deref(type)					\
static __kprobes void FETCH_FUNC_NAME(deref, type)(struct pt_regs *regs,\
					    void *data, void *dest)	\
{									\
	struct deref_fetch_param *dprm = data;				\
	unsigned long addr;						\
	call_fetch(&dprm->orig, regs, &addr);				\
	if (addr) {							\
		addr += dprm->offset;					\
		fetch_memory_##type(regs, (void *)addr, dest);		\
	} else								\
		*(type *)dest = 0;					\
}
DEFINE_BASIC_FETCH_FUNCS(deref)
DEFINE_FETCH_deref(string)
DEFINE_FETCH_deref(string_size)

static __kprobes void update_deref_fetch_param(struct deref_fetch_param *data)
{
	if (CHECK_FETCH_FUNCS(deref, data->orig.fn))
		update_deref_fetch_param(data->orig.data);
	else if (CHECK_FETCH_FUNCS(symbol, data->orig.fn))
		update_symbol_cache(data->orig.data);
}

static __kprobes void free_deref_fetch_param(struct deref_fetch_param *data)
{
	if (CHECK_FETCH_FUNCS(deref, data->orig.fn))
		free_deref_fetch_param(data->orig.data);
	else if (CHECK_FETCH_FUNCS(symbol, data->orig.fn))
		free_symbol_cache(data->orig.data);
	kfree(data);
}

/* Bitfield fetch function */
struct bitfield_fetch_param {
	struct fetch_param orig;
	unsigned char hi_shift;
	unsigned char low_shift;
};

#define DEFINE_FETCH_bitfield(type)					\
static __kprobes void FETCH_FUNC_NAME(bitfield, type)(struct pt_regs *regs,\
					    void *data, void *dest)	\
{									\
	struct bitfield_fetch_param *bprm = data;			\
	type buf = 0;							\
	call_fetch(&bprm->orig, regs, &buf);				\
	if (buf) {							\
		buf <<= bprm->hi_shift;					\
		buf >>= bprm->low_shift;				\
	}								\
	*(type *)dest = buf;						\
}
DEFINE_BASIC_FETCH_FUNCS(bitfield)
#define fetch_bitfield_string NULL
#define fetch_bitfield_string_size NULL

static __kprobes void
update_bitfield_fetch_param(struct bitfield_fetch_param *data)
{
	/*
	 * Don't check the bitfield itself, because this must be the
	 * last fetch function.
	 */
	if (CHECK_FETCH_FUNCS(deref, data->orig.fn))
		update_deref_fetch_param(data->orig.data);
	else if (CHECK_FETCH_FUNCS(symbol, data->orig.fn))
		update_symbol_cache(data->orig.data);
}

static __kprobes void
free_bitfield_fetch_param(struct bitfield_fetch_param *data)
{
	/*
	 * Don't check the bitfield itself, because this must be the
	 * last fetch function.
	 */
	if (CHECK_FETCH_FUNCS(deref, data->orig.fn))
		free_deref_fetch_param(data->orig.data);
	else if (CHECK_FETCH_FUNCS(symbol, data->orig.fn))
		free_symbol_cache(data->orig.data);
	kfree(data);
}

/* Default (unsigned long) fetch type */
#define __DEFAULT_FETCH_TYPE(t) u##t
#define _DEFAULT_FETCH_TYPE(t) __DEFAULT_FETCH_TYPE(t)
#define DEFAULT_FETCH_TYPE _DEFAULT_FETCH_TYPE(BITS_PER_LONG)
#define DEFAULT_FETCH_TYPE_STR __stringify(DEFAULT_FETCH_TYPE)

/* Fetch types */
enum {
	FETCH_MTD_reg = 0,
	FETCH_MTD_stack,
	FETCH_MTD_retval,
	FETCH_MTD_memory,
	FETCH_MTD_symbol,
	FETCH_MTD_deref,
	FETCH_MTD_bitfield,
	FETCH_MTD_END,
};

#define ASSIGN_FETCH_FUNC(method, type)	\
	[FETCH_MTD_##method] = FETCH_FUNC_NAME(method, type)

#define __ASSIGN_FETCH_TYPE(_name, ptype, ftype, _size, sign, _fmttype)	\
	{.name = _name,				\
	 .size = _size,					\
	 .is_signed = sign,				\
	 .print = PRINT_TYPE_FUNC_NAME(ptype),		\
	 .fmt = PRINT_TYPE_FMT_NAME(ptype),		\
	 .fmttype = _fmttype,				\
	 .fetch = {					\
ASSIGN_FETCH_FUNC(reg, ftype),				\
ASSIGN_FETCH_FUNC(stack, ftype),			\
ASSIGN_FETCH_FUNC(retval, ftype),			\
ASSIGN_FETCH_FUNC(memory, ftype),			\
ASSIGN_FETCH_FUNC(symbol, ftype),			\
ASSIGN_FETCH_FUNC(deref, ftype),			\
ASSIGN_FETCH_FUNC(bitfield, ftype),			\
	  }						\
	}

#define ASSIGN_FETCH_TYPE(ptype, ftype, sign)			\
	__ASSIGN_FETCH_TYPE(#ptype, ptype, ftype, sizeof(ftype), sign, #ptype)

#define FETCH_TYPE_STRING 0
#define FETCH_TYPE_STRSIZE 1

/* Fetch type information table */
static const struct fetch_type {
	const char	*name;		/* Name of type */
	size_t		size;		/* Byte size of type */
	int		is_signed;	/* Signed flag */
	print_type_func_t	print;	/* Print functions */
	const char	*fmt;		/* Fromat string */
	const char	*fmttype;	/* Name in format file */
	/* Fetch functions */
	fetch_func_t	fetch[FETCH_MTD_END];
} fetch_type_table[] = {
	/* Special types */
	[FETCH_TYPE_STRING] = __ASSIGN_FETCH_TYPE("string", string, string,
					sizeof(u32), 1, "__data_loc char[]"),
	[FETCH_TYPE_STRSIZE] = __ASSIGN_FETCH_TYPE("string_size", u32,
					string_size, sizeof(u32), 0, "u32"),
	/* Basic types */
	ASSIGN_FETCH_TYPE(u8,  u8,  0),
	ASSIGN_FETCH_TYPE(u16, u16, 0),
	ASSIGN_FETCH_TYPE(u32, u32, 0),
	ASSIGN_FETCH_TYPE(u64, u64, 0),
	ASSIGN_FETCH_TYPE(s8,  u8,  1),
	ASSIGN_FETCH_TYPE(s16, u16, 1),
	ASSIGN_FETCH_TYPE(s32, u32, 1),
	ASSIGN_FETCH_TYPE(s64, u64, 1),
};

static const struct fetch_type *find_fetch_type(const char *type)
{
	int i;

	if (!type)
		type = DEFAULT_FETCH_TYPE_STR;

	/* Special case: bitfield */
	if (*type == 'b') {
		unsigned long bs;
		type = strchr(type, '/');
		if (!type)
			goto fail;
		type++;
		if (strict_strtoul(type, 0, &bs))
			goto fail;
		switch (bs) {
		case 8:
			return find_fetch_type("u8");
		case 16:
			return find_fetch_type("u16");
		case 32:
			return find_fetch_type("u32");
		case 64:
			return find_fetch_type("u64");
		default:
			goto fail;
		}
	}

	for (i = 0; i < ARRAY_SIZE(fetch_type_table); i++)
		if (strcmp(type, fetch_type_table[i].name) == 0)
			return &fetch_type_table[i];
fail:
	return NULL;
}

/* Special function : only accept unsigned long */
static __kprobes void fetch_stack_address(struct pt_regs *regs,
					  void *dummy, void *dest)
{
	*(unsigned long *)dest = kernel_stack_pointer(regs);
}

static fetch_func_t get_fetch_size_function(const struct fetch_type *type,
					    fetch_func_t orig_fn)
{
	int i;

	if (type != &fetch_type_table[FETCH_TYPE_STRING])
		return NULL;	/* Only string type needs size function */
	for (i = 0; i < FETCH_MTD_END; i++)
		if (type->fetch[i] == orig_fn)
			return fetch_type_table[FETCH_TYPE_STRSIZE].fetch[i];

	WARN_ON(1);	/* This should not happen */
	return NULL;
}

/**
 * Kprobe event core functions
 */

struct probe_arg {
	struct fetch_param	fetch;
	struct fetch_param	fetch_size;
	unsigned int		offset;	/* Offset from argument entry */
	const char		*name;	/* Name of this argument */
	const char		*comm;	/* Command of this argument */
	const struct fetch_type	*type;	/* Type of this argument */
};

/* Flags for trace_probe */
#define TP_FLAG_TRACE	1
#define TP_FLAG_PROFILE	2
#define TP_FLAG_REGISTERED 4

struct trace_probe {
	struct list_head	list;
	struct kretprobe	rp;	/* Use rp.kp for kprobe use */
	unsigned long 		nhit;
	unsigned int		flags;	/* For TP_FLAG_* */
	const char		*symbol;	/* symbol name */
	struct ftrace_event_class	class;
	struct ftrace_event_call	call;
	ssize_t			size;		/* trace entry size */
	unsigned int		nr_args;
	struct probe_arg	args[];
};

#define SIZEOF_TRACE_PROBE(n)			\
	(offsetof(struct trace_probe, args) +	\
	(sizeof(struct probe_arg) * (n)))


static __kprobes int trace_probe_is_return(struct trace_probe *tp)
{
	return tp->rp.handler != NULL;
}

static __kprobes const char *trace_probe_symbol(struct trace_probe *tp)
{
	return tp->symbol ? tp->symbol : "unknown";
}

static __kprobes unsigned long trace_probe_offset(struct trace_probe *tp)
{
	return tp->rp.kp.offset;
}

static __kprobes bool trace_probe_is_enabled(struct trace_probe *tp)
{
	return !!(tp->flags & (TP_FLAG_TRACE | TP_FLAG_PROFILE));
}

static __kprobes bool trace_probe_is_registered(struct trace_probe *tp)
{
	return !!(tp->flags & TP_FLAG_REGISTERED);
}

static __kprobes bool trace_probe_has_gone(struct trace_probe *tp)
{
	return !!(kprobe_gone(&tp->rp.kp));
}

static __kprobes bool trace_probe_within_module(struct trace_probe *tp,
						struct module *mod)
{
	int len = strlen(mod->name);
	const char *name = trace_probe_symbol(tp);
	return strncmp(mod->name, name, len) == 0 && name[len] == ':';
}

static __kprobes bool trace_probe_is_on_module(struct trace_probe *tp)
{
	return !!strchr(trace_probe_symbol(tp), ':');
}

static int register_probe_event(struct trace_probe *tp);
static void unregister_probe_event(struct trace_probe *tp);

static DEFINE_MUTEX(probe_lock);
static LIST_HEAD(probe_list);
=======
 */
#define pr_fmt(fmt)	"trace_kprobe: " fmt

#include <linux/bpf-cgroup.h>
#include <linux/security.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/rculist.h>
#include <linux/error-injection.h>

#include <asm/setup.h>  /* for COMMAND_LINE_SIZE */

#include "trace_dynevent.h"
#include "trace_kprobe_selftest.h"
#include "trace_probe.h"
#include "trace_probe_tmpl.h"
#include "trace_probe_kernel.h"

#define KPROBE_EVENT_SYSTEM "kprobes"
#define KRETPROBE_MAXACTIVE_MAX 4096

/* Kprobe early definition from command line */
static char kprobe_boot_events_buf[COMMAND_LINE_SIZE] __initdata;

static int __init set_kprobe_boot_events(char *str)
{
	strscpy(kprobe_boot_events_buf, str, COMMAND_LINE_SIZE);
	disable_tracing_selftest("running kprobe events");

	return 1;
}
__setup("kprobe_event=", set_kprobe_boot_events);

static int trace_kprobe_create(const char *raw_command);
static int trace_kprobe_show(struct seq_file *m, struct dyn_event *ev);
static int trace_kprobe_release(struct dyn_event *ev);
static bool trace_kprobe_is_busy(struct dyn_event *ev);
static bool trace_kprobe_match(const char *system, const char *event,
			int argc, const char **argv, struct dyn_event *ev);

static struct dyn_event_operations trace_kprobe_ops = {
	.create = trace_kprobe_create,
	.show = trace_kprobe_show,
	.is_busy = trace_kprobe_is_busy,
	.free = trace_kprobe_release,
	.match = trace_kprobe_match,
};

/*
 * Kprobe event core functions
 */
struct trace_kprobe {
	struct dyn_event	devent;
	struct kretprobe	rp;	/* Use rp.kp for kprobe use */
	unsigned long __percpu *nhit;
	const char		*symbol;	/* symbol name */
	struct trace_probe	tp;
};

static bool is_trace_kprobe(struct dyn_event *ev)
{
	return ev->ops == &trace_kprobe_ops;
}

static struct trace_kprobe *to_trace_kprobe(struct dyn_event *ev)
{
	return container_of(ev, struct trace_kprobe, devent);
}

/**
 * for_each_trace_kprobe - iterate over the trace_kprobe list
 * @pos:	the struct trace_kprobe * for each entry
 * @dpos:	the struct dyn_event * to use as a loop cursor
 */
#define for_each_trace_kprobe(pos, dpos)	\
	for_each_dyn_event(dpos)		\
		if (is_trace_kprobe(dpos) && (pos = to_trace_kprobe(dpos)))

static nokprobe_inline bool trace_kprobe_is_return(struct trace_kprobe *tk)
{
	return tk->rp.handler != NULL;
}

static nokprobe_inline const char *trace_kprobe_symbol(struct trace_kprobe *tk)
{
	return tk->symbol ? tk->symbol : "unknown";
}

static nokprobe_inline unsigned long trace_kprobe_offset(struct trace_kprobe *tk)
{
	return tk->rp.kp.offset;
}

static nokprobe_inline bool trace_kprobe_has_gone(struct trace_kprobe *tk)
{
	return kprobe_gone(&tk->rp.kp);
}

static nokprobe_inline bool trace_kprobe_within_module(struct trace_kprobe *tk,
						 struct module *mod)
{
	int len = strlen(module_name(mod));
	const char *name = trace_kprobe_symbol(tk);

	return strncmp(module_name(mod), name, len) == 0 && name[len] == ':';
}

static nokprobe_inline bool trace_kprobe_module_exist(struct trace_kprobe *tk)
{
	char *p;
	bool ret;

	if (!tk->symbol)
		return false;
	p = strchr(tk->symbol, ':');
	if (!p)
		return true;
	*p = '\0';
	rcu_read_lock_sched();
	ret = !!find_module(tk->symbol);
	rcu_read_unlock_sched();
	*p = ':';

	return ret;
}

static bool trace_kprobe_is_busy(struct dyn_event *ev)
{
	struct trace_kprobe *tk = to_trace_kprobe(ev);

	return trace_probe_is_enabled(&tk->tp);
}

static bool trace_kprobe_match_command_head(struct trace_kprobe *tk,
					    int argc, const char **argv)
{
	char buf[MAX_ARGSTR_LEN + 1];

	if (!argc)
		return true;

	if (!tk->symbol)
		snprintf(buf, sizeof(buf), "0x%p", tk->rp.kp.addr);
	else if (tk->rp.kp.offset)
		snprintf(buf, sizeof(buf), "%s+%u",
			 trace_kprobe_symbol(tk), tk->rp.kp.offset);
	else
		snprintf(buf, sizeof(buf), "%s", trace_kprobe_symbol(tk));
	if (strcmp(buf, argv[0]))
		return false;
	argc--; argv++;

	return trace_probe_match_command_args(&tk->tp, argc, argv);
}

static bool trace_kprobe_match(const char *system, const char *event,
			int argc, const char **argv, struct dyn_event *ev)
{
	struct trace_kprobe *tk = to_trace_kprobe(ev);

	return (event[0] == '\0' ||
		strcmp(trace_probe_name(&tk->tp), event) == 0) &&
	    (!system || strcmp(trace_probe_group_name(&tk->tp), system) == 0) &&
	    trace_kprobe_match_command_head(tk, argc, argv);
}

static nokprobe_inline unsigned long trace_kprobe_nhit(struct trace_kprobe *tk)
{
	unsigned long nhit = 0;
	int cpu;

	for_each_possible_cpu(cpu)
		nhit += *per_cpu_ptr(tk->nhit, cpu);

	return nhit;
}

static nokprobe_inline bool trace_kprobe_is_registered(struct trace_kprobe *tk)
{
	return !(list_empty(&tk->rp.kp.list) &&
		 hlist_unhashed(&tk->rp.kp.hlist));
}

/* Return 0 if it fails to find the symbol address */
static nokprobe_inline
unsigned long trace_kprobe_address(struct trace_kprobe *tk)
{
	unsigned long addr;

	if (tk->symbol) {
		addr = (unsigned long)
			kallsyms_lookup_name(trace_kprobe_symbol(tk));
		if (addr)
			addr += tk->rp.kp.offset;
	} else {
		addr = (unsigned long)tk->rp.kp.addr;
	}
	return addr;
}

static nokprobe_inline struct trace_kprobe *
trace_kprobe_primary_from_call(struct trace_event_call *call)
{
	struct trace_probe *tp;

	tp = trace_probe_primary_from_call(call);
	if (WARN_ON_ONCE(!tp))
		return NULL;

	return container_of(tp, struct trace_kprobe, tp);
}

bool trace_kprobe_on_func_entry(struct trace_event_call *call)
{
	struct trace_kprobe *tk = trace_kprobe_primary_from_call(call);

	return tk ? (kprobe_on_func_entry(tk->rp.kp.addr,
			tk->rp.kp.addr ? NULL : tk->rp.kp.symbol_name,
			tk->rp.kp.addr ? 0 : tk->rp.kp.offset) == 0) : false;
}

bool trace_kprobe_error_injectable(struct trace_event_call *call)
{
	struct trace_kprobe *tk = trace_kprobe_primary_from_call(call);

	return tk ? within_error_injection_list(trace_kprobe_address(tk)) :
	       false;
}

static int register_kprobe_event(struct trace_kprobe *tk);
static int unregister_kprobe_event(struct trace_kprobe *tk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int kprobe_dispatcher(struct kprobe *kp, struct pt_regs *regs);
static int kretprobe_dispatcher(struct kretprobe_instance *ri,
				struct pt_regs *regs);

<<<<<<< HEAD
/* Check the name is good for event/group/fields */
static int is_good_name(const char *name)
{
	if (!isalpha(*name) && *name != '_')
		return 0;
	while (*++name != '\0') {
		if (!isalpha(*name) && !isdigit(*name) && *name != '_')
			return 0;
	}
	return 1;
=======
static void free_trace_kprobe(struct trace_kprobe *tk)
{
	if (tk) {
		trace_probe_cleanup(&tk->tp);
		kfree(tk->symbol);
		free_percpu(tk->nhit);
		kfree(tk);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Allocate new trace_probe and initialize it (including kprobes).
 */
<<<<<<< HEAD
static struct trace_probe *alloc_trace_probe(const char *group,
=======
static struct trace_kprobe *alloc_trace_kprobe(const char *group,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					     const char *event,
					     void *addr,
					     const char *symbol,
					     unsigned long offs,
<<<<<<< HEAD
					     int nargs, int is_return)
{
	struct trace_probe *tp;
	int ret = -ENOMEM;

	tp = kzalloc(SIZEOF_TRACE_PROBE(nargs), GFP_KERNEL);
	if (!tp)
		return ERR_PTR(ret);

	if (symbol) {
		tp->symbol = kstrdup(symbol, GFP_KERNEL);
		if (!tp->symbol)
			goto error;
		tp->rp.kp.symbol_name = tp->symbol;
		tp->rp.kp.offset = offs;
	} else
		tp->rp.kp.addr = addr;

	if (is_return)
		tp->rp.handler = kretprobe_dispatcher;
	else
		tp->rp.kp.pre_handler = kprobe_dispatcher;

	if (!event || !is_good_name(event)) {
		ret = -EINVAL;
		goto error;
	}

	tp->call.class = &tp->class;
	tp->call.name = kstrdup(event, GFP_KERNEL);
	if (!tp->call.name)
		goto error;

	if (!group || !is_good_name(group)) {
		ret = -EINVAL;
		goto error;
	}

	tp->class.system = kstrdup(group, GFP_KERNEL);
	if (!tp->class.system)
		goto error;

	INIT_LIST_HEAD(&tp->list);
	return tp;
error:
	kfree(tp->call.name);
	kfree(tp->symbol);
	kfree(tp);
	return ERR_PTR(ret);
}

static void update_probe_arg(struct probe_arg *arg)
{
	if (CHECK_FETCH_FUNCS(bitfield, arg->fetch.fn))
		update_bitfield_fetch_param(arg->fetch.data);
	else if (CHECK_FETCH_FUNCS(deref, arg->fetch.fn))
		update_deref_fetch_param(arg->fetch.data);
	else if (CHECK_FETCH_FUNCS(symbol, arg->fetch.fn))
		update_symbol_cache(arg->fetch.data);
}

static void free_probe_arg(struct probe_arg *arg)
{
	if (CHECK_FETCH_FUNCS(bitfield, arg->fetch.fn))
		free_bitfield_fetch_param(arg->fetch.data);
	else if (CHECK_FETCH_FUNCS(deref, arg->fetch.fn))
		free_deref_fetch_param(arg->fetch.data);
	else if (CHECK_FETCH_FUNCS(symbol, arg->fetch.fn))
		free_symbol_cache(arg->fetch.data);
	kfree(arg->name);
	kfree(arg->comm);
}

static void free_trace_probe(struct trace_probe *tp)
{
	int i;

	for (i = 0; i < tp->nr_args; i++)
		free_probe_arg(&tp->args[i]);

	kfree(tp->call.class->system);
	kfree(tp->call.name);
	kfree(tp->symbol);
	kfree(tp);
}

static struct trace_probe *find_trace_probe(const char *event,
					    const char *group)
{
	struct trace_probe *tp;

	list_for_each_entry(tp, &probe_list, list)
		if (strcmp(tp->call.name, event) == 0 &&
		    strcmp(tp->call.class->system, group) == 0)
			return tp;
	return NULL;
}

/* Enable trace_probe - @flag must be TP_FLAG_TRACE or TP_FLAG_PROFILE */
static int enable_trace_probe(struct trace_probe *tp, int flag)
{
	int ret = 0;

	tp->flags |= flag;
	if (trace_probe_is_enabled(tp) && trace_probe_is_registered(tp) &&
	    !trace_probe_has_gone(tp)) {
		if (trace_probe_is_return(tp))
			ret = enable_kretprobe(&tp->rp);
		else
			ret = enable_kprobe(&tp->rp.kp);
=======
					     int maxactive,
					     int nargs, bool is_return)
{
	struct trace_kprobe *tk;
	int ret = -ENOMEM;

	tk = kzalloc(struct_size(tk, tp.args, nargs), GFP_KERNEL);
	if (!tk)
		return ERR_PTR(ret);

	tk->nhit = alloc_percpu(unsigned long);
	if (!tk->nhit)
		goto error;

	if (symbol) {
		tk->symbol = kstrdup(symbol, GFP_KERNEL);
		if (!tk->symbol)
			goto error;
		tk->rp.kp.symbol_name = tk->symbol;
		tk->rp.kp.offset = offs;
	} else
		tk->rp.kp.addr = addr;

	if (is_return)
		tk->rp.handler = kretprobe_dispatcher;
	else
		tk->rp.kp.pre_handler = kprobe_dispatcher;

	tk->rp.maxactive = maxactive;
	INIT_HLIST_NODE(&tk->rp.kp.hlist);
	INIT_LIST_HEAD(&tk->rp.kp.list);

	ret = trace_probe_init(&tk->tp, event, group, false, nargs);
	if (ret < 0)
		goto error;

	dyn_event_init(&tk->devent, &trace_kprobe_ops);
	return tk;
error:
	free_trace_kprobe(tk);
	return ERR_PTR(ret);
}

static struct trace_kprobe *find_trace_kprobe(const char *event,
					      const char *group)
{
	struct dyn_event *pos;
	struct trace_kprobe *tk;

	for_each_trace_kprobe(tk, pos)
		if (strcmp(trace_probe_name(&tk->tp), event) == 0 &&
		    strcmp(trace_probe_group_name(&tk->tp), group) == 0)
			return tk;
	return NULL;
}

static inline int __enable_trace_kprobe(struct trace_kprobe *tk)
{
	int ret = 0;

	if (trace_kprobe_is_registered(tk) && !trace_kprobe_has_gone(tk)) {
		if (trace_kprobe_is_return(tk))
			ret = enable_kretprobe(&tk->rp);
		else
			ret = enable_kprobe(&tk->rp.kp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return ret;
}

<<<<<<< HEAD
/* Disable trace_probe - @flag must be TP_FLAG_TRACE or TP_FLAG_PROFILE */
static void disable_trace_probe(struct trace_probe *tp, int flag)
{
	tp->flags &= ~flag;
	if (!trace_probe_is_enabled(tp) && trace_probe_is_registered(tp)) {
		if (trace_probe_is_return(tp))
			disable_kretprobe(&tp->rp);
		else
			disable_kprobe(&tp->rp.kp);
	}
}

/* Internal register function - just handle k*probes and flags */
static int __register_trace_probe(struct trace_probe *tp)
{
	int i, ret;

	if (trace_probe_is_registered(tp))
		return -EINVAL;

	for (i = 0; i < tp->nr_args; i++)
		update_probe_arg(&tp->args[i]);

	/* Set/clear disabled flag according to tp->flag */
	if (trace_probe_is_enabled(tp))
		tp->rp.kp.flags &= ~KPROBE_FLAG_DISABLED;
	else
		tp->rp.kp.flags |= KPROBE_FLAG_DISABLED;

	if (trace_probe_is_return(tp))
		ret = register_kretprobe(&tp->rp);
	else
		ret = register_kprobe(&tp->rp.kp);

	if (ret == 0)
		tp->flags |= TP_FLAG_REGISTERED;
	else {
		pr_warning("Could not insert probe at %s+%lu: %d\n",
			   trace_probe_symbol(tp), trace_probe_offset(tp), ret);
		if (ret == -ENOENT && trace_probe_is_on_module(tp)) {
			pr_warning("This probe might be able to register after"
				   "target module is loaded. Continue.\n");
			ret = 0;
		} else if (ret == -EILSEQ) {
			pr_warning("Probing address(0x%p) is not an "
				   "instruction boundary.\n",
				   tp->rp.kp.addr);
			ret = -EINVAL;
		}
	}
=======
static void __disable_trace_kprobe(struct trace_probe *tp)
{
	struct trace_kprobe *tk;

	list_for_each_entry(tk, trace_probe_probe_list(tp), tp.list) {
		if (!trace_kprobe_is_registered(tk))
			continue;
		if (trace_kprobe_is_return(tk))
			disable_kretprobe(&tk->rp);
		else
			disable_kprobe(&tk->rp.kp);
	}
}

/*
 * Enable trace_probe
 * if the file is NULL, enable "perf" handler, or enable "trace" handler.
 */
static int enable_trace_kprobe(struct trace_event_call *call,
				struct trace_event_file *file)
{
	struct trace_probe *tp;
	struct trace_kprobe *tk;
	bool enabled;
	int ret = 0;

	tp = trace_probe_primary_from_call(call);
	if (WARN_ON_ONCE(!tp))
		return -ENODEV;
	enabled = trace_probe_is_enabled(tp);

	/* This also changes "enabled" state */
	if (file) {
		ret = trace_probe_add_file(tp, file);
		if (ret)
			return ret;
	} else
		trace_probe_set_flag(tp, TP_FLAG_PROFILE);

	if (enabled)
		return 0;

	list_for_each_entry(tk, trace_probe_probe_list(tp), tp.list) {
		if (trace_kprobe_has_gone(tk))
			continue;
		ret = __enable_trace_kprobe(tk);
		if (ret)
			break;
		enabled = true;
	}

	if (ret) {
		/* Failed to enable one of them. Roll back all */
		if (enabled)
			__disable_trace_kprobe(tp);
		if (file)
			trace_probe_remove_file(tp, file);
		else
			trace_probe_clear_flag(tp, TP_FLAG_PROFILE);
	}

	return ret;
}

/*
 * Disable trace_probe
 * if the file is NULL, disable "perf" handler, or disable "trace" handler.
 */
static int disable_trace_kprobe(struct trace_event_call *call,
				struct trace_event_file *file)
{
	struct trace_probe *tp;

	tp = trace_probe_primary_from_call(call);
	if (WARN_ON_ONCE(!tp))
		return -ENODEV;

	if (file) {
		if (!trace_probe_get_file_link(tp, file))
			return -ENOENT;
		if (!trace_probe_has_single_file(tp))
			goto out;
		trace_probe_clear_flag(tp, TP_FLAG_TRACE);
	} else
		trace_probe_clear_flag(tp, TP_FLAG_PROFILE);

	if (!trace_probe_is_enabled(tp))
		__disable_trace_kprobe(tp);

 out:
	if (file)
		/*
		 * Synchronization is done in below function. For perf event,
		 * file == NULL and perf_trace_event_unreg() calls
		 * tracepoint_synchronize_unregister() to ensure synchronize
		 * event. We don't need to care about it.
		 */
		trace_probe_remove_file(tp, file);

	return 0;
}

#if defined(CONFIG_DYNAMIC_FTRACE) && \
	!defined(CONFIG_KPROBE_EVENTS_ON_NOTRACE)
static bool __within_notrace_func(unsigned long addr)
{
	unsigned long offset, size;

	if (!addr || !kallsyms_lookup_size_offset(addr, &size, &offset))
		return false;

	/* Get the entry address of the target function */
	addr -= offset;

	/*
	 * Since ftrace_location_range() does inclusive range check, we need
	 * to subtract 1 byte from the end address.
	 */
	return !ftrace_location_range(addr, addr + size - 1);
}

static bool within_notrace_func(struct trace_kprobe *tk)
{
	unsigned long addr = trace_kprobe_address(tk);
	char symname[KSYM_NAME_LEN], *p;

	if (!__within_notrace_func(addr))
		return false;

	/* Check if the address is on a suffixed-symbol */
	if (!lookup_symbol_name(addr, symname)) {
		p = strchr(symname, '.');
		if (!p)
			return true;
		*p = '\0';
		addr = (unsigned long)kprobe_lookup_name(symname, 0);
		if (addr)
			return __within_notrace_func(addr);
	}

	return true;
}
#else
#define within_notrace_func(tk)	(false)
#endif

/* Internal register function - just handle k*probes and flags */
static int __register_trace_kprobe(struct trace_kprobe *tk)
{
	int i, ret;

	ret = security_locked_down(LOCKDOWN_KPROBES);
	if (ret)
		return ret;

	if (trace_kprobe_is_registered(tk))
		return -EINVAL;

	if (within_notrace_func(tk)) {
		pr_warn("Could not probe notrace function %ps\n",
			(void *)trace_kprobe_address(tk));
		return -EINVAL;
	}

	for (i = 0; i < tk->tp.nr_args; i++) {
		ret = traceprobe_update_arg(&tk->tp.args[i]);
		if (ret)
			return ret;
	}

	/* Set/clear disabled flag according to tp->flag */
	if (trace_probe_is_enabled(&tk->tp))
		tk->rp.kp.flags &= ~KPROBE_FLAG_DISABLED;
	else
		tk->rp.kp.flags |= KPROBE_FLAG_DISABLED;

	if (trace_kprobe_is_return(tk))
		ret = register_kretprobe(&tk->rp);
	else
		ret = register_kprobe(&tk->rp.kp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

/* Internal unregister function - just handle k*probes and flags */
<<<<<<< HEAD
static void __unregister_trace_probe(struct trace_probe *tp)
{
	if (trace_probe_is_registered(tp)) {
		if (trace_probe_is_return(tp))
			unregister_kretprobe(&tp->rp);
		else
			unregister_kprobe(&tp->rp.kp);
		tp->flags &= ~TP_FLAG_REGISTERED;
		/* Cleanup kprobe for reuse */
		if (tp->rp.kp.symbol_name)
			tp->rp.kp.addr = NULL;
	}
}

/* Unregister a trace_probe and probe_event: call with locking probe_lock */
static int unregister_trace_probe(struct trace_probe *tp)
{
	/* Enabled event can not be unregistered */
	if (trace_probe_is_enabled(tp))
		return -EBUSY;

	__unregister_trace_probe(tp);
	list_del(&tp->list);
	unregister_probe_event(tp);
=======
static void __unregister_trace_kprobe(struct trace_kprobe *tk)
{
	if (trace_kprobe_is_registered(tk)) {
		if (trace_kprobe_is_return(tk))
			unregister_kretprobe(&tk->rp);
		else
			unregister_kprobe(&tk->rp.kp);
		/* Cleanup kprobe for reuse and mark it unregistered */
		INIT_HLIST_NODE(&tk->rp.kp.hlist);
		INIT_LIST_HEAD(&tk->rp.kp.list);
		if (tk->rp.kp.symbol_name)
			tk->rp.kp.addr = NULL;
	}
}

/* Unregister a trace_probe and probe_event */
static int unregister_trace_kprobe(struct trace_kprobe *tk)
{
	/* If other probes are on the event, just unregister kprobe */
	if (trace_probe_has_sibling(&tk->tp))
		goto unreg;

	/* Enabled event can not be unregistered */
	if (trace_probe_is_enabled(&tk->tp))
		return -EBUSY;

	/* If there's a reference to the dynamic event */
	if (trace_event_dyn_busy(trace_probe_event_call(&tk->tp)))
		return -EBUSY;

	/* Will fail if probe is being used by ftrace or perf */
	if (unregister_kprobe_event(tk))
		return -EBUSY;

unreg:
	__unregister_trace_kprobe(tk);
	dyn_event_remove(&tk->devent);
	trace_probe_unlink(&tk->tp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
/* Register a trace_probe and probe_event */
static int register_trace_probe(struct trace_probe *tp)
{
	struct trace_probe *old_tp;
	int ret;

	mutex_lock(&probe_lock);

	/* Delete old (same name) event if exist */
	old_tp = find_trace_probe(tp->call.name, tp->call.class->system);
	if (old_tp) {
		ret = unregister_trace_probe(old_tp);
		if (ret < 0)
			goto end;
		free_trace_probe(old_tp);
	}

	/* Register new event */
	ret = register_probe_event(tp);
	if (ret) {
		pr_warning("Failed to register probe event(%d)\n", ret);
=======
static bool trace_kprobe_has_same_kprobe(struct trace_kprobe *orig,
					 struct trace_kprobe *comp)
{
	struct trace_probe_event *tpe = orig->tp.event;
	int i;

	list_for_each_entry(orig, &tpe->probes, tp.list) {
		if (strcmp(trace_kprobe_symbol(orig),
			   trace_kprobe_symbol(comp)) ||
		    trace_kprobe_offset(orig) != trace_kprobe_offset(comp))
			continue;

		/*
		 * trace_probe_compare_arg_type() ensured that nr_args and
		 * each argument name and type are same. Let's compare comm.
		 */
		for (i = 0; i < orig->tp.nr_args; i++) {
			if (strcmp(orig->tp.args[i].comm,
				   comp->tp.args[i].comm))
				break;
		}

		if (i == orig->tp.nr_args)
			return true;
	}

	return false;
}

static int append_trace_kprobe(struct trace_kprobe *tk, struct trace_kprobe *to)
{
	int ret;

	ret = trace_probe_compare_arg_type(&tk->tp, &to->tp);
	if (ret) {
		/* Note that argument starts index = 2 */
		trace_probe_log_set_index(ret + 1);
		trace_probe_log_err(0, DIFF_ARG_TYPE);
		return -EEXIST;
	}
	if (trace_kprobe_has_same_kprobe(to, tk)) {
		trace_probe_log_set_index(0);
		trace_probe_log_err(0, SAME_PROBE);
		return -EEXIST;
	}

	/* Append to existing event */
	ret = trace_probe_append(&tk->tp, &to->tp);
	if (ret)
		return ret;

	/* Register k*probe */
	ret = __register_trace_kprobe(tk);
	if (ret == -ENOENT && !trace_kprobe_module_exist(tk)) {
		pr_warn("This probe might be able to register after target module is loaded. Continue.\n");
		ret = 0;
	}

	if (ret)
		trace_probe_unlink(&tk->tp);
	else
		dyn_event_add(&tk->devent, trace_probe_event_call(&tk->tp));

	return ret;
}

/* Register a trace_probe and probe_event */
static int register_trace_kprobe(struct trace_kprobe *tk)
{
	struct trace_kprobe *old_tk;
	int ret;

	mutex_lock(&event_mutex);

	old_tk = find_trace_kprobe(trace_probe_name(&tk->tp),
				   trace_probe_group_name(&tk->tp));
	if (old_tk) {
		if (trace_kprobe_is_return(tk) != trace_kprobe_is_return(old_tk)) {
			trace_probe_log_set_index(0);
			trace_probe_log_err(0, DIFF_PROBE_TYPE);
			ret = -EEXIST;
		} else {
			ret = append_trace_kprobe(tk, old_tk);
		}
		goto end;
	}

	/* Register new event */
	ret = register_kprobe_event(tk);
	if (ret) {
		if (ret == -EEXIST) {
			trace_probe_log_set_index(0);
			trace_probe_log_err(0, EVENT_EXIST);
		} else
			pr_warn("Failed to register probe event(%d)\n", ret);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto end;
	}

	/* Register k*probe */
<<<<<<< HEAD
	ret = __register_trace_probe(tp);
	if (ret < 0)
		unregister_probe_event(tp);
	else
		list_add_tail(&tp->list, &probe_list);

end:
	mutex_unlock(&probe_lock);
=======
	ret = __register_trace_kprobe(tk);
	if (ret == -ENOENT && !trace_kprobe_module_exist(tk)) {
		pr_warn("This probe might be able to register after target module is loaded. Continue.\n");
		ret = 0;
	}

	if (ret < 0)
		unregister_kprobe_event(tk);
	else
		dyn_event_add(&tk->devent, trace_probe_event_call(&tk->tp));

end:
	mutex_unlock(&event_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/* Module notifier call back, checking event on the module */
<<<<<<< HEAD
static int trace_probe_module_callback(struct notifier_block *nb,
				       unsigned long val, void *data)
{
	struct module *mod = data;
	struct trace_probe *tp;
=======
static int trace_kprobe_module_callback(struct notifier_block *nb,
				       unsigned long val, void *data)
{
	struct module *mod = data;
	struct dyn_event *pos;
	struct trace_kprobe *tk;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret;

	if (val != MODULE_STATE_COMING)
		return NOTIFY_DONE;

	/* Update probes on coming module */
<<<<<<< HEAD
	mutex_lock(&probe_lock);
	list_for_each_entry(tp, &probe_list, list) {
		if (trace_probe_within_module(tp, mod)) {
			/* Don't need to check busy - this should have gone. */
			__unregister_trace_probe(tp);
			ret = __register_trace_probe(tp);
			if (ret)
				pr_warning("Failed to re-register probe %s on"
					   "%s: %d\n",
					   tp->call.name, mod->name, ret);
		}
	}
	mutex_unlock(&probe_lock);
=======
	mutex_lock(&event_mutex);
	for_each_trace_kprobe(tk, pos) {
		if (trace_kprobe_within_module(tk, mod)) {
			/* Don't need to check busy - this should have gone. */
			__unregister_trace_kprobe(tk);
			ret = __register_trace_kprobe(tk);
			if (ret)
				pr_warn("Failed to re-register probe %s on %s: %d\n",
					trace_probe_name(&tk->tp),
					module_name(mod), ret);
		}
	}
	mutex_unlock(&event_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return NOTIFY_DONE;
}

<<<<<<< HEAD
static struct notifier_block trace_probe_module_nb = {
	.notifier_call = trace_probe_module_callback,
	.priority = 1	/* Invoked after kprobe module callback */
};

/* Split symbol and offset. */
static int split_symbol_offset(char *symbol, unsigned long *offset)
{
	char *tmp;
	int ret;

	if (!offset)
		return -EINVAL;

	tmp = strchr(symbol, '+');
	if (tmp) {
		/* skip sign because strict_strtol doesn't accept '+' */
		ret = strict_strtoul(tmp + 1, 0, offset);
		if (ret)
			return ret;
		*tmp = '\0';
	} else
		*offset = 0;
	return 0;
}

#define PARAM_MAX_ARGS 16
#define PARAM_MAX_STACK (THREAD_SIZE / sizeof(unsigned long))

static int parse_probe_vars(char *arg, const struct fetch_type *t,
			    struct fetch_param *f, int is_return)
{
	int ret = 0;
	unsigned long param;

	if (strcmp(arg, "retval") == 0) {
		if (is_return)
			f->fn = t->fetch[FETCH_MTD_retval];
		else
			ret = -EINVAL;
	} else if (strncmp(arg, "stack", 5) == 0) {
		if (arg[5] == '\0') {
			if (strcmp(t->name, DEFAULT_FETCH_TYPE_STR) == 0)
				f->fn = fetch_stack_address;
			else
				ret = -EINVAL;
		} else if (isdigit(arg[5])) {
			ret = strict_strtoul(arg + 5, 10, &param);
			if (ret || param > PARAM_MAX_STACK)
				ret = -EINVAL;
			else {
				f->fn = t->fetch[FETCH_MTD_stack];
				f->data = (void *)param;
			}
		} else
			ret = -EINVAL;
	} else
		ret = -EINVAL;
	return ret;
}

/* Recursive argument parser */
static int __parse_probe_arg(char *arg, const struct fetch_type *t,
			     struct fetch_param *f, int is_return)
{
	int ret = 0;
	unsigned long param;
	long offset;
	char *tmp;

	switch (arg[0]) {
	case '$':
		ret = parse_probe_vars(arg + 1, t, f, is_return);
		break;
	case '%':	/* named register */
		ret = regs_query_register_offset(arg + 1);
		if (ret >= 0) {
			f->fn = t->fetch[FETCH_MTD_reg];
			f->data = (void *)(unsigned long)ret;
			ret = 0;
		}
		break;
	case '@':	/* memory or symbol */
		if (isdigit(arg[1])) {
			ret = strict_strtoul(arg + 1, 0, &param);
			if (ret)
				break;
			f->fn = t->fetch[FETCH_MTD_memory];
			f->data = (void *)param;
		} else {
			ret = split_symbol_offset(arg + 1, &offset);
			if (ret)
				break;
			f->data = alloc_symbol_cache(arg + 1, offset);
			if (f->data)
				f->fn = t->fetch[FETCH_MTD_symbol];
		}
		break;
	case '+':	/* deref memory */
		arg++;	/* Skip '+', because strict_strtol() rejects it. */
	case '-':
		tmp = strchr(arg, '(');
		if (!tmp)
			break;
		*tmp = '\0';
		ret = strict_strtol(arg, 0, &offset);
		if (ret)
			break;
		arg = tmp + 1;
		tmp = strrchr(arg, ')');
		if (tmp) {
			struct deref_fetch_param *dprm;
			const struct fetch_type *t2 = find_fetch_type(NULL);
			*tmp = '\0';
			dprm = kzalloc(sizeof(struct deref_fetch_param),
				       GFP_KERNEL);
			if (!dprm)
				return -ENOMEM;
			dprm->offset = offset;
			ret = __parse_probe_arg(arg, t2, &dprm->orig,
						is_return);
			if (ret)
				kfree(dprm);
			else {
				f->fn = t->fetch[FETCH_MTD_deref];
				f->data = (void *)dprm;
			}
		}
		break;
	}
	if (!ret && !f->fn) {	/* Parsed, but do not find fetch method */
		pr_info("%s type has no corresponding fetch method.\n",
			t->name);
		ret = -EINVAL;
	}
	return ret;
}

#define BYTES_TO_BITS(nb)	((BITS_PER_LONG * (nb)) / sizeof(long))

/* Bitfield type needs to be parsed into a fetch function */
static int __parse_bitfield_probe_arg(const char *bf,
				      const struct fetch_type *t,
				      struct fetch_param *f)
{
	struct bitfield_fetch_param *bprm;
	unsigned long bw, bo;
	char *tail;

	if (*bf != 'b')
		return 0;

	bprm = kzalloc(sizeof(*bprm), GFP_KERNEL);
	if (!bprm)
		return -ENOMEM;
	bprm->orig = *f;
	f->fn = t->fetch[FETCH_MTD_bitfield];
	f->data = (void *)bprm;

	bw = simple_strtoul(bf + 1, &tail, 0);	/* Use simple one */
	if (bw == 0 || *tail != '@')
		return -EINVAL;

	bf = tail + 1;
	bo = simple_strtoul(bf, &tail, 0);
	if (tail == bf || *tail != '/')
		return -EINVAL;

	bprm->hi_shift = BYTES_TO_BITS(t->size) - (bw + bo);
	bprm->low_shift = bprm->hi_shift + bo;
	return (BYTES_TO_BITS(t->size) < (bw + bo)) ? -EINVAL : 0;
}

/* String length checking wrapper */
static int parse_probe_arg(char *arg, struct trace_probe *tp,
			   struct probe_arg *parg, int is_return)
{
	const char *t;
	int ret;

	if (strlen(arg) > MAX_ARGSTR_LEN) {
		pr_info("Argument is too long.: %s\n",  arg);
		return -ENOSPC;
	}
	parg->comm = kstrdup(arg, GFP_KERNEL);
	if (!parg->comm) {
		pr_info("Failed to allocate memory for command '%s'.\n", arg);
		return -ENOMEM;
	}
	t = strchr(parg->comm, ':');
	if (t) {
		arg[t - parg->comm] = '\0';
		t++;
	}
	parg->type = find_fetch_type(t);
	if (!parg->type) {
		pr_info("Unsupported type: %s\n", t);
		return -EINVAL;
	}
	parg->offset = tp->size;
	tp->size += parg->type->size;
	ret = __parse_probe_arg(arg, parg->type, &parg->fetch, is_return);
	if (ret >= 0 && t != NULL)
		ret = __parse_bitfield_probe_arg(t, parg->type, &parg->fetch);
	if (ret >= 0) {
		parg->fetch_size.fn = get_fetch_size_function(parg->type,
							      parg->fetch.fn);
		parg->fetch_size.data = parg->fetch.data;
	}
	return ret;
}

/* Return 1 if name is reserved or already used by another argument */
static int conflict_field_name(const char *name,
			       struct probe_arg *args, int narg)
{
	int i;
	for (i = 0; i < ARRAY_SIZE(reserved_field_names); i++)
		if (strcmp(reserved_field_names[i], name) == 0)
			return 1;
	for (i = 0; i < narg; i++)
		if (strcmp(args[i].name, name) == 0)
			return 1;
	return 0;
}

static int create_trace_probe(int argc, char **argv)
{
	/*
	 * Argument syntax:
	 *  - Add kprobe: p[:[GRP/]EVENT] [MOD:]KSYM[+OFFS]|KADDR [FETCHARGS]
	 *  - Add kretprobe: r[:[GRP/]EVENT] [MOD:]KSYM[+0] [FETCHARGS]
=======
static struct notifier_block trace_kprobe_module_nb = {
	.notifier_call = trace_kprobe_module_callback,
	.priority = 1	/* Invoked after kprobe module callback */
};

static int count_symbols(void *data, unsigned long unused)
{
	unsigned int *count = data;

	(*count)++;

	return 0;
}

struct sym_count_ctx {
	unsigned int count;
	const char *name;
};

static int count_mod_symbols(void *data, const char *name, unsigned long unused)
{
	struct sym_count_ctx *ctx = data;

	if (strcmp(name, ctx->name) == 0)
		ctx->count++;

	return 0;
}

static unsigned int number_of_same_symbols(char *func_name)
{
	struct sym_count_ctx ctx = { .count = 0, .name = func_name };

	kallsyms_on_each_match_symbol(count_symbols, func_name, &ctx.count);

	module_kallsyms_on_each_symbol(NULL, count_mod_symbols, &ctx);

	return ctx.count;
}

static int trace_kprobe_entry_handler(struct kretprobe_instance *ri,
				      struct pt_regs *regs);

static int __trace_kprobe_create(int argc, const char *argv[])
{
	/*
	 * Argument syntax:
	 *  - Add kprobe:
	 *      p[:[GRP/][EVENT]] [MOD:]KSYM[+OFFS]|KADDR [FETCHARGS]
	 *  - Add kretprobe:
	 *      r[MAXACTIVE][:[GRP/][EVENT]] [MOD:]KSYM[+0] [FETCHARGS]
	 *    Or
	 *      p[:[GRP/][EVENT]] [MOD:]KSYM[+0]%return [FETCHARGS]
	 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * Fetch args:
	 *  $retval	: fetch return value
	 *  $stack	: fetch stack address
	 *  $stackN	: fetch Nth of stack (N:0-)
<<<<<<< HEAD
=======
	 *  $comm       : fetch current task comm
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 *  @ADDR	: fetch memory at ADDR (ADDR should be in kernel)
	 *  @SYM[+|-offs] : fetch memory at SYM +|- offs (SYM is a data symbol)
	 *  %REG	: fetch register REG
	 * Dereferencing memory fetch:
	 *  +|-offs(ARG) : fetch memory at ARG +|- offs address.
	 * Alias name of args:
	 *  NAME=FETCHARG : set NAME as alias of FETCHARG.
	 * Type of args:
	 *  FETCHARG:TYPE : use TYPE instead of unsigned long.
	 */
<<<<<<< HEAD
	struct trace_probe *tp;
	int i, ret = 0;
	int is_return = 0, is_delete = 0;
	char *symbol = NULL, *event = NULL, *group = NULL;
	char *arg;
	unsigned long offset = 0;
	void *addr = NULL;
	char buf[MAX_EVENT_NAME_LEN];

	/* argc must be >= 1 */
	if (argv[0][0] == 'p')
		is_return = 0;
	else if (argv[0][0] == 'r')
		is_return = 1;
	else if (argv[0][0] == '-')
		is_delete = 1;
	else {
		pr_info("Probe definition must be started with 'p', 'r' or"
			" '-'.\n");
		return -EINVAL;
	}

	if (argv[0][1] == ':') {
		event = &argv[0][2];
		if (strchr(event, '/')) {
			group = event;
			event = strchr(group, '/') + 1;
			event[-1] = '\0';
			if (strlen(group) == 0) {
				pr_info("Group name is not specified\n");
				return -EINVAL;
			}
		}
		if (strlen(event) == 0) {
			pr_info("Event name is not specified\n");
			return -EINVAL;
		}
	}
	if (!group)
		group = KPROBE_EVENT_SYSTEM;

	if (is_delete) {
		if (!event) {
			pr_info("Delete command needs an event name.\n");
			return -EINVAL;
		}
		mutex_lock(&probe_lock);
		tp = find_trace_probe(event, group);
		if (!tp) {
			mutex_unlock(&probe_lock);
			pr_info("Event %s/%s doesn't exist.\n", group, event);
			return -ENOENT;
		}
		/* delete an event */
		ret = unregister_trace_probe(tp);
		if (ret == 0)
			free_trace_probe(tp);
		mutex_unlock(&probe_lock);
		return ret;
	}

	if (argc < 2) {
		pr_info("Probe point is not specified.\n");
		return -EINVAL;
	}
	if (isdigit(argv[1][0])) {
		if (is_return) {
			pr_info("Return probe point must be a symbol.\n");
			return -EINVAL;
		}
		/* an address specified */
		ret = strict_strtoul(&argv[1][0], 0, (unsigned long *)&addr);
		if (ret) {
			pr_info("Failed to parse address.\n");
			return ret;
		}
	} else {
		/* a symbol specified */
		symbol = argv[1];
		/* TODO: support .init module functions */
		ret = split_symbol_offset(symbol, &offset);
		if (ret) {
			pr_info("Failed to parse symbol.\n");
			return ret;
		}
		if (offset && is_return) {
			pr_info("Return probe must be used without offset.\n");
			return -EINVAL;
		}
	}
	argc -= 2; argv += 2;

	/* setup a probe */
=======
	struct trace_kprobe *tk = NULL;
	int i, len, new_argc = 0, ret = 0;
	bool is_return = false;
	char *symbol = NULL, *tmp = NULL;
	const char **new_argv = NULL;
	const char *event = NULL, *group = KPROBE_EVENT_SYSTEM;
	enum probe_print_type ptype;
	int maxactive = 0;
	long offset = 0;
	void *addr = NULL;
	char buf[MAX_EVENT_NAME_LEN];
	char gbuf[MAX_EVENT_NAME_LEN];
	char abuf[MAX_BTF_ARGS_LEN];
	struct traceprobe_parse_context ctx = { .flags = TPARG_FL_KERNEL };

	switch (argv[0][0]) {
	case 'r':
		is_return = true;
		break;
	case 'p':
		break;
	default:
		return -ECANCELED;
	}
	if (argc < 2)
		return -ECANCELED;

	trace_probe_log_init("trace_kprobe", argc, argv);

	event = strchr(&argv[0][1], ':');
	if (event)
		event++;

	if (isdigit(argv[0][1])) {
		if (!is_return) {
			trace_probe_log_err(1, BAD_MAXACT_TYPE);
			goto parse_error;
		}
		if (event)
			len = event - &argv[0][1] - 1;
		else
			len = strlen(&argv[0][1]);
		if (len > MAX_EVENT_NAME_LEN - 1) {
			trace_probe_log_err(1, BAD_MAXACT);
			goto parse_error;
		}
		memcpy(buf, &argv[0][1], len);
		buf[len] = '\0';
		ret = kstrtouint(buf, 0, &maxactive);
		if (ret || !maxactive) {
			trace_probe_log_err(1, BAD_MAXACT);
			goto parse_error;
		}
		/* kretprobes instances are iterated over via a list. The
		 * maximum should stay reasonable.
		 */
		if (maxactive > KRETPROBE_MAXACTIVE_MAX) {
			trace_probe_log_err(1, MAXACT_TOO_BIG);
			goto parse_error;
		}
	}

	/* try to parse an address. if that fails, try to read the
	 * input as a symbol. */
	if (kstrtoul(argv[1], 0, (unsigned long *)&addr)) {
		trace_probe_log_set_index(1);
		/* Check whether uprobe event specified */
		if (strchr(argv[1], '/') && strchr(argv[1], ':')) {
			ret = -ECANCELED;
			goto error;
		}
		/* a symbol specified */
		symbol = kstrdup(argv[1], GFP_KERNEL);
		if (!symbol)
			return -ENOMEM;

		tmp = strchr(symbol, '%');
		if (tmp) {
			if (!strcmp(tmp, "%return")) {
				*tmp = '\0';
				is_return = true;
			} else {
				trace_probe_log_err(tmp - symbol, BAD_ADDR_SUFFIX);
				goto parse_error;
			}
		}

		/* TODO: support .init module functions */
		ret = traceprobe_split_symbol_offset(symbol, &offset);
		if (ret || offset < 0 || offset > UINT_MAX) {
			trace_probe_log_err(0, BAD_PROBE_ADDR);
			goto parse_error;
		}
		if (is_return)
			ctx.flags |= TPARG_FL_RETURN;
		ret = kprobe_on_func_entry(NULL, symbol, offset);
		if (ret == 0 && !is_return)
			ctx.flags |= TPARG_FL_FENTRY;
		/* Defer the ENOENT case until register kprobe */
		if (ret == -EINVAL && is_return) {
			trace_probe_log_err(0, BAD_RETPROBE);
			goto parse_error;
		}
	}

	if (symbol && !strchr(symbol, ':')) {
		unsigned int count;

		count = number_of_same_symbols(symbol);
		if (count > 1) {
			/*
			 * Users should use ADDR to remove the ambiguity of
			 * using KSYM only.
			 */
			trace_probe_log_err(0, NON_UNIQ_SYMBOL);
			ret = -EADDRNOTAVAIL;

			goto error;
		} else if (count == 0) {
			/*
			 * We can return ENOENT earlier than when register the
			 * kprobe.
			 */
			trace_probe_log_err(0, BAD_PROBE_ADDR);
			ret = -ENOENT;

			goto error;
		}
	}

	trace_probe_log_set_index(0);
	if (event) {
		ret = traceprobe_parse_event_name(&event, &group, gbuf,
						  event - argv[0]);
		if (ret)
			goto parse_error;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!event) {
		/* Make a new event name */
		if (symbol)
			snprintf(buf, MAX_EVENT_NAME_LEN, "%c_%s_%ld",
				 is_return ? 'r' : 'p', symbol, offset);
		else
			snprintf(buf, MAX_EVENT_NAME_LEN, "%c_0x%p",
				 is_return ? 'r' : 'p', addr);
<<<<<<< HEAD
		event = buf;
	}
	tp = alloc_trace_probe(group, event, addr, symbol, offset, argc,
			       is_return);
	if (IS_ERR(tp)) {
		pr_info("Failed to allocate trace_probe.(%d)\n",
			(int)PTR_ERR(tp));
		return PTR_ERR(tp);
	}

	/* parse arguments */
	ret = 0;
	for (i = 0; i < argc && i < MAX_TRACE_ARGS; i++) {
		/* Increment count for freeing args in error case */
		tp->nr_args++;

		/* Parse argument name */
		arg = strchr(argv[i], '=');
		if (arg) {
			*arg++ = '\0';
			tp->args[i].name = kstrdup(argv[i], GFP_KERNEL);
		} else {
			arg = argv[i];
			/* If argument name is omitted, set "argN" */
			snprintf(buf, MAX_EVENT_NAME_LEN, "arg%d", i + 1);
			tp->args[i].name = kstrdup(buf, GFP_KERNEL);
		}

		if (!tp->args[i].name) {
			pr_info("Failed to allocate argument[%d] name.\n", i);
			ret = -ENOMEM;
			goto error;
		}

		if (!is_good_name(tp->args[i].name)) {
			pr_info("Invalid argument[%d] name: %s\n",
				i, tp->args[i].name);
			ret = -EINVAL;
			goto error;
		}

		if (conflict_field_name(tp->args[i].name, tp->args, i)) {
			pr_info("Argument[%d] name '%s' conflicts with "
				"another field.\n", i, argv[i]);
			ret = -EINVAL;
			goto error;
		}

		/* Parse fetch argument */
		ret = parse_probe_arg(arg, tp, &tp->args[i], is_return);
		if (ret) {
			pr_info("Parse error at argument[%d]. (%d)\n", i, ret);
			goto error;
		}
	}

	ret = register_trace_probe(tp);
	if (ret)
		goto error;
	return 0;

error:
	free_trace_probe(tp);
	return ret;
}

static int release_all_trace_probes(void)
{
	struct trace_probe *tp;
	int ret = 0;

	mutex_lock(&probe_lock);
	/* Ensure no probe is in use. */
	list_for_each_entry(tp, &probe_list, list)
		if (trace_probe_is_enabled(tp)) {
			ret = -EBUSY;
			goto end;
		}
	/* TODO: Use batch unregistration */
	while (!list_empty(&probe_list)) {
		tp = list_entry(probe_list.next, struct trace_probe, list);
		unregister_trace_probe(tp);
		free_trace_probe(tp);
	}

end:
	mutex_unlock(&probe_lock);

	return ret;
}

/* Probes listing interfaces */
static void *probes_seq_start(struct seq_file *m, loff_t *pos)
{
	mutex_lock(&probe_lock);
	return seq_list_start(&probe_list, *pos);
}

static void *probes_seq_next(struct seq_file *m, void *v, loff_t *pos)
{
	return seq_list_next(v, &probe_list, pos);
}

static void probes_seq_stop(struct seq_file *m, void *v)
{
	mutex_unlock(&probe_lock);
=======
		sanitize_event_name(buf);
		event = buf;
	}

	argc -= 2; argv += 2;
	ctx.funcname = symbol;
	new_argv = traceprobe_expand_meta_args(argc, argv, &new_argc,
					       abuf, MAX_BTF_ARGS_LEN, &ctx);
	if (IS_ERR(new_argv)) {
		ret = PTR_ERR(new_argv);
		new_argv = NULL;
		goto out;
	}
	if (new_argv) {
		argc = new_argc;
		argv = new_argv;
	}

	/* setup a probe */
	tk = alloc_trace_kprobe(group, event, addr, symbol, offset, maxactive,
				argc, is_return);
	if (IS_ERR(tk)) {
		ret = PTR_ERR(tk);
		/* This must return -ENOMEM, else there is a bug */
		WARN_ON_ONCE(ret != -ENOMEM);
		goto out;	/* We know tk is not allocated */
	}

	/* parse arguments */
	for (i = 0; i < argc && i < MAX_TRACE_ARGS; i++) {
		trace_probe_log_set_index(i + 2);
		ctx.offset = 0;
		ret = traceprobe_parse_probe_arg(&tk->tp, i, argv[i], &ctx);
		if (ret)
			goto error;	/* This can be -ENOMEM */
	}
	/* entry handler for kretprobe */
	if (is_return && tk->tp.entry_arg) {
		tk->rp.entry_handler = trace_kprobe_entry_handler;
		tk->rp.data_size = traceprobe_get_entry_data_size(&tk->tp);
	}

	ptype = is_return ? PROBE_PRINT_RETURN : PROBE_PRINT_NORMAL;
	ret = traceprobe_set_print_fmt(&tk->tp, ptype);
	if (ret < 0)
		goto error;

	ret = register_trace_kprobe(tk);
	if (ret) {
		trace_probe_log_set_index(1);
		if (ret == -EILSEQ)
			trace_probe_log_err(0, BAD_INSN_BNDRY);
		else if (ret == -ENOENT)
			trace_probe_log_err(0, BAD_PROBE_ADDR);
		else if (ret != -ENOMEM && ret != -EEXIST)
			trace_probe_log_err(0, FAIL_REG_PROBE);
		goto error;
	}

out:
	traceprobe_finish_parse(&ctx);
	trace_probe_log_clear();
	kfree(new_argv);
	kfree(symbol);
	return ret;

parse_error:
	ret = -EINVAL;
error:
	free_trace_kprobe(tk);
	goto out;
}

static int trace_kprobe_create(const char *raw_command)
{
	return trace_probe_create(raw_command, __trace_kprobe_create);
}

static int create_or_delete_trace_kprobe(const char *raw_command)
{
	int ret;

	if (raw_command[0] == '-')
		return dyn_event_release(raw_command, &trace_kprobe_ops);

	ret = trace_kprobe_create(raw_command);
	return ret == -ECANCELED ? -EINVAL : ret;
}

static int trace_kprobe_run_command(struct dynevent_cmd *cmd)
{
	return create_or_delete_trace_kprobe(cmd->seq.buffer);
}

/**
 * kprobe_event_cmd_init - Initialize a kprobe event command object
 * @cmd: A pointer to the dynevent_cmd struct representing the new event
 * @buf: A pointer to the buffer used to build the command
 * @maxlen: The length of the buffer passed in @buf
 *
 * Initialize a synthetic event command object.  Use this before
 * calling any of the other kprobe_event functions.
 */
void kprobe_event_cmd_init(struct dynevent_cmd *cmd, char *buf, int maxlen)
{
	dynevent_cmd_init(cmd, buf, maxlen, DYNEVENT_TYPE_KPROBE,
			  trace_kprobe_run_command);
}
EXPORT_SYMBOL_GPL(kprobe_event_cmd_init);

/**
 * __kprobe_event_gen_cmd_start - Generate a kprobe event command from arg list
 * @cmd: A pointer to the dynevent_cmd struct representing the new event
 * @kretprobe: Is this a return probe?
 * @name: The name of the kprobe event
 * @loc: The location of the kprobe event
 * @...: Variable number of arg (pairs), one pair for each field
 *
 * NOTE: Users normally won't want to call this function directly, but
 * rather use the kprobe_event_gen_cmd_start() wrapper, which automatically
 * adds a NULL to the end of the arg list.  If this function is used
 * directly, make sure the last arg in the variable arg list is NULL.
 *
 * Generate a kprobe event command to be executed by
 * kprobe_event_gen_cmd_end().  This function can be used to generate the
 * complete command or only the first part of it; in the latter case,
 * kprobe_event_add_fields() can be used to add more fields following this.
 *
 * Unlikely the synth_event_gen_cmd_start(), @loc must be specified. This
 * returns -EINVAL if @loc == NULL.
 *
 * Return: 0 if successful, error otherwise.
 */
int __kprobe_event_gen_cmd_start(struct dynevent_cmd *cmd, bool kretprobe,
				 const char *name, const char *loc, ...)
{
	char buf[MAX_EVENT_NAME_LEN];
	struct dynevent_arg arg;
	va_list args;
	int ret;

	if (cmd->type != DYNEVENT_TYPE_KPROBE)
		return -EINVAL;

	if (!loc)
		return -EINVAL;

	if (kretprobe)
		snprintf(buf, MAX_EVENT_NAME_LEN, "r:kprobes/%s", name);
	else
		snprintf(buf, MAX_EVENT_NAME_LEN, "p:kprobes/%s", name);

	ret = dynevent_str_add(cmd, buf);
	if (ret)
		return ret;

	dynevent_arg_init(&arg, 0);
	arg.str = loc;
	ret = dynevent_arg_add(cmd, &arg, NULL);
	if (ret)
		return ret;

	va_start(args, loc);
	for (;;) {
		const char *field;

		field = va_arg(args, const char *);
		if (!field)
			break;

		if (++cmd->n_fields > MAX_TRACE_ARGS) {
			ret = -EINVAL;
			break;
		}

		arg.str = field;
		ret = dynevent_arg_add(cmd, &arg, NULL);
		if (ret)
			break;
	}
	va_end(args);

	return ret;
}
EXPORT_SYMBOL_GPL(__kprobe_event_gen_cmd_start);

/**
 * __kprobe_event_add_fields - Add probe fields to a kprobe command from arg list
 * @cmd: A pointer to the dynevent_cmd struct representing the new event
 * @...: Variable number of arg (pairs), one pair for each field
 *
 * NOTE: Users normally won't want to call this function directly, but
 * rather use the kprobe_event_add_fields() wrapper, which
 * automatically adds a NULL to the end of the arg list.  If this
 * function is used directly, make sure the last arg in the variable
 * arg list is NULL.
 *
 * Add probe fields to an existing kprobe command using a variable
 * list of args.  Fields are added in the same order they're listed.
 *
 * Return: 0 if successful, error otherwise.
 */
int __kprobe_event_add_fields(struct dynevent_cmd *cmd, ...)
{
	struct dynevent_arg arg;
	va_list args;
	int ret = 0;

	if (cmd->type != DYNEVENT_TYPE_KPROBE)
		return -EINVAL;

	dynevent_arg_init(&arg, 0);

	va_start(args, cmd);
	for (;;) {
		const char *field;

		field = va_arg(args, const char *);
		if (!field)
			break;

		if (++cmd->n_fields > MAX_TRACE_ARGS) {
			ret = -EINVAL;
			break;
		}

		arg.str = field;
		ret = dynevent_arg_add(cmd, &arg, NULL);
		if (ret)
			break;
	}
	va_end(args);

	return ret;
}
EXPORT_SYMBOL_GPL(__kprobe_event_add_fields);

/**
 * kprobe_event_delete - Delete a kprobe event
 * @name: The name of the kprobe event to delete
 *
 * Delete a kprobe event with the give @name from kernel code rather
 * than directly from the command line.
 *
 * Return: 0 if successful, error otherwise.
 */
int kprobe_event_delete(const char *name)
{
	char buf[MAX_EVENT_NAME_LEN];

	snprintf(buf, MAX_EVENT_NAME_LEN, "-:%s", name);

	return create_or_delete_trace_kprobe(buf);
}
EXPORT_SYMBOL_GPL(kprobe_event_delete);

static int trace_kprobe_release(struct dyn_event *ev)
{
	struct trace_kprobe *tk = to_trace_kprobe(ev);
	int ret = unregister_trace_kprobe(tk);

	if (!ret)
		free_trace_kprobe(tk);
	return ret;
}

static int trace_kprobe_show(struct seq_file *m, struct dyn_event *ev)
{
	struct trace_kprobe *tk = to_trace_kprobe(ev);
	int i;

	seq_putc(m, trace_kprobe_is_return(tk) ? 'r' : 'p');
	if (trace_kprobe_is_return(tk) && tk->rp.maxactive)
		seq_printf(m, "%d", tk->rp.maxactive);
	seq_printf(m, ":%s/%s", trace_probe_group_name(&tk->tp),
				trace_probe_name(&tk->tp));

	if (!tk->symbol)
		seq_printf(m, " 0x%p", tk->rp.kp.addr);
	else if (tk->rp.kp.offset)
		seq_printf(m, " %s+%u", trace_kprobe_symbol(tk),
			   tk->rp.kp.offset);
	else
		seq_printf(m, " %s", trace_kprobe_symbol(tk));

	for (i = 0; i < tk->tp.nr_args; i++)
		seq_printf(m, " %s=%s", tk->tp.args[i].name, tk->tp.args[i].comm);
	seq_putc(m, '\n');

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int probes_seq_show(struct seq_file *m, void *v)
{
<<<<<<< HEAD
	struct trace_probe *tp = v;
	int i;

	seq_printf(m, "%c", trace_probe_is_return(tp) ? 'r' : 'p');
	seq_printf(m, ":%s/%s", tp->call.class->system, tp->call.name);

	if (!tp->symbol)
		seq_printf(m, " 0x%p", tp->rp.kp.addr);
	else if (tp->rp.kp.offset)
		seq_printf(m, " %s+%u", trace_probe_symbol(tp),
			   tp->rp.kp.offset);
	else
		seq_printf(m, " %s", trace_probe_symbol(tp));

	for (i = 0; i < tp->nr_args; i++)
		seq_printf(m, " %s=%s", tp->args[i].name, tp->args[i].comm);
	seq_printf(m, "\n");

	return 0;
}

static const struct seq_operations probes_seq_op = {
	.start  = probes_seq_start,
	.next   = probes_seq_next,
	.stop   = probes_seq_stop,
=======
	struct dyn_event *ev = v;

	if (!is_trace_kprobe(ev))
		return 0;

	return trace_kprobe_show(m, ev);
}

static const struct seq_operations probes_seq_op = {
	.start  = dyn_event_seq_start,
	.next   = dyn_event_seq_next,
	.stop   = dyn_event_seq_stop,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.show   = probes_seq_show
};

static int probes_open(struct inode *inode, struct file *file)
{
	int ret;

<<<<<<< HEAD
	if ((file->f_mode & FMODE_WRITE) && (file->f_flags & O_TRUNC)) {
		ret = release_all_trace_probes();
=======
	ret = security_locked_down(LOCKDOWN_TRACEFS);
	if (ret)
		return ret;

	if ((file->f_mode & FMODE_WRITE) && (file->f_flags & O_TRUNC)) {
		ret = dyn_events_release_all(&trace_kprobe_ops);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret < 0)
			return ret;
	}

	return seq_open(file, &probes_seq_op);
}

<<<<<<< HEAD
static int command_trace_probe(const char *buf)
{
	char **argv;
	int argc = 0, ret = 0;

	argv = argv_split(GFP_KERNEL, buf, &argc);
	if (!argv)
		return -ENOMEM;

	if (argc)
		ret = create_trace_probe(argc, argv);

	argv_free(argv);
	return ret;
}

#define WRITE_BUFSIZE 4096

static ssize_t probes_write(struct file *file, const char __user *buffer,
			    size_t count, loff_t *ppos)
{
	char *kbuf, *tmp;
	int ret;
	size_t done;
	size_t size;

	kbuf = kmalloc(WRITE_BUFSIZE, GFP_KERNEL);
	if (!kbuf)
		return -ENOMEM;

	ret = done = 0;
	while (done < count) {
		size = count - done;
		if (size >= WRITE_BUFSIZE)
			size = WRITE_BUFSIZE - 1;
		if (copy_from_user(kbuf, buffer + done, size)) {
			ret = -EFAULT;
			goto out;
		}
		kbuf[size] = '\0';
		tmp = strchr(kbuf, '\n');
		if (tmp) {
			*tmp = '\0';
			size = tmp - kbuf + 1;
		} else if (done + size < count) {
			pr_warning("Line length is too long: "
				   "Should be less than %d.", WRITE_BUFSIZE);
			ret = -EINVAL;
			goto out;
		}
		done += size;
		/* Remove comments */
		tmp = strchr(kbuf, '#');
		if (tmp)
			*tmp = '\0';

		ret = command_trace_probe(kbuf);
		if (ret)
			goto out;
	}
	ret = done;
out:
	kfree(kbuf);
	return ret;
=======
static ssize_t probes_write(struct file *file, const char __user *buffer,
			    size_t count, loff_t *ppos)
{
	return trace_parse_run_command(file, buffer, count, ppos,
				       create_or_delete_trace_kprobe);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct file_operations kprobe_events_ops = {
	.owner          = THIS_MODULE,
	.open           = probes_open,
	.read           = seq_read,
	.llseek         = seq_lseek,
	.release        = seq_release,
	.write		= probes_write,
};

<<<<<<< HEAD
/* Probes profiling interfaces */
static int probes_profile_seq_show(struct seq_file *m, void *v)
{
	struct trace_probe *tp = v;

	seq_printf(m, "  %-44s %15lu %15lu\n", tp->call.name, tp->nhit,
		   tp->rp.kp.nmissed);
=======
static unsigned long trace_kprobe_missed(struct trace_kprobe *tk)
{
	return trace_kprobe_is_return(tk) ?
		tk->rp.kp.nmissed + tk->rp.nmissed : tk->rp.kp.nmissed;
}

/* Probes profiling interfaces */
static int probes_profile_seq_show(struct seq_file *m, void *v)
{
	struct dyn_event *ev = v;
	struct trace_kprobe *tk;
	unsigned long nmissed;

	if (!is_trace_kprobe(ev))
		return 0;

	tk = to_trace_kprobe(ev);
	nmissed = trace_kprobe_missed(tk);
	seq_printf(m, "  %-44s %15lu %15lu\n",
		   trace_probe_name(&tk->tp),
		   trace_kprobe_nhit(tk),
		   nmissed);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static const struct seq_operations profile_seq_op = {
<<<<<<< HEAD
	.start  = probes_seq_start,
	.next   = probes_seq_next,
	.stop   = probes_seq_stop,
=======
	.start  = dyn_event_seq_start,
	.next   = dyn_event_seq_next,
	.stop   = dyn_event_seq_stop,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.show   = probes_profile_seq_show
};

static int profile_open(struct inode *inode, struct file *file)
{
<<<<<<< HEAD
=======
	int ret;

	ret = security_locked_down(LOCKDOWN_TRACEFS);
	if (ret)
		return ret;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return seq_open(file, &profile_seq_op);
}

static const struct file_operations kprobe_profile_ops = {
	.owner          = THIS_MODULE,
	.open           = profile_open,
	.read           = seq_read,
	.llseek         = seq_lseek,
	.release        = seq_release,
};

<<<<<<< HEAD
/* Sum up total data length for dynamic arraies (strings) */
static __kprobes int __get_data_size(struct trace_probe *tp,
				     struct pt_regs *regs)
{
	int i, ret = 0;
	u32 len;

	for (i = 0; i < tp->nr_args; i++)
		if (unlikely(tp->args[i].fetch_size.fn)) {
			call_fetch(&tp->args[i].fetch_size, regs, &len);
			ret += len;
		}

	return ret;
}

/* Store the value of each argument */
static __kprobes void store_trace_args(int ent_size, struct trace_probe *tp,
				       struct pt_regs *regs,
				       u8 *data, int maxlen)
{
	int i;
	u32 end = tp->size;
	u32 *dl;	/* Data (relative) location */

	for (i = 0; i < tp->nr_args; i++) {
		if (unlikely(tp->args[i].fetch_size.fn)) {
			/*
			 * First, we set the relative location and
			 * maximum data length to *dl
			 */
			dl = (u32 *)(data + tp->args[i].offset);
			*dl = make_data_rloc(maxlen, end - tp->args[i].offset);
			/* Then try to fetch string or dynamic array data */
			call_fetch(&tp->args[i].fetch, regs, dl);
			/* Reduce maximum length */
			end += get_rloc_len(*dl);
			maxlen -= get_rloc_len(*dl);
			/* Trick here, convert data_rloc to data_loc */
			*dl = convert_rloc_to_loc(*dl,
				 ent_size + tp->args[i].offset);
		} else
			/* Just fetching data normally */
			call_fetch(&tp->args[i].fetch, regs,
				   data + tp->args[i].offset);
	}
}

/* Kprobe handler */
static __kprobes void kprobe_trace_func(struct kprobe *kp, struct pt_regs *regs)
{
	struct trace_probe *tp = container_of(kp, struct trace_probe, rp.kp);
	struct kprobe_trace_entry_head *entry;
	struct ring_buffer_event *event;
	struct ring_buffer *buffer;
	int size, dsize, pc;
	unsigned long irq_flags;
	struct ftrace_event_call *call = &tp->call;

	tp->nhit++;

	local_save_flags(irq_flags);
	pc = preempt_count();

	dsize = __get_data_size(tp, regs);
	size = sizeof(*entry) + tp->size + dsize;

	event = trace_current_buffer_lock_reserve(&buffer, call->event.type,
						  size, irq_flags, pc);
	if (!event)
		return;

	entry = ring_buffer_event_data(event);
	entry->ip = (unsigned long)kp->addr;
	store_trace_args(sizeof(*entry), tp, regs, (u8 *)&entry[1], dsize);

	if (!filter_current_check_discard(buffer, call, entry, event))
		trace_nowake_buffer_unlock_commit_regs(buffer, event,
						       irq_flags, pc, regs);
}

/* Kretprobe handler */
static __kprobes void kretprobe_trace_func(struct kretprobe_instance *ri,
					  struct pt_regs *regs)
{
	struct trace_probe *tp = container_of(ri->rp, struct trace_probe, rp);
	struct kretprobe_trace_entry_head *entry;
	struct ring_buffer_event *event;
	struct ring_buffer *buffer;
	int size, pc, dsize;
	unsigned long irq_flags;
	struct ftrace_event_call *call = &tp->call;

	local_save_flags(irq_flags);
	pc = preempt_count();

	dsize = __get_data_size(tp, regs);
	size = sizeof(*entry) + tp->size + dsize;

	event = trace_current_buffer_lock_reserve(&buffer, call->event.type,
						  size, irq_flags, pc);
	if (!event)
		return;

	entry = ring_buffer_event_data(event);
	entry->func = (unsigned long)tp->rp.kp.addr;
	entry->ret_ip = (unsigned long)ri->ret_addr;
	store_trace_args(sizeof(*entry), tp, regs, (u8 *)&entry[1], dsize);

	if (!filter_current_check_discard(buffer, call, entry, event))
		trace_nowake_buffer_unlock_commit_regs(buffer, event,
						       irq_flags, pc, regs);
}

/* Event entry printers */
enum print_line_t
=======
/* Note that we don't verify it, since the code does not come from user space */
static int
process_fetch_insn(struct fetch_insn *code, void *rec, void *edata,
		   void *dest, void *base)
{
	struct pt_regs *regs = rec;
	unsigned long val;
	int ret;

retry:
	/* 1st stage: get value from context */
	switch (code->op) {
	case FETCH_OP_REG:
		val = regs_get_register(regs, code->param);
		break;
	case FETCH_OP_STACK:
		val = regs_get_kernel_stack_nth(regs, code->param);
		break;
	case FETCH_OP_STACKP:
		val = kernel_stack_pointer(regs);
		break;
	case FETCH_OP_RETVAL:
		val = regs_return_value(regs);
		break;
#ifdef CONFIG_HAVE_FUNCTION_ARG_ACCESS_API
	case FETCH_OP_ARG:
		val = regs_get_kernel_argument(regs, code->param);
		break;
	case FETCH_OP_EDATA:
		val = *(unsigned long *)((unsigned long)edata + code->offset);
		break;
#endif
	case FETCH_NOP_SYMBOL:	/* Ignore a place holder */
		code++;
		goto retry;
	default:
		ret = process_common_fetch_insn(code, &val);
		if (ret < 0)
			return ret;
	}
	code++;

	return process_fetch_insn_bottom(code, val, dest, base);
}
NOKPROBE_SYMBOL(process_fetch_insn)

/* Kprobe handler */
static nokprobe_inline void
__kprobe_trace_func(struct trace_kprobe *tk, struct pt_regs *regs,
		    struct trace_event_file *trace_file)
{
	struct kprobe_trace_entry_head *entry;
	struct trace_event_call *call = trace_probe_event_call(&tk->tp);
	struct trace_event_buffer fbuffer;
	int dsize;

	WARN_ON(call != trace_file->event_call);

	if (trace_trigger_soft_disabled(trace_file))
		return;

	dsize = __get_data_size(&tk->tp, regs, NULL);

	entry = trace_event_buffer_reserve(&fbuffer, trace_file,
					   sizeof(*entry) + tk->tp.size + dsize);
	if (!entry)
		return;

	fbuffer.regs = regs;
	entry->ip = (unsigned long)tk->rp.kp.addr;
	store_trace_args(&entry[1], &tk->tp, regs, NULL, sizeof(*entry), dsize);

	trace_event_buffer_commit(&fbuffer);
}

static void
kprobe_trace_func(struct trace_kprobe *tk, struct pt_regs *regs)
{
	struct event_file_link *link;

	trace_probe_for_each_link_rcu(link, &tk->tp)
		__kprobe_trace_func(tk, regs, link->file);
}
NOKPROBE_SYMBOL(kprobe_trace_func);

/* Kretprobe handler */

static int trace_kprobe_entry_handler(struct kretprobe_instance *ri,
				      struct pt_regs *regs)
{
	struct kretprobe *rp = get_kretprobe(ri);
	struct trace_kprobe *tk;

	/*
	 * There is a small chance that get_kretprobe(ri) returns NULL when
	 * the kretprobe is unregister on another CPU between kretprobe's
	 * trampoline_handler and this function.
	 */
	if (unlikely(!rp))
		return -ENOENT;

	tk = container_of(rp, struct trace_kprobe, rp);

	/* store argument values into ri->data as entry data */
	if (tk->tp.entry_arg)
		store_trace_entry_data(ri->data, &tk->tp, regs);

	return 0;
}


static nokprobe_inline void
__kretprobe_trace_func(struct trace_kprobe *tk, struct kretprobe_instance *ri,
		       struct pt_regs *regs,
		       struct trace_event_file *trace_file)
{
	struct kretprobe_trace_entry_head *entry;
	struct trace_event_buffer fbuffer;
	struct trace_event_call *call = trace_probe_event_call(&tk->tp);
	int dsize;

	WARN_ON(call != trace_file->event_call);

	if (trace_trigger_soft_disabled(trace_file))
		return;

	dsize = __get_data_size(&tk->tp, regs, ri->data);

	entry = trace_event_buffer_reserve(&fbuffer, trace_file,
					   sizeof(*entry) + tk->tp.size + dsize);
	if (!entry)
		return;

	fbuffer.regs = regs;
	entry->func = (unsigned long)tk->rp.kp.addr;
	entry->ret_ip = get_kretprobe_retaddr(ri);
	store_trace_args(&entry[1], &tk->tp, regs, ri->data, sizeof(*entry), dsize);

	trace_event_buffer_commit(&fbuffer);
}

static void
kretprobe_trace_func(struct trace_kprobe *tk, struct kretprobe_instance *ri,
		     struct pt_regs *regs)
{
	struct event_file_link *link;

	trace_probe_for_each_link_rcu(link, &tk->tp)
		__kretprobe_trace_func(tk, ri, regs, link->file);
}
NOKPROBE_SYMBOL(kretprobe_trace_func);

/* Event entry printers */
static enum print_line_t
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
print_kprobe_event(struct trace_iterator *iter, int flags,
		   struct trace_event *event)
{
	struct kprobe_trace_entry_head *field;
	struct trace_seq *s = &iter->seq;
	struct trace_probe *tp;
<<<<<<< HEAD
	u8 *data;
	int i;

	field = (struct kprobe_trace_entry_head *)iter->ent;
	tp = container_of(event, struct trace_probe, call.event);

	if (!trace_seq_printf(s, "%s: (", tp->call.name))
		goto partial;

	if (!seq_print_ip_sym(s, field->ip, flags | TRACE_ITER_SYM_OFFSET))
		goto partial;

	if (!trace_seq_puts(s, ")"))
		goto partial;

	data = (u8 *)&field[1];
	for (i = 0; i < tp->nr_args; i++)
		if (!tp->args[i].type->print(s, tp->args[i].name,
					     data + tp->args[i].offset, field))
			goto partial;

	if (!trace_seq_puts(s, "\n"))
		goto partial;

	return TRACE_TYPE_HANDLED;
partial:
	return TRACE_TYPE_PARTIAL_LINE;
}

enum print_line_t
=======

	field = (struct kprobe_trace_entry_head *)iter->ent;
	tp = trace_probe_primary_from_call(
		container_of(event, struct trace_event_call, event));
	if (WARN_ON_ONCE(!tp))
		goto out;

	trace_seq_printf(s, "%s: (", trace_probe_name(tp));

	if (!seq_print_ip_sym(s, field->ip, flags | TRACE_ITER_SYM_OFFSET))
		goto out;

	trace_seq_putc(s, ')');

	if (trace_probe_print_args(s, tp->args, tp->nr_args,
			     (u8 *)&field[1], field) < 0)
		goto out;

	trace_seq_putc(s, '\n');
 out:
	return trace_handle_return(s);
}

static enum print_line_t
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
print_kretprobe_event(struct trace_iterator *iter, int flags,
		      struct trace_event *event)
{
	struct kretprobe_trace_entry_head *field;
	struct trace_seq *s = &iter->seq;
	struct trace_probe *tp;
<<<<<<< HEAD
	u8 *data;
	int i;

	field = (struct kretprobe_trace_entry_head *)iter->ent;
	tp = container_of(event, struct trace_probe, call.event);

	if (!trace_seq_printf(s, "%s: (", tp->call.name))
		goto partial;

	if (!seq_print_ip_sym(s, field->ret_ip, flags | TRACE_ITER_SYM_OFFSET))
		goto partial;

	if (!trace_seq_puts(s, " <- "))
		goto partial;

	if (!seq_print_ip_sym(s, field->func, flags & ~TRACE_ITER_SYM_OFFSET))
		goto partial;

	if (!trace_seq_puts(s, ")"))
		goto partial;

	data = (u8 *)&field[1];
	for (i = 0; i < tp->nr_args; i++)
		if (!tp->args[i].type->print(s, tp->args[i].name,
					     data + tp->args[i].offset, field))
			goto partial;

	if (!trace_seq_puts(s, "\n"))
		goto partial;

	return TRACE_TYPE_HANDLED;
partial:
	return TRACE_TYPE_PARTIAL_LINE;
}

#undef DEFINE_FIELD
#define DEFINE_FIELD(type, item, name, is_signed)			\
	do {								\
		ret = trace_define_field(event_call, #type, name,	\
					 offsetof(typeof(field), item),	\
					 sizeof(field.item), is_signed, \
					 FILTER_OTHER);			\
		if (ret)						\
			return ret;					\
	} while (0)

static int kprobe_event_define_fields(struct ftrace_event_call *event_call)
{
	int ret, i;
	struct kprobe_trace_entry_head field;
	struct trace_probe *tp = (struct trace_probe *)event_call->data;

	DEFINE_FIELD(unsigned long, ip, FIELD_STRING_IP, 0);
	/* Set argument names as fields */
	for (i = 0; i < tp->nr_args; i++) {
		ret = trace_define_field(event_call, tp->args[i].type->fmttype,
					 tp->args[i].name,
					 sizeof(field) + tp->args[i].offset,
					 tp->args[i].type->size,
					 tp->args[i].type->is_signed,
					 FILTER_OTHER);
		if (ret)
			return ret;
	}
	return 0;
}

static int kretprobe_event_define_fields(struct ftrace_event_call *event_call)
{
	int ret, i;
	struct kretprobe_trace_entry_head field;
	struct trace_probe *tp = (struct trace_probe *)event_call->data;

	DEFINE_FIELD(unsigned long, func, FIELD_STRING_FUNC, 0);
	DEFINE_FIELD(unsigned long, ret_ip, FIELD_STRING_RETIP, 0);
	/* Set argument names as fields */
	for (i = 0; i < tp->nr_args; i++) {
		ret = trace_define_field(event_call, tp->args[i].type->fmttype,
					 tp->args[i].name,
					 sizeof(field) + tp->args[i].offset,
					 tp->args[i].type->size,
					 tp->args[i].type->is_signed,
					 FILTER_OTHER);
		if (ret)
			return ret;
	}
	return 0;
}

static int __set_print_fmt(struct trace_probe *tp, char *buf, int len)
{
	int i;
	int pos = 0;

	const char *fmt, *arg;

	if (!trace_probe_is_return(tp)) {
		fmt = "(%lx)";
		arg = "REC->" FIELD_STRING_IP;
	} else {
		fmt = "(%lx <- %lx)";
		arg = "REC->" FIELD_STRING_FUNC ", REC->" FIELD_STRING_RETIP;
	}

	/* When len=0, we just calculate the needed length */
#define LEN_OR_ZERO (len ? len - pos : 0)

	pos += snprintf(buf + pos, LEN_OR_ZERO, "\"%s", fmt);

	for (i = 0; i < tp->nr_args; i++) {
		pos += snprintf(buf + pos, LEN_OR_ZERO, " %s=%s",
				tp->args[i].name, tp->args[i].type->fmt);
	}

	pos += snprintf(buf + pos, LEN_OR_ZERO, "\", %s", arg);

	for (i = 0; i < tp->nr_args; i++) {
		if (strcmp(tp->args[i].type->name, "string") == 0)
			pos += snprintf(buf + pos, LEN_OR_ZERO,
					", __get_str(%s)",
					tp->args[i].name);
		else
			pos += snprintf(buf + pos, LEN_OR_ZERO, ", REC->%s",
					tp->args[i].name);
	}

#undef LEN_OR_ZERO

	/* return the length of print_fmt */
	return pos;
}

static int set_print_fmt(struct trace_probe *tp)
{
	int len;
	char *print_fmt;

	/* First: called with 0 length to calculate the needed length */
	len = __set_print_fmt(tp, NULL, 0);
	print_fmt = kmalloc(len + 1, GFP_KERNEL);
	if (!print_fmt)
		return -ENOMEM;

	/* Second: actually write the @print_fmt */
	__set_print_fmt(tp, print_fmt, len + 1);
	tp->call.print_fmt = print_fmt;

	return 0;
=======

	field = (struct kretprobe_trace_entry_head *)iter->ent;
	tp = trace_probe_primary_from_call(
		container_of(event, struct trace_event_call, event));
	if (WARN_ON_ONCE(!tp))
		goto out;

	trace_seq_printf(s, "%s: (", trace_probe_name(tp));

	if (!seq_print_ip_sym(s, field->ret_ip, flags | TRACE_ITER_SYM_OFFSET))
		goto out;

	trace_seq_puts(s, " <- ");

	if (!seq_print_ip_sym(s, field->func, flags & ~TRACE_ITER_SYM_OFFSET))
		goto out;

	trace_seq_putc(s, ')');

	if (trace_probe_print_args(s, tp->args, tp->nr_args,
			     (u8 *)&field[1], field) < 0)
		goto out;

	trace_seq_putc(s, '\n');

 out:
	return trace_handle_return(s);
}


static int kprobe_event_define_fields(struct trace_event_call *event_call)
{
	int ret;
	struct kprobe_trace_entry_head field;
	struct trace_probe *tp;

	tp = trace_probe_primary_from_call(event_call);
	if (WARN_ON_ONCE(!tp))
		return -ENOENT;

	DEFINE_FIELD(unsigned long, ip, FIELD_STRING_IP, 0);

	return traceprobe_define_arg_fields(event_call, sizeof(field), tp);
}

static int kretprobe_event_define_fields(struct trace_event_call *event_call)
{
	int ret;
	struct kretprobe_trace_entry_head field;
	struct trace_probe *tp;

	tp = trace_probe_primary_from_call(event_call);
	if (WARN_ON_ONCE(!tp))
		return -ENOENT;

	DEFINE_FIELD(unsigned long, func, FIELD_STRING_FUNC, 0);
	DEFINE_FIELD(unsigned long, ret_ip, FIELD_STRING_RETIP, 0);

	return traceprobe_define_arg_fields(event_call, sizeof(field), tp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifdef CONFIG_PERF_EVENTS

/* Kprobe profile handler */
<<<<<<< HEAD
static __kprobes void kprobe_perf_func(struct kprobe *kp,
					 struct pt_regs *regs)
{
	struct trace_probe *tp = container_of(kp, struct trace_probe, rp.kp);
	struct ftrace_event_call *call = &tp->call;
=======
static int
kprobe_perf_func(struct trace_kprobe *tk, struct pt_regs *regs)
{
	struct trace_event_call *call = trace_probe_event_call(&tk->tp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct kprobe_trace_entry_head *entry;
	struct hlist_head *head;
	int size, __size, dsize;
	int rctx;

<<<<<<< HEAD
	dsize = __get_data_size(tp, regs);
	__size = sizeof(*entry) + tp->size + dsize;
	size = ALIGN(__size + sizeof(u32), sizeof(u64));
	size -= sizeof(u32);
	if (WARN_ONCE(size > PERF_MAX_TRACE_SIZE,
		     "profile buffer not large enough"))
		return;

	entry = perf_trace_buf_prepare(size, call->event.type, regs, &rctx);
	if (!entry)
		return;

	entry->ip = (unsigned long)kp->addr;
	memset(&entry[1], 0, dsize);
	store_trace_args(sizeof(*entry), tp, regs, (u8 *)&entry[1], dsize);

	head = this_cpu_ptr(call->perf_events);
	perf_trace_buf_submit(entry, size, rctx, entry->ip, 1, regs, head);
}

/* Kretprobe profile handler */
static __kprobes void kretprobe_perf_func(struct kretprobe_instance *ri,
					    struct pt_regs *regs)
{
	struct trace_probe *tp = container_of(ri->rp, struct trace_probe, rp);
	struct ftrace_event_call *call = &tp->call;
=======
	if (bpf_prog_array_valid(call)) {
		unsigned long orig_ip = instruction_pointer(regs);
		int ret;

		ret = trace_call_bpf(call, regs);

		/*
		 * We need to check and see if we modified the pc of the
		 * pt_regs, and if so return 1 so that we don't do the
		 * single stepping.
		 */
		if (orig_ip != instruction_pointer(regs))
			return 1;
		if (!ret)
			return 0;
	}

	head = this_cpu_ptr(call->perf_events);
	if (hlist_empty(head))
		return 0;

	dsize = __get_data_size(&tk->tp, regs, NULL);
	__size = sizeof(*entry) + tk->tp.size + dsize;
	size = ALIGN(__size + sizeof(u32), sizeof(u64));
	size -= sizeof(u32);

	entry = perf_trace_buf_alloc(size, NULL, &rctx);
	if (!entry)
		return 0;

	entry->ip = (unsigned long)tk->rp.kp.addr;
	memset(&entry[1], 0, dsize);
	store_trace_args(&entry[1], &tk->tp, regs, NULL, sizeof(*entry), dsize);
	perf_trace_buf_submit(entry, size, rctx, call->event.type, 1, regs,
			      head, NULL);
	return 0;
}
NOKPROBE_SYMBOL(kprobe_perf_func);

/* Kretprobe profile handler */
static void
kretprobe_perf_func(struct trace_kprobe *tk, struct kretprobe_instance *ri,
		    struct pt_regs *regs)
{
	struct trace_event_call *call = trace_probe_event_call(&tk->tp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct kretprobe_trace_entry_head *entry;
	struct hlist_head *head;
	int size, __size, dsize;
	int rctx;

<<<<<<< HEAD
	dsize = __get_data_size(tp, regs);
	__size = sizeof(*entry) + tp->size + dsize;
	size = ALIGN(__size + sizeof(u32), sizeof(u64));
	size -= sizeof(u32);
	if (WARN_ONCE(size > PERF_MAX_TRACE_SIZE,
		     "profile buffer not large enough"))
		return;

	entry = perf_trace_buf_prepare(size, call->event.type, regs, &rctx);
	if (!entry)
		return;

	entry->func = (unsigned long)tp->rp.kp.addr;
	entry->ret_ip = (unsigned long)ri->ret_addr;
	store_trace_args(sizeof(*entry), tp, regs, (u8 *)&entry[1], dsize);

	head = this_cpu_ptr(call->perf_events);
	perf_trace_buf_submit(entry, size, rctx, entry->ret_ip, 1, regs, head);
}
#endif	/* CONFIG_PERF_EVENTS */

static __kprobes
int kprobe_register(struct ftrace_event_call *event,
		    enum trace_reg type, void *data)
{
	struct trace_probe *tp = (struct trace_probe *)event->data;

	switch (type) {
	case TRACE_REG_REGISTER:
		return enable_trace_probe(tp, TP_FLAG_TRACE);
	case TRACE_REG_UNREGISTER:
		disable_trace_probe(tp, TP_FLAG_TRACE);
		return 0;

#ifdef CONFIG_PERF_EVENTS
	case TRACE_REG_PERF_REGISTER:
		return enable_trace_probe(tp, TP_FLAG_PROFILE);
	case TRACE_REG_PERF_UNREGISTER:
		disable_trace_probe(tp, TP_FLAG_PROFILE);
		return 0;
=======
	if (bpf_prog_array_valid(call) && !trace_call_bpf(call, regs))
		return;

	head = this_cpu_ptr(call->perf_events);
	if (hlist_empty(head))
		return;

	dsize = __get_data_size(&tk->tp, regs, ri->data);
	__size = sizeof(*entry) + tk->tp.size + dsize;
	size = ALIGN(__size + sizeof(u32), sizeof(u64));
	size -= sizeof(u32);

	entry = perf_trace_buf_alloc(size, NULL, &rctx);
	if (!entry)
		return;

	entry->func = (unsigned long)tk->rp.kp.addr;
	entry->ret_ip = get_kretprobe_retaddr(ri);
	store_trace_args(&entry[1], &tk->tp, regs, ri->data, sizeof(*entry), dsize);
	perf_trace_buf_submit(entry, size, rctx, call->event.type, 1, regs,
			      head, NULL);
}
NOKPROBE_SYMBOL(kretprobe_perf_func);

int bpf_get_kprobe_info(const struct perf_event *event, u32 *fd_type,
			const char **symbol, u64 *probe_offset,
			u64 *probe_addr, unsigned long *missed,
			bool perf_type_tracepoint)
{
	const char *pevent = trace_event_name(event->tp_event);
	const char *group = event->tp_event->class->system;
	struct trace_kprobe *tk;

	if (perf_type_tracepoint)
		tk = find_trace_kprobe(pevent, group);
	else
		tk = trace_kprobe_primary_from_call(event->tp_event);
	if (!tk)
		return -EINVAL;

	*fd_type = trace_kprobe_is_return(tk) ? BPF_FD_TYPE_KRETPROBE
					      : BPF_FD_TYPE_KPROBE;
	*probe_offset = tk->rp.kp.offset;
	*probe_addr = kallsyms_show_value(current_cred()) ?
		      (unsigned long)tk->rp.kp.addr : 0;
	*symbol = tk->symbol;
	if (missed)
		*missed = trace_kprobe_missed(tk);
	return 0;
}
#endif	/* CONFIG_PERF_EVENTS */

/*
 * called by perf_trace_init() or __ftrace_set_clr_event() under event_mutex.
 *
 * kprobe_trace_self_tests_init() does enable_trace_probe/disable_trace_probe
 * lockless, but we can't race with this __init function.
 */
static int kprobe_register(struct trace_event_call *event,
			   enum trace_reg type, void *data)
{
	struct trace_event_file *file = data;

	switch (type) {
	case TRACE_REG_REGISTER:
		return enable_trace_kprobe(event, file);
	case TRACE_REG_UNREGISTER:
		return disable_trace_kprobe(event, file);

#ifdef CONFIG_PERF_EVENTS
	case TRACE_REG_PERF_REGISTER:
		return enable_trace_kprobe(event, NULL);
	case TRACE_REG_PERF_UNREGISTER:
		return disable_trace_kprobe(event, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
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
static __kprobes
int kprobe_dispatcher(struct kprobe *kp, struct pt_regs *regs)
{
	struct trace_probe *tp = container_of(kp, struct trace_probe, rp.kp);

	if (tp->flags & TP_FLAG_TRACE)
		kprobe_trace_func(kp, regs);
#ifdef CONFIG_PERF_EVENTS
	if (tp->flags & TP_FLAG_PROFILE)
		kprobe_perf_func(kp, regs);
#endif
	return 0;	/* We don't tweek kernel, so just return 0 */
}

static __kprobes
int kretprobe_dispatcher(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	struct trace_probe *tp = container_of(ri->rp, struct trace_probe, rp);

	if (tp->flags & TP_FLAG_TRACE)
		kretprobe_trace_func(ri, regs);
#ifdef CONFIG_PERF_EVENTS
	if (tp->flags & TP_FLAG_PROFILE)
		kretprobe_perf_func(ri, regs);
#endif
	return 0;	/* We don't tweek kernel, so just return 0 */
}
=======
static int kprobe_dispatcher(struct kprobe *kp, struct pt_regs *regs)
{
	struct trace_kprobe *tk = container_of(kp, struct trace_kprobe, rp.kp);
	int ret = 0;

	raw_cpu_inc(*tk->nhit);

	if (trace_probe_test_flag(&tk->tp, TP_FLAG_TRACE))
		kprobe_trace_func(tk, regs);
#ifdef CONFIG_PERF_EVENTS
	if (trace_probe_test_flag(&tk->tp, TP_FLAG_PROFILE))
		ret = kprobe_perf_func(tk, regs);
#endif
	return ret;
}
NOKPROBE_SYMBOL(kprobe_dispatcher);

static int
kretprobe_dispatcher(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	struct kretprobe *rp = get_kretprobe(ri);
	struct trace_kprobe *tk;

	/*
	 * There is a small chance that get_kretprobe(ri) returns NULL when
	 * the kretprobe is unregister on another CPU between kretprobe's
	 * trampoline_handler and this function.
	 */
	if (unlikely(!rp))
		return 0;

	tk = container_of(rp, struct trace_kprobe, rp);
	raw_cpu_inc(*tk->nhit);

	if (trace_probe_test_flag(&tk->tp, TP_FLAG_TRACE))
		kretprobe_trace_func(tk, ri, regs);
#ifdef CONFIG_PERF_EVENTS
	if (trace_probe_test_flag(&tk->tp, TP_FLAG_PROFILE))
		kretprobe_perf_func(tk, ri, regs);
#endif
	return 0;	/* We don't tweak kernel, so just return 0 */
}
NOKPROBE_SYMBOL(kretprobe_dispatcher);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct trace_event_functions kretprobe_funcs = {
	.trace		= print_kretprobe_event
};

static struct trace_event_functions kprobe_funcs = {
	.trace		= print_kprobe_event
};

<<<<<<< HEAD
static int register_probe_event(struct trace_probe *tp)
{
	struct ftrace_event_call *call = &tp->call;
	int ret;

	/* Initialize ftrace_event_call */
	INIT_LIST_HEAD(&call->class->fields);
	if (trace_probe_is_return(tp)) {
		call->event.funcs = &kretprobe_funcs;
		call->class->define_fields = kretprobe_event_define_fields;
	} else {
		call->event.funcs = &kprobe_funcs;
		call->class->define_fields = kprobe_event_define_fields;
	}
	if (set_print_fmt(tp) < 0)
		return -ENOMEM;
	ret = register_ftrace_event(&call->event);
	if (!ret) {
		kfree(call->print_fmt);
		return -ENODEV;
	}
	call->flags = 0;
	call->class->reg = kprobe_register;
	call->data = tp;
	ret = trace_add_event_call(call);
	if (ret) {
		pr_info("Failed to register kprobe event: %s\n", call->name);
		kfree(call->print_fmt);
		unregister_ftrace_event(&call->event);
	}
	return ret;
}

static void unregister_probe_event(struct trace_probe *tp)
{
	/* tp->event is unregistered in trace_remove_event_call() */
	trace_remove_event_call(&tp->call);
	kfree(tp->call.print_fmt);
}

/* Make a debugfs interface for controlling probe points */
static __init int init_kprobe_trace(void)
{
	struct dentry *d_tracer;
	struct dentry *entry;

	if (register_module_notifier(&trace_probe_module_nb))
		return -EINVAL;

	d_tracer = tracing_init_dentry();
	if (!d_tracer)
		return 0;

	entry = debugfs_create_file("kprobe_events", 0644, d_tracer,
				    NULL, &kprobe_events_ops);

	/* Event list interface */
	if (!entry)
		pr_warning("Could not create debugfs "
			   "'kprobe_events' entry\n");

	/* Profile interface */
	entry = debugfs_create_file("kprobe_profile", 0444, d_tracer,
				    NULL, &kprobe_profile_ops);

	if (!entry)
		pr_warning("Could not create debugfs "
			   "'kprobe_profile' entry\n");
=======
static struct trace_event_fields kretprobe_fields_array[] = {
	{ .type = TRACE_FUNCTION_TYPE,
	  .define_fields = kretprobe_event_define_fields },
	{}
};

static struct trace_event_fields kprobe_fields_array[] = {
	{ .type = TRACE_FUNCTION_TYPE,
	  .define_fields = kprobe_event_define_fields },
	{}
};

static inline void init_trace_event_call(struct trace_kprobe *tk)
{
	struct trace_event_call *call = trace_probe_event_call(&tk->tp);

	if (trace_kprobe_is_return(tk)) {
		call->event.funcs = &kretprobe_funcs;
		call->class->fields_array = kretprobe_fields_array;
	} else {
		call->event.funcs = &kprobe_funcs;
		call->class->fields_array = kprobe_fields_array;
	}

	call->flags = TRACE_EVENT_FL_KPROBE;
	call->class->reg = kprobe_register;
}

static int register_kprobe_event(struct trace_kprobe *tk)
{
	init_trace_event_call(tk);

	return trace_probe_register_event_call(&tk->tp);
}

static int unregister_kprobe_event(struct trace_kprobe *tk)
{
	return trace_probe_unregister_event_call(&tk->tp);
}

#ifdef CONFIG_PERF_EVENTS

/* create a trace_kprobe, but don't add it to global lists */
struct trace_event_call *
create_local_trace_kprobe(char *func, void *addr, unsigned long offs,
			  bool is_return)
{
	enum probe_print_type ptype;
	struct trace_kprobe *tk;
	int ret;
	char *event;

	if (func) {
		unsigned int count;

		count = number_of_same_symbols(func);
		if (count > 1)
			/*
			 * Users should use addr to remove the ambiguity of
			 * using func only.
			 */
			return ERR_PTR(-EADDRNOTAVAIL);
		else if (count == 0)
			/*
			 * We can return ENOENT earlier than when register the
			 * kprobe.
			 */
			return ERR_PTR(-ENOENT);
	}

	/*
	 * local trace_kprobes are not added to dyn_event, so they are never
	 * searched in find_trace_kprobe(). Therefore, there is no concern of
	 * duplicated name here.
	 */
	event = func ? func : "DUMMY_EVENT";

	tk = alloc_trace_kprobe(KPROBE_EVENT_SYSTEM, event, (void *)addr, func,
				offs, 0 /* maxactive */, 0 /* nargs */,
				is_return);

	if (IS_ERR(tk)) {
		pr_info("Failed to allocate trace_probe.(%d)\n",
			(int)PTR_ERR(tk));
		return ERR_CAST(tk);
	}

	init_trace_event_call(tk);

	ptype = trace_kprobe_is_return(tk) ?
		PROBE_PRINT_RETURN : PROBE_PRINT_NORMAL;
	if (traceprobe_set_print_fmt(&tk->tp, ptype) < 0) {
		ret = -ENOMEM;
		goto error;
	}

	ret = __register_trace_kprobe(tk);
	if (ret < 0)
		goto error;

	return trace_probe_event_call(&tk->tp);
error:
	free_trace_kprobe(tk);
	return ERR_PTR(ret);
}

void destroy_local_trace_kprobe(struct trace_event_call *event_call)
{
	struct trace_kprobe *tk;

	tk = trace_kprobe_primary_from_call(event_call);
	if (unlikely(!tk))
		return;

	if (trace_probe_is_enabled(&tk->tp)) {
		WARN_ON(1);
		return;
	}

	__unregister_trace_kprobe(tk);

	free_trace_kprobe(tk);
}
#endif /* CONFIG_PERF_EVENTS */

static __init void enable_boot_kprobe_events(void)
{
	struct trace_array *tr = top_trace_array();
	struct trace_event_file *file;
	struct trace_kprobe *tk;
	struct dyn_event *pos;

	mutex_lock(&event_mutex);
	for_each_trace_kprobe(tk, pos) {
		list_for_each_entry(file, &tr->events, list)
			if (file->event_call == trace_probe_event_call(&tk->tp))
				trace_event_enable_disable(file, 1, 0);
	}
	mutex_unlock(&event_mutex);
}

static __init void setup_boot_kprobe_events(void)
{
	char *p, *cmd = kprobe_boot_events_buf;
	int ret;

	strreplace(kprobe_boot_events_buf, ',', ' ');

	while (cmd && *cmd != '\0') {
		p = strchr(cmd, ';');
		if (p)
			*p++ = '\0';

		ret = create_or_delete_trace_kprobe(cmd);
		if (ret)
			pr_warn("Failed to add event(%d): %s\n", ret, cmd);

		cmd = p;
	}

	enable_boot_kprobe_events();
}

/*
 * Register dynevent at core_initcall. This allows kernel to setup kprobe
 * events in postcore_initcall without tracefs.
 */
static __init int init_kprobe_trace_early(void)
{
	int ret;

	ret = dyn_event_register(&trace_kprobe_ops);
	if (ret)
		return ret;

	if (register_module_notifier(&trace_kprobe_module_nb))
		return -EINVAL;

	return 0;
}
core_initcall(init_kprobe_trace_early);

/* Make a tracefs interface for controlling probe points */
static __init int init_kprobe_trace(void)
{
	int ret;

	ret = tracing_init_dentry();
	if (ret)
		return 0;

	/* Event list interface */
	trace_create_file("kprobe_events", TRACE_MODE_WRITE,
			  NULL, NULL, &kprobe_events_ops);

	/* Profile interface */
	trace_create_file("kprobe_profile", TRACE_MODE_READ,
			  NULL, NULL, &kprobe_profile_ops);

	setup_boot_kprobe_events();

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
fs_initcall(init_kprobe_trace);


#ifdef CONFIG_FTRACE_STARTUP_TEST
<<<<<<< HEAD

/*
 * The "__used" keeps gcc from removing the function symbol
 * from the kallsyms table.
 */
static __used int kprobe_trace_selftest_target(int a1, int a2, int a3,
					       int a4, int a5, int a6)
{
	return a1 + a2 + a3 + a4 + a5 + a6;
}

=======
static __init struct trace_event_file *
find_trace_probe_file(struct trace_kprobe *tk, struct trace_array *tr)
{
	struct trace_event_file *file;

	list_for_each_entry(file, &tr->events, list)
		if (file->event_call == trace_probe_event_call(&tk->tp))
			return file;

	return NULL;
}

/*
 * Nobody but us can call enable_trace_kprobe/disable_trace_kprobe at this
 * stage, we can do this lockless.
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static __init int kprobe_trace_self_tests_init(void)
{
	int ret, warn = 0;
	int (*target)(int, int, int, int, int, int);
<<<<<<< HEAD
	struct trace_probe *tp;
=======
	struct trace_kprobe *tk;
	struct trace_event_file *file;

	if (tracing_is_disabled())
		return -ENODEV;

	if (tracing_selftest_disabled)
		return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	target = kprobe_trace_selftest_target;

	pr_info("Testing kprobe tracing: ");

<<<<<<< HEAD
	ret = command_trace_probe("p:testprobe kprobe_trace_selftest_target "
				  "$stack $stack0 +0($stack)");
	if (WARN_ON_ONCE(ret)) {
		pr_warning("error on probing function entry.\n");
		warn++;
	} else {
		/* Enable trace point */
		tp = find_trace_probe("testprobe", KPROBE_EVENT_SYSTEM);
		if (WARN_ON_ONCE(tp == NULL)) {
			pr_warning("error on getting new probe.\n");
			warn++;
		} else
			enable_trace_probe(tp, TP_FLAG_TRACE);
	}

	ret = command_trace_probe("r:testprobe2 kprobe_trace_selftest_target "
				  "$retval");
	if (WARN_ON_ONCE(ret)) {
		pr_warning("error on probing function return.\n");
		warn++;
	} else {
		/* Enable trace point */
		tp = find_trace_probe("testprobe2", KPROBE_EVENT_SYSTEM);
		if (WARN_ON_ONCE(tp == NULL)) {
			pr_warning("error on getting new probe.\n");
			warn++;
		} else
			enable_trace_probe(tp, TP_FLAG_TRACE);
=======
	ret = create_or_delete_trace_kprobe("p:testprobe kprobe_trace_selftest_target $stack $stack0 +0($stack)");
	if (WARN_ON_ONCE(ret)) {
		pr_warn("error on probing function entry.\n");
		warn++;
	} else {
		/* Enable trace point */
		tk = find_trace_kprobe("testprobe", KPROBE_EVENT_SYSTEM);
		if (WARN_ON_ONCE(tk == NULL)) {
			pr_warn("error on getting new probe.\n");
			warn++;
		} else {
			file = find_trace_probe_file(tk, top_trace_array());
			if (WARN_ON_ONCE(file == NULL)) {
				pr_warn("error on getting probe file.\n");
				warn++;
			} else
				enable_trace_kprobe(
					trace_probe_event_call(&tk->tp), file);
		}
	}

	ret = create_or_delete_trace_kprobe("r:testprobe2 kprobe_trace_selftest_target $retval");
	if (WARN_ON_ONCE(ret)) {
		pr_warn("error on probing function return.\n");
		warn++;
	} else {
		/* Enable trace point */
		tk = find_trace_kprobe("testprobe2", KPROBE_EVENT_SYSTEM);
		if (WARN_ON_ONCE(tk == NULL)) {
			pr_warn("error on getting 2nd new probe.\n");
			warn++;
		} else {
			file = find_trace_probe_file(tk, top_trace_array());
			if (WARN_ON_ONCE(file == NULL)) {
				pr_warn("error on getting probe file.\n");
				warn++;
			} else
				enable_trace_kprobe(
					trace_probe_event_call(&tk->tp), file);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (warn)
		goto end;

	ret = target(1, 2, 3, 4, 5, 6);

<<<<<<< HEAD
	/* Disable trace points before removing it */
	tp = find_trace_probe("testprobe", KPROBE_EVENT_SYSTEM);
	if (WARN_ON_ONCE(tp == NULL)) {
		pr_warning("error on getting test probe.\n");
		warn++;
	} else
		disable_trace_probe(tp, TP_FLAG_TRACE);

	tp = find_trace_probe("testprobe2", KPROBE_EVENT_SYSTEM);
	if (WARN_ON_ONCE(tp == NULL)) {
		pr_warning("error on getting 2nd test probe.\n");
		warn++;
	} else
		disable_trace_probe(tp, TP_FLAG_TRACE);

	ret = command_trace_probe("-:testprobe");
	if (WARN_ON_ONCE(ret)) {
		pr_warning("error on deleting a probe.\n");
		warn++;
	}

	ret = command_trace_probe("-:testprobe2");
	if (WARN_ON_ONCE(ret)) {
		pr_warning("error on deleting a probe.\n");
=======
	/*
	 * Not expecting an error here, the check is only to prevent the
	 * optimizer from removing the call to target() as otherwise there
	 * are no side-effects and the call is never performed.
	 */
	if (ret != 21)
		warn++;

	/* Disable trace points before removing it */
	tk = find_trace_kprobe("testprobe", KPROBE_EVENT_SYSTEM);
	if (WARN_ON_ONCE(tk == NULL)) {
		pr_warn("error on getting test probe.\n");
		warn++;
	} else {
		if (trace_kprobe_nhit(tk) != 1) {
			pr_warn("incorrect number of testprobe hits\n");
			warn++;
		}

		file = find_trace_probe_file(tk, top_trace_array());
		if (WARN_ON_ONCE(file == NULL)) {
			pr_warn("error on getting probe file.\n");
			warn++;
		} else
			disable_trace_kprobe(
				trace_probe_event_call(&tk->tp), file);
	}

	tk = find_trace_kprobe("testprobe2", KPROBE_EVENT_SYSTEM);
	if (WARN_ON_ONCE(tk == NULL)) {
		pr_warn("error on getting 2nd test probe.\n");
		warn++;
	} else {
		if (trace_kprobe_nhit(tk) != 1) {
			pr_warn("incorrect number of testprobe2 hits\n");
			warn++;
		}

		file = find_trace_probe_file(tk, top_trace_array());
		if (WARN_ON_ONCE(file == NULL)) {
			pr_warn("error on getting probe file.\n");
			warn++;
		} else
			disable_trace_kprobe(
				trace_probe_event_call(&tk->tp), file);
	}

	ret = create_or_delete_trace_kprobe("-:testprobe");
	if (WARN_ON_ONCE(ret)) {
		pr_warn("error on deleting a probe.\n");
		warn++;
	}

	ret = create_or_delete_trace_kprobe("-:testprobe2");
	if (WARN_ON_ONCE(ret)) {
		pr_warn("error on deleting a probe.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		warn++;
	}

end:
<<<<<<< HEAD
	release_all_trace_probes();
=======
	ret = dyn_events_release_all(&trace_kprobe_ops);
	if (WARN_ON_ONCE(ret)) {
		pr_warn("error on cleaning up probes.\n");
		warn++;
	}
	/*
	 * Wait for the optimizer work to finish. Otherwise it might fiddle
	 * with probes in already freed __init text.
	 */
	wait_for_kprobe_optimizer();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (warn)
		pr_cont("NG: Some tests are failed. Please check them.\n");
	else
		pr_cont("OK\n");
	return 0;
}

late_initcall(kprobe_trace_self_tests_init);

#endif
