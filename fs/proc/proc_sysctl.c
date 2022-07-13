<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * /proc/sys support
 */
#include <linux/init.h>
#include <linux/sysctl.h>
#include <linux/poll.h>
#include <linux/proc_fs.h>
<<<<<<< HEAD
#include <linux/security.h>
#include <linux/sched.h>
#include <linux/namei.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/kmemleak.h>
#include "internal.h"

=======
#include <linux/printk.h>
#include <linux/security.h>
#include <linux/sched.h>
#include <linux/cred.h>
#include <linux/namei.h>
#include <linux/mm.h>
#include <linux/uio.h>
#include <linux/module.h>
#include <linux/bpf-cgroup.h>
#include <linux/mount.h>
#include <linux/kmemleak.h>
#include "internal.h"

#define list_for_each_table_entry(entry, header)	\
	entry = header->ctl_table;			\
	for (size_t i = 0 ; i < header->ctl_table_size && entry->procname; ++i, entry++)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct dentry_operations proc_sys_dentry_operations;
static const struct file_operations proc_sys_file_operations;
static const struct inode_operations proc_sys_inode_operations;
static const struct file_operations proc_sys_dir_file_operations;
static const struct inode_operations proc_sys_dir_operations;

<<<<<<< HEAD
=======
/* Support for permanently empty directories */
static struct ctl_table sysctl_mount_point[] = {
	{.type = SYSCTL_TABLE_TYPE_PERMANENTLY_EMPTY }
};

/**
 * register_sysctl_mount_point() - registers a sysctl mount point
 * @path: path for the mount point
 *
 * Used to create a permanently empty directory to serve as mount point.
 * There are some subtle but important permission checks this allows in the
 * case of unprivileged mounts.
 */
struct ctl_table_header *register_sysctl_mount_point(const char *path)
{
	return register_sysctl(path, sysctl_mount_point);
}
EXPORT_SYMBOL(register_sysctl_mount_point);

#define sysctl_is_perm_empty_ctl_table(tptr)		\
	(tptr[0].type == SYSCTL_TABLE_TYPE_PERMANENTLY_EMPTY)
#define sysctl_is_perm_empty_ctl_header(hptr)		\
	(sysctl_is_perm_empty_ctl_table(hptr->ctl_table))
#define sysctl_set_perm_empty_ctl_header(hptr)		\
	(hptr->ctl_table[0].type = SYSCTL_TABLE_TYPE_PERMANENTLY_EMPTY)
#define sysctl_clear_perm_empty_ctl_header(hptr)	\
	(hptr->ctl_table[0].type = SYSCTL_TABLE_TYPE_DEFAULT)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void proc_sys_poll_notify(struct ctl_table_poll *poll)
{
	if (!poll)
		return;

	atomic_inc(&poll->event);
	wake_up_interruptible(&poll->wait);
}

static struct ctl_table root_table[] = {
	{
		.procname = "",
		.mode = S_IFDIR|S_IRUGO|S_IXUGO,
	},
<<<<<<< HEAD
	{ }
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
static struct ctl_table_root sysctl_table_root = {
	.default_set.dir.header = {
		{{.count = 1,
		  .nreg = 1,
		  .ctl_table = root_table }},
		.ctl_table_arg = root_table,
		.root = &sysctl_table_root,
		.set = &sysctl_table_root.default_set,
	},
};

static DEFINE_SPINLOCK(sysctl_lock);

static void drop_sysctl_table(struct ctl_table_header *header);
static int sysctl_follow_link(struct ctl_table_header **phead,
<<<<<<< HEAD
	struct ctl_table **pentry, struct nsproxy *namespaces);
=======
	struct ctl_table **pentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int insert_links(struct ctl_table_header *head);
static void put_links(struct ctl_table_header *header);

static void sysctl_print_dir(struct ctl_dir *dir)
{
	if (dir->header.parent)
		sysctl_print_dir(dir->header.parent);
<<<<<<< HEAD
	printk(KERN_CONT "%s/", dir->header.ctl_table[0].procname);
=======
	pr_cont("%s/", dir->header.ctl_table[0].procname);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int namecmp(const char *name1, int len1, const char *name2, int len2)
{
<<<<<<< HEAD
	int minlen;
	int cmp;

	minlen = len1;
	if (minlen > len2)
		minlen = len2;

	cmp = memcmp(name1, name2, minlen);
=======
	int cmp;

	cmp = memcmp(name1, name2, min(len1, len2));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (cmp == 0)
		cmp = len1 - len2;
	return cmp;
}

/* Called under sysctl_lock */
static struct ctl_table *find_entry(struct ctl_table_header **phead,
	struct ctl_dir *dir, const char *name, int namelen)
{
	struct ctl_table_header *head;
	struct ctl_table *entry;
	struct rb_node *node = dir->root.rb_node;

	while (node)
	{
		struct ctl_node *ctl_node;
		const char *procname;
		int cmp;

		ctl_node = rb_entry(node, struct ctl_node, node);
		head = ctl_node->header;
		entry = &head->ctl_table[ctl_node - head->node];
		procname = entry->procname;

		cmp = namecmp(name, namelen, procname, strlen(procname));
		if (cmp < 0)
			node = node->rb_left;
		else if (cmp > 0)
			node = node->rb_right;
		else {
			*phead = head;
			return entry;
		}
	}
	return NULL;
}

static int insert_entry(struct ctl_table_header *head, struct ctl_table *entry)
{
	struct rb_node *node = &head->node[entry - head->ctl_table].node;
	struct rb_node **p = &head->parent->root.rb_node;
	struct rb_node *parent = NULL;
	const char *name = entry->procname;
	int namelen = strlen(name);

	while (*p) {
		struct ctl_table_header *parent_head;
		struct ctl_table *parent_entry;
		struct ctl_node *parent_node;
		const char *parent_name;
		int cmp;

		parent = *p;
		parent_node = rb_entry(parent, struct ctl_node, node);
		parent_head = parent_node->header;
		parent_entry = &parent_head->ctl_table[parent_node - parent_head->node];
		parent_name = parent_entry->procname;

		cmp = namecmp(name, namelen, parent_name, strlen(parent_name));
		if (cmp < 0)
			p = &(*p)->rb_left;
		else if (cmp > 0)
			p = &(*p)->rb_right;
		else {
<<<<<<< HEAD
			printk(KERN_ERR "sysctl duplicate entry: ");
			sysctl_print_dir(head->parent);
			printk(KERN_CONT "/%s\n", entry->procname);
=======
			pr_err("sysctl duplicate entry: ");
			sysctl_print_dir(head->parent);
			pr_cont("%s\n", entry->procname);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EEXIST;
		}
	}

	rb_link_node(node, parent, p);
	rb_insert_color(node, &head->parent->root);
	return 0;
}

static void erase_entry(struct ctl_table_header *head, struct ctl_table *entry)
{
	struct rb_node *node = &head->node[entry - head->ctl_table].node;

	rb_erase(node, &head->parent->root);
}

static void init_header(struct ctl_table_header *head,
	struct ctl_table_root *root, struct ctl_table_set *set,
<<<<<<< HEAD
	struct ctl_node *node, struct ctl_table *table)
{
	head->ctl_table = table;
=======
	struct ctl_node *node, struct ctl_table *table, size_t table_size)
{
	head->ctl_table = table;
	head->ctl_table_size = table_size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	head->ctl_table_arg = table;
	head->used = 0;
	head->count = 1;
	head->nreg = 1;
	head->unregistering = NULL;
	head->root = root;
	head->set = set;
	head->parent = NULL;
	head->node = node;
<<<<<<< HEAD
	if (node) {
		struct ctl_table *entry;
		for (entry = table; entry->procname; entry++, node++) {
			rb_init_node(&node->node);
			node->header = head;
=======
	INIT_HLIST_HEAD(&head->inodes);
	if (node) {
		struct ctl_table *entry;

		list_for_each_table_entry(entry, head) {
			node->header = head;
			node++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
}

static void erase_header(struct ctl_table_header *head)
{
	struct ctl_table *entry;
<<<<<<< HEAD
	for (entry = head->ctl_table; entry->procname; entry++)
=======

	list_for_each_table_entry(entry, head)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		erase_entry(head, entry);
}

static int insert_header(struct ctl_dir *dir, struct ctl_table_header *header)
{
	struct ctl_table *entry;
<<<<<<< HEAD
	int err;

	dir->header.nreg++;
=======
	struct ctl_table_header *dir_h = &dir->header;
	int err;


	/* Is this a permanently empty directory? */
	if (sysctl_is_perm_empty_ctl_header(dir_h))
		return -EROFS;

	/* Am I creating a permanently empty directory? */
	if (header->ctl_table_size > 0 &&
	    sysctl_is_perm_empty_ctl_table(header->ctl_table)) {
		if (!RB_EMPTY_ROOT(&dir->root))
			return -EINVAL;
		sysctl_set_perm_empty_ctl_header(dir_h);
	}

	dir_h->nreg++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	header->parent = dir;
	err = insert_links(header);
	if (err)
		goto fail_links;
<<<<<<< HEAD
	for (entry = header->ctl_table; entry->procname; entry++) {
=======
	list_for_each_table_entry(entry, header) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = insert_entry(header, entry);
		if (err)
			goto fail;
	}
	return 0;
fail:
	erase_header(header);
	put_links(header);
fail_links:
<<<<<<< HEAD
	header->parent = NULL;
	drop_sysctl_table(&dir->header);
=======
	if (header->ctl_table == sysctl_mount_point)
		sysctl_clear_perm_empty_ctl_header(dir_h);
	header->parent = NULL;
	drop_sysctl_table(dir_h);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

/* called under sysctl_lock */
static int use_table(struct ctl_table_header *p)
{
	if (unlikely(p->unregistering))
		return 0;
	p->used++;
	return 1;
}

/* called under sysctl_lock */
static void unuse_table(struct ctl_table_header *p)
{
	if (!--p->used)
		if (unlikely(p->unregistering))
			complete(p->unregistering);
}

<<<<<<< HEAD
=======
static void proc_sys_invalidate_dcache(struct ctl_table_header *head)
{
	proc_invalidate_siblings_dcache(&head->inodes, &sysctl_lock);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* called under sysctl_lock, will reacquire if has to wait */
static void start_unregistering(struct ctl_table_header *p)
{
	/*
	 * if p->used is 0, nobody will ever touch that entry again;
	 * we'll eliminate all paths to it before dropping sysctl_lock
	 */
	if (unlikely(p->used)) {
		struct completion wait;
		init_completion(&wait);
		p->unregistering = &wait;
		spin_unlock(&sysctl_lock);
		wait_for_completion(&wait);
<<<<<<< HEAD
		spin_lock(&sysctl_lock);
	} else {
		/* anything non-NULL; we'll never dereference it */
		p->unregistering = ERR_PTR(-EINVAL);
	}
	/*
	 * do not remove from the list until nobody holds it; walking the
	 * list in do_sysctl() relies on that.
	 */
	erase_header(p);
}

static void sysctl_head_get(struct ctl_table_header *head)
{
	spin_lock(&sysctl_lock);
	head->count++;
	spin_unlock(&sysctl_lock);
}

void sysctl_head_put(struct ctl_table_header *head)
{
	spin_lock(&sysctl_lock);
	if (!--head->count)
		kfree_rcu(head, rcu);
	spin_unlock(&sysctl_lock);
}

static struct ctl_table_header *sysctl_head_grab(struct ctl_table_header *head)
{
	if (!head)
		BUG();
=======
	} else {
		/* anything non-NULL; we'll never dereference it */
		p->unregistering = ERR_PTR(-EINVAL);
		spin_unlock(&sysctl_lock);
	}
	/*
	 * Invalidate dentries for unregistered sysctls: namespaced sysctls
	 * can have duplicate names and contaminate dcache very badly.
	 */
	proc_sys_invalidate_dcache(p);
	/*
	 * do not remove from the list until nobody holds it; walking the
	 * list in do_sysctl() relies on that.
	 */
	spin_lock(&sysctl_lock);
	erase_header(p);
}

static struct ctl_table_header *sysctl_head_grab(struct ctl_table_header *head)
{
	BUG_ON(!head);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_lock(&sysctl_lock);
	if (!use_table(head))
		head = ERR_PTR(-ENOENT);
	spin_unlock(&sysctl_lock);
	return head;
}

static void sysctl_head_finish(struct ctl_table_header *head)
{
	if (!head)
		return;
	spin_lock(&sysctl_lock);
	unuse_table(head);
	spin_unlock(&sysctl_lock);
}

static struct ctl_table_set *
<<<<<<< HEAD
lookup_header_set(struct ctl_table_root *root, struct nsproxy *namespaces)
{
	struct ctl_table_set *set = &root->default_set;
	if (root->lookup)
		set = root->lookup(root, namespaces);
=======
lookup_header_set(struct ctl_table_root *root)
{
	struct ctl_table_set *set = &root->default_set;
	if (root->lookup)
		set = root->lookup(root);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return set;
}

static struct ctl_table *lookup_entry(struct ctl_table_header **phead,
				      struct ctl_dir *dir,
				      const char *name, int namelen)
{
	struct ctl_table_header *head;
	struct ctl_table *entry;

	spin_lock(&sysctl_lock);
	entry = find_entry(&head, dir, name, namelen);
	if (entry && use_table(head))
		*phead = head;
	else
		entry = NULL;
	spin_unlock(&sysctl_lock);
	return entry;
}

static struct ctl_node *first_usable_entry(struct rb_node *node)
{
	struct ctl_node *ctl_node;

	for (;node; node = rb_next(node)) {
		ctl_node = rb_entry(node, struct ctl_node, node);
		if (use_table(ctl_node->header))
			return ctl_node;
	}
	return NULL;
}

static void first_entry(struct ctl_dir *dir,
	struct ctl_table_header **phead, struct ctl_table **pentry)
{
	struct ctl_table_header *head = NULL;
	struct ctl_table *entry = NULL;
	struct ctl_node *ctl_node;

	spin_lock(&sysctl_lock);
	ctl_node = first_usable_entry(rb_first(&dir->root));
	spin_unlock(&sysctl_lock);
	if (ctl_node) {
		head = ctl_node->header;
		entry = &head->ctl_table[ctl_node - head->node];
	}
	*phead = head;
	*pentry = entry;
}

static void next_entry(struct ctl_table_header **phead, struct ctl_table **pentry)
{
	struct ctl_table_header *head = *phead;
	struct ctl_table *entry = *pentry;
	struct ctl_node *ctl_node = &head->node[entry - head->ctl_table];

	spin_lock(&sysctl_lock);
	unuse_table(head);

	ctl_node = first_usable_entry(rb_next(&ctl_node->node));
	spin_unlock(&sysctl_lock);
	head = NULL;
	if (ctl_node) {
		head = ctl_node->header;
		entry = &head->ctl_table[ctl_node - head->node];
	}
	*phead = head;
	*pentry = entry;
}

<<<<<<< HEAD
void register_sysctl_root(struct ctl_table_root *root)
{
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * sysctl_perm does NOT grant the superuser all rights automatically, because
 * some sysctl variables are readonly even to root.
 */

static int test_perm(int mode, int op)
{
<<<<<<< HEAD
	if (!current_euid())
		mode >>= 6;
	else if (in_egroup_p(0))
=======
	if (uid_eq(current_euid(), GLOBAL_ROOT_UID))
		mode >>= 6;
	else if (in_egroup_p(GLOBAL_ROOT_GID))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mode >>= 3;
	if ((op & ~mode & (MAY_READ|MAY_WRITE|MAY_EXEC)) == 0)
		return 0;
	return -EACCES;
}

<<<<<<< HEAD
static int sysctl_perm(struct ctl_table_root *root, struct ctl_table *table, int op)
{
	int mode;

	if (root->permissions)
		mode = root->permissions(root, current->nsproxy, table);
=======
static int sysctl_perm(struct ctl_table_header *head, struct ctl_table *table, int op)
{
	struct ctl_table_root *root = head->root;
	int mode;

	if (root->permissions)
		mode = root->permissions(head, table);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else
		mode = table->mode;

	return test_perm(mode, op);
}

static struct inode *proc_sys_make_inode(struct super_block *sb,
		struct ctl_table_header *head, struct ctl_table *table)
{
<<<<<<< HEAD
=======
	struct ctl_table_root *root = head->root;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct inode *inode;
	struct proc_inode *ei;

	inode = new_inode(sb);
	if (!inode)
<<<<<<< HEAD
		goto out;

	inode->i_ino = get_next_ino();

	sysctl_head_get(head);
	ei = PROC_I(inode);
	ei->sysctl = head;
	ei->sysctl_entry = table;

	inode->i_mtime = inode->i_atime = inode->i_ctime = CURRENT_TIME;
=======
		return ERR_PTR(-ENOMEM);

	inode->i_ino = get_next_ino();

	ei = PROC_I(inode);

	spin_lock(&sysctl_lock);
	if (unlikely(head->unregistering)) {
		spin_unlock(&sysctl_lock);
		iput(inode);
		return ERR_PTR(-ENOENT);
	}
	ei->sysctl = head;
	ei->sysctl_entry = table;
	hlist_add_head_rcu(&ei->sibling_inodes, &head->inodes);
	head->count++;
	spin_unlock(&sysctl_lock);

	simple_inode_init_ts(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	inode->i_mode = table->mode;
	if (!S_ISDIR(table->mode)) {
		inode->i_mode |= S_IFREG;
		inode->i_op = &proc_sys_inode_operations;
		inode->i_fop = &proc_sys_file_operations;
	} else {
		inode->i_mode |= S_IFDIR;
		inode->i_op = &proc_sys_dir_operations;
		inode->i_fop = &proc_sys_dir_file_operations;
<<<<<<< HEAD
	}
out:
	return inode;
}

=======
		if (sysctl_is_perm_empty_ctl_header(head))
			make_empty_dir_inode(inode);
	}

	if (root->set_ownership)
		root->set_ownership(head, table, &inode->i_uid, &inode->i_gid);
	else {
		inode->i_uid = GLOBAL_ROOT_UID;
		inode->i_gid = GLOBAL_ROOT_GID;
	}

	return inode;
}

void proc_sys_evict_inode(struct inode *inode, struct ctl_table_header *head)
{
	spin_lock(&sysctl_lock);
	hlist_del_init_rcu(&PROC_I(inode)->sibling_inodes);
	if (!--head->count)
		kfree_rcu(head, rcu);
	spin_unlock(&sysctl_lock);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct ctl_table_header *grab_header(struct inode *inode)
{
	struct ctl_table_header *head = PROC_I(inode)->sysctl;
	if (!head)
		head = &sysctl_table_root.default_set.dir.header;
	return sysctl_head_grab(head);
}

static struct dentry *proc_sys_lookup(struct inode *dir, struct dentry *dentry,
					unsigned int flags)
{
	struct ctl_table_header *head = grab_header(dir);
	struct ctl_table_header *h = NULL;
<<<<<<< HEAD
	struct qstr *name = &dentry->d_name;
=======
	const struct qstr *name = &dentry->d_name;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ctl_table *p;
	struct inode *inode;
	struct dentry *err = ERR_PTR(-ENOENT);
	struct ctl_dir *ctl_dir;
	int ret;

	if (IS_ERR(head))
		return ERR_CAST(head);

	ctl_dir = container_of(head, struct ctl_dir, header);

	p = lookup_entry(&h, ctl_dir, name->name, name->len);
	if (!p)
		goto out;

	if (S_ISLNK(p->mode)) {
<<<<<<< HEAD
		ret = sysctl_follow_link(&h, &p, current->nsproxy);
=======
		ret = sysctl_follow_link(&h, &p);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = ERR_PTR(ret);
		if (ret)
			goto out;
	}

<<<<<<< HEAD
	err = ERR_PTR(-ENOMEM);
	inode = proc_sys_make_inode(dir->i_sb, h ? h : head, p);
	if (!inode)
		goto out;

	err = NULL;
	d_set_d_op(dentry, &proc_sys_dentry_operations);
	d_add(dentry, inode);
=======
	d_set_d_op(dentry, &proc_sys_dentry_operations);
	inode = proc_sys_make_inode(dir->i_sb, h ? h : head, p);
	err = d_splice_alias(inode, dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

out:
	if (h)
		sysctl_head_finish(h);
	sysctl_head_finish(head);
	return err;
}

<<<<<<< HEAD
static ssize_t proc_sys_call_handler(struct file *filp, void __user *buf,
		size_t count, loff_t *ppos, int write)
{
	struct inode *inode = filp->f_path.dentry->d_inode;
	struct ctl_table_header *head = grab_header(inode);
	struct ctl_table *table = PROC_I(inode)->sysctl_entry;
	ssize_t error;
	size_t res;
=======
static ssize_t proc_sys_call_handler(struct kiocb *iocb, struct iov_iter *iter,
		int write)
{
	struct inode *inode = file_inode(iocb->ki_filp);
	struct ctl_table_header *head = grab_header(inode);
	struct ctl_table *table = PROC_I(inode)->sysctl_entry;
	size_t count = iov_iter_count(iter);
	char *kbuf;
	ssize_t error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (IS_ERR(head))
		return PTR_ERR(head);

	/*
	 * At this point we know that the sysctl was not unregistered
	 * and won't be until we finish.
	 */
	error = -EPERM;
<<<<<<< HEAD
	if (sysctl_perm(head->root, table, write ? MAY_WRITE : MAY_READ))
=======
	if (sysctl_perm(head, table, write ? MAY_WRITE : MAY_READ))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;

	/* if that can happen at all, it should be -EINVAL, not -EISDIR */
	error = -EINVAL;
	if (!table->proc_handler)
		goto out;

<<<<<<< HEAD
	/* careful: calling conventions are nasty here */
	res = count;
	error = table->proc_handler(table, write, buf, &res, ppos);
	if (!error)
		error = res;
=======
	/* don't even try if the size is too large */
	error = -ENOMEM;
	if (count >= KMALLOC_MAX_SIZE)
		goto out;
	kbuf = kvzalloc(count + 1, GFP_KERNEL);
	if (!kbuf)
		goto out;

	if (write) {
		error = -EFAULT;
		if (!copy_from_iter_full(kbuf, count, iter))
			goto out_free_buf;
		kbuf[count] = '\0';
	}

	error = BPF_CGROUP_RUN_PROG_SYSCTL(head, table, write, &kbuf, &count,
					   &iocb->ki_pos);
	if (error)
		goto out_free_buf;

	/* careful: calling conventions are nasty here */
	error = table->proc_handler(table, write, kbuf, &count, &iocb->ki_pos);
	if (error)
		goto out_free_buf;

	if (!write) {
		error = -EFAULT;
		if (copy_to_iter(kbuf, count, iter) < count)
			goto out_free_buf;
	}

	error = count;
out_free_buf:
	kvfree(kbuf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	sysctl_head_finish(head);

	return error;
}

<<<<<<< HEAD
static ssize_t proc_sys_read(struct file *filp, char __user *buf,
				size_t count, loff_t *ppos)
{
	return proc_sys_call_handler(filp, (void __user *)buf, count, ppos, 0);
}

static ssize_t proc_sys_write(struct file *filp, const char __user *buf,
				size_t count, loff_t *ppos)
{
	return proc_sys_call_handler(filp, (void __user *)buf, count, ppos, 1);
=======
static ssize_t proc_sys_read(struct kiocb *iocb, struct iov_iter *iter)
{
	return proc_sys_call_handler(iocb, iter, 0);
}

static ssize_t proc_sys_write(struct kiocb *iocb, struct iov_iter *iter)
{
	return proc_sys_call_handler(iocb, iter, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int proc_sys_open(struct inode *inode, struct file *filp)
{
	struct ctl_table_header *head = grab_header(inode);
	struct ctl_table *table = PROC_I(inode)->sysctl_entry;

	/* sysctl was unregistered */
	if (IS_ERR(head))
		return PTR_ERR(head);

	if (table->poll)
		filp->private_data = proc_sys_poll_event(table->poll);

	sysctl_head_finish(head);

	return 0;
}

<<<<<<< HEAD
static unsigned int proc_sys_poll(struct file *filp, poll_table *wait)
{
	struct inode *inode = filp->f_path.dentry->d_inode;
	struct ctl_table_header *head = grab_header(inode);
	struct ctl_table *table = PROC_I(inode)->sysctl_entry;
	unsigned int ret = DEFAULT_POLLMASK;
=======
static __poll_t proc_sys_poll(struct file *filp, poll_table *wait)
{
	struct inode *inode = file_inode(filp);
	struct ctl_table_header *head = grab_header(inode);
	struct ctl_table *table = PROC_I(inode)->sysctl_entry;
	__poll_t ret = DEFAULT_POLLMASK;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long event;

	/* sysctl was unregistered */
	if (IS_ERR(head))
<<<<<<< HEAD
		return POLLERR | POLLHUP;
=======
		return EPOLLERR | EPOLLHUP;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!table->proc_handler)
		goto out;

	if (!table->poll)
		goto out;

	event = (unsigned long)filp->private_data;
	poll_wait(filp, &table->poll->wait, wait);

	if (event != atomic_read(&table->poll->event)) {
		filp->private_data = proc_sys_poll_event(table->poll);
<<<<<<< HEAD
		ret = POLLIN | POLLRDNORM | POLLERR | POLLPRI;
=======
		ret = EPOLLIN | EPOLLRDNORM | EPOLLERR | EPOLLPRI;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

out:
	sysctl_head_finish(head);

	return ret;
}

<<<<<<< HEAD
static int proc_sys_fill_cache(struct file *filp, void *dirent,
				filldir_t filldir,
				struct ctl_table_header *head,
				struct ctl_table *table)
{
	struct dentry *child, *dir = filp->f_path.dentry;
=======
static bool proc_sys_fill_cache(struct file *file,
				struct dir_context *ctx,
				struct ctl_table_header *head,
				struct ctl_table *table)
{
	struct dentry *child, *dir = file->f_path.dentry;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct inode *inode;
	struct qstr qname;
	ino_t ino = 0;
	unsigned type = DT_UNKNOWN;

	qname.name = table->procname;
	qname.len  = strlen(table->procname);
<<<<<<< HEAD
	qname.hash = full_name_hash(qname.name, qname.len);

	child = d_lookup(dir, &qname);
	if (!child) {
		child = d_alloc(dir, &qname);
		if (child) {
			inode = proc_sys_make_inode(dir->d_sb, head, table);
			if (!inode) {
				dput(child);
				return -ENOMEM;
			} else {
				d_set_d_op(child, &proc_sys_dentry_operations);
				d_add(child, inode);
			}
		} else {
			return -ENOMEM;
		}
	}
	inode = child->d_inode;
	ino  = inode->i_ino;
	type = inode->i_mode >> 12;
	dput(child);
	return !!filldir(dirent, qname.name, qname.len, filp->f_pos, ino, type);
}

static int proc_sys_link_fill_cache(struct file *filp, void *dirent,
				    filldir_t filldir,
				    struct ctl_table_header *head,
				    struct ctl_table *table)
{
	int err, ret = 0;
	head = sysctl_head_grab(head);

	if (S_ISLNK(table->mode)) {
		/* It is not an error if we can not follow the link ignore it */
		err = sysctl_follow_link(&head, &table, current->nsproxy);
		if (err)
			goto out;
	}

	ret = proc_sys_fill_cache(filp, dirent, filldir, head, table);
=======
	qname.hash = full_name_hash(dir, qname.name, qname.len);

	child = d_lookup(dir, &qname);
	if (!child) {
		DECLARE_WAIT_QUEUE_HEAD_ONSTACK(wq);
		child = d_alloc_parallel(dir, &qname, &wq);
		if (IS_ERR(child))
			return false;
		if (d_in_lookup(child)) {
			struct dentry *res;
			d_set_d_op(child, &proc_sys_dentry_operations);
			inode = proc_sys_make_inode(dir->d_sb, head, table);
			res = d_splice_alias(inode, child);
			d_lookup_done(child);
			if (unlikely(res)) {
				if (IS_ERR(res)) {
					dput(child);
					return false;
				}
				dput(child);
				child = res;
			}
		}
	}
	inode = d_inode(child);
	ino  = inode->i_ino;
	type = inode->i_mode >> 12;
	dput(child);
	return dir_emit(ctx, qname.name, qname.len, ino, type);
}

static bool proc_sys_link_fill_cache(struct file *file,
				    struct dir_context *ctx,
				    struct ctl_table_header *head,
				    struct ctl_table *table)
{
	bool ret = true;

	head = sysctl_head_grab(head);
	if (IS_ERR(head))
		return false;

	/* It is not an error if we can not follow the link ignore it */
	if (sysctl_follow_link(&head, &table))
		goto out;

	ret = proc_sys_fill_cache(file, ctx, head, table);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	sysctl_head_finish(head);
	return ret;
}

<<<<<<< HEAD
static int scan(struct ctl_table_header *head, ctl_table *table,
		unsigned long *pos, struct file *file,
		void *dirent, filldir_t filldir)
{
	int res;

	if ((*pos)++ < file->f_pos)
		return 0;

	if (unlikely(S_ISLNK(table->mode)))
		res = proc_sys_link_fill_cache(file, dirent, filldir, head, table);
	else
		res = proc_sys_fill_cache(file, dirent, filldir, head, table);

	if (res == 0)
		file->f_pos = *pos;
=======
static int scan(struct ctl_table_header *head, struct ctl_table *table,
		unsigned long *pos, struct file *file,
		struct dir_context *ctx)
{
	bool res;

	if ((*pos)++ < ctx->pos)
		return true;

	if (unlikely(S_ISLNK(table->mode)))
		res = proc_sys_link_fill_cache(file, ctx, head, table);
	else
		res = proc_sys_fill_cache(file, ctx, head, table);

	if (res)
		ctx->pos = *pos;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return res;
}

<<<<<<< HEAD
static int proc_sys_readdir(struct file *filp, void *dirent, filldir_t filldir)
{
	struct dentry *dentry = filp->f_path.dentry;
	struct inode *inode = dentry->d_inode;
	struct ctl_table_header *head = grab_header(inode);
=======
static int proc_sys_readdir(struct file *file, struct dir_context *ctx)
{
	struct ctl_table_header *head = grab_header(file_inode(file));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ctl_table_header *h = NULL;
	struct ctl_table *entry;
	struct ctl_dir *ctl_dir;
	unsigned long pos;
<<<<<<< HEAD
	int ret = -EINVAL;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (IS_ERR(head))
		return PTR_ERR(head);

	ctl_dir = container_of(head, struct ctl_dir, header);

<<<<<<< HEAD
	ret = 0;
	/* Avoid a switch here: arm builds fail with missing __cmpdi2 */
	if (filp->f_pos == 0) {
		if (filldir(dirent, ".", 1, filp->f_pos,
				inode->i_ino, DT_DIR) < 0)
			goto out;
		filp->f_pos++;
	}
	if (filp->f_pos == 1) {
		if (filldir(dirent, "..", 2, filp->f_pos,
				parent_ino(dentry), DT_DIR) < 0)
			goto out;
		filp->f_pos++;
	}
	pos = 2;

	for (first_entry(ctl_dir, &h, &entry); h; next_entry(&h, &entry)) {
		ret = scan(h, entry, &pos, filp, dirent, filldir);
		if (ret) {
=======
	if (!dir_emit_dots(file, ctx))
		goto out;

	pos = 2;

	for (first_entry(ctl_dir, &h, &entry); h; next_entry(&h, &entry)) {
		if (!scan(h, entry, &pos, file, ctx)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			sysctl_head_finish(h);
			break;
		}
	}
<<<<<<< HEAD
	ret = 1;
out:
	sysctl_head_finish(head);
	return ret;
}

static int proc_sys_permission(struct inode *inode, int mask)
=======
out:
	sysctl_head_finish(head);
	return 0;
}

static int proc_sys_permission(struct mnt_idmap *idmap,
			       struct inode *inode, int mask)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/*
	 * sysctl entries that are not writeable,
	 * are _NOT_ writeable, capabilities or not.
	 */
	struct ctl_table_header *head;
	struct ctl_table *table;
	int error;

	/* Executable files are not allowed under /proc/sys/ */
	if ((mask & MAY_EXEC) && S_ISREG(inode->i_mode))
		return -EACCES;

	head = grab_header(inode);
	if (IS_ERR(head))
		return PTR_ERR(head);

	table = PROC_I(inode)->sysctl_entry;
	if (!table) /* global root - r-xr-xr-x */
		error = mask & MAY_WRITE ? -EACCES : 0;
	else /* Use the permissions on the sysctl table entry */
<<<<<<< HEAD
		error = sysctl_perm(head->root, table, mask & ~MAY_NOT_BLOCK);
=======
		error = sysctl_perm(head, table, mask & ~MAY_NOT_BLOCK);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	sysctl_head_finish(head);
	return error;
}

<<<<<<< HEAD
static int proc_sys_setattr(struct dentry *dentry, struct iattr *attr)
{
	struct inode *inode = dentry->d_inode;
=======
static int proc_sys_setattr(struct mnt_idmap *idmap,
			    struct dentry *dentry, struct iattr *attr)
{
	struct inode *inode = d_inode(dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int error;

	if (attr->ia_valid & (ATTR_MODE | ATTR_UID | ATTR_GID))
		return -EPERM;

<<<<<<< HEAD
	error = inode_change_ok(inode, attr);
	if (error)
		return error;

	if ((attr->ia_valid & ATTR_SIZE) &&
	    attr->ia_size != i_size_read(inode)) {
		error = vmtruncate(inode, attr->ia_size);
		if (error)
			return error;
	}

	setattr_copy(inode, attr);
	mark_inode_dirty(inode);
	return 0;
}

static int proc_sys_getattr(struct vfsmount *mnt, struct dentry *dentry, struct kstat *stat)
{
	struct inode *inode = dentry->d_inode;
=======
	error = setattr_prepare(&nop_mnt_idmap, dentry, attr);
	if (error)
		return error;

	setattr_copy(&nop_mnt_idmap, inode, attr);
	return 0;
}

static int proc_sys_getattr(struct mnt_idmap *idmap,
			    const struct path *path, struct kstat *stat,
			    u32 request_mask, unsigned int query_flags)
{
	struct inode *inode = d_inode(path->dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ctl_table_header *head = grab_header(inode);
	struct ctl_table *table = PROC_I(inode)->sysctl_entry;

	if (IS_ERR(head))
		return PTR_ERR(head);

<<<<<<< HEAD
	generic_fillattr(inode, stat);
=======
	generic_fillattr(&nop_mnt_idmap, request_mask, inode, stat);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (table)
		stat->mode = (stat->mode & S_IFMT) | table->mode;

	sysctl_head_finish(head);
	return 0;
}

static const struct file_operations proc_sys_file_operations = {
	.open		= proc_sys_open,
	.poll		= proc_sys_poll,
<<<<<<< HEAD
	.read		= proc_sys_read,
	.write		= proc_sys_write,
=======
	.read_iter	= proc_sys_read,
	.write_iter	= proc_sys_write,
	.splice_read	= copy_splice_read,
	.splice_write	= iter_file_splice_write,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.llseek		= default_llseek,
};

static const struct file_operations proc_sys_dir_file_operations = {
	.read		= generic_read_dir,
<<<<<<< HEAD
	.readdir	= proc_sys_readdir,
=======
	.iterate_shared	= proc_sys_readdir,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.llseek		= generic_file_llseek,
};

static const struct inode_operations proc_sys_inode_operations = {
	.permission	= proc_sys_permission,
	.setattr	= proc_sys_setattr,
	.getattr	= proc_sys_getattr,
};

static const struct inode_operations proc_sys_dir_operations = {
	.lookup		= proc_sys_lookup,
	.permission	= proc_sys_permission,
	.setattr	= proc_sys_setattr,
	.getattr	= proc_sys_getattr,
};

static int proc_sys_revalidate(struct dentry *dentry, unsigned int flags)
{
	if (flags & LOOKUP_RCU)
		return -ECHILD;
<<<<<<< HEAD
	return !PROC_I(dentry->d_inode)->sysctl->unregistering;
=======
	return !PROC_I(d_inode(dentry))->sysctl->unregistering;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int proc_sys_delete(const struct dentry *dentry)
{
<<<<<<< HEAD
	return !!PROC_I(dentry->d_inode)->sysctl->unregistering;
=======
	return !!PROC_I(d_inode(dentry))->sysctl->unregistering;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int sysctl_is_seen(struct ctl_table_header *p)
{
	struct ctl_table_set *set = p->set;
	int res;
	spin_lock(&sysctl_lock);
	if (p->unregistering)
		res = 0;
	else if (!set->is_seen)
		res = 1;
	else
		res = set->is_seen(set);
	spin_unlock(&sysctl_lock);
	return res;
}

<<<<<<< HEAD
static int proc_sys_compare(const struct dentry *parent,
		const struct inode *pinode,
		const struct dentry *dentry, const struct inode *inode,
		unsigned int len, const char *str, const struct qstr *name)
{
	struct ctl_table_header *head;
	/* Although proc doesn't have negative dentries, rcu-walk means
	 * that inode here can be NULL */
	/* AV: can it, indeed? */
=======
static int proc_sys_compare(const struct dentry *dentry,
		unsigned int len, const char *str, const struct qstr *name)
{
	struct ctl_table_header *head;
	struct inode *inode;

	/* Although proc doesn't have negative dentries, rcu-walk means
	 * that inode here can be NULL */
	/* AV: can it, indeed? */
	inode = d_inode_rcu(dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!inode)
		return 1;
	if (name->len != len)
		return 1;
	if (memcmp(name->name, str, len))
		return 1;
	head = rcu_dereference(PROC_I(inode)->sysctl);
	return !head || !sysctl_is_seen(head);
}

static const struct dentry_operations proc_sys_dentry_operations = {
	.d_revalidate	= proc_sys_revalidate,
	.d_delete	= proc_sys_delete,
	.d_compare	= proc_sys_compare,
};

static struct ctl_dir *find_subdir(struct ctl_dir *dir,
				   const char *name, int namelen)
{
	struct ctl_table_header *head;
	struct ctl_table *entry;

	entry = find_entry(&head, dir, name, namelen);
	if (!entry)
		return ERR_PTR(-ENOENT);
	if (!S_ISDIR(entry->mode))
		return ERR_PTR(-ENOTDIR);
	return container_of(head, struct ctl_dir, header);
}

static struct ctl_dir *new_dir(struct ctl_table_set *set,
			       const char *name, int namelen)
{
	struct ctl_table *table;
	struct ctl_dir *new;
	struct ctl_node *node;
	char *new_name;

	new = kzalloc(sizeof(*new) + sizeof(struct ctl_node) +
		      sizeof(struct ctl_table)*2 +  namelen + 1,
		      GFP_KERNEL);
	if (!new)
		return NULL;

	node = (struct ctl_node *)(new + 1);
	table = (struct ctl_table *)(node + 1);
	new_name = (char *)(table + 2);
	memcpy(new_name, name, namelen);
<<<<<<< HEAD
	new_name[namelen] = '\0';
	table[0].procname = new_name;
	table[0].mode = S_IFDIR|S_IRUGO|S_IXUGO;
	init_header(&new->header, set->dir.header.root, set, node, table);
=======
	table[0].procname = new_name;
	table[0].mode = S_IFDIR|S_IRUGO|S_IXUGO;
	init_header(&new->header, set->dir.header.root, set, node, table, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return new;
}

/**
 * get_subdir - find or create a subdir with the specified name.
 * @dir:  Directory to create the subdirectory in
 * @name: The name of the subdirectory to find or create
 * @namelen: The length of name
 *
 * Takes a directory with an elevated reference count so we know that
 * if we drop the lock the directory will not go away.  Upon success
 * the reference is moved from @dir to the returned subdirectory.
 * Upon error an error code is returned and the reference on @dir is
 * simply dropped.
 */
static struct ctl_dir *get_subdir(struct ctl_dir *dir,
				  const char *name, int namelen)
{
	struct ctl_table_set *set = dir->header.set;
	struct ctl_dir *subdir, *new = NULL;
	int err;

	spin_lock(&sysctl_lock);
	subdir = find_subdir(dir, name, namelen);
	if (!IS_ERR(subdir))
		goto found;
	if (PTR_ERR(subdir) != -ENOENT)
		goto failed;

	spin_unlock(&sysctl_lock);
	new = new_dir(set, name, namelen);
	spin_lock(&sysctl_lock);
	subdir = ERR_PTR(-ENOMEM);
	if (!new)
		goto failed;

	/* Was the subdir added while we dropped the lock? */
	subdir = find_subdir(dir, name, namelen);
	if (!IS_ERR(subdir))
		goto found;
	if (PTR_ERR(subdir) != -ENOENT)
		goto failed;

	/* Nope.  Use the our freshly made directory entry. */
	err = insert_header(dir, &new->header);
	subdir = ERR_PTR(err);
	if (err)
		goto failed;
	subdir = new;
found:
	subdir->header.nreg++;
failed:
<<<<<<< HEAD
	if (unlikely(IS_ERR(subdir))) {
		printk(KERN_ERR "sysctl could not get directory: ");
		sysctl_print_dir(dir);
		printk(KERN_CONT "/%*.*s %ld\n",
			namelen, namelen, name, PTR_ERR(subdir));
=======
	if (IS_ERR(subdir)) {
		pr_err("sysctl could not get directory: ");
		sysctl_print_dir(dir);
		pr_cont("%*.*s %ld\n", namelen, namelen, name,
			PTR_ERR(subdir));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	drop_sysctl_table(&dir->header);
	if (new)
		drop_sysctl_table(&new->header);
	spin_unlock(&sysctl_lock);
	return subdir;
}

static struct ctl_dir *xlate_dir(struct ctl_table_set *set, struct ctl_dir *dir)
{
	struct ctl_dir *parent;
	const char *procname;
	if (!dir->header.parent)
		return &set->dir;
	parent = xlate_dir(set, dir->header.parent);
	if (IS_ERR(parent))
		return parent;
	procname = dir->header.ctl_table[0].procname;
	return find_subdir(parent, procname, strlen(procname));
}

static int sysctl_follow_link(struct ctl_table_header **phead,
<<<<<<< HEAD
	struct ctl_table **pentry, struct nsproxy *namespaces)
=======
	struct ctl_table **pentry)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ctl_table_header *head;
	struct ctl_table_root *root;
	struct ctl_table_set *set;
	struct ctl_table *entry;
	struct ctl_dir *dir;
	int ret;

<<<<<<< HEAD
	ret = 0;
	spin_lock(&sysctl_lock);
	root = (*pentry)->data;
	set = lookup_header_set(root, namespaces);
=======
	spin_lock(&sysctl_lock);
	root = (*pentry)->data;
	set = lookup_header_set(root);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dir = xlate_dir(set, (*phead)->parent);
	if (IS_ERR(dir))
		ret = PTR_ERR(dir);
	else {
		const char *procname = (*pentry)->procname;
		head = NULL;
		entry = find_entry(&head, dir, procname, strlen(procname));
		ret = -ENOENT;
		if (entry && use_table(head)) {
			unuse_table(*phead);
			*phead = head;
			*pentry = entry;
			ret = 0;
		}
	}

	spin_unlock(&sysctl_lock);
	return ret;
}

static int sysctl_err(const char *path, struct ctl_table *table, char *fmt, ...)
{
	struct va_format vaf;
	va_list args;

	va_start(args, fmt);
	vaf.fmt = fmt;
	vaf.va = &args;

<<<<<<< HEAD
	printk(KERN_ERR "sysctl table check failed: %s/%s %pV\n",
		path, table->procname, &vaf);
=======
	pr_err("sysctl table check failed: %s/%s %pV\n",
	       path, table->procname, &vaf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	va_end(args);
	return -EINVAL;
}

<<<<<<< HEAD
static int sysctl_check_table(const char *path, struct ctl_table *table)
{
	int err = 0;
	for (; table->procname; table++) {
		if (table->child)
			err = sysctl_err(path, table, "Not a file");

		if ((table->proc_handler == proc_dostring) ||
		    (table->proc_handler == proc_dointvec) ||
		    (table->proc_handler == proc_dointvec_minmax) ||
		    (table->proc_handler == proc_dointvec_jiffies) ||
		    (table->proc_handler == proc_dointvec_userhz_jiffies) ||
		    (table->proc_handler == proc_dointvec_ms_jiffies) ||
		    (table->proc_handler == proc_doulongvec_minmax) ||
		    (table->proc_handler == proc_doulongvec_ms_jiffies_minmax)) {
			if (!table->data)
				err = sysctl_err(path, table, "No data");
			if (!table->maxlen)
				err = sysctl_err(path, table, "No maxlen");
		}
		if (!table->proc_handler)
			err = sysctl_err(path, table, "No proc_handler");

		if ((table->mode & (S_IRUGO|S_IWUGO)) != table->mode)
			err = sysctl_err(path, table, "bogus .mode 0%o",
				table->mode);
=======
static int sysctl_check_table_array(const char *path, struct ctl_table *table)
{
	int err = 0;

	if ((table->proc_handler == proc_douintvec) ||
	    (table->proc_handler == proc_douintvec_minmax)) {
		if (table->maxlen != sizeof(unsigned int))
			err |= sysctl_err(path, table, "array not allowed");
	}

	if (table->proc_handler == proc_dou8vec_minmax) {
		if (table->maxlen != sizeof(u8))
			err |= sysctl_err(path, table, "array not allowed");
	}

	if (table->proc_handler == proc_dobool) {
		if (table->maxlen != sizeof(bool))
			err |= sysctl_err(path, table, "array not allowed");
	}

	return err;
}

static int sysctl_check_table(const char *path, struct ctl_table_header *header)
{
	struct ctl_table *entry;
	int err = 0;
	list_for_each_table_entry(entry, header) {
		if ((entry->proc_handler == proc_dostring) ||
		    (entry->proc_handler == proc_dobool) ||
		    (entry->proc_handler == proc_dointvec) ||
		    (entry->proc_handler == proc_douintvec) ||
		    (entry->proc_handler == proc_douintvec_minmax) ||
		    (entry->proc_handler == proc_dointvec_minmax) ||
		    (entry->proc_handler == proc_dou8vec_minmax) ||
		    (entry->proc_handler == proc_dointvec_jiffies) ||
		    (entry->proc_handler == proc_dointvec_userhz_jiffies) ||
		    (entry->proc_handler == proc_dointvec_ms_jiffies) ||
		    (entry->proc_handler == proc_doulongvec_minmax) ||
		    (entry->proc_handler == proc_doulongvec_ms_jiffies_minmax)) {
			if (!entry->data)
				err |= sysctl_err(path, entry, "No data");
			if (!entry->maxlen)
				err |= sysctl_err(path, entry, "No maxlen");
			else
				err |= sysctl_check_table_array(path, entry);
		}
		if (!entry->proc_handler)
			err |= sysctl_err(path, entry, "No proc_handler");

		if ((entry->mode & (S_IRUGO|S_IWUGO)) != entry->mode)
			err |= sysctl_err(path, entry, "bogus .mode 0%o",
				entry->mode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return err;
}

<<<<<<< HEAD
static struct ctl_table_header *new_links(struct ctl_dir *dir, struct ctl_table *table,
	struct ctl_table_root *link_root)
=======
static struct ctl_table_header *new_links(struct ctl_dir *dir, struct ctl_table_header *head)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ctl_table *link_table, *entry, *link;
	struct ctl_table_header *links;
	struct ctl_node *node;
	char *link_name;
	int nr_entries, name_bytes;

	name_bytes = 0;
	nr_entries = 0;
<<<<<<< HEAD
	for (entry = table; entry->procname; entry++) {
=======
	list_for_each_table_entry(entry, head) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		nr_entries++;
		name_bytes += strlen(entry->procname) + 1;
	}

	links = kzalloc(sizeof(struct ctl_table_header) +
			sizeof(struct ctl_node)*nr_entries +
			sizeof(struct ctl_table)*(nr_entries + 1) +
			name_bytes,
			GFP_KERNEL);

	if (!links)
		return NULL;

	node = (struct ctl_node *)(links + 1);
	link_table = (struct ctl_table *)(node + nr_entries);
	link_name = (char *)&link_table[nr_entries + 1];
<<<<<<< HEAD

	for (link = link_table, entry = table; entry->procname; link++, entry++) {
=======
	link = link_table;

	list_for_each_table_entry(entry, head) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		int len = strlen(entry->procname) + 1;
		memcpy(link_name, entry->procname, len);
		link->procname = link_name;
		link->mode = S_IFLNK|S_IRWXUGO;
<<<<<<< HEAD
		link->data = link_root;
		link_name += len;
	}
	init_header(links, dir->header.root, dir->header.set, node, link_table);
=======
		link->data = head->root;
		link_name += len;
		link++;
	}
	init_header(links, dir->header.root, dir->header.set, node, link_table,
		    head->ctl_table_size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	links->nreg = nr_entries;

	return links;
}

static bool get_links(struct ctl_dir *dir,
<<<<<<< HEAD
	struct ctl_table *table, struct ctl_table_root *link_root)
{
	struct ctl_table_header *head;
	struct ctl_table *entry, *link;

	/* Are there links available for every entry in table? */
	for (entry = table; entry->procname; entry++) {
		const char *procname = entry->procname;
		link = find_entry(&head, dir, procname, strlen(procname));
=======
		      struct ctl_table_header *header,
		      struct ctl_table_root *link_root)
{
	struct ctl_table_header *tmp_head;
	struct ctl_table *entry, *link;

	if (header->ctl_table_size == 0 ||
	    sysctl_is_perm_empty_ctl_table(header->ctl_table))
		return true;

	/* Are there links available for every entry in table? */
	list_for_each_table_entry(entry, header) {
		const char *procname = entry->procname;
		link = find_entry(&tmp_head, dir, procname, strlen(procname));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!link)
			return false;
		if (S_ISDIR(link->mode) && S_ISDIR(entry->mode))
			continue;
		if (S_ISLNK(link->mode) && (link->data == link_root))
			continue;
		return false;
	}

	/* The checks passed.  Increase the registration count on the links */
<<<<<<< HEAD
	for (entry = table; entry->procname; entry++) {
		const char *procname = entry->procname;
		link = find_entry(&head, dir, procname, strlen(procname));
		head->nreg++;
=======
	list_for_each_table_entry(entry, header) {
		const char *procname = entry->procname;
		link = find_entry(&tmp_head, dir, procname, strlen(procname));
		tmp_head->nreg++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return true;
}

static int insert_links(struct ctl_table_header *head)
{
	struct ctl_table_set *root_set = &sysctl_table_root.default_set;
<<<<<<< HEAD
	struct ctl_dir *core_parent = NULL;
=======
	struct ctl_dir *core_parent;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ctl_table_header *links;
	int err;

	if (head->set == root_set)
		return 0;

	core_parent = xlate_dir(root_set, head->parent);
	if (IS_ERR(core_parent))
		return 0;

<<<<<<< HEAD
	if (get_links(core_parent, head->ctl_table, head->root))
=======
	if (get_links(core_parent, head, head->root))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	core_parent->header.nreg++;
	spin_unlock(&sysctl_lock);

<<<<<<< HEAD
	links = new_links(core_parent, head->ctl_table, head->root);
=======
	links = new_links(core_parent, head);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock(&sysctl_lock);
	err = -ENOMEM;
	if (!links)
		goto out;

	err = 0;
<<<<<<< HEAD
	if (get_links(core_parent, head->ctl_table, head->root)) {
=======
	if (get_links(core_parent, head, head->root)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		kfree(links);
		goto out;
	}

	err = insert_header(core_parent, links);
	if (err)
		kfree(links);
out:
	drop_sysctl_table(&core_parent->header);
	return err;
}

<<<<<<< HEAD
=======
/* Find the directory for the ctl_table. If one is not found create it. */
static struct ctl_dir *sysctl_mkdir_p(struct ctl_dir *dir, const char *path)
{
	const char *name, *nextname;

	for (name = path; name; name = nextname) {
		int namelen;
		nextname = strchr(name, '/');
		if (nextname) {
			namelen = nextname - name;
			nextname++;
		} else {
			namelen = strlen(name);
		}
		if (namelen == 0)
			continue;

		/*
		 * namelen ensures if name is "foo/bar/yay" only foo is
		 * registered first. We traverse as if using mkdir -p and
		 * return a ctl_dir for the last directory entry.
		 */
		dir = get_subdir(dir, name, namelen);
		if (IS_ERR(dir))
			break;
	}
	return dir;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * __register_sysctl_table - register a leaf sysctl table
 * @set: Sysctl tree to register on
 * @path: The path to the directory the sysctl table is in.
<<<<<<< HEAD
 * @table: the top-level table structure
=======
 * @table: the top-level table structure without any child. This table
 * 	 should not be free'd after registration. So it should not be
 * 	 used on stack. It can either be a global or dynamically allocated
 * 	 by the caller and free'd later after sysctl unregistration.
 * @table_size : The number of elements in table
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Register a sysctl table hierarchy. @table should be a filled in ctl_table
 * array. A completely 0 filled entry terminates the table.
 *
 * The members of the &struct ctl_table structure are used as follows:
 *
 * procname - the name of the sysctl file under /proc/sys. Set to %NULL to not
 *            enter a sysctl file
 *
 * data - a pointer to data for use by proc_handler
 *
 * maxlen - the maximum size in bytes of the data
 *
 * mode - the file permissions for the /proc/sys file
 *
 * child - must be %NULL.
 *
 * proc_handler - the text handler routine (described below)
 *
 * extra1, extra2 - extra pointers usable by the proc handler routines
<<<<<<< HEAD
 *
 * Leaf nodes in the sysctl tree will be represented by a single file
 * under /proc; non-leaf nodes will be represented by directories.
=======
 * XXX: we should eventually modify these to use long min / max [0]
 * [0] https://lkml.kernel.org/87zgpte9o4.fsf@email.froward.int.ebiederm.org
 *
 * Leaf nodes in the sysctl tree will be represented by a single file
 * under /proc; non-leaf nodes (where child is not NULL) are not allowed,
 * sysctl_check_table() verifies this.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * There must be a proc_handler routine for any terminal nodes.
 * Several default handlers are available to cover common cases -
 *
 * proc_dostring(), proc_dointvec(), proc_dointvec_jiffies(),
 * proc_dointvec_userhz_jiffies(), proc_dointvec_minmax(),
 * proc_doulongvec_ms_jiffies_minmax(), proc_doulongvec_minmax()
 *
 * It is the handler's job to read the input buffer from user memory
 * and process it. The handler should return 0 on success.
 *
 * This routine returns %NULL on a failure to register, and a pointer
 * to the table header on success.
 */
struct ctl_table_header *__register_sysctl_table(
	struct ctl_table_set *set,
<<<<<<< HEAD
	const char *path, struct ctl_table *table)
{
	struct ctl_table_root *root = set->dir.header.root;
	struct ctl_table_header *header;
	const char *name, *nextname;
	struct ctl_dir *dir;
	struct ctl_table *entry;
	struct ctl_node *node;
	int nr_entries = 0;

	for (entry = table; entry->procname; entry++)
		nr_entries++;

	header = kzalloc(sizeof(struct ctl_table_header) +
			 sizeof(struct ctl_node)*nr_entries, GFP_KERNEL);
	if (!header)
		return NULL;

	kmemleak_not_leak(header);

	node = (struct ctl_node *)(header + 1);
	init_header(header, root, set, node, table);
	if (sysctl_check_table(path, table))
=======
	const char *path, struct ctl_table *table, size_t table_size)
{
	struct ctl_table_root *root = set->dir.header.root;
	struct ctl_table_header *header;
	struct ctl_dir *dir;
	struct ctl_node *node;

	header = kzalloc(sizeof(struct ctl_table_header) +
			 sizeof(struct ctl_node)*table_size, GFP_KERNEL_ACCOUNT);
	if (!header)
		return NULL;

	node = (struct ctl_node *)(header + 1);
	init_header(header, root, set, node, table, table_size);
	if (sysctl_check_table(path, header))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto fail;

	spin_lock(&sysctl_lock);
	dir = &set->dir;
<<<<<<< HEAD
	/* Reference moved down the diretory tree get_subdir */
	dir->header.nreg++;
	spin_unlock(&sysctl_lock);

	/* Find the directory for the ctl_table */
	for (name = path; name; name = nextname) {
		int namelen;
		nextname = strchr(name, '/');
		if (nextname) {
			namelen = nextname - name;
			nextname++;
		} else {
			namelen = strlen(name);
		}
		if (namelen == 0)
			continue;

		dir = get_subdir(dir, name, namelen);
		if (IS_ERR(dir))
			goto fail;
	}

=======
	/* Reference moved down the directory tree get_subdir */
	dir->header.nreg++;
	spin_unlock(&sysctl_lock);

	dir = sysctl_mkdir_p(dir, path);
	if (IS_ERR(dir))
		goto fail;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_lock(&sysctl_lock);
	if (insert_header(dir, header))
		goto fail_put_dir_locked;

	drop_sysctl_table(&dir->header);
	spin_unlock(&sysctl_lock);

	return header;

fail_put_dir_locked:
	drop_sysctl_table(&dir->header);
	spin_unlock(&sysctl_lock);
fail:
	kfree(header);
<<<<<<< HEAD
	dump_stack();
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return NULL;
}

/**
<<<<<<< HEAD
 * register_sysctl - register a sysctl table
 * @path: The path to the directory the sysctl table is in.
 * @table: the table structure
=======
 * register_sysctl_sz - register a sysctl table
 * @path: The path to the directory the sysctl table is in. If the path
 * 	doesn't exist we will create it for you.
 * @table: the table structure. The calller must ensure the life of the @table
 * 	will be kept during the lifetime use of the syctl. It must not be freed
 * 	until unregister_sysctl_table() is called with the given returned table
 * 	with this registration. If your code is non modular then you don't need
 * 	to call unregister_sysctl_table() and can instead use something like
 * 	register_sysctl_init() which does not care for the result of the syctl
 * 	registration.
 * @table_size: The number of elements in table.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Register a sysctl table. @table should be a filled in ctl_table
 * array. A completely 0 filled entry terminates the table.
 *
 * See __register_sysctl_table for more details.
 */
<<<<<<< HEAD
struct ctl_table_header *register_sysctl(const char *path, struct ctl_table *table)
{
	return __register_sysctl_table(&sysctl_table_root.default_set,
					path, table);
}
EXPORT_SYMBOL(register_sysctl);

static char *append_path(const char *path, char *pos, const char *name)
{
	int namelen;
	namelen = strlen(name);
	if (((pos - path) + namelen + 2) >= PATH_MAX)
		return NULL;
	memcpy(pos, name, namelen);
	pos[namelen] = '/';
	pos[namelen + 1] = '\0';
	pos += namelen + 1;
	return pos;
}

static int count_subheaders(struct ctl_table *table)
{
	int has_files = 0;
	int nr_subheaders = 0;
	struct ctl_table *entry;

	/* special case: no directory and empty directory */
	if (!table || !table->procname)
		return 1;

	for (entry = table; entry->procname; entry++) {
		if (entry->child)
			nr_subheaders += count_subheaders(entry->child);
		else
			has_files = 1;
	}
	return nr_subheaders + has_files;
}

static int register_leaf_sysctl_tables(const char *path, char *pos,
	struct ctl_table_header ***subheader, struct ctl_table_set *set,
	struct ctl_table *table)
{
	struct ctl_table *ctl_table_arg = NULL;
	struct ctl_table *entry, *files;
	int nr_files = 0;
	int nr_dirs = 0;
	int err = -ENOMEM;

	for (entry = table; entry->procname; entry++) {
		if (entry->child)
			nr_dirs++;
		else
			nr_files++;
	}

	files = table;
	/* If there are mixed files and directories we need a new table */
	if (nr_dirs && nr_files) {
		struct ctl_table *new;
		files = kzalloc(sizeof(struct ctl_table) * (nr_files + 1),
				GFP_KERNEL);
		if (!files)
			goto out;

		ctl_table_arg = files;
		for (new = files, entry = table; entry->procname; entry++) {
			if (entry->child)
				continue;
			*new = *entry;
			new++;
		}
	}

	/* Register everything except a directory full of subdirectories */
	if (nr_files || !nr_dirs) {
		struct ctl_table_header *header;
		header = __register_sysctl_table(set, path, files);
		if (!header) {
			kfree(ctl_table_arg);
			goto out;
		}

		/* Remember if we need to free the file table */
		header->ctl_table_arg = ctl_table_arg;
		**subheader = header;
		(*subheader)++;
	}

	/* Recurse into the subdirectories. */
	for (entry = table; entry->procname; entry++) {
		char *child_pos;

		if (!entry->child)
			continue;

		err = -ENAMETOOLONG;
		child_pos = append_path(path, pos, entry->procname);
		if (!child_pos)
			goto out;

		err = register_leaf_sysctl_tables(path, child_pos, subheader,
						  set, entry->child);
		pos[0] = '\0';
		if (err)
			goto out;
	}
	err = 0;
out:
	/* On failure our caller will unregister all registered subheaders */
	return err;
}

/**
 * __register_sysctl_paths - register a sysctl table hierarchy
 * @set: Sysctl tree to register on
 * @path: The path to the directory the sysctl table is in.
 * @table: the top-level table structure
 *
 * Register a sysctl table hierarchy. @table should be a filled in ctl_table
 * array. A completely 0 filled entry terminates the table.
 *
 * See __register_sysctl_table for more details.
 */
struct ctl_table_header *__register_sysctl_paths(
	struct ctl_table_set *set,
	const struct ctl_path *path, struct ctl_table *table)
{
	struct ctl_table *ctl_table_arg = table;
	int nr_subheaders = count_subheaders(table);
	struct ctl_table_header *header = NULL, **subheaders, **subheader;
	const struct ctl_path *component;
	char *new_path, *pos;

	pos = new_path = kmalloc(PATH_MAX, GFP_KERNEL);
	if (!new_path)
		return NULL;

	pos[0] = '\0';
	for (component = path; component->procname; component++) {
		pos = append_path(new_path, pos, component->procname);
		if (!pos)
			goto out;
	}
	while (table->procname && table->child && !table[1].procname) {
		pos = append_path(new_path, pos, table->procname);
		if (!pos)
			goto out;
		table = table->child;
	}
	if (nr_subheaders == 1) {
		header = __register_sysctl_table(set, new_path, table);
		if (header)
			header->ctl_table_arg = ctl_table_arg;
	} else {
		header = kzalloc(sizeof(*header) +
				 sizeof(*subheaders)*nr_subheaders, GFP_KERNEL);
		if (!header)
			goto out;

		subheaders = (struct ctl_table_header **) (header + 1);
		subheader = subheaders;
		header->ctl_table_arg = ctl_table_arg;

		if (register_leaf_sysctl_tables(new_path, pos, &subheader,
						set, table))
			goto err_register_leaves;
	}

out:
	kfree(new_path);
	return header;

err_register_leaves:
	while (subheader > subheaders) {
		struct ctl_table_header *subh = *(--subheader);
		struct ctl_table *table = subh->ctl_table_arg;
		unregister_sysctl_table(subh);
		kfree(table);
	}
	kfree(header);
	header = NULL;
	goto out;
}

/**
 * register_sysctl_table_path - register a sysctl table hierarchy
 * @path: The path to the directory the sysctl table is in.
 * @table: the top-level table structure
 *
 * Register a sysctl table hierarchy. @table should be a filled in ctl_table
 * array. A completely 0 filled entry terminates the table.
 *
 * See __register_sysctl_paths for more details.
 */
struct ctl_table_header *register_sysctl_paths(const struct ctl_path *path,
						struct ctl_table *table)
{
	return __register_sysctl_paths(&sysctl_table_root.default_set,
					path, table);
}
EXPORT_SYMBOL(register_sysctl_paths);

/**
 * register_sysctl_table - register a sysctl table hierarchy
 * @table: the top-level table structure
 *
 * Register a sysctl table hierarchy. @table should be a filled in ctl_table
 * array. A completely 0 filled entry terminates the table.
 *
 * See register_sysctl_paths for more details.
 */
struct ctl_table_header *register_sysctl_table(struct ctl_table *table)
{
	static const struct ctl_path null_path[] = { {} };

	return register_sysctl_paths(null_path, table);
}
EXPORT_SYMBOL(register_sysctl_table);

=======
struct ctl_table_header *register_sysctl_sz(const char *path, struct ctl_table *table,
					    size_t table_size)
{
	return __register_sysctl_table(&sysctl_table_root.default_set,
					path, table, table_size);
}
EXPORT_SYMBOL(register_sysctl_sz);

/**
 * __register_sysctl_init() - register sysctl table to path
 * @path: path name for sysctl base. If that path doesn't exist we will create
 * 	it for you.
 * @table: This is the sysctl table that needs to be registered to the path.
 * 	The caller must ensure the life of the @table will be kept during the
 * 	lifetime use of the sysctl.
 * @table_name: The name of sysctl table, only used for log printing when
 *              registration fails
 * @table_size: The number of elements in table
 *
 * The sysctl interface is used by userspace to query or modify at runtime
 * a predefined value set on a variable. These variables however have default
 * values pre-set. Code which depends on these variables will always work even
 * if register_sysctl() fails. If register_sysctl() fails you'd just loose the
 * ability to query or modify the sysctls dynamically at run time. Chances of
 * register_sysctl() failing on init are extremely low, and so for both reasons
 * this function does not return any error as it is used by initialization code.
 *
 * Context: if your base directory does not exist it will be created for you.
 */
void __init __register_sysctl_init(const char *path, struct ctl_table *table,
				 const char *table_name, size_t table_size)
{
	struct ctl_table_header *hdr = register_sysctl_sz(path, table, table_size);

	if (unlikely(!hdr)) {
		pr_err("failed when register_sysctl_sz %s to %s\n", table_name, path);
		return;
	}
	kmemleak_not_leak(hdr);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void put_links(struct ctl_table_header *header)
{
	struct ctl_table_set *root_set = &sysctl_table_root.default_set;
	struct ctl_table_root *root = header->root;
	struct ctl_dir *parent = header->parent;
	struct ctl_dir *core_parent;
	struct ctl_table *entry;

	if (header->set == root_set)
		return;

	core_parent = xlate_dir(root_set, parent);
	if (IS_ERR(core_parent))
		return;

<<<<<<< HEAD
	for (entry = header->ctl_table; entry->procname; entry++) {
=======
	list_for_each_table_entry(entry, header) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct ctl_table_header *link_head;
		struct ctl_table *link;
		const char *name = entry->procname;

		link = find_entry(&link_head, core_parent, name, strlen(name));
		if (link &&
		    ((S_ISDIR(link->mode) && S_ISDIR(entry->mode)) ||
		     (S_ISLNK(link->mode) && (link->data == root)))) {
			drop_sysctl_table(link_head);
		}
		else {
<<<<<<< HEAD
			printk(KERN_ERR "sysctl link missing during unregister: ");
			sysctl_print_dir(parent);
			printk(KERN_CONT "/%s\n", name);
=======
			pr_err("sysctl link missing during unregister: ");
			sysctl_print_dir(parent);
			pr_cont("%s\n", name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
}

static void drop_sysctl_table(struct ctl_table_header *header)
{
	struct ctl_dir *parent = header->parent;

	if (--header->nreg)
		return;

<<<<<<< HEAD
	put_links(header);
	start_unregistering(header);
=======
	if (parent) {
		put_links(header);
		start_unregistering(header);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!--header->count)
		kfree_rcu(header, rcu);

	if (parent)
		drop_sysctl_table(&parent->header);
}

/**
 * unregister_sysctl_table - unregister a sysctl table hierarchy
<<<<<<< HEAD
 * @header: the header returned from register_sysctl_table
=======
 * @header: the header returned from register_sysctl or __register_sysctl_table
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Unregisters the sysctl table and all children. proc entries may not
 * actually be removed until they are no longer used by anyone.
 */
void unregister_sysctl_table(struct ctl_table_header * header)
{
<<<<<<< HEAD
	int nr_subheaders;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	might_sleep();

	if (header == NULL)
		return;

<<<<<<< HEAD
	nr_subheaders = count_subheaders(header->ctl_table_arg);
	if (unlikely(nr_subheaders > 1)) {
		struct ctl_table_header **subheaders;
		int i;

		subheaders = (struct ctl_table_header **)(header + 1);
		for (i = nr_subheaders -1; i >= 0; i--) {
			struct ctl_table_header *subh = subheaders[i];
			struct ctl_table *table = subh->ctl_table_arg;
			unregister_sysctl_table(subh);
			kfree(table);
		}
		kfree(header);
		return;
	}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_lock(&sysctl_lock);
	drop_sysctl_table(header);
	spin_unlock(&sysctl_lock);
}
EXPORT_SYMBOL(unregister_sysctl_table);

void setup_sysctl_set(struct ctl_table_set *set,
	struct ctl_table_root *root,
	int (*is_seen)(struct ctl_table_set *))
{
	memset(set, 0, sizeof(*set));
	set->is_seen = is_seen;
<<<<<<< HEAD
	init_header(&set->dir.header, root, set, NULL, root_table);
=======
	init_header(&set->dir.header, root, set, NULL, root_table, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void retire_sysctl_set(struct ctl_table_set *set)
{
	WARN_ON(!RB_EMPTY_ROOT(&set->dir.root));
}

int __init proc_sys_init(void)
{
	struct proc_dir_entry *proc_sys_root;

	proc_sys_root = proc_mkdir("sys", NULL);
	proc_sys_root->proc_iops = &proc_sys_dir_operations;
<<<<<<< HEAD
	proc_sys_root->proc_fops = &proc_sys_dir_file_operations;
	proc_sys_root->nlink = 0;

	return sysctl_init();
=======
	proc_sys_root->proc_dir_ops = &proc_sys_dir_file_operations;
	proc_sys_root->nlink = 0;

	return sysctl_init_bases();
}

struct sysctl_alias {
	const char *kernel_param;
	const char *sysctl_param;
};

/*
 * Historically some settings had both sysctl and a command line parameter.
 * With the generic sysctl. parameter support, we can handle them at a single
 * place and only keep the historical name for compatibility. This is not meant
 * to add brand new aliases. When adding existing aliases, consider whether
 * the possibly different moment of changing the value (e.g. from early_param
 * to the moment do_sysctl_args() is called) is an issue for the specific
 * parameter.
 */
static const struct sysctl_alias sysctl_aliases[] = {
	{"hardlockup_all_cpu_backtrace",	"kernel.hardlockup_all_cpu_backtrace" },
	{"hung_task_panic",			"kernel.hung_task_panic" },
	{"numa_zonelist_order",			"vm.numa_zonelist_order" },
	{"softlockup_all_cpu_backtrace",	"kernel.softlockup_all_cpu_backtrace" },
	{ }
};

static const char *sysctl_find_alias(char *param)
{
	const struct sysctl_alias *alias;

	for (alias = &sysctl_aliases[0]; alias->kernel_param != NULL; alias++) {
		if (strcmp(alias->kernel_param, param) == 0)
			return alias->sysctl_param;
	}

	return NULL;
}

bool sysctl_is_alias(char *param)
{
	const char *alias = sysctl_find_alias(param);

	return alias != NULL;
}

/* Set sysctl value passed on kernel command line. */
static int process_sysctl_arg(char *param, char *val,
			       const char *unused, void *arg)
{
	char *path;
	struct vfsmount **proc_mnt = arg;
	struct file_system_type *proc_fs_type;
	struct file *file;
	int len;
	int err;
	loff_t pos = 0;
	ssize_t wret;

	if (strncmp(param, "sysctl", sizeof("sysctl") - 1) == 0) {
		param += sizeof("sysctl") - 1;

		if (param[0] != '/' && param[0] != '.')
			return 0;

		param++;
	} else {
		param = (char *) sysctl_find_alias(param);
		if (!param)
			return 0;
	}

	if (!val)
		return -EINVAL;
	len = strlen(val);
	if (len == 0)
		return -EINVAL;

	/*
	 * To set sysctl options, we use a temporary mount of proc, look up the
	 * respective sys/ file and write to it. To avoid mounting it when no
	 * options were given, we mount it only when the first sysctl option is
	 * found. Why not a persistent mount? There are problems with a
	 * persistent mount of proc in that it forces userspace not to use any
	 * proc mount options.
	 */
	if (!*proc_mnt) {
		proc_fs_type = get_fs_type("proc");
		if (!proc_fs_type) {
			pr_err("Failed to find procfs to set sysctl from command line\n");
			return 0;
		}
		*proc_mnt = kern_mount(proc_fs_type);
		put_filesystem(proc_fs_type);
		if (IS_ERR(*proc_mnt)) {
			pr_err("Failed to mount procfs to set sysctl from command line\n");
			return 0;
		}
	}

	path = kasprintf(GFP_KERNEL, "sys/%s", param);
	if (!path)
		panic("%s: Failed to allocate path for %s\n", __func__, param);
	strreplace(path, '.', '/');

	file = file_open_root_mnt(*proc_mnt, path, O_WRONLY, 0);
	if (IS_ERR(file)) {
		err = PTR_ERR(file);
		if (err == -ENOENT)
			pr_err("Failed to set sysctl parameter '%s=%s': parameter not found\n",
				param, val);
		else if (err == -EACCES)
			pr_err("Failed to set sysctl parameter '%s=%s': permission denied (read-only?)\n",
				param, val);
		else
			pr_err("Error %pe opening proc file to set sysctl parameter '%s=%s'\n",
				file, param, val);
		goto out;
	}
	wret = kernel_write(file, val, len, &pos);
	if (wret < 0) {
		err = wret;
		if (err == -EINVAL)
			pr_err("Failed to set sysctl parameter '%s=%s': invalid value\n",
				param, val);
		else
			pr_err("Error %pe writing to proc file to set sysctl parameter '%s=%s'\n",
				ERR_PTR(err), param, val);
	} else if (wret != len) {
		pr_err("Wrote only %zd bytes of %d writing to proc file %s to set sysctl parameter '%s=%s\n",
			wret, len, path, param, val);
	}

	err = filp_close(file, NULL);
	if (err)
		pr_err("Error %pe closing proc file to set sysctl parameter '%s=%s\n",
			ERR_PTR(err), param, val);
out:
	kfree(path);
	return 0;
}

void do_sysctl_args(void)
{
	char *command_line;
	struct vfsmount *proc_mnt = NULL;

	command_line = kstrdup(saved_command_line, GFP_KERNEL);
	if (!command_line)
		panic("%s: Failed to allocate copy of command line\n", __func__);

	parse_args("Setting sysctl args", command_line,
		   NULL, 0, -1, -1, &proc_mnt, process_sysctl_arg);

	if (proc_mnt)
		kern_unmount(proc_mnt);

	kfree(command_line);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
