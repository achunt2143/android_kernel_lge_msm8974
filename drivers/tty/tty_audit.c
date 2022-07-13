<<<<<<< HEAD
/*
 * Creating audit events from TTY input.
 *
 * Copyright (C) 2007 Red Hat, Inc.  All rights reserved.  This copyrighted
 * material is made available to anyone wishing to use, modify, copy, or
 * redistribute it subject to the terms and conditions of the GNU General
 * Public License v.2.
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Creating audit events from TTY input.
 *
 * Copyright (C) 2007 Red Hat, Inc.  All rights reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Authors: Miloslav Trmac <mitr@redhat.com>
 */

#include <linux/audit.h>
#include <linux/slab.h>
#include <linux/tty.h>
<<<<<<< HEAD

struct tty_audit_buf {
	atomic_t count;
	struct mutex mutex;	/* Protects all data below */
	int major, minor;	/* The TTY which the data is from */
	unsigned icanon:1;
	size_t valid;
	unsigned char *data;	/* Allocated size N_TTY_BUF_SIZE */
};

static struct tty_audit_buf *tty_audit_buf_alloc(int major, int minor,
						 int icanon)
{
	struct tty_audit_buf *buf;

	buf = kmalloc(sizeof(*buf), GFP_KERNEL);
	if (!buf)
		goto err;
	buf->data = kmalloc(N_TTY_BUF_SIZE, GFP_KERNEL);
	if (!buf->data)
		goto err_buf;
	atomic_set(&buf->count, 1);
	mutex_init(&buf->mutex);
	buf->major = major;
	buf->minor = minor;
	buf->icanon = icanon;
	buf->valid = 0;
=======
#include "tty.h"

struct tty_audit_buf {
	struct mutex mutex;	/* Protects all data below */
	dev_t dev;		/* The TTY which the data is from */
	bool icanon;
	size_t valid;
	u8 *data;		/* Allocated size N_TTY_BUF_SIZE */
};

static struct tty_audit_buf *tty_audit_buf_ref(void)
{
	struct tty_audit_buf *buf;

	buf = current->signal->tty_audit_buf;
	WARN_ON(buf == ERR_PTR(-ESRCH));
	return buf;
}

static struct tty_audit_buf *tty_audit_buf_alloc(void)
{
	struct tty_audit_buf *buf;

	buf = kzalloc(sizeof(*buf), GFP_KERNEL);
	if (!buf)
		goto err;

	buf->data = kmalloc(N_TTY_BUF_SIZE, GFP_KERNEL);
	if (!buf->data)
		goto err_buf;

	mutex_init(&buf->mutex);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return buf;

err_buf:
	kfree(buf);
err:
	return NULL;
}

static void tty_audit_buf_free(struct tty_audit_buf *buf)
{
	WARN_ON(buf->valid != 0);
	kfree(buf->data);
	kfree(buf);
}

<<<<<<< HEAD
static void tty_audit_buf_put(struct tty_audit_buf *buf)
{
	if (atomic_dec_and_test(&buf->count))
		tty_audit_buf_free(buf);
}

static void tty_audit_log(const char *description, struct task_struct *tsk,
			  uid_t loginuid, unsigned sessionid, int major,
			  int minor, unsigned char *data, size_t size)
{
	struct audit_buffer *ab;

	ab = audit_log_start(NULL, GFP_KERNEL, AUDIT_TTY);
	if (ab) {
		char name[sizeof(tsk->comm)];
		uid_t uid = task_uid(tsk);

		audit_log_format(ab, "%s pid=%u uid=%u auid=%u ses=%u "
				 "major=%d minor=%d comm=", description,
				 tsk->pid, uid, loginuid, sessionid,
				 major, minor);
		get_task_comm(name, tsk);
		audit_log_untrustedstring(ab, name);
		audit_log_format(ab, " data=");
		audit_log_n_hex(ab, data, size);
		audit_log_end(ab);
	}
}

/**
 *	tty_audit_buf_push	-	Push buffered data out
 *
 *	Generate an audit message from the contents of @buf, which is owned by
 *	@tsk with @loginuid.  @buf->mutex must be locked.
 */
static void tty_audit_buf_push(struct task_struct *tsk, uid_t loginuid,
			       unsigned int sessionid,
			       struct tty_audit_buf *buf)
{
	if (buf->valid == 0)
		return;
	if (audit_enabled == 0) {
		buf->valid = 0;
		return;
	}
	tty_audit_log("tty", tsk, loginuid, sessionid, buf->major, buf->minor,
		      buf->data, buf->valid);
	buf->valid = 0;
}

/**
 *	tty_audit_buf_push_current	-	Push buffered data out
 *
 *	Generate an audit message from the contents of @buf, which is owned by
 *	the current task.  @buf->mutex must be locked.
 */
static void tty_audit_buf_push_current(struct tty_audit_buf *buf)
{
	uid_t auid = audit_get_loginuid(current);
	unsigned int sessionid = audit_get_sessionid(current);
	tty_audit_buf_push(current, auid, sessionid, buf);
=======
static void tty_audit_log(const char *description, dev_t dev,
			  const u8 *data, size_t size)
{
	struct audit_buffer *ab;
	pid_t pid = task_pid_nr(current);
	uid_t uid = from_kuid(&init_user_ns, task_uid(current));
	uid_t loginuid = from_kuid(&init_user_ns, audit_get_loginuid(current));
	unsigned int sessionid = audit_get_sessionid(current);
	char name[TASK_COMM_LEN];

	ab = audit_log_start(audit_context(), GFP_KERNEL, AUDIT_TTY);
	if (!ab)
		return;

	audit_log_format(ab, "%s pid=%u uid=%u auid=%u ses=%u major=%d minor=%d comm=",
			 description, pid, uid, loginuid, sessionid,
			 MAJOR(dev), MINOR(dev));
	get_task_comm(name, current);
	audit_log_untrustedstring(ab, name);
	audit_log_format(ab, " data=");
	audit_log_n_hex(ab, data, size);
	audit_log_end(ab);
}

/*
 *	tty_audit_buf_push	-	Push buffered data out
 *
 *	Generate an audit message from the contents of @buf, which is owned by
 *	the current task.  @buf->mutex must be locked.
 */
static void tty_audit_buf_push(struct tty_audit_buf *buf)
{
	if (buf->valid == 0)
		return;
	if (audit_enabled == AUDIT_OFF) {
		buf->valid = 0;
		return;
	}
	tty_audit_log("tty", buf->dev, buf->data, buf->valid);
	buf->valid = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 *	tty_audit_exit	-	Handle a task exit
 *
 *	Make sure all buffered data is written out and deallocate the buffer.
 *	Only needs to be called if current->signal->tty_audit_buf != %NULL.
<<<<<<< HEAD
=======
 *
 *	The process is single-threaded at this point; no other threads share
 *	current->signal.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
void tty_audit_exit(void)
{
	struct tty_audit_buf *buf;

<<<<<<< HEAD
	spin_lock_irq(&current->sighand->siglock);
	buf = current->signal->tty_audit_buf;
	current->signal->tty_audit_buf = NULL;
	spin_unlock_irq(&current->sighand->siglock);
	if (!buf)
		return;

	mutex_lock(&buf->mutex);
	tty_audit_buf_push_current(buf);
	mutex_unlock(&buf->mutex);

	tty_audit_buf_put(buf);
}

/**
=======
	buf = xchg(&current->signal->tty_audit_buf, ERR_PTR(-ESRCH));
	if (!buf)
		return;

	tty_audit_buf_push(buf);
	tty_audit_buf_free(buf);
}

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	tty_audit_fork	-	Copy TTY audit state for a new task
 *
 *	Set up TTY audit state in @sig from current.  @sig needs no locking.
 */
void tty_audit_fork(struct signal_struct *sig)
{
<<<<<<< HEAD
	spin_lock_irq(&current->sighand->siglock);
	sig->audit_tty = current->signal->audit_tty;
	spin_unlock_irq(&current->sighand->siglock);
}

/**
 *	tty_audit_tiocsti	-	Log TIOCSTI
 */
void tty_audit_tiocsti(struct tty_struct *tty, char ch)
{
	struct tty_audit_buf *buf;
	int major, minor, should_audit;

	spin_lock_irq(&current->sighand->siglock);
	should_audit = current->signal->audit_tty;
	buf = current->signal->tty_audit_buf;
	if (buf)
		atomic_inc(&buf->count);
	spin_unlock_irq(&current->sighand->siglock);

	major = tty->driver->major;
	minor = tty->driver->minor_start + tty->index;
	if (buf) {
		mutex_lock(&buf->mutex);
		if (buf->major == major && buf->minor == minor)
			tty_audit_buf_push_current(buf);
		mutex_unlock(&buf->mutex);
		tty_audit_buf_put(buf);
	}

	if (should_audit && audit_enabled) {
		uid_t auid;
		unsigned int sessionid;

		auid = audit_get_loginuid(current);
		sessionid = audit_get_sessionid(current);
		tty_audit_log("ioctl=TIOCSTI", current, auid, sessionid, major,
			      minor, &ch, 1);
	}
}

/**
 * tty_audit_push_task	-	Flush task's pending audit data
 * @tsk:		task pointer
 * @loginuid:		sender login uid
 * @sessionid:		sender session id
 *
 * Called with a ref on @tsk held. Try to lock sighand and get a
 * reference to the tty audit buffer if available.
 * Flush the buffer or return an appropriate error code.
 */
int tty_audit_push_task(struct task_struct *tsk, uid_t loginuid, u32 sessionid)
{
	struct tty_audit_buf *buf = ERR_PTR(-EPERM);
	unsigned long flags;

	if (!lock_task_sighand(tsk, &flags))
		return -ESRCH;

	if (tsk->signal->audit_tty) {
		buf = tsk->signal->tty_audit_buf;
		if (buf)
			atomic_inc(&buf->count);
	}
	unlock_task_sighand(tsk, &flags);

	/*
	 * Return 0 when signal->audit_tty set
	 * but tsk->signal->tty_audit_buf == NULL.
	 */
	if (!buf || IS_ERR(buf))
		return PTR_ERR(buf);

	mutex_lock(&buf->mutex);
	tty_audit_buf_push(tsk, loginuid, sessionid, buf);
	mutex_unlock(&buf->mutex);

	tty_audit_buf_put(buf);
	return 0;
}

/**
 *	tty_audit_buf_get	-	Get an audit buffer.
 *
 *	Get an audit buffer for @tty, allocate it if necessary.  Return %NULL
 *	if TTY auditing is disabled or out of memory.  Otherwise, return a new
 *	reference to the buffer.
 */
static struct tty_audit_buf *tty_audit_buf_get(struct tty_struct *tty)
{
	struct tty_audit_buf *buf, *buf2;

	buf = NULL;
	buf2 = NULL;
	spin_lock_irq(&current->sighand->siglock);
	if (likely(!current->signal->audit_tty))
		goto out;
	buf = current->signal->tty_audit_buf;
	if (buf) {
		atomic_inc(&buf->count);
		goto out;
	}
	spin_unlock_irq(&current->sighand->siglock);

	buf2 = tty_audit_buf_alloc(tty->driver->major,
				   tty->driver->minor_start + tty->index,
				   tty->icanon);
	if (buf2 == NULL) {
=======
	sig->audit_tty = current->signal->audit_tty;
}

/*
 *	tty_audit_tiocsti	-	Log TIOCSTI
 */
void tty_audit_tiocsti(const struct tty_struct *tty, u8 ch)
{
	dev_t dev;

	dev = MKDEV(tty->driver->major, tty->driver->minor_start) + tty->index;
	if (tty_audit_push())
		return;

	if (audit_enabled)
		tty_audit_log("ioctl=TIOCSTI", dev, &ch, 1);
}

/*
 *	tty_audit_push	-	Flush current's pending audit data
 *
 *	Returns 0 if success, -EPERM if tty audit is disabled
 */
int tty_audit_push(void)
{
	struct tty_audit_buf *buf;

	if (~current->signal->audit_tty & AUDIT_TTY_ENABLE)
		return -EPERM;

	buf = tty_audit_buf_ref();
	if (!IS_ERR_OR_NULL(buf)) {
		mutex_lock(&buf->mutex);
		tty_audit_buf_push(buf);
		mutex_unlock(&buf->mutex);
	}
	return 0;
}

/*
 *	tty_audit_buf_get	-	Get an audit buffer.
 *
 *	Get an audit buffer, allocate it if necessary.  Return %NULL
 *	if out of memory or ERR_PTR(-ESRCH) if tty_audit_exit() has already
 *	occurred.  Otherwise, return a new reference to the buffer.
 */
static struct tty_audit_buf *tty_audit_buf_get(void)
{
	struct tty_audit_buf *buf;

	buf = tty_audit_buf_ref();
	if (buf)
		return buf;

	buf = tty_audit_buf_alloc();
	if (buf == NULL) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		audit_log_lost("out of memory in TTY auditing");
		return NULL;
	}

<<<<<<< HEAD
	spin_lock_irq(&current->sighand->siglock);
	if (!current->signal->audit_tty)
		goto out;
	buf = current->signal->tty_audit_buf;
	if (!buf) {
		current->signal->tty_audit_buf = buf2;
		buf = buf2;
		buf2 = NULL;
	}
	atomic_inc(&buf->count);
	/* Fall through */
 out:
	spin_unlock_irq(&current->sighand->siglock);
	if (buf2)
		tty_audit_buf_free(buf2);
	return buf;
}

/**
=======
	/* Race to use this buffer, free it if another wins */
	if (cmpxchg(&current->signal->tty_audit_buf, NULL, buf) != NULL)
		tty_audit_buf_free(buf);
	return tty_audit_buf_ref();
}

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	tty_audit_add_data	-	Add data for TTY auditing.
 *
 *	Audit @data of @size from @tty, if necessary.
 */
<<<<<<< HEAD
void tty_audit_add_data(struct tty_struct *tty, unsigned char *data,
			size_t size)
{
	struct tty_audit_buf *buf;
	int major, minor;
=======
void tty_audit_add_data(const struct tty_struct *tty, const void *data,
			size_t size)
{
	struct tty_audit_buf *buf;
	unsigned int audit_tty;
	bool icanon = L_ICANON(tty);
	dev_t dev;

	audit_tty = READ_ONCE(current->signal->audit_tty);
	if (~audit_tty & AUDIT_TTY_ENABLE)
		return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (unlikely(size == 0))
		return;

	if (tty->driver->type == TTY_DRIVER_TYPE_PTY
	    && tty->driver->subtype == PTY_TYPE_MASTER)
		return;

<<<<<<< HEAD
	buf = tty_audit_buf_get(tty);
	if (!buf)
		return;

	mutex_lock(&buf->mutex);
	major = tty->driver->major;
	minor = tty->driver->minor_start + tty->index;
	if (buf->major != major || buf->minor != minor
	    || buf->icanon != tty->icanon) {
		tty_audit_buf_push_current(buf);
		buf->major = major;
		buf->minor = minor;
		buf->icanon = tty->icanon;
=======
	if ((~audit_tty & AUDIT_TTY_LOG_PASSWD) && icanon && !L_ECHO(tty))
		return;

	buf = tty_audit_buf_get();
	if (IS_ERR_OR_NULL(buf))
		return;

	mutex_lock(&buf->mutex);
	dev = MKDEV(tty->driver->major, tty->driver->minor_start) + tty->index;
	if (buf->dev != dev || buf->icanon != icanon) {
		tty_audit_buf_push(buf);
		buf->dev = dev;
		buf->icanon = icanon;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	do {
		size_t run;

		run = N_TTY_BUF_SIZE - buf->valid;
		if (run > size)
			run = size;
		memcpy(buf->data + buf->valid, data, run);
		buf->valid += run;
		data += run;
		size -= run;
		if (buf->valid == N_TTY_BUF_SIZE)
<<<<<<< HEAD
			tty_audit_buf_push_current(buf);
	} while (size != 0);
	mutex_unlock(&buf->mutex);
	tty_audit_buf_put(buf);
}

/**
 *	tty_audit_push	-	Push buffered data out
 *
 *	Make sure no audit data is pending for @tty on the current process.
 */
void tty_audit_push(struct tty_struct *tty)
{
	struct tty_audit_buf *buf;

	spin_lock_irq(&current->sighand->siglock);
	if (likely(!current->signal->audit_tty)) {
		spin_unlock_irq(&current->sighand->siglock);
		return;
	}
	buf = current->signal->tty_audit_buf;
	if (buf)
		atomic_inc(&buf->count);
	spin_unlock_irq(&current->sighand->siglock);

	if (buf) {
		int major, minor;

		major = tty->driver->major;
		minor = tty->driver->minor_start + tty->index;
		mutex_lock(&buf->mutex);
		if (buf->major == major && buf->minor == minor)
			tty_audit_buf_push_current(buf);
		mutex_unlock(&buf->mutex);
		tty_audit_buf_put(buf);
	}
=======
			tty_audit_buf_push(buf);
	} while (size != 0);
	mutex_unlock(&buf->mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
