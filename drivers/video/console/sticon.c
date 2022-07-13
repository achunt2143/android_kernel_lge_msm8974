/*
 *  linux/drivers/video/console/sticon.c - console driver using HP's STI firmware
 *
 *	Copyright (C) 2000 Philipp Rumpf <prumpf@tux.org>
<<<<<<< HEAD
 *	Copyright (C) 2002 Helge Deller <deller@gmx.de>
=======
 *	Copyright (C) 2002-2020 Helge Deller <deller@gmx.de>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 *  Based on linux/drivers/video/vgacon.c and linux/drivers/video/fbcon.c,
 *  which were
 *
 *	Created 28 Sep 1997 by Geert Uytterhoeven
 *	Rewritten by Martin Mares <mj@ucw.cz>, July 1998
 *	Copyright (C) 1991, 1992  Linus Torvalds
 *			    1995  Jay Estabrook
 *	Copyright (C) 1995 Geert Uytterhoeven
 *	Copyright (C) 1993 Bjoern Brauel
 *			   Roman Hodek
 *	Copyright (C) 1993 Hamish Macdonald
 *			   Greg Harp
 *	Copyright (C) 1994 David Carter [carter@compsci.bristol.ac.uk]
 *
 *	      with work by William Rucklidge (wjr@cs.cornell.edu)
 *			   Geert Uytterhoeven
 *			   Jes Sorensen (jds@kom.auc.dk)
 *			   Martin Apel
 *	      with work by Guenther Kelleter
 *			   Martin Schaller
 *			   Andreas Schwab
 *			   Emmanuel Marty (core@ggi-project.org)
 *			   Jakub Jelinek (jj@ultra.linux.cz)
 *			   Martin Mares <mj@ucw.cz>
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License.  See the file COPYING in the main directory of this archive for
 *  more details.
 *
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/console.h>
#include <linux/errno.h>
#include <linux/vt_kern.h>
#include <linux/kd.h>
#include <linux/selection.h>
#include <linux/module.h>
<<<<<<< HEAD

#include <asm/io.h>

#include "../sticore.h"
=======
#include <linux/slab.h>
#include <linux/font.h>
#include <linux/crc32.h>
#include <linux/fb.h>

#include <asm/io.h>

#include <video/sticore.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* switching to graphics mode */
#define BLANK 0
static int vga_is_gfx;

<<<<<<< HEAD
/* this is the sti_struct used for this console */
static struct sti_struct *sticon_sti;

/* Software scrollback */
static unsigned long softback_buf, softback_curr;
static unsigned long softback_in;
static unsigned long /* softback_top, */ softback_end;
static int softback_lines;

/* software cursor */
static int cursor_drawn;
#define CURSOR_DRAW_DELAY		(1)
#define DEFAULT_CURSOR_BLINK_RATE	(20)

static int vbl_cursor_cnt;

static inline void cursor_undrawn(void)
{
    vbl_cursor_cnt = 0;
    cursor_drawn = 0;
}

=======
#define STI_DEF_FONT	sticon_sti->font

/* borrowed from fbcon.c */
#define FNTREFCOUNT(fd)	(fd->refcount)
#define FNTCRC(fd)	(fd->crc)
static struct sti_cooked_font *font_data[MAX_NR_CONSOLES];

/* this is the sti_struct used for this console */
static struct sti_struct *sticon_sti;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const char *sticon_startup(void)
{
    return "STI console";
}

<<<<<<< HEAD
static int sticon_set_palette(struct vc_data *c, unsigned char *table)
{
    return -EINVAL;
}

static void sticon_putc(struct vc_data *conp, int c, int ypos, int xpos)
{
    int redraw_cursor = 0;

=======
static void sticon_putcs(struct vc_data *conp, const u16 *s, unsigned int count,
			 unsigned int ypos, unsigned int xpos)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
    if (vga_is_gfx || console_blanked)
	    return;

    if (conp->vc_mode != KD_TEXT)
    	    return;
<<<<<<< HEAD
#if 0
    if ((p->cursor_x == xpos) && (p->cursor_y == ypos)) {
	    cursor_undrawn();
	    redraw_cursor = 1;
    }
#endif

    sti_putc(sticon_sti, c, ypos, xpos);

    if (redraw_cursor)
	    vbl_cursor_cnt = CURSOR_DRAW_DELAY;
}

static void sticon_putcs(struct vc_data *conp, const unsigned short *s,
			 int count, int ypos, int xpos)
{
    int redraw_cursor = 0;

    if (vga_is_gfx || console_blanked)
	    return;

    if (conp->vc_mode != KD_TEXT)
    	    return;

#if 0
    if ((p->cursor_y == ypos) && (xpos <= p->cursor_x) &&
	(p->cursor_x < (xpos + count))) {
	    cursor_undrawn();
	    redraw_cursor = 1;
    }
#endif

    while (count--) {
	sti_putc(sticon_sti, scr_readw(s++), ypos, xpos++);
    }

    if (redraw_cursor)
	    vbl_cursor_cnt = CURSOR_DRAW_DELAY;
}

static void sticon_cursor(struct vc_data *conp, int mode)
{
    unsigned short car1;

    car1 = conp->vc_screenbuf[conp->vc_x + conp->vc_y * conp->vc_cols];
    switch (mode) {
    case CM_ERASE:
	sti_putc(sticon_sti, car1, conp->vc_y, conp->vc_x);
	break;
    case CM_MOVE:
    case CM_DRAW:
	switch (conp->vc_cursor_type & 0x0f) {
	case CUR_UNDERLINE:
	case CUR_LOWER_THIRD:
	case CUR_LOWER_HALF:
	case CUR_TWO_THIRDS:
	case CUR_BLOCK:
	    sti_putc(sticon_sti, (car1 & 255) + (0 << 8) + (7 << 11),
		     conp->vc_y, conp->vc_x);
	    break;
	}
=======

    while (count--) {
	sti_putc(sticon_sti, scr_readw(s++), ypos, xpos++,
		 font_data[conp->vc_num]);
    }
}

static void sticon_cursor(struct vc_data *conp, bool enable)
{
    unsigned short car1;

    /* no cursor update if screen is blanked */
    if (vga_is_gfx || console_blanked)
	return;

    car1 = conp->vc_screenbuf[conp->state.x + conp->state.y * conp->vc_cols];
    if (!enable) {
	sti_putc(sticon_sti, car1, conp->state.y, conp->state.x,
		 font_data[conp->vc_num]);
	return;
    }

    switch (CUR_SIZE(conp->vc_cursor_type)) {
    case CUR_UNDERLINE:
    case CUR_LOWER_THIRD:
    case CUR_LOWER_HALF:
    case CUR_TWO_THIRDS:
    case CUR_BLOCK:
	sti_putc(sticon_sti, (car1 & 255) + (0 << 8) + (7 << 11),
		 conp->state.y, conp->state.x, font_data[conp->vc_num]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	break;
    }
}

<<<<<<< HEAD
static int sticon_scroll(struct vc_data *conp, int t, int b, int dir, int count)
=======
static bool sticon_scroll(struct vc_data *conp, unsigned int t,
		unsigned int b, enum con_scroll dir, unsigned int count)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
    struct sti_struct *sti = sticon_sti;

    if (vga_is_gfx)
<<<<<<< HEAD
        return 0;

    sticon_cursor(conp, CM_ERASE);

    switch (dir) {
    case SM_UP:
	sti_bmove(sti, t + count, 0, t, 0, b - t - count, conp->vc_cols);
	sti_clear(sti, b - count, 0, count, conp->vc_cols, conp->vc_video_erase_char);
	break;

    case SM_DOWN:
	sti_bmove(sti, t, 0, t + count, 0, b - t - count, conp->vc_cols);
	sti_clear(sti, t, 0, count, conp->vc_cols, conp->vc_video_erase_char);
	break;
    }

    return 0;
}

static void sticon_bmove(struct vc_data *conp, int sy, int sx, 
	int dy, int dx, int height, int width)
{
    if (!width || !height)
	    return;
#if 0
    if (((sy <= p->cursor_y) && (p->cursor_y < sy+height) &&
	(sx <= p->cursor_x) && (p->cursor_x < sx+width)) ||
	((dy <= p->cursor_y) && (p->cursor_y < dy+height) &&
	(dx <= p->cursor_x) && (p->cursor_x < dx+width)))
		sticon_cursor(p, CM_ERASE /*|CM_SOFTBACK*/);
#endif

    sti_bmove(sticon_sti, sy, sx, dy, dx, height, width);
}

static void sticon_init(struct vc_data *c, int init)
=======
        return false;

    sticon_cursor(conp, false);

    switch (dir) {
    case SM_UP:
	sti_bmove(sti, t + count, 0, t, 0, b - t - count, conp->vc_cols,
		  font_data[conp->vc_num]);
	sti_clear(sti, b - count, 0, count, conp->vc_cols,
		  conp->vc_video_erase_char, font_data[conp->vc_num]);
	break;

    case SM_DOWN:
	sti_bmove(sti, t, 0, t + count, 0, b - t - count, conp->vc_cols,
		  font_data[conp->vc_num]);
	sti_clear(sti, t, 0, count, conp->vc_cols,
		  conp->vc_video_erase_char, font_data[conp->vc_num]);
	break;
    }

    return false;
}

static void sticon_set_def_font(int unit)
{
	if (font_data[unit] != STI_DEF_FONT) {
		if (--FNTREFCOUNT(font_data[unit]) == 0) {
			kfree(font_data[unit]->raw_ptr);
			kfree(font_data[unit]);
		}
		font_data[unit] = STI_DEF_FONT;
	}
}

static int sticon_set_font(struct vc_data *vc, const struct console_font *op,
			   unsigned int vpitch)
{
	struct sti_struct *sti = sticon_sti;
	int vc_cols, vc_rows, vc_old_cols, vc_old_rows;
	int unit = vc->vc_num;
	int w = op->width;
	int h = op->height;
	int size, i, bpc, pitch;
	struct sti_rom_font *new_font;
	struct sti_cooked_font *cooked_font;
	unsigned char *data = op->data, *p;

	if ((w < 6) || (h < 6) || (w > 32) || (h > 32) || (vpitch != 32)
	    || (op->charcount != 256 && op->charcount != 512))
		return -EINVAL;
	pitch = ALIGN(w, 8) / 8;
	bpc = pitch * h;
	size = bpc * op->charcount;

	new_font = kmalloc(sizeof(*new_font) + size, STI_LOWMEM);
	if (!new_font)
		return -ENOMEM;

	new_font->first_char = 0;
	new_font->last_char = op->charcount - 1;
	new_font->width = w;
	new_font->height = h;
	new_font->font_type = STI_FONT_HPROMAN8;
	new_font->bytes_per_char = bpc;
	new_font->underline_height = 0;
	new_font->underline_pos = 0;

	cooked_font = kzalloc(sizeof(*cooked_font), GFP_KERNEL);
	if (!cooked_font) {
		kfree(new_font);
		return -ENOMEM;
	}
	cooked_font->raw = new_font;
	cooked_font->raw_ptr = new_font;
	cooked_font->width = w;
	cooked_font->height = h;
	FNTREFCOUNT(cooked_font) = 0;	/* usage counter */

	p = (unsigned char *) new_font;
	p += sizeof(*new_font);
	for (i = 0; i < op->charcount; i++) {
		memcpy(p, data, bpc);
		data += pitch*32;
		p += bpc;
	}
	FNTCRC(cooked_font) = crc32(0, new_font, size + sizeof(*new_font));
	sti_font_convert_bytemode(sti, cooked_font);
	new_font = cooked_font->raw_ptr;

	/* check if font is already used by other console */
	for (i = 0; i < MAX_NR_CONSOLES; i++) {
		if (font_data[i] != STI_DEF_FONT
		    && (FNTCRC(font_data[i]) == FNTCRC(cooked_font))) {
			kfree(new_font);
			kfree(cooked_font);
			/* current font is the same as the new one */
			if (i == unit)
				return 0;
			cooked_font = font_data[i];
			new_font = cooked_font->raw_ptr;
			break;
		}
	}

	/* clear screen with old font: we now may have less rows */
	vc_old_rows = vc->vc_rows;
	vc_old_cols = vc->vc_cols;
	sti_clear(sticon_sti, 0, 0, vc_old_rows, vc_old_cols,
		  vc->vc_video_erase_char, font_data[vc->vc_num]);

	/* delete old font in case it is a user font */
	sticon_set_def_font(unit);

	FNTREFCOUNT(cooked_font)++;
	font_data[unit] = cooked_font;

	vc_cols = sti_onscreen_x(sti) / cooked_font->width;
	vc_rows = sti_onscreen_y(sti) / cooked_font->height;
	vc_resize(vc, vc_cols, vc_rows);

	/* need to repaint screen if cols & rows are same as old font */
	if (vc_cols == vc_old_cols && vc_rows == vc_old_rows)
		update_screen(vc);

	return 0;
}

static int sticon_font_default(struct vc_data *vc, struct console_font *op,
			       const char *name)
{
	sticon_set_def_font(vc->vc_num);

	return 0;
}

static int sticon_font_set(struct vc_data *vc, const struct console_font *font,
			   unsigned int vpitch, unsigned int flags)
{
	return sticon_set_font(vc, font, vpitch);
}

static void sticon_init(struct vc_data *c, bool init)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
    struct sti_struct *sti = sticon_sti;
    int vc_cols, vc_rows;

    sti_set(sti, 0, 0, sti_onscreen_y(sti), sti_onscreen_x(sti), 0);
<<<<<<< HEAD
    vc_cols = sti_onscreen_x(sti) / sti->font_width;
    vc_rows = sti_onscreen_y(sti) / sti->font_height;
    c->vc_can_do_color = 1;
    
=======
    vc_cols = sti_onscreen_x(sti) / sti->font->width;
    vc_rows = sti_onscreen_y(sti) / sti->font->height;
    c->vc_can_do_color = 1;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
    if (init) {
	c->vc_cols = vc_cols;
	c->vc_rows = vc_rows;
    } else {
<<<<<<< HEAD
	/* vc_rows = (c->vc_rows > vc_rows) ? vc_rows : c->vc_rows; */
	/* vc_cols = (c->vc_cols > vc_cols) ? vc_cols : c->vc_cols; */
	vc_resize(c, vc_cols, vc_rows);
/*	vc_resize_con(vc_rows, vc_cols, c->vc_num); */
=======
	vc_resize(c, vc_cols, vc_rows);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
    }
}

static void sticon_deinit(struct vc_data *c)
{
<<<<<<< HEAD
}

static void sticon_clear(struct vc_data *conp, int sy, int sx, int height,
			 int width)
{
    if (!height || !width)
	return;

    sti_clear(sticon_sti, sy, sx, height, width, conp->vc_video_erase_char);
}

static int sticon_switch(struct vc_data *conp)
{
    return 1;	/* needs refreshing */
}

static int sticon_set_origin(struct vc_data *conp)
{
    return 0;
}

static int sticon_blank(struct vc_data *c, int blank, int mode_switch)
{
    if (blank == 0) {
	if (mode_switch)
	    vga_is_gfx = 0;
	return 1;
    }
    sticon_set_origin(c);
    sti_clear(sticon_sti, 0,0, c->vc_rows, c->vc_cols, BLANK);
    if (mode_switch)
	vga_is_gfx = 1;
    return 1;
}

static int sticon_scrolldelta(struct vc_data *conp, int lines)
{
    return 0;
}

static u16 *sticon_screen_pos(struct vc_data *conp, int offset)
{
    int line;
    unsigned long p;

    if (conp->vc_num != fg_console || !softback_lines)
    	return (u16 *)(conp->vc_origin + offset);
    line = offset / conp->vc_size_row;
    if (line >= softback_lines)
    	return (u16 *)(conp->vc_origin + offset - softback_lines * conp->vc_size_row);
    p = softback_curr + offset;
    if (p >= softback_end)
    	p += softback_buf - softback_end;
    return (u16 *)p;
}

static unsigned long sticon_getxy(struct vc_data *conp, unsigned long pos,
				  int *px, int *py)
{
    int x, y;
    unsigned long ret;
    if (pos >= conp->vc_origin && pos < conp->vc_scr_end) {
    	unsigned long offset = (pos - conp->vc_origin) / 2;
    	
    	x = offset % conp->vc_cols;
    	y = offset / conp->vc_cols;
    	if (conp->vc_num == fg_console)
    	    y += softback_lines;
    	ret = pos + (conp->vc_cols - x) * 2;
    } else if (conp->vc_num == fg_console && softback_lines) {
    	unsigned long offset = pos - softback_curr;
    	
    	if (pos < softback_curr)
    	    offset += softback_end - softback_buf;
    	offset /= 2;
    	x = offset % conp->vc_cols;
    	y = offset / conp->vc_cols;
	ret = pos + (conp->vc_cols - x) * 2;
	if (ret == softback_end)
	    ret = softback_buf;
	if (ret == softback_in)
	    ret = conp->vc_origin;
    } else {
    	/* Should not happen */
    	x = y = 0;
    	ret = conp->vc_origin;
    }
    if (px) *px = x;
    if (py) *py = y;
    return ret;
}

static u8 sticon_build_attr(struct vc_data *conp, u8 color, u8 intens,
			    u8 blink, u8 underline, u8 reverse, u8 italic)
{
    u8 attr = ((color & 0x70) >> 1) | ((color & 7));

    if (reverse) {
	color = ((color >> 3) & 0x7) | ((color & 0x7) << 3);
    }

    return attr;
=======
    int i;

    /* free memory used by user font */
    for (i = 0; i < MAX_NR_CONSOLES; i++)
	sticon_set_def_font(i);
}

static void sticon_clear(struct vc_data *conp, unsigned int sy, unsigned int sx,
			 unsigned int width)
{
    sti_clear(sticon_sti, sy, sx, 1, width,
	      conp->vc_video_erase_char, font_data[conp->vc_num]);
}

static bool sticon_switch(struct vc_data *conp)
{
    return true;	/* needs refreshing */
}

static bool sticon_blank(struct vc_data *c, enum vesa_blank_mode blank,
			 bool mode_switch)
{
    if (blank == VESA_NO_BLANKING) {
	if (mode_switch)
	    vga_is_gfx = 0;
	return true;
    }
    sti_clear(sticon_sti, 0, 0, c->vc_rows, c->vc_cols, BLANK,
	      font_data[c->vc_num]);
    if (mode_switch)
	vga_is_gfx = 1;

    return true;
}

static u8 sticon_build_attr(struct vc_data *conp, u8 color,
			    enum vc_intensity intens,
			    bool blink, bool underline, bool reverse,
			    bool italic)
{
	u8 fg = color & 7;
	u8 bg = (color & 0x70) >> 4;

	if (reverse)
		return (fg << 3) | bg;
	else
		return (bg << 3) | fg;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void sticon_invert_region(struct vc_data *conp, u16 *p, int count)
{
    int col = 1; /* vga_can_do_color; */

    while (count--) {
	u16 a = scr_readw(p);

	if (col)
		a = ((a) & 0x88ff) | (((a) & 0x7000) >> 4) | (((a) & 0x0700) << 4);
	else
		a = ((a & 0x0700) == 0x0100) ? 0x7000 : 0x7700;

	scr_writew(a, p++);
    }
}

<<<<<<< HEAD
static void sticon_save_screen(struct vc_data *conp)
{
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct consw sti_con = {
	.owner			= THIS_MODULE,
	.con_startup		= sticon_startup,
	.con_init		= sticon_init,
	.con_deinit		= sticon_deinit,
	.con_clear		= sticon_clear,
<<<<<<< HEAD
	.con_putc		= sticon_putc,
	.con_putcs		= sticon_putcs,
	.con_cursor		= sticon_cursor,
	.con_scroll		= sticon_scroll,
	.con_bmove		= sticon_bmove,
	.con_switch		= sticon_switch,
	.con_blank		= sticon_blank,
	.con_set_palette	= sticon_set_palette,
	.con_scrolldelta	= sticon_scrolldelta,
	.con_set_origin		= sticon_set_origin,
	.con_save_screen	= sticon_save_screen, 
	.con_build_attr		= sticon_build_attr,
	.con_invert_region	= sticon_invert_region, 
	.con_screen_pos		= sticon_screen_pos,
	.con_getxy		= sticon_getxy,
=======
	.con_putcs		= sticon_putcs,
	.con_cursor		= sticon_cursor,
	.con_scroll		= sticon_scroll,
	.con_switch		= sticon_switch,
	.con_blank		= sticon_blank,
	.con_font_set		= sticon_font_set,
	.con_font_default	= sticon_font_default,
	.con_build_attr		= sticon_build_attr,
	.con_invert_region	= sticon_invert_region,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};



static int __init sticonsole_init(void)
{
<<<<<<< HEAD
=======
    int err, i;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
    /* already initialized ? */
    if (sticon_sti)
	 return 0;

    sticon_sti = sti_get_rom(0);
    if (!sticon_sti)
	return -ENODEV;

<<<<<<< HEAD
    if (conswitchp == &dummy_con) {
	printk(KERN_INFO "sticon: Initializing STI text console.\n");
	return take_over_console(&sti_con, 0, MAX_NR_CONSOLES - 1, 1);
    }
    return 0;
=======
    for (i = 0; i < MAX_NR_CONSOLES; i++)
	font_data[i] = STI_DEF_FONT;

    pr_info("sticon: Initializing STI text console on %s at [%s]\n",
	sticon_sti->sti_data->inq_outptr.dev_name,
	sticon_sti->pa_path);
    console_lock();
    err = do_take_over_console(&sti_con, 0, MAX_NR_CONSOLES - 1,
		PAGE0->mem_cons.cl_class != CL_DUPLEX);
    console_unlock();

    return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(sticonsole_init);
MODULE_LICENSE("GPL");
