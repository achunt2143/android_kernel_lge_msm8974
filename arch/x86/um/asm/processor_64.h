/*
 * Copyright 2003 PathScale, Inc.
 *
 * Licensed under the GPL
 */

#ifndef __UM_PROCESSOR_X86_64_H
#define __UM_PROCESSOR_X86_64_H

struct arch_thread {
        unsigned long debugregs[8];
        int debugregs_seq;
<<<<<<< HEAD
        unsigned long fs;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
        struct faultinfo faultinfo;
};

#define INIT_ARCH_THREAD { .debugregs  		= { [ 0 ... 7 ] = 0 }, \
			   .debugregs_seq	= 0, \
<<<<<<< HEAD
			   .fs			= 0, \
			   .faultinfo		= { 0, 0, 0 } }

=======
			   .faultinfo		= { 0, 0, 0 } }

#define STACKSLOTS_PER_LINE 4

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void arch_flush_thread(struct arch_thread *thread)
{
}

static inline void arch_copy_thread(struct arch_thread *from,
                                    struct arch_thread *to)
{
<<<<<<< HEAD
	to->fs = from->fs;
}

#define current_text_addr() \
	({ void *pc; __asm__("movq $1f,%0\n1:":"=g" (pc)); pc; })
=======
}

#define current_sp() ({ void *sp; __asm__("movq %%rsp, %0" : "=r" (sp) : ); sp; })
#define current_bp() ({ unsigned long bp; __asm__("movq %%rbp, %0" : "=r" (bp) : ); bp; })
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
