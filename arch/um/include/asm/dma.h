<<<<<<< HEAD
#ifndef __UM_DMA_H
#define __UM_DMA_H

#include "asm/io.h"
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __UM_DMA_H
#define __UM_DMA_H

#include <asm/io.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern unsigned long uml_physmem;

#define MAX_DMA_ADDRESS (uml_physmem)

#endif
