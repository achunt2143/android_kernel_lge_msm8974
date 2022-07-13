<<<<<<< HEAD
/*
 *  drivers/s390/cio/qdio_debug.h
 *
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *  Copyright IBM Corp. 2008
 *
 *  Author: Jan Glauber (jang@linux.vnet.ibm.com)
 */
#ifndef QDIO_DEBUG_H
#define QDIO_DEBUG_H

#include <asm/debug.h>
#include <asm/qdio.h>
#include "qdio.h"

/* that gives us 15 characters in the text event views */
<<<<<<< HEAD
#define QDIO_DBF_LEN	16
=======
#define QDIO_DBF_LEN	32
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern debug_info_t *qdio_dbf_setup;
extern debug_info_t *qdio_dbf_error;

<<<<<<< HEAD
/* sort out low debug levels early to avoid wasted sprints */
static inline int qdio_dbf_passes(debug_info_t *dbf_grp, int level)
{
	return (level <= dbf_grp->level);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define DBF_ERR		3	/* error conditions	*/
#define DBF_WARN	4	/* warning conditions	*/
#define DBF_INFO	6	/* informational	*/

#undef DBF_EVENT
#undef DBF_ERROR
#undef DBF_DEV_EVENT

#define DBF_EVENT(text...) \
	do { \
		char debug_buffer[QDIO_DBF_LEN]; \
		snprintf(debug_buffer, QDIO_DBF_LEN, text); \
		debug_text_event(qdio_dbf_setup, DBF_ERR, debug_buffer); \
	} while (0)

<<<<<<< HEAD
#define DBF_HEX(addr, len) \
	do { \
		debug_event(qdio_dbf_setup, DBF_ERR, (void*)(addr), len); \
	} while (0)
=======
static inline void DBF_HEX(void *addr, int len)
{
	debug_event(qdio_dbf_setup, DBF_ERR, addr, len);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define DBF_ERROR(text...) \
	do { \
		char debug_buffer[QDIO_DBF_LEN]; \
		snprintf(debug_buffer, QDIO_DBF_LEN, text); \
		debug_text_event(qdio_dbf_error, DBF_ERR, debug_buffer); \
	} while (0)

<<<<<<< HEAD
#define DBF_ERROR_HEX(addr, len) \
	do { \
		debug_event(qdio_dbf_error, DBF_ERR, (void*)(addr), len); \
	} while (0)

=======
static inline void DBF_ERROR_HEX(void *addr, int len)
{
	debug_event(qdio_dbf_error, DBF_ERR, addr, len);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define DBF_DEV_EVENT(level, device, text...) \
	do { \
		char debug_buffer[QDIO_DBF_LEN]; \
<<<<<<< HEAD
		if (qdio_dbf_passes(device->debug_area, level)) { \
=======
		if (debug_level_enabled(device->debug_area, level)) { \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			snprintf(debug_buffer, QDIO_DBF_LEN, text); \
			debug_text_event(device->debug_area, level, debug_buffer); \
		} \
	} while (0)

<<<<<<< HEAD
#define DBF_DEV_HEX(level, device, addr, len) \
	do { \
		debug_event(device->debug_area, level, (void*)(addr), len); \
	} while (0)

void qdio_allocate_dbf(struct qdio_initialize *init_data,
		       struct qdio_irq *irq_ptr);
void qdio_setup_debug_entries(struct qdio_irq *irq_ptr,
			      struct ccw_device *cdev);
void qdio_shutdown_debug_entries(struct qdio_irq *irq_ptr,
				 struct ccw_device *cdev);
=======
static inline void DBF_DEV_HEX(struct qdio_irq *dev, void *addr,
			       int len, int level)
{
	debug_event(dev->debug_area, level, addr, len);
}

int qdio_allocate_dbf(struct qdio_irq *irq_ptr);
void qdio_setup_debug_entries(struct qdio_irq *irq_ptr);
void qdio_shutdown_debug_entries(struct qdio_irq *irq_ptr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int qdio_debug_init(void);
void qdio_debug_exit(void);

#endif
