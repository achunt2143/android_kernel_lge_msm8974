<<<<<<< HEAD
/* 
 * Copyright (C) 2001, 2002 Jeff Dike (jdike@karaya.com)
 * Licensed under the GPL
=======
/* SPDX-License-Identifier: GPL-2.0 */
/* 
 * Copyright (C) 2001, 2002 Jeff Dike (jdike@karaya.com)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __LINE_H__
#define __LINE_H__

<<<<<<< HEAD
#include "linux/list.h"
#include "linux/workqueue.h"
#include "linux/tty.h"
#include "linux/interrupt.h"
#include "linux/spinlock.h"
#include "linux/mutex.h"
=======
#include <linux/list.h>
#include <linux/workqueue.h>
#include <linux/tty.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/mutex.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "chan_user.h"
#include "mconsole_kern.h"

/* There's only two modifiable fields in this - .mc.list and .driver */
struct line_driver {
	const char *name;
	const char *device_name;
	const short major;
	const short minor_start;
	const short type;
	const short subtype;
<<<<<<< HEAD
	const int read_irq;
	const char *read_irq_name;
	const int write_irq;
=======
	const char *read_irq_name;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	const char *write_irq_name;
	struct mc_device mc;
	struct tty_driver *driver;
};

struct line {
<<<<<<< HEAD
	struct tty_struct *tty;
	struct mutex count_lock;
	unsigned long count;
	int valid;

=======
	struct tty_port port;
	int valid;

	int read_irq, write_irq;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char *init_str;
	struct list_head chan_list;
	struct chan *chan_in, *chan_out;

	/*This lock is actually, mostly, local to*/
	spinlock_t lock;
	int throttled;
	/* Yes, this is a real circular buffer.
	 * XXX: And this should become a struct kfifo!
	 *
	 * buffer points to a buffer allocated on demand, of length
	 * LINE_BUFSIZE, head to the start of the ring, tail to the end.*/
<<<<<<< HEAD
	char *buffer;
	char *head;
	char *tail;
=======
	u8 *buffer;
	u8 *head;
	u8 *tail;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	int sigio;
	struct delayed_work task;
	const struct line_driver *driver;
};

extern void line_close(struct tty_struct *tty, struct file * filp);
<<<<<<< HEAD
extern int line_open(struct line *lines, struct tty_struct *tty);
extern int line_setup(char **conf, unsigned nlines, char **def,
		      char *init, char *name);
extern int line_write(struct tty_struct *tty, const unsigned char *buf,
		      int len);
extern int line_put_char(struct tty_struct *tty, unsigned char ch);
extern void line_set_termios(struct tty_struct *tty, struct ktermios * old);
extern int line_chars_in_buffer(struct tty_struct *tty);
extern void line_flush_buffer(struct tty_struct *tty);
extern void line_flush_chars(struct tty_struct *tty);
extern int line_write_room(struct tty_struct *tty);
extern int line_ioctl(struct tty_struct *tty, unsigned int cmd,
				unsigned long arg);
=======
extern int line_open(struct tty_struct *tty, struct file *filp);
extern int line_install(struct tty_driver *driver, struct tty_struct *tty,
	struct line *line);
extern void line_cleanup(struct tty_struct *tty);
extern void line_hangup(struct tty_struct *tty);
extern int line_setup(char **conf, unsigned nlines, char **def,
		      char *init, char *name);
extern ssize_t line_write(struct tty_struct *tty, const u8 *buf, size_t len);
extern unsigned int line_chars_in_buffer(struct tty_struct *tty);
extern void line_flush_buffer(struct tty_struct *tty);
extern void line_flush_chars(struct tty_struct *tty);
extern unsigned int line_write_room(struct tty_struct *tty);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern void line_throttle(struct tty_struct *tty);
extern void line_unthrottle(struct tty_struct *tty);

extern char *add_xterm_umid(char *base);
extern int line_setup_irq(int fd, int input, int output, struct line *line,
			  void *data);
extern void line_close_chan(struct line *line);
extern int register_lines(struct line_driver *line_driver,
			  const struct tty_operations *driver,
			  struct line *lines, int nlines);
extern int setup_one_line(struct line *lines, int n, char *init,
			  const struct chan_opts *opts, char **error_out);
extern void close_lines(struct line *lines, int nlines);

extern int line_config(struct line *lines, unsigned int sizeof_lines,
		       char *str, const struct chan_opts *opts,
		       char **error_out);
extern int line_id(char **str, int *start_out, int *end_out);
extern int line_remove(struct line *lines, unsigned int sizeof_lines, int n,
		       char **error_out);
extern int line_get_config(char *dev, struct line *lines,
			   unsigned int sizeof_lines, char *str,
			   int size, char **error_out);

#endif
