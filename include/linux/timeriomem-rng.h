<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/include/linux/timeriomem-rng.h
 *
 * Copyright (c) 2009 Alexander Clouter <alex@digriz.org.uk>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/completion.h>

struct timeriomem_rng_data {
	struct completion	completion;
	unsigned int		present:1;

=======
 */

#ifndef _LINUX_TIMERIOMEM_RNG_H
#define _LINUX_TIMERIOMEM_RNG_H

struct timeriomem_rng_data {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	void __iomem		*address;

	/* measures in usecs */
	unsigned int		period;
<<<<<<< HEAD
};
=======

	/* bits of entropy per 1024 bits read */
	unsigned int		quality;
};

#endif /* _LINUX_TIMERIOMEM_RNG_H */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
