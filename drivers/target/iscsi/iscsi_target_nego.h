<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef ISCSI_TARGET_NEGO_H
#define ISCSI_TARGET_NEGO_H

#define DECIMAL         0
#define HEX             1
<<<<<<< HEAD
=======
#define BASE64          2

struct iscsit_conn;
struct iscsi_login;
struct iscsi_np;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern void convert_null_to_semi(char *, int);
extern int extract_param(const char *, const char *, unsigned int, char *,
		unsigned char *);
<<<<<<< HEAD
extern struct iscsi_login *iscsi_target_init_negotiation(
		struct iscsi_np *, struct iscsi_conn *, char *);
extern int iscsi_target_start_negotiation(
		struct iscsi_login *, struct iscsi_conn *);
extern void iscsi_target_nego_release(
		struct iscsi_login *, struct iscsi_conn *);

=======
extern int iscsi_target_check_login_request(struct iscsit_conn *,
		struct iscsi_login *);
extern int iscsi_target_get_initial_payload(struct iscsit_conn *,
		struct iscsi_login *);
extern int iscsi_target_locate_portal(struct iscsi_np *, struct iscsit_conn *,
		struct iscsi_login *);
extern int iscsi_target_start_negotiation(
		struct iscsi_login *, struct iscsit_conn *);
extern void iscsi_target_nego_release(struct iscsit_conn *);
extern bool iscsi_conn_auth_required(struct iscsit_conn *conn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* ISCSI_TARGET_NEGO_H */
