<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Support for SATA devices on Serial Attached SCSI (SAS) controllers
 *
 * Copyright (C) 2006 IBM Corporation
 *
 * Written by: Darrick J. Wong <djwong@us.ibm.com>, IBM Corporation
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _SAS_ATA_H_
#define _SAS_ATA_H_

#include <linux/libata.h>
#include <scsi/libsas.h>

#ifdef CONFIG_SCSI_SAS_ATA

static inline int dev_is_sata(struct domain_device *dev)
{
<<<<<<< HEAD
	return dev->dev_type == SATA_DEV || dev->dev_type == SATA_PM ||
	       dev->dev_type == SATA_PM_PORT || dev->dev_type == SATA_PENDING;
=======
	return dev->dev_type == SAS_SATA_DEV || dev->dev_type == SAS_SATA_PM ||
	       dev->dev_type == SAS_SATA_PM_PORT || dev->dev_type == SAS_SATA_PENDING;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int sas_get_ata_info(struct domain_device *dev, struct ex_phy *phy);
int sas_ata_init(struct domain_device *dev);
void sas_ata_task_abort(struct sas_task *task);
void sas_ata_strategy_handler(struct Scsi_Host *shost);
<<<<<<< HEAD
void sas_ata_eh(struct Scsi_Host *shost, struct list_head *work_q,
		struct list_head *done_q);
void sas_ata_schedule_reset(struct domain_device *dev);
void sas_ata_wait_eh(struct domain_device *dev);
void sas_probe_sata(struct asd_sas_port *port);
#else

=======
void sas_ata_eh(struct Scsi_Host *shost, struct list_head *work_q);
void sas_ata_schedule_reset(struct domain_device *dev);
void sas_ata_wait_eh(struct domain_device *dev);
void sas_probe_sata(struct asd_sas_port *port);
void sas_suspend_sata(struct asd_sas_port *port);
void sas_resume_sata(struct asd_sas_port *port);
void sas_ata_end_eh(struct ata_port *ap);
void sas_ata_device_link_abort(struct domain_device *dev, bool force_reset);
int sas_execute_ata_cmd(struct domain_device *device, u8 *fis,
			int force_phy_id);
int smp_ata_check_ready_type(struct ata_link *link);
int sas_discover_sata(struct domain_device *dev);
int sas_ata_add_dev(struct domain_device *parent, struct ex_phy *phy,
		    struct domain_device *child, int phy_id);
#else

static inline void sas_ata_disabled_notice(void)
{
	pr_notice_once("ATA device seen but CONFIG_SCSI_SAS_ATA=N\n");
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline int dev_is_sata(struct domain_device *dev)
{
	return 0;
}
static inline int sas_ata_init(struct domain_device *dev)
{
	return 0;
}
static inline void sas_ata_task_abort(struct sas_task *task)
{
}

static inline void sas_ata_strategy_handler(struct Scsi_Host *shost)
{
}

<<<<<<< HEAD
static inline void sas_ata_eh(struct Scsi_Host *shost, struct list_head *work_q,
			      struct list_head *done_q)
=======
static inline void sas_ata_eh(struct Scsi_Host *shost, struct list_head *work_q)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
}

static inline void sas_ata_schedule_reset(struct domain_device *dev)
{
}

static inline void sas_ata_wait_eh(struct domain_device *dev)
{
}

static inline void sas_probe_sata(struct asd_sas_port *port)
{
}

<<<<<<< HEAD
=======
static inline void sas_suspend_sata(struct asd_sas_port *port)
{
}

static inline void sas_resume_sata(struct asd_sas_port *port)
{
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int sas_get_ata_info(struct domain_device *dev, struct ex_phy *phy)
{
	return 0;
}
<<<<<<< HEAD
=======

static inline void sas_ata_end_eh(struct ata_port *ap)
{
}

static inline void sas_ata_device_link_abort(struct domain_device *dev,
					     bool force_reset)
{
}

static inline int sas_execute_ata_cmd(struct domain_device *device, u8 *fis,
				      int force_phy_id)
{
	return 0;
}

static inline int smp_ata_check_ready_type(struct ata_link *link)
{
	return 0;
}

static inline int sas_discover_sata(struct domain_device *dev)
{
	sas_ata_disabled_notice();
	return -ENXIO;
}

static inline int sas_ata_add_dev(struct domain_device *parent, struct ex_phy *phy,
				  struct domain_device *child, int phy_id)
{
	sas_ata_disabled_notice();
	return -ENODEV;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#endif /* _SAS_ATA_H_ */
