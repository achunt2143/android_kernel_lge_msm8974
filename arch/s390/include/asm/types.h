<<<<<<< HEAD
/*
 *  include/asm-s390/types.h
 *
 *  S390 version
 *
 *  Derived from "include/asm-i386/types.h"
 */

#ifndef _S390_TYPES_H
#define _S390_TYPES_H

#include <asm-generic/int-ll64.h>

#ifndef __ASSEMBLY__

/* A address type so that arithmetic can be done on it & it can be upgraded to
   64 bit when necessary 
*/
typedef unsigned long addr_t; 
typedef __signed__ long saddr_t;

#endif /* __ASSEMBLY__ */

/*
 * These aren't exported outside the kernel to avoid name space clashes
 */
#ifdef __KERNEL__

#ifndef __ASSEMBLY__

#ifndef __s390x__
typedef union {
	unsigned long long pair;
	struct {
		unsigned long even;
		unsigned long odd;
	} subreg;
} register_pair;

#endif /* ! __s390x__   */
#endif /* __ASSEMBLY__  */
#endif /* __KERNEL__    */
#endif /* _S390_TYPES_H */
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef _ASM_S390_TYPES_H
#define _ASM_S390_TYPES_H

#include <uapi/asm/types.h>

#ifndef __ASSEMBLY__

union register_pair {
	unsigned __int128 pair;
	struct {
		unsigned long even;
		unsigned long odd;
	};
};

#endif /* __ASSEMBLY__ */
#endif /* _ASM_S390_TYPES_H */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
