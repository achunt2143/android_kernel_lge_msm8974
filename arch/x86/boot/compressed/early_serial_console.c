#include "misc.h"

<<<<<<< HEAD
int early_serial_base;
=======
/* This might be accessed before .bss is cleared, so use .data instead. */
int early_serial_base __section(".data");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "../early_serial_console.c"
