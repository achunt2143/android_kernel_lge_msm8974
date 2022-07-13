<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#undef TRACE_SYSTEM
#define TRACE_SYSTEM asoc

#if !defined(_TRACE_ASOC_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_ASOC_H

#include <linux/ktime.h>
#include <linux/tracepoint.h>
<<<<<<< HEAD

struct snd_soc_jack;
struct snd_soc_codec;
struct snd_soc_platform;
struct snd_soc_card;
struct snd_soc_dapm_widget;

/*
 * Log register events
 */
DECLARE_EVENT_CLASS(snd_soc_reg,

	TP_PROTO(struct snd_soc_codec *codec, unsigned int reg,
		 unsigned int val),

	TP_ARGS(codec, reg, val),

	TP_STRUCT__entry(
		__string(	name,		codec->name	)
		__field(	int,		id		)
		__field(	unsigned int,	reg		)
		__field(	unsigned int,	val		)
	),

	TP_fast_assign(
		__assign_str(name, codec->name);
		__entry->id = codec->id;
		__entry->reg = reg;
		__entry->val = val;
	),

	TP_printk("codec=%s.%d reg=%x val=%x", __get_str(name),
		  (int)__entry->id, (unsigned int)__entry->reg,
		  (unsigned int)__entry->val)
);

DEFINE_EVENT(snd_soc_reg, snd_soc_reg_write,

	TP_PROTO(struct snd_soc_codec *codec, unsigned int reg,
		 unsigned int val),

	TP_ARGS(codec, reg, val)

);

DEFINE_EVENT(snd_soc_reg, snd_soc_reg_read,

	TP_PROTO(struct snd_soc_codec *codec, unsigned int reg,
		 unsigned int val),

	TP_ARGS(codec, reg, val)

);

DECLARE_EVENT_CLASS(snd_soc_preg,

	TP_PROTO(struct snd_soc_platform *platform, unsigned int reg,
		 unsigned int val),

	TP_ARGS(platform, reg, val),

	TP_STRUCT__entry(
		__string(	name,		platform->name	)
		__field(	int,		id		)
		__field(	unsigned int,	reg		)
		__field(	unsigned int,	val		)
	),

	TP_fast_assign(
		__assign_str(name, platform->name);
		__entry->id = platform->id;
		__entry->reg = reg;
		__entry->val = val;
	),

	TP_printk("platform=%s.%d reg=%x val=%x", __get_str(name),
		  (int)__entry->id, (unsigned int)__entry->reg,
		  (unsigned int)__entry->val)
);

DEFINE_EVENT(snd_soc_preg, snd_soc_preg_write,

	TP_PROTO(struct snd_soc_platform *platform, unsigned int reg,
		 unsigned int val),

	TP_ARGS(platform, reg, val)

);

DEFINE_EVENT(snd_soc_preg, snd_soc_preg_read,

	TP_PROTO(struct snd_soc_platform *platform, unsigned int reg,
		 unsigned int val),

	TP_ARGS(platform, reg, val)

);

DECLARE_EVENT_CLASS(snd_soc_card,

	TP_PROTO(struct snd_soc_card *card, int val),

	TP_ARGS(card, val),

	TP_STRUCT__entry(
		__string(	name,		card->name	)
		__field(	int,		val		)
	),

	TP_fast_assign(
		__assign_str(name, card->name);
		__entry->val = val;
	),

	TP_printk("card=%s val=%d", __get_str(name), (int)__entry->val)
);

DEFINE_EVENT(snd_soc_card, snd_soc_bias_level_start,

	TP_PROTO(struct snd_soc_card *card, int val),

	TP_ARGS(card, val)

);

DEFINE_EVENT(snd_soc_card, snd_soc_bias_level_done,

	TP_PROTO(struct snd_soc_card *card, int val),

	TP_ARGS(card, val)
=======
#include <sound/jack.h>

#define DAPM_DIRECT "(direct)"
#define DAPM_ARROW(dir) (((dir) == SND_SOC_DAPM_DIR_OUT) ? "->" : "<-")

struct snd_soc_jack;
struct snd_soc_card;
struct snd_soc_dapm_widget;
struct snd_soc_dapm_path;

DECLARE_EVENT_CLASS(snd_soc_dapm,

	TP_PROTO(struct snd_soc_dapm_context *dapm, int val),

	TP_ARGS(dapm, val),

	TP_STRUCT__entry(
		__string(	card_name,	dapm->card->name)
		__string(	comp_name,	dapm->component ? dapm->component->name : "(none)")
		__field(	int,		val)
	),

	TP_fast_assign(
		__assign_str(card_name, dapm->card->name);
		__assign_str(comp_name, dapm->component ? dapm->component->name : "(none)");
		__entry->val = val;
	),

	TP_printk("card=%s component=%s val=%d",
		  __get_str(card_name), __get_str(comp_name), (int)__entry->val)
);

DEFINE_EVENT(snd_soc_dapm, snd_soc_bias_level_start,

	TP_PROTO(struct snd_soc_dapm_context *dapm, int val),

	TP_ARGS(dapm, val)

);

DEFINE_EVENT(snd_soc_dapm, snd_soc_bias_level_done,

	TP_PROTO(struct snd_soc_dapm_context *dapm, int val),

	TP_ARGS(dapm, val)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

);

DECLARE_EVENT_CLASS(snd_soc_dapm_basic,

<<<<<<< HEAD
	TP_PROTO(struct snd_soc_card *card),

	TP_ARGS(card),

	TP_STRUCT__entry(
		__string(	name,	card->name	)
=======
	TP_PROTO(struct snd_soc_card *card, int event),

	TP_ARGS(card, event),

	TP_STRUCT__entry(
		__string(	name,	card->name	)
		__field(	int,	event		)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	),

	TP_fast_assign(
		__assign_str(name, card->name);
<<<<<<< HEAD
	),

	TP_printk("card=%s", __get_str(name))
=======
		__entry->event = event;
	),

	TP_printk("card=%s event=%d", __get_str(name), (int)__entry->event)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
);

DEFINE_EVENT(snd_soc_dapm_basic, snd_soc_dapm_start,

<<<<<<< HEAD
	TP_PROTO(struct snd_soc_card *card),

	TP_ARGS(card)
=======
	TP_PROTO(struct snd_soc_card *card, int event),

	TP_ARGS(card, event)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

);

DEFINE_EVENT(snd_soc_dapm_basic, snd_soc_dapm_done,

<<<<<<< HEAD
	TP_PROTO(struct snd_soc_card *card),

	TP_ARGS(card)
=======
	TP_PROTO(struct snd_soc_card *card, int event),

	TP_ARGS(card, event)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

);

DECLARE_EVENT_CLASS(snd_soc_dapm_widget,

	TP_PROTO(struct snd_soc_dapm_widget *w, int val),

	TP_ARGS(w, val),

	TP_STRUCT__entry(
		__string(	name,	w->name		)
		__field(	int,	val		)
	),

	TP_fast_assign(
		__assign_str(name, w->name);
		__entry->val = val;
	),

	TP_printk("widget=%s val=%d", __get_str(name),
		  (int)__entry->val)
);

DEFINE_EVENT(snd_soc_dapm_widget, snd_soc_dapm_widget_power,

	TP_PROTO(struct snd_soc_dapm_widget *w, int val),

	TP_ARGS(w, val)

);

DEFINE_EVENT(snd_soc_dapm_widget, snd_soc_dapm_widget_event_start,

	TP_PROTO(struct snd_soc_dapm_widget *w, int val),

	TP_ARGS(w, val)

);

DEFINE_EVENT(snd_soc_dapm_widget, snd_soc_dapm_widget_event_done,

	TP_PROTO(struct snd_soc_dapm_widget *w, int val),

	TP_ARGS(w, val)

);

TRACE_EVENT(snd_soc_dapm_walk_done,

	TP_PROTO(struct snd_soc_card *card),

	TP_ARGS(card),

	TP_STRUCT__entry(
		__string(	name,	card->name		)
		__field(	int,	power_checks		)
		__field(	int,	path_checks		)
		__field(	int,	neighbour_checks	)
	),

	TP_fast_assign(
		__assign_str(name, card->name);
		__entry->power_checks = card->dapm_stats.power_checks;
		__entry->path_checks = card->dapm_stats.path_checks;
		__entry->neighbour_checks = card->dapm_stats.neighbour_checks;
	),

	TP_printk("%s: checks %d power, %d path, %d neighbour",
		  __get_str(name), (int)__entry->power_checks,
		  (int)__entry->path_checks, (int)__entry->neighbour_checks)
);

<<<<<<< HEAD
=======
TRACE_EVENT(snd_soc_dapm_path,

	TP_PROTO(struct snd_soc_dapm_widget *widget,
		enum snd_soc_dapm_direction dir,
		struct snd_soc_dapm_path *path),

	TP_ARGS(widget, dir, path),

	TP_STRUCT__entry(
		__string(	wname,	widget->name		)
		__string(	pname,	path->name ? path->name : DAPM_DIRECT)
		__string(	pnname,	path->node[dir]->name	)
		__field(	int,	path_node		)
		__field(	int,	path_connect		)
		__field(	int,	path_dir		)
	),

	TP_fast_assign(
		__assign_str(wname, widget->name);
		__assign_str(pname, path->name ? path->name : DAPM_DIRECT);
		__assign_str(pnname, path->node[dir]->name);
		__entry->path_connect = path->connect;
		__entry->path_node = (long)path->node[dir];
		__entry->path_dir = dir;
	),

	TP_printk("%c%s %s %s %s %s",
		(int) __entry->path_node &&
		(int) __entry->path_connect ? '*' : ' ',
		__get_str(wname), DAPM_ARROW(__entry->path_dir),
		__get_str(pname), DAPM_ARROW(__entry->path_dir),
		__get_str(pnname))
);

TRACE_EVENT(snd_soc_dapm_connected,

	TP_PROTO(int paths, int stream),

	TP_ARGS(paths, stream),

	TP_STRUCT__entry(
		__field(	int,	paths		)
		__field(	int,	stream		)
	),

	TP_fast_assign(
		__entry->paths = paths;
		__entry->stream = stream;
	),

	TP_printk("%s: found %d paths",
		__entry->stream ? "capture" : "playback", __entry->paths)
);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
TRACE_EVENT(snd_soc_jack_irq,

	TP_PROTO(const char *name),

	TP_ARGS(name),

	TP_STRUCT__entry(
		__string(	name,	name		)
	),

	TP_fast_assign(
		__assign_str(name, name);
	),

	TP_printk("%s", __get_str(name))
);

TRACE_EVENT(snd_soc_jack_report,

	TP_PROTO(struct snd_soc_jack *jack, int mask, int val),

	TP_ARGS(jack, mask, val),

	TP_STRUCT__entry(
<<<<<<< HEAD
		__string(	name,		jack->jack->name	)
=======
		__string(	name,		jack->jack->id		)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__field(	int,		mask			)
		__field(	int,		val			)
	),

	TP_fast_assign(
<<<<<<< HEAD
		__assign_str(name, jack->jack->name);
=======
		__assign_str(name, jack->jack->id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__entry->mask = mask;
		__entry->val = val;
	),

	TP_printk("jack=%s %x/%x", __get_str(name), (int)__entry->val,
		  (int)__entry->mask)
);

TRACE_EVENT(snd_soc_jack_notify,

	TP_PROTO(struct snd_soc_jack *jack, int val),

	TP_ARGS(jack, val),

	TP_STRUCT__entry(
<<<<<<< HEAD
		__string(	name,		jack->jack->name	)
=======
		__string(	name,		jack->jack->id		)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__field(	int,		val			)
	),

	TP_fast_assign(
<<<<<<< HEAD
		__assign_str(name, jack->jack->name);
=======
		__assign_str(name, jack->jack->id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__entry->val = val;
	),

	TP_printk("jack=%s %x", __get_str(name), (int)__entry->val)
);

<<<<<<< HEAD
TRACE_EVENT(snd_soc_cache_sync,

	TP_PROTO(struct snd_soc_codec *codec, const char *type,
		 const char *status),

	TP_ARGS(codec, type, status),

	TP_STRUCT__entry(
		__string(	name,		codec->name	)
		__string(	status,		status		)
		__string(	type,		type		)
		__field(	int,		id		)
	),

	TP_fast_assign(
		__assign_str(name, codec->name);
		__assign_str(status, status);
		__assign_str(type, type);
		__entry->id = codec->id;
	),

	TP_printk("codec=%s.%d type=%s status=%s", __get_str(name),
		  (int)__entry->id, __get_str(type), __get_str(status))
);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _TRACE_ASOC_H */

/* This part must be outside protection */
#include <trace/define_trace.h>
