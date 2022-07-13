<<<<<<< HEAD
/*
 * Copyright (C) 2000, 2001 Jeff Dike (jdike@karaya.com)
 * Licensed under the GPL
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2000, 2001 Jeff Dike (jdike@karaya.com)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __CHAN_USER_H__
#define __CHAN_USER_H__

<<<<<<< HEAD
#include "init.h"
=======
#include <init.h>
#include <linux/types.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct chan_opts {
	void (*const announce)(char *dev_name, int dev);
	char *xterm_title;
<<<<<<< HEAD
	const int raw;
=======
	int raw;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct chan_ops {
	char *type;
	void *(*init)(char *, int, const struct chan_opts *);
	int (*open)(int, int, int, void *, char **);
	void (*close)(int, void *);
<<<<<<< HEAD
	int (*read)(int, char *, void *);
	int (*write)(int, const char *, int, void *);
=======
	int (*read)(int, __u8 *, void *);
	int (*write)(int, const __u8 *, size_t, void *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int (*console_write)(int, const char *, int);
	int (*window_size)(int, void *, unsigned short *, unsigned short *);
	void (*free)(void *);
	int winch;
};

extern const struct chan_ops fd_ops, null_ops, port_ops, pts_ops, pty_ops,
	tty_ops, xterm_ops;

extern void generic_close(int fd, void *unused);
<<<<<<< HEAD
extern int generic_read(int fd, char *c_out, void *unused);
extern int generic_write(int fd, const char *buf, int n, void *unused);
=======
extern int generic_read(int fd, __u8 *c_out, void *unused);
extern int generic_write(int fd, const __u8 *buf, size_t n, void *unused);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern int generic_console_write(int fd, const char *buf, int n);
extern int generic_window_size(int fd, void *unused, unsigned short *rows_out,
			       unsigned short *cols_out);
extern void generic_free(void *data);

<<<<<<< HEAD
struct tty_struct;
extern void register_winch(int fd,  struct tty_struct *tty);
extern void register_winch_irq(int fd, int tty_fd, int pid,
			       struct tty_struct *tty, unsigned long stack);
=======
struct tty_port;
extern void register_winch(int fd,  struct tty_port *port);
extern void register_winch_irq(int fd, int tty_fd, int pid,
			       struct tty_port *port, unsigned long stack);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define __channel_help(fn, prefix) \
__uml_help(fn, prefix "[0-9]*=<channel description>\n" \
"    Attach a console or serial line to a host channel.  See\n" \
"    http://user-mode-linux.sourceforge.net/old/input.html for a complete\n" \
"    description of this switch.\n\n" \
);

#endif
