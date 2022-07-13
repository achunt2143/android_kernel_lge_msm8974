<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Logitech PS/2++ mouse driver header
 *
 * Copyright (c) 2003 Vojtech Pavlik <vojtech@suse.cz>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _LOGIPS2PP_H
#define _LOGIPS2PP_H

<<<<<<< HEAD
#ifdef CONFIG_MOUSE_PS2_LOGIPS2PP
int ps2pp_init(struct psmouse *psmouse, bool set_properties);
#else
inline int ps2pp_init(struct psmouse *psmouse, bool set_properties)
{
	return -ENOSYS;
}
#endif /* CONFIG_MOUSE_PS2_LOGIPS2PP */
=======
int ps2pp_detect(struct psmouse *psmouse, bool set_properties);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
