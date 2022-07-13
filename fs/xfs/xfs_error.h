<<<<<<< HEAD
/*
 * Copyright (c) 2000-2002,2005 Silicon Graphics, Inc.
 * All Rights Reserved.
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
 * Copyright (c) 2000-2002,2005 Silicon Graphics, Inc.
 * All Rights Reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef	__XFS_ERROR_H__
#define	__XFS_ERROR_H__

<<<<<<< HEAD
#ifdef DEBUG
#define	XFS_ERROR_NTRAP	10
extern int	xfs_etrap[XFS_ERROR_NTRAP];
extern int	xfs_error_trap(int);
#define	XFS_ERROR(e)	xfs_error_trap(e)
#else
#define	XFS_ERROR(e)	(e)
#endif

struct xfs_mount;

extern void xfs_error_report(const char *tag, int level, struct xfs_mount *mp,
			const char *filename, int linenum, inst_t *ra);
extern void xfs_corruption_error(const char *tag, int level,
			struct xfs_mount *mp, void *p, const char *filename,
			int linenum, inst_t *ra);

#define	XFS_ERROR_REPORT(e, lvl, mp)	\
	xfs_error_report(e, lvl, mp, __FILE__, __LINE__, __return_address)
#define	XFS_CORRUPTION_ERROR(e, lvl, mp, mem)	\
	xfs_corruption_error(e, lvl, mp, mem, \
=======
struct xfs_mount;

extern void xfs_error_report(const char *tag, int level, struct xfs_mount *mp,
			const char *filename, int linenum,
			xfs_failaddr_t failaddr);
extern void xfs_corruption_error(const char *tag, int level,
			struct xfs_mount *mp, const void *buf, size_t bufsize,
			const char *filename, int linenum,
			xfs_failaddr_t failaddr);
void xfs_buf_corruption_error(struct xfs_buf *bp, xfs_failaddr_t fa);
extern void xfs_buf_verifier_error(struct xfs_buf *bp, int error,
			const char *name, const void *buf, size_t bufsz,
			xfs_failaddr_t failaddr);
extern void xfs_verifier_error(struct xfs_buf *bp, int error,
			xfs_failaddr_t failaddr);
extern void xfs_inode_verifier_error(struct xfs_inode *ip, int error,
			const char *name, const void *buf, size_t bufsz,
			xfs_failaddr_t failaddr);

#define	XFS_ERROR_REPORT(e, lvl, mp)	\
	xfs_error_report(e, lvl, mp, __FILE__, __LINE__, __return_address)
#define	XFS_CORRUPTION_ERROR(e, lvl, mp, buf, bufsize)	\
	xfs_corruption_error(e, lvl, mp, buf, bufsize, \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			     __FILE__, __LINE__, __return_address)

#define XFS_ERRLEVEL_OFF	0
#define XFS_ERRLEVEL_LOW	1
#define XFS_ERRLEVEL_HIGH	5

<<<<<<< HEAD
/*
 * Macros to set EFSCORRUPTED & return/branch.
 */
#define	XFS_WANT_CORRUPTED_GOTO(x,l)	\
	{ \
		int fs_is_ok = (x); \
		ASSERT(fs_is_ok); \
		if (unlikely(!fs_is_ok)) { \
			XFS_ERROR_REPORT("XFS_WANT_CORRUPTED_GOTO", \
					 XFS_ERRLEVEL_LOW, NULL); \
			error = XFS_ERROR(EFSCORRUPTED); \
			goto l; \
		} \
	}

#define	XFS_WANT_CORRUPTED_RETURN(x)	\
	{ \
		int fs_is_ok = (x); \
		ASSERT(fs_is_ok); \
		if (unlikely(!fs_is_ok)) { \
			XFS_ERROR_REPORT("XFS_WANT_CORRUPTED_RETURN", \
					 XFS_ERRLEVEL_LOW, NULL); \
			return XFS_ERROR(EFSCORRUPTED); \
		} \
	}

/*
 * error injection tags - the labels can be anything you want
 * but each tag should have its own unique number
 */

#define XFS_ERRTAG_NOERROR				0
#define XFS_ERRTAG_IFLUSH_1				1
#define XFS_ERRTAG_IFLUSH_2				2
#define XFS_ERRTAG_IFLUSH_3				3
#define XFS_ERRTAG_IFLUSH_4				4
#define XFS_ERRTAG_IFLUSH_5				5
#define XFS_ERRTAG_IFLUSH_6				6
#define	XFS_ERRTAG_DA_READ_BUF				7
#define	XFS_ERRTAG_BTREE_CHECK_LBLOCK			8
#define	XFS_ERRTAG_BTREE_CHECK_SBLOCK			9
#define	XFS_ERRTAG_ALLOC_READ_AGF			10
#define	XFS_ERRTAG_IALLOC_READ_AGI			11
#define	XFS_ERRTAG_ITOBP_INOTOBP			12
#define	XFS_ERRTAG_IUNLINK				13
#define	XFS_ERRTAG_IUNLINK_REMOVE			14
#define	XFS_ERRTAG_DIR_INO_VALIDATE			15
#define XFS_ERRTAG_BULKSTAT_READ_CHUNK			16
#define XFS_ERRTAG_IODONE_IOERR				17
#define XFS_ERRTAG_STRATREAD_IOERR			18
#define XFS_ERRTAG_STRATCMPL_IOERR			19
#define XFS_ERRTAG_DIOWRITE_IOERR			20
#define XFS_ERRTAG_BMAPIFORMAT				21
#define XFS_ERRTAG_MAX					22

/*
 * Random factors for above tags, 1 means always, 2 means 1/2 time, etc.
 */
#define XFS_RANDOM_DEFAULT				100
#define XFS_RANDOM_IFLUSH_1				XFS_RANDOM_DEFAULT
#define XFS_RANDOM_IFLUSH_2				XFS_RANDOM_DEFAULT
#define XFS_RANDOM_IFLUSH_3				XFS_RANDOM_DEFAULT
#define XFS_RANDOM_IFLUSH_4				XFS_RANDOM_DEFAULT
#define XFS_RANDOM_IFLUSH_5				XFS_RANDOM_DEFAULT
#define XFS_RANDOM_IFLUSH_6				XFS_RANDOM_DEFAULT
#define XFS_RANDOM_DA_READ_BUF				XFS_RANDOM_DEFAULT
#define XFS_RANDOM_BTREE_CHECK_LBLOCK			(XFS_RANDOM_DEFAULT/4)
#define XFS_RANDOM_BTREE_CHECK_SBLOCK			XFS_RANDOM_DEFAULT
#define XFS_RANDOM_ALLOC_READ_AGF			XFS_RANDOM_DEFAULT
#define XFS_RANDOM_IALLOC_READ_AGI			XFS_RANDOM_DEFAULT
#define XFS_RANDOM_ITOBP_INOTOBP			XFS_RANDOM_DEFAULT
#define XFS_RANDOM_IUNLINK				XFS_RANDOM_DEFAULT
#define XFS_RANDOM_IUNLINK_REMOVE			XFS_RANDOM_DEFAULT
#define XFS_RANDOM_DIR_INO_VALIDATE			XFS_RANDOM_DEFAULT
#define XFS_RANDOM_BULKSTAT_READ_CHUNK			XFS_RANDOM_DEFAULT
#define XFS_RANDOM_IODONE_IOERR				(XFS_RANDOM_DEFAULT/10)
#define XFS_RANDOM_STRATREAD_IOERR			(XFS_RANDOM_DEFAULT/10)
#define XFS_RANDOM_STRATCMPL_IOERR			(XFS_RANDOM_DEFAULT/10)
#define XFS_RANDOM_DIOWRITE_IOERR			(XFS_RANDOM_DEFAULT/10)
#define	XFS_RANDOM_BMAPIFORMAT				XFS_RANDOM_DEFAULT

#ifdef DEBUG
extern int xfs_error_test_active;
extern int xfs_error_test(int, int *, char *, int, char *, unsigned long);

#define	XFS_NUM_INJECT_ERROR				10
#define XFS_TEST_ERROR(expr, mp, tag, rf)		\
	((expr) || (xfs_error_test_active && \
	 xfs_error_test((tag), (mp)->m_fixedfsid, "expr", __LINE__, __FILE__, \
			(rf))))

extern int xfs_errortag_add(int error_tag, struct xfs_mount *mp);
extern int xfs_errortag_clearall(struct xfs_mount *mp, int loud);
#else
#define XFS_TEST_ERROR(expr, mp, tag, rf)	(expr)
#define xfs_errortag_add(tag, mp)		(ENOSYS)
#define xfs_errortag_clearall(mp, loud)		(ENOSYS)
=======
/* Dump 128 bytes of any corrupt buffer */
#define XFS_CORRUPTION_DUMP_LEN		(128)

#ifdef DEBUG
extern int xfs_errortag_init(struct xfs_mount *mp);
extern void xfs_errortag_del(struct xfs_mount *mp);
extern bool xfs_errortag_test(struct xfs_mount *mp, const char *expression,
		const char *file, int line, unsigned int error_tag);
#define XFS_TEST_ERROR(expr, mp, tag)		\
	((expr) || xfs_errortag_test((mp), #expr, __FILE__, __LINE__, (tag)))
bool xfs_errortag_enabled(struct xfs_mount *mp, unsigned int tag);
#define XFS_ERRORTAG_DELAY(mp, tag)		\
	do { \
		might_sleep(); \
		if (!xfs_errortag_enabled((mp), (tag))) \
			break; \
		xfs_warn_ratelimited((mp), \
"Injecting %ums delay at file %s, line %d, on filesystem \"%s\"", \
				(mp)->m_errortag[(tag)], __FILE__, __LINE__, \
				(mp)->m_super->s_id); \
		mdelay((mp)->m_errortag[(tag)]); \
	} while (0)

extern int xfs_errortag_get(struct xfs_mount *mp, unsigned int error_tag);
extern int xfs_errortag_set(struct xfs_mount *mp, unsigned int error_tag,
		unsigned int tag_value);
extern int xfs_errortag_add(struct xfs_mount *mp, unsigned int error_tag);
extern int xfs_errortag_clearall(struct xfs_mount *mp);
#else
#define xfs_errortag_init(mp)			(0)
#define xfs_errortag_del(mp)
#define XFS_TEST_ERROR(expr, mp, tag)		(expr)
#define XFS_ERRORTAG_DELAY(mp, tag)		((void)0)
#define xfs_errortag_set(mp, tag, val)		(ENOSYS)
#define xfs_errortag_add(mp, tag)		(ENOSYS)
#define xfs_errortag_clearall(mp)		(ENOSYS)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* DEBUG */

/*
 * XFS panic tags -- allow a call to xfs_alert_tag() be turned into
<<<<<<< HEAD
 *			a panic by setting xfs_panic_mask in a sysctl.
 */
#define		XFS_NO_PTAG			0
#define		XFS_PTAG_IFLUSH			0x00000001
#define		XFS_PTAG_LOGRES			0x00000002
#define		XFS_PTAG_AILDELETE		0x00000004
#define		XFS_PTAG_ERROR_REPORT		0x00000008
#define		XFS_PTAG_SHUTDOWN_CORRUPT	0x00000010
#define		XFS_PTAG_SHUTDOWN_IOERROR	0x00000020
#define		XFS_PTAG_SHUTDOWN_LOGERROR	0x00000040
#define		XFS_PTAG_FSBLOCK_ZERO		0x00000080
=======
 *			a panic by setting fs.xfs.panic_mask in a sysctl.
 */
#define		XFS_NO_PTAG			0u
#define		XFS_PTAG_IFLUSH			(1u << 0)
#define		XFS_PTAG_LOGRES			(1u << 1)
#define		XFS_PTAG_AILDELETE		(1u << 2)
#define		XFS_PTAG_ERROR_REPORT		(1u << 3)
#define		XFS_PTAG_SHUTDOWN_CORRUPT	(1u << 4)
#define		XFS_PTAG_SHUTDOWN_IOERROR	(1u << 5)
#define		XFS_PTAG_SHUTDOWN_LOGERROR	(1u << 6)
#define		XFS_PTAG_FSBLOCK_ZERO		(1u << 7)
#define		XFS_PTAG_VERIFIER_ERROR		(1u << 8)

#define		XFS_PTAG_MASK	(XFS_PTAG_IFLUSH | \
				 XFS_PTAG_LOGRES | \
				 XFS_PTAG_AILDELETE | \
				 XFS_PTAG_ERROR_REPORT | \
				 XFS_PTAG_SHUTDOWN_CORRUPT | \
				 XFS_PTAG_SHUTDOWN_IOERROR | \
				 XFS_PTAG_SHUTDOWN_LOGERROR | \
				 XFS_PTAG_FSBLOCK_ZERO | \
				 XFS_PTAG_VERIFIER_ERROR)

#define XFS_PTAG_STRINGS \
	{ XFS_NO_PTAG,			"none" }, \
	{ XFS_PTAG_IFLUSH,		"iflush" }, \
	{ XFS_PTAG_LOGRES,		"logres" }, \
	{ XFS_PTAG_AILDELETE,		"aildelete" }, \
	{ XFS_PTAG_ERROR_REPORT	,	"error_report" }, \
	{ XFS_PTAG_SHUTDOWN_CORRUPT,	"corrupt" }, \
	{ XFS_PTAG_SHUTDOWN_IOERROR,	"ioerror" }, \
	{ XFS_PTAG_SHUTDOWN_LOGERROR,	"logerror" }, \
	{ XFS_PTAG_FSBLOCK_ZERO,	"fsb_zero" }, \
	{ XFS_PTAG_VERIFIER_ERROR,	"verifier" }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif	/* __XFS_ERROR_H__ */
