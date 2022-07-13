#ifndef __LINUX_STRINGIFY_H
#define __LINUX_STRINGIFY_H

/* Indirect stringification.  Doing two levels allows the parameter to be a
 * macro itself.  For example, compile with -DFOO=bar, __stringify(FOO)
 * converts to "bar".
 */

#define __stringify_1(x...)	#x
#define __stringify(x...)	__stringify_1(x)

<<<<<<< HEAD
=======
#define FILE_LINE	__FILE__ ":" __stringify(__LINE__)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif	/* !__LINUX_STRINGIFY_H */
