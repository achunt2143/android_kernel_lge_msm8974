<<<<<<< HEAD
#ifndef ISCSI_TARGET_ERL0_H
#define ISCSI_TARGET_ERL0_H

extern void iscsit_set_dataout_sequence_values(struct iscsi_cmd *);
extern int iscsit_check_pre_dataout(struct iscsi_cmd *, unsigned char *);
extern int iscsit_check_post_dataout(struct iscsi_cmd *, unsigned char *, u8);
extern void iscsit_start_time2retain_handler(struct iscsi_session *);
extern int iscsit_stop_time2retain_timer(struct iscsi_session *);
extern void iscsit_connection_reinstatement_rcfr(struct iscsi_conn *);
extern void iscsit_cause_connection_reinstatement(struct iscsi_conn *, int);
extern void iscsit_fall_back_to_erl0(struct iscsi_session *);
extern void iscsit_take_action_for_connection_exit(struct iscsi_conn *);
extern int iscsit_recover_from_unknown_opcode(struct iscsi_conn *);
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef ISCSI_TARGET_ERL0_H
#define ISCSI_TARGET_ERL0_H

#include <linux/types.h>

struct iscsit_cmd;
struct iscsit_conn;
struct iscsit_session;

extern void iscsit_set_dataout_sequence_values(struct iscsit_cmd *);
extern int iscsit_check_pre_dataout(struct iscsit_cmd *, unsigned char *);
extern int iscsit_check_post_dataout(struct iscsit_cmd *, unsigned char *, u8);
extern void iscsit_start_time2retain_handler(struct iscsit_session *);
extern void iscsit_handle_time2retain_timeout(struct timer_list *t);
extern int iscsit_stop_time2retain_timer(struct iscsit_session *);
extern void iscsit_connection_reinstatement_rcfr(struct iscsit_conn *);
extern void iscsit_cause_connection_reinstatement(struct iscsit_conn *, int);
extern void iscsit_fall_back_to_erl0(struct iscsit_session *);
extern void iscsit_take_action_for_connection_exit(struct iscsit_conn *, bool *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif   /*** ISCSI_TARGET_ERL0_H ***/
