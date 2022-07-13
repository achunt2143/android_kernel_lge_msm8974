<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Jack-detection handling for HD-audio
 *
 * Copyright (c) 2011 Takashi Iwai <tiwai@suse.de>
<<<<<<< HEAD
 *
 * This driver is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __SOUND_HDA_JACK_H
#define __SOUND_HDA_JACK_H

<<<<<<< HEAD
struct hda_jack_tbl {
	hda_nid_t nid;
	unsigned char action;		/* event action (0 = none) */
	unsigned char tag;		/* unsol event tag */
	unsigned int private_data;	/* arbitrary data */
=======
#include <linux/err.h>
#include <sound/jack.h>

struct auto_pin_cfg;
struct hda_jack_tbl;
struct hda_jack_callback;

typedef void (*hda_jack_callback_fn) (struct hda_codec *, struct hda_jack_callback *);

struct hda_jack_callback {
	hda_nid_t nid;
	int dev_id;
	hda_jack_callback_fn func;
	unsigned int private_data;	/* arbitrary data */
	unsigned int unsol_res;		/* unsolicited event bits */
	struct hda_jack_tbl *jack;	/* associated jack entry */
	struct hda_jack_callback *next;
};

struct hda_jack_tbl {
	hda_nid_t nid;
	int dev_id;
	unsigned char tag;		/* unsol event tag */
	struct hda_jack_callback *callback;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* jack-detection stuff */
	unsigned int pin_sense;		/* cached pin-sense value */
	unsigned int jack_detect:1;	/* capable of jack-detection? */
	unsigned int jack_dirty:1;	/* needs to update? */
<<<<<<< HEAD
	struct snd_kcontrol *kctl;	/* assigned kctl for jack-detection */
#ifdef CONFIG_SND_HDA_INPUT_JACK
	int type;
	struct snd_jack *jack;
#endif
};

struct hda_jack_tbl *
snd_hda_jack_tbl_get(struct hda_codec *codec, hda_nid_t nid);
struct hda_jack_tbl *
snd_hda_jack_tbl_get_from_tag(struct hda_codec *codec, unsigned char tag);

struct hda_jack_tbl *
snd_hda_jack_tbl_new(struct hda_codec *codec, hda_nid_t nid);
void snd_hda_jack_tbl_clear(struct hda_codec *codec);

/**
 * snd_hda_jack_get_action - get jack-tbl entry for the tag
 *
 * Call this from the unsol event handler to get the assigned action for the
 * event.  This will mark the dirty flag for the later reporting, too.
 */
static inline unsigned char
snd_hda_jack_get_action(struct hda_codec *codec, unsigned int tag)
{
	struct hda_jack_tbl *jack = snd_hda_jack_tbl_get_from_tag(codec, tag);
	if (jack) {
		jack->jack_dirty = 1;
		return jack->action;
	}
	return 0;
}

void snd_hda_jack_set_dirty_all(struct hda_codec *codec);

int snd_hda_jack_detect_enable(struct hda_codec *codec, hda_nid_t nid,
			       unsigned char action);

u32 snd_hda_pin_sense(struct hda_codec *codec, hda_nid_t nid);
int snd_hda_jack_detect(struct hda_codec *codec, hda_nid_t nid);

bool is_jack_detectable(struct hda_codec *codec, hda_nid_t nid);

int snd_hda_jack_add_kctl(struct hda_codec *codec, hda_nid_t nid,
			  const char *name, int idx);
=======
	unsigned int phantom_jack:1;    /* a fixed, always present port? */
	unsigned int block_report:1;    /* in a transitional state - do not report to userspace */
	hda_nid_t gating_jack;		/* valid when gating jack plugged */
	hda_nid_t gated_jack;		/* gated is dependent on this jack */
	hda_nid_t key_report_jack;	/* key reports to this jack */
	int type;
	int button_state;
	struct snd_jack *jack;
};

struct hda_jack_keymap {
	enum snd_jack_types type;
	int key;
};

struct hda_jack_tbl *
snd_hda_jack_tbl_get_mst(struct hda_codec *codec, hda_nid_t nid, int dev_id);

/**
 * snd_hda_jack_tbl_get - query the jack-table entry for the given NID
 * @codec: the HDA codec
 * @nid: pin NID to refer to
 */
static inline struct hda_jack_tbl *
snd_hda_jack_tbl_get(struct hda_codec *codec, hda_nid_t nid)
{
	return snd_hda_jack_tbl_get_mst(codec, nid, 0);
}

struct hda_jack_tbl *
snd_hda_jack_tbl_get_from_tag(struct hda_codec *codec,
			      unsigned char tag, int dev_id);

void snd_hda_jack_tbl_disconnect(struct hda_codec *codec);
void snd_hda_jack_tbl_clear(struct hda_codec *codec);

void snd_hda_jack_set_dirty_all(struct hda_codec *codec);

int snd_hda_jack_detect_enable(struct hda_codec *codec, hda_nid_t nid,
			       int dev_id);

struct hda_jack_callback *
snd_hda_jack_detect_enable_callback_mst(struct hda_codec *codec, hda_nid_t nid,
					int dev_id, hda_jack_callback_fn func);

/**
 * snd_hda_jack_detect_enable - enable the jack-detection
 * @codec: the HDA codec
 * @nid: pin NID to enable
 * @func: callback function to register
 *
 * In the case of error, the return value will be a pointer embedded with
 * errno.  Check and handle the return value appropriately with standard
 * macros such as @IS_ERR() and @PTR_ERR().
 */
static inline struct hda_jack_callback *
snd_hda_jack_detect_enable_callback(struct hda_codec *codec, hda_nid_t nid,
				    hda_jack_callback_fn cb)
{
	return snd_hda_jack_detect_enable_callback_mst(codec, nid, 0, cb);
}

int snd_hda_jack_set_gating_jack(struct hda_codec *codec, hda_nid_t gated_nid,
				 hda_nid_t gating_nid);

int snd_hda_jack_bind_keymap(struct hda_codec *codec, hda_nid_t key_nid,
			     const struct hda_jack_keymap *keymap,
			     hda_nid_t jack_nid);

void snd_hda_jack_set_button_state(struct hda_codec *codec, hda_nid_t jack_nid,
				   int button_state);

u32 snd_hda_jack_pin_sense(struct hda_codec *codec, hda_nid_t nid, int dev_id);

/* the jack state returned from snd_hda_jack_detect_state() */
enum {
	HDA_JACK_NOT_PRESENT, HDA_JACK_PRESENT, HDA_JACK_PHANTOM,
};

int snd_hda_jack_detect_state_mst(struct hda_codec *codec, hda_nid_t nid,
				  int dev_id);

/**
 * snd_hda_jack_detect_state - query pin Presence Detect status
 * @codec: the CODEC to sense
 * @nid: the pin NID to sense
 *
 * Query and return the pin's Presence Detect status, as either
 * HDA_JACK_NOT_PRESENT, HDA_JACK_PRESENT or HDA_JACK_PHANTOM.
 */
static inline int
snd_hda_jack_detect_state(struct hda_codec *codec, hda_nid_t nid)
{
	return snd_hda_jack_detect_state_mst(codec, nid, 0);
}

/**
 * snd_hda_jack_detect_mst - Detect the jack
 * @codec: the HDA codec
 * @nid: pin NID to check jack detection
 * @dev_id: pin device entry id
 */
static inline bool
snd_hda_jack_detect_mst(struct hda_codec *codec, hda_nid_t nid, int dev_id)
{
	return snd_hda_jack_detect_state_mst(codec, nid, dev_id) !=
			HDA_JACK_NOT_PRESENT;
}

/**
 * snd_hda_jack_detect - Detect the jack
 * @codec: the HDA codec
 * @nid: pin NID to check jack detection
 */
static inline bool
snd_hda_jack_detect(struct hda_codec *codec, hda_nid_t nid)
{
	return snd_hda_jack_detect_mst(codec, nid, 0);
}

bool is_jack_detectable(struct hda_codec *codec, hda_nid_t nid);

int snd_hda_jack_add_kctl_mst(struct hda_codec *codec, hda_nid_t nid,
			      int dev_id, const char *name, bool phantom_jack,
			      int type, const struct hda_jack_keymap *keymap);

/**
 * snd_hda_jack_add_kctl - Add a kctl for the given pin
 * @codec: the HDA codec
 * @nid: pin NID to assign
 * @name: string name for the jack
 * @phantom_jack: flag to deal as a phantom jack
 * @type: jack type bits to be reported, 0 for guessing from pincfg
 * @keymap: optional jack / key mapping
 *
 * This assigns a jack-detection kctl to the given pin.  The kcontrol
 * will have the given name and index.
 */
static inline int
snd_hda_jack_add_kctl(struct hda_codec *codec, hda_nid_t nid,
		      const char *name, bool phantom_jack,
		      int type, const struct hda_jack_keymap *keymap)
{
	return snd_hda_jack_add_kctl_mst(codec, nid, 0,
					 name, phantom_jack, type, keymap);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int snd_hda_jack_add_kctls(struct hda_codec *codec,
			   const struct auto_pin_cfg *cfg);

void snd_hda_jack_report_sync(struct hda_codec *codec);

<<<<<<< HEAD
=======
void snd_hda_jack_unsol_event(struct hda_codec *codec, unsigned int res);

void snd_hda_jack_poll_all(struct hda_codec *codec);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __SOUND_HDA_JACK_H */
