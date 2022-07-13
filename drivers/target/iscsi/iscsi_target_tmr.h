<<<<<<< HEAD
#ifndef ISCSI_TARGET_TMR_H
#define ISCSI_TARGET_TMR_H

extern u8 iscsit_tmr_abort_task(struct iscsi_cmd *, unsigned char *);
extern int iscsit_tmr_task_warm_reset(struct iscsi_conn *, struct iscsi_tmr_req *,
			unsigned char *);
extern int iscsit_tmr_task_cold_reset(struct iscsi_conn *, struct iscsi_tmr_req *,
			unsigned char *);
extern u8 iscsit_tmr_task_reassign(struct iscsi_cmd *, unsigned char *);
extern int iscsit_tmr_post_handler(struct iscsi_cmd *, struct iscsi_conn *);
extern int iscsit_check_task_reassign_expdatasn(struct iscsi_tmr_req *,
			struct iscsi_conn *);
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef ISCSI_TARGET_TMR_H
#define ISCSI_TARGET_TMR_H

#include <linux/types.h>

struct iscsit_cmd;
struct iscsit_conn;
struct iscsi_tmr_req;

extern u8 iscsit_tmr_abort_task(struct iscsit_cmd *, unsigned char *);
extern int iscsit_tmr_task_warm_reset(struct iscsit_conn *, struct iscsi_tmr_req *,
			unsigned char *);
extern int iscsit_tmr_task_cold_reset(struct iscsit_conn *, struct iscsi_tmr_req *,
			unsigned char *);
extern u8 iscsit_tmr_task_reassign(struct iscsit_cmd *, unsigned char *);
extern int iscsit_tmr_post_handler(struct iscsit_cmd *, struct iscsit_conn *);
extern int iscsit_check_task_reassign_expdatasn(struct iscsi_tmr_req *,
			struct iscsit_conn *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* ISCSI_TARGET_TMR_H */
