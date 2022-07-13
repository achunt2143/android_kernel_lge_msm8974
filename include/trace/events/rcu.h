<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#undef TRACE_SYSTEM
#define TRACE_SYSTEM rcu

#if !defined(_TRACE_RCU_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_RCU_H

#include <linux/tracepoint.h>

<<<<<<< HEAD
=======
#ifdef CONFIG_RCU_TRACE
#define TRACE_EVENT_RCU TRACE_EVENT
#else
#define TRACE_EVENT_RCU TRACE_EVENT_NOP
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Tracepoint for start/end markers used for utilization calculations.
 * By convention, the string is of the following forms:
 *
 * "Start <activity>" -- Mark the start of the specified activity,
 *			 such as "context switch".  Nesting is permitted.
 * "End <activity>" -- Mark the end of the specified activity.
 *
 * An "@" character within "<activity>" is a comment character: Data
 * reduction scripts will ignore the "@" and the remainder of the line.
 */
TRACE_EVENT(rcu_utilization,

<<<<<<< HEAD
	TP_PROTO(char *s),
=======
	TP_PROTO(const char *s),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	TP_ARGS(s),

	TP_STRUCT__entry(
<<<<<<< HEAD
		__field(char *, s)
=======
		__field(const char *, s)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	),

	TP_fast_assign(
		__entry->s = s;
	),

	TP_printk("%s", __entry->s)
);

<<<<<<< HEAD
#ifdef CONFIG_RCU_TRACE

#if defined(CONFIG_TREE_RCU) || defined(CONFIG_TREE_PREEMPT_RCU)

/*
 * Tracepoint for grace-period events: starting and ending a grace
 * period ("start" and "end", respectively), a CPU noting the start
 * of a new grace period or the end of an old grace period ("cpustart"
 * and "cpuend", respectively), a CPU passing through a quiescent
 * state ("cpuqs"), a CPU coming online or going offline ("cpuonl"
 * and "cpuofl", respectively), and a CPU being kicked for being too
 * long in dyntick-idle mode ("kick").
 */
TRACE_EVENT(rcu_grace_period,

	TP_PROTO(char *rcuname, unsigned long gpnum, char *gpevent),

	TP_ARGS(rcuname, gpnum, gpevent),

	TP_STRUCT__entry(
		__field(char *, rcuname)
		__field(unsigned long, gpnum)
		__field(char *, gpevent)
=======
#if defined(CONFIG_TREE_RCU)

/*
 * Tracepoint for grace-period events.  Takes a string identifying the
 * RCU flavor, the grace-period number, and a string identifying the
 * grace-period-related event as follows:
 *
 *	"AccReadyCB": CPU accelerates new callbacks to RCU_NEXT_READY_TAIL.
 *	"AccWaitCB": CPU accelerates new callbacks to RCU_WAIT_TAIL.
 *	"newreq": Request a new grace period.
 *	"start": Start a grace period.
 *	"cpustart": CPU first notices a grace-period start.
 *	"cpuqs": CPU passes through a quiescent state.
 *	"cpuonl": CPU comes online.
 *	"cpuofl": CPU goes offline.
 *	"cpuofl-bgp": CPU goes offline while blocking a grace period.
 *	"reqwait": GP kthread sleeps waiting for grace-period request.
 *	"reqwaitsig": GP kthread awakened by signal from reqwait state.
 *	"fqswait": GP kthread waiting until time to force quiescent states.
 *	"fqsstart": GP kthread starts forcing quiescent states.
 *	"fqsend": GP kthread done forcing quiescent states.
 *	"fqswaitsig": GP kthread awakened by signal from fqswait state.
 *	"end": End a grace period.
 *	"cpuend": CPU first notices a grace-period end.
 */
TRACE_EVENT_RCU(rcu_grace_period,

	TP_PROTO(const char *rcuname, unsigned long gp_seq, const char *gpevent),

	TP_ARGS(rcuname, gp_seq, gpevent),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(long, gp_seq)
		__field(const char *, gpevent)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
<<<<<<< HEAD
		__entry->gpnum = gpnum;
		__entry->gpevent = gpevent;
	),

	TP_printk("%s %lu %s",
		  __entry->rcuname, __entry->gpnum, __entry->gpevent)
=======
		__entry->gp_seq = (long)gp_seq;
		__entry->gpevent = gpevent;
	),

	TP_printk("%s %ld %s",
		  __entry->rcuname, __entry->gp_seq, __entry->gpevent)
);

/*
 * Tracepoint for future grace-period events.  The caller should pull
 * the data from the rcu_node structure, other than rcuname, which comes
 * from the rcu_state structure, and event, which is one of the following:
 *
 * "Cleanup": Clean up rcu_node structure after previous GP.
 * "CleanupMore": Clean up, and another GP is needed.
 * "EndWait": Complete wait.
 * "NoGPkthread": The RCU grace-period kthread has not yet started.
 * "Prestarted": Someone beat us to the request
 * "Startedleaf": Leaf node marked for future GP.
 * "Startedleafroot": All nodes from leaf to root marked for future GP.
 * "Startedroot": Requested a nocb grace period based on root-node data.
 * "Startleaf": Request a grace period based on leaf-node data.
 * "StartWait": Start waiting for the requested grace period.
 */
TRACE_EVENT_RCU(rcu_future_grace_period,

	TP_PROTO(const char *rcuname, unsigned long gp_seq,
		 unsigned long gp_seq_req, u8 level, int grplo, int grphi,
		 const char *gpevent),

	TP_ARGS(rcuname, gp_seq, gp_seq_req, level, grplo, grphi, gpevent),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(long, gp_seq)
		__field(long, gp_seq_req)
		__field(u8, level)
		__field(int, grplo)
		__field(int, grphi)
		__field(const char *, gpevent)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->gp_seq = (long)gp_seq;
		__entry->gp_seq_req = (long)gp_seq_req;
		__entry->level = level;
		__entry->grplo = grplo;
		__entry->grphi = grphi;
		__entry->gpevent = gpevent;
	),

	TP_printk("%s %ld %ld %u %d %d %s",
		  __entry->rcuname, (long)__entry->gp_seq, (long)__entry->gp_seq_req, __entry->level,
		  __entry->grplo, __entry->grphi, __entry->gpevent)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
);

/*
 * Tracepoint for grace-period-initialization events.  These are
 * distinguished by the type of RCU, the new grace-period number, the
 * rcu_node structure level, the starting and ending CPU covered by the
 * rcu_node structure, and the mask of CPUs that will be waited for.
 * All but the type of RCU are extracted from the rcu_node structure.
 */
<<<<<<< HEAD
TRACE_EVENT(rcu_grace_period_init,

	TP_PROTO(char *rcuname, unsigned long gpnum, u8 level,
		 int grplo, int grphi, unsigned long qsmask),

	TP_ARGS(rcuname, gpnum, level, grplo, grphi, qsmask),

	TP_STRUCT__entry(
		__field(char *, rcuname)
		__field(unsigned long, gpnum)
=======
TRACE_EVENT_RCU(rcu_grace_period_init,

	TP_PROTO(const char *rcuname, unsigned long gp_seq, u8 level,
		 int grplo, int grphi, unsigned long qsmask),

	TP_ARGS(rcuname, gp_seq, level, grplo, grphi, qsmask),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(long, gp_seq)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__field(u8, level)
		__field(int, grplo)
		__field(int, grphi)
		__field(unsigned long, qsmask)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
<<<<<<< HEAD
		__entry->gpnum = gpnum;
=======
		__entry->gp_seq = (long)gp_seq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__entry->level = level;
		__entry->grplo = grplo;
		__entry->grphi = grphi;
		__entry->qsmask = qsmask;
	),

<<<<<<< HEAD
	TP_printk("%s %lu %u %d %d %lx",
		  __entry->rcuname, __entry->gpnum, __entry->level,
=======
	TP_printk("%s %ld %u %d %d %lx",
		  __entry->rcuname, __entry->gp_seq, __entry->level,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		  __entry->grplo, __entry->grphi, __entry->qsmask)
);

/*
<<<<<<< HEAD
=======
 * Tracepoint for expedited grace-period events.  Takes a string identifying
 * the RCU flavor, the expedited grace-period sequence number, and a string
 * identifying the grace-period-related event as follows:
 *
 *	"snap": Captured snapshot of expedited grace period sequence number.
 *	"start": Started a real expedited grace period.
 *	"reset": Started resetting the tree
 *	"select": Started selecting the CPUs to wait on.
 *	"selectofl": Selected CPU partially offline.
 *	"startwait": Started waiting on selected CPUs.
 *	"end": Ended a real expedited grace period.
 *	"endwake": Woke piggybackers up.
 *	"done": Someone else did the expedited grace period for us.
 */
TRACE_EVENT_RCU(rcu_exp_grace_period,

	TP_PROTO(const char *rcuname, unsigned long gpseq, const char *gpevent),

	TP_ARGS(rcuname, gpseq, gpevent),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(long, gpseq)
		__field(const char *, gpevent)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->gpseq = (long)gpseq;
		__entry->gpevent = gpevent;
	),

	TP_printk("%s %ld %s",
		  __entry->rcuname, __entry->gpseq, __entry->gpevent)
);

/*
 * Tracepoint for expedited grace-period funnel-locking events.  Takes a
 * string identifying the RCU flavor, an integer identifying the rcu_node
 * combining-tree level, another pair of integers identifying the lowest-
 * and highest-numbered CPU associated with the current rcu_node structure,
 * and a string.  identifying the grace-period-related event as follows:
 *
 *	"nxtlvl": Advance to next level of rcu_node funnel
 *	"wait": Wait for someone else to do expedited GP
 */
TRACE_EVENT_RCU(rcu_exp_funnel_lock,

	TP_PROTO(const char *rcuname, u8 level, int grplo, int grphi,
		 const char *gpevent),

	TP_ARGS(rcuname, level, grplo, grphi, gpevent),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(u8, level)
		__field(int, grplo)
		__field(int, grphi)
		__field(const char *, gpevent)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->level = level;
		__entry->grplo = grplo;
		__entry->grphi = grphi;
		__entry->gpevent = gpevent;
	),

	TP_printk("%s %d %d %d %s",
		  __entry->rcuname, __entry->level, __entry->grplo,
		  __entry->grphi, __entry->gpevent)
);

#ifdef CONFIG_RCU_NOCB_CPU
/*
 * Tracepoint for RCU no-CBs CPU callback handoffs.  This event is intended
 * to assist debugging of these handoffs.
 *
 * The first argument is the name of the RCU flavor, and the second is
 * the number of the offloaded CPU are extracted.  The third and final
 * argument is a string as follows:
 *
 * "AlreadyAwake": The to-be-awakened rcuo kthread is already awake.
 * "Bypass": rcuo GP kthread sees non-empty ->nocb_bypass.
 * "CBSleep": rcuo CB kthread sleeping waiting for CBs.
 * "Check": rcuo GP kthread checking specified CPU for work.
 * "DeferredWake": Timer expired or polled check, time to wake.
 * "DoWake": The to-be-awakened rcuo kthread needs to be awakened.
 * "EndSleep": Done waiting for GP for !rcu_nocb_poll.
 * "FirstBQ": New CB to empty ->nocb_bypass (->cblist maybe non-empty).
 * "FirstBQnoWake": FirstBQ plus rcuo kthread need not be awakened.
 * "FirstBQwake": FirstBQ plus rcuo kthread must be awakened.
 * "FirstQ": New CB to empty ->cblist (->nocb_bypass maybe non-empty).
 * "NeedWaitGP": rcuo GP kthread must wait on a grace period.
 * "Poll": Start of new polling cycle for rcu_nocb_poll.
 * "Sleep": Sleep waiting for GP for !rcu_nocb_poll.
 * "Timer": Deferred-wake timer expired.
 * "WakeEmptyIsDeferred": Wake rcuo kthread later, first CB to empty list.
 * "WakeEmpty": Wake rcuo kthread, first CB to empty list.
 * "WakeNot": Don't wake rcuo kthread.
 * "WakeNotPoll": Don't wake rcuo kthread because it is polling.
 * "WakeOvfIsDeferred": Wake rcuo kthread later, CB list is huge.
 * "WakeBypassIsDeferred": Wake rcuo kthread later, bypass list is contended.
 * "WokeEmpty": rcuo CB kthread woke to find empty list.
 */
TRACE_EVENT_RCU(rcu_nocb_wake,

	TP_PROTO(const char *rcuname, int cpu, const char *reason),

	TP_ARGS(rcuname, cpu, reason),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(int, cpu)
		__field(const char *, reason)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->cpu = cpu;
		__entry->reason = reason;
	),

	TP_printk("%s %d %s", __entry->rcuname, __entry->cpu, __entry->reason)
);
#endif

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Tracepoint for tasks blocking within preemptible-RCU read-side
 * critical sections.  Track the type of RCU (which one day might
 * include SRCU), the grace-period number that the task is blocking
 * (the current or the next), and the task's PID.
 */
<<<<<<< HEAD
TRACE_EVENT(rcu_preempt_task,

	TP_PROTO(char *rcuname, int pid, unsigned long gpnum),

	TP_ARGS(rcuname, pid, gpnum),

	TP_STRUCT__entry(
		__field(char *, rcuname)
		__field(unsigned long, gpnum)
=======
TRACE_EVENT_RCU(rcu_preempt_task,

	TP_PROTO(const char *rcuname, int pid, unsigned long gp_seq),

	TP_ARGS(rcuname, pid, gp_seq),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(long, gp_seq)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__field(int, pid)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
<<<<<<< HEAD
		__entry->gpnum = gpnum;
		__entry->pid = pid;
	),

	TP_printk("%s %lu %d",
		  __entry->rcuname, __entry->gpnum, __entry->pid)
=======
		__entry->gp_seq = (long)gp_seq;
		__entry->pid = pid;
	),

	TP_printk("%s %ld %d",
		  __entry->rcuname, __entry->gp_seq, __entry->pid)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
);

/*
 * Tracepoint for tasks that blocked within a given preemptible-RCU
 * read-side critical section exiting that critical section.  Track the
 * type of RCU (which one day might include SRCU) and the task's PID.
 */
<<<<<<< HEAD
TRACE_EVENT(rcu_unlock_preempted_task,

	TP_PROTO(char *rcuname, unsigned long gpnum, int pid),

	TP_ARGS(rcuname, gpnum, pid),

	TP_STRUCT__entry(
		__field(char *, rcuname)
		__field(unsigned long, gpnum)
=======
TRACE_EVENT_RCU(rcu_unlock_preempted_task,

	TP_PROTO(const char *rcuname, unsigned long gp_seq, int pid),

	TP_ARGS(rcuname, gp_seq, pid),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(long, gp_seq)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__field(int, pid)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
<<<<<<< HEAD
		__entry->gpnum = gpnum;
		__entry->pid = pid;
	),

	TP_printk("%s %lu %d", __entry->rcuname, __entry->gpnum, __entry->pid)
=======
		__entry->gp_seq = (long)gp_seq;
		__entry->pid = pid;
	),

	TP_printk("%s %ld %d", __entry->rcuname, __entry->gp_seq, __entry->pid)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
);

/*
 * Tracepoint for quiescent-state-reporting events.  These are
 * distinguished by the type of RCU, the grace-period number, the
 * mask of quiescent lower-level entities, the rcu_node structure level,
 * the starting and ending CPU covered by the rcu_node structure, and
 * whether there are any blocked tasks blocking the current grace period.
 * All but the type of RCU are extracted from the rcu_node structure.
 */
<<<<<<< HEAD
TRACE_EVENT(rcu_quiescent_state_report,

	TP_PROTO(char *rcuname, unsigned long gpnum,
		 unsigned long mask, unsigned long qsmask,
		 u8 level, int grplo, int grphi, int gp_tasks),

	TP_ARGS(rcuname, gpnum, mask, qsmask, level, grplo, grphi, gp_tasks),

	TP_STRUCT__entry(
		__field(char *, rcuname)
		__field(unsigned long, gpnum)
=======
TRACE_EVENT_RCU(rcu_quiescent_state_report,

	TP_PROTO(const char *rcuname, unsigned long gp_seq,
		 unsigned long mask, unsigned long qsmask,
		 u8 level, int grplo, int grphi, int gp_tasks),

	TP_ARGS(rcuname, gp_seq, mask, qsmask, level, grplo, grphi, gp_tasks),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(long, gp_seq)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__field(unsigned long, mask)
		__field(unsigned long, qsmask)
		__field(u8, level)
		__field(int, grplo)
		__field(int, grphi)
		__field(u8, gp_tasks)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
<<<<<<< HEAD
		__entry->gpnum = gpnum;
=======
		__entry->gp_seq = (long)gp_seq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__entry->mask = mask;
		__entry->qsmask = qsmask;
		__entry->level = level;
		__entry->grplo = grplo;
		__entry->grphi = grphi;
		__entry->gp_tasks = gp_tasks;
	),

<<<<<<< HEAD
	TP_printk("%s %lu %lx>%lx %u %d %d %u",
		  __entry->rcuname, __entry->gpnum,
=======
	TP_printk("%s %ld %lx>%lx %u %d %d %u",
		  __entry->rcuname, __entry->gp_seq,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		  __entry->mask, __entry->qsmask, __entry->level,
		  __entry->grplo, __entry->grphi, __entry->gp_tasks)
);

/*
 * Tracepoint for quiescent states detected by force_quiescent_state().
 * These trace events include the type of RCU, the grace-period number
 * that was blocked by the CPU, the CPU itself, and the type of quiescent
<<<<<<< HEAD
 * state, which can be "dti" for dyntick-idle mode, "ofl" for CPU offline,
 * or "kick" when kicking a CPU that has been in dyntick-idle mode for
 * too long.
 */
TRACE_EVENT(rcu_fqs,

	TP_PROTO(char *rcuname, unsigned long gpnum, int cpu, char *qsevent),

	TP_ARGS(rcuname, gpnum, cpu, qsevent),

	TP_STRUCT__entry(
		__field(char *, rcuname)
		__field(unsigned long, gpnum)
		__field(int, cpu)
		__field(char *, qsevent)
=======
 * state, which can be "dti" for dyntick-idle mode or "kick" when kicking
 * a CPU that has been in dyntick-idle mode for too long.
 */
TRACE_EVENT_RCU(rcu_fqs,

	TP_PROTO(const char *rcuname, unsigned long gp_seq, int cpu, const char *qsevent),

	TP_ARGS(rcuname, gp_seq, cpu, qsevent),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(long, gp_seq)
		__field(int, cpu)
		__field(const char *, qsevent)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
<<<<<<< HEAD
		__entry->gpnum = gpnum;
=======
		__entry->gp_seq = (long)gp_seq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__entry->cpu = cpu;
		__entry->qsevent = qsevent;
	),

<<<<<<< HEAD
	TP_printk("%s %lu %d %s",
		  __entry->rcuname, __entry->gpnum,
		  __entry->cpu, __entry->qsevent)
);

#endif /* #if defined(CONFIG_TREE_RCU) || defined(CONFIG_TREE_PREEMPT_RCU) */

/*
 * Tracepoint for dyntick-idle entry/exit events.  These take a string
 * as argument: "Start" for entering dyntick-idle mode, "End" for
 * leaving it, "--=" for events moving towards idle, and "++=" for events
 * moving away from idle.  "Error on entry: not idle task" and "Error on
 * exit: not idle task" indicate that a non-idle task is erroneously
 * toying with the idle loop.
 *
 * These events also take a pair of numbers, which indicate the nesting
 * depth before and after the event of interest.  Note that task-related
 * events use the upper bits of each number, while interrupt-related
 * events use the lower bits.
 */
TRACE_EVENT(rcu_dyntick,

	TP_PROTO(char *polarity, long long oldnesting, long long newnesting),

	TP_ARGS(polarity, oldnesting, newnesting),

	TP_STRUCT__entry(
		__field(char *, polarity)
		__field(long long, oldnesting)
		__field(long long, newnesting)
=======
	TP_printk("%s %ld %d %s",
		  __entry->rcuname, __entry->gp_seq,
		  __entry->cpu, __entry->qsevent)
);

/*
 * Tracepoint for RCU stall events. Takes a string identifying the RCU flavor
 * and a string identifying which function detected the RCU stall as follows:
 *
 *	"StallDetected": Scheduler-tick detects other CPU's stalls.
 *	"SelfDetected": Scheduler-tick detects a current CPU's stall.
 *	"ExpeditedStall": Expedited grace period detects stalls.
 */
TRACE_EVENT(rcu_stall_warning,

	TP_PROTO(const char *rcuname, const char *msg),

	TP_ARGS(rcuname, msg),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(const char *, msg)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->msg = msg;
	),

	TP_printk("%s %s",
		  __entry->rcuname, __entry->msg)
);

#endif /* #if defined(CONFIG_TREE_RCU) */

/*
 * Tracepoint for dyntick-idle entry/exit events.  These take 2 strings
 * as argument:
 * polarity: "Start", "End", "StillNonIdle" for entering, exiting or still not
 *            being in dyntick-idle mode.
 * context: "USER" or "IDLE" or "IRQ".
 * NMIs nested in IRQs are inferred with dynticks_nesting > 1 in IRQ context.
 *
 * These events also take a pair of numbers, which indicate the nesting
 * depth before and after the event of interest, and a third number that is
 * the ->dynticks counter.  Note that task-related and interrupt-related
 * events use two separate counters, and that the "++=" and "--=" events
 * for irq/NMI will change the counter by two, otherwise by one.
 */
TRACE_EVENT_RCU(rcu_dyntick,

	TP_PROTO(const char *polarity, long oldnesting, long newnesting, int dynticks),

	TP_ARGS(polarity, oldnesting, newnesting, dynticks),

	TP_STRUCT__entry(
		__field(const char *, polarity)
		__field(long, oldnesting)
		__field(long, newnesting)
		__field(int, dynticks)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	),

	TP_fast_assign(
		__entry->polarity = polarity;
		__entry->oldnesting = oldnesting;
		__entry->newnesting = newnesting;
<<<<<<< HEAD
	),

	TP_printk("%s %llx %llx", __entry->polarity,
		  __entry->oldnesting, __entry->newnesting)
);

/*
 * Tracepoint for RCU preparation for idle, the goal being to get RCU
 * processing done so that the current CPU can shut off its scheduling
 * clock and enter dyntick-idle mode.  One way to accomplish this is
 * to drain all RCU callbacks from this CPU, and the other is to have
 * done everything RCU requires for the current grace period.  In this
 * latter case, the CPU will be awakened at the end of the current grace
 * period in order to process the remainder of its callbacks.
 *
 * These tracepoints take a string as argument:
 *
 *	"No callbacks": Nothing to do, no callbacks on this CPU.
 *	"In holdoff": Nothing to do, holding off after unsuccessful attempt.
 *	"Begin holdoff": Attempt failed, don't retry until next jiffy.
 *	"Dyntick with callbacks": Entering dyntick-idle despite callbacks.
 *	"Dyntick with lazy callbacks": Entering dyntick-idle w/lazy callbacks.
 *	"More callbacks": Still more callbacks, try again to clear them out.
 *	"Callbacks drained": All callbacks processed, off to dyntick idle!
 *	"Timer": Timer fired to cause CPU to continue processing callbacks.
 *	"Demigrate": Timer fired on wrong CPU, woke up correct CPU.
 *	"Cleanup after idle": Idle exited, timer canceled.
 */
TRACE_EVENT(rcu_prep_idle,

	TP_PROTO(char *reason),

	TP_ARGS(reason),

	TP_STRUCT__entry(
		__field(char *, reason)
	),

	TP_fast_assign(
		__entry->reason = reason;
	),

	TP_printk("%s", __entry->reason)
=======
		__entry->dynticks = dynticks;
	),

	TP_printk("%s %lx %lx %#3x", __entry->polarity,
		  __entry->oldnesting, __entry->newnesting,
		  __entry->dynticks & 0xfff)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
);

/*
 * Tracepoint for the registration of a single RCU callback function.
 * The first argument is the type of RCU, the second argument is
 * a pointer to the RCU callback itself, the third element is the
 * number of lazy callbacks queued, and the fourth element is the
 * total number of callbacks queued.
 */
<<<<<<< HEAD
TRACE_EVENT(rcu_callback,

	TP_PROTO(char *rcuname, struct rcu_head *rhp, long qlen_lazy,
		 long qlen),

	TP_ARGS(rcuname, rhp, qlen_lazy, qlen),

	TP_STRUCT__entry(
		__field(char *, rcuname)
		__field(void *, rhp)
		__field(void *, func)
		__field(long, qlen_lazy)
=======
TRACE_EVENT_RCU(rcu_callback,

	TP_PROTO(const char *rcuname, struct rcu_head *rhp, long qlen),

	TP_ARGS(rcuname, rhp, qlen),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(void *, rhp)
		__field(void *, func)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__field(long, qlen)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->rhp = rhp;
		__entry->func = rhp->func;
<<<<<<< HEAD
		__entry->qlen_lazy = qlen_lazy;
		__entry->qlen = qlen;
	),

	TP_printk("%s rhp=%p func=%pf %ld/%ld",
		  __entry->rcuname, __entry->rhp, __entry->func,
		  __entry->qlen_lazy, __entry->qlen)
=======
		__entry->qlen = qlen;
	),

	TP_printk("%s rhp=%p func=%ps %ld",
		  __entry->rcuname, __entry->rhp, __entry->func,
		  __entry->qlen)
);

TRACE_EVENT_RCU(rcu_segcb_stats,

		TP_PROTO(struct rcu_segcblist *rs, const char *ctx),

		TP_ARGS(rs, ctx),

		TP_STRUCT__entry(
			__field(const char *, ctx)
			__array(unsigned long, gp_seq, RCU_CBLIST_NSEGS)
			__array(long, seglen, RCU_CBLIST_NSEGS)
		),

		TP_fast_assign(
			__entry->ctx = ctx;
			memcpy(__entry->seglen, rs->seglen, RCU_CBLIST_NSEGS * sizeof(long));
			memcpy(__entry->gp_seq, rs->gp_seq, RCU_CBLIST_NSEGS * sizeof(unsigned long));

		),

		TP_printk("%s seglen: (DONE=%ld, WAIT=%ld, NEXT_READY=%ld, NEXT=%ld) "
			  "gp_seq: (DONE=%lu, WAIT=%lu, NEXT_READY=%lu, NEXT=%lu)", __entry->ctx,
			  __entry->seglen[0], __entry->seglen[1], __entry->seglen[2], __entry->seglen[3],
			  __entry->gp_seq[0], __entry->gp_seq[1], __entry->gp_seq[2], __entry->gp_seq[3])

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
);

/*
 * Tracepoint for the registration of a single RCU callback of the special
<<<<<<< HEAD
 * kfree() form.  The first argument is the RCU type, the second argument
=======
 * kvfree() form.  The first argument is the RCU type, the second argument
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * is a pointer to the RCU callback, the third argument is the offset
 * of the callback within the enclosing RCU-protected data structure,
 * the fourth argument is the number of lazy callbacks queued, and the
 * fifth argument is the total number of callbacks queued.
 */
<<<<<<< HEAD
TRACE_EVENT(rcu_kfree_callback,

	TP_PROTO(char *rcuname, struct rcu_head *rhp, unsigned long offset,
		 long qlen_lazy, long qlen),

	TP_ARGS(rcuname, rhp, offset, qlen_lazy, qlen),

	TP_STRUCT__entry(
		__field(char *, rcuname)
		__field(void *, rhp)
		__field(unsigned long, offset)
		__field(long, qlen_lazy)
=======
TRACE_EVENT_RCU(rcu_kvfree_callback,

	TP_PROTO(const char *rcuname, struct rcu_head *rhp, unsigned long offset,
		 long qlen),

	TP_ARGS(rcuname, rhp, offset, qlen),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(void *, rhp)
		__field(unsigned long, offset)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__field(long, qlen)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->rhp = rhp;
		__entry->offset = offset;
<<<<<<< HEAD
		__entry->qlen_lazy = qlen_lazy;
		__entry->qlen = qlen;
	),

	TP_printk("%s rhp=%p func=%ld %ld/%ld",
		  __entry->rcuname, __entry->rhp, __entry->offset,
		  __entry->qlen_lazy, __entry->qlen)
=======
		__entry->qlen = qlen;
	),

	TP_printk("%s rhp=%p func=%ld %ld",
		  __entry->rcuname, __entry->rhp, __entry->offset,
		  __entry->qlen)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
);

/*
 * Tracepoint for marking the beginning rcu_do_batch, performed to start
 * RCU callback invocation.  The first argument is the RCU flavor,
 * the second is the number of lazy callbacks queued, the third is
 * the total number of callbacks queued, and the fourth argument is
 * the current RCU-callback batch limit.
 */
<<<<<<< HEAD
TRACE_EVENT(rcu_batch_start,

	TP_PROTO(char *rcuname, long qlen_lazy, long qlen, int blimit),

	TP_ARGS(rcuname, qlen_lazy, qlen, blimit),

	TP_STRUCT__entry(
		__field(char *, rcuname)
		__field(long, qlen_lazy)
		__field(long, qlen)
		__field(int, blimit)
=======
TRACE_EVENT_RCU(rcu_batch_start,

	TP_PROTO(const char *rcuname, long qlen, long blimit),

	TP_ARGS(rcuname, qlen, blimit),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(long, qlen)
		__field(long, blimit)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
<<<<<<< HEAD
		__entry->qlen_lazy = qlen_lazy;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__entry->qlen = qlen;
		__entry->blimit = blimit;
	),

<<<<<<< HEAD
	TP_printk("%s CBs=%ld/%ld bl=%d",
		  __entry->rcuname, __entry->qlen_lazy, __entry->qlen,
		  __entry->blimit)
=======
	TP_printk("%s CBs=%ld bl=%ld",
		  __entry->rcuname, __entry->qlen, __entry->blimit)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
);

/*
 * Tracepoint for the invocation of a single RCU callback function.
 * The first argument is the type of RCU, and the second argument is
 * a pointer to the RCU callback itself.
 */
<<<<<<< HEAD
TRACE_EVENT(rcu_invoke_callback,

	TP_PROTO(char *rcuname, struct rcu_head *rhp),
=======
TRACE_EVENT_RCU(rcu_invoke_callback,

	TP_PROTO(const char *rcuname, struct rcu_head *rhp),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	TP_ARGS(rcuname, rhp),

	TP_STRUCT__entry(
<<<<<<< HEAD
		__field(char *, rcuname)
=======
		__field(const char *, rcuname)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__field(void *, rhp)
		__field(void *, func)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->rhp = rhp;
		__entry->func = rhp->func;
	),

<<<<<<< HEAD
	TP_printk("%s rhp=%p func=%pf",
=======
	TP_printk("%s rhp=%p func=%ps",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		  __entry->rcuname, __entry->rhp, __entry->func)
);

/*
 * Tracepoint for the invocation of a single RCU callback of the special
<<<<<<< HEAD
 * kfree() form.  The first argument is the RCU flavor, the second
=======
 * kvfree() form.  The first argument is the RCU flavor, the second
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * argument is a pointer to the RCU callback, and the third argument
 * is the offset of the callback within the enclosing RCU-protected
 * data structure.
 */
<<<<<<< HEAD
TRACE_EVENT(rcu_invoke_kfree_callback,

	TP_PROTO(char *rcuname, struct rcu_head *rhp, unsigned long offset),
=======
TRACE_EVENT_RCU(rcu_invoke_kvfree_callback,

	TP_PROTO(const char *rcuname, struct rcu_head *rhp, unsigned long offset),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	TP_ARGS(rcuname, rhp, offset),

	TP_STRUCT__entry(
<<<<<<< HEAD
		__field(char *, rcuname)
=======
		__field(const char *, rcuname)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__field(void *, rhp)
		__field(unsigned long, offset)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->rhp = rhp;
		__entry->offset	= offset;
	),

	TP_printk("%s rhp=%p func=%ld",
		  __entry->rcuname, __entry->rhp, __entry->offset)
);

/*
<<<<<<< HEAD
=======
 * Tracepoint for the invocation of a single RCU callback of the special
 * kfree_bulk() form. The first argument is the RCU flavor, the second
 * argument is a number of elements in array to free, the third is an
 * address of the array holding nr_records entries.
 */
TRACE_EVENT_RCU(rcu_invoke_kfree_bulk_callback,

	TP_PROTO(const char *rcuname, unsigned long nr_records, void **p),

	TP_ARGS(rcuname, nr_records, p),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(unsigned long, nr_records)
		__field(void **, p)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->nr_records = nr_records;
		__entry->p = p;
	),

	TP_printk("%s bulk=0x%p nr_records=%lu",
		__entry->rcuname, __entry->p, __entry->nr_records)
);

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Tracepoint for exiting rcu_do_batch after RCU callbacks have been
 * invoked.  The first argument is the name of the RCU flavor,
 * the second argument is number of callbacks actually invoked,
 * the third argument (cb) is whether or not any of the callbacks that
 * were ready to invoke at the beginning of this batch are still
 * queued, the fourth argument (nr) is the return value of need_resched(),
 * the fifth argument (iit) is 1 if the current task is the idle task,
 * and the sixth argument (risk) is the return value from
 * rcu_is_callbacks_kthread().
 */
<<<<<<< HEAD
TRACE_EVENT(rcu_batch_end,

	TP_PROTO(char *rcuname, int callbacks_invoked,
		 bool cb, bool nr, bool iit, bool risk),
=======
TRACE_EVENT_RCU(rcu_batch_end,

	TP_PROTO(const char *rcuname, int callbacks_invoked,
		 char cb, char nr, char iit, char risk),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	TP_ARGS(rcuname, callbacks_invoked, cb, nr, iit, risk),

	TP_STRUCT__entry(
<<<<<<< HEAD
		__field(char *, rcuname)
		__field(int, callbacks_invoked)
		__field(bool, cb)
		__field(bool, nr)
		__field(bool, iit)
		__field(bool, risk)
=======
		__field(const char *, rcuname)
		__field(int, callbacks_invoked)
		__field(char, cb)
		__field(char, nr)
		__field(char, iit)
		__field(char, risk)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->callbacks_invoked = callbacks_invoked;
		__entry->cb = cb;
		__entry->nr = nr;
		__entry->iit = iit;
		__entry->risk = risk;
	),

	TP_printk("%s CBs-invoked=%d idle=%c%c%c%c",
		  __entry->rcuname, __entry->callbacks_invoked,
		  __entry->cb ? 'C' : '.',
		  __entry->nr ? 'S' : '.',
		  __entry->iit ? 'I' : '.',
		  __entry->risk ? 'R' : '.')
);

/*
 * Tracepoint for rcutorture readers.  The first argument is the name
 * of the RCU flavor from rcutorture's viewpoint and the second argument
<<<<<<< HEAD
 * is the callback address.
 */
TRACE_EVENT(rcu_torture_read,

	TP_PROTO(char *rcutorturename, struct rcu_head *rhp),

	TP_ARGS(rcutorturename, rhp),

	TP_STRUCT__entry(
		__field(char *, rcutorturename)
		__field(struct rcu_head *, rhp)
	),

	TP_fast_assign(
		__entry->rcutorturename = rcutorturename;
		__entry->rhp = rhp;
	),

	TP_printk("%s torture read %p",
		  __entry->rcutorturename, __entry->rhp)
);

#else /* #ifdef CONFIG_RCU_TRACE */

#define trace_rcu_grace_period(rcuname, gpnum, gpevent) do { } while (0)
#define trace_rcu_grace_period_init(rcuname, gpnum, level, grplo, grphi, \
				    qsmask) do { } while (0)
#define trace_rcu_preempt_task(rcuname, pid, gpnum) do { } while (0)
#define trace_rcu_unlock_preempted_task(rcuname, gpnum, pid) do { } while (0)
#define trace_rcu_quiescent_state_report(rcuname, gpnum, mask, qsmask, level, \
					 grplo, grphi, gp_tasks) do { } \
	while (0)
#define trace_rcu_fqs(rcuname, gpnum, cpu, qsevent) do { } while (0)
#define trace_rcu_dyntick(polarity, oldnesting, newnesting) do { } while (0)
#define trace_rcu_prep_idle(reason) do { } while (0)
#define trace_rcu_callback(rcuname, rhp, qlen_lazy, qlen) do { } while (0)
#define trace_rcu_kfree_callback(rcuname, rhp, offset, qlen_lazy, qlen) \
	do { } while (0)
#define trace_rcu_batch_start(rcuname, qlen_lazy, qlen, blimit) \
	do { } while (0)
#define trace_rcu_invoke_callback(rcuname, rhp) do { } while (0)
#define trace_rcu_invoke_kfree_callback(rcuname, rhp, offset) do { } while (0)
#define trace_rcu_batch_end(rcuname, callbacks_invoked, cb, nr, iit, risk) \
	do { } while (0)
#define trace_rcu_torture_read(rcutorturename, rhp) do { } while (0)

#endif /* #else #ifdef CONFIG_RCU_TRACE */
=======
 * is the callback address.  The third argument is the start time in
 * seconds, and the last two arguments are the grace period numbers
 * at the beginning and end of the read, respectively.  Note that the
 * callback address can be NULL.
 */
#define RCUTORTURENAME_LEN 8
TRACE_EVENT_RCU(rcu_torture_read,

	TP_PROTO(const char *rcutorturename, struct rcu_head *rhp,
		 unsigned long secs, unsigned long c_old, unsigned long c),

	TP_ARGS(rcutorturename, rhp, secs, c_old, c),

	TP_STRUCT__entry(
		__array(char, rcutorturename, RCUTORTURENAME_LEN)
		__field(struct rcu_head *, rhp)
		__field(unsigned long, secs)
		__field(unsigned long, c_old)
		__field(unsigned long, c)
	),

	TP_fast_assign(
		strscpy(__entry->rcutorturename, rcutorturename, RCUTORTURENAME_LEN);
		__entry->rhp = rhp;
		__entry->secs = secs;
		__entry->c_old = c_old;
		__entry->c = c;
	),

	TP_printk("%s torture read %p %luus c: %lu %lu",
		  __entry->rcutorturename, __entry->rhp,
		  __entry->secs, __entry->c_old, __entry->c)
);

/*
 * Tracepoint for rcu_barrier() execution.  The string "s" describes
 * the rcu_barrier phase:
 *	"Begin": rcu_barrier() started.
 *	"CB": An rcu_barrier_callback() invoked a callback, not the last.
 *	"EarlyExit": rcu_barrier() piggybacked, thus early exit.
 *	"Inc1": rcu_barrier() piggyback check counter incremented.
 *	"Inc2": rcu_barrier() piggyback check counter incremented.
 *	"IRQ": An rcu_barrier_callback() callback posted on remote CPU.
 *	"IRQNQ": An rcu_barrier_callback() callback found no callbacks.
 *	"LastCB": An rcu_barrier_callback() invoked the last callback.
 *	"NQ": rcu_barrier() found a CPU with no callbacks.
 *	"OnlineQ": rcu_barrier() found online CPU with callbacks.
 * The "cpu" argument is the CPU or -1 if meaningless, the "cnt" argument
 * is the count of remaining callbacks, and "done" is the piggybacking count.
 */
TRACE_EVENT_RCU(rcu_barrier,

	TP_PROTO(const char *rcuname, const char *s, int cpu, int cnt, unsigned long done),

	TP_ARGS(rcuname, s, cpu, cnt, done),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(const char *, s)
		__field(int, cpu)
		__field(int, cnt)
		__field(unsigned long, done)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->s = s;
		__entry->cpu = cpu;
		__entry->cnt = cnt;
		__entry->done = done;
	),

	TP_printk("%s %s cpu %d remaining %d # %lu",
		  __entry->rcuname, __entry->s, __entry->cpu, __entry->cnt,
		  __entry->done)
);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _TRACE_RCU_H */

/* This part must be outside protection */
#include <trace/define_trace.h>
