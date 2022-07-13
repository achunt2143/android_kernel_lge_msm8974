<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * A symbol table (symtab) maintains associations between symbol
 * strings and datum values.  The type of the datum values
 * is arbitrary.  The symbol table type is implemented
 * using the hash table type (hashtab).
 *
<<<<<<< HEAD
 * Author : Stephen Smalley, <sds@epoch.ncsc.mil>
 */
=======
 * Author : Stephen Smalley, <stephen.smalley.work@gmail.com>
 */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _SS_SYMTAB_H_
#define _SS_SYMTAB_H_

#include "hashtab.h"

struct symtab {
<<<<<<< HEAD
	struct hashtab *table;	/* hash table (keyed on a string) */
	u32 nprim;		/* number of primary names in table */
};

int symtab_init(struct symtab *s, unsigned int size);

#endif	/* _SS_SYMTAB_H_ */


=======
	struct hashtab table; /* hash table (keyed on a string) */
	u32 nprim; /* number of primary names in table */
};

int symtab_init(struct symtab *s, u32 size);

int symtab_insert(struct symtab *s, char *name, void *datum);
void *symtab_search(struct symtab *s, const char *name);

#endif /* _SS_SYMTAB_H_ */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
