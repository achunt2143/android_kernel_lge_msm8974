<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * selection.h
 *
 * Interface between console.c, tty_io.c, vt.c, vc_screen.c and selection.c
 */

#ifndef _LINUX_SELECTION_H_
#define _LINUX_SELECTION_H_

#include <linux/tiocl.h>
#include <linux/vt_buffer.h>

struct tty_struct;
<<<<<<< HEAD

extern struct vc_data *sel_cons;
struct tty_struct;

extern void clear_selection(void);
extern int set_selection(const struct tiocl_selection __user *sel, struct tty_struct *tty);
extern int paste_selection(struct tty_struct *tty);
extern int sel_loadlut(char __user *p);
extern int mouse_reporting(void);
extern void mouse_report(struct tty_struct * tty, int butt, int mrx, int mry);

extern int console_blanked;

extern unsigned char color_table[];
extern int default_red[];
extern int default_grn[];
extern int default_blu[];

extern unsigned short *screen_pos(struct vc_data *vc, int w_offset, int viewed);
extern u16 screen_glyph(struct vc_data *vc, int offset);
extern void complement_pos(struct vc_data *vc, int offset);
extern void invert_screen(struct vc_data *vc, int offset, int count, int shift);

extern void getconsxy(struct vc_data *vc, unsigned char *p);
extern void putconsxy(struct vc_data *vc, unsigned char *p);

extern u16 vcs_scr_readw(struct vc_data *vc, const u16 *org);
extern void vcs_scr_writew(struct vc_data *vc, u16 val, u16 *org);
extern void vcs_scr_updated(struct vc_data *vc);
=======
struct vc_data;

void clear_selection(void);
int set_selection_user(const struct tiocl_selection __user *sel,
		       struct tty_struct *tty);
int set_selection_kernel(struct tiocl_selection *v, struct tty_struct *tty);
int paste_selection(struct tty_struct *tty);
int sel_loadlut(u32 __user *lut);
int mouse_reporting(void);
void mouse_report(struct tty_struct *tty, int butt, int mrx, int mry);

bool vc_is_sel(const struct vc_data *vc);

extern int console_blanked;

extern const unsigned char color_table[];
extern unsigned char default_red[];
extern unsigned char default_grn[];
extern unsigned char default_blu[];

unsigned short *screen_pos(const struct vc_data *vc, int w_offset, bool viewed);
u16 screen_glyph(const struct vc_data *vc, int offset);
u32 screen_glyph_unicode(const struct vc_data *vc, int offset);
void complement_pos(struct vc_data *vc, int offset);
void invert_screen(struct vc_data *vc, int offset, int count, bool viewed);

void getconsxy(const struct vc_data *vc, unsigned char xy[static 2]);
void putconsxy(struct vc_data *vc, unsigned char xy[static const 2]);

u16 vcs_scr_readw(const struct vc_data *vc, const u16 *org);
void vcs_scr_writew(struct vc_data *vc, u16 val, u16 *org);
void vcs_scr_updated(struct vc_data *vc);

int vc_uniscr_check(struct vc_data *vc);
void vc_uniscr_copy_line(const struct vc_data *vc, void *dest, bool viewed,
			 unsigned int row, unsigned int col, unsigned int nr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
