<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm-generic/audit_dir_write.h>
__NR_acct,
#ifdef __NR_swapon
__NR_swapon,
#endif
__NR_quotactl,
#ifdef __NR_truncate
__NR_truncate,
#endif
#ifdef __NR_truncate64
__NR_truncate64,
#endif
<<<<<<< HEAD
#ifdef __NR_bind
__NR_bind,		/* bind can affect fs object only in one way... */
#endif
=======
#ifdef __NR_ftruncate
__NR_ftruncate,
#endif
#ifdef __NR_ftruncate64
__NR_ftruncate64,
#endif
#ifdef __NR_bind
__NR_bind,		/* bind can affect fs object only in one way... */
#endif
#ifdef __NR_fallocate
__NR_fallocate,
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
