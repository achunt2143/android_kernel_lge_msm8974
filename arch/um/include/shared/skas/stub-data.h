<<<<<<< HEAD
/*
 * Copyright (C) 2005 Jeff Dike (jdike@karaya.com)
 * Licensed under the GPL
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*

 * Copyright (C) 2015 Thomas Meyer (thomas@m3y3r.de)
 * Copyright (C) 2005 Jeff Dike (jdike@karaya.com)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __STUB_DATA_H
#define __STUB_DATA_H

<<<<<<< HEAD
#include <sys/time.h>

struct stub_data {
	long offset;
	int fd;
	struct itimerval timer;
	long err;
=======
struct stub_data {
	unsigned long offset;
	int fd;
	long parent_err, child_err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#endif
