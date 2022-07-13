<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Derived from arch/ppc/mm/extable.c and arch/i386/mm/extable.c.
 *
 * Copyright (C) 2004 Paul Mackerras, IBM Corp.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/sort.h>
#include <asm/uaccess.h>

#ifndef ARCH_HAS_SORT_EXTABLE
=======
 */

#include <linux/bsearch.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sort.h>
#include <linux/uaccess.h>
#include <linux/extable.h>

#ifndef ARCH_HAS_RELATIVE_EXTABLE
#define ex_to_insn(x)	((x)->insn)
#else
static inline unsigned long ex_to_insn(const struct exception_table_entry *x)
{
	return (unsigned long)&x->insn + x->insn;
}
#endif

#ifndef ARCH_HAS_RELATIVE_EXTABLE
#define swap_ex		NULL
#else
static void swap_ex(void *a, void *b, int size)
{
	struct exception_table_entry *x = a, *y = b, tmp;
	int delta = b - a;

	tmp = *x;
	x->insn = y->insn + delta;
	y->insn = tmp.insn - delta;

#ifdef swap_ex_entry_fixup
	swap_ex_entry_fixup(x, y, tmp, delta);
#else
	x->fixup = y->fixup + delta;
	y->fixup = tmp.fixup - delta;
#endif
}
#endif /* ARCH_HAS_RELATIVE_EXTABLE */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * The exception table needs to be sorted so that the binary
 * search that we use to find entries in it works properly.
 * This is used both for the kernel exception table and for
 * the exception tables of modules that get loaded.
 */
<<<<<<< HEAD
static int cmp_ex(const void *a, const void *b)
=======
static int cmp_ex_sort(const void *a, const void *b)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const struct exception_table_entry *x = a, *y = b;

	/* avoid overflow */
<<<<<<< HEAD
	if (x->insn > y->insn)
		return 1;
	if (x->insn < y->insn)
=======
	if (ex_to_insn(x) > ex_to_insn(y))
		return 1;
	if (ex_to_insn(x) < ex_to_insn(y))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -1;
	return 0;
}

void sort_extable(struct exception_table_entry *start,
		  struct exception_table_entry *finish)
{
	sort(start, finish - start, sizeof(struct exception_table_entry),
<<<<<<< HEAD
	     cmp_ex, NULL);
=======
	     cmp_ex_sort, swap_ex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifdef CONFIG_MODULES
/*
 * If the exception table is sorted, any referring to the module init
 * will be at the beginning or the end.
 */
void trim_init_extable(struct module *m)
{
	/*trim the beginning*/
<<<<<<< HEAD
	while (m->num_exentries && within_module_init(m->extable[0].insn, m)) {
=======
	while (m->num_exentries &&
	       within_module_init(ex_to_insn(&m->extable[0]), m)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		m->extable++;
		m->num_exentries--;
	}
	/*trim the end*/
	while (m->num_exentries &&
<<<<<<< HEAD
		within_module_init(m->extable[m->num_exentries-1].insn, m))
		m->num_exentries--;
}
#endif /* CONFIG_MODULES */
#endif /* !ARCH_HAS_SORT_EXTABLE */

#ifndef ARCH_HAS_SEARCH_EXTABLE
=======
	       within_module_init(ex_to_insn(&m->extable[m->num_exentries - 1]),
				  m))
		m->num_exentries--;
}
#endif /* CONFIG_MODULES */

static int cmp_ex_search(const void *key, const void *elt)
{
	const struct exception_table_entry *_elt = elt;
	unsigned long _key = *(unsigned long *)key;

	/* avoid overflow */
	if (_key > ex_to_insn(_elt))
		return 1;
	if (_key < ex_to_insn(_elt))
		return -1;
	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Search one exception table for an entry corresponding to the
 * given instruction address, and return the address of the entry,
 * or NULL if none is found.
 * We use a binary search, and thus we assume that the table is
 * already sorted.
 */
const struct exception_table_entry *
<<<<<<< HEAD
search_extable(const struct exception_table_entry *first,
	       const struct exception_table_entry *last,
	       unsigned long value)
{
	while (first <= last) {
		const struct exception_table_entry *mid;

		mid = ((last - first) >> 1) + first;
		/*
		 * careful, the distance between value and insn
		 * can be larger than MAX_LONG:
		 */
		if (mid->insn < value)
			first = mid + 1;
		else if (mid->insn > value)
			last = mid - 1;
		else
			return mid;
        }
        return NULL;
}
#endif
=======
search_extable(const struct exception_table_entry *base,
	       const size_t num,
	       unsigned long value)
{
	return bsearch(&value, base, num,
		       sizeof(struct exception_table_entry), cmp_ex_search);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
