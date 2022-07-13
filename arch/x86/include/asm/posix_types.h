<<<<<<< HEAD
#ifdef __KERNEL__
# ifdef CONFIG_X86_32
#  include "posix_types_32.h"
# else
#  include "posix_types_64.h"
# endif
#else
# ifdef __i386__
#  include "posix_types_32.h"
# elif defined(__ILP32__)
#  include "posix_types_x32.h"
# else
#  include "posix_types_64.h"
# endif
#endif
=======
/* SPDX-License-Identifier: GPL-2.0 */
# ifdef CONFIG_X86_32
#  include <asm/posix_types_32.h>
# else
#  include <asm/posix_types_64.h>
# endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
