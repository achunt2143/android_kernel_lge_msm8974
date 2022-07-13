<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for SCTP support code.
#

obj-$(CONFIG_IP_SCTP) += sctp.o
<<<<<<< HEAD
obj-$(CONFIG_NET_SCTPPROBE) += sctp_probe.o
=======
obj-$(CONFIG_INET_SCTP_DIAG) += sctp_diag.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

sctp-y := sm_statetable.o sm_statefuns.o sm_sideeffect.o \
	  protocol.o endpointola.o associola.o \
	  transport.o chunk.o sm_make_chunk.o ulpevent.o \
<<<<<<< HEAD
	  inqueue.o outqueue.o ulpqueue.o command.o \
	  tsnmap.o bind_addr.o socket.o primitive.o \
	  output.o input.o debug.o ssnmap.o auth.o

sctp_probe-y := probe.o
=======
	  inqueue.o outqueue.o ulpqueue.o \
	  tsnmap.o bind_addr.o socket.o primitive.o \
	  output.o input.o debug.o stream.o auth.o \
	  offload.o stream_sched.o stream_sched_prio.o \
	  stream_sched_rr.o stream_sched_fc.o \
	  stream_interleave.o

sctp_diag-y := diag.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

sctp-$(CONFIG_SCTP_DBG_OBJCNT) += objcnt.o
sctp-$(CONFIG_PROC_FS) += proc.o
sctp-$(CONFIG_SYSCTL) += sysctl.o

sctp-$(subst m,y,$(CONFIG_IPV6))	+= ipv6.o
