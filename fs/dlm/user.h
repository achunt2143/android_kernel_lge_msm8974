<<<<<<< HEAD
/*
 * Copyright (C) 2006-2010 Red Hat, Inc.  All rights reserved.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the GNU General Public License v.2.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2006-2010 Red Hat, Inc.  All rights reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __USER_DOT_H__
#define __USER_DOT_H__

<<<<<<< HEAD
void dlm_user_add_ast(struct dlm_lkb *lkb, uint32_t flags, int mode,
                      int status, uint32_t sbflags, uint64_t seq);
=======
void dlm_purge_lkb_callbacks(struct dlm_lkb *lkb);
void dlm_user_add_ast(struct dlm_lkb *lkb, uint32_t flags, int mode,
		      int status, uint32_t sbflags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int dlm_user_init(void);
void dlm_user_exit(void);
int dlm_device_deregister(struct dlm_ls *ls);
int dlm_user_daemon_available(void);

#endif
