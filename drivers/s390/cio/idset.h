<<<<<<< HEAD
/*
 *  drivers/s390/cio/idset.h
 *
 *    Copyright IBM Corp. 2007
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 *    Copyright IBM Corp. 2007, 2012
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *    Author(s): Peter Oberparleiter <peter.oberparleiter@de.ibm.com>
 */

#ifndef S390_IDSET_H
<<<<<<< HEAD
#define S390_IDSET_H S390_IDSET_H
=======
#define S390_IDSET_H
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <asm/schid.h>

struct idset;

void idset_free(struct idset *set);
<<<<<<< HEAD
void idset_clear(struct idset *set);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void idset_fill(struct idset *set);

struct idset *idset_sch_new(void);
void idset_sch_add(struct idset *set, struct subchannel_id id);
void idset_sch_del(struct idset *set, struct subchannel_id id);
<<<<<<< HEAD
int idset_sch_contains(struct idset *set, struct subchannel_id id);
int idset_sch_get_first(struct idset *set, struct subchannel_id *id);
=======
void idset_sch_del_subseq(struct idset *set, struct subchannel_id schid);
int idset_sch_contains(struct idset *set, struct subchannel_id id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int idset_is_empty(struct idset *set);
void idset_add_set(struct idset *to, struct idset *from);

#endif /* S390_IDSET_H */
