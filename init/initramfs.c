<<<<<<< HEAD
#include <linux/init.h>
=======
// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>
#include <linux/async.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/dirent.h>
#include <linux/syscalls.h>
#include <linux/utime.h>
<<<<<<< HEAD
=======
#include <linux/file.h>
#include <linux/kstrtox.h>
#include <linux/memblock.h>
#include <linux/mm.h>
#include <linux/namei.h>
#include <linux/init_syscalls.h>
#include <linux/umh.h>

#include "do_mounts.h"

static __initdata bool csum_present;
static __initdata u32 io_csum;

static ssize_t __init xwrite(struct file *file, const unsigned char *p,
		size_t count, loff_t *pos)
{
	ssize_t out = 0;

	/* sys_write only can write MAX_RW_COUNT aka 2G-4K bytes at most */
	while (count) {
		ssize_t rv = kernel_write(file, p, count, pos);

		if (rv < 0) {
			if (rv == -EINTR || rv == -EAGAIN)
				continue;
			return out ? out : rv;
		} else if (rv == 0)
			break;

		if (csum_present) {
			ssize_t i;

			for (i = 0; i < rv; i++)
				io_csum += p[i];
		}

		p += rv;
		out += rv;
		count -= rv;
	}

	return out;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static __initdata char *message;
static void __init error(char *x)
{
	if (!message)
		message = x;
}

<<<<<<< HEAD
=======
#define panic_show_mem(fmt, ...) \
	({ show_mem(); panic(fmt, ##__VA_ARGS__); })

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* link hash */

#define N_ALIGN(len) ((((len) + 1) & ~3) + 2)

static __initdata struct hash {
	int ino, minor, major;
	umode_t mode;
	struct hash *next;
	char name[N_ALIGN(PATH_MAX)];
} *head[32];

static inline int hash(int major, int minor, int ino)
{
	unsigned long tmp = ino + minor + (major << 3);
	tmp += tmp >> 5;
	return tmp & 31;
}

static char __init *find_link(int major, int minor, int ino,
			      umode_t mode, char *name)
{
	struct hash **p, *q;
	for (p = head + hash(major, minor, ino); *p; p = &(*p)->next) {
		if ((*p)->ino != ino)
			continue;
		if ((*p)->minor != minor)
			continue;
		if ((*p)->major != major)
			continue;
		if (((*p)->mode ^ mode) & S_IFMT)
			continue;
		return (*p)->name;
	}
	q = kmalloc(sizeof(struct hash), GFP_KERNEL);
	if (!q)
<<<<<<< HEAD
		panic("can't allocate link hash entry");
=======
		panic_show_mem("can't allocate link hash entry");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	q->major = major;
	q->minor = minor;
	q->ino = ino;
	q->mode = mode;
	strcpy(q->name, name);
	q->next = NULL;
	*p = q;
	return NULL;
}

static void __init free_hash(void)
{
	struct hash **p, *q;
	for (p = head; p < head + 32; p++) {
		while (*p) {
			q = *p;
			*p = q->next;
			kfree(q);
		}
	}
}

<<<<<<< HEAD
static long __init do_utime(char __user *filename, time_t mtime)
{
	struct timespec t[2];

	t[0].tv_sec = mtime;
	t[0].tv_nsec = 0;
	t[1].tv_sec = mtime;
	t[1].tv_nsec = 0;

	return do_utimes(AT_FDCWD, filename, t, AT_SYMLINK_NOFOLLOW);
=======
#ifdef CONFIG_INITRAMFS_PRESERVE_MTIME
static void __init do_utime(char *filename, time64_t mtime)
{
	struct timespec64 t[2] = { { .tv_sec = mtime }, { .tv_sec = mtime } };
	init_utimes(filename, t);
}

static void __init do_utime_path(const struct path *path, time64_t mtime)
{
	struct timespec64 t[2] = { { .tv_sec = mtime }, { .tv_sec = mtime } };
	vfs_utimes(path, t);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static __initdata LIST_HEAD(dir_list);
struct dir_entry {
	struct list_head list;
<<<<<<< HEAD
	char *name;
	time_t mtime;
};

static void __init dir_add(const char *name, time_t mtime)
{
	struct dir_entry *de = kmalloc(sizeof(struct dir_entry), GFP_KERNEL);
	if (!de)
		panic("can't allocate dir_entry buffer");
	INIT_LIST_HEAD(&de->list);
	de->name = kstrdup(name, GFP_KERNEL);
=======
	time64_t mtime;
	char name[];
};

static void __init dir_add(const char *name, time64_t mtime)
{
	size_t nlen = strlen(name) + 1;
	struct dir_entry *de;

	de = kmalloc(sizeof(struct dir_entry) + nlen, GFP_KERNEL);
	if (!de)
		panic_show_mem("can't allocate dir_entry buffer");
	INIT_LIST_HEAD(&de->list);
	strscpy(de->name, name, nlen);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	de->mtime = mtime;
	list_add(&de->list, &dir_list);
}

static void __init dir_utime(void)
{
	struct dir_entry *de, *tmp;
	list_for_each_entry_safe(de, tmp, &dir_list, list) {
		list_del(&de->list);
		do_utime(de->name, de->mtime);
<<<<<<< HEAD
		kfree(de->name);
		kfree(de);
	}
}

static __initdata time_t mtime;
=======
		kfree(de);
	}
}
#else
static void __init do_utime(char *filename, time64_t mtime) {}
static void __init do_utime_path(const struct path *path, time64_t mtime) {}
static void __init dir_add(const char *name, time64_t mtime) {}
static void __init dir_utime(void) {}
#endif

static __initdata time64_t mtime;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* cpio header parsing */

static __initdata unsigned long ino, major, minor, nlink;
static __initdata umode_t mode;
static __initdata unsigned long body_len, name_len;
static __initdata uid_t uid;
static __initdata gid_t gid;
static __initdata unsigned rdev;
<<<<<<< HEAD

static void __init parse_header(char *s)
{
	unsigned long parsed[12];
=======
static __initdata u32 hdr_csum;

static void __init parse_header(char *s)
{
	unsigned long parsed[13];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char buf[9];
	int i;

	buf[8] = '\0';
<<<<<<< HEAD
	for (i = 0, s += 6; i < 12; i++, s += 8) {
=======
	for (i = 0, s += 6; i < 13; i++, s += 8) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		memcpy(buf, s, 8);
		parsed[i] = simple_strtoul(buf, NULL, 16);
	}
	ino = parsed[0];
	mode = parsed[1];
	uid = parsed[2];
	gid = parsed[3];
	nlink = parsed[4];
<<<<<<< HEAD
	mtime = parsed[5];
=======
	mtime = parsed[5]; /* breaks in y2106 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	body_len = parsed[6];
	major = parsed[7];
	minor = parsed[8];
	rdev = new_encode_dev(MKDEV(parsed[9], parsed[10]));
	name_len = parsed[11];
<<<<<<< HEAD
=======
	hdr_csum = parsed[12];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* FSM */

static __initdata enum state {
	Start,
	Collect,
	GotHeader,
	SkipIt,
	GotName,
	CopyFile,
	GotSymlink,
	Reset
} state, next_state;

static __initdata char *victim;
<<<<<<< HEAD
static __initdata unsigned count;
=======
static unsigned long byte_count __initdata;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static __initdata loff_t this_header, next_header;

static inline void __init eat(unsigned n)
{
	victim += n;
	this_header += n;
<<<<<<< HEAD
	count -= n;
}

static __initdata char *vcollected;
static __initdata char *collected;
static __initdata int remains;
=======
	byte_count -= n;
}

static __initdata char *collected;
static long remains __initdata;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static __initdata char *collect;

static void __init read_into(char *buf, unsigned size, enum state next)
{
<<<<<<< HEAD
	if (count >= size) {
=======
	if (byte_count >= size) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		collected = victim;
		eat(size);
		state = next;
	} else {
		collect = collected = buf;
		remains = size;
		next_state = next;
		state = Collect;
	}
}

static __initdata char *header_buf, *symlink_buf, *name_buf;

static int __init do_start(void)
{
	read_into(header_buf, 110, GotHeader);
	return 0;
}

static int __init do_collect(void)
{
<<<<<<< HEAD
	unsigned n = remains;
	if (count < n)
		n = count;
=======
	unsigned long n = remains;
	if (byte_count < n)
		n = byte_count;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	memcpy(collect, victim, n);
	eat(n);
	collect += n;
	if ((remains -= n) != 0)
		return 1;
	state = next_state;
	return 0;
}

static int __init do_header(void)
{
<<<<<<< HEAD
	if (memcmp(collected, "070707", 6)==0) {
		error("incorrect cpio method used: use -H newc option");
		return 1;
	}
	if (memcmp(collected, "070701", 6)) {
		error("no cpio magic");
=======
	if (!memcmp(collected, "070701", 6)) {
		csum_present = false;
	} else if (!memcmp(collected, "070702", 6)) {
		csum_present = true;
	} else {
		if (memcmp(collected, "070707", 6) == 0)
			error("incorrect cpio method used: use -H newc option");
		else
			error("no cpio magic");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 1;
	}
	parse_header(collected);
	next_header = this_header + N_ALIGN(name_len) + body_len;
	next_header = (next_header + 3) & ~3;
	state = SkipIt;
	if (name_len <= 0 || name_len > PATH_MAX)
		return 0;
	if (S_ISLNK(mode)) {
		if (body_len > PATH_MAX)
			return 0;
		collect = collected = symlink_buf;
		remains = N_ALIGN(name_len) + body_len;
		next_state = GotSymlink;
		state = Collect;
		return 0;
	}
	if (S_ISREG(mode) || !body_len)
		read_into(name_buf, N_ALIGN(name_len), GotName);
	return 0;
}

static int __init do_skip(void)
{
<<<<<<< HEAD
	if (this_header + count < next_header) {
		eat(count);
=======
	if (this_header + byte_count < next_header) {
		eat(byte_count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 1;
	} else {
		eat(next_header - this_header);
		state = next_state;
		return 0;
	}
}

static int __init do_reset(void)
{
<<<<<<< HEAD
	while(count && *victim == '\0')
		eat(1);
	if (count && (this_header & 3))
=======
	while (byte_count && *victim == '\0')
		eat(1);
	if (byte_count && (this_header & 3))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		error("broken padding");
	return 1;
}

<<<<<<< HEAD
=======
static void __init clean_path(char *path, umode_t fmode)
{
	struct kstat st;

	if (!init_stat(path, &st, AT_SYMLINK_NOFOLLOW) &&
	    (st.mode ^ fmode) & S_IFMT) {
		if (S_ISDIR(st.mode))
			init_rmdir(path);
		else
			init_unlink(path);
	}
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int __init maybe_link(void)
{
	if (nlink >= 2) {
		char *old = find_link(major, minor, ino, mode, collected);
<<<<<<< HEAD
		if (old)
			return (sys_link(old, collected) < 0) ? -1 : 1;
=======
		if (old) {
			clean_path(collected, 0);
			return (init_link(old, collected) < 0) ? -1 : 1;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return 0;
}

<<<<<<< HEAD
static void __init clean_path(char *path, umode_t mode)
{
	struct stat st;

	if (!sys_newlstat(path, &st) && (st.st_mode^mode) & S_IFMT) {
		if (S_ISDIR(st.st_mode))
			sys_rmdir(path);
		else
			sys_unlink(path);
	}
}

static __initdata int wfd;
=======
static __initdata struct file *wfile;
static __initdata loff_t wfile_pos;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int __init do_name(void)
{
	state = SkipIt;
	next_state = Reset;
	if (strcmp(collected, "TRAILER!!!") == 0) {
		free_hash();
		return 0;
	}
	clean_path(collected, mode);
	if (S_ISREG(mode)) {
		int ml = maybe_link();
		if (ml >= 0) {
<<<<<<< HEAD
			int openflags = O_WRONLY|O_CREAT;
			if (ml != 1)
				openflags |= O_TRUNC;
			wfd = sys_open(collected, openflags, mode);

			if (wfd >= 0) {
				sys_fchown(wfd, uid, gid);
				sys_fchmod(wfd, mode);
				if (body_len)
					sys_ftruncate(wfd, body_len);
				vcollected = kstrdup(collected, GFP_KERNEL);
				state = CopyFile;
			}
		}
	} else if (S_ISDIR(mode)) {
		sys_mkdir(collected, mode);
		sys_chown(collected, uid, gid);
		sys_chmod(collected, mode);
=======
			int openflags = O_WRONLY|O_CREAT|O_LARGEFILE;
			if (ml != 1)
				openflags |= O_TRUNC;
			wfile = filp_open(collected, openflags, mode);
			if (IS_ERR(wfile))
				return 0;
			wfile_pos = 0;
			io_csum = 0;

			vfs_fchown(wfile, uid, gid);
			vfs_fchmod(wfile, mode);
			if (body_len)
				vfs_truncate(&wfile->f_path, body_len);
			state = CopyFile;
		}
	} else if (S_ISDIR(mode)) {
		init_mkdir(collected, mode);
		init_chown(collected, uid, gid, 0);
		init_chmod(collected, mode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dir_add(collected, mtime);
	} else if (S_ISBLK(mode) || S_ISCHR(mode) ||
		   S_ISFIFO(mode) || S_ISSOCK(mode)) {
		if (maybe_link() == 0) {
<<<<<<< HEAD
			sys_mknod(collected, mode, rdev);
			sys_chown(collected, uid, gid);
			sys_chmod(collected, mode);
=======
			init_mknod(collected, mode, rdev);
			init_chown(collected, uid, gid, 0);
			init_chmod(collected, mode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			do_utime(collected, mtime);
		}
	}
	return 0;
}

static int __init do_copy(void)
{
<<<<<<< HEAD
	if (count >= body_len) {
		sys_write(wfd, victim, body_len);
		sys_close(wfd);
		do_utime(vcollected, mtime);
		kfree(vcollected);
=======
	if (byte_count >= body_len) {
		if (xwrite(wfile, victim, body_len, &wfile_pos) != body_len)
			error("write error");

		do_utime_path(&wfile->f_path, mtime);
		fput(wfile);
		if (csum_present && io_csum != hdr_csum)
			error("bad data checksum");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		eat(body_len);
		state = SkipIt;
		return 0;
	} else {
<<<<<<< HEAD
		sys_write(wfd, victim, count);
		body_len -= count;
		eat(count);
=======
		if (xwrite(wfile, victim, byte_count, &wfile_pos) != byte_count)
			error("write error");
		body_len -= byte_count;
		eat(byte_count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 1;
	}
}

static int __init do_symlink(void)
{
	collected[N_ALIGN(name_len) + body_len] = '\0';
	clean_path(collected, 0);
<<<<<<< HEAD
	sys_symlink(collected + N_ALIGN(name_len), collected);
	sys_lchown(collected, uid, gid);
=======
	init_symlink(collected + N_ALIGN(name_len), collected);
	init_chown(collected, uid, gid, AT_SYMLINK_NOFOLLOW);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	do_utime(collected, mtime);
	state = SkipIt;
	next_state = Reset;
	return 0;
}

static __initdata int (*actions[])(void) = {
	[Start]		= do_start,
	[Collect]	= do_collect,
	[GotHeader]	= do_header,
	[SkipIt]	= do_skip,
	[GotName]	= do_name,
	[CopyFile]	= do_copy,
	[GotSymlink]	= do_symlink,
	[Reset]		= do_reset,
};

<<<<<<< HEAD
static int __init write_buffer(char *buf, unsigned len)
{
	count = len;
=======
static long __init write_buffer(char *buf, unsigned long len)
{
	byte_count = len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	victim = buf;

	while (!actions[state]())
		;
<<<<<<< HEAD
	return len - count;
}

static int __init flush_buffer(void *bufv, unsigned len)
{
	char *buf = (char *) bufv;
	int written;
	int origLen = len;
=======
	return len - byte_count;
}

static long __init flush_buffer(void *bufv, unsigned long len)
{
	char *buf = bufv;
	long written;
	long origLen = len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (message)
		return -1;
	while ((written = write_buffer(buf, len)) < len && !message) {
		char c = buf[written];
		if (c == '0') {
			buf += written;
			len -= written;
			state = Start;
		} else if (c == 0) {
			buf += written;
			len -= written;
			state = Reset;
		} else
<<<<<<< HEAD
			error("junk in compressed archive");
=======
			error("junk within compressed archive");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return origLen;
}

<<<<<<< HEAD
static unsigned my_inptr;   /* index of next byte to be processed in inbuf */

#include <linux/decompress/generic.h>

static char * __init unpack_to_rootfs(char *buf, unsigned len)
{
	int written, res;
=======
static unsigned long my_inptr __initdata; /* index of next byte to be processed in inbuf */

#include <linux/decompress/generic.h>

static char * __init unpack_to_rootfs(char *buf, unsigned long len)
{
	long written;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	decompress_fn decompress;
	const char *compress_name;
	static __initdata char msg_buf[64];

	header_buf = kmalloc(110, GFP_KERNEL);
	symlink_buf = kmalloc(PATH_MAX + N_ALIGN(PATH_MAX) + 1, GFP_KERNEL);
	name_buf = kmalloc(N_ALIGN(PATH_MAX), GFP_KERNEL);

	if (!header_buf || !symlink_buf || !name_buf)
<<<<<<< HEAD
		panic("can't allocate buffers");
=======
		panic_show_mem("can't allocate buffers");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	state = Start;
	this_header = 0;
	message = NULL;
	while (!message && len) {
		loff_t saved_offset = this_header;
		if (*buf == '0' && !(this_header & 3)) {
			state = Start;
			written = write_buffer(buf, len);
			buf += written;
			len -= written;
			continue;
		}
		if (!*buf) {
			buf++;
			len--;
			this_header++;
			continue;
		}
		this_header = 0;
		decompress = decompress_method(buf, len, &compress_name);
<<<<<<< HEAD
		if (decompress) {
			res = decompress(buf, len, NULL, flush_buffer, NULL,
=======
		pr_debug("Detected %s compressed data\n", compress_name);
		if (decompress) {
			int res = decompress(buf, len, NULL, flush_buffer, NULL,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				   &my_inptr, error);
			if (res)
				error("decompressor failed");
		} else if (compress_name) {
			if (!message) {
				snprintf(msg_buf, sizeof msg_buf,
					 "compression method %s not configured",
					 compress_name);
				message = msg_buf;
			}
		} else
<<<<<<< HEAD
			error("junk in compressed archive");
		if (state != Reset)
			error("junk in compressed archive");
=======
			error("invalid magic at start of compressed archive");
		if (state != Reset)
			error("junk at the end of compressed archive");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		this_header = saved_offset + my_inptr;
		buf += my_inptr;
		len -= my_inptr;
	}
	dir_utime();
	kfree(name_buf);
	kfree(symlink_buf);
	kfree(header_buf);
	return message;
}

static int __initdata do_retain_initrd;

static int __init retain_initrd_param(char *str)
{
	if (*str)
		return 0;
	do_retain_initrd = 1;
	return 1;
}
__setup("retain_initrd", retain_initrd_param);

<<<<<<< HEAD
=======
#ifdef CONFIG_ARCH_HAS_KEEPINITRD
static int __init keepinitrd_setup(char *__unused)
{
	do_retain_initrd = 1;
	return 1;
}
__setup("keepinitrd", keepinitrd_setup);
#endif

static bool __initdata initramfs_async = true;
static int __init initramfs_async_setup(char *str)
{
	return kstrtobool(str, &initramfs_async) == 0;
}
__setup("initramfs_async=", initramfs_async_setup);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern char __initramfs_start[];
extern unsigned long __initramfs_size;
#include <linux/initrd.h>
#include <linux/kexec.h>

<<<<<<< HEAD
static void __init free_initrd(void)
{
#ifdef CONFIG_KEXEC
	unsigned long crashk_start = (unsigned long)__va(crashk_res.start);
	unsigned long crashk_end   = (unsigned long)__va(crashk_res.end);
#endif
	if (do_retain_initrd)
		goto skip;

#ifdef CONFIG_KEXEC
	/*
	 * If the initrd region is overlapped with crashkernel reserved region,
	 * free only memory that is not part of crashkernel region.
	 */
	if (initrd_start < crashk_end && initrd_end > crashk_start) {
		/*
		 * Initialize initrd memory region since the kexec boot does
		 * not do.
		 */
		memset((void *)initrd_start, 0, initrd_end - initrd_start);
		if (initrd_start < crashk_start)
			free_initrd_mem(initrd_start, crashk_start);
		if (initrd_end > crashk_end)
			free_initrd_mem(crashk_end, initrd_end);
	} else
#endif
		free_initrd_mem(initrd_start, initrd_end);
skip:
=======
static ssize_t raw_read(struct file *file, struct kobject *kobj,
			struct bin_attribute *attr, char *buf,
			loff_t pos, size_t count)
{
	memcpy(buf, attr->private + pos, count);
	return count;
}

static BIN_ATTR(initrd, 0440, raw_read, NULL, 0);

void __init reserve_initrd_mem(void)
{
	phys_addr_t start;
	unsigned long size;

	/* Ignore the virtul address computed during device tree parsing */
	initrd_start = initrd_end = 0;

	if (!phys_initrd_size)
		return;
	/*
	 * Round the memory region to page boundaries as per free_initrd_mem()
	 * This allows us to detect whether the pages overlapping the initrd
	 * are in use, but more importantly, reserves the entire set of pages
	 * as we don't want these pages allocated for other purposes.
	 */
	start = round_down(phys_initrd_start, PAGE_SIZE);
	size = phys_initrd_size + (phys_initrd_start - start);
	size = round_up(size, PAGE_SIZE);

	if (!memblock_is_region_memory(start, size)) {
		pr_err("INITRD: 0x%08llx+0x%08lx is not a memory region",
		       (u64)start, size);
		goto disable;
	}

	if (memblock_is_region_reserved(start, size)) {
		pr_err("INITRD: 0x%08llx+0x%08lx overlaps in-use memory region\n",
		       (u64)start, size);
		goto disable;
	}

	memblock_reserve(start, size);
	/* Now convert initrd to virtual addresses */
	initrd_start = (unsigned long)__va(phys_initrd_start);
	initrd_end = initrd_start + phys_initrd_size;
	initrd_below_start_ok = 1;

	return;
disable:
	pr_cont(" - disabling initrd\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	initrd_start = 0;
	initrd_end = 0;
}

<<<<<<< HEAD
#ifdef CONFIG_BLK_DEV_RAM
#define BUF_SIZE 1024
static void __init clean_rootfs(void)
{
	int fd;
	void *buf;
	struct linux_dirent64 *dirp;
	int num;

	fd = sys_open((const char __user __force *) "/", O_RDONLY, 0);
	WARN_ON(fd < 0);
	if (fd < 0)
		return;
	buf = kzalloc(BUF_SIZE, GFP_KERNEL);
	WARN_ON(!buf);
	if (!buf) {
		sys_close(fd);
		return;
	}

	dirp = buf;
	num = sys_getdents64(fd, dirp, BUF_SIZE);
	while (num > 0) {
		while (num > 0) {
			struct stat st;
			int ret;

			ret = sys_newlstat(dirp->d_name, &st);
			WARN_ON_ONCE(ret);
			if (!ret) {
				if (S_ISDIR(st.st_mode))
					sys_rmdir(dirp->d_name);
				else
					sys_unlink(dirp->d_name);
			}

			num -= dirp->d_reclen;
			dirp = (void *)dirp + dirp->d_reclen;
		}
		dirp = buf;
		memset(buf, 0, BUF_SIZE);
		num = sys_getdents64(fd, dirp, BUF_SIZE);
	}

	sys_close(fd);
	kfree(buf);
}
#endif

static int __init populate_rootfs(void)
{
	char *err = unpack_to_rootfs(__initramfs_start, __initramfs_size);
	if (err)
		panic(err);	/* Failed to decompress INTERNAL initramfs */
	if (initrd_start) {
#ifdef CONFIG_BLK_DEV_RAM
		int fd;
		printk(KERN_INFO "Trying to unpack rootfs image as initramfs...\n");
		err = unpack_to_rootfs((char *)initrd_start,
			initrd_end - initrd_start);
		if (!err) {
			free_initrd();
			return 0;
		} else {
			clean_rootfs();
			unpack_to_rootfs(__initramfs_start, __initramfs_size);
		}
		printk(KERN_INFO "rootfs image is not initramfs (%s)"
				"; looks like an initrd\n", err);
		fd = sys_open((const char __user __force *) "/initrd.image",
			      O_WRONLY|O_CREAT, 0700);
		if (fd >= 0) {
			sys_write(fd, (char *)initrd_start,
					initrd_end - initrd_start);
			sys_close(fd);
			free_initrd();
		}
#else
		printk(KERN_INFO "Unpacking initramfs...\n");
		err = unpack_to_rootfs((char *)initrd_start,
			initrd_end - initrd_start);
		if (err)
			printk(KERN_EMERG "Initramfs unpacking failed: %s\n", err);
		free_initrd();
#endif
	}
=======
void __weak __init free_initrd_mem(unsigned long start, unsigned long end)
{
#ifdef CONFIG_ARCH_KEEP_MEMBLOCK
	unsigned long aligned_start = ALIGN_DOWN(start, PAGE_SIZE);
	unsigned long aligned_end = ALIGN(end, PAGE_SIZE);

	memblock_free((void *)aligned_start, aligned_end - aligned_start);
#endif

	free_reserved_area((void *)start, (void *)end, POISON_FREE_INITMEM,
			"initrd");
}

#ifdef CONFIG_CRASH_RESERVE
static bool __init kexec_free_initrd(void)
{
	unsigned long crashk_start = (unsigned long)__va(crashk_res.start);
	unsigned long crashk_end   = (unsigned long)__va(crashk_res.end);

	/*
	 * If the initrd region is overlapped with crashkernel reserved region,
	 * free only memory that is not part of crashkernel region.
	 */
	if (initrd_start >= crashk_end || initrd_end <= crashk_start)
		return false;

	/*
	 * Initialize initrd memory region since the kexec boot does not do.
	 */
	memset((void *)initrd_start, 0, initrd_end - initrd_start);
	if (initrd_start < crashk_start)
		free_initrd_mem(initrd_start, crashk_start);
	if (initrd_end > crashk_end)
		free_initrd_mem(crashk_end, initrd_end);
	return true;
}
#else
static inline bool kexec_free_initrd(void)
{
	return false;
}
#endif /* CONFIG_KEXEC_CORE */

#ifdef CONFIG_BLK_DEV_RAM
static void __init populate_initrd_image(char *err)
{
	ssize_t written;
	struct file *file;
	loff_t pos = 0;

	printk(KERN_INFO "rootfs image is not initramfs (%s); looks like an initrd\n",
			err);
	file = filp_open("/initrd.image", O_WRONLY|O_CREAT|O_LARGEFILE, 0700);
	if (IS_ERR(file))
		return;

	written = xwrite(file, (char *)initrd_start, initrd_end - initrd_start,
			&pos);
	if (written != initrd_end - initrd_start)
		pr_err("/initrd.image: incomplete write (%zd != %ld)\n",
		       written, initrd_end - initrd_start);
	fput(file);
}
#endif /* CONFIG_BLK_DEV_RAM */

static void __init do_populate_rootfs(void *unused, async_cookie_t cookie)
{
	/* Load the built in initramfs */
	char *err = unpack_to_rootfs(__initramfs_start, __initramfs_size);
	if (err)
		panic_show_mem("%s", err); /* Failed to decompress INTERNAL initramfs */

	if (!initrd_start || IS_ENABLED(CONFIG_INITRAMFS_FORCE))
		goto done;

	if (IS_ENABLED(CONFIG_BLK_DEV_RAM))
		printk(KERN_INFO "Trying to unpack rootfs image as initramfs...\n");
	else
		printk(KERN_INFO "Unpacking initramfs...\n");

	err = unpack_to_rootfs((char *)initrd_start, initrd_end - initrd_start);
	if (err) {
#ifdef CONFIG_BLK_DEV_RAM
		populate_initrd_image(err);
#else
		printk(KERN_EMERG "Initramfs unpacking failed: %s\n", err);
#endif
	}

done:
	/*
	 * If the initrd region is overlapped with crashkernel reserved region,
	 * free only memory that is not part of crashkernel region.
	 */
	if (!do_retain_initrd && initrd_start && !kexec_free_initrd()) {
		free_initrd_mem(initrd_start, initrd_end);
	} else if (do_retain_initrd && initrd_start) {
		bin_attr_initrd.size = initrd_end - initrd_start;
		bin_attr_initrd.private = (void *)initrd_start;
		if (sysfs_create_bin_file(firmware_kobj, &bin_attr_initrd))
			pr_err("Failed to create initrd sysfs file");
	}
	initrd_start = 0;
	initrd_end = 0;

	init_flush_fput();
}

static ASYNC_DOMAIN_EXCLUSIVE(initramfs_domain);
static async_cookie_t initramfs_cookie;

void wait_for_initramfs(void)
{
	if (!initramfs_cookie) {
		/*
		 * Something before rootfs_initcall wants to access
		 * the filesystem/initramfs. Probably a bug. Make a
		 * note, avoid deadlocking the machine, and let the
		 * caller's access fail as it used to.
		 */
		pr_warn_once("wait_for_initramfs() called before rootfs_initcalls\n");
		return;
	}
	async_synchronize_cookie_domain(initramfs_cookie + 1, &initramfs_domain);
}
EXPORT_SYMBOL_GPL(wait_for_initramfs);

static int __init populate_rootfs(void)
{
	initramfs_cookie = async_schedule_domain(do_populate_rootfs, NULL,
						 &initramfs_domain);
	usermodehelper_enable();
	if (!initramfs_async)
		wait_for_initramfs();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
rootfs_initcall(populate_rootfs);
