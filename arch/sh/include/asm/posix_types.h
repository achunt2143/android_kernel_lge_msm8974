<<<<<<< HEAD
#ifdef __KERNEL__
# ifdef CONFIG_SUPERH32
#  include "posix_types_32.h"
# else
#  include "posix_types_64.h"
# endif
#else
# ifdef __SH5__
#  include "posix_types_64.h"
# else
#  include "posix_types_32.h"
# endif
#endif /* __KERNEL__ */
=======
/* SPDX-License-Identifier: GPL-2.0 */
#include <asm/posix_types_32.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
