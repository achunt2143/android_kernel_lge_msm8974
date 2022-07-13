<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _NFSD4_CURRENT_STATE_H
#define _NFSD4_CURRENT_STATE_H

#include "state.h"
#include "xdr4.h"

extern void clear_current_stateid(struct nfsd4_compound_state *cstate);
/*
 * functions to set current state id
 */
<<<<<<< HEAD
extern void nfsd4_set_opendowngradestateid(struct nfsd4_compound_state *cstate, struct nfsd4_open_downgrade *);
extern void nfsd4_set_openstateid(struct nfsd4_compound_state *, struct nfsd4_open *);
extern void nfsd4_set_lockstateid(struct nfsd4_compound_state *, struct nfsd4_lock *);
extern void nfsd4_set_closestateid(struct nfsd4_compound_state *, struct nfsd4_close *);
=======
extern void nfsd4_set_opendowngradestateid(struct nfsd4_compound_state *,
		union nfsd4_op_u *);
extern void nfsd4_set_openstateid(struct nfsd4_compound_state *,
		union nfsd4_op_u *);
extern void nfsd4_set_lockstateid(struct nfsd4_compound_state *,
		union nfsd4_op_u *);
extern void nfsd4_set_closestateid(struct nfsd4_compound_state *,
		union nfsd4_op_u *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * functions to consume current state id
 */
<<<<<<< HEAD
extern void nfsd4_get_opendowngradestateid(struct nfsd4_compound_state *cstate, struct nfsd4_open_downgrade *);
extern void nfsd4_get_delegreturnstateid(struct nfsd4_compound_state *, struct nfsd4_delegreturn *);
extern void nfsd4_get_freestateid(struct nfsd4_compound_state *, struct nfsd4_free_stateid *);
extern void nfsd4_get_setattrstateid(struct nfsd4_compound_state *, struct nfsd4_setattr *);
extern void nfsd4_get_closestateid(struct nfsd4_compound_state *, struct nfsd4_close *);
extern void nfsd4_get_lockustateid(struct nfsd4_compound_state *, struct nfsd4_locku *);
extern void nfsd4_get_readstateid(struct nfsd4_compound_state *, struct nfsd4_read *);
extern void nfsd4_get_writestateid(struct nfsd4_compound_state *, struct nfsd4_write *);
=======
extern void nfsd4_get_opendowngradestateid(struct nfsd4_compound_state *,
		union nfsd4_op_u *);
extern void nfsd4_get_delegreturnstateid(struct nfsd4_compound_state *,
		union nfsd4_op_u *);
extern void nfsd4_get_freestateid(struct nfsd4_compound_state *,
		union nfsd4_op_u *);
extern void nfsd4_get_setattrstateid(struct nfsd4_compound_state *,
		union nfsd4_op_u *);
extern void nfsd4_get_closestateid(struct nfsd4_compound_state *,
		union nfsd4_op_u *);
extern void nfsd4_get_lockustateid(struct nfsd4_compound_state *,
		union nfsd4_op_u *);
extern void nfsd4_get_readstateid(struct nfsd4_compound_state *,
		union nfsd4_op_u *);
extern void nfsd4_get_writestateid(struct nfsd4_compound_state *,
		union nfsd4_op_u *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif   /* _NFSD4_CURRENT_STATE_H */
