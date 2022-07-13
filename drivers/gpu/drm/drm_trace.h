<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#if !defined(_DRM_TRACE_H_) || defined(TRACE_HEADER_MULTI_READ)
#define _DRM_TRACE_H_

#include <linux/stringify.h>
#include <linux/types.h>
#include <linux/tracepoint.h>

<<<<<<< HEAD
#undef TRACE_SYSTEM
#define TRACE_SYSTEM drm
#define TRACE_SYSTEM_STRING __stringify(TRACE_SYSTEM)
#define TRACE_INCLUDE_FILE drm_trace

TRACE_EVENT(drm_vblank_event,
	    TP_PROTO(int crtc, unsigned int seq),
	    TP_ARGS(crtc, seq),
	    TP_STRUCT__entry(
		    __field(int, crtc)
		    __field(unsigned int, seq)
=======
struct drm_file;

#undef TRACE_SYSTEM
#define TRACE_SYSTEM drm
#define TRACE_INCLUDE_FILE drm_trace

TRACE_EVENT(drm_vblank_event,
	    TP_PROTO(int crtc, unsigned int seq, ktime_t time, bool high_prec),
	    TP_ARGS(crtc, seq, time, high_prec),
	    TP_STRUCT__entry(
		    __field(int, crtc)
		    __field(unsigned int, seq)
		    __field(ktime_t, time)
		    __field(bool, high_prec)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		    ),
	    TP_fast_assign(
		    __entry->crtc = crtc;
		    __entry->seq = seq;
<<<<<<< HEAD
		    ),
	    TP_printk("crtc=%d, seq=%d", __entry->crtc, __entry->seq)
);

TRACE_EVENT(drm_vblank_event_queued,
	    TP_PROTO(pid_t pid, int crtc, unsigned int seq),
	    TP_ARGS(pid, crtc, seq),
	    TP_STRUCT__entry(
		    __field(pid_t, pid)
=======
		    __entry->time = time;
		    __entry->high_prec = high_prec;
			),
	    TP_printk("crtc=%d, seq=%u, time=%lld, high-prec=%s",
			__entry->crtc, __entry->seq, __entry->time,
			__entry->high_prec ? "true" : "false")
);

TRACE_EVENT(drm_vblank_event_queued,
	    TP_PROTO(struct drm_file *file, int crtc, unsigned int seq),
	    TP_ARGS(file, crtc, seq),
	    TP_STRUCT__entry(
		    __field(struct drm_file *, file)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		    __field(int, crtc)
		    __field(unsigned int, seq)
		    ),
	    TP_fast_assign(
<<<<<<< HEAD
		    __entry->pid = pid;
		    __entry->crtc = crtc;
		    __entry->seq = seq;
		    ),
	    TP_printk("pid=%d, crtc=%d, seq=%d", __entry->pid, __entry->crtc, \
=======
		    __entry->file = file;
		    __entry->crtc = crtc;
		    __entry->seq = seq;
		    ),
	    TP_printk("file=%p, crtc=%d, seq=%u", __entry->file, __entry->crtc, \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		      __entry->seq)
);

TRACE_EVENT(drm_vblank_event_delivered,
<<<<<<< HEAD
	    TP_PROTO(pid_t pid, int crtc, unsigned int seq),
	    TP_ARGS(pid, crtc, seq),
	    TP_STRUCT__entry(
		    __field(pid_t, pid)
=======
	    TP_PROTO(struct drm_file *file, int crtc, unsigned int seq),
	    TP_ARGS(file, crtc, seq),
	    TP_STRUCT__entry(
		    __field(struct drm_file *, file)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		    __field(int, crtc)
		    __field(unsigned int, seq)
		    ),
	    TP_fast_assign(
<<<<<<< HEAD
		    __entry->pid = pid;
		    __entry->crtc = crtc;
		    __entry->seq = seq;
		    ),
	    TP_printk("pid=%d, crtc=%d, seq=%d", __entry->pid, __entry->crtc, \
=======
		    __entry->file = file;
		    __entry->crtc = crtc;
		    __entry->seq = seq;
		    ),
	    TP_printk("file=%p, crtc=%d, seq=%u", __entry->file, __entry->crtc, \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		      __entry->seq)
);

#endif /* _DRM_TRACE_H_ */

/* This part must be outside protection */
#undef TRACE_INCLUDE_PATH
<<<<<<< HEAD
#define TRACE_INCLUDE_PATH .
=======
#define TRACE_INCLUDE_PATH ../../drivers/gpu/drm
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <trace/define_trace.h>
