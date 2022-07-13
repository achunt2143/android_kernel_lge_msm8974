<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __PERF_TOOL_H
#define __PERF_TOOL_H

#include <stdbool.h>

<<<<<<< HEAD
struct perf_session;
union perf_event;
struct perf_evlist;
struct perf_evsel;
struct perf_sample;
struct perf_tool;
struct machine;

typedef int (*event_sample)(struct perf_tool *tool, union perf_event *event,
			    struct perf_sample *sample,
			    struct perf_evsel *evsel, struct machine *machine);
=======
#include <linux/types.h>

struct perf_session;
union perf_event;
struct evlist;
struct evsel;
struct perf_sample;
struct perf_tool;
struct machine;
struct ordered_events;

typedef int (*event_sample)(struct perf_tool *tool, union perf_event *event,
			    struct perf_sample *sample,
			    struct evsel *evsel, struct machine *machine);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

typedef int (*event_op)(struct perf_tool *tool, union perf_event *event,
			struct perf_sample *sample, struct machine *machine);

<<<<<<< HEAD
typedef int (*event_attr_op)(union perf_event *event,
			     struct perf_evlist **pevlist);
typedef int (*event_simple_op)(struct perf_tool *tool, union perf_event *event);

typedef int (*event_synth_op)(union perf_event *event,
			      struct perf_session *session);

typedef int (*event_op2)(struct perf_tool *tool, union perf_event *event,
			 struct perf_session *session);
=======
typedef int (*event_attr_op)(struct perf_tool *tool,
			     union perf_event *event,
			     struct evlist **pevlist);

typedef int (*event_op2)(struct perf_session *session, union perf_event *event);
typedef s64 (*event_op3)(struct perf_session *session, union perf_event *event);
typedef int (*event_op4)(struct perf_session *session, union perf_event *event, u64 data,
			 const char *str);

typedef int (*event_oe)(struct perf_tool *tool, union perf_event *event,
			struct ordered_events *oe);

enum show_feature_header {
	SHOW_FEAT_NO_HEADER = 0,
	SHOW_FEAT_HEADER,
	SHOW_FEAT_HEADER_FULL_INFO,
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct perf_tool {
	event_sample	sample,
			read;
	event_op	mmap,
<<<<<<< HEAD
			comm,
			fork,
			exit,
			lost,
			throttle,
			unthrottle;
	event_attr_op	attr;
	event_synth_op	tracing_data;
	event_simple_op	event_type;
	event_op2	finished_round,
			build_id;
	bool		ordered_samples;
	bool		ordering_requires_timestamps;
=======
			mmap2,
			comm,
			namespaces,
			cgroup,
			fork,
			exit,
			lost,
			lost_samples,
			aux,
			itrace_start,
			aux_output_hw_id,
			context_switch,
			throttle,
			unthrottle,
			ksymbol,
			bpf,
			text_poke;

	event_attr_op	attr;
	event_attr_op	event_update;
	event_op2	tracing_data;
	event_oe	finished_round;
	event_op2	build_id,
			id_index,
			auxtrace_info,
			auxtrace_error,
			time_conv,
			thread_map,
			cpu_map,
			stat_config,
			stat,
			stat_round,
			feature,
			finished_init;
	event_op4	compressed;
	event_op3	auxtrace;
	bool		ordered_events;
	bool		ordering_requires_timestamps;
	bool		namespace_events;
	bool		cgroup_events;
	bool		no_warn;
	enum show_feature_header show_feat_hdr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#endif /* __PERF_TOOL_H */
