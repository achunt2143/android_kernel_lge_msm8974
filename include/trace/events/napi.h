<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#undef TRACE_SYSTEM
#define TRACE_SYSTEM napi

#if !defined(_TRACE_NAPI_H) || defined(TRACE_HEADER_MULTI_READ)
<<<<<<< HEAD
#define _TRACE_NAPI_H_
=======
#define _TRACE_NAPI_H
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/netdevice.h>
#include <linux/tracepoint.h>
#include <linux/ftrace.h>

#define NO_DEV "(no_device)"

TRACE_EVENT(napi_poll,

<<<<<<< HEAD
	TP_PROTO(struct napi_struct *napi),

	TP_ARGS(napi),
=======
	TP_PROTO(struct napi_struct *napi, int work, int budget),

	TP_ARGS(napi, work, budget),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	TP_STRUCT__entry(
		__field(	struct napi_struct *,	napi)
		__string(	dev_name, napi->dev ? napi->dev->name : NO_DEV)
<<<<<<< HEAD
=======
		__field(	int,			work)
		__field(	int,			budget)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	),

	TP_fast_assign(
		__entry->napi = napi;
		__assign_str(dev_name, napi->dev ? napi->dev->name : NO_DEV);
<<<<<<< HEAD
	),

	TP_printk("napi poll on napi struct %p for device %s",
		__entry->napi, __get_str(dev_name))
=======
		__entry->work = work;
		__entry->budget = budget;
	),

	TP_printk("napi poll on napi struct %p for device %s work %d budget %d",
		  __entry->napi, __get_str(dev_name),
		  __entry->work, __entry->budget)
);

TRACE_EVENT(dql_stall_detected,

	TP_PROTO(unsigned short thrs, unsigned int len,
		 unsigned long last_reap, unsigned long hist_head,
		 unsigned long now, unsigned long *hist),

	TP_ARGS(thrs, len, last_reap, hist_head, now, hist),

	TP_STRUCT__entry(
		__field(	unsigned short,		thrs)
		__field(	unsigned int,		len)
		__field(	unsigned long,		last_reap)
		__field(	unsigned long,		hist_head)
		__field(	unsigned long,		now)
		__array(	unsigned long,		hist, 4)
	),

	TP_fast_assign(
		__entry->thrs = thrs;
		__entry->len = len;
		__entry->last_reap = last_reap;
		__entry->hist_head = hist_head * BITS_PER_LONG;
		__entry->now = now;
		memcpy(__entry->hist, hist, sizeof(entry->hist));
	),

	TP_printk("thrs %u  len %u  last_reap %lu  hist_head %lu  now %lu  hist %016lx %016lx %016lx %016lx",
		  __entry->thrs, __entry->len,
		  __entry->last_reap, __entry->hist_head, __entry->now,
		  __entry->hist[0], __entry->hist[1],
		  __entry->hist[2], __entry->hist[3])
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
);

#undef NO_DEV

<<<<<<< HEAD
#endif /* _TRACE_NAPI_H_ */
=======
#endif /* _TRACE_NAPI_H */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* This part must be outside protection */
#include <trace/define_trace.h>
