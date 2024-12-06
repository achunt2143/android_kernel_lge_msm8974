/*******************************************************************
 * This file is part of the Emulex Linux Device Driver for         *
 * Fibre Channel Host Bus Adapters.                                *
<<<<<<< HEAD
 * Copyright (C) 2004-2012 Emulex.  All rights reserved.           *
 * EMULEX and SLI are trademarks of Emulex.                        *
 * www.emulex.com                                                  *
=======
 * Copyright (C) 2017-2024 Broadcom. All Rights Reserved. The term *
 * “Broadcom” refers to Broadcom Inc. and/or its subsidiaries.     *
 * Copyright (C) 2004-2016 Emulex.  All rights reserved.           *
 * EMULEX and SLI are trademarks of Emulex.                        *
 * www.broadcom.com                                                *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *                                                                 *
 * This program is free software; you can redistribute it and/or   *
 * modify it under the terms of version 2 of the GNU General       *
 * Public License as published by the Free Software Foundation.    *
 * This program is distributed in the hope that it will be useful. *
 * ALL EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND          *
 * WARRANTIES, INCLUDING ANY IMPLIED WARRANTY OF MERCHANTABILITY,  *
 * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT, ARE      *
 * DISCLAIMED, EXCEPT TO THE EXTENT THAT SUCH DISCLAIMERS ARE HELD *
 * TO BE LEGALLY INVALID.  See the GNU General Public License for  *
 * more details, a copy of which can be found in the file COPYING  *
 * included with this package.                                     *
 *******************************************************************/

<<<<<<< HEAD
#define LPFC_DRIVER_VERSION "8.3.30"
#define LPFC_DRIVER_NAME		"lpfc"
#define LPFC_SP_DRIVER_HANDLER_NAME	"lpfc:sp"
#define LPFC_FP_DRIVER_HANDLER_NAME	"lpfc:fp"

#define LPFC_MODULE_DESC "Emulex LightPulse Fibre Channel SCSI driver " \
		LPFC_DRIVER_VERSION
#define LPFC_COPYRIGHT "Copyright(c) 2004-2009 Emulex.  All rights reserved."
=======
#define LPFC_DRIVER_VERSION "14.4.0.1"
#define LPFC_DRIVER_NAME		"lpfc"

/* Used for SLI 2/3 */
#define LPFC_SP_DRIVER_HANDLER_NAME	"lpfc:sp"
#define LPFC_FP_DRIVER_HANDLER_NAME	"lpfc:fp"

/* Used for SLI4 */
#define LPFC_DRIVER_HANDLER_NAME	"lpfc:"

#define LPFC_MODULE_DESC "Emulex LightPulse Fibre Channel SCSI driver " \
		LPFC_DRIVER_VERSION
#define LPFC_COPYRIGHT "Copyright (C) 2017-2024 Broadcom. All Rights " \
		"Reserved. The term \"Broadcom\" refers to Broadcom Inc. " \
		"and/or its subsidiaries."
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
