<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _FIPS_H
#define _FIPS_H

#ifdef CONFIG_CRYPTO_FIPS
extern int fips_enabled;
<<<<<<< HEAD
extern int get_fips_error_state(void);
extern int get_cc_mode_state(void);
#else
#define fips_enabled 0
static inline int get_fips_error_state(void)
{
	return 0;
}
static inline int get_cc_mode_state(void)
{
	return 0;
}
=======
extern struct atomic_notifier_head fips_fail_notif_chain;

void fips_fail_notify(void);

#else
#define fips_enabled 0

static inline void fips_fail_notify(void) {}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#endif
