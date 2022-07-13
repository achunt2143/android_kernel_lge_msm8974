<<<<<<< HEAD
#ifndef __PERF_SESSION_H
#define __PERF_SESSION_H

#include "hist.h"
#include "event.h"
#include "header.h"
#include "symbol.h"
#include "thread.h"
#include <linux/rbtree.h>
#include "../../../include/linux/perf_event.h"

struct sample_queue;
struct ip_callchain;
struct thread;

struct ordered_samples {
	u64			last_flush;
	u64			next_flush;
	u64			max_timestamp;
	struct list_head	samples;
	struct list_head	sample_cache;
	struct list_head	to_free;
	struct sample_queue	*sample_buffer;
	struct sample_queue	*last_sample;
	int			sample_buffer_idx;
	unsigned int		nr_samples;
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __PERF_SESSION_H
#define __PERF_SESSION_H

#include "trace-event.h"
#include "event.h"
#include "header.h"
#include "machine.h"
#include "data.h"
#include "ordered-events.h"
#include "util/compress.h"
#include <linux/kernel.h>
#include <linux/rbtree.h>
#include <linux/perf_event.h>

struct ip_callchain;
struct symbol;
struct thread;

struct auxtrace;
struct itrace_synth_opts;

struct decomp_data {
	struct decomp	 *decomp;
	struct decomp	 *decomp_last;
	struct zstd_data *zstd_decomp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct perf_session {
	struct perf_header	header;
<<<<<<< HEAD
	unsigned long		size;
	unsigned long		mmap_window;
	struct machine		host_machine;
	struct rb_root		machines;
	struct perf_evlist	*evlist;
	/*
	 * FIXME: Need to split this up further, we need global
	 *	  stats + per event stats. 'perf diff' also needs
	 *	  to properly support multiple events in a single
	 *	  perf.data file.
	 */
	struct hists		hists;
	u64			sample_type;
	int			sample_size;
	int			fd;
	bool			fd_pipe;
	bool			repipe;
	bool			sample_id_all;
	u16			id_hdr_size;
	int			cwdlen;
	char			*cwd;
	struct ordered_samples	ordered_samples;
	char			filename[1];
=======
	struct machines		machines;
	struct evlist	*evlist;
	struct auxtrace		*auxtrace;
	struct itrace_synth_opts *itrace_synth_opts;
	struct list_head	auxtrace_index;
#ifdef HAVE_LIBTRACEEVENT
	struct trace_event	tevent;
#endif
	struct perf_record_time_conv	time_conv;
	bool			repipe;
	bool			one_mmap;
	void			*one_mmap_addr;
	u64			one_mmap_offset;
	struct ordered_events	ordered_events;
	struct perf_data	*data;
	struct perf_tool	*tool;
	u64			bytes_transferred;
	u64			bytes_compressed;
	struct zstd_data	zstd_data;
	struct decomp_data	decomp_data;
	struct decomp_data	*active_decomp;
};

struct decomp {
	struct decomp *next;
	u64 file_pos;
	const char *file_path;
	size_t mmap_len;
	u64 head;
	size_t size;
	char data[];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct perf_tool;

<<<<<<< HEAD
struct perf_session *perf_session__new(const char *filename, int mode,
				       bool force, bool repipe,
				       struct perf_tool *tool);
void perf_session__delete(struct perf_session *self);

void perf_event_header__bswap(struct perf_event_header *self);

int __perf_session__process_events(struct perf_session *self,
				   u64 data_offset, u64 data_size, u64 size,
				   struct perf_tool *tool);
int perf_session__process_events(struct perf_session *self,
				 struct perf_tool *tool);

int perf_session__resolve_callchain(struct perf_session *self, struct perf_evsel *evsel,
=======
struct perf_session *__perf_session__new(struct perf_data *data,
					 bool repipe, int repipe_fd,
					 struct perf_tool *tool);

static inline struct perf_session *perf_session__new(struct perf_data *data,
						     struct perf_tool *tool)
{
	return __perf_session__new(data, false, -1, tool);
}

void perf_session__delete(struct perf_session *session);

void perf_event_header__bswap(struct perf_event_header *hdr);

int perf_session__peek_event(struct perf_session *session, off_t file_offset,
			     void *buf, size_t buf_sz,
			     union perf_event **event_ptr,
			     struct perf_sample *sample);
typedef int (*peek_events_cb_t)(struct perf_session *session,
				union perf_event *event, u64 offset,
				void *data);
int perf_session__peek_events(struct perf_session *session, u64 offset,
			      u64 size, peek_events_cb_t cb, void *data);

int perf_session__process_events(struct perf_session *session);

int perf_session__queue_event(struct perf_session *s, union perf_event *event,
			      u64 timestamp, u64 file_offset, const char *file_path);

void perf_tool__fill_defaults(struct perf_tool *tool);

int perf_session__resolve_callchain(struct perf_session *session,
				    struct evsel *evsel,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				    struct thread *thread,
				    struct ip_callchain *chain,
				    struct symbol **parent);

<<<<<<< HEAD
struct branch_info *machine__resolve_bstack(struct machine *self,
					    struct thread *thread,
					    struct branch_stack *bs);

bool perf_session__has_traces(struct perf_session *self, const char *msg);

void mem_bswap_64(void *src, int byte_size);
void perf_event__attr_swap(struct perf_event_attr *attr);

int perf_session__create_kernel_maps(struct perf_session *self);

void perf_session__update_sample_type(struct perf_session *self);
void perf_session__remove_thread(struct perf_session *self, struct thread *th);

static inline
struct machine *perf_session__find_host_machine(struct perf_session *self)
{
	return &self->host_machine;
}

static inline
struct machine *perf_session__find_machine(struct perf_session *self, pid_t pid)
{
	if (pid == HOST_KERNEL_ID)
		return &self->host_machine;
	return machines__find(&self->machines, pid);
}

static inline
struct machine *perf_session__findnew_machine(struct perf_session *self, pid_t pid)
{
	if (pid == HOST_KERNEL_ID)
		return &self->host_machine;
	return machines__findnew(&self->machines, pid);
}

static inline
void perf_session__process_machines(struct perf_session *self,
				    struct perf_tool *tool,
				    machine__process_t process)
{
	process(&self->host_machine, tool);
	return machines__process(&self->machines, process, tool);
}

struct thread *perf_session__findnew(struct perf_session *self, pid_t pid);
size_t perf_session__fprintf(struct perf_session *self, FILE *fp);

size_t perf_session__fprintf_dsos(struct perf_session *self, FILE *fp);

size_t perf_session__fprintf_dsos_buildid(struct perf_session *self,
					  FILE *fp, bool with_hits);

size_t perf_session__fprintf_nr_events(struct perf_session *session, FILE *fp);

static inline int perf_session__parse_sample(struct perf_session *session,
					     const union perf_event *event,
					     struct perf_sample *sample)
{
	return perf_event__parse_sample(event, session->sample_type,
					session->sample_size,
					session->sample_id_all, sample,
					session->header.needs_swap);
}

static inline int perf_session__synthesize_sample(struct perf_session *session,
						  union perf_event *event,
						  const struct perf_sample *sample)
{
	return perf_event__synthesize_sample(event, session->sample_type,
					     sample, session->header.needs_swap);
}

struct perf_evsel *perf_session__find_first_evtype(struct perf_session *session,
					    unsigned int type);

void perf_event__print_ip(union perf_event *event, struct perf_sample *sample,
			  struct machine *machine, struct perf_evsel *evsel,
			  int print_sym, int print_dso, int print_symoffset);

=======
bool perf_session__has_traces(struct perf_session *session, const char *msg);

void perf_event__attr_swap(struct perf_event_attr *attr);

int perf_session__create_kernel_maps(struct perf_session *session);

void perf_session__set_id_hdr_size(struct perf_session *session);

static inline
struct machine *perf_session__find_machine(struct perf_session *session, pid_t pid)
{
	return machines__find(&session->machines, pid);
}

static inline
struct machine *perf_session__findnew_machine(struct perf_session *session, pid_t pid)
{
	return machines__findnew(&session->machines, pid);
}

struct thread *perf_session__findnew(struct perf_session *session, pid_t pid);
int perf_session__register_idle_thread(struct perf_session *session);

size_t perf_session__fprintf(struct perf_session *session, FILE *fp);

size_t perf_session__fprintf_dsos(struct perf_session *session, FILE *fp);

size_t perf_session__fprintf_dsos_buildid(struct perf_session *session, FILE *fp,
					  bool (fn)(struct dso *dso, int parm), int parm);

size_t perf_session__fprintf_nr_events(struct perf_session *session, FILE *fp,
				       bool skip_empty);

void perf_session__dump_kmaps(struct perf_session *session);

struct evsel *perf_session__find_first_evtype(struct perf_session *session,
					    unsigned int type);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int perf_session__cpu_bitmap(struct perf_session *session,
			     const char *cpu_list, unsigned long *cpu_bitmap);

void perf_session__fprintf_info(struct perf_session *s, FILE *fp, bool full);
<<<<<<< HEAD
=======

struct evsel_str_handler;

#define perf_session__set_tracepoints_handlers(session, array) \
	__evlist__set_tracepoints_handlers(session->evlist, array, ARRAY_SIZE(array))

extern volatile int session_done;

#define session_done()	READ_ONCE(session_done)

int perf_session__deliver_synth_event(struct perf_session *session,
				      union perf_event *event,
				      struct perf_sample *sample);

int perf_event__process_id_index(struct perf_session *session,
				 union perf_event *event);

int perf_event__process_finished_round(struct perf_tool *tool,
				       union perf_event *event,
				       struct ordered_events *oe);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __PERF_SESSION_H */
