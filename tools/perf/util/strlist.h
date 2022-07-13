<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __PERF_STRLIST_H
#define __PERF_STRLIST_H

#include <linux/rbtree.h>
#include <stdbool.h>

<<<<<<< HEAD
=======
#include "rblist.h"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct str_node {
	struct rb_node rb_node;
	const char     *s;
};

struct strlist {
<<<<<<< HEAD
	struct rb_root entries;
	unsigned int   nr_entries;
	bool	       dupstr;
};

struct strlist *strlist__new(bool dupstr, const char *slist);
void strlist__delete(struct strlist *self);

void strlist__remove(struct strlist *self, struct str_node *sn);
int strlist__load(struct strlist *self, const char *filename);
int strlist__add(struct strlist *self, const char *str);

struct str_node *strlist__entry(const struct strlist *self, unsigned int idx);
struct str_node *strlist__find(struct strlist *self, const char *entry);

static inline bool strlist__has_entry(struct strlist *self, const char *entry)
{
	return strlist__find(self, entry) != NULL;
}

static inline bool strlist__empty(const struct strlist *self)
{
	return self->nr_entries == 0;
}

static inline unsigned int strlist__nr_entries(const struct strlist *self)
{
	return self->nr_entries;
}

/* For strlist iteration */
static inline struct str_node *strlist__first(struct strlist *self)
{
	struct rb_node *rn = rb_first(&self->entries);
=======
	struct rblist rblist;
	bool	      dupstr;
	bool	      file_only;
};

/*
 * @file_only: When dirname is present, only consider entries as filenames,
 *             that should not be added to the list if dirname/entry is not
 *             found
 */
struct strlist_config {
	bool dont_dupstr;
	bool file_only;
	const char *dirname;
};

struct strlist *strlist__new(const char *slist, const struct strlist_config *config);
void strlist__delete(struct strlist *slist);

void strlist__remove(struct strlist *slist, struct str_node *sn);
int strlist__load(struct strlist *slist, const char *filename);
int strlist__add(struct strlist *slist, const char *str);

struct str_node *strlist__entry(const struct strlist *slist, unsigned int idx);
struct str_node *strlist__find(struct strlist *slist, const char *entry);

static inline bool strlist__has_entry(struct strlist *slist, const char *entry)
{
	return strlist__find(slist, entry) != NULL;
}

static inline bool strlist__empty(const struct strlist *slist)
{
	return rblist__empty(&slist->rblist);
}

static inline unsigned int strlist__nr_entries(const struct strlist *slist)
{
	return rblist__nr_entries(&slist->rblist);
}

/* For strlist iteration */
static inline struct str_node *strlist__first(struct strlist *slist)
{
	struct rb_node *rn = rb_first_cached(&slist->rblist.entries);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rn ? rb_entry(rn, struct str_node, rb_node) : NULL;
}
static inline struct str_node *strlist__next(struct str_node *sn)
{
	struct rb_node *rn;
	if (!sn)
		return NULL;
	rn = rb_next(&sn->rb_node);
	return rn ? rb_entry(rn, struct str_node, rb_node) : NULL;
}

/**
 * strlist_for_each      - iterate over a strlist
 * @pos:	the &struct str_node to use as a loop cursor.
<<<<<<< HEAD
 * @self:	the &struct strlist for loop.
 */
#define strlist__for_each(pos, self)	\
	for (pos = strlist__first(self); pos; pos = strlist__next(pos))
=======
 * @slist:	the &struct strlist for loop.
 */
#define strlist__for_each_entry(pos, slist)	\
	for (pos = strlist__first(slist); pos; pos = strlist__next(pos))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * strlist_for_each_safe - iterate over a strlist safe against removal of
 *                         str_node
 * @pos:	the &struct str_node to use as a loop cursor.
 * @n:		another &struct str_node to use as temporary storage.
<<<<<<< HEAD
 * @self:	the &struct strlist for loop.
 */
#define strlist__for_each_safe(pos, n, self)	\
	for (pos = strlist__first(self), n = strlist__next(pos); pos;\
	     pos = n, n = strlist__next(n))

int strlist__parse_list(struct strlist *self, const char *s);
=======
 * @slist:	the &struct strlist for loop.
 */
#define strlist__for_each_entry_safe(pos, n, slist)	\
	for (pos = strlist__first(slist), n = strlist__next(pos); pos;\
	     pos = n, n = strlist__next(n))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __PERF_STRLIST_H */
