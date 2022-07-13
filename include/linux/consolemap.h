<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * consolemap.h
 *
 * Interface between console.c, selection.c  and consolemap.c
 */
#ifndef __LINUX_CONSOLEMAP_H__
#define __LINUX_CONSOLEMAP_H__

<<<<<<< HEAD
#define LAT1_MAP 0
#define GRAF_MAP 1
#define IBMPC_MAP 2
#define USER_MAP 3

#include <linux/types.h>

#ifdef CONFIG_CONSOLE_TRANSLATIONS
struct vc_data;

extern u16 inverse_translate(struct vc_data *conp, int glyph, int use_unicode);
extern unsigned short *set_translate(int m, struct vc_data *vc);
extern int conv_uni_to_pc(struct vc_data *conp, long ucs);
extern u32 conv_8bit_to_uni(unsigned char c);
extern int conv_uni_to_8bit(u32 uni);
void console_map_init(void);
#else
#define inverse_translate(conp, glyph, uni) ((uint16_t)glyph)
#define set_translate(m, vc) ((unsigned short *)NULL)
#define conv_uni_to_pc(conp, ucs) ((int) (ucs > 0xff ? -1: ucs))
#define conv_8bit_to_uni(c) ((uint32_t)(c))
#define conv_uni_to_8bit(c) ((int) ((c) & 0xff))
#define console_map_init(c) do { ; } while (0)
=======
enum translation_map {
	LAT1_MAP,
	GRAF_MAP,
	IBMPC_MAP,
	USER_MAP,

	FIRST_MAP = LAT1_MAP,
	LAST_MAP = USER_MAP,
};

#include <linux/types.h>

struct vc_data;

#ifdef CONFIG_CONSOLE_TRANSLATIONS
u16 inverse_translate(const struct vc_data *conp, u16 glyph, bool use_unicode);
unsigned short *set_translate(enum translation_map m, struct vc_data *vc);
int conv_uni_to_pc(struct vc_data *conp, long ucs);
u32 conv_8bit_to_uni(unsigned char c);
int conv_uni_to_8bit(u32 uni);
void console_map_init(void);
#else
static inline u16 inverse_translate(const struct vc_data *conp, u16 glyph,
		bool use_unicode)
{
	return glyph;
}

static inline unsigned short *set_translate(enum translation_map m,
		struct vc_data *vc)
{
	return NULL;
}

static inline int conv_uni_to_pc(struct vc_data *conp, long ucs)
{
	return ucs > 0xff ? -1 : ucs;
}

static inline u32 conv_8bit_to_uni(unsigned char c)
{
	return c;
}

static inline int conv_uni_to_8bit(u32 uni)
{
	return uni & 0xff;
}

static inline void console_map_init(void) { }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* CONFIG_CONSOLE_TRANSLATIONS */

#endif /* __LINUX_CONSOLEMAP_H__ */
