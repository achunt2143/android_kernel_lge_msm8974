<<<<<<< HEAD
/*
 * Access vector cache interface for the security server.
 *
 * Author : Stephen Smalley, <sds@epoch.ncsc.mil>
 */
#ifndef _SELINUX_AVC_SS_H_
#define _SELINUX_AVC_SS_H_

#include "flask.h"
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Access vector cache interface for the security server.
 *
 * Author : Stephen Smalley, <stephen.smalley.work@gmail.com>
 */

#ifndef _SELINUX_AVC_SS_H_
#define _SELINUX_AVC_SS_H_

#include <linux/types.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int avc_ss_reset(u32 seqno);

/* Class/perm mapping support */
struct security_class_mapping {
	const char *name;
	const char *perms[sizeof(u32) * 8 + 1];
};

<<<<<<< HEAD
extern struct security_class_mapping secclass_map[];

/*
 * The security server must be initialized before
 * any labeling or access decisions can be provided.
 */
extern int ss_initialized;

#endif /* _SELINUX_AVC_SS_H_ */

=======
extern const struct security_class_mapping secclass_map[];

#endif /* _SELINUX_AVC_SS_H_ */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
