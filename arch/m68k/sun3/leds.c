<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/contregs.h>
#include <asm/sun3mmu.h>
#include <asm/io.h>

<<<<<<< HEAD
=======
#include "sun3.h"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void sun3_leds(unsigned char byte)
{
	unsigned char dfc;

	GET_DFC(dfc);
	SET_DFC(FC_CONTROL);
	SET_CONTROL_BYTE(AC_LEDS, byte);
	SET_DFC(dfc);
}
