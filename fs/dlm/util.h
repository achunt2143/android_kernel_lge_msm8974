<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/******************************************************************************
*******************************************************************************
**
**  Copyright (C) 2005 Red Hat, Inc.  All rights reserved.
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

#ifndef __UTIL_DOT_H__
#define __UTIL_DOT_H__

<<<<<<< HEAD
void dlm_message_out(struct dlm_message *ms);
void dlm_message_in(struct dlm_message *ms);
void dlm_rcom_out(struct dlm_rcom *rc);
void dlm_rcom_in(struct dlm_rcom *rc);
=======
int to_dlm_errno(int err);
int from_dlm_errno(int err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif

