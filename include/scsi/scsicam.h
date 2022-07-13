<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * scsicam.h - SCSI CAM support functions, use for HDIO_GETGEO, etc.
 *
 * Copyright 1993, 1994 Drew Eckhardt
 *      Visionary Computing 
 *      (Unix and Linux consulting and custom programming)
 *      drew@Colorado.EDU
 *	+1 (303) 786-7975
 *
 * For more information, please consult the SCSI-CAM draft.
 */

#ifndef SCSICAM_H
#define SCSICAM_H
<<<<<<< HEAD
extern int scsicam_bios_param (struct block_device *bdev, sector_t capacity, int *ip);
extern int scsi_partsize(unsigned char *buf, unsigned long capacity,
           unsigned int  *cyls, unsigned int *hds, unsigned int *secs);
extern unsigned char *scsi_bios_ptable(struct block_device *bdev);
=======
int scsicam_bios_param(struct block_device *bdev, sector_t capacity, int *ip);
bool scsi_partsize(struct block_device *bdev, sector_t capacity, int geom[3]);
unsigned char *scsi_bios_ptable(struct block_device *bdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* def SCSICAM_H */
