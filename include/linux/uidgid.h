<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _LINUX_UIDGID_H
#define _LINUX_UIDGID_H

/*
 * A set of types for the internal kernel types representing uids and gids.
 *
 * The types defined in this header allow distinguishing which uids and gids in
 * the kernel are values used by userspace and which uid and gid values are
 * the internal kernel values.  With the addition of user namespaces the values
 * can be different.  Using the type system makes it possible for the compiler
 * to detect when we overlook these differences.
 *
 */
<<<<<<< HEAD
#include <linux/types.h>
=======
#include <linux/uidgid_types.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/highuid.h>

struct user_namespace;
extern struct user_namespace init_user_ns;
<<<<<<< HEAD

#if defined(NOTYET)

typedef struct {
	uid_t val;
} kuid_t;


typedef struct {
	gid_t val;
} kgid_t;
=======
struct uid_gid_map;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define KUIDT_INIT(value) (kuid_t){ value }
#define KGIDT_INIT(value) (kgid_t){ value }

<<<<<<< HEAD
=======
#ifdef CONFIG_MULTIUSER
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline uid_t __kuid_val(kuid_t uid)
{
	return uid.val;
}

static inline gid_t __kgid_val(kgid_t gid)
{
	return gid.val;
}
<<<<<<< HEAD

#else

typedef uid_t kuid_t;
typedef gid_t kgid_t;

static inline uid_t __kuid_val(kuid_t uid)
{
	return uid;
=======
#else
static inline uid_t __kuid_val(kuid_t uid)
{
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline gid_t __kgid_val(kgid_t gid)
{
<<<<<<< HEAD
	return gid;
}

#define KUIDT_INIT(value) ((kuid_t) value )
#define KGIDT_INIT(value) ((kgid_t) value )

=======
	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#define GLOBAL_ROOT_UID KUIDT_INIT(0)
#define GLOBAL_ROOT_GID KGIDT_INIT(0)

#define INVALID_UID KUIDT_INIT(-1)
#define INVALID_GID KGIDT_INIT(-1)

static inline bool uid_eq(kuid_t left, kuid_t right)
{
	return __kuid_val(left) == __kuid_val(right);
}

static inline bool gid_eq(kgid_t left, kgid_t right)
{
	return __kgid_val(left) == __kgid_val(right);
}

static inline bool uid_gt(kuid_t left, kuid_t right)
{
	return __kuid_val(left) > __kuid_val(right);
}

static inline bool gid_gt(kgid_t left, kgid_t right)
{
	return __kgid_val(left) > __kgid_val(right);
}

static inline bool uid_gte(kuid_t left, kuid_t right)
{
	return __kuid_val(left) >= __kuid_val(right);
}

static inline bool gid_gte(kgid_t left, kgid_t right)
{
	return __kgid_val(left) >= __kgid_val(right);
}

static inline bool uid_lt(kuid_t left, kuid_t right)
{
	return __kuid_val(left) < __kuid_val(right);
}

static inline bool gid_lt(kgid_t left, kgid_t right)
{
	return __kgid_val(left) < __kgid_val(right);
}

static inline bool uid_lte(kuid_t left, kuid_t right)
{
	return __kuid_val(left) <= __kuid_val(right);
}

static inline bool gid_lte(kgid_t left, kgid_t right)
{
	return __kgid_val(left) <= __kgid_val(right);
}

static inline bool uid_valid(kuid_t uid)
{
<<<<<<< HEAD
	return !uid_eq(uid, INVALID_UID);
=======
	return __kuid_val(uid) != (uid_t) -1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline bool gid_valid(kgid_t gid)
{
<<<<<<< HEAD
	return !gid_eq(gid, INVALID_GID);
}

=======
	return __kgid_val(gid) != (gid_t) -1;
}

#ifdef CONFIG_USER_NS

extern kuid_t make_kuid(struct user_namespace *from, uid_t uid);
extern kgid_t make_kgid(struct user_namespace *from, gid_t gid);

extern uid_t from_kuid(struct user_namespace *to, kuid_t uid);
extern gid_t from_kgid(struct user_namespace *to, kgid_t gid);
extern uid_t from_kuid_munged(struct user_namespace *to, kuid_t uid);
extern gid_t from_kgid_munged(struct user_namespace *to, kgid_t gid);

static inline bool kuid_has_mapping(struct user_namespace *ns, kuid_t uid)
{
	return from_kuid(ns, uid) != (uid_t) -1;
}

static inline bool kgid_has_mapping(struct user_namespace *ns, kgid_t gid)
{
	return from_kgid(ns, gid) != (gid_t) -1;
}

u32 map_id_down(struct uid_gid_map *map, u32 id);
u32 map_id_up(struct uid_gid_map *map, u32 id);

#else

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline kuid_t make_kuid(struct user_namespace *from, uid_t uid)
{
	return KUIDT_INIT(uid);
}

static inline kgid_t make_kgid(struct user_namespace *from, gid_t gid)
{
	return KGIDT_INIT(gid);
}

static inline uid_t from_kuid(struct user_namespace *to, kuid_t kuid)
{
	return __kuid_val(kuid);
}

static inline gid_t from_kgid(struct user_namespace *to, kgid_t kgid)
{
	return __kgid_val(kgid);
}

static inline uid_t from_kuid_munged(struct user_namespace *to, kuid_t kuid)
{
	uid_t uid = from_kuid(to, kuid);
	if (uid == (uid_t)-1)
		uid = overflowuid;
	return uid;
}

static inline gid_t from_kgid_munged(struct user_namespace *to, kgid_t kgid)
{
	gid_t gid = from_kgid(to, kgid);
	if (gid == (gid_t)-1)
		gid = overflowgid;
	return gid;
}

static inline bool kuid_has_mapping(struct user_namespace *ns, kuid_t uid)
{
<<<<<<< HEAD
	return true;
=======
	return uid_valid(uid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline bool kgid_has_mapping(struct user_namespace *ns, kgid_t gid)
{
<<<<<<< HEAD
	return true;
}

=======
	return gid_valid(gid);
}

static inline u32 map_id_down(struct uid_gid_map *map, u32 id)
{
	return id;
}

static inline u32 map_id_up(struct uid_gid_map *map, u32 id)
{
	return id;
}
#endif /* CONFIG_USER_NS */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _LINUX_UIDGID_H */
