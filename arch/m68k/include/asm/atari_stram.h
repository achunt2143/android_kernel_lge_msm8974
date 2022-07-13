<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _M68K_ATARI_STRAM_H
#define _M68K_ATARI_STRAM_H

/*
 * Functions for Atari ST-RAM management
 */

/* public interface */
void *atari_stram_alloc(unsigned long size, const char *owner);
void atari_stram_free(void *);
<<<<<<< HEAD
=======
void *atari_stram_to_virt(unsigned long phys);
unsigned long atari_stram_to_phys(void *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* functions called internally by other parts of the kernel */
void atari_stram_init(void);
void atari_stram_reserve_pages(void *start_mem);

#endif /*_M68K_ATARI_STRAM_H */
