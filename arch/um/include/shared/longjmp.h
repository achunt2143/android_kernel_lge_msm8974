<<<<<<< HEAD
#ifndef __UML_LONGJMP_H
#define __UML_LONGJMP_H

#include "sysdep/archsetjmp.h"
#include "os.h"

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __UML_LONGJMP_H
#define __UML_LONGJMP_H

#include <sysdep/archsetjmp.h>
#include <os.h>

extern int signals_enabled;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern int setjmp(jmp_buf);
extern void longjmp(jmp_buf, int);

#define UML_LONGJMP(buf, val) do { \
	longjmp(*buf, val);	\
} while(0)

<<<<<<< HEAD
#define UML_SETJMP(buf) ({ \
	int n;	   \
	volatile int enable;	\
	enable = get_signals(); \
	n = setjmp(*buf); \
	if(n != 0) \
		set_signals(enable); \
=======
#define UML_SETJMP(buf) ({				\
	int n, enable;					\
	enable = *(volatile int *)&signals_enabled;	\
	n = setjmp(*buf);				\
	if(n != 0)					\
		um_set_signals_trace(enable);		\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	n; })

#endif
