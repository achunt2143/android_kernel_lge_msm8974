<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef SOUND_FIREWIRE_LIB_H_INCLUDED
#define SOUND_FIREWIRE_LIB_H_INCLUDED

#include <linux/firewire-constants.h>
#include <linux/types.h>
<<<<<<< HEAD

struct fw_unit;

int snd_fw_transaction(struct fw_unit *unit, int tcode,
		       u64 offset, void *buffer, size_t length);
const char *rcode_string(unsigned int rcode);
=======
#include <linux/sched.h>
#include <sound/rawmidi.h>

struct fw_unit;

#define FW_GENERATION_MASK	0x00ff
#define FW_FIXED_GENERATION	0x0100
#define FW_QUIET		0x0200

int snd_fw_transaction(struct fw_unit *unit, int tcode,
		       u64 offset, void *buffer, size_t length,
		       unsigned int flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* returns true if retrying the transaction would not make sense */
static inline bool rcode_is_permanent_error(int rcode)
{
	return rcode == RCODE_TYPE_ERROR || rcode == RCODE_ADDRESS_ERROR;
}

#endif
