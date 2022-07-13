<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _LINUX_FS_STRUCT_H
#define _LINUX_FS_STRUCT_H

#include <linux/path.h>
#include <linux/spinlock.h>
#include <linux/seqlock.h>

struct fs_struct {
	int users;
	spinlock_t lock;
<<<<<<< HEAD
	seqcount_t seq;
	int umask;
	int in_exec;
	struct path root, pwd;
};
=======
	seqcount_spinlock_t seq;
	int umask;
	int in_exec;
	struct path root, pwd;
} __randomize_layout;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern struct kmem_cache *fs_cachep;

extern void exit_fs(struct task_struct *);
<<<<<<< HEAD
extern void set_fs_root(struct fs_struct *, struct path *);
extern void set_fs_pwd(struct fs_struct *, struct path *);
extern struct fs_struct *copy_fs_struct(struct fs_struct *);
extern void free_fs_struct(struct fs_struct *);
extern void daemonize_fs_struct(void);
=======
extern void set_fs_root(struct fs_struct *, const struct path *);
extern void set_fs_pwd(struct fs_struct *, const struct path *);
extern struct fs_struct *copy_fs_struct(struct fs_struct *);
extern void free_fs_struct(struct fs_struct *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern int unshare_fs_struct(void);

static inline void get_fs_root(struct fs_struct *fs, struct path *root)
{
	spin_lock(&fs->lock);
	*root = fs->root;
	path_get(root);
	spin_unlock(&fs->lock);
}

static inline void get_fs_pwd(struct fs_struct *fs, struct path *pwd)
{
	spin_lock(&fs->lock);
	*pwd = fs->pwd;
	path_get(pwd);
	spin_unlock(&fs->lock);
}

<<<<<<< HEAD
static inline void get_fs_root_and_pwd(struct fs_struct *fs, struct path *root,
				       struct path *pwd)
{
	spin_lock(&fs->lock);
	*root = fs->root;
	path_get(root);
	*pwd = fs->pwd;
	path_get(pwd);
	spin_unlock(&fs->lock);
}
=======
extern bool current_chrooted(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _LINUX_FS_STRUCT_H */
