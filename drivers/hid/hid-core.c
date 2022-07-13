<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  HID support for Linux
 *
 *  Copyright (c) 1999 Andreas Gal
 *  Copyright (c) 2000-2005 Vojtech Pavlik <vojtech@suse.cz>
 *  Copyright (c) 2005 Michael Haboustak <mike-@cinci.rr.com> for Concept2, Inc
 *  Copyright (c) 2006-2012 Jiri Kosina
 */

/*
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/mm.h>
#include <linux/spinlock.h>
#include <asm/unaligned.h>
#include <asm/byteorder.h>
#include <linux/input.h>
#include <linux/wait.h>
#include <linux/vmalloc.h>
#include <linux/sched.h>
#include <linux/semaphore.h>

#include <linux/hid.h>
#include <linux/hiddev.h>
#include <linux/hid-debug.h>
#include <linux/hidraw.h>

#include "hid-ids.h"

/*
 * Version Information
 */

#define DRIVER_DESC "HID core driver"
<<<<<<< HEAD
#define DRIVER_LICENSE "GPL"

int hid_debug = 0;
module_param_named(debug, hid_debug, int, 0600);
MODULE_PARM_DESC(debug, "toggle HID debugging messages");
EXPORT_SYMBOL_GPL(hid_debug);

static int hid_ignore_special_drivers = 0;
module_param_named(ignore_special_drivers, hid_ignore_special_drivers, int, 0600);
MODULE_PARM_DESC(debug, "Ignore any special drivers and handle all devices by generic driver");
=======

static int hid_ignore_special_drivers = 0;
module_param_named(ignore_special_drivers, hid_ignore_special_drivers, int, 0600);
MODULE_PARM_DESC(ignore_special_drivers, "Ignore any special drivers and handle all devices by generic driver");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Register a new report for a device.
 */

<<<<<<< HEAD
struct hid_report *hid_register_report(struct hid_device *device, unsigned type, unsigned id)
=======
struct hid_report *hid_register_report(struct hid_device *device,
				       enum hid_report_type type, unsigned int id,
				       unsigned int application)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct hid_report_enum *report_enum = device->report_enum + type;
	struct hid_report *report;

	if (id >= HID_MAX_IDS)
		return NULL;
	if (report_enum->report_id_hash[id])
		return report_enum->report_id_hash[id];

	report = kzalloc(sizeof(struct hid_report), GFP_KERNEL);
	if (!report)
		return NULL;

	if (id != 0)
		report_enum->numbered = 1;

	report->id = id;
	report->type = type;
	report->size = 0;
	report->device = device;
<<<<<<< HEAD
	report_enum->report_id_hash[id] = report;

	list_add_tail(&report->list, &report_enum->report_list);
=======
	report->application = application;
	report_enum->report_id_hash[id] = report;

	list_add_tail(&report->list, &report_enum->report_list);
	INIT_LIST_HEAD(&report->field_entry_list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return report;
}
EXPORT_SYMBOL_GPL(hid_register_report);

/*
 * Register a new field for this report.
 */

<<<<<<< HEAD
static struct hid_field *hid_register_field(struct hid_report *report, unsigned usages, unsigned values)
=======
static struct hid_field *hid_register_field(struct hid_report *report, unsigned usages)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct hid_field *field;

	if (report->maxfield == HID_MAX_FIELDS) {
		hid_err(report->device, "too many fields in report\n");
		return NULL;
	}

	field = kzalloc((sizeof(struct hid_field) +
			 usages * sizeof(struct hid_usage) +
<<<<<<< HEAD
			 values * sizeof(unsigned)), GFP_KERNEL);
=======
			 3 * usages * sizeof(unsigned int)), GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!field)
		return NULL;

	field->index = report->maxfield++;
	report->field[field->index] = field;
	field->usage = (struct hid_usage *)(field + 1);
	field->value = (s32 *)(field->usage + usages);
<<<<<<< HEAD
=======
	field->new_value = (s32 *)(field->value + usages);
	field->usages_priorities = (s32 *)(field->new_value + usages);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	field->report = report;

	return field;
}

/*
 * Open a collection. The type/usage is pushed on the stack.
 */

static int open_collection(struct hid_parser *parser, unsigned type)
{
	struct hid_collection *collection;
	unsigned usage;
<<<<<<< HEAD

	usage = parser->local.usage[0];

	if (parser->collection_stack_ptr == HID_COLLECTION_STACK_SIZE) {
		hid_err(parser->device, "collection stack overflow\n");
		return -1;
	}

	if (parser->device->maxcollection == parser->device->collection_size) {
		collection = kmalloc(sizeof(struct hid_collection) *
				parser->device->collection_size * 2, GFP_KERNEL);
		if (collection == NULL) {
			hid_err(parser->device, "failed to reallocate collection array\n");
			return -1;
=======
	int collection_index;

	usage = parser->local.usage[0];

	if (parser->collection_stack_ptr == parser->collection_stack_size) {
		unsigned int *collection_stack;
		unsigned int new_size = parser->collection_stack_size +
					HID_COLLECTION_STACK_SIZE;

		collection_stack = krealloc(parser->collection_stack,
					    new_size * sizeof(unsigned int),
					    GFP_KERNEL);
		if (!collection_stack)
			return -ENOMEM;

		parser->collection_stack = collection_stack;
		parser->collection_stack_size = new_size;
	}

	if (parser->device->maxcollection == parser->device->collection_size) {
		collection = kmalloc(
				array3_size(sizeof(struct hid_collection),
					    parser->device->collection_size,
					    2),
				GFP_KERNEL);
		if (collection == NULL) {
			hid_err(parser->device, "failed to reallocate collection array\n");
			return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		memcpy(collection, parser->device->collection,
			sizeof(struct hid_collection) *
			parser->device->collection_size);
		memset(collection + parser->device->collection_size, 0,
			sizeof(struct hid_collection) *
			parser->device->collection_size);
		kfree(parser->device->collection);
		parser->device->collection = collection;
		parser->device->collection_size *= 2;
	}

	parser->collection_stack[parser->collection_stack_ptr++] =
		parser->device->maxcollection;

<<<<<<< HEAD
	collection = parser->device->collection +
		parser->device->maxcollection++;
	collection->type = type;
	collection->usage = usage;
	collection->level = parser->collection_stack_ptr - 1;
=======
	collection_index = parser->device->maxcollection++;
	collection = parser->device->collection + collection_index;
	collection->type = type;
	collection->usage = usage;
	collection->level = parser->collection_stack_ptr - 1;
	collection->parent_idx = (collection->level == 0) ? -1 :
		parser->collection_stack[collection->level - 1];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (type == HID_COLLECTION_APPLICATION)
		parser->device->maxapplication++;

	return 0;
}

/*
 * Close a collection.
 */

static int close_collection(struct hid_parser *parser)
{
	if (!parser->collection_stack_ptr) {
		hid_err(parser->device, "collection stack underflow\n");
<<<<<<< HEAD
		return -1;
=======
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	parser->collection_stack_ptr--;
	return 0;
}

/*
 * Climb up the stack, search for the specified collection type
 * and return the usage.
 */

static unsigned hid_lookup_collection(struct hid_parser *parser, unsigned type)
{
	struct hid_collection *collection = parser->device->collection;
	int n;

	for (n = parser->collection_stack_ptr - 1; n >= 0; n--) {
		unsigned index = parser->collection_stack[n];
		if (collection[index].type == type)
			return collection[index].usage;
	}
	return 0; /* we know nothing about this usage type */
}

/*
<<<<<<< HEAD
 * Add a usage to the temporary parser table.
 */

static int hid_add_usage(struct hid_parser *parser, unsigned usage)
=======
 * Concatenate usage which defines 16 bits or less with the
 * currently defined usage page to form a 32 bit usage
 */

static void complete_usage(struct hid_parser *parser, unsigned int index)
{
	parser->local.usage[index] &= 0xFFFF;
	parser->local.usage[index] |=
		(parser->global.usage_page & 0xFFFF) << 16;
}

/*
 * Add a usage to the temporary parser table.
 */

static int hid_add_usage(struct hid_parser *parser, unsigned usage, u8 size)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (parser->local.usage_index >= HID_MAX_USAGES) {
		hid_err(parser->device, "usage index exceeded\n");
		return -1;
	}
	parser->local.usage[parser->local.usage_index] = usage;
<<<<<<< HEAD
=======

	/*
	 * If Usage item only includes usage id, concatenate it with
	 * currently defined usage page
	 */
	if (size <= 2)
		complete_usage(parser, parser->local.usage_index);

	parser->local.usage_size[parser->local.usage_index] = size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	parser->local.collection_index[parser->local.usage_index] =
		parser->collection_stack_ptr ?
		parser->collection_stack[parser->collection_stack_ptr - 1] : 0;
	parser->local.usage_index++;
	return 0;
}

/*
 * Register a new field for this report.
 */

static int hid_add_field(struct hid_parser *parser, unsigned report_type, unsigned flags)
{
	struct hid_report *report;
	struct hid_field *field;
<<<<<<< HEAD
	unsigned usages;
	unsigned offset;
	unsigned i;

	report = hid_register_report(parser->device, report_type, parser->global.report_id);
=======
	unsigned int max_buffer_size = HID_MAX_BUFFER_SIZE;
	unsigned int usages;
	unsigned int offset;
	unsigned int i;
	unsigned int application;

	application = hid_lookup_collection(parser, HID_COLLECTION_APPLICATION);

	report = hid_register_report(parser->device, report_type,
				     parser->global.report_id, application);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!report) {
		hid_err(parser->device, "hid_register_report failed\n");
		return -1;
	}

<<<<<<< HEAD
	if (parser->global.logical_maximum < parser->global.logical_minimum) {
		hid_err(parser->device, "logical range invalid %d %d\n",
				parser->global.logical_minimum, parser->global.logical_maximum);
=======
	/* Handle both signed and unsigned cases properly */
	if ((parser->global.logical_minimum < 0 &&
		parser->global.logical_maximum <
		parser->global.logical_minimum) ||
		(parser->global.logical_minimum >= 0 &&
		(__u32)parser->global.logical_maximum <
		(__u32)parser->global.logical_minimum)) {
		dbg_hid("logical range invalid 0x%x 0x%x\n",
			parser->global.logical_minimum,
			parser->global.logical_maximum);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -1;
	}

	offset = report->size;
	report->size += parser->global.report_size * parser->global.report_count;

<<<<<<< HEAD
=======
	if (parser->device->ll_driver->max_buffer_size)
		max_buffer_size = parser->device->ll_driver->max_buffer_size;

	/* Total size check: Allow for possible report index byte */
	if (report->size > (max_buffer_size - 1) << 3) {
		hid_err(parser->device, "report is too long\n");
		return -1;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!parser->local.usage_index) /* Ignore padding fields */
		return 0;

	usages = max_t(unsigned, parser->local.usage_index,
				 parser->global.report_count);

<<<<<<< HEAD
	field = hid_register_field(report, usages, parser->global.report_count);
=======
	field = hid_register_field(report, usages);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!field)
		return 0;

	field->physical = hid_lookup_collection(parser, HID_COLLECTION_PHYSICAL);
	field->logical = hid_lookup_collection(parser, HID_COLLECTION_LOGICAL);
<<<<<<< HEAD
	field->application = hid_lookup_collection(parser, HID_COLLECTION_APPLICATION);
=======
	field->application = application;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (i = 0; i < usages; i++) {
		unsigned j = i;
		/* Duplicate the last usage we parsed if we have excess values */
		if (i >= parser->local.usage_index)
			j = parser->local.usage_index - 1;
		field->usage[i].hid = parser->local.usage[j];
		field->usage[i].collection_index =
			parser->local.collection_index[j];
<<<<<<< HEAD
=======
		field->usage[i].usage_index = i;
		field->usage[i].resolution_multiplier = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	field->maxusage = usages;
	field->flags = flags;
	field->report_offset = offset;
	field->report_type = report_type;
	field->report_size = parser->global.report_size;
	field->report_count = parser->global.report_count;
	field->logical_minimum = parser->global.logical_minimum;
	field->logical_maximum = parser->global.logical_maximum;
	field->physical_minimum = parser->global.physical_minimum;
	field->physical_maximum = parser->global.physical_maximum;
	field->unit_exponent = parser->global.unit_exponent;
	field->unit = parser->global.unit;

	return 0;
}

/*
 * Read data value from item.
 */

static u32 item_udata(struct hid_item *item)
{
	switch (item->size) {
	case 1: return item->data.u8;
	case 2: return item->data.u16;
	case 4: return item->data.u32;
	}
	return 0;
}

static s32 item_sdata(struct hid_item *item)
{
	switch (item->size) {
	case 1: return item->data.s8;
	case 2: return item->data.s16;
	case 4: return item->data.s32;
	}
	return 0;
}

/*
 * Process a global item.
 */

static int hid_parser_global(struct hid_parser *parser, struct hid_item *item)
{
<<<<<<< HEAD
=======
	__s32 raw_value;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (item->tag) {
	case HID_GLOBAL_ITEM_TAG_PUSH:

		if (parser->global_stack_ptr == HID_GLOBAL_STACK_SIZE) {
			hid_err(parser->device, "global environment stack overflow\n");
			return -1;
		}

		memcpy(parser->global_stack + parser->global_stack_ptr++,
			&parser->global, sizeof(struct hid_global));
		return 0;

	case HID_GLOBAL_ITEM_TAG_POP:

		if (!parser->global_stack_ptr) {
			hid_err(parser->device, "global environment stack underflow\n");
			return -1;
		}

		memcpy(&parser->global, parser->global_stack +
			--parser->global_stack_ptr, sizeof(struct hid_global));
		return 0;

	case HID_GLOBAL_ITEM_TAG_USAGE_PAGE:
		parser->global.usage_page = item_udata(item);
		return 0;

	case HID_GLOBAL_ITEM_TAG_LOGICAL_MINIMUM:
		parser->global.logical_minimum = item_sdata(item);
		return 0;

	case HID_GLOBAL_ITEM_TAG_LOGICAL_MAXIMUM:
		if (parser->global.logical_minimum < 0)
			parser->global.logical_maximum = item_sdata(item);
		else
			parser->global.logical_maximum = item_udata(item);
		return 0;

	case HID_GLOBAL_ITEM_TAG_PHYSICAL_MINIMUM:
		parser->global.physical_minimum = item_sdata(item);
		return 0;

	case HID_GLOBAL_ITEM_TAG_PHYSICAL_MAXIMUM:
		if (parser->global.physical_minimum < 0)
			parser->global.physical_maximum = item_sdata(item);
		else
			parser->global.physical_maximum = item_udata(item);
		return 0;

	case HID_GLOBAL_ITEM_TAG_UNIT_EXPONENT:
<<<<<<< HEAD
		parser->global.unit_exponent = item_sdata(item);
=======
		/* Many devices provide unit exponent as a two's complement
		 * nibble due to the common misunderstanding of HID
		 * specification 1.11, 6.2.2.7 Global Items. Attempt to handle
		 * both this and the standard encoding. */
		raw_value = item_sdata(item);
		if (!(raw_value & 0xfffffff0))
			parser->global.unit_exponent = hid_snto32(raw_value, 4);
		else
			parser->global.unit_exponent = raw_value;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	case HID_GLOBAL_ITEM_TAG_UNIT:
		parser->global.unit = item_udata(item);
		return 0;

	case HID_GLOBAL_ITEM_TAG_REPORT_SIZE:
		parser->global.report_size = item_udata(item);
<<<<<<< HEAD
		if (parser->global.report_size > 96) {
=======
		if (parser->global.report_size > 256) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			hid_err(parser->device, "invalid report_size %d\n",
					parser->global.report_size);
			return -1;
		}
		return 0;

	case HID_GLOBAL_ITEM_TAG_REPORT_COUNT:
		parser->global.report_count = item_udata(item);
		if (parser->global.report_count > HID_MAX_USAGES) {
			hid_err(parser->device, "invalid report_count %d\n",
					parser->global.report_count);
			return -1;
		}
		return 0;

	case HID_GLOBAL_ITEM_TAG_REPORT_ID:
		parser->global.report_id = item_udata(item);
		if (parser->global.report_id == 0 ||
		    parser->global.report_id >= HID_MAX_IDS) {
			hid_err(parser->device, "report_id %u is invalid\n",
				parser->global.report_id);
			return -1;
		}
		return 0;

	default:
		hid_err(parser->device, "unknown global tag 0x%x\n", item->tag);
		return -1;
	}
}

/*
 * Process a local item.
 */

static int hid_parser_local(struct hid_parser *parser, struct hid_item *item)
{
	__u32 data;
	unsigned n;
<<<<<<< HEAD
=======
	__u32 count;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	data = item_udata(item);

	switch (item->tag) {
	case HID_LOCAL_ITEM_TAG_DELIMITER:

		if (data) {
			/*
			 * We treat items before the first delimiter
			 * as global to all usage sets (branch 0).
			 * In the moment we process only these global
			 * items and the first delimiter set.
			 */
			if (parser->local.delimiter_depth != 0) {
				hid_err(parser->device, "nested delimiters\n");
				return -1;
			}
			parser->local.delimiter_depth++;
			parser->local.delimiter_branch++;
		} else {
			if (parser->local.delimiter_depth < 1) {
				hid_err(parser->device, "bogus close delimiter\n");
				return -1;
			}
			parser->local.delimiter_depth--;
		}
<<<<<<< HEAD
		return 1;
=======
		return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	case HID_LOCAL_ITEM_TAG_USAGE:

		if (parser->local.delimiter_branch > 1) {
			dbg_hid("alternative usage ignored\n");
			return 0;
		}

<<<<<<< HEAD
		if (item->size <= 2)
			data = (parser->global.usage_page << 16) + data;

		return hid_add_usage(parser, data);
=======
		return hid_add_usage(parser, data, item->size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	case HID_LOCAL_ITEM_TAG_USAGE_MINIMUM:

		if (parser->local.delimiter_branch > 1) {
			dbg_hid("alternative usage ignored\n");
			return 0;
		}

<<<<<<< HEAD
		if (item->size <= 2)
			data = (parser->global.usage_page << 16) + data;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		parser->local.usage_minimum = data;
		return 0;

	case HID_LOCAL_ITEM_TAG_USAGE_MAXIMUM:

		if (parser->local.delimiter_branch > 1) {
			dbg_hid("alternative usage ignored\n");
			return 0;
		}

<<<<<<< HEAD
		if (item->size <= 2)
			data = (parser->global.usage_page << 16) + data;

		for (n = parser->local.usage_minimum; n <= data; n++)
			if (hid_add_usage(parser, n)) {
=======
		count = data - parser->local.usage_minimum;
		if (count + parser->local.usage_index >= HID_MAX_USAGES) {
			/*
			 * We do not warn if the name is not set, we are
			 * actually pre-scanning the device.
			 */
			if (dev_name(&parser->device->dev))
				hid_warn(parser->device,
					 "ignoring exceeding usage max\n");
			data = HID_MAX_USAGES - parser->local.usage_index +
				parser->local.usage_minimum - 1;
			if (data <= 0) {
				hid_err(parser->device,
					"no more usage index available\n");
				return -1;
			}
		}

		for (n = parser->local.usage_minimum; n <= data; n++)
			if (hid_add_usage(parser, n, item->size)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				dbg_hid("hid_add_usage failed\n");
				return -1;
			}
		return 0;

	default:

		dbg_hid("unknown local item tag 0x%x\n", item->tag);
		return 0;
	}
	return 0;
}

/*
<<<<<<< HEAD
=======
 * Concatenate Usage Pages into Usages where relevant:
 * As per specification, 6.2.2.8: "When the parser encounters a main item it
 * concatenates the last declared Usage Page with a Usage to form a complete
 * usage value."
 */

static void hid_concatenate_last_usage_page(struct hid_parser *parser)
{
	int i;
	unsigned int usage_page;
	unsigned int current_page;

	if (!parser->local.usage_index)
		return;

	usage_page = parser->global.usage_page;

	/*
	 * Concatenate usage page again only if last declared Usage Page
	 * has not been already used in previous usages concatenation
	 */
	for (i = parser->local.usage_index - 1; i >= 0; i--) {
		if (parser->local.usage_size[i] > 2)
			/* Ignore extended usages */
			continue;

		current_page = parser->local.usage[i] >> 16;
		if (current_page == usage_page)
			break;

		complete_usage(parser, i);
	}
}

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Process a main item.
 */

static int hid_parser_main(struct hid_parser *parser, struct hid_item *item)
{
	__u32 data;
	int ret;

<<<<<<< HEAD
=======
	hid_concatenate_last_usage_page(parser);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	data = item_udata(item);

	switch (item->tag) {
	case HID_MAIN_ITEM_TAG_BEGIN_COLLECTION:
		ret = open_collection(parser, data & 0xff);
		break;
	case HID_MAIN_ITEM_TAG_END_COLLECTION:
		ret = close_collection(parser);
		break;
	case HID_MAIN_ITEM_TAG_INPUT:
		ret = hid_add_field(parser, HID_INPUT_REPORT, data);
		break;
	case HID_MAIN_ITEM_TAG_OUTPUT:
		ret = hid_add_field(parser, HID_OUTPUT_REPORT, data);
		break;
	case HID_MAIN_ITEM_TAG_FEATURE:
		ret = hid_add_field(parser, HID_FEATURE_REPORT, data);
		break;
	default:
<<<<<<< HEAD
		hid_err(parser->device, "unknown main item tag 0x%x\n", item->tag);
=======
		hid_warn(parser->device, "unknown main item tag 0x%x\n", item->tag);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = 0;
	}

	memset(&parser->local, 0, sizeof(parser->local));	/* Reset the local parser environment */

	return ret;
}

/*
 * Process a reserved item.
 */

static int hid_parser_reserved(struct hid_parser *parser, struct hid_item *item)
{
	dbg_hid("reserved item type, tag 0x%x\n", item->tag);
	return 0;
}

/*
 * Free a report and all registered fields. The field->usage and
 * field->value table's are allocated behind the field, so we need
 * only to free(field) itself.
 */

static void hid_free_report(struct hid_report *report)
{
	unsigned n;

<<<<<<< HEAD
=======
	kfree(report->field_entries);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (n = 0; n < report->maxfield; n++)
		kfree(report->field[n]);
	kfree(report);
}

/*
<<<<<<< HEAD
 * Free a device structure, all reports, and all fields.
 */

static void hid_device_release(struct device *dev)
{
	struct hid_device *device = container_of(dev, struct hid_device, dev);
=======
 * Close report. This function returns the device
 * state to the point prior to hid_open_report().
 */
static void hid_close_report(struct hid_device *device)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned i, j;

	for (i = 0; i < HID_REPORT_TYPES; i++) {
		struct hid_report_enum *report_enum = device->report_enum + i;

		for (j = 0; j < HID_MAX_IDS; j++) {
			struct hid_report *report = report_enum->report_id_hash[j];
			if (report)
				hid_free_report(report);
		}
<<<<<<< HEAD
	}

	kfree(device->rdesc);
	kfree(device->collection);
	kfree(device);
=======
		memset(report_enum, 0, sizeof(*report_enum));
		INIT_LIST_HEAD(&report_enum->report_list);
	}

	kfree(device->rdesc);
	device->rdesc = NULL;
	device->rsize = 0;

	kfree(device->collection);
	device->collection = NULL;
	device->collection_size = 0;
	device->maxcollection = 0;
	device->maxapplication = 0;

	device->status &= ~HID_STAT_PARSED;
}

/*
 * Free a device structure, all reports, and all fields.
 */

void hiddev_free(struct kref *ref)
{
	struct hid_device *hid = container_of(ref, struct hid_device, ref);

	hid_close_report(hid);
	kfree(hid->dev_rdesc);
	kfree(hid);
}

static void hid_device_release(struct device *dev)
{
	struct hid_device *hid = to_hid_device(dev);

	kref_put(&hid->ref, hiddev_free);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Fetch a report description item from the data stream. We support long
 * items, though they are not used yet.
 */

static u8 *fetch_item(__u8 *start, __u8 *end, struct hid_item *item)
{
	u8 b;

	if ((end - start) <= 0)
		return NULL;

	b = *start++;

	item->type = (b >> 2) & 3;
	item->tag  = (b >> 4) & 15;

	if (item->tag == HID_ITEM_TAG_LONG) {

		item->format = HID_ITEM_FORMAT_LONG;

		if ((end - start) < 2)
			return NULL;

		item->size = *start++;
		item->tag  = *start++;

		if ((end - start) < item->size)
			return NULL;

		item->data.longdata = start;
		start += item->size;
		return start;
	}

	item->format = HID_ITEM_FORMAT_SHORT;
	item->size = b & 3;

	switch (item->size) {
	case 0:
		return start;

	case 1:
		if ((end - start) < 1)
			return NULL;
		item->data.u8 = *start++;
		return start;

	case 2:
		if ((end - start) < 2)
			return NULL;
		item->data.u16 = get_unaligned_le16(start);
		start = (__u8 *)((__le16 *)start + 1);
		return start;

	case 3:
		item->size++;
		if ((end - start) < 4)
			return NULL;
		item->data.u32 = get_unaligned_le32(start);
		start = (__u8 *)((__le32 *)start + 1);
		return start;
	}

	return NULL;
}

<<<<<<< HEAD
/**
 * hid_parse_report - parse device report
 *
 * @device: hid device
 * @start: report start
 * @size: report size
 *
 * Parse a report description into a hid_device structure. Reports are
 * enumerated, fields are attached to these reports.
 * 0 returned on success, otherwise nonzero error value.
 */
int hid_parse_report(struct hid_device *device, __u8 *start,
		unsigned size)
{
	struct hid_parser *parser;
	struct hid_item item;
	__u8 *end;
	int ret;
	static int (*dispatch_type[])(struct hid_parser *parser,
				      struct hid_item *item) = {
		hid_parser_main,
=======
static void hid_scan_input_usage(struct hid_parser *parser, u32 usage)
{
	struct hid_device *hid = parser->device;

	if (usage == HID_DG_CONTACTID)
		hid->group = HID_GROUP_MULTITOUCH;
}

static void hid_scan_feature_usage(struct hid_parser *parser, u32 usage)
{
	if (usage == 0xff0000c5 && parser->global.report_count == 256 &&
	    parser->global.report_size == 8)
		parser->scan_flags |= HID_SCAN_FLAG_MT_WIN_8;

	if (usage == 0xff0000c6 && parser->global.report_count == 1 &&
	    parser->global.report_size == 8)
		parser->scan_flags |= HID_SCAN_FLAG_MT_WIN_8;
}

static void hid_scan_collection(struct hid_parser *parser, unsigned type)
{
	struct hid_device *hid = parser->device;
	int i;

	if (((parser->global.usage_page << 16) == HID_UP_SENSOR) &&
	    (type == HID_COLLECTION_PHYSICAL ||
	     type == HID_COLLECTION_APPLICATION))
		hid->group = HID_GROUP_SENSOR_HUB;

	if (hid->vendor == USB_VENDOR_ID_MICROSOFT &&
	    hid->product == USB_DEVICE_ID_MS_POWER_COVER &&
	    hid->group == HID_GROUP_MULTITOUCH)
		hid->group = HID_GROUP_GENERIC;

	if ((parser->global.usage_page << 16) == HID_UP_GENDESK)
		for (i = 0; i < parser->local.usage_index; i++)
			if (parser->local.usage[i] == HID_GD_POINTER)
				parser->scan_flags |= HID_SCAN_FLAG_GD_POINTER;

	if ((parser->global.usage_page << 16) >= HID_UP_MSVENDOR)
		parser->scan_flags |= HID_SCAN_FLAG_VENDOR_SPECIFIC;

	if ((parser->global.usage_page << 16) == HID_UP_GOOGLEVENDOR)
		for (i = 0; i < parser->local.usage_index; i++)
			if (parser->local.usage[i] ==
					(HID_UP_GOOGLEVENDOR | 0x0001))
				parser->device->group =
					HID_GROUP_VIVALDI;
}

static int hid_scan_main(struct hid_parser *parser, struct hid_item *item)
{
	__u32 data;
	int i;

	hid_concatenate_last_usage_page(parser);

	data = item_udata(item);

	switch (item->tag) {
	case HID_MAIN_ITEM_TAG_BEGIN_COLLECTION:
		hid_scan_collection(parser, data & 0xff);
		break;
	case HID_MAIN_ITEM_TAG_END_COLLECTION:
		break;
	case HID_MAIN_ITEM_TAG_INPUT:
		/* ignore constant inputs, they will be ignored by hid-input */
		if (data & HID_MAIN_ITEM_CONSTANT)
			break;
		for (i = 0; i < parser->local.usage_index; i++)
			hid_scan_input_usage(parser, parser->local.usage[i]);
		break;
	case HID_MAIN_ITEM_TAG_OUTPUT:
		break;
	case HID_MAIN_ITEM_TAG_FEATURE:
		for (i = 0; i < parser->local.usage_index; i++)
			hid_scan_feature_usage(parser, parser->local.usage[i]);
		break;
	}

	/* Reset the local parser environment */
	memset(&parser->local, 0, sizeof(parser->local));

	return 0;
}

/*
 * Scan a report descriptor before the device is added to the bus.
 * Sets device groups and other properties that determine what driver
 * to load.
 */
static int hid_scan_report(struct hid_device *hid)
{
	struct hid_parser *parser;
	struct hid_item item;
	__u8 *start = hid->dev_rdesc;
	__u8 *end = start + hid->dev_rsize;
	static int (*dispatch_type[])(struct hid_parser *parser,
				      struct hid_item *item) = {
		hid_scan_main,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		hid_parser_global,
		hid_parser_local,
		hid_parser_reserved
	};

<<<<<<< HEAD
	if (device->driver->report_fixup)
		start = device->driver->report_fixup(device, start, &size);

	device->rdesc = kmemdup(start, size, GFP_KERNEL);
	if (device->rdesc == NULL)
		return -ENOMEM;
	device->rsize = size;

	parser = vzalloc(sizeof(struct hid_parser));
	if (!parser) {
		ret = -ENOMEM;
		goto err;
	}

	parser->device = device;

	end = start + size;
	ret = -EINVAL;
	while ((start = fetch_item(start, end, &item)) != NULL) {

		if (item.format != HID_ITEM_FORMAT_SHORT) {
			hid_err(device, "unexpected long global item\n");
			goto err;
		}

		if (dispatch_type[item.type](parser, &item)) {
			hid_err(device, "item %u %u %u %u parsing failed\n",
				item.format, (unsigned)item.size,
				(unsigned)item.type, (unsigned)item.tag);
			goto err;
		}

		if (start == end) {
			if (parser->collection_stack_ptr) {
				hid_err(device, "unbalanced collection at end of report description\n");
				goto err;
			}
			if (parser->local.delimiter_depth) {
				hid_err(device, "unbalanced delimiter at end of report description\n");
				goto err;
			}
			vfree(parser);
			return 0;
		}
	}

	hid_err(device, "item fetching failed at offset %d\n", (int)(end - start));
err:
	vfree(parser);
	return ret;
}
EXPORT_SYMBOL_GPL(hid_parse_report);

/*
 * Convert a signed n-bit integer to signed 32-bit integer. Common
 * cases are done through the compiler, the screwed things has to be
 * done by hand.
 */

static s32 snto32(__u32 value, unsigned n)
{
	switch (n) {
	case 8:  return ((__s8)value);
	case 16: return ((__s16)value);
	case 32: return ((__s32)value);
	}
	return value & (1 << (n - 1)) ? value | (-1 << n) : value;
}

/*
 * Convert a signed 32-bit integer to a signed n-bit integer.
 */

static u32 s32ton(__s32 value, unsigned n)
{
	s32 a = value >> (n - 1);
	if (a && a != -1)
		return value < 0 ? 1 << (n - 1) : (1 << (n - 1)) - 1;
	return value & ((1 << n) - 1);
}

/*
 * Extract/implement a data field from/to a little endian report (bit array).
 *
 * Code sort-of follows HID spec:
 *     http://www.usb.org/developers/devclass_docs/HID1_11.pdf
 *
 * While the USB HID spec allows unlimited length bit fields in "report
 * descriptors", most devices never use more than 16 bits.
 * One model of UPS is claimed to report "LINEV" as a 32-bit field.
 * Search linux-kernel and linux-usb-devel archives for "hid-core extract".
 */

static __u32 extract(const struct hid_device *hid, __u8 *report,
		     unsigned offset, unsigned n)
{
	u64 x;

	if (n > 32)
		hid_warn(hid, "extract() called with n (%d) > 32! (%s)\n",
			 n, current->comm);

	report += offset >> 3;  /* adjust byte index */
	offset &= 7;            /* now only need bit offset into one byte */
	x = get_unaligned_le64(report);
	x = (x >> offset) & ((1ULL << n) - 1);  /* extract bit field */
	return (u32) x;
}

/*
 * "implement" : set bits in a little endian bit stream.
 * Same concepts as "extract" (see comments above).
 * The data mangled in the bit stream remains in little endian
 * order the whole time. It make more sense to talk about
 * endianness of register values by considering a register
 * a "cached" copy of the little endiad bit stream.
 */
static void implement(const struct hid_device *hid, __u8 *report,
		      unsigned offset, unsigned n, __u32 value)
{
	u64 x;
	u64 m = (1ULL << n) - 1;

	if (n > 32)
		hid_warn(hid, "%s() called with n (%d) > 32! (%s)\n",
			 __func__, n, current->comm);

	if (value > m)
		hid_warn(hid, "%s() called with too large value %d! (%s)\n",
			 __func__, value, current->comm);
	WARN_ON(value > m);
	value &= m;

	report += offset >> 3;
	offset &= 7;

	x = get_unaligned_le64(report);
	x &= ~(m << offset);
	x |= ((u64)value) << offset;
	put_unaligned_le64(x, report);
}

/*
 * Search an array for a value.
 */

static int search(__s32 *array, __s32 value, unsigned n)
{
	while (n--) {
		if (*array++ == value)
			return 0;
	}
	return -1;
}

=======
	parser = vzalloc(sizeof(struct hid_parser));
	if (!parser)
		return -ENOMEM;

	parser->device = hid;
	hid->group = HID_GROUP_GENERIC;

	/*
	 * The parsing is simpler than the one in hid_open_report() as we should
	 * be robust against hid errors. Those errors will be raised by
	 * hid_open_report() anyway.
	 */
	while ((start = fetch_item(start, end, &item)) != NULL)
		dispatch_type[item.type](parser, &item);

	/*
	 * Handle special flags set during scanning.
	 */
	if ((parser->scan_flags & HID_SCAN_FLAG_MT_WIN_8) &&
	    (hid->group == HID_GROUP_MULTITOUCH))
		hid->group = HID_GROUP_MULTITOUCH_WIN_8;

	/*
	 * Vendor specific handlings
	 */
	switch (hid->vendor) {
	case USB_VENDOR_ID_WACOM:
		hid->group = HID_GROUP_WACOM;
		break;
	case USB_VENDOR_ID_SYNAPTICS:
		if (hid->group == HID_GROUP_GENERIC)
			if ((parser->scan_flags & HID_SCAN_FLAG_VENDOR_SPECIFIC)
			    && (parser->scan_flags & HID_SCAN_FLAG_GD_POINTER))
				/*
				 * hid-rmi should take care of them,
				 * not hid-generic
				 */
				hid->group = HID_GROUP_RMI;
		break;
	}

	kfree(parser->collection_stack);
	vfree(parser);
	return 0;
}

/**
 * hid_parse_report - parse device report
 *
 * @hid: hid device
 * @start: report start
 * @size: report size
 *
 * Allocate the device report as read by the bus driver. This function should
 * only be called from parse() in ll drivers.
 */
int hid_parse_report(struct hid_device *hid, __u8 *start, unsigned size)
{
	hid->dev_rdesc = kmemdup(start, size, GFP_KERNEL);
	if (!hid->dev_rdesc)
		return -ENOMEM;
	hid->dev_rsize = size;
	return 0;
}
EXPORT_SYMBOL_GPL(hid_parse_report);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const char * const hid_report_names[] = {
	"HID_INPUT_REPORT",
	"HID_OUTPUT_REPORT",
	"HID_FEATURE_REPORT",
};
/**
 * hid_validate_values - validate existing device report's value indexes
 *
<<<<<<< HEAD
 * @device: hid device
=======
 * @hid: hid device
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @type: which report type to examine
 * @id: which report ID to examine (0 for first)
 * @field_index: which report field to examine
 * @report_counts: expected number of values
 *
 * Validate the number of values in a given field of a given report, after
 * parsing.
 */
struct hid_report *hid_validate_values(struct hid_device *hid,
<<<<<<< HEAD
				       unsigned int type, unsigned int id,
=======
				       enum hid_report_type type, unsigned int id,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				       unsigned int field_index,
				       unsigned int report_counts)
{
	struct hid_report *report;

	if (type > HID_FEATURE_REPORT) {
		hid_err(hid, "invalid HID report type %u\n", type);
		return NULL;
	}

	if (id >= HID_MAX_IDS) {
		hid_err(hid, "invalid HID report id %u\n", id);
		return NULL;
	}

	/*
	 * Explicitly not using hid_get_report() here since it depends on
	 * ->numbered being checked, which may not always be the case when
	 * drivers go to access report values.
	 */
	if (id == 0) {
		/*
		 * Validating on id 0 means we should examine the first
		 * report in the list.
		 */
<<<<<<< HEAD
		report = list_entry(
				hid->report_enum[type].report_list.next,
=======
		report = list_first_entry_or_null(
				&hid->report_enum[type].report_list,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				struct hid_report, list);
	} else {
		report = hid->report_enum[type].report_id_hash[id];
	}
	if (!report) {
		hid_err(hid, "missing %s %u\n", hid_report_names[type], id);
		return NULL;
	}
	if (report->maxfield <= field_index) {
		hid_err(hid, "not enough fields in %s %u\n",
			hid_report_names[type], id);
		return NULL;
	}
	if (report->field[field_index]->report_count < report_counts) {
		hid_err(hid, "not enough values in %s %u field %u\n",
			hid_report_names[type], id, field_index);
		return NULL;
	}
	return report;
}
EXPORT_SYMBOL_GPL(hid_validate_values);

<<<<<<< HEAD
=======
static int hid_calculate_multiplier(struct hid_device *hid,
				     struct hid_field *multiplier)
{
	int m;
	__s32 v = *multiplier->value;
	__s32 lmin = multiplier->logical_minimum;
	__s32 lmax = multiplier->logical_maximum;
	__s32 pmin = multiplier->physical_minimum;
	__s32 pmax = multiplier->physical_maximum;

	/*
	 * "Because OS implementations will generally divide the control's
	 * reported count by the Effective Resolution Multiplier, designers
	 * should take care not to establish a potential Effective
	 * Resolution Multiplier of zero."
	 * HID Usage Table, v1.12, Section 4.3.1, p31
	 */
	if (lmax - lmin == 0)
		return 1;
	/*
	 * Handling the unit exponent is left as an exercise to whoever
	 * finds a device where that exponent is not 0.
	 */
	m = ((v - lmin)/(lmax - lmin) * (pmax - pmin) + pmin);
	if (unlikely(multiplier->unit_exponent != 0)) {
		hid_warn(hid,
			 "unsupported Resolution Multiplier unit exponent %d\n",
			 multiplier->unit_exponent);
	}

	/* There are no devices with an effective multiplier > 255 */
	if (unlikely(m == 0 || m > 255 || m < -255)) {
		hid_warn(hid, "unsupported Resolution Multiplier %d\n", m);
		m = 1;
	}

	return m;
}

static void hid_apply_multiplier_to_field(struct hid_device *hid,
					  struct hid_field *field,
					  struct hid_collection *multiplier_collection,
					  int effective_multiplier)
{
	struct hid_collection *collection;
	struct hid_usage *usage;
	int i;

	/*
	 * If multiplier_collection is NULL, the multiplier applies
	 * to all fields in the report.
	 * Otherwise, it is the Logical Collection the multiplier applies to
	 * but our field may be in a subcollection of that collection.
	 */
	for (i = 0; i < field->maxusage; i++) {
		usage = &field->usage[i];

		collection = &hid->collection[usage->collection_index];
		while (collection->parent_idx != -1 &&
		       collection != multiplier_collection)
			collection = &hid->collection[collection->parent_idx];

		if (collection->parent_idx != -1 ||
		    multiplier_collection == NULL)
			usage->resolution_multiplier = effective_multiplier;

	}
}

static void hid_apply_multiplier(struct hid_device *hid,
				 struct hid_field *multiplier)
{
	struct hid_report_enum *rep_enum;
	struct hid_report *rep;
	struct hid_field *field;
	struct hid_collection *multiplier_collection;
	int effective_multiplier;
	int i;

	/*
	 * "The Resolution Multiplier control must be contained in the same
	 * Logical Collection as the control(s) to which it is to be applied.
	 * If no Resolution Multiplier is defined, then the Resolution
	 * Multiplier defaults to 1.  If more than one control exists in a
	 * Logical Collection, the Resolution Multiplier is associated with
	 * all controls in the collection. If no Logical Collection is
	 * defined, the Resolution Multiplier is associated with all
	 * controls in the report."
	 * HID Usage Table, v1.12, Section 4.3.1, p30
	 *
	 * Thus, search from the current collection upwards until we find a
	 * logical collection. Then search all fields for that same parent
	 * collection. Those are the fields the multiplier applies to.
	 *
	 * If we have more than one multiplier, it will overwrite the
	 * applicable fields later.
	 */
	multiplier_collection = &hid->collection[multiplier->usage->collection_index];
	while (multiplier_collection->parent_idx != -1 &&
	       multiplier_collection->type != HID_COLLECTION_LOGICAL)
		multiplier_collection = &hid->collection[multiplier_collection->parent_idx];

	effective_multiplier = hid_calculate_multiplier(hid, multiplier);

	rep_enum = &hid->report_enum[HID_INPUT_REPORT];
	list_for_each_entry(rep, &rep_enum->report_list, list) {
		for (i = 0; i < rep->maxfield; i++) {
			field = rep->field[i];
			hid_apply_multiplier_to_field(hid, field,
						      multiplier_collection,
						      effective_multiplier);
		}
	}
}

/*
 * hid_setup_resolution_multiplier - set up all resolution multipliers
 *
 * @device: hid device
 *
 * Search for all Resolution Multiplier Feature Reports and apply their
 * value to all matching Input items. This only updates the internal struct
 * fields.
 *
 * The Resolution Multiplier is applied by the hardware. If the multiplier
 * is anything other than 1, the hardware will send pre-multiplied events
 * so that the same physical interaction generates an accumulated
 *	accumulated_value = value * * multiplier
 * This may be achieved by sending
 * - "value * multiplier" for each event, or
 * - "value" but "multiplier" times as frequently, or
 * - a combination of the above
 * The only guarantee is that the same physical interaction always generates
 * an accumulated 'value * multiplier'.
 *
 * This function must be called before any event processing and after
 * any SetRequest to the Resolution Multiplier.
 */
void hid_setup_resolution_multiplier(struct hid_device *hid)
{
	struct hid_report_enum *rep_enum;
	struct hid_report *rep;
	struct hid_usage *usage;
	int i, j;

	rep_enum = &hid->report_enum[HID_FEATURE_REPORT];
	list_for_each_entry(rep, &rep_enum->report_list, list) {
		for (i = 0; i < rep->maxfield; i++) {
			/* Ignore if report count is out of bounds. */
			if (rep->field[i]->report_count < 1)
				continue;

			for (j = 0; j < rep->field[i]->maxusage; j++) {
				usage = &rep->field[i]->usage[j];
				if (usage->hid == HID_GD_RESOLUTION_MULTIPLIER)
					hid_apply_multiplier(hid,
							     rep->field[i]);
			}
		}
	}
}
EXPORT_SYMBOL_GPL(hid_setup_resolution_multiplier);

/**
 * hid_open_report - open a driver-specific device report
 *
 * @device: hid device
 *
 * Parse a report description into a hid_device structure. Reports are
 * enumerated, fields are attached to these reports.
 * 0 returned on success, otherwise nonzero error value.
 *
 * This function (or the equivalent hid_parse() macro) should only be
 * called from probe() in drivers, before starting the device.
 */
int hid_open_report(struct hid_device *device)
{
	struct hid_parser *parser;
	struct hid_item item;
	unsigned int size;
	__u8 *start;
	__u8 *buf;
	__u8 *end;
	__u8 *next;
	int ret;
	int i;
	static int (*dispatch_type[])(struct hid_parser *parser,
				      struct hid_item *item) = {
		hid_parser_main,
		hid_parser_global,
		hid_parser_local,
		hid_parser_reserved
	};

	if (WARN_ON(device->status & HID_STAT_PARSED))
		return -EBUSY;

	start = device->dev_rdesc;
	if (WARN_ON(!start))
		return -ENODEV;
	size = device->dev_rsize;

	/* call_hid_bpf_rdesc_fixup() ensures we work on a copy of rdesc */
	buf = call_hid_bpf_rdesc_fixup(device, start, &size);
	if (buf == NULL)
		return -ENOMEM;

	if (device->driver->report_fixup)
		start = device->driver->report_fixup(device, buf, &size);
	else
		start = buf;

	start = kmemdup(start, size, GFP_KERNEL);
	kfree(buf);
	if (start == NULL)
		return -ENOMEM;

	device->rdesc = start;
	device->rsize = size;

	parser = vzalloc(sizeof(struct hid_parser));
	if (!parser) {
		ret = -ENOMEM;
		goto alloc_err;
	}

	parser->device = device;

	end = start + size;

	device->collection = kcalloc(HID_DEFAULT_NUM_COLLECTIONS,
				     sizeof(struct hid_collection), GFP_KERNEL);
	if (!device->collection) {
		ret = -ENOMEM;
		goto err;
	}
	device->collection_size = HID_DEFAULT_NUM_COLLECTIONS;
	for (i = 0; i < HID_DEFAULT_NUM_COLLECTIONS; i++)
		device->collection[i].parent_idx = -1;

	ret = -EINVAL;
	while ((next = fetch_item(start, end, &item)) != NULL) {
		start = next;

		if (item.format != HID_ITEM_FORMAT_SHORT) {
			hid_err(device, "unexpected long global item\n");
			goto err;
		}

		if (dispatch_type[item.type](parser, &item)) {
			hid_err(device, "item %u %u %u %u parsing failed\n",
				item.format, (unsigned)item.size,
				(unsigned)item.type, (unsigned)item.tag);
			goto err;
		}

		if (start == end) {
			if (parser->collection_stack_ptr) {
				hid_err(device, "unbalanced collection at end of report description\n");
				goto err;
			}
			if (parser->local.delimiter_depth) {
				hid_err(device, "unbalanced delimiter at end of report description\n");
				goto err;
			}

			/*
			 * fetch initial values in case the device's
			 * default multiplier isn't the recommended 1
			 */
			hid_setup_resolution_multiplier(device);

			kfree(parser->collection_stack);
			vfree(parser);
			device->status |= HID_STAT_PARSED;

			return 0;
		}
	}

	hid_err(device, "item fetching failed at offset %u/%u\n",
		size - (unsigned int)(end - start), size);
err:
	kfree(parser->collection_stack);
alloc_err:
	vfree(parser);
	hid_close_report(device);
	return ret;
}
EXPORT_SYMBOL_GPL(hid_open_report);

/*
 * Convert a signed n-bit integer to signed 32-bit integer. Common
 * cases are done through the compiler, the screwed things has to be
 * done by hand.
 */

static s32 snto32(__u32 value, unsigned n)
{
	if (!value || !n)
		return 0;

	if (n > 32)
		n = 32;

	switch (n) {
	case 8:  return ((__s8)value);
	case 16: return ((__s16)value);
	case 32: return ((__s32)value);
	}
	return value & (1 << (n - 1)) ? value | (~0U << n) : value;
}

s32 hid_snto32(__u32 value, unsigned n)
{
	return snto32(value, n);
}
EXPORT_SYMBOL_GPL(hid_snto32);

/*
 * Convert a signed 32-bit integer to a signed n-bit integer.
 */

static u32 s32ton(__s32 value, unsigned n)
{
	s32 a = value >> (n - 1);
	if (a && a != -1)
		return value < 0 ? 1 << (n - 1) : (1 << (n - 1)) - 1;
	return value & ((1 << n) - 1);
}

/*
 * Extract/implement a data field from/to a little endian report (bit array).
 *
 * Code sort-of follows HID spec:
 *     http://www.usb.org/developers/hidpage/HID1_11.pdf
 *
 * While the USB HID spec allows unlimited length bit fields in "report
 * descriptors", most devices never use more than 16 bits.
 * One model of UPS is claimed to report "LINEV" as a 32-bit field.
 * Search linux-kernel and linux-usb-devel archives for "hid-core extract".
 */

static u32 __extract(u8 *report, unsigned offset, int n)
{
	unsigned int idx = offset / 8;
	unsigned int bit_nr = 0;
	unsigned int bit_shift = offset % 8;
	int bits_to_copy = 8 - bit_shift;
	u32 value = 0;
	u32 mask = n < 32 ? (1U << n) - 1 : ~0U;

	while (n > 0) {
		value |= ((u32)report[idx] >> bit_shift) << bit_nr;
		n -= bits_to_copy;
		bit_nr += bits_to_copy;
		bits_to_copy = 8;
		bit_shift = 0;
		idx++;
	}

	return value & mask;
}

u32 hid_field_extract(const struct hid_device *hid, u8 *report,
			unsigned offset, unsigned n)
{
	if (n > 32) {
		hid_warn_once(hid, "%s() called with n (%d) > 32! (%s)\n",
			      __func__, n, current->comm);
		n = 32;
	}

	return __extract(report, offset, n);
}
EXPORT_SYMBOL_GPL(hid_field_extract);

/*
 * "implement" : set bits in a little endian bit stream.
 * Same concepts as "extract" (see comments above).
 * The data mangled in the bit stream remains in little endian
 * order the whole time. It make more sense to talk about
 * endianness of register values by considering a register
 * a "cached" copy of the little endian bit stream.
 */

static void __implement(u8 *report, unsigned offset, int n, u32 value)
{
	unsigned int idx = offset / 8;
	unsigned int bit_shift = offset % 8;
	int bits_to_set = 8 - bit_shift;

	while (n - bits_to_set >= 0) {
		report[idx] &= ~(0xff << bit_shift);
		report[idx] |= value << bit_shift;
		value >>= bits_to_set;
		n -= bits_to_set;
		bits_to_set = 8;
		bit_shift = 0;
		idx++;
	}

	/* last nibble */
	if (n) {
		u8 bit_mask = ((1U << n) - 1);
		report[idx] &= ~(bit_mask << bit_shift);
		report[idx] |= value << bit_shift;
	}
}

static void implement(const struct hid_device *hid, u8 *report,
		      unsigned offset, unsigned n, u32 value)
{
	if (unlikely(n > 32)) {
		hid_warn(hid, "%s() called with n (%d) > 32! (%s)\n",
			 __func__, n, current->comm);
		n = 32;
	} else if (n < 32) {
		u32 m = (1U << n) - 1;

		if (unlikely(value > m)) {
			hid_warn(hid,
				 "%s() called with too large value %d (n: %d)! (%s)\n",
				 __func__, value, n, current->comm);
			WARN_ON(1);
			value &= m;
		}
	}

	__implement(report, offset, n, value);
}

/*
 * Search an array for a value.
 */

static int search(__s32 *array, __s32 value, unsigned n)
{
	while (n--) {
		if (*array++ == value)
			return 0;
	}
	return -1;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * hid_match_report - check if driver's raw_event should be called
 *
 * @hid: hid device
<<<<<<< HEAD
 * @report_type: type to match against
=======
 * @report: hid report to match against
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * compare hid->driver->report_table->report_type to report->type
 */
static int hid_match_report(struct hid_device *hid, struct hid_report *report)
{
	const struct hid_report_id *id = hid->driver->report_table;

	if (!id) /* NULL means all */
		return 1;

	for (; id->report_type != HID_TERMINATOR; id++)
		if (id->report_type == HID_ANY_ID ||
				id->report_type == report->type)
			return 1;
	return 0;
}

/**
 * hid_match_usage - check if driver's event should be called
 *
 * @hid: hid device
 * @usage: usage to match against
 *
 * compare hid->driver->usage_table->usage_{type,code} to
 * usage->usage_{type,code}
 */
static int hid_match_usage(struct hid_device *hid, struct hid_usage *usage)
{
	const struct hid_usage_id *id = hid->driver->usage_table;

	if (!id) /* NULL means all */
		return 1;

	for (; id->usage_type != HID_ANY_ID - 1; id++)
		if ((id->usage_hid == HID_ANY_ID ||
				id->usage_hid == usage->hid) &&
				(id->usage_type == HID_ANY_ID ||
				id->usage_type == usage->type) &&
				(id->usage_code == HID_ANY_ID ||
				 id->usage_code == usage->code))
			return 1;
	return 0;
}

static void hid_process_event(struct hid_device *hid, struct hid_field *field,
		struct hid_usage *usage, __s32 value, int interrupt)
{
	struct hid_driver *hdrv = hid->driver;
	int ret;

<<<<<<< HEAD
	hid_dump_input(hid, usage, value);
=======
	if (!list_empty(&hid->debug_list))
		hid_dump_input(hid, usage, value);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (hdrv && hdrv->event && hid_match_usage(hid, usage)) {
		ret = hdrv->event(hid, field, usage, value);
		if (ret != 0) {
			if (ret < 0)
				hid_err(hid, "%s's event failed with %d\n",
						hdrv->name, ret);
			return;
		}
	}

	if (hid->claimed & HID_CLAIMED_INPUT)
		hidinput_hid_event(hid, field, usage, value);
	if (hid->claimed & HID_CLAIMED_HIDDEV && interrupt && hid->hiddev_hid_event)
		hid->hiddev_hid_event(hid, field, usage, value);
}

/*
<<<<<<< HEAD
 * Analyse a received field, and fetch the data from it. The field
 * content is stored for next report processing (we do differential
 * reporting to the layer).
 */

static void hid_input_field(struct hid_device *hid, struct hid_field *field,
			    __u8 *data, int interrupt)
=======
 * Checks if the given value is valid within this field
 */
static inline int hid_array_value_is_valid(struct hid_field *field,
					   __s32 value)
{
	__s32 min = field->logical_minimum;

	/*
	 * Value needs to be between logical min and max, and
	 * (value - min) is used as an index in the usage array.
	 * This array is of size field->maxusage
	 */
	return value >= min &&
	       value <= field->logical_maximum &&
	       value - min < field->maxusage;
}

/*
 * Fetch the field from the data. The field content is stored for next
 * report processing (we do differential reporting to the layer).
 */
static void hid_input_fetch_field(struct hid_device *hid,
				  struct hid_field *field,
				  __u8 *data)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned n;
	unsigned count = field->report_count;
	unsigned offset = field->report_offset;
	unsigned size = field->report_size;
	__s32 min = field->logical_minimum;
<<<<<<< HEAD
	__s32 max = field->logical_maximum;
	__s32 *value;

	value = kmalloc(sizeof(__s32) * count, GFP_ATOMIC);
	if (!value)
		return;
=======
	__s32 *value;

	value = field->new_value;
	memset(value, 0, count * sizeof(__s32));
	field->ignored = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (n = 0; n < count; n++) {

		value[n] = min < 0 ?
<<<<<<< HEAD
			snto32(extract(hid, data, offset + n * size, size),
			       size) :
			extract(hid, data, offset + n * size, size);

		/* Ignore report if ErrorRollOver */
		if (!(field->flags & HID_MAIN_ITEM_VARIABLE) &&
		    value[n] >= min && value[n] <= max &&
		    value[n] - min < field->maxusage &&
		    field->usage[value[n] - min].hid == HID_UP_KEYBOARD + 1)
			goto exit;
	}

	for (n = 0; n < count; n++) {

		if (HID_MAIN_ITEM_VARIABLE & field->flags) {
			hid_process_event(hid, field, &field->usage[n], value[n], interrupt);
			continue;
		}

		if (field->value[n] >= min && field->value[n] <= max
			&& field->value[n] - min < field->maxusage
			&& field->usage[field->value[n] - min].hid
			&& search(value, field->value[n], count))
				hid_process_event(hid, field, &field->usage[field->value[n] - min], 0, interrupt);

		if (value[n] >= min && value[n] <= max
			&& value[n] - min < field->maxusage
			&& field->usage[value[n] - min].hid
			&& search(field->value, value[n], count))
				hid_process_event(hid, field, &field->usage[value[n] - min], 1, interrupt);
	}

	memcpy(field->value, value, count * sizeof(__s32));
exit:
	kfree(value);
=======
			snto32(hid_field_extract(hid, data, offset + n * size,
			       size), size) :
			hid_field_extract(hid, data, offset + n * size, size);

		/* Ignore report if ErrorRollOver */
		if (!(field->flags & HID_MAIN_ITEM_VARIABLE) &&
		    hid_array_value_is_valid(field, value[n]) &&
		    field->usage[value[n] - min].hid == HID_UP_KEYBOARD + 1) {
			field->ignored = true;
			return;
		}
	}
}

/*
 * Process a received variable field.
 */

static void hid_input_var_field(struct hid_device *hid,
				struct hid_field *field,
				int interrupt)
{
	unsigned int count = field->report_count;
	__s32 *value = field->new_value;
	unsigned int n;

	for (n = 0; n < count; n++)
		hid_process_event(hid,
				  field,
				  &field->usage[n],
				  value[n],
				  interrupt);

	memcpy(field->value, value, count * sizeof(__s32));
}

/*
 * Process a received array field. The field content is stored for
 * next report processing (we do differential reporting to the layer).
 */

static void hid_input_array_field(struct hid_device *hid,
				  struct hid_field *field,
				  int interrupt)
{
	unsigned int n;
	unsigned int count = field->report_count;
	__s32 min = field->logical_minimum;
	__s32 *value;

	value = field->new_value;

	/* ErrorRollOver */
	if (field->ignored)
		return;

	for (n = 0; n < count; n++) {
		if (hid_array_value_is_valid(field, field->value[n]) &&
		    search(value, field->value[n], count))
			hid_process_event(hid,
					  field,
					  &field->usage[field->value[n] - min],
					  0,
					  interrupt);

		if (hid_array_value_is_valid(field, value[n]) &&
		    search(field->value, value[n], count))
			hid_process_event(hid,
					  field,
					  &field->usage[value[n] - min],
					  1,
					  interrupt);
	}

	memcpy(field->value, value, count * sizeof(__s32));
}

/*
 * Analyse a received report, and fetch the data from it. The field
 * content is stored for next report processing (we do differential
 * reporting to the layer).
 */
static void hid_process_report(struct hid_device *hid,
			       struct hid_report *report,
			       __u8 *data,
			       int interrupt)
{
	unsigned int a;
	struct hid_field_entry *entry;
	struct hid_field *field;

	/* first retrieve all incoming values in data */
	for (a = 0; a < report->maxfield; a++)
		hid_input_fetch_field(hid, report->field[a], data);

	if (!list_empty(&report->field_entry_list)) {
		/* INPUT_REPORT, we have a priority list of fields */
		list_for_each_entry(entry,
				    &report->field_entry_list,
				    list) {
			field = entry->field;

			if (field->flags & HID_MAIN_ITEM_VARIABLE)
				hid_process_event(hid,
						  field,
						  &field->usage[entry->index],
						  field->new_value[entry->index],
						  interrupt);
			else
				hid_input_array_field(hid, field, interrupt);
		}

		/* we need to do the memcpy at the end for var items */
		for (a = 0; a < report->maxfield; a++) {
			field = report->field[a];

			if (field->flags & HID_MAIN_ITEM_VARIABLE)
				memcpy(field->value, field->new_value,
				       field->report_count * sizeof(__s32));
		}
	} else {
		/* FEATURE_REPORT, regular processing */
		for (a = 0; a < report->maxfield; a++) {
			field = report->field[a];

			if (field->flags & HID_MAIN_ITEM_VARIABLE)
				hid_input_var_field(hid, field, interrupt);
			else
				hid_input_array_field(hid, field, interrupt);
		}
	}
}

/*
 * Insert a given usage_index in a field in the list
 * of processed usages in the report.
 *
 * The elements of lower priority score are processed
 * first.
 */
static void __hid_insert_field_entry(struct hid_device *hid,
				     struct hid_report *report,
				     struct hid_field_entry *entry,
				     struct hid_field *field,
				     unsigned int usage_index)
{
	struct hid_field_entry *next;

	entry->field = field;
	entry->index = usage_index;
	entry->priority = field->usages_priorities[usage_index];

	/* insert the element at the correct position */
	list_for_each_entry(next,
			    &report->field_entry_list,
			    list) {
		/*
		 * the priority of our element is strictly higher
		 * than the next one, insert it before
		 */
		if (entry->priority > next->priority) {
			list_add_tail(&entry->list, &next->list);
			return;
		}
	}

	/* lowest priority score: insert at the end */
	list_add_tail(&entry->list, &report->field_entry_list);
}

static void hid_report_process_ordering(struct hid_device *hid,
					struct hid_report *report)
{
	struct hid_field *field;
	struct hid_field_entry *entries;
	unsigned int a, u, usages;
	unsigned int count = 0;

	/* count the number of individual fields in the report */
	for (a = 0; a < report->maxfield; a++) {
		field = report->field[a];

		if (field->flags & HID_MAIN_ITEM_VARIABLE)
			count += field->report_count;
		else
			count++;
	}

	/* allocate the memory to process the fields */
	entries = kcalloc(count, sizeof(*entries), GFP_KERNEL);
	if (!entries)
		return;

	report->field_entries = entries;

	/*
	 * walk through all fields in the report and
	 * store them by priority order in report->field_entry_list
	 *
	 * - Var elements are individualized (field + usage_index)
	 * - Arrays are taken as one, we can not chose an order for them
	 */
	usages = 0;
	for (a = 0; a < report->maxfield; a++) {
		field = report->field[a];

		if (field->flags & HID_MAIN_ITEM_VARIABLE) {
			for (u = 0; u < field->report_count; u++) {
				__hid_insert_field_entry(hid, report,
							 &entries[usages],
							 field, u);
				usages++;
			}
		} else {
			__hid_insert_field_entry(hid, report, &entries[usages],
						 field, 0);
			usages++;
		}
	}
}

static void hid_process_ordering(struct hid_device *hid)
{
	struct hid_report *report;
	struct hid_report_enum *report_enum = &hid->report_enum[HID_INPUT_REPORT];

	list_for_each_entry(report, &report_enum->report_list, list)
		hid_report_process_ordering(hid, report);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Output the field into the report.
 */

static void hid_output_field(const struct hid_device *hid,
			     struct hid_field *field, __u8 *data)
{
	unsigned count = field->report_count;
	unsigned offset = field->report_offset;
	unsigned size = field->report_size;
	unsigned n;

	for (n = 0; n < count; n++) {
		if (field->logical_minimum < 0)	/* signed values */
			implement(hid, data, offset + n * size, size,
				  s32ton(field->value[n], size));
		else				/* unsigned values */
			implement(hid, data, offset + n * size, size,
				  field->value[n]);
	}
}

/*
<<<<<<< HEAD
 * Create a report.
=======
 * Compute the size of a report.
 */
static size_t hid_compute_report_size(struct hid_report *report)
{
	if (report->size)
		return ((report->size - 1) >> 3) + 1;

	return 0;
}

/*
 * Create a report. 'data' has to be allocated using
 * hid_alloc_report_buf() so that it has proper size.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

void hid_output_report(struct hid_report *report, __u8 *data)
{
	unsigned n;

	if (report->id > 0)
		*data++ = report->id;

<<<<<<< HEAD
	memset(data, 0, ((report->size - 1) >> 3) + 1);
=======
	memset(data, 0, hid_compute_report_size(report));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (n = 0; n < report->maxfield; n++)
		hid_output_field(report->device, report->field[n], data);
}
EXPORT_SYMBOL_GPL(hid_output_report);

/*
<<<<<<< HEAD
=======
 * Allocator for buffer that is going to be passed to hid_output_report()
 */
u8 *hid_alloc_report_buf(struct hid_report *report, gfp_t flags)
{
	/*
	 * 7 extra bytes are necessary to achieve proper functionality
	 * of implement() working on 8 byte chunks
	 */

	u32 len = hid_report_len(report) + 7;

	return kmalloc(len, flags);
}
EXPORT_SYMBOL_GPL(hid_alloc_report_buf);

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Set a field value. The report this field belongs to has to be
 * created and transferred to the device, to set this value in the
 * device.
 */

int hid_set_field(struct hid_field *field, unsigned offset, __s32 value)
{
	unsigned size;

	if (!field)
		return -1;

	size = field->report_size;

	hid_dump_input(field->report->device, field->usage + offset, value);

	if (offset >= field->report_count) {
		hid_err(field->report->device, "offset (%d) exceeds report_count (%d)\n",
				offset, field->report_count);
		return -1;
	}
	if (field->logical_minimum < 0) {
		if (value != snto32(s32ton(value, size), size)) {
			hid_err(field->report->device, "value %d is out of range\n", value);
			return -1;
		}
	}
	field->value[offset] = value;
	return 0;
}
EXPORT_SYMBOL_GPL(hid_set_field);

static struct hid_report *hid_get_report(struct hid_report_enum *report_enum,
		const u8 *data)
{
	struct hid_report *report;
	unsigned int n = 0;	/* Normally report number is 0 */

	/* Device uses numbered reports, data[0] is report number */
	if (report_enum->numbered)
		n = *data;

	report = report_enum->report_id_hash[n];
	if (report == NULL)
		dbg_hid("undefined report_id %u received\n", n);

	return report;
}

<<<<<<< HEAD
int hid_report_raw_event(struct hid_device *hid, int type, u8 *data, int size,
		int interrupt)
{
	struct hid_report_enum *report_enum = hid->report_enum + type;
	struct hid_report *report;
	unsigned int a;
	int rsize, csize = size;
=======
/*
 * Implement a generic .request() callback, using .raw_request()
 * DO NOT USE in hid drivers directly, but through hid_hw_request instead.
 */
int __hid_request(struct hid_device *hid, struct hid_report *report,
		enum hid_class_request reqtype)
{
	char *buf;
	int ret;
	u32 len;

	buf = hid_alloc_report_buf(report, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	len = hid_report_len(report);

	if (reqtype == HID_REQ_SET_REPORT)
		hid_output_report(report, buf);

	ret = hid->ll_driver->raw_request(hid, report->id, buf, len,
					  report->type, reqtype);
	if (ret < 0) {
		dbg_hid("unable to complete request: %d\n", ret);
		goto out;
	}

	if (reqtype == HID_REQ_GET_REPORT)
		hid_input_report(hid, report->type, buf, ret, 0);

	ret = 0;

out:
	kfree(buf);
	return ret;
}
EXPORT_SYMBOL_GPL(__hid_request);

int hid_report_raw_event(struct hid_device *hid, enum hid_report_type type, u8 *data, u32 size,
			 int interrupt)
{
	struct hid_report_enum *report_enum = hid->report_enum + type;
	struct hid_report *report;
	struct hid_driver *hdrv;
	int max_buffer_size = HID_MAX_BUFFER_SIZE;
	u32 rsize, csize = size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u8 *cdata = data;
	int ret = 0;

	report = hid_get_report(report_enum, data);
	if (!report)
		goto out;

	if (report_enum->numbered) {
		cdata++;
		csize--;
	}

<<<<<<< HEAD
	rsize = ((report->size - 1) >> 3) + 1;

	if (rsize > HID_MAX_BUFFER_SIZE)
		rsize = HID_MAX_BUFFER_SIZE;
=======
	rsize = hid_compute_report_size(report);

	if (hid->ll_driver->max_buffer_size)
		max_buffer_size = hid->ll_driver->max_buffer_size;

	if (report_enum->numbered && rsize >= max_buffer_size)
		rsize = max_buffer_size - 1;
	else if (rsize > max_buffer_size)
		rsize = max_buffer_size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (csize < rsize) {
		dbg_hid("report %d is too short, (%d < %d)\n", report->id,
				csize, rsize);
		memset(cdata + csize, 0, rsize - csize);
	}

	if ((hid->claimed & HID_CLAIMED_HIDDEV) && hid->hiddev_report_event)
		hid->hiddev_report_event(hid, report);
	if (hid->claimed & HID_CLAIMED_HIDRAW) {
		ret = hidraw_report_event(hid, data, size);
		if (ret)
			goto out;
	}

<<<<<<< HEAD
	for (a = 0; a < report->maxfield; a++)
		hid_input_field(hid, report->field[a], cdata, interrupt);
=======
	if (hid->claimed != HID_CLAIMED_HIDRAW && report->maxfield) {
		hid_process_report(hid, report, cdata, interrupt);
		hdrv = hid->driver;
		if (hdrv && hdrv->report)
			hdrv->report(hid, report);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (hid->claimed & HID_CLAIMED_INPUT)
		hidinput_report_event(hid, report);
out:
	return ret;
}
EXPORT_SYMBOL_GPL(hid_report_raw_event);

/**
 * hid_input_report - report data from lower layer (usb, bt...)
 *
 * @hid: hid device
 * @type: HID report type (HID_*_REPORT)
 * @data: report contents
 * @size: size of data parameter
 * @interrupt: distinguish between interrupt and control transfers
 *
 * This is data entry for lower layers.
 */
<<<<<<< HEAD
int hid_input_report(struct hid_device *hid, int type, u8 *data, int size, int interrupt)
=======
int hid_input_report(struct hid_device *hid, enum hid_report_type type, u8 *data, u32 size,
		     int interrupt)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct hid_report_enum *report_enum;
	struct hid_driver *hdrv;
	struct hid_report *report;
<<<<<<< HEAD
	char *buf;
	unsigned int i;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret = 0;

	if (!hid)
		return -ENODEV;

<<<<<<< HEAD
	if (down_trylock(&hid->driver_lock))
=======
	if (down_trylock(&hid->driver_input_lock))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EBUSY;

	if (!hid->driver) {
		ret = -ENODEV;
		goto unlock;
	}
	report_enum = hid->report_enum + type;
	hdrv = hid->driver;

<<<<<<< HEAD
=======
	data = dispatch_hid_bpf_device_event(hid, type, data, &size, interrupt);
	if (IS_ERR(data)) {
		ret = PTR_ERR(data);
		goto unlock;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!size) {
		dbg_hid("empty report\n");
		ret = -1;
		goto unlock;
	}

<<<<<<< HEAD
	buf = kmalloc(sizeof(char) * HID_DEBUG_BUFSIZE, GFP_ATOMIC);

	if (!buf)
		goto nomem;

	/* dump the report */
	snprintf(buf, HID_DEBUG_BUFSIZE - 1,
			"\nreport (size %u) (%snumbered) = ", size, report_enum->numbered ? "" : "un");
	hid_debug_event(hid, buf);

	for (i = 0; i < size; i++) {
		snprintf(buf, HID_DEBUG_BUFSIZE - 1,
				" %02x", data[i]);
		hid_debug_event(hid, buf);
	}
	hid_debug_event(hid, "\n");
	kfree(buf);

nomem:
=======
	/* Avoid unnecessary overhead if debugfs is disabled */
	if (!list_empty(&hid->debug_list))
		hid_dump_report(hid, type, data, size);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	report = hid_get_report(report_enum, data);

	if (!report) {
		ret = -1;
		goto unlock;
	}

	if (hdrv && hdrv->raw_event && hid_match_report(hid, report)) {
		ret = hdrv->raw_event(hid, report, data, size);
<<<<<<< HEAD
		if (ret != 0) {
			ret = ret < 0 ? ret : 0;
			goto unlock;
		}
=======
		if (ret < 0)
			goto unlock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	ret = hid_report_raw_event(hid, type, data, size, interrupt);

unlock:
<<<<<<< HEAD
	up(&hid->driver_lock);
=======
	up(&hid->driver_input_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}
EXPORT_SYMBOL_GPL(hid_input_report);

<<<<<<< HEAD
static bool hid_match_one_id(struct hid_device *hdev,
		const struct hid_device_id *id)
{
	return id->bus == hdev->bus &&
=======
bool hid_match_one_id(const struct hid_device *hdev,
		      const struct hid_device_id *id)
{
	return (id->bus == HID_BUS_ANY || id->bus == hdev->bus) &&
		(id->group == HID_GROUP_ANY || id->group == hdev->group) &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		(id->vendor == HID_ANY_ID || id->vendor == hdev->vendor) &&
		(id->product == HID_ANY_ID || id->product == hdev->product);
}

<<<<<<< HEAD
const struct hid_device_id *hid_match_id(struct hid_device *hdev,
=======
const struct hid_device_id *hid_match_id(const struct hid_device *hdev,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		const struct hid_device_id *id)
{
	for (; id->bus; id++)
		if (hid_match_one_id(hdev, id))
			return id;

	return NULL;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(hid_match_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static const struct hid_device_id hid_hiddev_list[] = {
	{ HID_USB_DEVICE(USB_VENDOR_ID_MGE, USB_DEVICE_ID_MGE_UPS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MGE, USB_DEVICE_ID_MGE_UPS1) },
	{ }
};

static bool hid_hiddev(struct hid_device *hdev)
{
	return !!hid_match_id(hdev, hid_hiddev_list);
}


static ssize_t
read_report_descriptor(struct file *filp, struct kobject *kobj,
		struct bin_attribute *attr,
		char *buf, loff_t off, size_t count)
{
<<<<<<< HEAD
	struct device *dev = container_of(kobj, struct device, kobj);
	struct hid_device *hdev = container_of(dev, struct hid_device, dev);
=======
	struct device *dev = kobj_to_dev(kobj);
	struct hid_device *hdev = to_hid_device(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (off >= hdev->rsize)
		return 0;

	if (off + count > hdev->rsize)
		count = hdev->rsize - off;

	memcpy(buf, hdev->rdesc + off, count);

	return count;
}

<<<<<<< HEAD
=======
static ssize_t
show_country(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	struct hid_device *hdev = to_hid_device(dev);

	return sprintf(buf, "%02x\n", hdev->country & 0xff);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct bin_attribute dev_bin_attr_report_desc = {
	.attr = { .name = "report_descriptor", .mode = 0444 },
	.read = read_report_descriptor,
	.size = HID_MAX_DESCRIPTOR_SIZE,
};

<<<<<<< HEAD
=======
static const struct device_attribute dev_attr_country = {
	.attr = { .name = "country", .mode = 0444 },
	.show = show_country,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int hid_connect(struct hid_device *hdev, unsigned int connect_mask)
{
	static const char *types[] = { "Device", "Pointer", "Mouse", "Device",
		"Joystick", "Gamepad", "Keyboard", "Keypad",
		"Multi-Axis Controller"
	};
	const char *type, *bus;
	char buf[64] = "";
	unsigned int i;
	int len;
	int ret;

<<<<<<< HEAD
=======
	ret = hid_bpf_connect_device(hdev);
	if (ret)
		return ret;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (hdev->quirks & HID_QUIRK_HIDDEV_FORCE)
		connect_mask |= (HID_CONNECT_HIDDEV_FORCE | HID_CONNECT_HIDDEV);
	if (hdev->quirks & HID_QUIRK_HIDINPUT_FORCE)
		connect_mask |= HID_CONNECT_HIDINPUT_FORCE;
	if (hdev->bus != BUS_USB)
		connect_mask &= ~HID_CONNECT_HIDDEV;
	if (hid_hiddev(hdev))
		connect_mask |= HID_CONNECT_HIDDEV_FORCE;

	if ((connect_mask & HID_CONNECT_HIDINPUT) && !hidinput_connect(hdev,
				connect_mask & HID_CONNECT_HIDINPUT_FORCE))
		hdev->claimed |= HID_CLAIMED_INPUT;
<<<<<<< HEAD
	if (hdev->quirks & HID_QUIRK_MULTITOUCH) {
		/* this device should be handled by hid-multitouch, skip it */
		return -ENODEV;
	}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if ((connect_mask & HID_CONNECT_HIDDEV) && hdev->hiddev_connect &&
			!hdev->hiddev_connect(hdev,
				connect_mask & HID_CONNECT_HIDDEV_FORCE))
		hdev->claimed |= HID_CLAIMED_HIDDEV;
	if ((connect_mask & HID_CONNECT_HIDRAW) && !hidraw_connect(hdev))
		hdev->claimed |= HID_CLAIMED_HIDRAW;

<<<<<<< HEAD
	if (!hdev->claimed) {
		hid_err(hdev, "claimed by neither input, hiddev nor hidraw\n");
		return -ENODEV;
	}

=======
	if (connect_mask & HID_CONNECT_DRIVER)
		hdev->claimed |= HID_CLAIMED_DRIVER;

	/* Drivers with the ->raw_event callback set are not required to connect
	 * to any other listener. */
	if (!hdev->claimed && !hdev->driver->raw_event) {
		hid_err(hdev, "device has no listeners, quitting\n");
		return -ENODEV;
	}

	hid_process_ordering(hdev);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if ((hdev->claimed & HID_CLAIMED_INPUT) &&
			(connect_mask & HID_CONNECT_FF) && hdev->ff_init)
		hdev->ff_init(hdev);

	len = 0;
	if (hdev->claimed & HID_CLAIMED_INPUT)
		len += sprintf(buf + len, "input");
	if (hdev->claimed & HID_CLAIMED_HIDDEV)
		len += sprintf(buf + len, "%shiddev%d", len ? "," : "",
<<<<<<< HEAD
				hdev->minor);
=======
				((struct hiddev *)hdev->hiddev)->minor);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (hdev->claimed & HID_CLAIMED_HIDRAW)
		len += sprintf(buf + len, "%shidraw%d", len ? "," : "",
				((struct hidraw *)hdev->hidraw)->minor);

	type = "Device";
	for (i = 0; i < hdev->maxcollection; i++) {
		struct hid_collection *col = &hdev->collection[i];
		if (col->type == HID_COLLECTION_APPLICATION &&
		   (col->usage & HID_USAGE_PAGE) == HID_UP_GENDESK &&
		   (col->usage & 0xffff) < ARRAY_SIZE(types)) {
			type = types[col->usage & 0xffff];
			break;
		}
	}

	switch (hdev->bus) {
	case BUS_USB:
		bus = "USB";
		break;
	case BUS_BLUETOOTH:
		bus = "BLUETOOTH";
		break;
<<<<<<< HEAD
=======
	case BUS_I2C:
		bus = "I2C";
		break;
	case BUS_VIRTUAL:
		bus = "VIRTUAL";
		break;
	case BUS_INTEL_ISHTP:
	case BUS_AMD_SFH:
		bus = "SENSOR HUB";
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		bus = "<UNKNOWN>";
	}

<<<<<<< HEAD
	ret = device_create_bin_file(&hdev->dev, &dev_bin_attr_report_desc);
	if (ret)
		hid_warn(hdev,
			 "can't create sysfs report descriptor attribute err: %d\n", ret);
=======
	ret = device_create_file(&hdev->dev, &dev_attr_country);
	if (ret)
		hid_warn(hdev,
			 "can't create sysfs country code attribute err: %d\n", ret);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	hid_info(hdev, "%s: %s HID v%x.%02x %s [%s] on %s\n",
		 buf, bus, hdev->version >> 8, hdev->version & 0xff,
		 type, hdev->name, hdev->phys);

	return 0;
}
EXPORT_SYMBOL_GPL(hid_connect);

void hid_disconnect(struct hid_device *hdev)
{
<<<<<<< HEAD
	device_remove_bin_file(&hdev->dev, &dev_bin_attr_report_desc);
=======
	device_remove_file(&hdev->dev, &dev_attr_country);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (hdev->claimed & HID_CLAIMED_INPUT)
		hidinput_disconnect(hdev);
	if (hdev->claimed & HID_CLAIMED_HIDDEV)
		hdev->hiddev_disconnect(hdev);
	if (hdev->claimed & HID_CLAIMED_HIDRAW)
		hidraw_disconnect(hdev);
<<<<<<< HEAD
}
EXPORT_SYMBOL_GPL(hid_disconnect);

/* a list of devices for which there is a specialized driver on HID bus */
static const struct hid_device_id hid_have_special_driver[] = {
	{ HID_USB_DEVICE(USB_VENDOR_ID_3M, USB_DEVICE_ID_3M1968) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_3M, USB_DEVICE_ID_3M2256) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_A4TECH, USB_DEVICE_ID_A4TECH_WCP32PU) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_A4TECH, USB_DEVICE_ID_A4TECH_X5_005D) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_A4TECH, USB_DEVICE_ID_A4TECH_RP_649) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ACRUX, 0x0802) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ACTIONSTAR, USB_DEVICE_ID_ACTIONSTAR_1011) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_ATV_IRCONTROL) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_IRCONTROL4) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_MIGHTYMOUSE) },
	{ HID_BLUETOOTH_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_MAGICMOUSE) },
	{ HID_BLUETOOTH_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_MAGICTRACKPAD) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_FOUNTAIN_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_FOUNTAIN_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_GEYSER_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_GEYSER_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_GEYSER_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_GEYSER3_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_GEYSER3_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_GEYSER3_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_GEYSER4_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_GEYSER4_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_GEYSER4_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_ALU_MINI_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_ALU_MINI_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_ALU_MINI_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_ALU_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_ALU_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_ALU_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_GEYSER4_HF_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_GEYSER4_HF_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_GEYSER4_HF_JIS) },
	{ HID_BLUETOOTH_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_ALU_WIRELESS_ANSI) },
	{ HID_BLUETOOTH_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_ALU_WIRELESS_ISO) },
	{ HID_BLUETOOTH_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_ALU_WIRELESS_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING2_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING2_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING2_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING3_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING3_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING3_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING4_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING4_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING4_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING4A_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING4A_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING4A_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING5_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING5_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING5_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING5A_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING5A_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING5A_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_ALU_REVB_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_ALU_REVB_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_ALU_REVB_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING6_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING6_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING6_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING6A_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING6A_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING6A_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING7_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING7_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING7_JIS) },
	{ HID_BLUETOOTH_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_ALU_WIRELESS_2009_ANSI) },
	{ HID_BLUETOOTH_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_ALU_WIRELESS_2009_ISO) },
	{ HID_BLUETOOTH_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_ALU_WIRELESS_2009_JIS) },
	{ HID_BLUETOOTH_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_ALU_WIRELESS_2011_ANSI) },
	{ HID_BLUETOOTH_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_ALU_WIRELESS_2011_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_FOUNTAIN_TP_ONLY) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_GEYSER1_TP_ONLY) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ASUS, USB_DEVICE_ID_ASUS_T91MT) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ASUS, USB_DEVICE_ID_ASUSTEK_MULTITOUCH_YFO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_BELKIN, USB_DEVICE_ID_FLIP_KVM) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_BAANTO, USB_DEVICE_ID_BAANTO_MT_190W2), },
	{ HID_USB_DEVICE(USB_VENDOR_ID_BTC, USB_DEVICE_ID_BTC_EMPREX_REMOTE) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_BTC, USB_DEVICE_ID_BTC_EMPREX_REMOTE_2) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CANDO, USB_DEVICE_ID_CANDO_PIXCIR_MULTI_TOUCH) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CANDO, USB_DEVICE_ID_CANDO_MULTI_TOUCH) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CANDO, USB_DEVICE_ID_CANDO_MULTI_TOUCH_10_1) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CANDO, USB_DEVICE_ID_CANDO_MULTI_TOUCH_11_6) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CANDO, USB_DEVICE_ID_CANDO_MULTI_TOUCH_15_6) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CHERRY, USB_DEVICE_ID_CHERRY_CYMOTION) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CHERRY, USB_DEVICE_ID_CHERRY_CYMOTION_SOLAR) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CHICONY, USB_DEVICE_ID_CHICONY_TACTICAL_PAD) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CHICONY, USB_DEVICE_ID_CHICONY_WIRELESS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CHICONY, USB_DEVICE_ID_CHICONY_WIRELESS2) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CHICONY, USB_DEVICE_ID_CHICONY_AK1D) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CHUNGHWAT, USB_DEVICE_ID_CHUNGHWAT_MULTITOUCH) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CREATIVELABS, USB_DEVICE_ID_PRODIKEYS_PCMIDI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CVTOUCH, USB_DEVICE_ID_CVTOUCH_SCREEN) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CYPRESS, USB_DEVICE_ID_CYPRESS_BARCODE_1) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CYPRESS, USB_DEVICE_ID_CYPRESS_BARCODE_2) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CYPRESS, USB_DEVICE_ID_CYPRESS_BARCODE_3) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CYPRESS, USB_DEVICE_ID_CYPRESS_BARCODE_4) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CYPRESS, USB_DEVICE_ID_CYPRESS_MOUSE) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CYPRESS, USB_DEVICE_ID_CYPRESS_TRUETOUCH) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_DRAGONRISE, 0x0006) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_DRAGONRISE, 0x0011) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_DWAV, USB_DEVICE_ID_DWAV_EGALAX_MULTITOUCH_480D) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_DWAV, USB_DEVICE_ID_DWAV_EGALAX_MULTITOUCH_480E) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_DWAV, USB_DEVICE_ID_DWAV_EGALAX_MULTITOUCH_720C) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_DWAV, USB_DEVICE_ID_DWAV_EGALAX_MULTITOUCH_7224) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_DWAV, USB_DEVICE_ID_DWAV_EGALAX_MULTITOUCH_725E) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_DWAV, USB_DEVICE_ID_DWAV_EGALAX_MULTITOUCH_726B) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_DWAV, USB_DEVICE_ID_DWAV_EGALAX_MULTITOUCH_72A1) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_DWAV, USB_DEVICE_ID_DWAV_EGALAX_MULTITOUCH_7302) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_DWAV, USB_DEVICE_ID_DWAV_EGALAX_MULTITOUCH_A001) },
	{ HID_BLUETOOTH_DEVICE(USB_VENDOR_ID_ELECOM, USB_DEVICE_ID_ELECOM_BM084) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ELO, USB_DEVICE_ID_ELO_TS2515) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_EMS, USB_DEVICE_ID_EMS_TRIO_LINKER_PLUS_II) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_EZKEY, USB_DEVICE_ID_BTC_8193) },
	{ HID_BLUETOOTH_DEVICE(USB_VENDOR_ID_FRUCTEL, USB_DEVICE_ID_GAMETEL_MT_MODE) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GAMERON, USB_DEVICE_ID_GAMERON_DUAL_PSX_ADAPTOR) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GAMERON, USB_DEVICE_ID_GAMERON_DUAL_PCS_ADAPTOR) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GENERAL_TOUCH, USB_DEVICE_ID_GENERAL_TOUCH_WIN7_TWOFINGERS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GOODTOUCH, USB_DEVICE_ID_GOODTOUCH_000f) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GREENASIA, 0x0003) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GREENASIA, 0x0012) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GYRATION, USB_DEVICE_ID_GYRATION_REMOTE) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GYRATION, USB_DEVICE_ID_GYRATION_REMOTE_2) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GYRATION, USB_DEVICE_ID_GYRATION_REMOTE_3) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_HANVON, USB_DEVICE_ID_HANVON_MULTITOUCH) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_HANVON_ALT, USB_DEVICE_ID_HANVON_ALT_MULTITOUCH) },
 	{ HID_USB_DEVICE(USB_VENDOR_ID_IDEACOM, USB_DEVICE_ID_IDEACOM_IDC6650) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_HOLTEK, USB_DEVICE_ID_HOLTEK_ON_LINE_GRIP) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ILITEK, USB_DEVICE_ID_ILITEK_MULTITOUCH) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_IRTOUCHSYSTEMS, USB_DEVICE_ID_IRTOUCH_INFRARED_USB) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_KENSINGTON, USB_DEVICE_ID_KS_SLIMBLADE) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_KEYTOUCH, USB_DEVICE_ID_KEYTOUCH_IEC) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_KYE, USB_DEVICE_ID_KYE_ERGO_525V) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_KYE, USB_DEVICE_ID_KYE_EASYPEN_I405X) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_KYE, USB_DEVICE_ID_KYE_MOUSEPEN_I608X) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_KYE, USB_DEVICE_ID_KYE_EASYPEN_M610X) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LABTEC, USB_DEVICE_ID_LABTEC_WIRELESS_KEYBOARD) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LCPOWER, USB_DEVICE_ID_LCPOWER_LC1000 ) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LG, USB_DEVICE_ID_LG_MULTITOUCH) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_MX3000_RECEIVER) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_S510_RECEIVER) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_S510_RECEIVER_2) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_LOGITECH_RECEIVER) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_DINOVO_DESKTOP) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_DINOVO_EDGE) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_DINOVO_MINI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_LOGITECH_ELITE_KBD) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_LOGITECH_CORDLESS_DESKTOP_LX500) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_LOGITECH_EXTREME_3D) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_LOGITECH_WHEEL) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_LOGITECH_RUMBLEPAD_CORD) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_LOGITECH_RUMBLEPAD) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_LOGITECH_RUMBLEPAD2_2) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_LOGITECH_WINGMAN_F3D) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_LOGITECH_WINGMAN_FFG ) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_LOGITECH_FORCE3D_PRO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_LOGITECH_FLIGHT_SYSTEM_G940) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_LOGITECH_MOMO_WHEEL) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_LOGITECH_MOMO_WHEEL2) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_LOGITECH_DFP_WHEEL) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_LOGITECH_DFGT_WHEEL) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_LOGITECH_G25_WHEEL) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_LOGITECH_G27_WHEEL) },
#if IS_ENABLED(CONFIG_HID_LOGITECH_DJ)
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_LOGITECH_UNIFYING_RECEIVER) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_LOGITECH_UNIFYING_RECEIVER_2) },
#endif
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_LOGITECH_WII_WHEEL) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_LOGITECH_RUMBLEPAD2) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_SPACETRAVELLER) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LOGITECH, USB_DEVICE_ID_SPACENAVIGATOR) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LUMIO, USB_DEVICE_ID_CRYSTALTOUCH) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LUMIO, USB_DEVICE_ID_CRYSTALTOUCH_DUAL) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MICROCHIP, USB_DEVICE_ID_PICOLCD) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MICROCHIP, USB_DEVICE_ID_PICOLCD_BOOTLOADER) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MICROSOFT, USB_DEVICE_ID_MS_COMFORT_MOUSE_4500) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MICROSOFT, USB_DEVICE_ID_SIDEWINDER_GV) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MICROSOFT, USB_DEVICE_ID_MS_NE4K) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MICROSOFT, USB_DEVICE_ID_MS_LK6K) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MICROSOFT, USB_DEVICE_ID_MS_PRESENTER_8K_USB) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MICROSOFT, USB_DEVICE_ID_MS_DIGITAL_MEDIA_3K) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MICROSOFT, USB_DEVICE_ID_WIRELESS_OPTICAL_DESKTOP_3_0) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MONTEREY, USB_DEVICE_ID_GENIUS_KB29E) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_NTRIG, USB_DEVICE_ID_NTRIG_TOUCH_SCREEN) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_NTRIG, USB_DEVICE_ID_NTRIG_TOUCH_SCREEN_1) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_NTRIG, USB_DEVICE_ID_NTRIG_TOUCH_SCREEN_2) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_NTRIG, USB_DEVICE_ID_NTRIG_TOUCH_SCREEN_3) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_NTRIG, USB_DEVICE_ID_NTRIG_TOUCH_SCREEN_4) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_NTRIG, USB_DEVICE_ID_NTRIG_TOUCH_SCREEN_5) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_NTRIG, USB_DEVICE_ID_NTRIG_TOUCH_SCREEN_6) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_NTRIG, USB_DEVICE_ID_NTRIG_TOUCH_SCREEN_7) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_NTRIG, USB_DEVICE_ID_NTRIG_TOUCH_SCREEN_8) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_NTRIG, USB_DEVICE_ID_NTRIG_TOUCH_SCREEN_9) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_NTRIG, USB_DEVICE_ID_NTRIG_TOUCH_SCREEN_10) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_NTRIG, USB_DEVICE_ID_NTRIG_TOUCH_SCREEN_11) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_NTRIG, USB_DEVICE_ID_NTRIG_TOUCH_SCREEN_12) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_NTRIG, USB_DEVICE_ID_NTRIG_TOUCH_SCREEN_13) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_NTRIG, USB_DEVICE_ID_NTRIG_TOUCH_SCREEN_14) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_NTRIG, USB_DEVICE_ID_NTRIG_TOUCH_SCREEN_15) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_NTRIG, USB_DEVICE_ID_NTRIG_TOUCH_SCREEN_16) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_NTRIG, USB_DEVICE_ID_NTRIG_TOUCH_SCREEN_17) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_NTRIG, USB_DEVICE_ID_NTRIG_TOUCH_SCREEN_18) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ORTEK, USB_DEVICE_ID_ORTEK_PKB1700) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ORTEK, USB_DEVICE_ID_ORTEK_WKB2000) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_PANASONIC, USB_DEVICE_ID_PANABOARD_UBT780) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_PANASONIC, USB_DEVICE_ID_PANABOARD_UBT880) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_PENMOUNT, USB_DEVICE_ID_PENMOUNT_PCI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_PETALYNX, USB_DEVICE_ID_PETALYNX_MAXTER_REMOTE) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_PIXART, USB_DEVICE_ID_PIXART_OPTICAL_TOUCH_SCREEN) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_PIXART, USB_DEVICE_ID_PIXART_OPTICAL_TOUCH_SCREEN1) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_PIXART, USB_DEVICE_ID_PIXART_OPTICAL_TOUCH_SCREEN2) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_PRIMAX, USB_DEVICE_ID_PRIMAX_KEYBOARD) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_QUANTA, USB_DEVICE_ID_QUANTA_OPTICAL_TOUCH) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_QUANTA, USB_DEVICE_ID_PIXART_IMAGING_INC_OPTICAL_TOUCH_SCREEN) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ROCCAT, USB_DEVICE_ID_ROCCAT_KONE) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ROCCAT, USB_DEVICE_ID_ROCCAT_ARVO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ROCCAT, USB_DEVICE_ID_ROCCAT_ISKU) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ROCCAT, USB_DEVICE_ID_ROCCAT_KONEPLUS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ROCCAT, USB_DEVICE_ID_ROCCAT_KOVAPLUS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ROCCAT, USB_DEVICE_ID_ROCCAT_PYRA_WIRED) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ROCCAT, USB_DEVICE_ID_ROCCAT_PYRA_WIRELESS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_SAITEK, USB_DEVICE_ID_SAITEK_PS1000) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_SAMSUNG, USB_DEVICE_ID_SAMSUNG_IR_REMOTE) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_SAMSUNG, USB_DEVICE_ID_SAMSUNG_WIRELESS_KBD_MOUSE) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_SKYCABLE, USB_DEVICE_ID_SKYCABLE_WIRELESS_PRESENTER) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_SONY, USB_DEVICE_ID_SONY_PS3_CONTROLLER) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_SONY, USB_DEVICE_ID_SONY_NAVIGATION_CONTROLLER) },
	{ HID_BLUETOOTH_DEVICE(USB_VENDOR_ID_SONY, USB_DEVICE_ID_SONY_PS3_CONTROLLER) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_SONY, USB_DEVICE_ID_SONY_VAIO_VGX_MOUSE) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_SONY, USB_DEVICE_ID_SONY_VAIO_VGP_MOUSE) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_STANTUM, USB_DEVICE_ID_MTP) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_STANTUM_STM, USB_DEVICE_ID_MTP_STM) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_STANTUM_SITRONIX, USB_DEVICE_ID_MTP_SITRONIX) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_SUNPLUS, USB_DEVICE_ID_SUNPLUS_WDESKTOP) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_THRUSTMASTER, 0xb300) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_THRUSTMASTER, 0xb304) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_THRUSTMASTER, 0xb323) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_THRUSTMASTER, 0xb324) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_THRUSTMASTER, 0xb651) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_THRUSTMASTER, 0xb653) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_THRUSTMASTER, 0xb654) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_THRUSTMASTER, 0xb65a) },
	{ HID_BLUETOOTH_DEVICE(USB_VENDOR_ID_TIVO, USB_DEVICE_ID_TIVO_SLIDE_BT) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_TIVO, USB_DEVICE_ID_TIVO_SLIDE) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_TOPSEED, USB_DEVICE_ID_TOPSEED_CYBERLINK) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_TOPSEED2, USB_DEVICE_ID_TOPSEED2_RF_COMBO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_TOUCH_INTL, USB_DEVICE_ID_TOUCH_INTL_MULTI_TOUCH) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_TWINHAN, USB_DEVICE_ID_TWINHAN_IR_REMOTE) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_TURBOX, USB_DEVICE_ID_TURBOX_TOUCHSCREEN_MOSART) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_UCLOGIC, USB_DEVICE_ID_UCLOGIC_TABLET_PF1209) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_UCLOGIC, USB_DEVICE_ID_UCLOGIC_TABLET_WP4030U) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_UCLOGIC, USB_DEVICE_ID_UCLOGIC_TABLET_WP5540U) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_UCLOGIC, USB_DEVICE_ID_UCLOGIC_TABLET_WP8060U) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_UCLOGIC, USB_DEVICE_ID_UCLOGIC_TABLET_WP1062) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_UNITEC, USB_DEVICE_ID_UNITEC_USB_TOUCH_0709) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_UNITEC, USB_DEVICE_ID_UNITEC_USB_TOUCH_0A19) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_WISEGROUP, USB_DEVICE_ID_SMARTJOY_PLUS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_WISEGROUP, USB_DEVICE_ID_SUPER_JOY_BOX_3) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_WISEGROUP, USB_DEVICE_ID_DUAL_USB_JOYPAD) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_WISEGROUP_LTD, USB_DEVICE_ID_SUPER_JOY_BOX_3_PRO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_WISEGROUP_LTD, USB_DEVICE_ID_SUPER_DUAL_BOX_PRO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_WISEGROUP_LTD, USB_DEVICE_ID_SUPER_JOY_BOX_5_PRO) },
	{ HID_BLUETOOTH_DEVICE(USB_VENDOR_ID_WACOM, USB_DEVICE_ID_WACOM_GRAPHIRE_BLUETOOTH) },
	{ HID_BLUETOOTH_DEVICE(USB_VENDOR_ID_WACOM, USB_DEVICE_ID_WACOM_INTUOS4_BLUETOOTH) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_WALTOP, USB_DEVICE_ID_WALTOP_SLIM_TABLET_5_8_INCH) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_WALTOP, USB_DEVICE_ID_WALTOP_SLIM_TABLET_12_1_INCH) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_WALTOP, USB_DEVICE_ID_WALTOP_Q_PAD) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_WALTOP, USB_DEVICE_ID_WALTOP_PID_0038) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_WALTOP, USB_DEVICE_ID_WALTOP_MEDIA_TABLET_10_6_INCH) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_WALTOP, USB_DEVICE_ID_WALTOP_MEDIA_TABLET_14_1_INCH) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_XAT, USB_DEVICE_ID_XAT_CSR) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_XIROKU, USB_DEVICE_ID_XIROKU_SPX) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_XIROKU, USB_DEVICE_ID_XIROKU_MPX) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_XIROKU, USB_DEVICE_ID_XIROKU_CSR) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_XIROKU, USB_DEVICE_ID_XIROKU_SPX1) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_XIROKU, USB_DEVICE_ID_XIROKU_MPX1) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_XIROKU, USB_DEVICE_ID_XIROKU_CSR1) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_XIROKU, USB_DEVICE_ID_XIROKU_SPX2) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_XIROKU, USB_DEVICE_ID_XIROKU_MPX2) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_XIROKU, USB_DEVICE_ID_XIROKU_CSR2) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_X_TENSIONS, USB_DEVICE_ID_SPEEDLINK_VAD_CEZANNE) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ZEROPLUS, 0x0005) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ZEROPLUS, 0x0030) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ZYDACRON, USB_DEVICE_ID_ZYDACRON_REMOTE_CONTROL) },

	{ HID_BLUETOOTH_DEVICE(USB_VENDOR_ID_MICROSOFT, USB_DEVICE_ID_MS_PRESENTER_8K_BT) },
	{ HID_BLUETOOTH_DEVICE(USB_VENDOR_ID_NINTENDO, USB_DEVICE_ID_NINTENDO_WIIMOTE) },
	{ HID_BLUETOOTH_DEVICE(USB_VENDOR_ID_NINTENDO, USB_DEVICE_ID_NINTENDO_WIIMOTE2) },
	{ }
};
=======
	hdev->claimed = 0;

	hid_bpf_disconnect_device(hdev);
}
EXPORT_SYMBOL_GPL(hid_disconnect);

/**
 * hid_hw_start - start underlying HW
 * @hdev: hid device
 * @connect_mask: which outputs to connect, see HID_CONNECT_*
 *
 * Call this in probe function *after* hid_parse. This will setup HW
 * buffers and start the device (if not defeirred to device open).
 * hid_hw_stop must be called if this was successful.
 */
int hid_hw_start(struct hid_device *hdev, unsigned int connect_mask)
{
	int error;

	error = hdev->ll_driver->start(hdev);
	if (error)
		return error;

	if (connect_mask) {
		error = hid_connect(hdev, connect_mask);
		if (error) {
			hdev->ll_driver->stop(hdev);
			return error;
		}
	}

	return 0;
}
EXPORT_SYMBOL_GPL(hid_hw_start);

/**
 * hid_hw_stop - stop underlying HW
 * @hdev: hid device
 *
 * This is usually called from remove function or from probe when something
 * failed and hid_hw_start was called already.
 */
void hid_hw_stop(struct hid_device *hdev)
{
	hid_disconnect(hdev);
	hdev->ll_driver->stop(hdev);
}
EXPORT_SYMBOL_GPL(hid_hw_stop);

/**
 * hid_hw_open - signal underlying HW to start delivering events
 * @hdev: hid device
 *
 * Tell underlying HW to start delivering events from the device.
 * This function should be called sometime after successful call
 * to hid_hw_start().
 */
int hid_hw_open(struct hid_device *hdev)
{
	int ret;

	ret = mutex_lock_killable(&hdev->ll_open_lock);
	if (ret)
		return ret;

	if (!hdev->ll_open_count++) {
		ret = hdev->ll_driver->open(hdev);
		if (ret)
			hdev->ll_open_count--;
	}

	mutex_unlock(&hdev->ll_open_lock);
	return ret;
}
EXPORT_SYMBOL_GPL(hid_hw_open);

/**
 * hid_hw_close - signal underlaying HW to stop delivering events
 *
 * @hdev: hid device
 *
 * This function indicates that we are not interested in the events
 * from this device anymore. Delivery of events may or may not stop,
 * depending on the number of users still outstanding.
 */
void hid_hw_close(struct hid_device *hdev)
{
	mutex_lock(&hdev->ll_open_lock);
	if (!--hdev->ll_open_count)
		hdev->ll_driver->close(hdev);
	mutex_unlock(&hdev->ll_open_lock);
}
EXPORT_SYMBOL_GPL(hid_hw_close);

/**
 * hid_hw_request - send report request to device
 *
 * @hdev: hid device
 * @report: report to send
 * @reqtype: hid request type
 */
void hid_hw_request(struct hid_device *hdev,
		    struct hid_report *report, enum hid_class_request reqtype)
{
	if (hdev->ll_driver->request)
		return hdev->ll_driver->request(hdev, report, reqtype);

	__hid_request(hdev, report, reqtype);
}
EXPORT_SYMBOL_GPL(hid_hw_request);

/**
 * hid_hw_raw_request - send report request to device
 *
 * @hdev: hid device
 * @reportnum: report ID
 * @buf: in/out data to transfer
 * @len: length of buf
 * @rtype: HID report type
 * @reqtype: HID_REQ_GET_REPORT or HID_REQ_SET_REPORT
 *
 * Return: count of data transferred, negative if error
 *
 * Same behavior as hid_hw_request, but with raw buffers instead.
 */
int hid_hw_raw_request(struct hid_device *hdev,
		       unsigned char reportnum, __u8 *buf,
		       size_t len, enum hid_report_type rtype, enum hid_class_request reqtype)
{
	unsigned int max_buffer_size = HID_MAX_BUFFER_SIZE;

	if (hdev->ll_driver->max_buffer_size)
		max_buffer_size = hdev->ll_driver->max_buffer_size;

	if (len < 1 || len > max_buffer_size || !buf)
		return -EINVAL;

	return hdev->ll_driver->raw_request(hdev, reportnum, buf, len,
					    rtype, reqtype);
}
EXPORT_SYMBOL_GPL(hid_hw_raw_request);

/**
 * hid_hw_output_report - send output report to device
 *
 * @hdev: hid device
 * @buf: raw data to transfer
 * @len: length of buf
 *
 * Return: count of data transferred, negative if error
 */
int hid_hw_output_report(struct hid_device *hdev, __u8 *buf, size_t len)
{
	unsigned int max_buffer_size = HID_MAX_BUFFER_SIZE;

	if (hdev->ll_driver->max_buffer_size)
		max_buffer_size = hdev->ll_driver->max_buffer_size;

	if (len < 1 || len > max_buffer_size || !buf)
		return -EINVAL;

	if (hdev->ll_driver->output_report)
		return hdev->ll_driver->output_report(hdev, buf, len);

	return -ENOSYS;
}
EXPORT_SYMBOL_GPL(hid_hw_output_report);

#ifdef CONFIG_PM
int hid_driver_suspend(struct hid_device *hdev, pm_message_t state)
{
	if (hdev->driver && hdev->driver->suspend)
		return hdev->driver->suspend(hdev, state);

	return 0;
}
EXPORT_SYMBOL_GPL(hid_driver_suspend);

int hid_driver_reset_resume(struct hid_device *hdev)
{
	if (hdev->driver && hdev->driver->reset_resume)
		return hdev->driver->reset_resume(hdev);

	return 0;
}
EXPORT_SYMBOL_GPL(hid_driver_reset_resume);

int hid_driver_resume(struct hid_device *hdev)
{
	if (hdev->driver && hdev->driver->resume)
		return hdev->driver->resume(hdev);

	return 0;
}
EXPORT_SYMBOL_GPL(hid_driver_resume);
#endif /* CONFIG_PM */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct hid_dynid {
	struct list_head list;
	struct hid_device_id id;
};

/**
<<<<<<< HEAD
 * store_new_id - add a new HID device ID to this driver and re-probe devices
 * @driver: target device driver
=======
 * new_id_store - add a new HID device ID to this driver and re-probe devices
 * @drv: target device driver
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @buf: buffer for scanning device ID data
 * @count: input size
 *
 * Adds a new dynamic hid device ID to this driver,
 * and causes the driver to probe for all devices again.
 */
<<<<<<< HEAD
static ssize_t store_new_id(struct device_driver *drv, const char *buf,
		size_t count)
{
	struct hid_driver *hdrv = container_of(drv, struct hid_driver, driver);
=======
static ssize_t new_id_store(struct device_driver *drv, const char *buf,
		size_t count)
{
	struct hid_driver *hdrv = to_hid_driver(drv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct hid_dynid *dynid;
	__u32 bus, vendor, product;
	unsigned long driver_data = 0;
	int ret;

	ret = sscanf(buf, "%x %x %x %lx",
			&bus, &vendor, &product, &driver_data);
	if (ret < 3)
		return -EINVAL;

	dynid = kzalloc(sizeof(*dynid), GFP_KERNEL);
	if (!dynid)
		return -ENOMEM;

	dynid->id.bus = bus;
<<<<<<< HEAD
=======
	dynid->id.group = HID_GROUP_ANY;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dynid->id.vendor = vendor;
	dynid->id.product = product;
	dynid->id.driver_data = driver_data;

	spin_lock(&hdrv->dyn_lock);
	list_add_tail(&dynid->list, &hdrv->dyn_list);
	spin_unlock(&hdrv->dyn_lock);

	ret = driver_attach(&hdrv->driver);

	return ret ? : count;
}
<<<<<<< HEAD
static DRIVER_ATTR(new_id, S_IWUSR, NULL, store_new_id);
=======
static DRIVER_ATTR_WO(new_id);

static struct attribute *hid_drv_attrs[] = {
	&driver_attr_new_id.attr,
	NULL,
};
ATTRIBUTE_GROUPS(hid_drv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void hid_free_dynids(struct hid_driver *hdrv)
{
	struct hid_dynid *dynid, *n;

	spin_lock(&hdrv->dyn_lock);
	list_for_each_entry_safe(dynid, n, &hdrv->dyn_list, list) {
		list_del(&dynid->list);
		kfree(dynid);
	}
	spin_unlock(&hdrv->dyn_lock);
}

<<<<<<< HEAD
static const struct hid_device_id *hid_match_device(struct hid_device *hdev,
		struct hid_driver *hdrv)
=======
const struct hid_device_id *hid_match_device(struct hid_device *hdev,
					     struct hid_driver *hdrv)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct hid_dynid *dynid;

	spin_lock(&hdrv->dyn_lock);
	list_for_each_entry(dynid, &hdrv->dyn_list, list) {
		if (hid_match_one_id(hdev, &dynid->id)) {
			spin_unlock(&hdrv->dyn_lock);
			return &dynid->id;
		}
	}
	spin_unlock(&hdrv->dyn_lock);

	return hid_match_id(hdev, hdrv->id_table);
}
<<<<<<< HEAD

static int hid_bus_match(struct device *dev, struct device_driver *drv)
{
	struct hid_driver *hdrv = container_of(drv, struct hid_driver, driver);
	struct hid_device *hdev = container_of(dev, struct hid_device, dev);

	if ((hdev->quirks & HID_QUIRK_MULTITOUCH) &&
		!strncmp(hdrv->name, "hid-multitouch", 14))
		return 1;

	if (!hid_match_device(hdev, hdrv))
		return 0;

	/* generic wants all that don't have specialized driver */
	if (!strncmp(hdrv->name, "generic-", 8) && !hid_ignore_special_drivers)
		return !hid_match_id(hdev, hid_have_special_driver);

	return 1;
=======
EXPORT_SYMBOL_GPL(hid_match_device);

static int hid_bus_match(struct device *dev, struct device_driver *drv)
{
	struct hid_driver *hdrv = to_hid_driver(drv);
	struct hid_device *hdev = to_hid_device(dev);

	return hid_match_device(hdev, hdrv) != NULL;
}

/**
 * hid_compare_device_paths - check if both devices share the same path
 * @hdev_a: hid device
 * @hdev_b: hid device
 * @separator: char to use as separator
 *
 * Check if two devices share the same path up to the last occurrence of
 * the separator char. Both paths must exist (i.e., zero-length paths
 * don't match).
 */
bool hid_compare_device_paths(struct hid_device *hdev_a,
			      struct hid_device *hdev_b, char separator)
{
	int n1 = strrchr(hdev_a->phys, separator) - hdev_a->phys;
	int n2 = strrchr(hdev_b->phys, separator) - hdev_b->phys;

	if (n1 != n2 || n1 <= 0 || n2 <= 0)
		return false;

	return !strncmp(hdev_a->phys, hdev_b->phys, n1);
}
EXPORT_SYMBOL_GPL(hid_compare_device_paths);

static bool hid_check_device_match(struct hid_device *hdev,
				   struct hid_driver *hdrv,
				   const struct hid_device_id **id)
{
	*id = hid_match_device(hdev, hdrv);
	if (!*id)
		return false;

	if (hdrv->match)
		return hdrv->match(hdev, hid_ignore_special_drivers);

	/*
	 * hid-generic implements .match(), so we must be dealing with a
	 * different HID driver here, and can simply check if
	 * hid_ignore_special_drivers is set or not.
	 */
	return !hid_ignore_special_drivers;
}

static int __hid_device_probe(struct hid_device *hdev, struct hid_driver *hdrv)
{
	const struct hid_device_id *id;
	int ret;

	if (!hid_check_device_match(hdev, hdrv, &id))
		return -ENODEV;

	hdev->devres_group_id = devres_open_group(&hdev->dev, NULL, GFP_KERNEL);
	if (!hdev->devres_group_id)
		return -ENOMEM;

	/* reset the quirks that has been previously set */
	hdev->quirks = hid_lookup_quirk(hdev);
	hdev->driver = hdrv;

	if (hdrv->probe) {
		ret = hdrv->probe(hdev, id);
	} else { /* default probe */
		ret = hid_open_report(hdev);
		if (!ret)
			ret = hid_hw_start(hdev, HID_CONNECT_DEFAULT);
	}

	/*
	 * Note that we are not closing the devres group opened above so
	 * even resources that were attached to the device after probe is
	 * run are released when hid_device_remove() is executed. This is
	 * needed as some drivers would allocate additional resources,
	 * for example when updating firmware.
	 */

	if (ret) {
		devres_release_group(&hdev->dev, hdev->devres_group_id);
		hid_close_report(hdev);
		hdev->driver = NULL;
	}

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int hid_device_probe(struct device *dev)
{
<<<<<<< HEAD
	struct hid_driver *hdrv = container_of(dev->driver,
			struct hid_driver, driver);
	struct hid_device *hdev = container_of(dev, struct hid_device, dev);
	const struct hid_device_id *id;
	int ret = 0;

	if (down_interruptible(&hdev->driver_lock))
		return -EINTR;

	if (!hdev->driver) {
		id = hid_match_device(hdev, hdrv);
		if (id == NULL) {
			if (!((hdev->quirks & HID_QUIRK_MULTITOUCH) &&
				!strncmp(hdrv->name, "hid-multitouch", 14))) {
				ret = -ENODEV;
				goto unlock;
			}
		}

		hdev->driver = hdrv;
		if (hdrv->probe) {
			ret = hdrv->probe(hdev, id);
		} else { /* default probe */
			ret = hid_parse(hdev);
			if (!ret)
				ret = hid_hw_start(hdev, HID_CONNECT_DEFAULT);
		}
		if (ret)
			hdev->driver = NULL;
	}
unlock:
	up(&hdev->driver_lock);
	return ret;
}

static int hid_device_remove(struct device *dev)
{
	struct hid_device *hdev = container_of(dev, struct hid_device, dev);
	struct hid_driver *hdrv;

	if (down_interruptible(&hdev->driver_lock))
		return -EINTR;
=======
	struct hid_device *hdev = to_hid_device(dev);
	struct hid_driver *hdrv = to_hid_driver(dev->driver);
	int ret = 0;

	if (down_interruptible(&hdev->driver_input_lock))
		return -EINTR;

	hdev->io_started = false;
	clear_bit(ffs(HID_STAT_REPROBED), &hdev->status);

	if (!hdev->driver)
		ret = __hid_device_probe(hdev, hdrv);

	if (!hdev->io_started)
		up(&hdev->driver_input_lock);

	return ret;
}

static void hid_device_remove(struct device *dev)
{
	struct hid_device *hdev = to_hid_device(dev);
	struct hid_driver *hdrv;

	down(&hdev->driver_input_lock);
	hdev->io_started = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	hdrv = hdev->driver;
	if (hdrv) {
		if (hdrv->remove)
			hdrv->remove(hdev);
		else /* default remove */
			hid_hw_stop(hdev);
<<<<<<< HEAD
		hdev->driver = NULL;
	}

	up(&hdev->driver_lock);
	return 0;
}

static int hid_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	struct hid_device *hdev = container_of(dev, struct hid_device, dev);

=======

		/* Release all devres resources allocated by the driver */
		devres_release_group(&hdev->dev, hdev->devres_group_id);

		hid_close_report(hdev);
		hdev->driver = NULL;
	}

	if (!hdev->io_started)
		up(&hdev->driver_input_lock);
}

static ssize_t modalias_show(struct device *dev, struct device_attribute *a,
			     char *buf)
{
	struct hid_device *hdev = container_of(dev, struct hid_device, dev);

	return scnprintf(buf, PAGE_SIZE, "hid:b%04Xg%04Xv%08Xp%08X\n",
			 hdev->bus, hdev->group, hdev->vendor, hdev->product);
}
static DEVICE_ATTR_RO(modalias);

static struct attribute *hid_dev_attrs[] = {
	&dev_attr_modalias.attr,
	NULL,
};
static struct bin_attribute *hid_dev_bin_attrs[] = {
	&dev_bin_attr_report_desc,
	NULL
};
static const struct attribute_group hid_dev_group = {
	.attrs = hid_dev_attrs,
	.bin_attrs = hid_dev_bin_attrs,
};
__ATTRIBUTE_GROUPS(hid_dev);

static int hid_uevent(const struct device *dev, struct kobj_uevent_env *env)
{
	const struct hid_device *hdev = to_hid_device(dev);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (add_uevent_var(env, "HID_ID=%04X:%08X:%08X",
			hdev->bus, hdev->vendor, hdev->product))
		return -ENOMEM;

	if (add_uevent_var(env, "HID_NAME=%s", hdev->name))
		return -ENOMEM;

	if (add_uevent_var(env, "HID_PHYS=%s", hdev->phys))
		return -ENOMEM;

	if (add_uevent_var(env, "HID_UNIQ=%s", hdev->uniq))
		return -ENOMEM;

<<<<<<< HEAD
	if (add_uevent_var(env, "MODALIAS=hid:b%04Xv%08Xp%08X",
			hdev->bus, hdev->vendor, hdev->product))
=======
	if (add_uevent_var(env, "MODALIAS=hid:b%04Xg%04Xv%08Xp%08X",
			   hdev->bus, hdev->group, hdev->vendor, hdev->product))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOMEM;

	return 0;
}

<<<<<<< HEAD
static struct bus_type hid_bus_type = {
	.name		= "hid",
=======
const struct bus_type hid_bus_type = {
	.name		= "hid",
	.dev_groups	= hid_dev_groups,
	.drv_groups	= hid_drv_groups,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.match		= hid_bus_match,
	.probe		= hid_device_probe,
	.remove		= hid_device_remove,
	.uevent		= hid_uevent,
};
<<<<<<< HEAD

/* a list of devices that shouldn't be handled by HID core at all */
static const struct hid_device_id hid_ignore_list[] = {
	{ HID_USB_DEVICE(USB_VENDOR_ID_ACECAD, USB_DEVICE_ID_ACECAD_FLAIR) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ACECAD, USB_DEVICE_ID_ACECAD_302) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ADS_TECH, USB_DEVICE_ID_ADS_TECH_RADIO_SI470X) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_AIPTEK, USB_DEVICE_ID_AIPTEK_01) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_AIPTEK, USB_DEVICE_ID_AIPTEK_10) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_AIPTEK, USB_DEVICE_ID_AIPTEK_20) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_AIPTEK, USB_DEVICE_ID_AIPTEK_21) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_AIPTEK, USB_DEVICE_ID_AIPTEK_22) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_AIPTEK, USB_DEVICE_ID_AIPTEK_23) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_AIPTEK, USB_DEVICE_ID_AIPTEK_24) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_AIRCABLE, USB_DEVICE_ID_AIRCABLE1) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ALCOR, USB_DEVICE_ID_ALCOR_USBRS232) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ASUSTEK, USB_DEVICE_ID_ASUSTEK_LCM)},
	{ HID_USB_DEVICE(USB_VENDOR_ID_ASUSTEK, USB_DEVICE_ID_ASUSTEK_LCM2)},
	{ HID_USB_DEVICE(USB_VENDOR_ID_AVERMEDIA, USB_DEVICE_ID_AVER_FM_MR800) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_BERKSHIRE, USB_DEVICE_ID_BERKSHIRE_PCWD) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CIDC, 0x0103) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CYGNAL, USB_DEVICE_ID_CYGNAL_RADIO_SI470X) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CMEDIA, USB_DEVICE_ID_CM109) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CYPRESS, USB_DEVICE_ID_CYPRESS_HIDCOM) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CYPRESS, USB_DEVICE_ID_CYPRESS_ULTRAMOUSE) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_DEALEXTREAME, USB_DEVICE_ID_DEALEXTREAME_RADIO_SI4701) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_DELORME, USB_DEVICE_ID_DELORME_EARTHMATE) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_DELORME, USB_DEVICE_ID_DELORME_EM_LT20) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_DREAM_CHEEKY, 0x0004) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_DREAM_CHEEKY, 0x000a) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ESSENTIAL_REALITY, USB_DEVICE_ID_ESSENTIAL_REALITY_P5) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ETT, USB_DEVICE_ID_TC5UH) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ETT, USB_DEVICE_ID_TC4UM) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GENERAL_TOUCH, 0x0001) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GENERAL_TOUCH, 0x0002) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GENERAL_TOUCH, 0x0004) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GLAB, USB_DEVICE_ID_4_PHIDGETSERVO_30) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GLAB, USB_DEVICE_ID_1_PHIDGETSERVO_30) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GLAB, USB_DEVICE_ID_0_0_4_IF_KIT) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GLAB, USB_DEVICE_ID_0_16_16_IF_KIT) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GLAB, USB_DEVICE_ID_8_8_8_IF_KIT) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GLAB, USB_DEVICE_ID_0_8_7_IF_KIT) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GLAB, USB_DEVICE_ID_0_8_8_IF_KIT) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GLAB, USB_DEVICE_ID_PHIDGET_MOTORCONTROL) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GOTOP, USB_DEVICE_ID_SUPER_Q2) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GOTOP, USB_DEVICE_ID_GOGOPEN) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GOTOP, USB_DEVICE_ID_PENPOWER) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GRETAGMACBETH, USB_DEVICE_ID_GRETAGMACBETH_HUEY) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GRIFFIN, USB_DEVICE_ID_POWERMATE) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GRIFFIN, USB_DEVICE_ID_SOUNDKNOB) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_90) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_100) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_101) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_103) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_104) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_105) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_106) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_107) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_108) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_200) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_201) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_202) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_203) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_204) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_205) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_206) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_207) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_300) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_301) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_302) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_303) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_304) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_305) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_306) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_307) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_308) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_309) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_400) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_401) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_402) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_403) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_404) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_405) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_500) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_501) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_502) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_503) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_504) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_1000) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_1001) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_1002) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_1003) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_1004) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_1005) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_1006) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_GTCO, USB_DEVICE_ID_GTCO_1007) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_IMATION, USB_DEVICE_ID_DISC_STAKKA) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_KBGEAR, USB_DEVICE_ID_KBGEAR_JAMSTUDIO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_KWORLD, USB_DEVICE_ID_KWORLD_RADIO_FM700) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_KYE, USB_DEVICE_ID_KYE_GPEN_560) },
	{ HID_BLUETOOTH_DEVICE(USB_VENDOR_ID_KYE, 0x0058) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_CASSY) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_CASSY2) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_POCKETCASSY) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_POCKETCASSY2) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_MOBILECASSY) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_MOBILECASSY2) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_MICROCASSYVOLTAGE) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_MICROCASSYCURRENT) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_MICROCASSYTIME) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_MICROCASSYTEMPERATURE) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_MICROCASSYPH) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_JWM) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_DMMP) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_UMIP) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_UMIC) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_UMIB) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_XRAY) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_XRAY2) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_VIDEOCOM) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_MOTOR) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_COM3LAB) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_TELEPORT) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_NETWORKANALYSER) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_POWERCONTROL) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_MACHINETEST) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_MOSTANALYSER) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_MOSTANALYSER2) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_ABSESP) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_AUTODATABUS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_MCT) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_HYBRID) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_LD, USB_DEVICE_ID_LD_HEATCONTROL) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MADCATZ, USB_DEVICE_ID_MADCATZ_BEATPAD) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MASTERKIT, USB_DEVICE_ID_MASTERKIT_MA901RADIO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MCC, USB_DEVICE_ID_MCC_PMD1024LS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MCC, USB_DEVICE_ID_MCC_PMD1208LS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MICROCHIP, USB_DEVICE_ID_PICKIT1) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MICROCHIP, USB_DEVICE_ID_PICKIT2) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_NATIONAL_SEMICONDUCTOR, USB_DEVICE_ID_N_S_HARMONY) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ONTRAK, USB_DEVICE_ID_ONTRAK_ADU100) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ONTRAK, USB_DEVICE_ID_ONTRAK_ADU100 + 20) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ONTRAK, USB_DEVICE_ID_ONTRAK_ADU100 + 30) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ONTRAK, USB_DEVICE_ID_ONTRAK_ADU100 + 100) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ONTRAK, USB_DEVICE_ID_ONTRAK_ADU100 + 108) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ONTRAK, USB_DEVICE_ID_ONTRAK_ADU100 + 118) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ONTRAK, USB_DEVICE_ID_ONTRAK_ADU100 + 200) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ONTRAK, USB_DEVICE_ID_ONTRAK_ADU100 + 300) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ONTRAK, USB_DEVICE_ID_ONTRAK_ADU100 + 400) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_ONTRAK, USB_DEVICE_ID_ONTRAK_ADU100 + 500) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_PANJIT, 0x0001) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_PANJIT, 0x0002) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_PANJIT, 0x0003) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_PANJIT, 0x0004) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_PHILIPS, USB_DEVICE_ID_PHILIPS_IEEE802154_DONGLE) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_POWERCOM, USB_DEVICE_ID_POWERCOM_UPS) },
#if defined(CONFIG_MOUSE_SYNAPTICS_USB) || defined(CONFIG_MOUSE_SYNAPTICS_USB_MODULE)
	{ HID_USB_DEVICE(USB_VENDOR_ID_SYNAPTICS, USB_DEVICE_ID_SYNAPTICS_TP) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_SYNAPTICS, USB_DEVICE_ID_SYNAPTICS_INT_TP) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_SYNAPTICS, USB_DEVICE_ID_SYNAPTICS_CPAD) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_SYNAPTICS, USB_DEVICE_ID_SYNAPTICS_STICK) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_SYNAPTICS, USB_DEVICE_ID_SYNAPTICS_WP) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_SYNAPTICS, USB_DEVICE_ID_SYNAPTICS_COMP_TP) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_SYNAPTICS, USB_DEVICE_ID_SYNAPTICS_WTP) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_SYNAPTICS, USB_DEVICE_ID_SYNAPTICS_DPAD) },
#endif
	{ HID_USB_DEVICE(USB_VENDOR_ID_VERNIER, USB_DEVICE_ID_VERNIER_LABPRO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_VERNIER, USB_DEVICE_ID_VERNIER_GOTEMP) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_VERNIER, USB_DEVICE_ID_VERNIER_SKIP) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_VERNIER, USB_DEVICE_ID_VERNIER_CYCLOPS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_VERNIER, USB_DEVICE_ID_VERNIER_LCSPEC) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_WACOM, HID_ANY_ID) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_WISEGROUP, USB_DEVICE_ID_4_PHIDGETSERVO_20) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_WISEGROUP, USB_DEVICE_ID_1_PHIDGETSERVO_20) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_WISEGROUP, USB_DEVICE_ID_8_8_4_IF_KIT) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_YEALINK, USB_DEVICE_ID_YEALINK_P1K_P4K_B2K) },
	{ }
};

/**
 * hid_mouse_ignore_list - mouse devices which should not be handled by the hid layer
 *
 * There are composite devices for which we want to ignore only a certain
 * interface. This is a list of devices for which only the mouse interface will
 * be ignored. This allows a dedicated driver to take care of the interface.
 */
static const struct hid_device_id hid_mouse_ignore_list[] = {
	/* appletouch driver */
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_FOUNTAIN_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_FOUNTAIN_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_GEYSER_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_GEYSER_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_GEYSER_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_GEYSER3_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_GEYSER3_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_GEYSER3_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_GEYSER4_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_GEYSER4_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_GEYSER4_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_GEYSER4_HF_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_GEYSER4_HF_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_GEYSER4_HF_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING2_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING2_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING2_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING3_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING3_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING3_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING4_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING4_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING4_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING4A_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING4A_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING4A_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING5_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING5_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING5_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING5A_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING5A_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING5A_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING6_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING6_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING6_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING6A_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING6A_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING6A_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING7_ANSI) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING7_ISO) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_WELLSPRING7_JIS) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_FOUNTAIN_TP_ONLY) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_GEYSER1_TP_ONLY) },
	{ }
};

static bool hid_ignore(struct hid_device *hdev)
{
	switch (hdev->vendor) {
	case USB_VENDOR_ID_CODEMERCS:
		/* ignore all Code Mercenaries IOWarrior devices */
		if (hdev->product >= USB_DEVICE_ID_CODEMERCS_IOW_FIRST &&
				hdev->product <= USB_DEVICE_ID_CODEMERCS_IOW_LAST)
			return true;
		break;
	case USB_VENDOR_ID_LOGITECH:
		if (hdev->product >= USB_DEVICE_ID_LOGITECH_HARMONY_FIRST &&
				hdev->product <= USB_DEVICE_ID_LOGITECH_HARMONY_LAST)
			return true;
		/*
		 * The Keene FM transmitter USB device has the same USB ID as
		 * the Logitech AudioHub Speaker, but it should ignore the hid.
		 * Check if the name is that of the Keene device.
		 * For reference: the name of the AudioHub is
		 * "HOLTEK  AudioHub Speaker".
		 */
		if (hdev->product == USB_DEVICE_ID_LOGITECH_AUDIOHUB &&
			!strcmp(hdev->name, "HOLTEK  B-LINK USB Audio  "))
				return true;
		break;
	case USB_VENDOR_ID_SOUNDGRAPH:
		if (hdev->product >= USB_DEVICE_ID_SOUNDGRAPH_IMON_FIRST &&
		    hdev->product <= USB_DEVICE_ID_SOUNDGRAPH_IMON_LAST)
			return true;
		break;
	case USB_VENDOR_ID_HANWANG:
		if (hdev->product >= USB_DEVICE_ID_HANWANG_TABLET_FIRST &&
		    hdev->product <= USB_DEVICE_ID_HANWANG_TABLET_LAST)
			return true;
		break;
	case USB_VENDOR_ID_JESS:
		if (hdev->product == USB_DEVICE_ID_JESS_YUREX &&
				hdev->type == HID_TYPE_USBNONE)
			return true;
	break;
	}

	if (hdev->type == HID_TYPE_USBMOUSE &&
			hid_match_id(hdev, hid_mouse_ignore_list))
		return true;

	return !!hid_match_id(hdev, hid_ignore_list);
}
=======
EXPORT_SYMBOL(hid_bus_type);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int hid_add_device(struct hid_device *hdev)
{
	static atomic_t id = ATOMIC_INIT(0);
	int ret;

	if (WARN_ON(hdev->status & HID_STAT_ADDED))
		return -EBUSY;

<<<<<<< HEAD
	/* we need to kill them here, otherwise they will stay allocated to
	 * wait for coming driver */
	if (!(hdev->quirks & HID_QUIRK_NO_IGNORE)
            && (hid_ignore(hdev) || (hdev->quirks & HID_QUIRK_IGNORE)))
		return -ENODEV;

	/* XXX hack, any other cleaner solution after the driver core
	 * is converted to allow more than 20 bytes as the device name? */
	dev_set_name(&hdev->dev, "%04X:%04X:%04X.%04X", hdev->bus,
		     hdev->vendor, hdev->product, atomic_inc_return(&id));
=======
	hdev->quirks = hid_lookup_quirk(hdev);

	/* we need to kill them here, otherwise they will stay allocated to
	 * wait for coming driver */
	if (hid_ignore(hdev))
		return -ENODEV;

	/*
	 * Check for the mandatory transport channel.
	 */
	 if (!hdev->ll_driver->raw_request) {
		hid_err(hdev, "transport driver missing .raw_request()\n");
		return -EINVAL;
	 }

	/*
	 * Read the device report descriptor once and use as template
	 * for the driver-specific modifications.
	 */
	ret = hdev->ll_driver->parse(hdev);
	if (ret)
		return ret;
	if (!hdev->dev_rdesc)
		return -ENODEV;

	/*
	 * Scan generic devices for group information
	 */
	if (hid_ignore_special_drivers) {
		hdev->group = HID_GROUP_GENERIC;
	} else if (!hdev->group &&
		   !(hdev->quirks & HID_QUIRK_HAVE_SPECIAL_DRIVER)) {
		ret = hid_scan_report(hdev);
		if (ret)
			hid_warn(hdev, "bad device descriptor (%d)\n", ret);
	}

	hdev->id = atomic_inc_return(&id);

	/* XXX hack, any other cleaner solution after the driver core
	 * is converted to allow more than 20 bytes as the device name? */
	dev_set_name(&hdev->dev, "%04X:%04X:%04X.%04X", hdev->bus,
		     hdev->vendor, hdev->product, hdev->id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	hid_debug_register(hdev, dev_name(&hdev->dev));
	ret = device_add(&hdev->dev);
	if (!ret)
		hdev->status |= HID_STAT_ADDED;
	else
		hid_debug_unregister(hdev);

	return ret;
}
EXPORT_SYMBOL_GPL(hid_add_device);

/**
 * hid_allocate_device - allocate new hid device descriptor
 *
 * Allocate and initialize hid device, so that hid_destroy_device might be
 * used to free it.
 *
 * New hid_device pointer is returned on success, otherwise ERR_PTR encoded
 * error value.
 */
struct hid_device *hid_allocate_device(void)
{
	struct hid_device *hdev;
<<<<<<< HEAD
	unsigned int i;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret = -ENOMEM;

	hdev = kzalloc(sizeof(*hdev), GFP_KERNEL);
	if (hdev == NULL)
		return ERR_PTR(ret);

	device_initialize(&hdev->dev);
	hdev->dev.release = hid_device_release;
	hdev->dev.bus = &hid_bus_type;
<<<<<<< HEAD

	hdev->collection = kcalloc(HID_DEFAULT_NUM_COLLECTIONS,
			sizeof(struct hid_collection), GFP_KERNEL);
	if (hdev->collection == NULL)
		goto err;
	hdev->collection_size = HID_DEFAULT_NUM_COLLECTIONS;

	for (i = 0; i < HID_REPORT_TYPES; i++)
		INIT_LIST_HEAD(&hdev->report_enum[i].report_list);

	init_waitqueue_head(&hdev->debug_wait);
	INIT_LIST_HEAD(&hdev->debug_list);
	sema_init(&hdev->driver_lock, 1);

	return hdev;
err:
	put_device(&hdev->dev);
	return ERR_PTR(ret);
=======
	device_enable_async_suspend(&hdev->dev);

	hid_close_report(hdev);

	init_waitqueue_head(&hdev->debug_wait);
	INIT_LIST_HEAD(&hdev->debug_list);
	spin_lock_init(&hdev->debug_list_lock);
	sema_init(&hdev->driver_input_lock, 1);
	mutex_init(&hdev->ll_open_lock);
	kref_init(&hdev->ref);

	hid_bpf_device_init(hdev);

	return hdev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(hid_allocate_device);

static void hid_remove_device(struct hid_device *hdev)
{
	if (hdev->status & HID_STAT_ADDED) {
		device_del(&hdev->dev);
		hid_debug_unregister(hdev);
		hdev->status &= ~HID_STAT_ADDED;
	}
<<<<<<< HEAD
=======
	kfree(hdev->dev_rdesc);
	hdev->dev_rdesc = NULL;
	hdev->dev_rsize = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * hid_destroy_device - free previously allocated device
 *
 * @hdev: hid device
 *
 * If you allocate hid_device through hid_allocate_device, you should ever
 * free by this function.
 */
void hid_destroy_device(struct hid_device *hdev)
{
<<<<<<< HEAD
=======
	hid_bpf_destroy_device(hdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	hid_remove_device(hdev);
	put_device(&hdev->dev);
}
EXPORT_SYMBOL_GPL(hid_destroy_device);

<<<<<<< HEAD
=======

static int __hid_bus_reprobe_drivers(struct device *dev, void *data)
{
	struct hid_driver *hdrv = data;
	struct hid_device *hdev = to_hid_device(dev);

	if (hdev->driver == hdrv &&
	    !hdrv->match(hdev, hid_ignore_special_drivers) &&
	    !test_and_set_bit(ffs(HID_STAT_REPROBED), &hdev->status))
		return device_reprobe(dev);

	return 0;
}

static int __hid_bus_driver_added(struct device_driver *drv, void *data)
{
	struct hid_driver *hdrv = to_hid_driver(drv);

	if (hdrv->match) {
		bus_for_each_dev(&hid_bus_type, NULL, hdrv,
				 __hid_bus_reprobe_drivers);
	}

	return 0;
}

static int __bus_removed_driver(struct device_driver *drv, void *data)
{
	return bus_rescan_devices(&hid_bus_type);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int __hid_register_driver(struct hid_driver *hdrv, struct module *owner,
		const char *mod_name)
{
	int ret;

	hdrv->driver.name = hdrv->name;
	hdrv->driver.bus = &hid_bus_type;
	hdrv->driver.owner = owner;
	hdrv->driver.mod_name = mod_name;

	INIT_LIST_HEAD(&hdrv->dyn_list);
	spin_lock_init(&hdrv->dyn_lock);

	ret = driver_register(&hdrv->driver);
<<<<<<< HEAD
	if (ret)
		return ret;

	ret = driver_create_file(&hdrv->driver, &driver_attr_new_id);
	if (ret)
		driver_unregister(&hdrv->driver);
=======

	if (ret == 0)
		bus_for_each_drv(&hid_bus_type, NULL, NULL,
				 __hid_bus_driver_added);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}
EXPORT_SYMBOL_GPL(__hid_register_driver);

void hid_unregister_driver(struct hid_driver *hdrv)
{
<<<<<<< HEAD
	driver_remove_file(&hdrv->driver, &driver_attr_new_id);
	driver_unregister(&hdrv->driver);
	hid_free_dynids(hdrv);
=======
	driver_unregister(&hdrv->driver);
	hid_free_dynids(hdrv);

	bus_for_each_drv(&hid_bus_type, NULL, hdrv, __bus_removed_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(hid_unregister_driver);

int hid_check_keys_pressed(struct hid_device *hid)
{
	struct hid_input *hidinput;
	int i;

	if (!(hid->claimed & HID_CLAIMED_INPUT))
		return 0;

	list_for_each_entry(hidinput, &hid->inputs, list) {
		for (i = 0; i < BITS_TO_LONGS(KEY_MAX); i++)
			if (hidinput->input->key[i])
				return 1;
	}

	return 0;
}
<<<<<<< HEAD

EXPORT_SYMBOL_GPL(hid_check_keys_pressed);

=======
EXPORT_SYMBOL_GPL(hid_check_keys_pressed);

#ifdef CONFIG_HID_BPF
static struct hid_bpf_ops hid_ops = {
	.hid_get_report = hid_get_report,
	.hid_hw_raw_request = hid_hw_raw_request,
	.owner = THIS_MODULE,
	.bus_type = &hid_bus_type,
};
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int __init hid_init(void)
{
	int ret;

<<<<<<< HEAD
	if (hid_debug)
		pr_warn("hid_debug is now used solely for parser and driver debugging.\n"
			"debugfs is now used for inspecting the device (report descriptor, reports)\n");

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = bus_register(&hid_bus_type);
	if (ret) {
		pr_err("can't register hid bus\n");
		goto err;
	}

<<<<<<< HEAD
=======
#ifdef CONFIG_HID_BPF
	hid_bpf_ops = &hid_ops;
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = hidraw_init();
	if (ret)
		goto err_bus;

	hid_debug_init();

	return 0;
err_bus:
	bus_unregister(&hid_bus_type);
err:
	return ret;
}

static void __exit hid_exit(void)
{
<<<<<<< HEAD
	hid_debug_exit();
	hidraw_exit();
	bus_unregister(&hid_bus_type);
=======
#ifdef CONFIG_HID_BPF
	hid_bpf_ops = NULL;
#endif
	hid_debug_exit();
	hidraw_exit();
	bus_unregister(&hid_bus_type);
	hid_quirks_exit(HID_BUS_ANY);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(hid_init);
module_exit(hid_exit);

MODULE_AUTHOR("Andreas Gal");
MODULE_AUTHOR("Vojtech Pavlik");
MODULE_AUTHOR("Jiri Kosina");
<<<<<<< HEAD
MODULE_LICENSE(DRIVER_LICENSE);

=======
MODULE_LICENSE("GPL");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
