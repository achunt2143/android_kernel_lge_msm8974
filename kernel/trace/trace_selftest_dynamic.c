<<<<<<< HEAD
#include "trace.h"

int DYN_FTRACE_TEST_NAME(void)
=======
// SPDX-License-Identifier: GPL-2.0
#include <linux/compiler.h>
#include "trace.h"

noinline __noclone int DYN_FTRACE_TEST_NAME(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* used to call mcount */
	return 0;
}

<<<<<<< HEAD
int DYN_FTRACE_TEST_NAME2(void)
=======
noinline __noclone int DYN_FTRACE_TEST_NAME2(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* used to call mcount */
	return 0;
}
