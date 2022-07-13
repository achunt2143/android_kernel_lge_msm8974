<<<<<<< HEAD
/*
 * APEI Hardware Error Souce Table support
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * APEI Hardware Error Source Table support
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * HEST describes error sources in detail; communicates operational
 * parameters (i.e. severity levels, masking bits, and threshold
 * values) to Linux as necessary. It also allows the BIOS to report
 * non-standard error sources to Linux (for example, chipset-specific
 * error registers).
 *
 * For more information about HEST, please refer to ACPI Specification
 * version 4.0, section 17.3.2.
 *
 * Copyright 2009 Intel Corp.
 *   Author: Huang Ying <ying.huang@intel.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/acpi.h>
#include <linux/kdebug.h>
#include <linux/highmem.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <acpi/apei.h>
<<<<<<< HEAD
=======
#include <acpi/ghes.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "apei-internal.h"

#define HEST_PFX "HEST: "

<<<<<<< HEAD
bool hest_disable;
=======
int hest_disable;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
EXPORT_SYMBOL_GPL(hest_disable);

/* HEST table parsing */

static struct acpi_table_hest *__read_mostly hest_tab;

<<<<<<< HEAD
=======
/*
 * Since GHES_ASSIST is not supported, skip initialization of GHES_ASSIST
 * structures for MCA.
 * During HEST parsing, detected MCA error sources are cached from early
 * table entries so that the Flags and Source Id fields from these cached
 * values are then referred to in later table entries to determine if the
 * encountered GHES_ASSIST structure should be initialized.
 */
static struct {
	struct acpi_hest_ia_corrected *cmc;
	struct acpi_hest_ia_machine_check *mc;
	struct acpi_hest_ia_deferred_check *dmc;
} mces;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const int hest_esrc_len_tab[ACPI_HEST_TYPE_RESERVED] = {
	[ACPI_HEST_TYPE_IA32_CHECK] = -1,	/* need further calculation */
	[ACPI_HEST_TYPE_IA32_CORRECTED_CHECK] = -1,
	[ACPI_HEST_TYPE_IA32_NMI] = sizeof(struct acpi_hest_ia_nmi),
	[ACPI_HEST_TYPE_AER_ROOT_PORT] = sizeof(struct acpi_hest_aer_root),
	[ACPI_HEST_TYPE_AER_ENDPOINT] = sizeof(struct acpi_hest_aer),
	[ACPI_HEST_TYPE_AER_BRIDGE] = sizeof(struct acpi_hest_aer_bridge),
	[ACPI_HEST_TYPE_GENERIC_ERROR] = sizeof(struct acpi_hest_generic),
<<<<<<< HEAD
};

=======
	[ACPI_HEST_TYPE_GENERIC_ERROR_V2] = sizeof(struct acpi_hest_generic_v2),
	[ACPI_HEST_TYPE_IA32_DEFERRED_CHECK] = -1,
};

static inline bool is_generic_error(struct acpi_hest_header *hest_hdr)
{
	return hest_hdr->type == ACPI_HEST_TYPE_GENERIC_ERROR ||
	       hest_hdr->type == ACPI_HEST_TYPE_GENERIC_ERROR_V2;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int hest_esrc_len(struct acpi_hest_header *hest_hdr)
{
	u16 hest_type = hest_hdr->type;
	int len;

	if (hest_type >= ACPI_HEST_TYPE_RESERVED)
		return 0;

	len = hest_esrc_len_tab[hest_type];

	if (hest_type == ACPI_HEST_TYPE_IA32_CORRECTED_CHECK) {
		struct acpi_hest_ia_corrected *cmc;
		cmc = (struct acpi_hest_ia_corrected *)hest_hdr;
		len = sizeof(*cmc) + cmc->num_hardware_banks *
			sizeof(struct acpi_hest_ia_error_bank);
<<<<<<< HEAD
=======
		mces.cmc = cmc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else if (hest_type == ACPI_HEST_TYPE_IA32_CHECK) {
		struct acpi_hest_ia_machine_check *mc;
		mc = (struct acpi_hest_ia_machine_check *)hest_hdr;
		len = sizeof(*mc) + mc->num_hardware_banks *
			sizeof(struct acpi_hest_ia_error_bank);
<<<<<<< HEAD
=======
		mces.mc = mc;
	} else if (hest_type == ACPI_HEST_TYPE_IA32_DEFERRED_CHECK) {
		struct acpi_hest_ia_deferred_check *mc;
		mc = (struct acpi_hest_ia_deferred_check *)hest_hdr;
		len = sizeof(*mc) + mc->num_hardware_banks *
			sizeof(struct acpi_hest_ia_error_bank);
		mces.dmc = mc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	BUG_ON(len == -1);

	return len;
};

<<<<<<< HEAD
int apei_hest_parse(apei_hest_func_t func, void *data)
=======
/*
 * GHES and GHESv2 structures share the same format, starting from
 * Source Id and ending in Error Status Block Length (inclusive).
 */
static bool is_ghes_assist_struct(struct acpi_hest_header *hest_hdr)
{
	struct acpi_hest_generic *ghes;
	u16 related_source_id;

	if (hest_hdr->type != ACPI_HEST_TYPE_GENERIC_ERROR &&
	    hest_hdr->type != ACPI_HEST_TYPE_GENERIC_ERROR_V2)
		return false;

	ghes = (struct acpi_hest_generic *)hest_hdr;
	related_source_id = ghes->related_source_id;

	if (mces.cmc && mces.cmc->flags & ACPI_HEST_GHES_ASSIST &&
	    related_source_id == mces.cmc->header.source_id)
		return true;
	if (mces.mc && mces.mc->flags & ACPI_HEST_GHES_ASSIST &&
	    related_source_id == mces.mc->header.source_id)
		return true;
	if (mces.dmc && mces.dmc->flags & ACPI_HEST_GHES_ASSIST &&
	    related_source_id == mces.dmc->header.source_id)
		return true;

	return false;
}

typedef int (*apei_hest_func_t)(struct acpi_hest_header *hest_hdr, void *data);

static int apei_hest_parse(apei_hest_func_t func, void *data)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct acpi_hest_header *hest_hdr;
	int i, rc, len;

<<<<<<< HEAD
	if (hest_disable)
=======
	if (hest_disable || !hest_tab)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;

	hest_hdr = (struct acpi_hest_header *)(hest_tab + 1);
	for (i = 0; i < hest_tab->error_source_count; i++) {
		len = hest_esrc_len(hest_hdr);
		if (!len) {
<<<<<<< HEAD
			pr_warning(FW_WARN HEST_PFX
				   "Unknown or unused hardware error source "
				   "type: %d for hardware error source: %d.\n",
				   hest_hdr->type, hest_hdr->source_id);
=======
			pr_warn(FW_WARN HEST_PFX
				"Unknown or unused hardware error source "
				"type: %d for hardware error source: %d.\n",
				hest_hdr->type, hest_hdr->source_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EINVAL;
		}
		if ((void *)hest_hdr + len >
		    (void *)hest_tab + hest_tab->header.length) {
<<<<<<< HEAD
			pr_warning(FW_BUG HEST_PFX
=======
			pr_warn(FW_BUG HEST_PFX
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		"Table contents overflow for hardware error source: %d.\n",
				hest_hdr->source_id);
			return -EINVAL;
		}

<<<<<<< HEAD
=======
		if (is_ghes_assist_struct(hest_hdr)) {
			hest_hdr = (void *)hest_hdr + len;
			continue;
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rc = func(hest_hdr, data);
		if (rc)
			return rc;

		hest_hdr = (void *)hest_hdr + len;
	}

	return 0;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(apei_hest_parse);
=======

/*
 * Check if firmware advertises firmware first mode. We need FF bit to be set
 * along with a set of MC banks which work in FF mode.
 */
static int __init hest_parse_cmc(struct acpi_hest_header *hest_hdr, void *data)
{
	if (hest_hdr->type != ACPI_HEST_TYPE_IA32_CORRECTED_CHECK)
		return 0;

	if (!acpi_disable_cmcff)
		return !arch_apei_enable_cmcff(hest_hdr, data);

	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct ghes_arr {
	struct platform_device **ghes_devs;
	unsigned int count;
};

static int __init hest_parse_ghes_count(struct acpi_hest_header *hest_hdr, void *data)
{
	int *count = data;

<<<<<<< HEAD
	if (hest_hdr->type == ACPI_HEST_TYPE_GENERIC_ERROR)
=======
	if (is_generic_error(hest_hdr))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		(*count)++;
	return 0;
}

static int __init hest_parse_ghes(struct acpi_hest_header *hest_hdr, void *data)
{
	struct platform_device *ghes_dev;
	struct ghes_arr *ghes_arr = data;
	int rc, i;

<<<<<<< HEAD
	if (hest_hdr->type != ACPI_HEST_TYPE_GENERIC_ERROR)
=======
	if (!is_generic_error(hest_hdr))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	if (!((struct acpi_hest_generic *)hest_hdr)->enabled)
		return 0;
	for (i = 0; i < ghes_arr->count; i++) {
		struct acpi_hest_header *hdr;
		ghes_dev = ghes_arr->ghes_devs[i];
		hdr = *(struct acpi_hest_header **)ghes_dev->dev.platform_data;
		if (hdr->source_id == hest_hdr->source_id) {
<<<<<<< HEAD
			pr_warning(FW_WARN HEST_PFX "Duplicated hardware error source ID: %d.\n",
				   hdr->source_id);
=======
			pr_warn(FW_WARN HEST_PFX "Duplicated hardware error source ID: %d.\n",
				hdr->source_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EIO;
		}
	}
	ghes_dev = platform_device_alloc("GHES", hest_hdr->source_id);
	if (!ghes_dev)
		return -ENOMEM;

	rc = platform_device_add_data(ghes_dev, &hest_hdr, sizeof(void *));
	if (rc)
		goto err;

	rc = platform_device_add(ghes_dev);
	if (rc)
		goto err;
	ghes_arr->ghes_devs[ghes_arr->count++] = ghes_dev;

	return 0;
err:
	platform_device_put(ghes_dev);
	return rc;
}

static int __init hest_ghes_dev_register(unsigned int ghes_count)
{
	int rc, i;
	struct ghes_arr ghes_arr;

	ghes_arr.count = 0;
<<<<<<< HEAD
	ghes_arr.ghes_devs = kmalloc(sizeof(void *) * ghes_count, GFP_KERNEL);
=======
	ghes_arr.ghes_devs = kmalloc_array(ghes_count, sizeof(void *),
					   GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!ghes_arr.ghes_devs)
		return -ENOMEM;

	rc = apei_hest_parse(hest_parse_ghes, &ghes_arr);
	if (rc)
		goto err;
<<<<<<< HEAD
=======

	rc = ghes_estatus_pool_init(ghes_count);
	if (rc)
		goto err;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	kfree(ghes_arr.ghes_devs);
	return rc;
err:
	for (i = 0; i < ghes_arr.count; i++)
		platform_device_unregister(ghes_arr.ghes_devs[i]);
	goto out;
}

static int __init setup_hest_disable(char *str)
{
<<<<<<< HEAD
	hest_disable = 1;
	return 0;
=======
	hest_disable = HEST_DISABLED;
	return 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

__setup("hest_disable", setup_hest_disable);

void __init acpi_hest_init(void)
{
	acpi_status status;
<<<<<<< HEAD
	int rc = -ENODEV;
=======
	int rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int ghes_count = 0;

	if (hest_disable) {
		pr_info(HEST_PFX "Table parsing disabled.\n");
		return;
	}

<<<<<<< HEAD
	if (acpi_disabled)
		goto err;

	status = acpi_get_table(ACPI_SIG_HEST, 0,
				(struct acpi_table_header **)&hest_tab);
	if (status == AE_NOT_FOUND)
		goto err;
	else if (ACPI_FAILURE(status)) {
		const char *msg = acpi_format_exception(status);
		pr_err(HEST_PFX "Failed to get table, %s\n", msg);
		rc = -EINVAL;
		goto err;
	}

=======
	status = acpi_get_table(ACPI_SIG_HEST, 0,
				(struct acpi_table_header **)&hest_tab);
	if (status == AE_NOT_FOUND) {
		hest_disable = HEST_NOT_FOUND;
		return;
	} else if (ACPI_FAILURE(status)) {
		const char *msg = acpi_format_exception(status);
		pr_err(HEST_PFX "Failed to get table, %s\n", msg);
		hest_disable = HEST_DISABLED;
		return;
	}

	rc = apei_hest_parse(hest_parse_cmc, NULL);
	if (rc)
		goto err;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!ghes_disable) {
		rc = apei_hest_parse(hest_parse_ghes_count, &ghes_count);
		if (rc)
			goto err;
<<<<<<< HEAD
		rc = hest_ghes_dev_register(ghes_count);
=======

		if (ghes_count)
			rc = hest_ghes_dev_register(ghes_count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (rc)
			goto err;
	}

	pr_info(HEST_PFX "Table parsing has been initialized.\n");
	return;
err:
<<<<<<< HEAD
	hest_disable = 1;
=======
	hest_disable = HEST_DISABLED;
	acpi_put_table((struct acpi_table_header *)hest_tab);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
