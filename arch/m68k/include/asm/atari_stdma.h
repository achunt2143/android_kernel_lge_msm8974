<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifndef _atari_stdma_h
#define _atari_stdma_h


#include <linux/interrupt.h>


/***************************** Prototypes *****************************/

<<<<<<< HEAD
void stdma_lock(irq_handler_t handler, void *data);
void stdma_release( void );
int stdma_others_waiting( void );
int stdma_islocked( void );
void *stdma_locked_by( void );
=======
int stdma_try_lock(irq_handler_t, void *);
void stdma_lock(irq_handler_t handler, void *data);
void stdma_release( void );
int stdma_islocked( void );
int stdma_is_locked_by(irq_handler_t);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void stdma_init( void );

/************************* End of Prototypes **************************/



#endif  /* _atari_stdma_h */
