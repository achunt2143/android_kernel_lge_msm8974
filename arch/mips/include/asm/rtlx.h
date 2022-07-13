/*
<<<<<<< HEAD
 * Copyright (C) 2004, 2005 MIPS Technologies, Inc.  All rights reserved.
 *
 */

=======
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2004, 2005 MIPS Technologies, Inc.  All rights reserved.
 * Copyright (C) 2013 Imagination Technologies Ltd.
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __ASM_RTLX_H_
#define __ASM_RTLX_H_

#include <irq.h>

<<<<<<< HEAD
=======
#define RTLX_MODULE_NAME "rtlx"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define LX_NODE_BASE 10

#define MIPS_CPU_RTLX_IRQ 0

#define RTLX_VERSION 2
#define RTLX_xID 0x12345600
#define RTLX_ID (RTLX_xID | RTLX_VERSION)
<<<<<<< HEAD
=======
#define RTLX_BUFFER_SIZE 2048
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define RTLX_CHANNELS 8

#define RTLX_CHANNEL_STDIO	0
#define RTLX_CHANNEL_DBG	1
#define RTLX_CHANNEL_SYSIO	2

<<<<<<< HEAD
extern int rtlx_open(int index, int can_sleep);
extern int rtlx_release(int index);
extern ssize_t rtlx_read(int index, void __user *buff, size_t count);
extern ssize_t rtlx_write(int index, const void __user *buffer, size_t count);
extern unsigned int rtlx_read_poll(int index, int can_sleep);
extern unsigned int rtlx_write_poll(int index);
=======
void rtlx_starting(int vpe);
void rtlx_stopping(int vpe);

int rtlx_open(int index, int can_sleep);
int rtlx_release(int index);
ssize_t rtlx_read(int index, void __user *buff, size_t count);
ssize_t rtlx_write(int index, const void __user *buffer, size_t count);
unsigned int rtlx_read_poll(int index, int can_sleep);
unsigned int rtlx_write_poll(int index);

int __init rtlx_module_init(void);
void __exit rtlx_module_exit(void);

void _interrupt_sp(void);

extern struct vpe_notifications rtlx_notify;
extern const struct file_operations rtlx_fops;
extern void (*aprp_hook)(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

enum rtlx_state {
	RTLX_STATE_UNUSED = 0,
	RTLX_STATE_INITIALISED,
	RTLX_STATE_REMOTE_READY,
	RTLX_STATE_OPENED
};

<<<<<<< HEAD
#define RTLX_BUFFER_SIZE 2048

/* each channel supports read and write.
   linux (vpe0) reads lx_buffer  and writes rt_buffer
=======
extern struct chan_waitqueues {
	wait_queue_head_t rt_queue;
	wait_queue_head_t lx_queue;
	atomic_t in_open;
	struct mutex mutex;
} channel_wqs[RTLX_CHANNELS];

/* each channel supports read and write.
   linux (vpe0) reads lx_buffer and writes rt_buffer
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
   SP (vpe1) reads rt_buffer and writes lx_buffer
*/
struct rtlx_channel {
	enum rtlx_state rt_state;
	enum rtlx_state lx_state;

	int buffer_size;

	/* read and write indexes per buffer */
	int rt_write, rt_read;
	char *rt_buffer;

	int lx_write, lx_read;
	char *lx_buffer;
};

<<<<<<< HEAD
struct rtlx_info {
=======
extern struct rtlx_info {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long id;
	enum rtlx_state state;

	struct rtlx_channel channel[RTLX_CHANNELS];
<<<<<<< HEAD
};

=======
} *rtlx;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __ASM_RTLX_H_ */
