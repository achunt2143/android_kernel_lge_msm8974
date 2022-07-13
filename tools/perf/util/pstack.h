<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _PERF_PSTACK_
#define _PERF_PSTACK_

#include <stdbool.h>

struct pstack;
struct pstack *pstack__new(unsigned short max_nr_entries);
<<<<<<< HEAD
void pstack__delete(struct pstack *self);
bool pstack__empty(const struct pstack *self);
void pstack__remove(struct pstack *self, void *key);
void pstack__push(struct pstack *self, void *key);
void *pstack__pop(struct pstack *self);
=======
void pstack__delete(struct pstack *pstack);
bool pstack__empty(const struct pstack *pstack);
void pstack__remove(struct pstack *pstack, void *key);
void pstack__push(struct pstack *pstack, void *key);
void *pstack__pop(struct pstack *pstack);
void *pstack__peek(struct pstack *pstack);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _PERF_PSTACK_ */
