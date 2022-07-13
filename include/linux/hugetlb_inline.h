<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _LINUX_HUGETLB_INLINE_H
#define _LINUX_HUGETLB_INLINE_H

#ifdef CONFIG_HUGETLB_PAGE

#include <linux/mm.h>

<<<<<<< HEAD
static inline int is_vm_hugetlb_page(struct vm_area_struct *vma)
=======
static inline bool is_vm_hugetlb_page(struct vm_area_struct *vma)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return !!(vma->vm_flags & VM_HUGETLB);
}

#else

<<<<<<< HEAD
static inline int is_vm_hugetlb_page(struct vm_area_struct *vma)
{
	return 0;
=======
static inline bool is_vm_hugetlb_page(struct vm_area_struct *vma)
{
	return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#endif

#endif
