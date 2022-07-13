<<<<<<< HEAD
/*
 *  drivers/s390/char/sclp_cpi_sys.h
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *    SCLP control program identification sysfs interface
 *
 *    Copyright IBM Corp. 2007
 *    Author(s): Michael Ernst <mernst@de.ibm.com>
 */

#ifndef __SCLP_CPI_SYS_H__
#define __SCLP_CPI_SYS_H__

int sclp_cpi_set_data(const char *system, const char *sysplex,
		      const char *type, u64 level);

#endif	 /* __SCLP_CPI_SYS_H__ */
