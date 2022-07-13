<<<<<<< HEAD
/*
 * Copyright (c) 2011 Red Hat, Inc.  All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write the Free Software Foundation,
 * Inc.,  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2011 Red Hat, Inc.  All Rights Reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include "xfs.h"
#include "xfs_fs.h"
<<<<<<< HEAD
#include "xfs_types.h"
#include "xfs_log.h"
#include "xfs_inum.h"
#include "xfs_trans.h"
#include "xfs_sb.h"
#include "xfs_ag.h"
=======
#include "xfs_error.h"
#include "xfs_shared.h"
#include "xfs_format.h"
#include "xfs_trans_resv.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "xfs_mount.h"

/*
 * XFS logging functions
 */
static void
__xfs_printk(
	const char		*level,
	const struct xfs_mount	*mp,
	struct va_format	*vaf)
{
<<<<<<< HEAD
	if (mp && mp->m_fsname) {
		printk("%sXFS (%s): %pV\n", level, mp->m_fsname, vaf);
=======
	if (mp && mp->m_super) {
		printk("%sXFS (%s): %pV\n", level, mp->m_super->s_id, vaf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	}
	printk("%sXFS: %pV\n", level, vaf);
}

<<<<<<< HEAD
#define define_xfs_printk_level(func, kern_level)		\
void func(const struct xfs_mount *mp, const char *fmt, ...)	\
{								\
	struct va_format	vaf;				\
	va_list			args;				\
								\
	va_start(args, fmt);					\
								\
	vaf.fmt = fmt;						\
	vaf.va = &args;						\
								\
	__xfs_printk(kern_level, mp, &vaf);			\
	va_end(args);						\
}								\

define_xfs_printk_level(xfs_emerg, KERN_EMERG);
define_xfs_printk_level(xfs_alert, KERN_ALERT);
define_xfs_printk_level(xfs_crit, KERN_CRIT);
define_xfs_printk_level(xfs_err, KERN_ERR);
define_xfs_printk_level(xfs_warn, KERN_WARNING);
define_xfs_printk_level(xfs_notice, KERN_NOTICE);
define_xfs_printk_level(xfs_info, KERN_INFO);
#ifdef DEBUG
define_xfs_printk_level(xfs_debug, KERN_DEBUG);
#endif

void
xfs_alert_tag(
	const struct xfs_mount	*mp,
	int			panic_tag,
=======
void
xfs_printk_level(
	const char *kern_level,
	const struct xfs_mount *mp,
	const char *fmt, ...)
{
	struct va_format	vaf;
	va_list			args;
	int			level;

	va_start(args, fmt);
	vaf.fmt = fmt;
	vaf.va = &args;

	__xfs_printk(kern_level, mp, &vaf);

	va_end(args);

	if (!kstrtoint(kern_level, 0, &level) &&
	    level <= LOGLEVEL_ERR &&
	    xfs_error_level >= XFS_ERRLEVEL_HIGH)
		xfs_stack_trace();
}

void
_xfs_alert_tag(
	const struct xfs_mount	*mp,
	uint32_t		panic_tag,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	const char		*fmt, ...)
{
	struct va_format	vaf;
	va_list			args;
	int			do_panic = 0;

	if (xfs_panic_mask && (xfs_panic_mask & panic_tag)) {
		xfs_alert(mp, "Transforming an alert into a BUG.");
		do_panic = 1;
	}

	va_start(args, fmt);

	vaf.fmt = fmt;
	vaf.va = &args;

	__xfs_printk(KERN_ALERT, mp, &vaf);
	va_end(args);

	BUG_ON(do_panic);
}

void
<<<<<<< HEAD
assfail(char *expr, char *file, int line)
{
	xfs_emerg(NULL, "Assertion failed: %s, file: %s, line: %d",
		expr, file, line);
	BUG();
}

void
xfs_hex_dump(void *p, int length)
{
	print_hex_dump(KERN_ALERT, "", DUMP_PREFIX_ADDRESS, 16, 1, p, length, 1);
=======
asswarn(
	struct xfs_mount	*mp,
	char			*expr,
	char			*file,
	int			line)
{
	xfs_warn(mp, "Assertion failed: %s, file: %s, line: %d",
		expr, file, line);
	WARN_ON(1);
}

void
assfail(
	struct xfs_mount	*mp,
	char			*expr,
	char			*file,
	int			line)
{
	xfs_emerg(mp, "Assertion failed: %s, file: %s, line: %d",
		expr, file, line);
	if (xfs_globals.bug_on_assert)
		BUG();
	else
		WARN_ON(1);
}

void
xfs_hex_dump(const void *p, int length)
{
	print_hex_dump(KERN_ALERT, "", DUMP_PREFIX_OFFSET, 16, 1, p, length, 1);
}

void
xfs_buf_alert_ratelimited(
	struct xfs_buf		*bp,
	const char		*rlmsg,
	const char		*fmt,
	...)
{
	struct xfs_mount	*mp = bp->b_mount;
	struct va_format	vaf;
	va_list			args;

	/* use the more aggressive per-target rate limit for buffers */
	if (!___ratelimit(&bp->b_target->bt_ioerror_rl, rlmsg))
		return;

	va_start(args, fmt);
	vaf.fmt = fmt;
	vaf.va = &args;
	__xfs_printk(KERN_ALERT, mp, &vaf);
	va_end(args);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
