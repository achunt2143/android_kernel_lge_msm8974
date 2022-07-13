<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __PERF_TOP_H
#define __PERF_TOP_H 1

#include "tool.h"
<<<<<<< HEAD
#include "types.h"
#include <stddef.h>
#include <stdbool.h>

struct perf_evlist;
struct perf_evsel;
=======
#include "evswitch.h"
#include "annotate.h"
#include "mutex.h"
#include "ordered-events.h"
#include "record.h"
#include <linux/types.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/ioctl.h>

struct evlist;
struct evsel;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct perf_session;

struct perf_top {
	struct perf_tool   tool;
<<<<<<< HEAD
	struct perf_evlist *evlist;
=======
	struct evlist *evlist, *sb_evlist;
	struct record_opts record_opts;
	struct evswitch	   evswitch;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Symbols will be added here in perf_event__process_sample and will
	 * get out after decayed.
	 */
<<<<<<< HEAD
	u64		   samples;
=======
	u64		   samples, lost, lost_total, drop, drop_total;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u64		   kernel_samples, us_samples;
	u64		   exact_samples;
	u64		   guest_us_samples, guest_kernel_samples;
	int		   print_entries, count_filter, delay_secs;
<<<<<<< HEAD
	int		   freq;
	const char	   *target_pid, *target_tid;
	uid_t		   uid;
	bool		   hide_kernel_symbols, hide_user_symbols, zero;
	bool		   system_wide;
	bool		   use_tui, use_stdio;
	bool		   sort_has_symbols;
	bool		   dont_use_callchains;
	bool		   kptr_restrict_warned;
	bool		   vmlinux_warned;
	bool		   inherit;
	bool		   group;
	bool		   sample_id_all_missing;
	bool		   exclude_guest_missing;
	bool		   dump_symtab;
	const char	   *cpu_list;
	struct hist_entry  *sym_filter_entry;
	struct perf_evsel  *sym_evsel;
	struct perf_session *session;
	struct winsize	   winsize;
	unsigned int	   mmap_pages;
	int		   default_interval;
	int		   realtime_prio;
	int		   sym_pcnt_filter;
	const char	   *sym_filter;
	const char	   *uid_str;
};

=======
	int		   max_stack;
	bool		   hide_kernel_symbols, hide_user_symbols, zero;
#ifdef HAVE_SLANG_SUPPORT
	bool		   use_tui;
#endif
	bool		   use_stdio;
	bool		   vmlinux_warned;
	bool		   dump_symtab;
	bool		   stitch_lbr;
	struct hist_entry  *sym_filter_entry;
	struct evsel 	   *sym_evsel;
	struct perf_session *session;
	struct winsize	   winsize;
	int		   realtime_prio;
	const char	   *sym_filter;
	float		   min_percent;
	unsigned int	   nr_threads_synthesize;

	struct {
		struct ordered_events	*in;
		struct ordered_events	 data[2];
		bool			 rotate;
		struct mutex mutex;
		struct cond cond;
	} qe;
};

#define CONSOLE_CLEAR "[H[2J"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
size_t perf_top__header_snprintf(struct perf_top *top, char *bf, size_t size);
void perf_top__reset_sample_counters(struct perf_top *top);
#endif /* __PERF_TOP_H */
