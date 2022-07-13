<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Supplementary group IDs
 */
#include <linux/cred.h>
#include <linux/export.h>
#include <linux/slab.h>
#include <linux/security.h>
<<<<<<< HEAD
#include <linux/syscalls.h>
#include <asm/uaccess.h>

/* init to 2 - one for init_task, one to ensure it is never freed */
struct group_info init_groups = { .usage = ATOMIC_INIT(2) };

struct group_info *groups_alloc(int gidsetsize)
{
	struct group_info *group_info;
	int nblocks;
	int i;

	nblocks = (gidsetsize + NGROUPS_PER_BLOCK - 1) / NGROUPS_PER_BLOCK;
	/* Make sure we always allocate at least one indirect block pointer */
	nblocks = nblocks ? : 1;
	group_info = kmalloc(sizeof(*group_info) + nblocks*sizeof(gid_t *), GFP_USER);
	if (!group_info)
		return NULL;
	group_info->ngroups = gidsetsize;
	group_info->nblocks = nblocks;
	atomic_set(&group_info->usage, 1);

	if (gidsetsize <= NGROUPS_SMALL)
		group_info->blocks[0] = group_info->small_block;
	else {
		for (i = 0; i < nblocks; i++) {
			gid_t *b;
			b = (void *)__get_free_page(GFP_USER);
			if (!b)
				goto out_undo_partial_alloc;
			group_info->blocks[i] = b;
		}
	}
	return group_info;

out_undo_partial_alloc:
	while (--i >= 0) {
		free_page((unsigned long)group_info->blocks[i]);
	}
	kfree(group_info);
	return NULL;
=======
#include <linux/sort.h>
#include <linux/syscalls.h>
#include <linux/user_namespace.h>
#include <linux/vmalloc.h>
#include <linux/uaccess.h>

struct group_info *groups_alloc(int gidsetsize)
{
	struct group_info *gi;
	gi = kvmalloc(struct_size(gi, gid, gidsetsize), GFP_KERNEL_ACCOUNT);
	if (!gi)
		return NULL;

	refcount_set(&gi->usage, 1);
	gi->ngroups = gidsetsize;
	return gi;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

EXPORT_SYMBOL(groups_alloc);

void groups_free(struct group_info *group_info)
{
<<<<<<< HEAD
	if (group_info->blocks[0] != group_info->small_block) {
		int i;
		for (i = 0; i < group_info->nblocks; i++)
			free_page((unsigned long)group_info->blocks[i]);
	}
	kfree(group_info);
=======
	kvfree(group_info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

EXPORT_SYMBOL(groups_free);

/* export the group_info to a user-space array */
static int groups_to_user(gid_t __user *grouplist,
			  const struct group_info *group_info)
{
<<<<<<< HEAD
	int i;
	unsigned int count = group_info->ngroups;

	for (i = 0; i < group_info->nblocks; i++) {
		unsigned int cp_count = min(NGROUPS_PER_BLOCK, count);
		unsigned int len = cp_count * sizeof(*grouplist);

		if (copy_to_user(grouplist, group_info->blocks[i], len))
			return -EFAULT;

		grouplist += NGROUPS_PER_BLOCK;
		count -= cp_count;
=======
	struct user_namespace *user_ns = current_user_ns();
	int i;
	unsigned int count = group_info->ngroups;

	for (i = 0; i < count; i++) {
		gid_t gid;
		gid = from_kgid_munged(user_ns, group_info->gid[i]);
		if (put_user(gid, grouplist+i))
			return -EFAULT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return 0;
}

/* fill a group_info from a user-space array - it must be allocated already */
static int groups_from_user(struct group_info *group_info,
    gid_t __user *grouplist)
{
<<<<<<< HEAD
	int i;
	unsigned int count = group_info->ngroups;

	for (i = 0; i < group_info->nblocks; i++) {
		unsigned int cp_count = min(NGROUPS_PER_BLOCK, count);
		unsigned int len = cp_count * sizeof(*grouplist);

		if (copy_from_user(group_info->blocks[i], grouplist, len))
			return -EFAULT;

		grouplist += NGROUPS_PER_BLOCK;
		count -= cp_count;
=======
	struct user_namespace *user_ns = current_user_ns();
	int i;
	unsigned int count = group_info->ngroups;

	for (i = 0; i < count; i++) {
		gid_t gid;
		kgid_t kgid;
		if (get_user(gid, grouplist+i))
			return -EFAULT;

		kgid = make_kgid(user_ns, gid);
		if (!gid_valid(kgid))
			return -EINVAL;

		group_info->gid[i] = kgid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return 0;
}

<<<<<<< HEAD
/* a simple Shell sort */
static void groups_sort(struct group_info *group_info)
{
	int base, max, stride;
	int gidsetsize = group_info->ngroups;

	for (stride = 1; stride < gidsetsize; stride = 3 * stride + 1)
		; /* nothing */
	stride /= 3;

	while (stride) {
		max = gidsetsize - stride;
		for (base = 0; base < max; base++) {
			int left = base;
			int right = left + stride;
			gid_t tmp = GROUP_AT(group_info, right);

			while (left >= 0 && GROUP_AT(group_info, left) > tmp) {
				GROUP_AT(group_info, right) =
				    GROUP_AT(group_info, left);
				right = left;
				left -= stride;
			}
			GROUP_AT(group_info, right) = tmp;
		}
		stride /= 3;
	}
}

/* a simple bsearch */
int groups_search(const struct group_info *group_info, gid_t grp)
=======
static int gid_cmp(const void *_a, const void *_b)
{
	kgid_t a = *(kgid_t *)_a;
	kgid_t b = *(kgid_t *)_b;

	return gid_gt(a, b) - gid_lt(a, b);
}

void groups_sort(struct group_info *group_info)
{
	sort(group_info->gid, group_info->ngroups, sizeof(*group_info->gid),
	     gid_cmp, NULL);
}
EXPORT_SYMBOL(groups_sort);

/* a simple bsearch */
int groups_search(const struct group_info *group_info, kgid_t grp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned int left, right;

	if (!group_info)
		return 0;

	left = 0;
	right = group_info->ngroups;
	while (left < right) {
		unsigned int mid = (left+right)/2;
<<<<<<< HEAD
		if (grp > GROUP_AT(group_info, mid))
			left = mid + 1;
		else if (grp < GROUP_AT(group_info, mid))
=======
		if (gid_gt(grp, group_info->gid[mid]))
			left = mid + 1;
		else if (gid_lt(grp, group_info->gid[mid]))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			right = mid;
		else
			return 1;
	}
	return 0;
}

/**
 * set_groups - Change a group subscription in a set of credentials
 * @new: The newly prepared set of credentials to alter
 * @group_info: The group list to install
<<<<<<< HEAD
 *
 * Validate a group subscription and, if valid, insert it into a set
 * of credentials.
 */
int set_groups(struct cred *new, struct group_info *group_info)
{
	put_group_info(new->group_info);
	groups_sort(group_info);
	get_group_info(group_info);
	new->group_info = group_info;
	return 0;
=======
 */
void set_groups(struct cred *new, struct group_info *group_info)
{
	put_group_info(new->group_info);
	get_group_info(group_info);
	new->group_info = group_info;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

EXPORT_SYMBOL(set_groups);

/**
 * set_current_groups - Change current's group subscription
 * @group_info: The group list to impose
 *
 * Validate a group subscription and, if valid, impose it upon current's task
 * security record.
 */
int set_current_groups(struct group_info *group_info)
{
	struct cred *new;
<<<<<<< HEAD
	int ret;
=======
	const struct cred *old;
	int retval;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	new = prepare_creds();
	if (!new)
		return -ENOMEM;

<<<<<<< HEAD
	ret = set_groups(new, group_info);
	if (ret < 0) {
		abort_creds(new);
		return ret;
	}

	return commit_creds(new);
=======
	old = current_cred();

	set_groups(new, group_info);

	retval = security_task_fix_setgroups(new, old);
	if (retval < 0)
		goto error;

	return commit_creds(new);

error:
	abort_creds(new);
	return retval;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

EXPORT_SYMBOL(set_current_groups);

SYSCALL_DEFINE2(getgroups, int, gidsetsize, gid_t __user *, grouplist)
{
	const struct cred *cred = current_cred();
	int i;

	if (gidsetsize < 0)
		return -EINVAL;

	/* no need to grab task_lock here; it cannot change */
	i = cred->group_info->ngroups;
	if (gidsetsize) {
		if (i > gidsetsize) {
			i = -EINVAL;
			goto out;
		}
		if (groups_to_user(grouplist, cred->group_info)) {
			i = -EFAULT;
			goto out;
		}
	}
out:
	return i;
}

<<<<<<< HEAD
=======
bool may_setgroups(void)
{
	struct user_namespace *user_ns = current_user_ns();

	return ns_capable_setid(user_ns, CAP_SETGID) &&
		userns_may_setgroups(user_ns);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	SMP: Our groups are copy-on-write. We can set them safely
 *	without another task interfering.
 */

SYSCALL_DEFINE2(setgroups, int, gidsetsize, gid_t __user *, grouplist)
{
	struct group_info *group_info;
	int retval;

<<<<<<< HEAD
	if (!nsown_capable(CAP_SETGID))
=======
	if (!may_setgroups())
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EPERM;
	if ((unsigned)gidsetsize > NGROUPS_MAX)
		return -EINVAL;

	group_info = groups_alloc(gidsetsize);
	if (!group_info)
		return -ENOMEM;
	retval = groups_from_user(group_info, grouplist);
	if (retval) {
		put_group_info(group_info);
		return retval;
	}

<<<<<<< HEAD
=======
	groups_sort(group_info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	retval = set_current_groups(group_info);
	put_group_info(group_info);

	return retval;
}

/*
 * Check whether we're fsgid/egid or in the supplemental group..
 */
<<<<<<< HEAD
int in_group_p(gid_t grp)
=======
int in_group_p(kgid_t grp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const struct cred *cred = current_cred();
	int retval = 1;

<<<<<<< HEAD
	if (grp != cred->fsgid)
=======
	if (!gid_eq(grp, cred->fsgid))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		retval = groups_search(cred->group_info, grp);
	return retval;
}

EXPORT_SYMBOL(in_group_p);

<<<<<<< HEAD
int in_egroup_p(gid_t grp)
=======
int in_egroup_p(kgid_t grp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const struct cred *cred = current_cred();
	int retval = 1;

<<<<<<< HEAD
	if (grp != cred->egid)
=======
	if (!gid_eq(grp, cred->egid))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		retval = groups_search(cred->group_info, grp);
	return retval;
}

EXPORT_SYMBOL(in_egroup_p);
