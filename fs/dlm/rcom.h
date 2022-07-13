<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/******************************************************************************
*******************************************************************************
**
**  Copyright (C) Sistina Software, Inc.  1997-2003  All rights reserved.
**  Copyright (C) 2005-2007 Red Hat, Inc.  All rights reserved.
**
<<<<<<< HEAD
**  This copyrighted material is made available to anyone wishing to use,
**  modify, copy, or redistribute it subject to the terms and conditions
**  of the GNU General Public License v.2.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
**
*******************************************************************************
******************************************************************************/

#ifndef __RCOM_DOT_H__
#define __RCOM_DOT_H__

<<<<<<< HEAD
int dlm_rcom_status(struct dlm_ls *ls, int nodeid, uint32_t status_flags);
int dlm_rcom_names(struct dlm_ls *ls, int nodeid, char *last_name,int last_len);
int dlm_send_rcom_lookup(struct dlm_rsb *r, int dir_nodeid);
int dlm_send_rcom_lock(struct dlm_rsb *r, struct dlm_lkb *lkb);
void dlm_receive_rcom(struct dlm_ls *ls, struct dlm_rcom *rc, int nodeid);
int dlm_send_ls_not_ready(int nodeid, struct dlm_rcom *rc_in);
=======
int dlm_rcom_status(struct dlm_ls *ls, int nodeid, uint32_t status_flags,
		    uint64_t seq);
int dlm_rcom_names(struct dlm_ls *ls, int nodeid, char *last_name,
		   int last_len, uint64_t seq);
int dlm_send_rcom_lookup(struct dlm_rsb *r, int dir_nodeid, uint64_t seq);
int dlm_send_rcom_lock(struct dlm_rsb *r, struct dlm_lkb *lkb, uint64_t seq);
void dlm_receive_rcom(struct dlm_ls *ls, const struct dlm_rcom *rc,
		      int nodeid);
int dlm_send_ls_not_ready(int nodeid, const struct dlm_rcom *rc_in);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif

