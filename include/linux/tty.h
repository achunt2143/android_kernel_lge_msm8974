<<<<<<< HEAD
#ifndef _LINUX_TTY_H
#define _LINUX_TTY_H

/*
 * 'tty.h' defines some structures used by tty_io.c and some defines.
 */

#define NR_LDISCS		30

/* line disciplines */
#define N_TTY		0
#define N_SLIP		1
#define N_MOUSE		2
#define N_PPP		3
#define N_STRIP		4
#define N_AX25		5
#define N_X25		6	/* X.25 async */
#define N_6PACK		7
#define N_MASC		8	/* Reserved for Mobitex module <kaz@cafe.net> */
#define N_R3964		9	/* Reserved for Simatic R3964 module */
#define N_PROFIBUS_FDL	10	/* Reserved for Profibus */
#define N_IRDA		11	/* Linux IrDa - http://irda.sourceforge.net/ */
#define N_SMSBLOCK	12	/* SMS block mode - for talking to GSM data */
				/* cards about SMS messages */
#define N_HDLC		13	/* synchronous HDLC */
#define N_SYNC_PPP	14	/* synchronous PPP */
#define N_HCI		15	/* Bluetooth HCI UART */
#define N_GIGASET_M101	16	/* Siemens Gigaset M101 serial DECT adapter */
#define N_SLCAN		17	/* Serial / USB serial CAN Adaptors */
#define N_PPS		18	/* Pulse per Second */
#define N_V253		19	/* Codec control over voice modem */
#define N_CAIF		20      /* CAIF protocol for talking to modems */
#define N_GSM0710	21	/* GSM 0710 Mux */
#define N_TI_WL		22	/* for TI's WL BT, FM, GPS combo chips */
#define N_TRACESINK	23	/* Trace data routing for MIPI P1149.7 */
#define N_TRACEROUTER	24	/* Trace data routing for MIPI P1149.7 */
#define N_SMUX		25	/* Serial MUX */

#ifdef __KERNEL__
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TTY_H
#define _LINUX_TTY_H

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/fs.h>
#include <linux/major.h>
#include <linux/termios.h>
#include <linux/workqueue.h>
#include <linux/tty_driver.h>
#include <linux/tty_ldisc.h>
<<<<<<< HEAD
#include <linux/mutex.h>

=======
#include <linux/tty_port.h>
#include <linux/mutex.h>
#include <linux/tty_flags.h>
#include <uapi/linux/tty.h>
#include <linux/rwsem.h>
#include <linux/llist.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)


/*
 * (Note: the *_driver.minor_start values 1, 64, 128, 192 are
 * hardcoded at present.)
 */
#define NR_UNIX98_PTY_DEFAULT	4096      /* Default maximum for Unix98 ptys */
#define NR_UNIX98_PTY_RESERVE	1024	  /* Default reserve for main devpts */
#define NR_UNIX98_PTY_MAX	(1 << MINORBITS) /* Absolute limit */

/*
 * This character is the same as _POSIX_VDISABLE: it cannot be used as
 * a c_cc[] character, but indicates that a particular special character
 * isn't in use (eg VINTR has no character etc)
 */
#define __DISABLED_CHAR '\0'

<<<<<<< HEAD
struct tty_buffer {
	struct tty_buffer *next;
	char *char_buf_ptr;
	unsigned char *flag_buf_ptr;
	int used;
	int size;
	int commit;
	int read;
	/* Data points here */
	unsigned long data[0];
};

/*
 * We default to dicing tty buffer allocations to this many characters
 * in order to avoid multiple page allocations. We know the size of
 * tty_buffer itself but it must also be taken into account that the
 * the buffer is 256 byte aligned. See tty_buffer_find for the allocation
 * logic this must match
 */

#define TTY_BUFFER_PAGE	(((PAGE_SIZE - sizeof(struct tty_buffer)) / 2) & ~0xFF)


struct tty_bufhead {
	struct work_struct work;
	spinlock_t lock;
	struct tty_buffer *head;	/* Queue head */
	struct tty_buffer *tail;	/* Active buffer */
	struct tty_buffer *free;	/* Free queue head */
	int memory_used;		/* Buffer space used excluding
								free queue */
};
/*
 * When a break, frame error, or parity error happens, these codes are
 * stuffed into the flags buffer.
 */
#define TTY_NORMAL	0
#define TTY_BREAK	1
#define TTY_FRAME	2
#define TTY_PARITY	3
#define TTY_OVERRUN	4

#define INTR_CHAR(tty) ((tty)->termios->c_cc[VINTR])
#define QUIT_CHAR(tty) ((tty)->termios->c_cc[VQUIT])
#define ERASE_CHAR(tty) ((tty)->termios->c_cc[VERASE])
#define KILL_CHAR(tty) ((tty)->termios->c_cc[VKILL])
#define EOF_CHAR(tty) ((tty)->termios->c_cc[VEOF])
#define TIME_CHAR(tty) ((tty)->termios->c_cc[VTIME])
#define MIN_CHAR(tty) ((tty)->termios->c_cc[VMIN])
#define SWTC_CHAR(tty) ((tty)->termios->c_cc[VSWTC])
#define START_CHAR(tty) ((tty)->termios->c_cc[VSTART])
#define STOP_CHAR(tty) ((tty)->termios->c_cc[VSTOP])
#define SUSP_CHAR(tty) ((tty)->termios->c_cc[VSUSP])
#define EOL_CHAR(tty) ((tty)->termios->c_cc[VEOL])
#define REPRINT_CHAR(tty) ((tty)->termios->c_cc[VREPRINT])
#define DISCARD_CHAR(tty) ((tty)->termios->c_cc[VDISCARD])
#define WERASE_CHAR(tty) ((tty)->termios->c_cc[VWERASE])
#define LNEXT_CHAR(tty)	((tty)->termios->c_cc[VLNEXT])
#define EOL2_CHAR(tty) ((tty)->termios->c_cc[VEOL2])

#define _I_FLAG(tty, f)	((tty)->termios->c_iflag & (f))
#define _O_FLAG(tty, f)	((tty)->termios->c_oflag & (f))
#define _C_FLAG(tty, f)	((tty)->termios->c_cflag & (f))
#define _L_FLAG(tty, f)	((tty)->termios->c_lflag & (f))
=======
#define INTR_CHAR(tty) ((tty)->termios.c_cc[VINTR])
#define QUIT_CHAR(tty) ((tty)->termios.c_cc[VQUIT])
#define ERASE_CHAR(tty) ((tty)->termios.c_cc[VERASE])
#define KILL_CHAR(tty) ((tty)->termios.c_cc[VKILL])
#define EOF_CHAR(tty) ((tty)->termios.c_cc[VEOF])
#define TIME_CHAR(tty) ((tty)->termios.c_cc[VTIME])
#define MIN_CHAR(tty) ((tty)->termios.c_cc[VMIN])
#define SWTC_CHAR(tty) ((tty)->termios.c_cc[VSWTC])
#define START_CHAR(tty) ((tty)->termios.c_cc[VSTART])
#define STOP_CHAR(tty) ((tty)->termios.c_cc[VSTOP])
#define SUSP_CHAR(tty) ((tty)->termios.c_cc[VSUSP])
#define EOL_CHAR(tty) ((tty)->termios.c_cc[VEOL])
#define REPRINT_CHAR(tty) ((tty)->termios.c_cc[VREPRINT])
#define DISCARD_CHAR(tty) ((tty)->termios.c_cc[VDISCARD])
#define WERASE_CHAR(tty) ((tty)->termios.c_cc[VWERASE])
#define LNEXT_CHAR(tty)	((tty)->termios.c_cc[VLNEXT])
#define EOL2_CHAR(tty) ((tty)->termios.c_cc[VEOL2])

#define _I_FLAG(tty, f)	((tty)->termios.c_iflag & (f))
#define _O_FLAG(tty, f)	((tty)->termios.c_oflag & (f))
#define _C_FLAG(tty, f)	((tty)->termios.c_cflag & (f))
#define _L_FLAG(tty, f)	((tty)->termios.c_lflag & (f))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define I_IGNBRK(tty)	_I_FLAG((tty), IGNBRK)
#define I_BRKINT(tty)	_I_FLAG((tty), BRKINT)
#define I_IGNPAR(tty)	_I_FLAG((tty), IGNPAR)
#define I_PARMRK(tty)	_I_FLAG((tty), PARMRK)
#define I_INPCK(tty)	_I_FLAG((tty), INPCK)
#define I_ISTRIP(tty)	_I_FLAG((tty), ISTRIP)
#define I_INLCR(tty)	_I_FLAG((tty), INLCR)
#define I_IGNCR(tty)	_I_FLAG((tty), IGNCR)
#define I_ICRNL(tty)	_I_FLAG((tty), ICRNL)
#define I_IUCLC(tty)	_I_FLAG((tty), IUCLC)
#define I_IXON(tty)	_I_FLAG((tty), IXON)
#define I_IXANY(tty)	_I_FLAG((tty), IXANY)
#define I_IXOFF(tty)	_I_FLAG((tty), IXOFF)
#define I_IMAXBEL(tty)	_I_FLAG((tty), IMAXBEL)
#define I_IUTF8(tty)	_I_FLAG((tty), IUTF8)

#define O_OPOST(tty)	_O_FLAG((tty), OPOST)
#define O_OLCUC(tty)	_O_FLAG((tty), OLCUC)
#define O_ONLCR(tty)	_O_FLAG((tty), ONLCR)
#define O_OCRNL(tty)	_O_FLAG((tty), OCRNL)
#define O_ONOCR(tty)	_O_FLAG((tty), ONOCR)
#define O_ONLRET(tty)	_O_FLAG((tty), ONLRET)
#define O_OFILL(tty)	_O_FLAG((tty), OFILL)
#define O_OFDEL(tty)	_O_FLAG((tty), OFDEL)
#define O_NLDLY(tty)	_O_FLAG((tty), NLDLY)
#define O_CRDLY(tty)	_O_FLAG((tty), CRDLY)
#define O_TABDLY(tty)	_O_FLAG((tty), TABDLY)
#define O_BSDLY(tty)	_O_FLAG((tty), BSDLY)
#define O_VTDLY(tty)	_O_FLAG((tty), VTDLY)
#define O_FFDLY(tty)	_O_FLAG((tty), FFDLY)

#define C_BAUD(tty)	_C_FLAG((tty), CBAUD)
#define C_CSIZE(tty)	_C_FLAG((tty), CSIZE)
#define C_CSTOPB(tty)	_C_FLAG((tty), CSTOPB)
#define C_CREAD(tty)	_C_FLAG((tty), CREAD)
#define C_PARENB(tty)	_C_FLAG((tty), PARENB)
#define C_PARODD(tty)	_C_FLAG((tty), PARODD)
#define C_HUPCL(tty)	_C_FLAG((tty), HUPCL)
#define C_CLOCAL(tty)	_C_FLAG((tty), CLOCAL)
#define C_CIBAUD(tty)	_C_FLAG((tty), CIBAUD)
#define C_CRTSCTS(tty)	_C_FLAG((tty), CRTSCTS)
<<<<<<< HEAD
=======
#define C_CMSPAR(tty)	_C_FLAG((tty), CMSPAR)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define L_ISIG(tty)	_L_FLAG((tty), ISIG)
#define L_ICANON(tty)	_L_FLAG((tty), ICANON)
#define L_XCASE(tty)	_L_FLAG((tty), XCASE)
#define L_ECHO(tty)	_L_FLAG((tty), ECHO)
#define L_ECHOE(tty)	_L_FLAG((tty), ECHOE)
#define L_ECHOK(tty)	_L_FLAG((tty), ECHOK)
#define L_ECHONL(tty)	_L_FLAG((tty), ECHONL)
#define L_NOFLSH(tty)	_L_FLAG((tty), NOFLSH)
#define L_TOSTOP(tty)	_L_FLAG((tty), TOSTOP)
#define L_ECHOCTL(tty)	_L_FLAG((tty), ECHOCTL)
#define L_ECHOPRT(tty)	_L_FLAG((tty), ECHOPRT)
#define L_ECHOKE(tty)	_L_FLAG((tty), ECHOKE)
#define L_FLUSHO(tty)	_L_FLAG((tty), FLUSHO)
#define L_PENDIN(tty)	_L_FLAG((tty), PENDIN)
#define L_IEXTEN(tty)	_L_FLAG((tty), IEXTEN)
#define L_EXTPROC(tty)	_L_FLAG((tty), EXTPROC)

struct device;
struct signal_struct;
<<<<<<< HEAD

/*
 * Port level information. Each device keeps its own port level information
 * so provide a common structure for those ports wanting to use common support
 * routines.
 *
 * The tty port has a different lifetime to the tty so must be kept apart.
 * In addition be careful as tty -> port mappings are valid for the life
 * of the tty object but in many cases port -> tty mappings are valid only
 * until a hangup so don't use the wrong path.
 */

struct tty_port;

struct tty_port_operations {
	/* Return 1 if the carrier is raised */
	int (*carrier_raised)(struct tty_port *port);
	/* Control the DTR line */
	void (*dtr_rts)(struct tty_port *port, int raise);
	/* Called when the last close completes or a hangup finishes
	   IFF the port was initialized. Do not use to free resources. Called
	   under the port mutex to serialize against activate/shutdowns */
	void (*shutdown)(struct tty_port *port);
	void (*drop)(struct tty_port *port);
	/* Called under the port mutex from tty_port_open, serialized using
	   the port mutex */
        /* FIXME: long term getting the tty argument *out* of this would be
           good for consoles */
	int (*activate)(struct tty_port *port, struct tty_struct *tty);
	/* Called on the final put of a port */
	void (*destruct)(struct tty_port *port);
};
	
struct tty_port {
	struct tty_struct	*tty;		/* Back pointer */
	const struct tty_port_operations *ops;	/* Port operations */
	spinlock_t		lock;		/* Lock protecting tty field */
	int			blocked_open;	/* Waiting to open */
	int			count;		/* Usage count */
	wait_queue_head_t	open_wait;	/* Open waiters */
	wait_queue_head_t	close_wait;	/* Close waiters */
	wait_queue_head_t	delta_msr_wait;	/* Modem status change */
	unsigned long		flags;		/* TTY flags ASY_*/
	unsigned char		console:1;	/* port is a console */
	struct mutex		mutex;		/* Locking */
	struct mutex		buf_mutex;	/* Buffer alloc lock */
	unsigned char		*xmit_buf;	/* Optional buffer */
	unsigned int		close_delay;	/* Close port delay */
	unsigned int		closing_wait;	/* Delay for output */
	int			drain_delay;	/* Set to zero if no pure time
						   based drain is needed else
						   set to size of fifo */
	struct kref		kref;		/* Ref counter */
};

/*
 * Where all of the state associated with a tty is kept while the tty
 * is open.  Since the termios state should be kept even if the tty
 * has been closed --- for things like the baud rate, etc --- it is
 * not stored here, but rather a pointer to the real state is stored
 * here.  Possible the winsize structure should have the same
 * treatment, but (1) the default 80x24 is usually right and (2) it's
 * most often used by a windowing system, which will set the correct
 * size each time the window is created or resized anyway.
 * 						- TYT, 9/14/92
 */

struct tty_operations;

struct tty_struct {
	int	magic;
	struct kref kref;
	struct device *dev;
	struct tty_driver *driver;
	const struct tty_operations *ops;
	int index;

	/* Protects ldisc changes: Lock tty not pty */
	struct mutex ldisc_mutex;
	struct tty_ldisc *ldisc;

	struct mutex termios_mutex;
	spinlock_t ctrl_lock;
	/* Termios values are protected by the termios mutex */
	struct ktermios *termios, *termios_locked;
	struct termiox *termiox;	/* May be NULL for unsupported */
	char name[64];
	struct pid *pgrp;		/* Protected by ctrl lock */
	struct pid *session;
	unsigned long flags;
	int count;
	struct winsize winsize;		/* termios mutex */
	unsigned char stopped:1, hw_stopped:1, flow_stopped:1, packet:1;
	unsigned char low_latency:1, warned:1;
	unsigned char ctrl_status;	/* ctrl_lock */
	unsigned int receive_room;	/* Bytes free for queue */

	struct tty_struct *link;
	struct fasync_struct *fasync;
	struct tty_bufhead buf;		/* Locked internally */
	int alt_speed;		/* For magic substitution of 38400 bps */
=======
struct tty_operations;

/**
 * struct tty_struct - state associated with a tty while open
 *
 * @kref: reference counting by tty_kref_get() and tty_kref_put(), reaching zero
 *	  frees the structure
 * @dev: class device or %NULL (e.g. ptys, serdev)
 * @driver: &struct tty_driver operating this tty
 * @ops: &struct tty_operations of @driver for this tty (open, close, etc.)
 * @index: index of this tty (e.g. to construct @name like tty12)
 * @ldisc_sem: protects line discipline changes (@ldisc) -- lock tty not pty
 * @ldisc: the current line discipline for this tty (n_tty by default)
 * @atomic_write_lock: protects against concurrent writers, i.e. locks
 *		       @write_cnt, @write_buf and similar
 * @legacy_mutex: leftover from history (BKL -> BTM -> @legacy_mutex),
 *		  protecting several operations on this tty
 * @throttle_mutex: protects against concurrent tty_throttle_safe() and
 *		    tty_unthrottle_safe() (but not tty_unthrottle())
 * @termios_rwsem: protects @termios and @termios_locked
 * @winsize_mutex: protects @winsize
 * @termios: termios for the current tty, copied from/to @driver.termios
 * @termios_locked: locked termios (by %TIOCGLCKTRMIOS and %TIOCSLCKTRMIOS
 *		    ioctls)
 * @name: name of the tty constructed by tty_line_name() (e.g. ttyS3)
 * @flags: bitwise OR of %TTY_THROTTLED, %TTY_IO_ERROR, ...
 * @count: count of open processes, reaching zero cancels all the work for
 *	   this tty and drops a @kref too (but does not free this tty)
 * @winsize: size of the terminal "window" (cf. @winsize_mutex)
 * @flow: flow settings grouped together, see also @flow.unused
 * @flow.lock: lock for @flow members
 * @flow.stopped: tty stopped/started by stop_tty()/start_tty()
 * @flow.tco_stopped: tty stopped/started by %TCOOFF/%TCOON ioctls (it has
 *		      precedence over @flow.stopped)
 * @flow.unused: alignment for Alpha, so that no members other than @flow.* are
 *		 modified by the same 64b word store. The @flow's __aligned is
 *		 there for the very same reason.
 * @ctrl: control settings grouped together, see also @ctrl.unused
 * @ctrl.lock: lock for @ctrl members
 * @ctrl.pgrp: process group of this tty (setpgrp(2))
 * @ctrl.session: session of this tty (setsid(2)). Writes are protected by both
 *		  @ctrl.lock and @legacy_mutex, readers must use at least one of
 *		  them.
 * @ctrl.pktstatus: packet mode status (bitwise OR of %TIOCPKT_ constants)
 * @ctrl.packet: packet mode enabled
 * @ctrl.unused: alignment for Alpha, see @flow.unused for explanation
 * @hw_stopped: not controlled by the tty layer, under @driver's control for CTS
 *		handling
 * @receive_room: bytes permitted to feed to @ldisc without any being lost
 * @flow_change: controls behavior of throttling, see tty_throttle_safe() and
 *		 tty_unthrottle_safe()
 * @link: link to another pty (master -> slave and vice versa)
 * @fasync: state for %O_ASYNC (for %SIGIO); managed by fasync_helper()
 * @write_wait: concurrent writers are waiting in this queue until they are
 *		allowed to write
 * @read_wait: readers wait for data in this queue
 * @hangup_work: normally a work to perform a hangup (do_tty_hangup()); while
 *		 freeing the tty, (re)used to release_one_tty()
 * @disc_data: pointer to @ldisc's private data (e.g. to &struct n_tty_data)
 * @driver_data: pointer to @driver's private data (e.g. &struct uart_state)
 * @files_lock:	protects @tty_files list
 * @tty_files: list of (re)openers of this tty (i.e. linked &struct
 *	       tty_file_private)
 * @closing: when set during close, n_tty processes only START & STOP chars
 * @write_buf: temporary buffer used during tty_write() to copy user data to
 * @write_cnt: count of bytes written in tty_write() to @write_buf
 * @SAK_work: if the tty has a pending do_SAK, it is queued here
 * @port: persistent storage for this device (i.e. &struct tty_port)
 *
 * All of the state associated with a tty while the tty is open. Persistent
 * storage for tty devices is referenced here as @port and is documented in
 * &struct tty_port.
 */
struct tty_struct {
	struct kref kref;
	int index;
	struct device *dev;
	struct tty_driver *driver;
	struct tty_port *port;
	const struct tty_operations *ops;

	struct tty_ldisc *ldisc;
	struct ld_semaphore ldisc_sem;

	struct mutex atomic_write_lock;
	struct mutex legacy_mutex;
	struct mutex throttle_mutex;
	struct rw_semaphore termios_rwsem;
	struct mutex winsize_mutex;
	struct ktermios termios, termios_locked;
	char name[64];
	unsigned long flags;
	int count;
	unsigned int receive_room;
	struct winsize winsize;

	struct {
		spinlock_t lock;
		bool stopped;
		bool tco_stopped;
		unsigned long unused[0];
	} __aligned(sizeof(unsigned long)) flow;

	struct {
		struct pid *pgrp;
		struct pid *session;
		spinlock_t lock;
		unsigned char pktstatus;
		bool packet;
		unsigned long unused[0];
	} __aligned(sizeof(unsigned long)) ctrl;

	bool hw_stopped;
	bool closing;
	int flow_change;

	struct tty_struct *link;
	struct fasync_struct *fasync;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	wait_queue_head_t write_wait;
	wait_queue_head_t read_wait;
	struct work_struct hangup_work;
	void *disc_data;
	void *driver_data;
<<<<<<< HEAD
	struct list_head tty_files;

#define N_TTY_BUF_SIZE 4096

	/*
	 * The following is data for the N_TTY line discipline.  For
	 * historical reasons, this is included in the tty structure.
	 * Mostly locked by the BKL.
	 */
	unsigned int column;
	unsigned char lnext:1, erasing:1, raw:1, real_raw:1, icanon:1;
	unsigned char closing:1;
	unsigned char echo_overrun:1;
	unsigned short minimum_to_wake;
	unsigned long overrun_time;
	int num_overrun;
	unsigned long process_char_map[256/(8*sizeof(unsigned long))];
	char *read_buf;
	int read_head;
	int read_tail;
	int read_cnt;
	unsigned long read_flags[N_TTY_BUF_SIZE/(8*sizeof(unsigned long))];
	unsigned char *echo_buf;
	unsigned int echo_pos;
	unsigned int echo_cnt;
	int canon_data;
	unsigned long canon_head;
	unsigned int canon_column;
	struct mutex atomic_read_lock;
	struct mutex atomic_write_lock;
	struct mutex output_lock;
	struct mutex echo_lock;
	unsigned char *write_buf;
	int write_cnt;
	spinlock_t read_lock;
	/* If the tty has a pending do_SAK, queue it here - akpm */
	struct work_struct SAK_work;
	struct tty_port *port;
};
=======
	spinlock_t files_lock;
	int write_cnt;
	u8 *write_buf;

	struct list_head tty_files;

#define N_TTY_BUF_SIZE 4096
	struct work_struct SAK_work;
} __randomize_layout;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Each of a tty's open files has private_data pointing to tty_file_private */
struct tty_file_private {
	struct tty_struct *tty;
	struct file *file;
	struct list_head list;
};

<<<<<<< HEAD
/* tty magic number */
#define TTY_MAGIC		0x5401

/*
 * These bits are used in the flags field of the tty structure.
=======
/**
 * DOC: TTY Struct Flags
 *
 * These bits are used in the :c:member:`tty_struct.flags` field.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * So that interrupts won't be able to mess up the queues,
 * copy_to_cooked must be atomic with respect to itself, as must
 * tty->write.  Thus, you must use the inline functions set_bit() and
 * clear_bit() to make things atomic.
<<<<<<< HEAD
 */
#define TTY_THROTTLED 		0	/* Call unthrottle() at threshold min */
#define TTY_IO_ERROR 		1	/* Cause an I/O error (may be no ldisc too) */
#define TTY_OTHER_CLOSED 	2	/* Other side (if any) has closed */
#define TTY_EXCLUSIVE 		3	/* Exclusive open mode */
#define TTY_DEBUG 		4	/* Debugging */
#define TTY_DO_WRITE_WAKEUP 	5	/* Call write_wakeup after queuing new */
#define TTY_PUSH 		6	/* n_tty private */
#define TTY_CLOSING 		7	/* ->close() in progress */
#define TTY_LDISC 		9	/* Line discipline attached */
#define TTY_LDISC_CHANGING 	10	/* Line discipline changing */
#define TTY_LDISC_OPEN	 	11	/* Line discipline is open */
#define TTY_HW_COOK_OUT 	14	/* Hardware can do output cooking */
#define TTY_HW_COOK_IN 		15	/* Hardware can do input cooking */
#define TTY_PTY_LOCK 		16	/* pty private */
#define TTY_NO_WRITE_SPLIT 	17	/* Preserve write boundaries to driver */
#define TTY_HUPPED 		18	/* Post driver->hangup() */
#define TTY_FLUSHING		19	/* Flushing to ldisc in progress */
#define TTY_FLUSHPENDING	20	/* Queued buffer flush pending */
#define TTY_HUPPING 		21	/* ->hangup() in progress */

#define TTY_WRITE_FLUSH(tty) tty_write_flush((tty))

extern void tty_write_flush(struct tty_struct *);

extern struct ktermios tty_std_termios;

extern void console_init(void);
extern int vcs_init(void);

extern struct class *tty_class;

/**
 *	tty_kref_get		-	get a tty reference
 *	@tty: tty device
 *
 *	Return a new reference to a tty object. The caller must hold
 *	sufficient locks/counts to ensure that their existing reference cannot
 *	go away
 */

=======
 *
 * TTY_THROTTLED
 *	Driver input is throttled. The ldisc should call
 *	:c:member:`tty_driver.unthrottle()` in order to resume reception when
 *	it is ready to process more data (at threshold min).
 *
 * TTY_IO_ERROR
 *	If set, causes all subsequent userspace read/write calls on the tty to
 *	fail, returning -%EIO. (May be no ldisc too.)
 *
 * TTY_OTHER_CLOSED
 *	Device is a pty and the other side has closed.
 *
 * TTY_EXCLUSIVE
 *	Exclusive open mode (a single opener).
 *
 * TTY_DO_WRITE_WAKEUP
 *	If set, causes the driver to call the
 *	:c:member:`tty_ldisc_ops.write_wakeup()` method in order to resume
 *	transmission when it can accept more data to transmit.
 *
 * TTY_LDISC_OPEN
 *	Indicates that a line discipline is open. For debugging purposes only.
 *
 * TTY_PTY_LOCK
 *	A flag private to pty code to implement %TIOCSPTLCK/%TIOCGPTLCK logic.
 *
 * TTY_NO_WRITE_SPLIT
 *	Prevent driver from splitting up writes into smaller chunks (preserve
 *	write boundaries to driver).
 *
 * TTY_HUPPED
 *	The TTY was hung up. This is set post :c:member:`tty_driver.hangup()`.
 *
 * TTY_HUPPING
 *	The TTY is in the process of hanging up to abort potential readers.
 *
 * TTY_LDISC_CHANGING
 *	Line discipline for this TTY is being changed. I/O should not block
 *	when this is set. Use tty_io_nonblock() to check.
 *
 * TTY_LDISC_HALTED
 *	Line discipline for this TTY was stopped. No work should be queued to
 *	this ldisc.
 */
#define TTY_THROTTLED		0
#define TTY_IO_ERROR		1
#define TTY_OTHER_CLOSED	2
#define TTY_EXCLUSIVE		3
#define TTY_DO_WRITE_WAKEUP	5
#define TTY_LDISC_OPEN		11
#define TTY_PTY_LOCK		16
#define TTY_NO_WRITE_SPLIT	17
#define TTY_HUPPED		18
#define TTY_HUPPING		19
#define TTY_LDISC_CHANGING	20
#define TTY_LDISC_HALTED	22

static inline bool tty_io_nonblock(struct tty_struct *tty, struct file *file)
{
	return file->f_flags & O_NONBLOCK ||
		test_bit(TTY_LDISC_CHANGING, &tty->flags);
}

static inline bool tty_io_error(struct tty_struct *tty)
{
	return test_bit(TTY_IO_ERROR, &tty->flags);
}

static inline bool tty_throttled(struct tty_struct *tty)
{
	return test_bit(TTY_THROTTLED, &tty->flags);
}

#ifdef CONFIG_TTY
void tty_kref_put(struct tty_struct *tty);
struct pid *tty_get_pgrp(struct tty_struct *tty);
void tty_vhangup_self(void);
void disassociate_ctty(int priv);
dev_t tty_devnum(struct tty_struct *tty);
void proc_clear_tty(struct task_struct *p);
struct tty_struct *get_current_tty(void);
/* tty_io.c */
int __init tty_init(void);
const char *tty_name(const struct tty_struct *tty);
struct tty_struct *tty_kopen_exclusive(dev_t device);
struct tty_struct *tty_kopen_shared(dev_t device);
void tty_kclose(struct tty_struct *tty);
int tty_dev_name_to_number(const char *name, dev_t *number);
#else
static inline void tty_kref_put(struct tty_struct *tty)
{ }
static inline struct pid *tty_get_pgrp(struct tty_struct *tty)
{ return NULL; }
static inline void tty_vhangup_self(void)
{ }
static inline void disassociate_ctty(int priv)
{ }
static inline dev_t tty_devnum(struct tty_struct *tty)
{ return 0; }
static inline void proc_clear_tty(struct task_struct *p)
{ }
static inline struct tty_struct *get_current_tty(void)
{ return NULL; }
/* tty_io.c */
static inline int __init tty_init(void)
{ return 0; }
static inline const char *tty_name(const struct tty_struct *tty)
{ return "(none)"; }
static inline struct tty_struct *tty_kopen_exclusive(dev_t device)
{ return ERR_PTR(-ENODEV); }
static inline void tty_kclose(struct tty_struct *tty)
{ }
static inline int tty_dev_name_to_number(const char *name, dev_t *number)
{ return -ENOTSUPP; }
#endif

extern struct ktermios tty_std_termios;

int vcs_init(void);

extern const struct class tty_class;

/**
 * tty_kref_get - get a tty reference
 * @tty: tty device
 *
 * Returns: a new reference to a tty object
 *
 * Locking: The caller must hold sufficient locks/counts to ensure that their
 * existing reference cannot go away.
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline struct tty_struct *tty_kref_get(struct tty_struct *tty)
{
	if (tty)
		kref_get(&tty->kref);
	return tty;
}
<<<<<<< HEAD
extern void tty_kref_put(struct tty_struct *tty);

extern int tty_paranoia_check(struct tty_struct *tty, struct inode *inode,
			      const char *routine);
extern char *tty_name(struct tty_struct *tty, char *buf);
extern void tty_wait_until_sent(struct tty_struct *tty, long timeout);
extern int tty_check_change(struct tty_struct *tty);
extern void stop_tty(struct tty_struct *tty);
extern void start_tty(struct tty_struct *tty);
extern int tty_register_driver(struct tty_driver *driver);
extern int tty_unregister_driver(struct tty_driver *driver);
extern struct device *tty_register_device(struct tty_driver *driver,
					  unsigned index, struct device *dev);
extern void tty_unregister_device(struct tty_driver *driver, unsigned index);
extern int tty_read_raw_data(struct tty_struct *tty, unsigned char *bufp,
			     int buflen);
extern void tty_write_message(struct tty_struct *tty, char *msg);
extern int tty_put_char(struct tty_struct *tty, unsigned char c);
extern int tty_chars_in_buffer(struct tty_struct *tty);
extern int tty_write_room(struct tty_struct *tty);
extern void tty_driver_flush_buffer(struct tty_struct *tty);
extern void tty_throttle(struct tty_struct *tty);
extern void tty_unthrottle(struct tty_struct *tty);
extern int tty_do_resize(struct tty_struct *tty, struct winsize *ws);
extern void tty_driver_remove_tty(struct tty_driver *driver,
				  struct tty_struct *tty);
extern void tty_shutdown(struct tty_struct *tty);
extern void tty_free_termios(struct tty_struct *tty);
extern int is_current_pgrp_orphaned(void);
extern struct pid *tty_get_pgrp(struct tty_struct *tty);
extern int is_ignored(int sig);
extern int tty_signal(int sig, struct tty_struct *tty);
extern void tty_hangup(struct tty_struct *tty);
extern void tty_vhangup(struct tty_struct *tty);
extern void tty_vhangup_locked(struct tty_struct *tty);
extern void tty_vhangup_self(void);
extern void tty_unhangup(struct file *filp);
extern int tty_hung_up_p(struct file *filp);
extern void do_SAK(struct tty_struct *tty);
extern void __do_SAK(struct tty_struct *tty);
extern void disassociate_ctty(int priv);
extern void no_tty(void);
extern void tty_flip_buffer_push(struct tty_struct *tty);
extern void tty_flush_to_ldisc(struct tty_struct *tty);
extern void tty_buffer_free_all(struct tty_struct *tty);
extern void tty_buffer_flush(struct tty_struct *tty);
extern void tty_buffer_init(struct tty_struct *tty);
extern speed_t tty_get_baud_rate(struct tty_struct *tty);
extern speed_t tty_termios_baud_rate(struct ktermios *termios);
extern speed_t tty_termios_input_baud_rate(struct ktermios *termios);
extern void tty_termios_encode_baud_rate(struct ktermios *termios,
						speed_t ibaud, speed_t obaud);
extern void tty_encode_baud_rate(struct tty_struct *tty,
						speed_t ibaud, speed_t obaud);
extern void tty_termios_copy_hw(struct ktermios *new, struct ktermios *old);
extern int tty_termios_hw_change(struct ktermios *a, struct ktermios *b);
extern int tty_set_termios(struct tty_struct *tty, struct ktermios *kt);

extern struct tty_ldisc *tty_ldisc_ref(struct tty_struct *);
extern void tty_ldisc_deref(struct tty_ldisc *);
extern struct tty_ldisc *tty_ldisc_ref_wait(struct tty_struct *);
extern void tty_ldisc_hangup(struct tty_struct *tty);
extern const struct file_operations tty_ldiscs_proc_fops;

extern void tty_wakeup(struct tty_struct *tty);
extern void tty_ldisc_flush(struct tty_struct *tty);

extern long tty_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
extern int tty_mode_ioctl(struct tty_struct *tty, struct file *file,
			unsigned int cmd, unsigned long arg);
extern int tty_perform_flush(struct tty_struct *tty, unsigned long arg);
extern dev_t tty_devnum(struct tty_struct *tty);
extern void proc_clear_tty(struct task_struct *p);
extern struct tty_struct *get_current_tty(void);
extern void tty_default_fops(struct file_operations *fops);
extern struct tty_struct *alloc_tty_struct(void);
extern int tty_alloc_file(struct file *file);
extern void tty_add_file(struct tty_struct *tty, struct file *file);
extern void tty_free_file(struct file *file);
extern void free_tty_struct(struct tty_struct *tty);
extern void initialize_tty_struct(struct tty_struct *tty,
		struct tty_driver *driver, int idx);
extern void deinitialize_tty_struct(struct tty_struct *tty);
extern struct tty_struct *tty_init_dev(struct tty_driver *driver, int idx);
extern int tty_release(struct inode *inode, struct file *filp);
extern int tty_init_termios(struct tty_struct *tty);
extern int tty_standard_install(struct tty_driver *driver,
		struct tty_struct *tty);

extern struct tty_struct *tty_pair_get_tty(struct tty_struct *tty);
extern struct tty_struct *tty_pair_get_pty(struct tty_struct *tty);

extern struct mutex tty_mutex;
extern spinlock_t tty_files_lock;

extern void tty_write_unlock(struct tty_struct *tty);
extern int tty_write_lock(struct tty_struct *tty, int ndelay);
#define tty_is_writelocked(tty)  (mutex_is_locked(&tty->atomic_write_lock))

extern void tty_port_init(struct tty_port *port);
extern int tty_port_alloc_xmit_buf(struct tty_port *port);
extern void tty_port_free_xmit_buf(struct tty_port *port);
extern void tty_port_put(struct tty_port *port);

static inline struct tty_port *tty_port_get(struct tty_port *port)
{
	if (port)
		kref_get(&port->kref);
	return port;
}

extern struct tty_struct *tty_port_tty_get(struct tty_port *port);
extern void tty_port_tty_set(struct tty_port *port, struct tty_struct *tty);
extern int tty_port_carrier_raised(struct tty_port *port);
extern void tty_port_raise_dtr_rts(struct tty_port *port);
extern void tty_port_lower_dtr_rts(struct tty_port *port);
extern void tty_port_hangup(struct tty_port *port);
extern int tty_port_block_til_ready(struct tty_port *port,
				struct tty_struct *tty, struct file *filp);
extern int tty_port_close_start(struct tty_port *port,
				struct tty_struct *tty, struct file *filp);
extern void tty_port_close_end(struct tty_port *port, struct tty_struct *tty);
extern void tty_port_close(struct tty_port *port,
				struct tty_struct *tty, struct file *filp);
extern int tty_port_open(struct tty_port *port,
				struct tty_struct *tty, struct file *filp);
static inline int tty_port_users(struct tty_port *port)
{
	return port->count + port->blocked_open;
}

extern int tty_register_ldisc(int disc, struct tty_ldisc_ops *new_ldisc);
extern int tty_unregister_ldisc(int disc);
extern int tty_set_ldisc(struct tty_struct *tty, int ldisc);
extern int tty_ldisc_setup(struct tty_struct *tty, struct tty_struct *o_tty);
extern void tty_ldisc_release(struct tty_struct *tty, struct tty_struct *o_tty);
extern void tty_ldisc_init(struct tty_struct *tty);
extern void tty_ldisc_deinit(struct tty_struct *tty);
extern void tty_ldisc_begin(void);
/* This last one is just for the tty layer internals and shouldn't be used elsewhere */
extern void tty_ldisc_enable(struct tty_struct *tty);


/* n_tty.c */
extern struct tty_ldisc_ops tty_ldisc_N_TTY;
extern void n_tty_inherit_ops(struct tty_ldisc_ops *ops);

/* tty_audit.c */
#ifdef CONFIG_AUDIT
extern void tty_audit_add_data(struct tty_struct *tty, unsigned char *data,
			       size_t size);
extern void tty_audit_exit(void);
extern void tty_audit_fork(struct signal_struct *sig);
extern void tty_audit_tiocsti(struct tty_struct *tty, char ch);
extern void tty_audit_push(struct tty_struct *tty);
extern int tty_audit_push_task(struct task_struct *tsk,
			       uid_t loginuid, u32 sessionid);
#else
static inline void tty_audit_add_data(struct tty_struct *tty,
				      unsigned char *data, size_t size)
{
}
static inline void tty_audit_tiocsti(struct tty_struct *tty, char ch)
{
}
=======

const char *tty_driver_name(const struct tty_struct *tty);
void tty_wait_until_sent(struct tty_struct *tty, long timeout);
void stop_tty(struct tty_struct *tty);
void start_tty(struct tty_struct *tty);
void tty_write_message(struct tty_struct *tty, char *msg);
int tty_send_xchar(struct tty_struct *tty, u8 ch);
int tty_put_char(struct tty_struct *tty, u8 c);
unsigned int tty_chars_in_buffer(struct tty_struct *tty);
unsigned int tty_write_room(struct tty_struct *tty);
void tty_driver_flush_buffer(struct tty_struct *tty);
void tty_unthrottle(struct tty_struct *tty);
bool tty_throttle_safe(struct tty_struct *tty);
bool tty_unthrottle_safe(struct tty_struct *tty);
int tty_do_resize(struct tty_struct *tty, struct winsize *ws);
int tty_get_icount(struct tty_struct *tty,
		struct serial_icounter_struct *icount);
int tty_get_tiocm(struct tty_struct *tty);
int is_current_pgrp_orphaned(void);
void tty_hangup(struct tty_struct *tty);
void tty_vhangup(struct tty_struct *tty);
int tty_hung_up_p(struct file *filp);
void do_SAK(struct tty_struct *tty);
void __do_SAK(struct tty_struct *tty);
void no_tty(void);
speed_t tty_termios_baud_rate(const struct ktermios *termios);
void tty_termios_encode_baud_rate(struct ktermios *termios, speed_t ibaud,
		speed_t obaud);
void tty_encode_baud_rate(struct tty_struct *tty, speed_t ibaud,
		speed_t obaud);

/**
 * tty_get_baud_rate - get tty bit rates
 * @tty: tty to query
 *
 * Returns: the baud rate as an integer for this terminal
 *
 * Locking: The termios lock must be held by the caller.
 */
static inline speed_t tty_get_baud_rate(const struct tty_struct *tty)
{
	return tty_termios_baud_rate(&tty->termios);
}

unsigned char tty_get_char_size(unsigned int cflag);
unsigned char tty_get_frame_size(unsigned int cflag);

void tty_termios_copy_hw(struct ktermios *new, const struct ktermios *old);
bool tty_termios_hw_change(const struct ktermios *a, const struct ktermios *b);
int tty_set_termios(struct tty_struct *tty, struct ktermios *kt);

void tty_wakeup(struct tty_struct *tty);

int tty_mode_ioctl(struct tty_struct *tty, unsigned int cmd, unsigned long arg);
int tty_perform_flush(struct tty_struct *tty, unsigned long arg);
struct tty_struct *tty_init_dev(struct tty_driver *driver, int idx);
void tty_release_struct(struct tty_struct *tty, int idx);
void tty_init_termios(struct tty_struct *tty);
void tty_save_termios(struct tty_struct *tty);
int tty_standard_install(struct tty_driver *driver,
		struct tty_struct *tty);

extern struct mutex tty_mutex;

/* n_tty.c */
void n_tty_inherit_ops(struct tty_ldisc_ops *ops);
#ifdef CONFIG_TTY
void __init n_tty_init(void);
#else
static inline void n_tty_init(void) { }
#endif

/* tty_audit.c */
#ifdef CONFIG_AUDIT
void tty_audit_exit(void);
void tty_audit_fork(struct signal_struct *sig);
int tty_audit_push(void);
#else
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void tty_audit_exit(void)
{
}
static inline void tty_audit_fork(struct signal_struct *sig)
{
}
<<<<<<< HEAD
static inline void tty_audit_push(struct tty_struct *tty)
{
}
static inline int tty_audit_push_task(struct task_struct *tsk,
				      uid_t loginuid, u32 sessionid)
=======
static inline int tty_audit_push(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return 0;
}
#endif

<<<<<<< HEAD
/* tty_io.c */
extern int __init tty_init(void);

/* tty_ioctl.c */
extern int n_tty_ioctl_helper(struct tty_struct *tty, struct file *file,
		       unsigned int cmd, unsigned long arg);
extern long n_tty_compat_ioctl_helper(struct tty_struct *tty, struct file *file,
		       unsigned int cmd, unsigned long arg);

/* serial.c */

extern void serial_console_init(void);

/* pcxx.c */

extern int pcxe_open(struct tty_struct *tty, struct file *filp);

/* vt.c */

extern int vt_ioctl(struct tty_struct *tty,
		    unsigned int cmd, unsigned long arg);

extern long vt_compat_ioctl(struct tty_struct *tty,
		     unsigned int cmd, unsigned long arg);

/* tty_mutex.c */
/* functions for preparation of BKL removal */
extern void __lockfunc tty_lock(void) __acquires(tty_lock);
extern void __lockfunc tty_unlock(void) __releases(tty_lock);

/*
 * this shall be called only from where BTM is held (like close)
 *
 * We need this to ensure nobody waits for us to finish while we are waiting.
 * Without this we were encountering system stalls.
 *
 * This should be indeed removed with BTM removal later.
 *
 * Locking: BTM required. Nobody is allowed to hold port->mutex.
 */
static inline void tty_wait_until_sent_from_close(struct tty_struct *tty,
		long timeout)
{
	tty_unlock(); /* tty->ops->close holds the BTM, drop it while waiting */
	tty_wait_until_sent(tty, timeout);
	tty_lock();
}

/*
 * wait_event_interruptible_tty -- wait for a condition with the tty lock held
 *
 * The condition we are waiting for might take a long time to
 * become true, or might depend on another thread taking the
 * BTM. In either case, we need to drop the BTM to guarantee
 * forward progress. This is a leftover from the conversion
 * from the BKL and should eventually get removed as the BTM
 * falls out of use.
 *
 * Do not use in new code.
 */
#define wait_event_interruptible_tty(wq, condition)			\
({									\
	int __ret = 0;							\
	if (!(condition)) {						\
		__wait_event_interruptible_tty(wq, condition, __ret);	\
	}								\
	__ret;								\
})

#define __wait_event_interruptible_tty(wq, condition, ret)		\
do {									\
	DEFINE_WAIT(__wait);						\
									\
	for (;;) {							\
		prepare_to_wait(&wq, &__wait, TASK_INTERRUPTIBLE);	\
		if (condition)						\
			break;						\
		if (!signal_pending(current)) {				\
			tty_unlock();					\
			schedule();					\
			tty_lock();					\
			continue;					\
		}							\
		ret = -ERESTARTSYS;					\
		break;							\
	}								\
	finish_wait(&wq, &__wait);					\
} while (0)


#endif /* __KERNEL__ */
=======
/* tty_ioctl.c */
int n_tty_ioctl_helper(struct tty_struct *tty, unsigned int cmd,
		unsigned long arg);

/* vt.c */

int vt_ioctl(struct tty_struct *tty, unsigned int cmd, unsigned long arg);

long vt_compat_ioctl(struct tty_struct *tty, unsigned int cmd,
		unsigned long arg);

/* tty_mutex.c */
/* functions for preparation of BKL removal */
void tty_lock(struct tty_struct *tty);
int  tty_lock_interruptible(struct tty_struct *tty);
void tty_unlock(struct tty_struct *tty);
void tty_lock_slave(struct tty_struct *tty);
void tty_unlock_slave(struct tty_struct *tty);
void tty_set_lock_subclass(struct tty_struct *tty);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
