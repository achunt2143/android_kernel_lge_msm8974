<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Simple pointer stack
 *
 * (c) 2010 Arnaldo Carvalho de Melo <acme@redhat.com>
 */

<<<<<<< HEAD
#include "util.h"
#include "pstack.h"
#include <linux/kernel.h>
#include <stdlib.h>
=======
#include "pstack.h"
#include "debug.h"
#include <linux/kernel.h>
#include <linux/zalloc.h>
#include <stdlib.h>
#include <string.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct pstack {
	unsigned short	top;
	unsigned short	max_nr_entries;
<<<<<<< HEAD
	void		*entries[0];
=======
	void		*entries[];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct pstack *pstack__new(unsigned short max_nr_entries)
{
<<<<<<< HEAD
	struct pstack *self = zalloc((sizeof(*self) +
				     max_nr_entries * sizeof(void *)));
	if (self != NULL)
		self->max_nr_entries = max_nr_entries;
	return self;
}

void pstack__delete(struct pstack *self)
{
	free(self);
}

bool pstack__empty(const struct pstack *self)
{
	return self->top == 0;
}

void pstack__remove(struct pstack *self, void *key)
{
	unsigned short i = self->top, last_index = self->top - 1;

	while (i-- != 0) {
		if (self->entries[i] == key) {
			if (i < last_index)
				memmove(self->entries + i,
					self->entries + i + 1,
					(last_index - i) * sizeof(void *));
			--self->top;
=======
	struct pstack *pstack = zalloc((sizeof(*pstack) +
				       max_nr_entries * sizeof(void *)));
	if (pstack != NULL)
		pstack->max_nr_entries = max_nr_entries;
	return pstack;
}

void pstack__delete(struct pstack *pstack)
{
	free(pstack);
}

bool pstack__empty(const struct pstack *pstack)
{
	return pstack->top == 0;
}

void pstack__remove(struct pstack *pstack, void *key)
{
	unsigned short i = pstack->top, last_index = pstack->top - 1;

	while (i-- != 0) {
		if (pstack->entries[i] == key) {
			if (i < last_index)
				memmove(pstack->entries + i,
					pstack->entries + i + 1,
					(last_index - i) * sizeof(void *));
			--pstack->top;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return;
		}
	}
	pr_err("%s: %p not on the pstack!\n", __func__, key);
}

<<<<<<< HEAD
void pstack__push(struct pstack *self, void *key)
{
	if (self->top == self->max_nr_entries) {
		pr_err("%s: top=%d, overflow!\n", __func__, self->top);
		return;
	}
	self->entries[self->top++] = key;
}

void *pstack__pop(struct pstack *self)
{
	void *ret;

	if (self->top == 0) {
=======
void pstack__push(struct pstack *pstack, void *key)
{
	if (pstack->top == pstack->max_nr_entries) {
		pr_err("%s: top=%d, overflow!\n", __func__, pstack->top);
		return;
	}
	pstack->entries[pstack->top++] = key;
}

void *pstack__pop(struct pstack *pstack)
{
	void *ret;

	if (pstack->top == 0) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		pr_err("%s: underflow!\n", __func__);
		return NULL;
	}

<<<<<<< HEAD
	ret = self->entries[--self->top];
	self->entries[self->top] = NULL;
	return ret;
}
=======
	ret = pstack->entries[--pstack->top];
	pstack->entries[pstack->top] = NULL;
	return ret;
}

void *pstack__peek(struct pstack *pstack)
{
	if (pstack->top == 0)
		return NULL;
	return pstack->entries[pstack->top - 1];
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
