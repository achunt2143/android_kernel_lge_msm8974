<<<<<<< HEAD
/*
 *  binfmt_misc.c
 *
 *  Copyright (C) 1997 Richard Günther
 *
 *  binfmt_misc detects binaries via a magic or filename extension and invokes
 *  a specified wrapper. This should obsolete binfmt_java, binfmt_em86 and
 *  binfmt_mz.
 *
 *  1997-04-25 first version
 *  [...]
 *  1997-05-19 cleanup
 *  1997-06-26 hpa: pass the real filename rather than argv[0]
 *  1997-06-30 minor cleanup
 *  1997-08-09 removed extension stripping, locking cleanup
 *  2001-02-28 AV: rewritten into something that resembles C. Original didn't.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * binfmt_misc.c
 *
 * Copyright (C) 1997 Richard Günther
 *
 * binfmt_misc detects binaries via a magic or filename extension and invokes
 * a specified wrapper. See Documentation/admin-guide/binfmt-misc.rst for more details.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched/mm.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/magic.h>
#include <linux/binfmts.h>
#include <linux/slab.h>
#include <linux/ctype.h>
<<<<<<< HEAD
=======
#include <linux/string_helpers.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/file.h>
#include <linux/pagemap.h>
#include <linux/namei.h>
#include <linux/mount.h>
<<<<<<< HEAD
#include <linux/syscalls.h>
#include <linux/fs.h>

#include <asm/uaccess.h>
=======
#include <linux/fs_context.h>
#include <linux/syscalls.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#include "internal.h"

#ifdef DEBUG
# define USE_DEBUG 1
#else
# define USE_DEBUG 0
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

enum {
	VERBOSE_STATUS = 1 /* make it zero to save 400 bytes kernel memory */
};

<<<<<<< HEAD
static LIST_HEAD(entries);
static int enabled = 1;

enum {Enabled, Magic};
#define MISC_FMT_PRESERVE_ARGV0 (1<<31)
#define MISC_FMT_OPEN_BINARY (1<<30)
#define MISC_FMT_CREDENTIALS (1<<29)
=======
enum {Enabled, Magic};
#define MISC_FMT_PRESERVE_ARGV0 (1UL << 31)
#define MISC_FMT_OPEN_BINARY (1UL << 30)
#define MISC_FMT_CREDENTIALS (1UL << 29)
#define MISC_FMT_OPEN_FILE (1UL << 28)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

typedef struct {
	struct list_head list;
	unsigned long flags;		/* type, status, etc. */
	int offset;			/* offset of magic */
	int size;			/* size of magic/mask */
	char *magic;			/* magic or filename extension */
	char *mask;			/* mask, NULL for exact match */
<<<<<<< HEAD
	char *interpreter;		/* filename of interpreter */
	char *name;
	struct dentry *dentry;
} Node;

static DEFINE_RWLOCK(entries_lock);
static struct file_system_type bm_fs_type;
static struct vfsmount *bm_mnt;
static int entry_count;

/* 
 * Check if we support the binfmt
 * if we do, return the node, else NULL
 * locking is done in load_misc_binary
 */
static Node *check_file(struct linux_binprm *bprm)
{
	char *p = strrchr(bprm->interp, '.');
	struct list_head *l;

	list_for_each(l, &entries) {
		Node *e = list_entry(l, Node, list);
		char *s;
		int j;

		if (!test_bit(Enabled, &e->flags))
			continue;

=======
	const char *interpreter;	/* filename of interpreter */
	char *name;
	struct dentry *dentry;
	struct file *interp_file;
	refcount_t users;		/* sync removal with load_misc_binary() */
} Node;

static struct file_system_type bm_fs_type;

/*
 * Max length of the register string.  Determined by:
 *  - 7 delimiters
 *  - name:   ~50 bytes
 *  - type:   1 byte
 *  - offset: 3 bytes (has to be smaller than BINPRM_BUF_SIZE)
 *  - magic:  128 bytes (512 in escaped form)
 *  - mask:   128 bytes (512 in escaped form)
 *  - interp: ~50 bytes
 *  - flags:  5 bytes
 * Round that up a bit, and then back off to hold the internal data
 * (like struct Node).
 */
#define MAX_REGISTER_LENGTH 1920

/**
 * search_binfmt_handler - search for a binary handler for @bprm
 * @misc: handle to binfmt_misc instance
 * @bprm: binary for which we are looking for a handler
 *
 * Search for a binary type handler for @bprm in the list of registered binary
 * type handlers.
 *
 * Return: binary type list entry on success, NULL on failure
 */
static Node *search_binfmt_handler(struct binfmt_misc *misc,
				   struct linux_binprm *bprm)
{
	char *p = strrchr(bprm->interp, '.');
	Node *e;

	/* Walk all the registered handlers. */
	list_for_each_entry(e, &misc->entries, list) {
		char *s;
		int j;

		/* Make sure this one is currently enabled. */
		if (!test_bit(Enabled, &e->flags))
			continue;

		/* Do matching based on extension if applicable. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!test_bit(Magic, &e->flags)) {
			if (p && !strcmp(e->magic, p + 1))
				return e;
			continue;
		}

<<<<<<< HEAD
=======
		/* Do matching based on magic & mask. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		s = bprm->buf + e->offset;
		if (e->mask) {
			for (j = 0; j < e->size; j++)
				if ((*s++ ^ e->magic[j]) & e->mask[j])
					break;
		} else {
			for (j = 0; j < e->size; j++)
				if ((*s++ ^ e->magic[j]))
					break;
		}
		if (j == e->size)
			return e;
	}
<<<<<<< HEAD
	return NULL;
}

/*
 * the loader itself
 */
static int load_misc_binary(struct linux_binprm *bprm, struct pt_regs *regs)
{
	Node *fmt;
	struct file * interp_file = NULL;
	char iname[BINPRM_BUF_SIZE];
	const char *iname_addr = iname;
	int retval;
	int fd_binary = -1;

	retval = -ENOEXEC;
	if (!enabled)
		goto _ret;

	/* to keep locking time low, we copy the interpreter string */
	read_lock(&entries_lock);
	fmt = check_file(bprm);
	if (fmt)
		strlcpy(iname, fmt->interpreter, BINPRM_BUF_SIZE);
	read_unlock(&entries_lock);
	if (!fmt)
		goto _ret;

	if (!(fmt->flags & MISC_FMT_PRESERVE_ARGV0)) {
		retval = remove_arg_zero(bprm);
		if (retval)
			goto _ret;
	}

	if (fmt->flags & MISC_FMT_OPEN_BINARY) {

		/* if the binary should be opened on behalf of the
		 * interpreter than keep it open and assign descriptor
		 * to it */
 		fd_binary = get_unused_fd();
 		if (fd_binary < 0) {
 			retval = fd_binary;
 			goto _ret;
 		}
 		fd_install(fd_binary, bprm->file);

		/* if the binary is not readable than enforce mm->dumpable=0
		   regardless of the interpreter's permissions */
		would_dump(bprm, bprm->file);

		allow_write_access(bprm->file);
		bprm->file = NULL;

		/* mark the bprm that fd should be passed to interp */
		bprm->interp_flags |= BINPRM_FLAGS_EXECFD;
		bprm->interp_data = fd_binary;

 	} else {
 		allow_write_access(bprm->file);
 		fput(bprm->file);
 		bprm->file = NULL;
 	}
	/* make argv[1] be the path to the binary */
	retval = copy_strings_kernel (1, &bprm->interp, bprm);
	if (retval < 0)
		goto _error;
	bprm->argc++;

	/* add the interp as argv[0] */
	retval = copy_strings_kernel (1, &iname_addr, bprm);
	if (retval < 0)
		goto _error;
	bprm->argc ++;

	/* Update interp in case binfmt_script needs it. */
	retval = bprm_change_interp(iname, bprm);
	if (retval < 0)
		goto _error;

	interp_file = open_exec (iname);
	retval = PTR_ERR (interp_file);
	if (IS_ERR (interp_file))
		goto _error;

	bprm->file = interp_file;
	if (fmt->flags & MISC_FMT_CREDENTIALS) {
		/*
		 * No need to call prepare_binprm(), it's already been
		 * done.  bprm->buf is stale, update from interp_file.
		 */
		memset(bprm->buf, 0, BINPRM_BUF_SIZE);
		retval = kernel_read(bprm->file, 0, bprm->buf, BINPRM_BUF_SIZE);
	} else
		retval = prepare_binprm (bprm);

	if (retval < 0)
		goto _error;

	retval = search_binary_handler (bprm, regs);
	if (retval < 0)
		goto _error;

_ret:
	return retval;
_error:
	if (fd_binary > 0)
		sys_close(fd_binary);
	bprm->interp_flags = 0;
	bprm->interp_data = 0;
	goto _ret;
=======

	return NULL;
}

/**
 * get_binfmt_handler - try to find a binary type handler
 * @misc: handle to binfmt_misc instance
 * @bprm: binary for which we are looking for a handler
 *
 * Try to find a binfmt handler for the binary type. If one is found take a
 * reference to protect against removal via bm_{entry,status}_write().
 *
 * Return: binary type list entry on success, NULL on failure
 */
static Node *get_binfmt_handler(struct binfmt_misc *misc,
				struct linux_binprm *bprm)
{
	Node *e;

	read_lock(&misc->entries_lock);
	e = search_binfmt_handler(misc, bprm);
	if (e)
		refcount_inc(&e->users);
	read_unlock(&misc->entries_lock);
	return e;
}

/**
 * put_binfmt_handler - put binary handler node
 * @e: node to put
 *
 * Free node syncing with load_misc_binary() and defer final free to
 * load_misc_binary() in case it is using the binary type handler we were
 * requested to remove.
 */
static void put_binfmt_handler(Node *e)
{
	if (refcount_dec_and_test(&e->users)) {
		if (e->flags & MISC_FMT_OPEN_FILE)
			filp_close(e->interp_file, NULL);
		kfree(e);
	}
}

/**
 * load_binfmt_misc - load the binfmt_misc of the caller's user namespace
 *
 * To be called in load_misc_binary() to load the relevant struct binfmt_misc.
 * If a user namespace doesn't have its own binfmt_misc mount it can make use
 * of its ancestor's binfmt_misc handlers. This mimicks the behavior of
 * pre-namespaced binfmt_misc where all registered binfmt_misc handlers where
 * available to all user and user namespaces on the system.
 *
 * Return: the binfmt_misc instance of the caller's user namespace
 */
static struct binfmt_misc *load_binfmt_misc(void)
{
	const struct user_namespace *user_ns;
	struct binfmt_misc *misc;

	user_ns = current_user_ns();
	while (user_ns) {
		/* Pairs with smp_store_release() in bm_fill_super(). */
		misc = smp_load_acquire(&user_ns->binfmt_misc);
		if (misc)
			return misc;

		user_ns = user_ns->parent;
	}

	return &init_binfmt_misc;
}

/*
 * the loader itself
 */
static int load_misc_binary(struct linux_binprm *bprm)
{
	Node *fmt;
	struct file *interp_file = NULL;
	int retval = -ENOEXEC;
	struct binfmt_misc *misc;

	misc = load_binfmt_misc();
	if (!misc->enabled)
		return retval;

	fmt = get_binfmt_handler(misc, bprm);
	if (!fmt)
		return retval;

	/* Need to be able to load the file after exec */
	retval = -ENOENT;
	if (bprm->interp_flags & BINPRM_FLAGS_PATH_INACCESSIBLE)
		goto ret;

	if (fmt->flags & MISC_FMT_PRESERVE_ARGV0) {
		bprm->interp_flags |= BINPRM_FLAGS_PRESERVE_ARGV0;
	} else {
		retval = remove_arg_zero(bprm);
		if (retval)
			goto ret;
	}

	if (fmt->flags & MISC_FMT_OPEN_BINARY)
		bprm->have_execfd = 1;

	/* make argv[1] be the path to the binary */
	retval = copy_string_kernel(bprm->interp, bprm);
	if (retval < 0)
		goto ret;
	bprm->argc++;

	/* add the interp as argv[0] */
	retval = copy_string_kernel(fmt->interpreter, bprm);
	if (retval < 0)
		goto ret;
	bprm->argc++;

	/* Update interp in case binfmt_script needs it. */
	retval = bprm_change_interp(fmt->interpreter, bprm);
	if (retval < 0)
		goto ret;

	if (fmt->flags & MISC_FMT_OPEN_FILE) {
		interp_file = file_clone_open(fmt->interp_file);
		if (!IS_ERR(interp_file))
			deny_write_access(interp_file);
	} else {
		interp_file = open_exec(fmt->interpreter);
	}
	retval = PTR_ERR(interp_file);
	if (IS_ERR(interp_file))
		goto ret;

	bprm->interpreter = interp_file;
	if (fmt->flags & MISC_FMT_CREDENTIALS)
		bprm->execfd_creds = 1;

	retval = 0;
ret:

	/*
	 * If we actually put the node here all concurrent calls to
	 * load_misc_binary() will have finished. We also know
	 * that for the refcount to be zero someone must have concurently
	 * removed the binary type handler from the list and it's our job to
	 * free it.
	 */
	put_binfmt_handler(fmt);

	return retval;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Command parsers */

/*
 * parses and copies one argument enclosed in del from *sp to *dp,
 * recognising the \x special.
 * returns pointer to the copied argument or NULL in case of an
 * error (and sets err) or null argument length.
 */
static char *scanarg(char *s, char del)
{
	char c;

	while ((c = *s++) != del) {
		if (c == '\\' && *s == 'x') {
			s++;
			if (!isxdigit(*s++))
				return NULL;
			if (!isxdigit(*s++))
				return NULL;
		}
	}
<<<<<<< HEAD
	return s;
}

static int unquote(char *from)
{
	char c = 0, *s = from, *p = from;

	while ((c = *s++) != '\0') {
		if (c == '\\' && *s == 'x') {
			s++;
			c = toupper(*s++);
			*p = (c - (isdigit(c) ? '0' : 'A' - 10)) << 4;
			c = toupper(*s++);
			*p++ |= c - (isdigit(c) ? '0' : 'A' - 10);
			continue;
		}
		*p++ = c;
	}
	return p - from;
}

static char * check_special_flags (char * sfs, Node * e)
{
	char * p = sfs;
=======
	s[-1] ='\0';
	return s;
}

static char *check_special_flags(char *sfs, Node *e)
{
	char *p = sfs;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int cont = 1;

	/* special flags */
	while (cont) {
		switch (*p) {
<<<<<<< HEAD
			case 'P':
				p++;
				e->flags |= MISC_FMT_PRESERVE_ARGV0;
				break;
			case 'O':
				p++;
				e->flags |= MISC_FMT_OPEN_BINARY;
				break;
			case 'C':
				p++;
				/* this flags also implies the
				   open-binary flag */
				e->flags |= (MISC_FMT_CREDENTIALS |
						MISC_FMT_OPEN_BINARY);
				break;
			default:
				cont = 0;
=======
		case 'P':
			pr_debug("register: flag: P (preserve argv0)\n");
			p++;
			e->flags |= MISC_FMT_PRESERVE_ARGV0;
			break;
		case 'O':
			pr_debug("register: flag: O (open binary)\n");
			p++;
			e->flags |= MISC_FMT_OPEN_BINARY;
			break;
		case 'C':
			pr_debug("register: flag: C (preserve creds)\n");
			p++;
			/* this flags also implies the
			   open-binary flag */
			e->flags |= (MISC_FMT_CREDENTIALS |
					MISC_FMT_OPEN_BINARY);
			break;
		case 'F':
			pr_debug("register: flag: F: open interpreter file now\n");
			p++;
			e->flags |= MISC_FMT_OPEN_FILE;
			break;
		default:
			cont = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	return p;
}
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * This registers a new binary format, it recognises the syntax
 * ':name:type:offset:magic:mask:interpreter:flags'
 * where the ':' is the IFS, that can be chosen with the first char
 */
static Node *create_entry(const char __user *buffer, size_t count)
{
	Node *e;
	int memsize, err;
	char *buf, *p;
	char del;

<<<<<<< HEAD
	/* some sanity checks */
	err = -EINVAL;
	if ((count < 11) || (count > 256))
=======
	pr_debug("register: received %zu bytes\n", count);

	/* some sanity checks */
	err = -EINVAL;
	if ((count < 11) || (count > MAX_REGISTER_LENGTH))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;

	err = -ENOMEM;
	memsize = sizeof(Node) + count + 8;
<<<<<<< HEAD
	e = kmalloc(memsize, GFP_USER);
=======
	e = kmalloc(memsize, GFP_KERNEL_ACCOUNT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!e)
		goto out;

	p = buf = (char *)e + sizeof(Node);

	memset(e, 0, sizeof(Node));
	if (copy_from_user(buf, buffer, count))
<<<<<<< HEAD
		goto Efault;

	del = *p++;	/* delimeter */

	memset(buf+count, del, 8);

	e->name = p;
	p = strchr(p, del);
	if (!p)
		goto Einval;
=======
		goto efault;

	del = *p++;	/* delimeter */

	pr_debug("register: delim: %#x {%c}\n", del, del);

	/* Pad the buffer with the delim to simplify parsing below. */
	memset(buf + count, del, 8);

	/* Parse the 'name' field. */
	e->name = p;
	p = strchr(p, del);
	if (!p)
		goto einval;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	*p++ = '\0';
	if (!e->name[0] ||
	    !strcmp(e->name, ".") ||
	    !strcmp(e->name, "..") ||
	    strchr(e->name, '/'))
<<<<<<< HEAD
		goto Einval;
	switch (*p++) {
		case 'E': e->flags = 1<<Enabled; break;
		case 'M': e->flags = (1<<Enabled) | (1<<Magic); break;
		default: goto Einval;
	}
	if (*p++ != del)
		goto Einval;
	if (test_bit(Magic, &e->flags)) {
		char *s = strchr(p, del);
		if (!s)
			goto Einval;
		*s++ = '\0';
		e->offset = simple_strtoul(p, &p, 10);
		if (*p++)
			goto Einval;
		e->magic = p;
		p = scanarg(p, del);
		if (!p)
			goto Einval;
		p[-1] = '\0';
		if (!e->magic[0])
			goto Einval;
		e->mask = p;
		p = scanarg(p, del);
		if (!p)
			goto Einval;
		p[-1] = '\0';
		if (!e->mask[0])
			e->mask = NULL;
		e->size = unquote(e->magic);
		if (e->mask && unquote(e->mask) != e->size)
			goto Einval;
		if (e->size + e->offset > BINPRM_BUF_SIZE)
			goto Einval;
	} else {
		p = strchr(p, del);
		if (!p)
			goto Einval;
		*p++ = '\0';
		e->magic = p;
		p = strchr(p, del);
		if (!p)
			goto Einval;
		*p++ = '\0';
		if (!e->magic[0] || strchr(e->magic, '/'))
			goto Einval;
		p = strchr(p, del);
		if (!p)
			goto Einval;
		*p++ = '\0';
	}
	e->interpreter = p;
	p = strchr(p, del);
	if (!p)
		goto Einval;
	*p++ = '\0';
	if (!e->interpreter[0])
		goto Einval;


	p = check_special_flags (p, e);

	if (*p == '\n')
		p++;
	if (p != buf + count)
		goto Einval;
=======
		goto einval;

	pr_debug("register: name: {%s}\n", e->name);

	/* Parse the 'type' field. */
	switch (*p++) {
	case 'E':
		pr_debug("register: type: E (extension)\n");
		e->flags = 1 << Enabled;
		break;
	case 'M':
		pr_debug("register: type: M (magic)\n");
		e->flags = (1 << Enabled) | (1 << Magic);
		break;
	default:
		goto einval;
	}
	if (*p++ != del)
		goto einval;

	if (test_bit(Magic, &e->flags)) {
		/* Handle the 'M' (magic) format. */
		char *s;

		/* Parse the 'offset' field. */
		s = strchr(p, del);
		if (!s)
			goto einval;
		*s = '\0';
		if (p != s) {
			int r = kstrtoint(p, 10, &e->offset);
			if (r != 0 || e->offset < 0)
				goto einval;
		}
		p = s;
		if (*p++)
			goto einval;
		pr_debug("register: offset: %#x\n", e->offset);

		/* Parse the 'magic' field. */
		e->magic = p;
		p = scanarg(p, del);
		if (!p)
			goto einval;
		if (!e->magic[0])
			goto einval;
		if (USE_DEBUG)
			print_hex_dump_bytes(
				KBUILD_MODNAME ": register: magic[raw]: ",
				DUMP_PREFIX_NONE, e->magic, p - e->magic);

		/* Parse the 'mask' field. */
		e->mask = p;
		p = scanarg(p, del);
		if (!p)
			goto einval;
		if (!e->mask[0]) {
			e->mask = NULL;
			pr_debug("register:  mask[raw]: none\n");
		} else if (USE_DEBUG)
			print_hex_dump_bytes(
				KBUILD_MODNAME ": register:  mask[raw]: ",
				DUMP_PREFIX_NONE, e->mask, p - e->mask);

		/*
		 * Decode the magic & mask fields.
		 * Note: while we might have accepted embedded NUL bytes from
		 * above, the unescape helpers here will stop at the first one
		 * it encounters.
		 */
		e->size = string_unescape_inplace(e->magic, UNESCAPE_HEX);
		if (e->mask &&
		    string_unescape_inplace(e->mask, UNESCAPE_HEX) != e->size)
			goto einval;
		if (e->size > BINPRM_BUF_SIZE ||
		    BINPRM_BUF_SIZE - e->size < e->offset)
			goto einval;
		pr_debug("register: magic/mask length: %i\n", e->size);
		if (USE_DEBUG) {
			print_hex_dump_bytes(
				KBUILD_MODNAME ": register: magic[decoded]: ",
				DUMP_PREFIX_NONE, e->magic, e->size);

			if (e->mask) {
				int i;
				char *masked = kmalloc(e->size, GFP_KERNEL_ACCOUNT);

				print_hex_dump_bytes(
					KBUILD_MODNAME ": register:  mask[decoded]: ",
					DUMP_PREFIX_NONE, e->mask, e->size);

				if (masked) {
					for (i = 0; i < e->size; ++i)
						masked[i] = e->magic[i] & e->mask[i];
					print_hex_dump_bytes(
						KBUILD_MODNAME ": register:  magic[masked]: ",
						DUMP_PREFIX_NONE, masked, e->size);

					kfree(masked);
				}
			}
		}
	} else {
		/* Handle the 'E' (extension) format. */

		/* Skip the 'offset' field. */
		p = strchr(p, del);
		if (!p)
			goto einval;
		*p++ = '\0';

		/* Parse the 'magic' field. */
		e->magic = p;
		p = strchr(p, del);
		if (!p)
			goto einval;
		*p++ = '\0';
		if (!e->magic[0] || strchr(e->magic, '/'))
			goto einval;
		pr_debug("register: extension: {%s}\n", e->magic);

		/* Skip the 'mask' field. */
		p = strchr(p, del);
		if (!p)
			goto einval;
		*p++ = '\0';
	}

	/* Parse the 'interpreter' field. */
	e->interpreter = p;
	p = strchr(p, del);
	if (!p)
		goto einval;
	*p++ = '\0';
	if (!e->interpreter[0])
		goto einval;
	pr_debug("register: interpreter: {%s}\n", e->interpreter);

	/* Parse the 'flags' field. */
	p = check_special_flags(p, e);
	if (*p == '\n')
		p++;
	if (p != buf + count)
		goto einval;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return e;

out:
	return ERR_PTR(err);

<<<<<<< HEAD
Efault:
	kfree(e);
	return ERR_PTR(-EFAULT);
Einval:
=======
efault:
	kfree(e);
	return ERR_PTR(-EFAULT);
einval:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(e);
	return ERR_PTR(-EINVAL);
}

/*
 * Set status of entry/binfmt_misc:
 * '1' enables, '0' disables and '-1' clears entry/binfmt_misc
 */
static int parse_command(const char __user *buffer, size_t count)
{
	char s[4];

<<<<<<< HEAD
	if (!count)
		return 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (count > 3)
		return -EINVAL;
	if (copy_from_user(s, buffer, count))
		return -EFAULT;
<<<<<<< HEAD
	if (s[count-1] == '\n')
=======
	if (!count)
		return 0;
	if (s[count - 1] == '\n')
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		count--;
	if (count == 1 && s[0] == '0')
		return 1;
	if (count == 1 && s[0] == '1')
		return 2;
	if (count == 2 && s[0] == '-' && s[1] == '1')
		return 3;
	return -EINVAL;
}

/* generic stuff */

static void entry_status(Node *e, char *page)
{
<<<<<<< HEAD
	char *dp;
	char *status = "disabled";
	const char * flags = "flags: ";
=======
	char *dp = page;
	const char *status = "disabled";
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (test_bit(Enabled, &e->flags))
		status = "enabled";

	if (!VERBOSE_STATUS) {
		sprintf(page, "%s\n", status);
		return;
	}

<<<<<<< HEAD
	sprintf(page, "%s\ninterpreter %s\n", status, e->interpreter);
	dp = page + strlen(page);

	/* print the special flags */
	sprintf (dp, "%s", flags);
	dp += strlen (flags);
	if (e->flags & MISC_FMT_PRESERVE_ARGV0) {
		*dp ++ = 'P';
	}
	if (e->flags & MISC_FMT_OPEN_BINARY) {
		*dp ++ = 'O';
	}
	if (e->flags & MISC_FMT_CREDENTIALS) {
		*dp ++ = 'C';
	}
	*dp ++ = '\n';

=======
	dp += sprintf(dp, "%s\ninterpreter %s\n", status, e->interpreter);

	/* print the special flags */
	dp += sprintf(dp, "flags: ");
	if (e->flags & MISC_FMT_PRESERVE_ARGV0)
		*dp++ = 'P';
	if (e->flags & MISC_FMT_OPEN_BINARY)
		*dp++ = 'O';
	if (e->flags & MISC_FMT_CREDENTIALS)
		*dp++ = 'C';
	if (e->flags & MISC_FMT_OPEN_FILE)
		*dp++ = 'F';
	*dp++ = '\n';
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!test_bit(Magic, &e->flags)) {
		sprintf(dp, "extension .%s\n", e->magic);
	} else {
<<<<<<< HEAD
		int i;

		sprintf(dp, "offset %i\nmagic ", e->offset);
		dp = page + strlen(page);
		for (i = 0; i < e->size; i++) {
			sprintf(dp, "%02x", 0xff & (int) (e->magic[i]));
			dp += 2;
		}
		if (e->mask) {
			sprintf(dp, "\nmask ");
			dp += 6;
			for (i = 0; i < e->size; i++) {
				sprintf(dp, "%02x", 0xff & (int) (e->mask[i]));
				dp += 2;
			}
=======
		dp += sprintf(dp, "offset %i\nmagic ", e->offset);
		dp = bin2hex(dp, e->magic, e->size);
		if (e->mask) {
			dp += sprintf(dp, "\nmask ");
			dp = bin2hex(dp, e->mask, e->size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		*dp++ = '\n';
		*dp = '\0';
	}
}

static struct inode *bm_get_inode(struct super_block *sb, int mode)
{
<<<<<<< HEAD
	struct inode * inode = new_inode(sb);
=======
	struct inode *inode = new_inode(sb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (inode) {
		inode->i_ino = get_next_ino();
		inode->i_mode = mode;
<<<<<<< HEAD
		inode->i_atime = inode->i_mtime = inode->i_ctime =
			current_fs_time(inode->i_sb);
=======
		simple_inode_init_ts(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return inode;
}

<<<<<<< HEAD
static void bm_evict_inode(struct inode *inode)
{
	end_writeback(inode);
	kfree(inode->i_private);
}

static void kill_node(Node *e)
{
	struct dentry *dentry;

	write_lock(&entries_lock);
	dentry = e->dentry;
	if (dentry) {
		list_del_init(&e->list);
		e->dentry = NULL;
	}
	write_unlock(&entries_lock);

	if (dentry) {
		drop_nlink(dentry->d_inode);
		d_drop(dentry);
		dput(dentry);
		simple_release_fs(&bm_mnt, &entry_count);
	}
}

/* /<entry> */

static ssize_t
bm_entry_read(struct file * file, char __user * buf, size_t nbytes, loff_t *ppos)
{
	Node *e = file->f_path.dentry->d_inode->i_private;
	ssize_t res;
	char *page;

	if (!(page = (char*) __get_free_page(GFP_KERNEL)))
=======
/**
 * i_binfmt_misc - retrieve struct binfmt_misc from a binfmt_misc inode
 * @inode: inode of the relevant binfmt_misc instance
 *
 * This helper retrieves struct binfmt_misc from a binfmt_misc inode. This can
 * be done without any memory barriers because we are guaranteed that
 * user_ns->binfmt_misc is fully initialized. It was fully initialized when the
 * binfmt_misc mount was first created.
 *
 * Return: struct binfmt_misc of the relevant binfmt_misc instance
 */
static struct binfmt_misc *i_binfmt_misc(struct inode *inode)
{
	return inode->i_sb->s_user_ns->binfmt_misc;
}

/**
 * bm_evict_inode - cleanup data associated with @inode
 * @inode: inode to which the data is attached
 *
 * Cleanup the binary type handler data associated with @inode if a binary type
 * entry is removed or the filesystem is unmounted and the super block is
 * shutdown.
 *
 * If the ->evict call was not caused by a super block shutdown but by a write
 * to remove the entry or all entries via bm_{entry,status}_write() the entry
 * will have already been removed from the list. We keep the list_empty() check
 * to make that explicit.
*/
static void bm_evict_inode(struct inode *inode)
{
	Node *e = inode->i_private;

	clear_inode(inode);

	if (e) {
		struct binfmt_misc *misc;

		misc = i_binfmt_misc(inode);
		write_lock(&misc->entries_lock);
		if (!list_empty(&e->list))
			list_del_init(&e->list);
		write_unlock(&misc->entries_lock);
		put_binfmt_handler(e);
	}
}

/**
 * unlink_binfmt_dentry - remove the dentry for the binary type handler
 * @dentry: dentry associated with the binary type handler
 *
 * Do the actual filesystem work to remove a dentry for a registered binary
 * type handler. Since binfmt_misc only allows simple files to be created
 * directly under the root dentry of the filesystem we ensure that we are
 * indeed passed a dentry directly beneath the root dentry, that the inode
 * associated with the root dentry is locked, and that it is a regular file we
 * are asked to remove.
 */
static void unlink_binfmt_dentry(struct dentry *dentry)
{
	struct dentry *parent = dentry->d_parent;
	struct inode *inode, *parent_inode;

	/* All entries are immediate descendants of the root dentry. */
	if (WARN_ON_ONCE(dentry->d_sb->s_root != parent))
		return;

	/* We only expect to be called on regular files. */
	inode = d_inode(dentry);
	if (WARN_ON_ONCE(!S_ISREG(inode->i_mode)))
		return;

	/* The parent inode must be locked. */
	parent_inode = d_inode(parent);
	if (WARN_ON_ONCE(!inode_is_locked(parent_inode)))
		return;

	if (simple_positive(dentry)) {
		dget(dentry);
		simple_unlink(parent_inode, dentry);
		d_delete(dentry);
		dput(dentry);
	}
}

/**
 * remove_binfmt_handler - remove a binary type handler
 * @misc: handle to binfmt_misc instance
 * @e: binary type handler to remove
 *
 * Remove a binary type handler from the list of binary type handlers and
 * remove its associated dentry. This is called from
 * binfmt_{entry,status}_write(). In the future, we might want to think about
 * adding a proper ->unlink() method to binfmt_misc instead of forcing caller's
 * to use writes to files in order to delete binary type handlers. But it has
 * worked for so long that it's not a pressing issue.
 */
static void remove_binfmt_handler(struct binfmt_misc *misc, Node *e)
{
	write_lock(&misc->entries_lock);
	list_del_init(&e->list);
	write_unlock(&misc->entries_lock);
	unlink_binfmt_dentry(e->dentry);
}

/* /<entry> */

static ssize_t
bm_entry_read(struct file *file, char __user *buf, size_t nbytes, loff_t *ppos)
{
	Node *e = file_inode(file)->i_private;
	ssize_t res;
	char *page;

	page = (char *) __get_free_page(GFP_KERNEL);
	if (!page)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOMEM;

	entry_status(e, page);

	res = simple_read_from_buffer(buf, nbytes, ppos, page, strlen(page));

	free_page((unsigned long) page);
	return res;
}

static ssize_t bm_entry_write(struct file *file, const char __user *buffer,
				size_t count, loff_t *ppos)
{
<<<<<<< HEAD
	struct dentry *root;
	Node *e = file->f_path.dentry->d_inode->i_private;
	int res = parse_command(buffer, count);

	switch (res) {
		case 1: clear_bit(Enabled, &e->flags);
			break;
		case 2: set_bit(Enabled, &e->flags);
			break;
		case 3: root = dget(file->f_path.dentry->d_sb->s_root);
			mutex_lock(&root->d_inode->i_mutex);

			kill_node(e);

			mutex_unlock(&root->d_inode->i_mutex);
			dput(root);
			break;
		default: return res;
	}
=======
	struct inode *inode = file_inode(file);
	Node *e = inode->i_private;
	int res = parse_command(buffer, count);

	switch (res) {
	case 1:
		/* Disable this handler. */
		clear_bit(Enabled, &e->flags);
		break;
	case 2:
		/* Enable this handler. */
		set_bit(Enabled, &e->flags);
		break;
	case 3:
		/* Delete this handler. */
		inode = d_inode(inode->i_sb->s_root);
		inode_lock(inode);

		/*
		 * In order to add new element or remove elements from the list
		 * via bm_{entry,register,status}_write() inode_lock() on the
		 * root inode must be held.
		 * The lock is exclusive ensuring that the list can't be
		 * modified. Only load_misc_binary() can access but does so
		 * read-only. So we only need to take the write lock when we
		 * actually remove the entry from the list.
		 */
		if (!list_empty(&e->list))
			remove_binfmt_handler(i_binfmt_misc(inode), e);

		inode_unlock(inode);
		break;
	default:
		return res;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return count;
}

static const struct file_operations bm_entry_operations = {
	.read		= bm_entry_read,
	.write		= bm_entry_write,
	.llseek		= default_llseek,
};

/* /register */

static ssize_t bm_register_write(struct file *file, const char __user *buffer,
			       size_t count, loff_t *ppos)
{
	Node *e;
	struct inode *inode;
<<<<<<< HEAD
	struct dentry *root, *dentry;
	struct super_block *sb = file->f_path.dentry->d_sb;
	int err = 0;
=======
	struct super_block *sb = file_inode(file)->i_sb;
	struct dentry *root = sb->s_root, *dentry;
	struct binfmt_misc *misc;
	int err = 0;
	struct file *f = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	e = create_entry(buffer, count);

	if (IS_ERR(e))
		return PTR_ERR(e);

<<<<<<< HEAD
	root = dget(sb->s_root);
	mutex_lock(&root->d_inode->i_mutex);
=======
	if (e->flags & MISC_FMT_OPEN_FILE) {
		const struct cred *old_cred;

		/*
		 * Now that we support unprivileged binfmt_misc mounts make
		 * sure we use the credentials that the register @file was
		 * opened with to also open the interpreter. Before that this
		 * didn't matter much as only a privileged process could open
		 * the register file.
		 */
		old_cred = override_creds(file->f_cred);
		f = open_exec(e->interpreter);
		revert_creds(old_cred);
		if (IS_ERR(f)) {
			pr_notice("register: failed to install interpreter file %s\n",
				 e->interpreter);
			kfree(e);
			return PTR_ERR(f);
		}
		e->interp_file = f;
	}

	inode_lock(d_inode(root));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dentry = lookup_one_len(e->name, root, strlen(e->name));
	err = PTR_ERR(dentry);
	if (IS_ERR(dentry))
		goto out;

	err = -EEXIST;
<<<<<<< HEAD
	if (dentry->d_inode)
=======
	if (d_really_is_positive(dentry))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out2;

	inode = bm_get_inode(sb, S_IFREG | 0644);

	err = -ENOMEM;
	if (!inode)
		goto out2;

<<<<<<< HEAD
	err = simple_pin_fs(&bm_fs_type, &bm_mnt, &entry_count);
	if (err) {
		iput(inode);
		inode = NULL;
		goto out2;
	}

=======
	refcount_set(&e->users, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	e->dentry = dget(dentry);
	inode->i_private = e;
	inode->i_fop = &bm_entry_operations;

	d_instantiate(dentry, inode);
<<<<<<< HEAD
	write_lock(&entries_lock);
	list_add(&e->list, &entries);
	write_unlock(&entries_lock);
=======
	misc = i_binfmt_misc(inode);
	write_lock(&misc->entries_lock);
	list_add(&e->list, &misc->entries);
	write_unlock(&misc->entries_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = 0;
out2:
	dput(dentry);
out:
<<<<<<< HEAD
	mutex_unlock(&root->d_inode->i_mutex);
	dput(root);

	if (err) {
		kfree(e);
		return -EINVAL;
=======
	inode_unlock(d_inode(root));

	if (err) {
		if (f)
			filp_close(f, NULL);
		kfree(e);
		return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return count;
}

static const struct file_operations bm_register_operations = {
	.write		= bm_register_write,
	.llseek		= noop_llseek,
};

/* /status */

static ssize_t
bm_status_read(struct file *file, char __user *buf, size_t nbytes, loff_t *ppos)
{
<<<<<<< HEAD
	char *s = enabled ? "enabled\n" : "disabled\n";

	return simple_read_from_buffer(buf, nbytes, ppos, s, strlen(s));
}

static ssize_t bm_status_write(struct file * file, const char __user * buffer,
		size_t count, loff_t *ppos)
{
	int res = parse_command(buffer, count);
	struct dentry *root;

	switch (res) {
		case 1: enabled = 0; break;
		case 2: enabled = 1; break;
		case 3: root = dget(file->f_path.dentry->d_sb->s_root);
			mutex_lock(&root->d_inode->i_mutex);

			while (!list_empty(&entries))
				kill_node(list_entry(entries.next, Node, list));

			mutex_unlock(&root->d_inode->i_mutex);
			dput(root);
		default: return res;
	}
=======
	struct binfmt_misc *misc;
	char *s;

	misc = i_binfmt_misc(file_inode(file));
	s = misc->enabled ? "enabled\n" : "disabled\n";
	return simple_read_from_buffer(buf, nbytes, ppos, s, strlen(s));
}

static ssize_t bm_status_write(struct file *file, const char __user *buffer,
		size_t count, loff_t *ppos)
{
	struct binfmt_misc *misc;
	int res = parse_command(buffer, count);
	Node *e, *next;
	struct inode *inode;

	misc = i_binfmt_misc(file_inode(file));
	switch (res) {
	case 1:
		/* Disable all handlers. */
		misc->enabled = false;
		break;
	case 2:
		/* Enable all handlers. */
		misc->enabled = true;
		break;
	case 3:
		/* Delete all handlers. */
		inode = d_inode(file_inode(file)->i_sb->s_root);
		inode_lock(inode);

		/*
		 * In order to add new element or remove elements from the list
		 * via bm_{entry,register,status}_write() inode_lock() on the
		 * root inode must be held.
		 * The lock is exclusive ensuring that the list can't be
		 * modified. Only load_misc_binary() can access but does so
		 * read-only. So we only need to take the write lock when we
		 * actually remove the entry from the list.
		 */
		list_for_each_entry_safe(e, next, &misc->entries, list)
			remove_binfmt_handler(misc, e);

		inode_unlock(inode);
		break;
	default:
		return res;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return count;
}

static const struct file_operations bm_status_operations = {
	.read		= bm_status_read,
	.write		= bm_status_write,
	.llseek		= default_llseek,
};

/* Superblock handling */

<<<<<<< HEAD
static const struct super_operations s_ops = {
	.statfs		= simple_statfs,
	.evict_inode	= bm_evict_inode,
};

static int bm_fill_super(struct super_block * sb, void * data, int silent)
{
	static struct tree_descr bm_files[] = {
=======
static void bm_put_super(struct super_block *sb)
{
	struct user_namespace *user_ns = sb->s_fs_info;

	sb->s_fs_info = NULL;
	put_user_ns(user_ns);
}

static const struct super_operations s_ops = {
	.statfs		= simple_statfs,
	.evict_inode	= bm_evict_inode,
	.put_super	= bm_put_super,
};

static int bm_fill_super(struct super_block *sb, struct fs_context *fc)
{
	int err;
	struct user_namespace *user_ns = sb->s_user_ns;
	struct binfmt_misc *misc;
	static const struct tree_descr bm_files[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		[2] = {"status", &bm_status_operations, S_IWUSR|S_IRUGO},
		[3] = {"register", &bm_register_operations, S_IWUSR},
		/* last one */ {""}
	};
<<<<<<< HEAD
	int err = simple_fill_super(sb, BINFMTFS_MAGIC, bm_files);
=======

	if (WARN_ON(user_ns != current_user_ns()))
		return -EINVAL;

	/*
	 * Lazily allocate a new binfmt_misc instance for this namespace, i.e.
	 * do it here during the first mount of binfmt_misc. We don't need to
	 * waste memory for every user namespace allocation. It's likely much
	 * more common to not mount a separate binfmt_misc instance than it is
	 * to mount one.
	 *
	 * While multiple superblocks can exist they are keyed by userns in
	 * s_fs_info for binfmt_misc. Hence, the vfs guarantees that
	 * bm_fill_super() is called exactly once whenever a binfmt_misc
	 * superblock for a userns is created. This in turn lets us conclude
	 * that when a binfmt_misc superblock is created for the first time for
	 * a userns there's no one racing us. Therefore we don't need any
	 * barriers when we dereference binfmt_misc.
	 */
	misc = user_ns->binfmt_misc;
	if (!misc) {
		/*
		 * If it turns out that most user namespaces actually want to
		 * register their own binary type handler and therefore all
		 * create their own separate binfm_misc mounts we should
		 * consider turning this into a kmem cache.
		 */
		misc = kzalloc(sizeof(struct binfmt_misc), GFP_KERNEL);
		if (!misc)
			return -ENOMEM;

		INIT_LIST_HEAD(&misc->entries);
		rwlock_init(&misc->entries_lock);

		/* Pairs with smp_load_acquire() in load_binfmt_misc(). */
		smp_store_release(&user_ns->binfmt_misc, misc);
	}

	/*
	 * When the binfmt_misc superblock for this userns is shutdown
	 * ->enabled might have been set to false and we don't reinitialize
	 * ->enabled again in put_super() as someone might already be mounting
	 * binfmt_misc again. It also would be pointless since by the time
	 * ->put_super() is called we know that the binary type list for this
	 * bintfmt_misc mount is empty making load_misc_binary() return
	 * -ENOEXEC independent of whether ->enabled is true. Instead, if
	 * someone mounts binfmt_misc for the first time or again we simply
	 * reset ->enabled to true.
	 */
	misc->enabled = true;

	err = simple_fill_super(sb, BINFMTFS_MAGIC, bm_files);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!err)
		sb->s_op = &s_ops;
	return err;
}

<<<<<<< HEAD
static struct dentry *bm_mount(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *data)
{
	return mount_single(fs_type, flags, data, bm_fill_super);
=======
static void bm_free(struct fs_context *fc)
{
	if (fc->s_fs_info)
		put_user_ns(fc->s_fs_info);
}

static int bm_get_tree(struct fs_context *fc)
{
	return get_tree_keyed(fc, bm_fill_super, get_user_ns(fc->user_ns));
}

static const struct fs_context_operations bm_context_ops = {
	.free		= bm_free,
	.get_tree	= bm_get_tree,
};

static int bm_init_fs_context(struct fs_context *fc)
{
	fc->ops = &bm_context_ops;
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct linux_binfmt misc_format = {
	.module = THIS_MODULE,
	.load_binary = load_misc_binary,
};

static struct file_system_type bm_fs_type = {
	.owner		= THIS_MODULE,
	.name		= "binfmt_misc",
<<<<<<< HEAD
	.mount		= bm_mount,
=======
	.init_fs_context = bm_init_fs_context,
	.fs_flags	= FS_USERNS_MOUNT,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.kill_sb	= kill_litter_super,
};
MODULE_ALIAS_FS("binfmt_misc");

static int __init init_misc_binfmt(void)
{
	int err = register_filesystem(&bm_fs_type);
	if (!err)
		insert_binfmt(&misc_format);
	return err;
}

static void __exit exit_misc_binfmt(void)
{
	unregister_binfmt(&misc_format);
	unregister_filesystem(&bm_fs_type);
}

core_initcall(init_misc_binfmt);
module_exit(exit_misc_binfmt);
MODULE_LICENSE("GPL");
