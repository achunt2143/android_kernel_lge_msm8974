<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _LINUX_PATH_H
#define _LINUX_PATH_H

struct dentry;
struct vfsmount;

struct path {
	struct vfsmount *mnt;
	struct dentry *dentry;
<<<<<<< HEAD
};

extern void path_get(struct path *);
extern void path_put(struct path *);
=======
} __randomize_layout;

extern void path_get(const struct path *);
extern void path_put(const struct path *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline int path_equal(const struct path *path1, const struct path *path2)
{
	return path1->mnt == path2->mnt && path1->dentry == path2->dentry;
}

<<<<<<< HEAD
=======
static inline void path_put_init(struct path *path)
{
	path_put(path);
	*path = (struct path) { };
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif  /* _LINUX_PATH_H */
