<<<<<<< HEAD
/*
 * Copyright (c) 2000-2003,2005 Silicon Graphics, Inc.
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
 * Copyright (c) 2000-2003,2005 Silicon Graphics, Inc.
 * All Rights Reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef	__XFS_LOG_H__
#define __XFS_LOG_H__

<<<<<<< HEAD
/* get lsn fields */
#define CYCLE_LSN(lsn) ((uint)((lsn)>>32))
#define BLOCK_LSN(lsn) ((uint)(lsn))

/* this is used in a spot where we might otherwise double-endian-flip */
#define CYCLE_LSN_DISK(lsn) (((__be32 *)&(lsn))[0])

#ifdef __KERNEL__
=======
struct xfs_cil_ctx;

struct xfs_log_vec {
	struct list_head	lv_list;	/* CIL lv chain ptrs */
	uint32_t		lv_order_id;	/* chain ordering info */
	int			lv_niovecs;	/* number of iovecs in lv */
	struct xfs_log_iovec	*lv_iovecp;	/* iovec array */
	struct xfs_log_item	*lv_item;	/* owner */
	char			*lv_buf;	/* formatted buffer */
	int			lv_bytes;	/* accounted space in buffer */
	int			lv_buf_len;	/* aligned size of buffer */
	int			lv_size;	/* size of allocated lv */
};

#define XFS_LOG_VEC_ORDERED	(-1)

/*
 * Calculate the log iovec length for a given user buffer length. Intended to be
 * used by ->iop_size implementations when sizing buffers of arbitrary
 * alignments.
 */
static inline int
xlog_calc_iovec_len(int len)
{
	return roundup(len, sizeof(uint32_t));
}

void *xlog_prepare_iovec(struct xfs_log_vec *lv, struct xfs_log_iovec **vecp,
		uint type);

static inline void
xlog_finish_iovec(struct xfs_log_vec *lv, struct xfs_log_iovec *vec,
		int data_len)
{
	struct xlog_op_header	*oph = vec->i_addr;
	int			len;

	/*
	 * Always round up the length to the correct alignment so callers don't
	 * need to know anything about this log vec layout requirement. This
	 * means we have to zero the area the data to be written does not cover.
	 * This is complicated by fact the payload region is offset into the
	 * logvec region by the opheader that tracks the payload.
	 */
	len = xlog_calc_iovec_len(data_len);
	if (len - data_len != 0) {
		char	*buf = vec->i_addr + sizeof(struct xlog_op_header);

		memset(buf + data_len, 0, len - data_len);
	}

	/*
	 * The opheader tracks aligned payload length, whilst the logvec tracks
	 * the overall region length.
	 */
	oph->oh_len = cpu_to_be32(len);

	len += sizeof(struct xlog_op_header);
	lv->lv_buf_len += len;
	lv->lv_bytes += len;
	vec->i_len = len;

	/* Catch buffer overruns */
	ASSERT((void *)lv->lv_buf + lv->lv_bytes <= (void *)lv + lv->lv_size);
}

/*
 * Copy the amount of data requested by the caller into a new log iovec.
 */
static inline void *
xlog_copy_iovec(struct xfs_log_vec *lv, struct xfs_log_iovec **vecp,
		uint type, void *data, int len)
{
	void *buf;

	buf = xlog_prepare_iovec(lv, vecp, type);
	memcpy(buf, data, len);
	xlog_finish_iovec(lv, *vecp, len);
	return buf;
}

static inline void *
xlog_copy_from_iovec(struct xfs_log_vec *lv, struct xfs_log_iovec **vecp,
		const struct xfs_log_iovec *src)
{
	return xlog_copy_iovec(lv, vecp, src->i_type, src->i_addr, src->i_len);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * By comparing each component, we don't have to worry about extra
 * endian issues in treating two 32 bit numbers as one 64 bit number
 */
static inline xfs_lsn_t	_lsn_cmp(xfs_lsn_t lsn1, xfs_lsn_t lsn2)
{
	if (CYCLE_LSN(lsn1) != CYCLE_LSN(lsn2))
		return (CYCLE_LSN(lsn1)<CYCLE_LSN(lsn2))? -999 : 999;

	if (BLOCK_LSN(lsn1) != BLOCK_LSN(lsn2))
		return (BLOCK_LSN(lsn1)<BLOCK_LSN(lsn2))? -999 : 999;

	return 0;
}

#define	XFS_LSN_CMP(x,y) _lsn_cmp(x,y)

/*
<<<<<<< HEAD
 * Macros, structures, prototypes for interface to the log manager.
 */

/*
 * Flags to xfs_log_done()
 */
#define XFS_LOG_REL_PERM_RESERV	0x1

/*
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Flags to xfs_log_force()
 *
 *	XFS_LOG_SYNC:	Synchronous force in-core log to disk
 */
#define XFS_LOG_SYNC		0x1

<<<<<<< HEAD
#endif	/* __KERNEL__ */


/* Log Clients */
#define XFS_TRANSACTION		0x69
#define XFS_VOLUME		0x2
#define XFS_LOG			0xaa


/* Region types for iovec's i_type */
#define XLOG_REG_TYPE_BFORMAT		1
#define XLOG_REG_TYPE_BCHUNK		2
#define XLOG_REG_TYPE_EFI_FORMAT	3
#define XLOG_REG_TYPE_EFD_FORMAT	4
#define XLOG_REG_TYPE_IFORMAT		5
#define XLOG_REG_TYPE_ICORE		6
#define XLOG_REG_TYPE_IEXT		7
#define XLOG_REG_TYPE_IBROOT		8
#define XLOG_REG_TYPE_ILOCAL		9
#define XLOG_REG_TYPE_IATTR_EXT		10
#define XLOG_REG_TYPE_IATTR_BROOT	11
#define XLOG_REG_TYPE_IATTR_LOCAL	12
#define XLOG_REG_TYPE_QFORMAT		13
#define XLOG_REG_TYPE_DQUOT		14
#define XLOG_REG_TYPE_QUOTAOFF		15
#define XLOG_REG_TYPE_LRHEADER		16
#define XLOG_REG_TYPE_UNMOUNT		17
#define XLOG_REG_TYPE_COMMIT		18
#define XLOG_REG_TYPE_TRANSHDR		19
#define XLOG_REG_TYPE_MAX		19

typedef struct xfs_log_iovec {
	void		*i_addr;	/* beginning address of region */
	int		i_len;		/* length in bytes of region */
	uint		i_type;		/* type of region */
} xfs_log_iovec_t;

struct xfs_log_vec {
	struct xfs_log_vec	*lv_next;	/* next lv in build list */
	int			lv_niovecs;	/* number of iovecs in lv */
	struct xfs_log_iovec	*lv_iovecp;	/* iovec array */
	struct xfs_log_item	*lv_item;	/* owner */
	char			*lv_buf;	/* formatted buffer */
	int			lv_buf_len;	/* size of formatted buffer */
};

/*
 * Structure used to pass callback function and the function's argument
 * to the log manager.
 */
typedef struct xfs_log_callback {
	struct xfs_log_callback	*cb_next;
	void			(*cb_func)(void *, int);
	void			*cb_arg;
} xfs_log_callback_t;


#ifdef __KERNEL__
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Log manager interfaces */
struct xfs_mount;
struct xlog_in_core;
struct xlog_ticket;
struct xfs_log_item;
struct xfs_item_ops;
struct xfs_trans;
<<<<<<< HEAD

void	xfs_log_item_init(struct xfs_mount	*mp,
			struct xfs_log_item	*item,
			int			type,
			const struct xfs_item_ops *ops);

xfs_lsn_t xfs_log_done(struct xfs_mount *mp,
		       struct xlog_ticket *ticket,
		       struct xlog_in_core **iclog,
		       uint		flags);
int	  _xfs_log_force(struct xfs_mount *mp,
			 uint		flags,
			 int		*log_forced);
void	  xfs_log_force(struct xfs_mount	*mp,
			uint			flags);
int	  _xfs_log_force_lsn(struct xfs_mount *mp,
			     xfs_lsn_t		lsn,
			     uint		flags,
			     int		*log_forced);
void	  xfs_log_force_lsn(struct xfs_mount	*mp,
			    xfs_lsn_t		lsn,
			    uint		flags);
=======
struct xlog;

int	  xfs_log_force(struct xfs_mount *mp, uint flags);
int	  xfs_log_force_seq(struct xfs_mount *mp, xfs_csn_t seq, uint flags,
		int *log_forced);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int	  xfs_log_mount(struct xfs_mount	*mp,
			struct xfs_buftarg	*log_target,
			xfs_daddr_t		start_block,
			int		 	num_bblocks);
int	  xfs_log_mount_finish(struct xfs_mount *mp);
<<<<<<< HEAD
xfs_lsn_t xlog_assign_tail_lsn(struct xfs_mount *mp);
void	  xfs_log_space_wake(struct xfs_mount *mp);
int	  xfs_log_notify(struct xfs_mount	*mp,
			 struct xlog_in_core	*iclog,
			 xfs_log_callback_t	*callback_entry);
int	  xfs_log_release_iclog(struct xfs_mount *mp,
			 struct xlog_in_core	 *iclog);
int	  xfs_log_reserve(struct xfs_mount *mp,
			  int		   length,
			  int		   count,
			  struct xlog_ticket **ticket,
			  __uint8_t	   clientid,
			  bool		   permanent,
			  uint		   t_type);
int	  xfs_log_regrant(struct xfs_mount *mp, struct xlog_ticket *tic);
int	  xfs_log_unmount_write(struct xfs_mount *mp);
void      xfs_log_unmount(struct xfs_mount *mp);
int	  xfs_log_force_umount(struct xfs_mount *mp, int logerror);
int	  xfs_log_need_covered(struct xfs_mount *mp);

void	  xlog_iodone(struct xfs_buf *);
=======
void	xfs_log_mount_cancel(struct xfs_mount *);
xfs_lsn_t xlog_assign_tail_lsn(struct xfs_mount *mp);
xfs_lsn_t xlog_assign_tail_lsn_locked(struct xfs_mount *mp);
void	xfs_log_space_wake(struct xfs_mount *mp);
int	xfs_log_reserve(struct xfs_mount *mp, int length, int count,
			struct xlog_ticket **ticket, bool permanent);
int	xfs_log_regrant(struct xfs_mount *mp, struct xlog_ticket *tic);
void	xfs_log_unmount(struct xfs_mount *mp);
bool	xfs_log_writable(struct xfs_mount *mp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct xlog_ticket *xfs_log_ticket_get(struct xlog_ticket *ticket);
void	  xfs_log_ticket_put(struct xlog_ticket *ticket);

<<<<<<< HEAD
int	xfs_log_commit_cil(struct xfs_mount *mp, struct xfs_trans *tp,
				xfs_lsn_t *commit_lsn, int flags);
bool	xfs_log_item_in_current_chkpt(struct xfs_log_item *lip);

#endif
=======
void	xlog_cil_process_committed(struct list_head *list);
bool	xfs_log_item_in_current_chkpt(struct xfs_log_item *lip);

void	xfs_log_work_queue(struct xfs_mount *mp);
int	xfs_log_quiesce(struct xfs_mount *mp);
void	xfs_log_clean(struct xfs_mount *mp);
bool	xfs_log_check_lsn(struct xfs_mount *, xfs_lsn_t);

xfs_lsn_t xlog_grant_push_threshold(struct xlog *log, int need_bytes);
bool	  xlog_force_shutdown(struct xlog *log, uint32_t shutdown_flags);

void xlog_use_incompat_feat(struct xlog *log);
void xlog_drop_incompat_feat(struct xlog *log);
int xfs_attr_use_log_assist(struct xfs_mount *mp);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif	/* __XFS_LOG_H__ */
