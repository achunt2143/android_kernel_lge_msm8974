<<<<<<< HEAD
/*
 * Implementation of the symbol table type.
 *
 * Author : Stephen Smalley, <sds@epoch.ncsc.mil>
 */
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Implementation of the symbol table type.
 *
 * Author : Stephen Smalley, <stephen.smalley.work@gmail.com>
 */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/errno.h>
#include "symtab.h"

<<<<<<< HEAD
static unsigned int symhash(struct hashtab *h, const void *key)
=======
static unsigned int symhash(const void *key)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const char *p, *keyp;
	unsigned int size;
	unsigned int val;

	val = 0;
	keyp = key;
	size = strlen(keyp);
	for (p = keyp; (p - keyp) < size; p++)
<<<<<<< HEAD
		val = (val << 4 | (val >> (8*sizeof(unsigned int)-4))) ^ (*p);
	return val & (h->size - 1);
}

static int symcmp(struct hashtab *h, const void *key1, const void *key2)
=======
		val = (val << 4 | (val >> (8 * sizeof(unsigned int) - 4))) ^
		      (*p);
	return val;
}

static int symcmp(const void *key1, const void *key2)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const char *keyp1, *keyp2;

	keyp1 = key1;
	keyp2 = key2;
	return strcmp(keyp1, keyp2);
}

<<<<<<< HEAD

int symtab_init(struct symtab *s, unsigned int size)
{
	s->table = hashtab_create(symhash, symcmp, size);
	if (!s->table)
		return -ENOMEM;
	s->nprim = 0;
	return 0;
}

=======
static const struct hashtab_key_params symtab_key_params = {
	.hash = symhash,
	.cmp = symcmp,
};

int symtab_init(struct symtab *s, u32 size)
{
	s->nprim = 0;
	return hashtab_init(&s->table, size);
}

int symtab_insert(struct symtab *s, char *name, void *datum)
{
	return hashtab_insert(&s->table, name, datum, symtab_key_params);
}

void *symtab_search(struct symtab *s, const char *name)
{
	return hashtab_search(&s->table, name, symtab_key_params);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
