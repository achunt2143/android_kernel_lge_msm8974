<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Apple Onboard Audio Alsa private helpers
 *
 * Copyright 2006 Johannes Berg <johannes@sipsolutions.net>
<<<<<<< HEAD
 *
 * GPL v2, can be found in COPYING.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __SND_AOA_ALSA_H
#define __SND_AOA_ALSA_H
#include "../aoa.h"

extern int aoa_alsa_init(char *name, struct module *mod, struct device *dev);
extern void aoa_alsa_cleanup(void);

#endif /* __SND_AOA_ALSA_H */
