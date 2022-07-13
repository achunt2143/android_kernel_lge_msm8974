<<<<<<< HEAD
/* const.h: Macros for dealing with constants.  */

#ifndef _LINUX_CONST_H
#define _LINUX_CONST_H

/* Some constant macros are used in both assembler and
 * C code.  Therefore we cannot annotate them always with
 * 'UL' and other type specifiers unilaterally.  We
 * use the following macros to deal with this.
 *
 * Similarly, _AT() will cast an expression with a type in C, but
 * leave it unchanged in asm.
 */

#ifdef __ASSEMBLY__
#define _AC(X,Y)	X
#define _AT(T,X)	X
#else
#define __AC(X,Y)	(X##Y)
#define _AC(X,Y)	__AC(X,Y)
#define _AT(T,X)	((T)(X))
#endif

#endif /* !(_LINUX_CONST_H) */
=======
#ifndef _LINUX_CONST_H
#define _LINUX_CONST_H

#include <vdso/const.h>

#endif /* _LINUX_CONST_H */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
