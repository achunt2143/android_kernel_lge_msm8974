<<<<<<< HEAD
#ifndef __PERF_TRACE_EVENTS_H
#define __PERF_TRACE_EVENTS_H

#include <stdbool.h>
#include "parse-events.h"

struct machine;
struct perf_sample;
union perf_event;
struct thread;

#define __unused __attribute__((unused))


#ifndef PAGE_MASK
#define PAGE_MASK (page_size - 1)
#endif

enum {
	RINGBUF_TYPE_PADDING		= 29,
	RINGBUF_TYPE_TIME_EXTEND	= 30,
	RINGBUF_TYPE_TIME_STAMP		= 31,
};

#ifndef TS_SHIFT
#define TS_SHIFT		27
#endif

#define NSECS_PER_SEC		1000000000ULL
#define NSECS_PER_USEC		1000ULL

enum format_flags {
	FIELD_IS_ARRAY		= 1,
	FIELD_IS_POINTER	= 2,
	FIELD_IS_SIGNED		= 4,
	FIELD_IS_STRING		= 8,
	FIELD_IS_DYNAMIC	= 16,
	FIELD_IS_FLAG		= 32,
	FIELD_IS_SYMBOLIC	= 64,
};

struct format_field {
	struct format_field	*next;
	char			*type;
	char			*name;
	int			offset;
	int			size;
	unsigned long		flags;
};

struct format {
	int			nr_common;
	int			nr_fields;
	struct format_field	*common_fields;
	struct format_field	*fields;
};

struct print_arg_atom {
	char			*atom;
};

struct print_arg_string {
	char			*string;
	int			offset;
};

struct print_arg_field {
	char			*name;
	struct format_field	*field;
};

struct print_flag_sym {
	struct print_flag_sym	*next;
	char			*value;
	char			*str;
};

struct print_arg_typecast {
	char 			*type;
	struct print_arg	*item;
};

struct print_arg_flags {
	struct print_arg	*field;
	char			*delim;
	struct print_flag_sym	*flags;
};

struct print_arg_symbol {
	struct print_arg	*field;
	struct print_flag_sym	*symbols;
};

struct print_arg;

struct print_arg_op {
	char			*op;
	int			prio;
	struct print_arg	*left;
	struct print_arg	*right;
};

struct print_arg_func {
	char			*name;
	struct print_arg	*args;
};

enum print_arg_type {
	PRINT_NULL,
	PRINT_ATOM,
	PRINT_FIELD,
	PRINT_FLAGS,
	PRINT_SYMBOL,
	PRINT_TYPE,
	PRINT_STRING,
	PRINT_OP,
};

struct print_arg {
	struct print_arg		*next;
	enum print_arg_type		type;
	union {
		struct print_arg_atom		atom;
		struct print_arg_field		field;
		struct print_arg_typecast	typecast;
		struct print_arg_flags		flags;
		struct print_arg_symbol		symbol;
		struct print_arg_func		func;
		struct print_arg_string		string;
		struct print_arg_op		op;
	};
};

struct print_fmt {
	char			*format;
	struct print_arg	*args;
};

struct event {
	struct event		*next;
	char			*name;
	int			id;
	int			flags;
	struct format		format;
	struct print_fmt	print_fmt;
	char			*system;
};

enum {
	EVENT_FL_ISFTRACE	= 0x01,
	EVENT_FL_ISPRINT	= 0x02,
	EVENT_FL_ISBPRINT	= 0x04,
	EVENT_FL_ISFUNC		= 0x08,
	EVENT_FL_ISFUNCENT	= 0x10,
	EVENT_FL_ISFUNCRET	= 0x20,

	EVENT_FL_FAILED		= 0x80000000
};

struct record {
	unsigned long long ts;
	int size;
	void *data;
};

struct record *trace_peek_data(int cpu);
struct record *trace_read_data(int cpu);

void parse_set_info(int nr_cpus, int long_sz);

ssize_t trace_report(int fd, bool repipe);

void *malloc_or_die(unsigned int size);

void parse_cmdlines(char *file, int size);
void parse_proc_kallsyms(char *file, unsigned int size);
void parse_ftrace_printk(char *file, unsigned int size);

void print_funcs(void);
void print_printk(void);

int parse_ftrace_file(char *buf, unsigned long size);
int parse_event_file(char *buf, unsigned long size, char *sys);
void print_trace_event(int cpu, void *data, int size);

extern int file_bigendian;
extern int host_bigendian;

int bigendian(void);

static inline unsigned short __data2host2(unsigned short data)
{
	unsigned short swap;

	if (host_bigendian == file_bigendian)
		return data;

	swap = ((data & 0xffULL) << 8) |
		((data & (0xffULL << 8)) >> 8);

	return swap;
}

static inline unsigned int __data2host4(unsigned int data)
{
	unsigned int swap;

	if (host_bigendian == file_bigendian)
		return data;

	swap = ((data & 0xffULL) << 24) |
		((data & (0xffULL << 8)) << 8) |
		((data & (0xffULL << 16)) >> 8) |
		((data & (0xffULL << 24)) >> 24);

	return swap;
}

static inline unsigned long long __data2host8(unsigned long long data)
{
	unsigned long long swap;

	if (host_bigendian == file_bigendian)
		return data;

	swap = ((data & 0xffULL) << 56) |
		((data & (0xffULL << 8)) << 40) |
		((data & (0xffULL << 16)) << 24) |
		((data & (0xffULL << 24)) << 8) |
		((data & (0xffULL << 32)) >> 8) |
		((data & (0xffULL << 40)) >> 24) |
		((data & (0xffULL << 48)) >> 40) |
		((data & (0xffULL << 56)) >> 56);

	return swap;
}

#define data2host2(ptr)		__data2host2(*(unsigned short *)ptr)
#define data2host4(ptr)		__data2host4(*(unsigned int *)ptr)
#define data2host8(ptr)		({				\
	unsigned long long __val;				\
								\
	memcpy(&__val, (ptr), sizeof(unsigned long long));	\
	__data2host8(__val);					\
})

extern int header_page_ts_offset;
extern int header_page_ts_size;
extern int header_page_size_offset;
extern int header_page_size_size;
extern int header_page_data_offset;
extern int header_page_data_size;

extern bool latency_format;

int trace_parse_common_type(void *data);
int trace_parse_common_pid(void *data);
int parse_common_pc(void *data);
int parse_common_flags(void *data);
int parse_common_lock_depth(void *data);
struct event *trace_find_event(int id);
struct event *trace_find_next_event(struct event *event);
unsigned long long read_size(void *ptr, int size);
unsigned long long
raw_field_value(struct event *event, const char *name, void *data);
void *raw_field_ptr(struct event *event, const char *name, void *data);
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _PERF_UTIL_TRACE_EVENT_H
#define _PERF_UTIL_TRACE_EVENT_H

#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <linux/types.h>

struct evlist;
struct machine;
struct perf_sample;
union perf_event;
struct perf_tool;
struct thread;
struct tep_plugin_list;
struct evsel;
struct tep_format_field;

struct trace_event {
	struct tep_handle	*pevent;
	struct tep_plugin_list	*plugin_list;
};

/* Computes a version number comparable with LIBTRACEEVENT_VERSION from Makefile.config. */
#define MAKE_LIBTRACEEVENT_VERSION(a, b, c) ((a)*255*255+(b)*255+(c))

typedef char *(tep_func_resolver_t)(void *priv,
				    unsigned long long *addrp, char **modp);

bool have_tracepoints(struct list_head *evlist);

int trace_event__init(struct trace_event *t);
void trace_event__cleanup(struct trace_event *t);
int trace_event__register_resolver(struct machine *machine,
				   tep_func_resolver_t *func);
struct tep_event*
trace_event__tp_format(const char *sys, const char *name);

struct tep_event *trace_event__tp_format_id(int id);

void event_format__fprintf(struct tep_event *event,
			   int cpu, void *data, int size, FILE *fp);

void event_format__print(struct tep_event *event,
			 int cpu, void *data, int size);

int parse_ftrace_file(struct tep_handle *pevent, char *buf, unsigned long size);
int parse_event_file(struct tep_handle *pevent,
		     char *buf, unsigned long size, char *sys);

unsigned long long
raw_field_value(struct tep_event *event, const char *name, void *data);

const char *parse_task_states(struct tep_format_field *state_field);

void parse_proc_kallsyms(struct tep_handle *pevent, char *file, unsigned int size);
void parse_ftrace_printk(struct tep_handle *pevent, char *file, unsigned int size);
void parse_saved_cmdline(struct tep_handle *pevent, char *file, unsigned int size);

ssize_t trace_report(int fd, struct trace_event *tevent, bool repipe);

unsigned long long read_size(struct tep_event *event, void *ptr, int size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
unsigned long long eval_flag(const char *flag);

int read_tracing_data(int fd, struct list_head *pattrs);

<<<<<<< HEAD
=======
/*
 * Return the tracepoint name in the format "subsystem:event_name",
 * callers should free the returned string.
 */
char *tracepoint_id_to_name(u64 config);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct tracing_data {
	/* size is only valid if temp is 'true' */
	ssize_t size;
	bool temp;
	char temp_file[50];
};

struct tracing_data *tracing_data_get(struct list_head *pattrs,
				      int fd, bool temp);
<<<<<<< HEAD
void tracing_data_put(struct tracing_data *tdata);


/* taken from kernel/trace/trace.h */
enum trace_flag_type {
	TRACE_FLAG_IRQS_OFF		= 0x01,
	TRACE_FLAG_IRQS_NOSUPPORT	= 0x02,
	TRACE_FLAG_NEED_RESCHED		= 0x04,
	TRACE_FLAG_HARDIRQ		= 0x08,
	TRACE_FLAG_SOFTIRQ		= 0x10,
};

struct scripting_ops {
	const char *name;
	int (*start_script) (const char *script, int argc, const char **argv);
	int (*stop_script) (void);
	void (*process_event) (union perf_event *event,
			       struct perf_sample *sample,
			       struct perf_evsel *evsel,
			       struct machine *machine,
			       struct thread *thread);
	int (*generate_script) (const char *outfile);
};

int script_spec_register(const char *spec, struct scripting_ops *ops);

void setup_perl_scripting(void);
void setup_python_scripting(void);
void setup_json_export(void);

struct scripting_context {
	void *event_data;
};

=======
int tracing_data_put(struct tracing_data *tdata);


struct addr_location;

struct perf_session;
struct perf_stat_config;

struct scripting_ops {
	const char *name;
	const char *dirname; /* For script path .../scripts/<dirname>/... */
	int (*start_script)(const char *script, int argc, const char **argv,
			    struct perf_session *session);
	int (*flush_script) (void);
	int (*stop_script) (void);
	void (*process_event) (union perf_event *event,
			       struct perf_sample *sample,
			       struct evsel *evsel,
			       struct addr_location *al,
			       struct addr_location *addr_al);
	void (*process_switch)(union perf_event *event,
			       struct perf_sample *sample,
			       struct machine *machine);
	void (*process_auxtrace_error)(struct perf_session *session,
				       union perf_event *event);
	void (*process_stat)(struct perf_stat_config *config,
			     struct evsel *evsel, u64 tstamp);
	void (*process_stat_interval)(u64 tstamp);
	void (*process_throttle)(union perf_event *event,
				 struct perf_sample *sample,
				 struct machine *machine);
	int (*generate_script) (struct tep_handle *pevent, const char *outfile);
};

extern unsigned int scripting_max_stack;

int script_spec_register(const char *spec, struct scripting_ops *ops);

void script_fetch_insn(struct perf_sample *sample, struct thread *thread,
		       struct machine *machine);

void setup_perl_scripting(void);
void setup_python_scripting(void);

struct scripting_context {
	struct tep_handle *pevent;
	void *event_data;
	union perf_event *event;
	struct perf_sample *sample;
	struct evsel *evsel;
	struct addr_location *al;
	struct addr_location *addr_al;
	struct perf_session *session;
};

void scripting_context__update(struct scripting_context *scripting_context,
			       union perf_event *event,
			       struct perf_sample *sample,
			       struct evsel *evsel,
			       struct addr_location *al,
			       struct addr_location *addr_al);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int common_pc(struct scripting_context *context);
int common_flags(struct scripting_context *context);
int common_lock_depth(struct scripting_context *context);

<<<<<<< HEAD
#endif /* __PERF_TRACE_EVENTS_H */
=======
#define SAMPLE_FLAGS_BUF_SIZE 64
int perf_sample__sprintf_flags(u32 flags, char *str, size_t sz);

#if defined(LIBTRACEEVENT_VERSION) &&  LIBTRACEEVENT_VERSION >= MAKE_LIBTRACEEVENT_VERSION(1, 5, 0)
#include <traceevent/event-parse.h>

static inline bool tep_field_is_relative(unsigned long flags)
{
	return (flags & TEP_FIELD_IS_RELATIVE) != 0;
}
#else
#include <linux/compiler.h>

static inline bool tep_field_is_relative(unsigned long flags __maybe_unused)
{
	return false;
}
#endif

#endif /* _PERF_UTIL_TRACE_EVENT_H */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
