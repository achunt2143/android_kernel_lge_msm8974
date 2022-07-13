<<<<<<< HEAD
#ifndef ISCSI_TARGET_DEVICE_H
#define ISCSI_TARGET_DEVICE_H

extern void iscsit_determine_maxcmdsn(struct iscsi_session *);
extern void iscsit_increment_maxcmdsn(struct iscsi_cmd *, struct iscsi_session *);
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef ISCSI_TARGET_DEVICE_H
#define ISCSI_TARGET_DEVICE_H

struct iscsit_cmd;
struct iscsit_session;

extern void iscsit_determine_maxcmdsn(struct iscsit_session *);
extern void iscsit_increment_maxcmdsn(struct iscsit_cmd *, struct iscsit_session *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* ISCSI_TARGET_DEVICE_H */
