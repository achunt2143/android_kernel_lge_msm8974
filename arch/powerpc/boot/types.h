<<<<<<< HEAD
#ifndef _TYPES_H_
#define _TYPES_H_

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdbool.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long long	u64;
typedef signed char		s8;
typedef short			s16;
typedef int			s32;
typedef long long		s64;

<<<<<<< HEAD
=======
/* required for opal-api.h */
typedef u8  uint8_t;
typedef u16 uint16_t;
typedef u32 uint32_t;
typedef u64 uint64_t;
typedef s8  int8_t;
typedef s16 int16_t;
typedef s32 int32_t;
typedef s64 int64_t;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define min(x,y) ({ \
	typeof(x) _x = (x);	\
	typeof(y) _y = (y);	\
	(void) (&_x == &_y);	\
	_x < _y ? _x : _y; })

#define max(x,y) ({ \
	typeof(x) _x = (x);	\
	typeof(y) _y = (y);	\
	(void) (&_x == &_y);	\
	_x > _y ? _x : _y; })

<<<<<<< HEAD
=======
#define min_t(type, a, b) min(((type) a), ((type) b))
#define max_t(type, a, b) max(((type) a), ((type) b))

typedef int bool;

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _TYPES_H_ */
