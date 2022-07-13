<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for the linux s390-specific parts of the memory manager.
#

<<<<<<< HEAD
obj-y	 := init.o fault.o extmem.o mmap.o vmem.o pgtable.o maccess.o \
	    page-states.o gup.o
obj-$(CONFIG_CMM) += cmm.o
obj-$(CONFIG_HUGETLB_PAGE) += hugetlbpage.o
obj-$(CONFIG_DEBUG_SET_MODULE_RONX) += pageattr.o
=======
obj-y		:= init.o fault.o extmem.o mmap.o vmem.o maccess.o
obj-y		+= page-states.o pageattr.o pgtable.o pgalloc.o extable.o

obj-$(CONFIG_CMM)		+= cmm.o
obj-$(CONFIG_DEBUG_VIRTUAL)	+= physaddr.o
obj-$(CONFIG_HUGETLB_PAGE)	+= hugetlbpage.o
obj-$(CONFIG_PTDUMP_CORE)	+= dump_pagetables.o
obj-$(CONFIG_PGSTE)		+= gmap.o
obj-$(CONFIG_PFAULT)		+= pfault.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
