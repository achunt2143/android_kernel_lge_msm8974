<<<<<<< HEAD
/*
 * nfsd-specific authentication stuff.
 * uid/gid mapping not yet implemented.
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * nfsd-specific authentication stuff.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Copyright (C) 1995, 1996 Olaf Kirch <okir@monad.swb.de>
 */

#ifndef LINUX_NFSD_AUTH_H
#define LINUX_NFSD_AUTH_H

<<<<<<< HEAD
#define nfsd_luid(rq, uid)	((u32)(uid))
#define nfsd_lgid(rq, gid)	((u32)(gid))
#define nfsd_ruid(rq, uid)	((u32)(uid))
#define nfsd_rgid(rq, gid)	((u32)(gid))

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Set the current process's fsuid/fsgid etc to those of the NFS
 * client user
 */
int nfsd_setuser(struct svc_rqst *, struct svc_export *);

#endif /* LINUX_NFSD_AUTH_H */
