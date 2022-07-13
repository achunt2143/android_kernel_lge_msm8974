<<<<<<< HEAD
/*
 * Copyright IBM Corporation, 2010
 * Author Venkateswararao Jujjuri <jvrao@linux.vnet.ibm.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2.1 of the GNU Lesser General Public License
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

void p9_release_pages(struct page **, int);
int p9_payload_gup(char *, int *, struct page **, int);
int p9_nr_pages(char *, int);
=======
/* SPDX-License-Identifier: LGPL-2.1 */
/*
 * Copyright IBM Corporation, 2010
 * Author Venkateswararao Jujjuri <jvrao@linux.vnet.ibm.com>
 */

void p9_release_pages(struct page **pages, int nr_pages);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
