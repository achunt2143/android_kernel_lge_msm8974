<<<<<<< HEAD
#ifndef __linux_video_edid_h__
#define __linux_video_edid_h__

struct edid_info {
	unsigned char dummy[128];
};

#ifdef __KERNEL__
#ifdef CONFIG_X86
extern struct edid_info edid_info;
#endif
#endif

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __linux_video_edid_h__
#define __linux_video_edid_h__

#include <uapi/video/edid.h>

#ifdef CONFIG_X86
extern struct edid_info edid_info;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __linux_video_edid_h__ */
