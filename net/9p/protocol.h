<<<<<<< HEAD
/*
 * net/9p/protocol.h
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * 9P Protocol Support Code
 *
 *  Copyright (C) 2008 by Eric Van Hensbergen <ericvh@gmail.com>
 *
 *  Base on code from Anthony Liguori <aliguori@us.ibm.com>
 *  Copyright (C) 2008 by IBM, Corp.
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to:
 *  Free Software Foundation
 *  51 Franklin Street, Fifth Floor
 *  Boston, MA  02111-1301  USA
 *
 */

int p9pdu_vwritef(struct p9_fcall *pdu, int proto_version, const char *fmt,
								va_list ap);
=======
 */

size_t p9_msg_buf_size(struct p9_client *c, enum p9_msg_t type,
			const char *fmt, va_list ap);
int p9pdu_vwritef(struct p9_fcall *pdu, int proto_version, const char *fmt,
		  va_list ap);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int p9pdu_readf(struct p9_fcall *pdu, int proto_version, const char *fmt, ...);
int p9pdu_prepare(struct p9_fcall *pdu, int16_t tag, int8_t type);
int p9pdu_finalize(struct p9_client *clnt, struct p9_fcall *pdu);
void p9pdu_reset(struct p9_fcall *pdu);
size_t pdu_read(struct p9_fcall *pdu, void *data, size_t size);
