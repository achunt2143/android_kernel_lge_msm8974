<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/******************************************************************************
*******************************************************************************
**
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

#ifndef __REQUESTQUEUE_DOT_H__
#define __REQUESTQUEUE_DOT_H__

<<<<<<< HEAD
void dlm_add_requestqueue(struct dlm_ls *ls, int nodeid, struct dlm_message *ms);
=======
void dlm_add_requestqueue(struct dlm_ls *ls, int nodeid,
			  const struct dlm_message *ms);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int dlm_process_requestqueue(struct dlm_ls *ls);
void dlm_wait_requestqueue(struct dlm_ls *ls);
void dlm_purge_requestqueue(struct dlm_ls *ls);

#endif

