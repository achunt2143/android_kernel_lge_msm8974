<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  ACPI-WMI mapping driver
 *
 *  Copyright (C) 2007-2008 Carlos Corbacho <carlos@strangeworlds.co.uk>
 *
 *  GUID parsing code from ldm.c is:
 *   Copyright (C) 2001,2002 Richard Russon <ldm@flatcap.org>
 *   Copyright (c) 2001-2007 Anton Altaparmakov
 *   Copyright (C) 2001,2002 Jakob Kemi <jakob.kemi@telia.com>
 *
<<<<<<< HEAD
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or (at
 *  your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
=======
 *  WMI bus infrastructure by Andrew Lutomirski and Darren Hart:
 *    Copyright (C) 2015 Andrew Lutomirski
 *    Copyright (C) 2017 VMware, Inc. All Rights Reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/list.h>
#include <linux/acpi.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <acpi/acpi_bus.h>
#include <acpi/acpi_drivers.h>

ACPI_MODULE_NAME("wmi");
=======
#include <linux/acpi.h>
#include <linux/bits.h>
#include <linux/build_bug.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/rwsem.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
#include <linux/types.h>
#include <linux/uuid.h>
#include <linux/wmi.h>
#include <linux/fs.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_AUTHOR("Carlos Corbacho");
MODULE_DESCRIPTION("ACPI-WMI Mapping Driver");
MODULE_LICENSE("GPL");

<<<<<<< HEAD
#define ACPI_WMI_CLASS "wmi"

static DEFINE_MUTEX(wmi_data_lock);
static LIST_HEAD(wmi_block_list);

struct guid_block {
	char guid[16];
=======
static LIST_HEAD(wmi_block_list);

struct guid_block {
	guid_t guid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	union {
		char object_id[2];
		struct {
			unsigned char notify_id;
			unsigned char reserved;
		};
	};
	u8 instance_count;
	u8 flags;
<<<<<<< HEAD
};

struct wmi_block {
	struct list_head list;
	struct guid_block gblock;
	acpi_handle handle;
	wmi_notify_handler handler;
	void *handler_data;
	struct device dev;
=======
} __packed;
static_assert(sizeof(typeof_member(struct guid_block, guid)) == 16);
static_assert(sizeof(struct guid_block) == 20);
static_assert(__alignof__(struct guid_block) == 1);

enum {	/* wmi_block flags */
	WMI_READ_TAKES_NO_ARGS,
	WMI_GUID_DUPLICATED,
	WMI_NO_EVENT_DATA,
};

struct wmi_block {
	struct wmi_device dev;
	struct list_head list;
	struct guid_block gblock;
	struct acpi_device *acpi_device;
	struct rw_semaphore notify_lock;	/* Protects notify callback add/remove */
	wmi_notify_handler handler;
	void *handler_data;
	bool driver_ready;
	unsigned long flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};


/*
 * If the GUID data block is marked as expensive, we must enable and
 * explicitily disable data collection.
 */
<<<<<<< HEAD
#define ACPI_WMI_EXPENSIVE   0x1
#define ACPI_WMI_METHOD      0x2	/* GUID is a method */
#define ACPI_WMI_STRING      0x4	/* GUID takes & returns a string */
#define ACPI_WMI_EVENT       0x8	/* GUID is an event */

static bool debug_event;
module_param(debug_event, bool, 0444);
MODULE_PARM_DESC(debug_event,
		 "Log WMI Events [0/1]");

static bool debug_dump_wdg;
module_param(debug_dump_wdg, bool, 0444);
MODULE_PARM_DESC(debug_dump_wdg,
		 "Dump available WMI interfaces [0/1]");

static int acpi_wmi_remove(struct acpi_device *device, int type);
static int acpi_wmi_add(struct acpi_device *device);
static void acpi_wmi_notify(struct acpi_device *device, u32 event);
=======
#define ACPI_WMI_EXPENSIVE   BIT(0)
#define ACPI_WMI_METHOD      BIT(1)	/* GUID is a method */
#define ACPI_WMI_STRING      BIT(2)	/* GUID takes & returns a string */
#define ACPI_WMI_EVENT       BIT(3)	/* GUID is an event */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static const struct acpi_device_id wmi_device_ids[] = {
	{"PNP0C14", 0},
	{"pnp0c14", 0},
<<<<<<< HEAD
	{"", 0},
};
MODULE_DEVICE_TABLE(acpi, wmi_device_ids);

static struct acpi_driver acpi_wmi_driver = {
	.name = "wmi",
	.class = ACPI_WMI_CLASS,
	.ids = wmi_device_ids,
	.ops = {
		.add = acpi_wmi_add,
		.remove = acpi_wmi_remove,
		.notify = acpi_wmi_notify,
	},
};
=======
	{ }
};
MODULE_DEVICE_TABLE(acpi, wmi_device_ids);

#define dev_to_wblock(__dev)	container_of_const(__dev, struct wmi_block, dev.dev)
#define dev_to_wdev(__dev)	container_of_const(__dev, struct wmi_device, dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * GUID parsing functions
 */

<<<<<<< HEAD
/**
 * wmi_parse_hexbyte - Convert a ASCII hex number to a byte
 * @src:  Pointer to at least 2 characters to convert.
 *
 * Convert a two character ASCII hex string to a number.
 *
 * Return:  0-255  Success, the byte was parsed correctly
 *          -1     Error, an invalid character was supplied
 */
static int wmi_parse_hexbyte(const u8 *src)
{
	int h;
	int value;

	/* high part */
	h = value = hex_to_bin(src[0]);
	if (value < 0)
		return -1;

	/* low part */
	value = hex_to_bin(src[1]);
	if (value >= 0)
		return (h << 4) | value;
	return -1;
}

/**
 * wmi_swap_bytes - Rearrange GUID bytes to match GUID binary
 * @src:   Memory block holding binary GUID (16 bytes)
 * @dest:  Memory block to hold byte swapped binary GUID (16 bytes)
 *
 * Byte swap a binary GUID to match it's real GUID value
 */
static void wmi_swap_bytes(u8 *src, u8 *dest)
{
	int i;

	for (i = 0; i <= 3; i++)
		memcpy(dest + i, src + (3 - i), 1);

	for (i = 0; i <= 1; i++)
		memcpy(dest + 4 + i, src + (5 - i), 1);

	for (i = 0; i <= 1; i++)
		memcpy(dest + 6 + i, src + (7 - i), 1);

	memcpy(dest + 8, src + 8, 8);
}

/**
 * wmi_parse_guid - Convert GUID from ASCII to binary
 * @src:   36 char string of the form fa50ff2b-f2e8-45de-83fa-65417f2f49ba
 * @dest:  Memory block to hold binary GUID (16 bytes)
 *
 * N.B. The GUID need not be NULL terminated.
 *
 * Return:  'true'   @dest contains binary GUID
 *          'false'  @dest contents are undefined
 */
static bool wmi_parse_guid(const u8 *src, u8 *dest)
{
	static const int size[] = { 4, 2, 2, 2, 6 };
	int i, j, v;

	if (src[8]  != '-' || src[13] != '-' ||
		src[18] != '-' || src[23] != '-')
		return false;

	for (j = 0; j < 5; j++, src++) {
		for (i = 0; i < size[j]; i++, src += 2, *dest++ = v) {
			v = wmi_parse_hexbyte(src);
			if (v < 0)
				return false;
		}
	}

	return true;
}

/*
 * Convert a raw GUID to the ACII string representation
 */
static int wmi_gtoa(const char *in, char *out)
{
	int i;

	for (i = 3; i >= 0; i--)
		out += sprintf(out, "%02X", in[i] & 0xFF);

	out += sprintf(out, "-");
	out += sprintf(out, "%02X", in[5] & 0xFF);
	out += sprintf(out, "%02X", in[4] & 0xFF);
	out += sprintf(out, "-");
	out += sprintf(out, "%02X", in[7] & 0xFF);
	out += sprintf(out, "%02X", in[6] & 0xFF);
	out += sprintf(out, "-");
	out += sprintf(out, "%02X", in[8] & 0xFF);
	out += sprintf(out, "%02X", in[9] & 0xFF);
	out += sprintf(out, "-");

	for (i = 10; i <= 15; i++)
		out += sprintf(out, "%02X", in[i] & 0xFF);

	*out = '\0';
	return 0;
}

static bool find_guid(const char *guid_string, struct wmi_block **out)
{
	char tmp[16], guid_input[16];
	struct wmi_block *wblock;
	struct guid_block *block;
	struct list_head *p;

	wmi_parse_guid(guid_string, tmp);
	wmi_swap_bytes(tmp, guid_input);

	list_for_each(p, &wmi_block_list) {
		wblock = list_entry(p, struct wmi_block, list);
		block = &wblock->gblock;

		if (memcmp(block->guid, guid_input, 16) == 0) {
			if (out)
				*out = wblock;
			return 1;
		}
	}
	return 0;
}

static acpi_status wmi_method_enable(struct wmi_block *wblock, int enable)
{
	struct guid_block *block = NULL;
	char method[5];
	struct acpi_object_list input;
	union acpi_object params[1];
=======
static bool guid_parse_and_compare(const char *string, const guid_t *guid)
{
	guid_t guid_input;

	if (guid_parse(string, &guid_input))
		return false;

	return guid_equal(&guid_input, guid);
}

static const void *find_guid_context(struct wmi_block *wblock,
				     struct wmi_driver *wdriver)
{
	const struct wmi_device_id *id;

	id = wdriver->id_table;
	if (!id)
		return NULL;

	while (*id->guid_string) {
		if (guid_parse_and_compare(id->guid_string, &wblock->gblock.guid))
			return id->context;
		id++;
	}
	return NULL;
}

static acpi_status wmi_method_enable(struct wmi_block *wblock, bool enable)
{
	struct guid_block *block;
	char method[5];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	acpi_status status;
	acpi_handle handle;

	block = &wblock->gblock;
<<<<<<< HEAD
	handle = wblock->handle;

	if (!block)
		return AE_NOT_EXIST;

	input.count = 1;
	input.pointer = params;
	params[0].type = ACPI_TYPE_INTEGER;
	params[0].integer.value = enable;

	snprintf(method, 5, "WE%02X", block->notify_id);
	status = acpi_evaluate_object(handle, method, &input, NULL);

	if (status != AE_OK && status != AE_NOT_FOUND)
		return status;
	else
		return AE_OK;
=======
	handle = wblock->acpi_device->handle;

	snprintf(method, 5, "WE%02X", block->notify_id);
	status = acpi_execute_simple_method(handle, method, enable);
	if (status == AE_NOT_FOUND)
		return AE_OK;

	return status;
}

#define WMI_ACPI_METHOD_NAME_SIZE 5

static inline void get_acpi_method_name(const struct wmi_block *wblock,
					const char method,
					char buffer[static WMI_ACPI_METHOD_NAME_SIZE])
{
	static_assert(ARRAY_SIZE(wblock->gblock.object_id) == 2);
	static_assert(WMI_ACPI_METHOD_NAME_SIZE >= 5);

	buffer[0] = 'W';
	buffer[1] = method;
	buffer[2] = wblock->gblock.object_id[0];
	buffer[3] = wblock->gblock.object_id[1];
	buffer[4] = '\0';
}

static inline acpi_object_type get_param_acpi_type(const struct wmi_block *wblock)
{
	if (wblock->gblock.flags & ACPI_WMI_STRING)
		return ACPI_TYPE_STRING;
	else
		return ACPI_TYPE_BUFFER;
}

static acpi_status get_event_data(const struct wmi_block *wblock, struct acpi_buffer *out)
{
	union acpi_object param = {
		.integer = {
			.type = ACPI_TYPE_INTEGER,
			.value = wblock->gblock.notify_id,
		}
	};
	struct acpi_object_list input = {
		.count = 1,
		.pointer = &param,
	};

	return acpi_evaluate_object(wblock->acpi_device->handle, "_WED", &input, out);
}

static int wmidev_match_guid(struct device *dev, const void *data)
{
	struct wmi_block *wblock = dev_to_wblock(dev);
	const guid_t *guid = data;

	/* Legacy GUID-based functions are restricted to only see
	 * a single WMI device for each GUID.
	 */
	if (test_bit(WMI_GUID_DUPLICATED, &wblock->flags))
		return 0;

	if (guid_equal(guid, &wblock->gblock.guid))
		return 1;

	return 0;
}

static int wmidev_match_notify_id(struct device *dev, const void *data)
{
	struct wmi_block *wblock = dev_to_wblock(dev);
	const u32 *notify_id = data;

	/* Legacy GUID-based functions are restricted to only see
	 * a single WMI device for each GUID.
	 */
	if (test_bit(WMI_GUID_DUPLICATED, &wblock->flags))
		return 0;

	if (wblock->gblock.flags & ACPI_WMI_EVENT && wblock->gblock.notify_id == *notify_id)
		return 1;

	return 0;
}

static const struct bus_type wmi_bus_type;

static struct wmi_device *wmi_find_device_by_guid(const char *guid_string)
{
	struct device *dev;
	guid_t guid;
	int ret;

	ret = guid_parse(guid_string, &guid);
	if (ret < 0)
		return ERR_PTR(ret);

	dev = bus_find_device(&wmi_bus_type, NULL, &guid, wmidev_match_guid);
	if (!dev)
		return ERR_PTR(-ENODEV);

	return dev_to_wdev(dev);
}

static struct wmi_device *wmi_find_event_by_notify_id(const u32 notify_id)
{
	struct device *dev;

	dev = bus_find_device(&wmi_bus_type, NULL, &notify_id, wmidev_match_notify_id);
	if (!dev)
		return ERR_PTR(-ENODEV);

	return to_wmi_device(dev);
}

static void wmi_device_put(struct wmi_device *wdev)
{
	put_device(&wdev->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Exported WMI functions
 */
<<<<<<< HEAD
/**
 * wmi_evaluate_method - Evaluate a WMI method
 * @guid_string: 36 char string of the form fa50ff2b-f2e8-45de-83fa-65417f2f49ba
 * @instance: Instance index
 * @method_id: Method ID to call
 * &in: Buffer containing input for the method call
 * &out: Empty buffer to return the method results
 *
 * Call an ACPI-WMI method
 */
acpi_status wmi_evaluate_method(const char *guid_string, u8 instance,
u32 method_id, const struct acpi_buffer *in, struct acpi_buffer *out)
{
	struct guid_block *block = NULL;
	struct wmi_block *wblock = NULL;
	acpi_handle handle;
	acpi_status status;
	struct acpi_object_list input;
	union acpi_object params[3];
	char method[5] = "WM";

	if (!find_guid(guid_string, &wblock))
		return AE_ERROR;

	block = &wblock->gblock;
	handle = wblock->handle;

	if (!(block->flags & ACPI_WMI_METHOD))
		return AE_BAD_DATA;

	if (block->instance_count < instance)
		return AE_BAD_PARAMETER;

	input.count = 2;
	input.pointer = params;
	params[0].type = ACPI_TYPE_INTEGER;
	params[0].integer.value = instance;
	params[1].type = ACPI_TYPE_INTEGER;
	params[1].integer.value = method_id;

	if (in) {
		input.count = 3;

		if (block->flags & ACPI_WMI_STRING) {
			params[2].type = ACPI_TYPE_STRING;
		} else {
			params[2].type = ACPI_TYPE_BUFFER;
		}
		params[2].buffer.length = in->length;
		params[2].buffer.pointer = in->pointer;
	}

	strncat(method, block->object_id, 2);

	status = acpi_evaluate_object(handle, method, &input, out);
=======

/**
 * wmi_instance_count - Get number of WMI object instances
 * @guid_string: 36 char string of the form fa50ff2b-f2e8-45de-83fa-65417f2f49ba
 *
 * Get the number of WMI object instances.
 *
 * Returns: Number of WMI object instances or negative error code.
 */
int wmi_instance_count(const char *guid_string)
{
	struct wmi_device *wdev;
	int ret;

	wdev = wmi_find_device_by_guid(guid_string);
	if (IS_ERR(wdev))
		return PTR_ERR(wdev);

	ret = wmidev_instance_count(wdev);
	wmi_device_put(wdev);

	return ret;
}
EXPORT_SYMBOL_GPL(wmi_instance_count);

/**
 * wmidev_instance_count - Get number of WMI object instances
 * @wdev: A wmi bus device from a driver
 *
 * Get the number of WMI object instances.
 *
 * Returns: Number of WMI object instances.
 */
u8 wmidev_instance_count(struct wmi_device *wdev)
{
	struct wmi_block *wblock = container_of(wdev, struct wmi_block, dev);

	return wblock->gblock.instance_count;
}
EXPORT_SYMBOL_GPL(wmidev_instance_count);

/**
 * wmi_evaluate_method - Evaluate a WMI method (deprecated)
 * @guid_string: 36 char string of the form fa50ff2b-f2e8-45de-83fa-65417f2f49ba
 * @instance: Instance index
 * @method_id: Method ID to call
 * @in: Mandatory buffer containing input for the method call
 * @out: Empty buffer to return the method results
 *
 * Call an ACPI-WMI method, the caller must free @out.
 *
 * Return: acpi_status signaling success or error.
 */
acpi_status wmi_evaluate_method(const char *guid_string, u8 instance, u32 method_id,
				const struct acpi_buffer *in, struct acpi_buffer *out)
{
	struct wmi_device *wdev;
	acpi_status status;

	wdev = wmi_find_device_by_guid(guid_string);
	if (IS_ERR(wdev))
		return AE_ERROR;

	status = wmidev_evaluate_method(wdev, instance, method_id, in, out);

	wmi_device_put(wdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return status;
}
EXPORT_SYMBOL_GPL(wmi_evaluate_method);

/**
<<<<<<< HEAD
 * wmi_query_block - Return contents of a WMI block
 * @guid_string: 36 char string of the form fa50ff2b-f2e8-45de-83fa-65417f2f49ba
 * @instance: Instance index
 * &out: Empty buffer to return the contents of the data block to
 *
 * Return the contents of an ACPI-WMI data block to a buffer
 */
acpi_status wmi_query_block(const char *guid_string, u8 instance,
struct acpi_buffer *out)
{
	struct guid_block *block = NULL;
	struct wmi_block *wblock = NULL;
	acpi_handle handle, wc_handle;
	acpi_status status, wc_status = AE_ERROR;
	struct acpi_object_list input, wc_input;
	union acpi_object wc_params[1], wq_params[1];
	char method[5];
	char wc_method[5] = "WC";

	if (!guid_string || !out)
		return AE_BAD_PARAMETER;

	if (!find_guid(guid_string, &wblock))
		return AE_ERROR;

	block = &wblock->gblock;
	handle = wblock->handle;

	if (block->instance_count < instance)
=======
 * wmidev_evaluate_method - Evaluate a WMI method
 * @wdev: A wmi bus device from a driver
 * @instance: Instance index
 * @method_id: Method ID to call
 * @in: Mandatory buffer containing input for the method call
 * @out: Empty buffer to return the method results
 *
 * Call an ACPI-WMI method, the caller must free @out.
 *
 * Return: acpi_status signaling success or error.
 */
acpi_status wmidev_evaluate_method(struct wmi_device *wdev, u8 instance, u32 method_id,
				   const struct acpi_buffer *in, struct acpi_buffer *out)
{
	struct guid_block *block;
	struct wmi_block *wblock;
	acpi_handle handle;
	struct acpi_object_list input;
	union acpi_object params[3];
	char method[WMI_ACPI_METHOD_NAME_SIZE];

	wblock = container_of(wdev, struct wmi_block, dev);
	block = &wblock->gblock;
	handle = wblock->acpi_device->handle;

	if (!in)
		return AE_BAD_DATA;

	if (!(block->flags & ACPI_WMI_METHOD))
		return AE_BAD_DATA;

	if (block->instance_count <= instance)
		return AE_BAD_PARAMETER;

	input.count = 3;
	input.pointer = params;

	params[0].type = ACPI_TYPE_INTEGER;
	params[0].integer.value = instance;
	params[1].type = ACPI_TYPE_INTEGER;
	params[1].integer.value = method_id;
	params[2].type = get_param_acpi_type(wblock);
	params[2].buffer.length = in->length;
	params[2].buffer.pointer = in->pointer;

	get_acpi_method_name(wblock, 'M', method);

	return acpi_evaluate_object(handle, method, &input, out);
}
EXPORT_SYMBOL_GPL(wmidev_evaluate_method);

static acpi_status __query_block(struct wmi_block *wblock, u8 instance,
				 struct acpi_buffer *out)
{
	struct guid_block *block;
	acpi_handle handle;
	acpi_status status, wc_status = AE_ERROR;
	struct acpi_object_list input;
	union acpi_object wq_params[1];
	char wc_method[WMI_ACPI_METHOD_NAME_SIZE];
	char method[WMI_ACPI_METHOD_NAME_SIZE];

	if (!out)
		return AE_BAD_PARAMETER;

	block = &wblock->gblock;
	handle = wblock->acpi_device->handle;

	if (block->instance_count <= instance)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return AE_BAD_PARAMETER;

	/* Check GUID is a data block */
	if (block->flags & (ACPI_WMI_EVENT | ACPI_WMI_METHOD))
		return AE_ERROR;

	input.count = 1;
	input.pointer = wq_params;
	wq_params[0].type = ACPI_TYPE_INTEGER;
	wq_params[0].integer.value = instance;

<<<<<<< HEAD
=======
	if (instance == 0 && test_bit(WMI_READ_TAKES_NO_ARGS, &wblock->flags))
		input.count = 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * If ACPI_WMI_EXPENSIVE, call the relevant WCxx method first to
	 * enable collection.
	 */
	if (block->flags & ACPI_WMI_EXPENSIVE) {
<<<<<<< HEAD
		wc_input.count = 1;
		wc_input.pointer = wc_params;
		wc_params[0].type = ACPI_TYPE_INTEGER;
		wc_params[0].integer.value = 1;

		strncat(wc_method, block->object_id, 2);
=======
		get_acpi_method_name(wblock, 'C', wc_method);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 * Some GUIDs break the specification by declaring themselves
		 * expensive, but have no corresponding WCxx method. So we
		 * should not fail if this happens.
		 */
<<<<<<< HEAD
		wc_status = acpi_get_handle(handle, wc_method, &wc_handle);
		if (ACPI_SUCCESS(wc_status))
			wc_status = acpi_evaluate_object(handle, wc_method,
				&wc_input, NULL);
	}

	strcpy(method, "WQ");
	strncat(method, block->object_id, 2);

=======
		wc_status = acpi_execute_simple_method(handle, wc_method, 1);
	}

	get_acpi_method_name(wblock, 'Q', method);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	status = acpi_evaluate_object(handle, method, &input, out);

	/*
	 * If ACPI_WMI_EXPENSIVE, call the relevant WCxx method, even if
	 * the WQxx method failed - we should disable collection anyway.
	 */
	if ((block->flags & ACPI_WMI_EXPENSIVE) && ACPI_SUCCESS(wc_status)) {
<<<<<<< HEAD
		wc_params[0].integer.value = 0;
		status = acpi_evaluate_object(handle,
		wc_method, &wc_input, NULL);
=======
		/*
		 * Ignore whether this WCxx call succeeds or not since
		 * the previously executed WQxx method call might have
		 * succeeded, and returning the failing status code
		 * of this call would throw away the result of the WQxx
		 * call, potentially leaking memory.
		 */
		acpi_execute_simple_method(handle, wc_method, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return status;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(wmi_query_block);

/**
 * wmi_set_block - Write to a WMI block
 * @guid_string: 36 char string of the form fa50ff2b-f2e8-45de-83fa-65417f2f49ba
 * @instance: Instance index
 * &in: Buffer containing new values for the data block
 *
 * Write the contents of the input buffer to an ACPI-WMI data block
 */
acpi_status wmi_set_block(const char *guid_string, u8 instance,
const struct acpi_buffer *in)
{
	struct guid_block *block = NULL;
	struct wmi_block *wblock = NULL;
	acpi_handle handle;
	struct acpi_object_list input;
	union acpi_object params[2];
	char method[5] = "WS";

	if (!guid_string || !in)
		return AE_BAD_DATA;

	if (!find_guid(guid_string, &wblock))
		return AE_ERROR;

	block = &wblock->gblock;
	handle = wblock->handle;

	if (block->instance_count < instance)
=======

/**
 * wmi_query_block - Return contents of a WMI block (deprecated)
 * @guid_string: 36 char string of the form fa50ff2b-f2e8-45de-83fa-65417f2f49ba
 * @instance: Instance index
 * @out: Empty buffer to return the contents of the data block to
 *
 * Query a ACPI-WMI block, the caller must free @out.
 *
 * Return: ACPI object containing the content of the WMI block.
 */
acpi_status wmi_query_block(const char *guid_string, u8 instance,
			    struct acpi_buffer *out)
{
	struct wmi_block *wblock;
	struct wmi_device *wdev;
	acpi_status status;

	wdev = wmi_find_device_by_guid(guid_string);
	if (IS_ERR(wdev))
		return AE_ERROR;

	wblock = container_of(wdev, struct wmi_block, dev);
	status = __query_block(wblock, instance, out);

	wmi_device_put(wdev);

	return status;
}
EXPORT_SYMBOL_GPL(wmi_query_block);

/**
 * wmidev_block_query - Return contents of a WMI block
 * @wdev: A wmi bus device from a driver
 * @instance: Instance index
 *
 * Query an ACPI-WMI block, the caller must free the result.
 *
 * Return: ACPI object containing the content of the WMI block.
 */
union acpi_object *wmidev_block_query(struct wmi_device *wdev, u8 instance)
{
	struct acpi_buffer out = { ACPI_ALLOCATE_BUFFER, NULL };
	struct wmi_block *wblock = container_of(wdev, struct wmi_block, dev);

	if (ACPI_FAILURE(__query_block(wblock, instance, &out)))
		return NULL;

	return out.pointer;
}
EXPORT_SYMBOL_GPL(wmidev_block_query);

/**
 * wmi_set_block - Write to a WMI block (deprecated)
 * @guid_string: 36 char string of the form fa50ff2b-f2e8-45de-83fa-65417f2f49ba
 * @instance: Instance index
 * @in: Buffer containing new values for the data block
 *
 * Write the contents of the input buffer to an ACPI-WMI data block.
 *
 * Return: acpi_status signaling success or error.
 */
acpi_status wmi_set_block(const char *guid_string, u8 instance, const struct acpi_buffer *in)
{
	struct wmi_device *wdev;
	acpi_status status;

	wdev = wmi_find_device_by_guid(guid_string);
	if (IS_ERR(wdev))
		return AE_ERROR;

	status =  wmidev_block_set(wdev, instance, in);
	wmi_device_put(wdev);

	return status;
}
EXPORT_SYMBOL_GPL(wmi_set_block);

/**
 * wmidev_block_set - Write to a WMI block
 * @wdev: A wmi bus device from a driver
 * @instance: Instance index
 * @in: Buffer containing new values for the data block
 *
 * Write contents of the input buffer to an ACPI-WMI data block.
 *
 * Return: acpi_status signaling success or error.
 */
acpi_status wmidev_block_set(struct wmi_device *wdev, u8 instance, const struct acpi_buffer *in)
{
	struct wmi_block *wblock = container_of(wdev, struct wmi_block, dev);
	acpi_handle handle = wblock->acpi_device->handle;
	struct guid_block *block = &wblock->gblock;
	char method[WMI_ACPI_METHOD_NAME_SIZE];
	struct acpi_object_list input;
	union acpi_object params[2];

	if (!in)
		return AE_BAD_DATA;

	if (block->instance_count <= instance)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return AE_BAD_PARAMETER;

	/* Check GUID is a data block */
	if (block->flags & (ACPI_WMI_EVENT | ACPI_WMI_METHOD))
		return AE_ERROR;

	input.count = 2;
	input.pointer = params;
	params[0].type = ACPI_TYPE_INTEGER;
	params[0].integer.value = instance;
<<<<<<< HEAD

	if (block->flags & ACPI_WMI_STRING) {
		params[1].type = ACPI_TYPE_STRING;
	} else {
		params[1].type = ACPI_TYPE_BUFFER;
	}
	params[1].buffer.length = in->length;
	params[1].buffer.pointer = in->pointer;

	strncat(method, block->object_id, 2);

	return acpi_evaluate_object(handle, method, &input, NULL);
}
EXPORT_SYMBOL_GPL(wmi_set_block);

static void wmi_dump_wdg(const struct guid_block *g)
{
	char guid_string[37];

	wmi_gtoa(g->guid, guid_string);

	pr_info("%s:\n", guid_string);
	pr_info("\tobject_id: %c%c\n", g->object_id[0], g->object_id[1]);
	pr_info("\tnotify_id: %02X\n", g->notify_id);
	pr_info("\treserved: %02X\n", g->reserved);
	pr_info("\tinstance_count: %d\n", g->instance_count);
	pr_info("\tflags: %#x", g->flags);
	if (g->flags) {
		if (g->flags & ACPI_WMI_EXPENSIVE)
			pr_cont(" ACPI_WMI_EXPENSIVE");
		if (g->flags & ACPI_WMI_METHOD)
			pr_cont(" ACPI_WMI_METHOD");
		if (g->flags & ACPI_WMI_STRING)
			pr_cont(" ACPI_WMI_STRING");
		if (g->flags & ACPI_WMI_EVENT)
			pr_cont(" ACPI_WMI_EVENT");
	}
	pr_cont("\n");

}

static void wmi_notify_debug(u32 value, void *context)
{
	struct acpi_buffer response = { ACPI_ALLOCATE_BUFFER, NULL };
	union acpi_object *obj;
	acpi_status status;

	status = wmi_get_event_data(value, &response);
	if (status != AE_OK) {
		pr_info("bad event status 0x%x\n", status);
		return;
	}

	obj = (union acpi_object *)response.pointer;

	if (!obj)
		return;

	pr_info("DEBUG Event ");
	switch(obj->type) {
	case ACPI_TYPE_BUFFER:
		pr_cont("BUFFER_TYPE - length %d\n", obj->buffer.length);
		break;
	case ACPI_TYPE_STRING:
		pr_cont("STRING_TYPE - %s\n", obj->string.pointer);
		break;
	case ACPI_TYPE_INTEGER:
		pr_cont("INTEGER_TYPE - %llu\n", obj->integer.value);
		break;
	case ACPI_TYPE_PACKAGE:
		pr_cont("PACKAGE_TYPE - %d elements\n", obj->package.count);
		break;
	default:
		pr_cont("object type 0x%X\n", obj->type);
	}
	kfree(obj);
}

/**
 * wmi_install_notify_handler - Register handler for WMI events
=======
	params[1].type = get_param_acpi_type(wblock);
	params[1].buffer.length = in->length;
	params[1].buffer.pointer = in->pointer;

	get_acpi_method_name(wblock, 'S', method);

	return acpi_evaluate_object(handle, method, &input, NULL);
}
EXPORT_SYMBOL_GPL(wmidev_block_set);

/**
 * wmi_install_notify_handler - Register handler for WMI events (deprecated)
 * @guid: 36 char string of the form fa50ff2b-f2e8-45de-83fa-65417f2f49ba
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @handler: Function to handle notifications
 * @data: Data to be returned to handler when event is fired
 *
 * Register a handler for events sent to the ACPI-WMI mapper device.
<<<<<<< HEAD
 */
acpi_status wmi_install_notify_handler(const char *guid,
wmi_notify_handler handler, void *data)
{
	struct wmi_block *block;
	acpi_status status = AE_NOT_EXIST;
	char tmp[16], guid_input[16];
	struct list_head *p;

	if (!guid || !handler)
		return AE_BAD_PARAMETER;

	wmi_parse_guid(guid, tmp);
	wmi_swap_bytes(tmp, guid_input);

	list_for_each(p, &wmi_block_list) {
		acpi_status wmi_status;
		block = list_entry(p, struct wmi_block, list);

		if (memcmp(block->gblock.guid, guid_input, 16) == 0) {
			if (block->handler &&
			    block->handler != wmi_notify_debug)
				return AE_ALREADY_ACQUIRED;

			block->handler = handler;
			block->handler_data = data;

			wmi_status = wmi_method_enable(block, 1);
			if ((wmi_status != AE_OK) ||
			    ((wmi_status == AE_OK) && (status == AE_NOT_EXIST)))
				status = wmi_status;
		}
	}
=======
 *
 * Return: acpi_status signaling success or error.
 */
acpi_status wmi_install_notify_handler(const char *guid,
				       wmi_notify_handler handler,
				       void *data)
{
	struct wmi_block *wblock;
	struct wmi_device *wdev;
	acpi_status status;

	wdev = wmi_find_device_by_guid(guid);
	if (IS_ERR(wdev))
		return AE_ERROR;

	wblock = container_of(wdev, struct wmi_block, dev);

	down_write(&wblock->notify_lock);
	if (wblock->handler) {
		status = AE_ALREADY_ACQUIRED;
	} else {
		wblock->handler = handler;
		wblock->handler_data = data;

		if (ACPI_FAILURE(wmi_method_enable(wblock, true)))
			dev_warn(&wblock->dev.dev, "Failed to enable device\n");

		status = AE_OK;
	}
	up_write(&wblock->notify_lock);

	wmi_device_put(wdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return status;
}
EXPORT_SYMBOL_GPL(wmi_install_notify_handler);

/**
<<<<<<< HEAD
 * wmi_uninstall_notify_handler - Unregister handler for WMI events
 *
 * Unregister handler for events sent to the ACPI-WMI mapper device.
 */
acpi_status wmi_remove_notify_handler(const char *guid)
{
	struct wmi_block *block;
	acpi_status status = AE_NOT_EXIST;
	char tmp[16], guid_input[16];
	struct list_head *p;

	if (!guid)
		return AE_BAD_PARAMETER;

	wmi_parse_guid(guid, tmp);
	wmi_swap_bytes(tmp, guid_input);

	list_for_each(p, &wmi_block_list) {
		acpi_status wmi_status;
		block = list_entry(p, struct wmi_block, list);

		if (memcmp(block->gblock.guid, guid_input, 16) == 0) {
			if (!block->handler ||
			    block->handler == wmi_notify_debug)
				return AE_NULL_ENTRY;

			if (debug_event) {
				block->handler = wmi_notify_debug;
				status = AE_OK;
			} else {
				wmi_status = wmi_method_enable(block, 0);
				block->handler = NULL;
				block->handler_data = NULL;
				if ((wmi_status != AE_OK) ||
				    ((wmi_status == AE_OK) &&
				     (status == AE_NOT_EXIST)))
					status = wmi_status;
			}
		}
	}
=======
 * wmi_remove_notify_handler - Unregister handler for WMI events (deprecated)
 * @guid: 36 char string of the form fa50ff2b-f2e8-45de-83fa-65417f2f49ba
 *
 * Unregister handler for events sent to the ACPI-WMI mapper device.
 *
 * Return: acpi_status signaling success or error.
 */
acpi_status wmi_remove_notify_handler(const char *guid)
{
	struct wmi_block *wblock;
	struct wmi_device *wdev;
	acpi_status status;

	wdev = wmi_find_device_by_guid(guid);
	if (IS_ERR(wdev))
		return AE_ERROR;

	wblock = container_of(wdev, struct wmi_block, dev);

	down_write(&wblock->notify_lock);
	if (!wblock->handler) {
		status = AE_NULL_ENTRY;
	} else {
		if (ACPI_FAILURE(wmi_method_enable(wblock, false)))
			dev_warn(&wblock->dev.dev, "Failed to disable device\n");

		wblock->handler = NULL;
		wblock->handler_data = NULL;

		status = AE_OK;
	}
	up_write(&wblock->notify_lock);

	wmi_device_put(wdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return status;
}
EXPORT_SYMBOL_GPL(wmi_remove_notify_handler);

/**
<<<<<<< HEAD
 * wmi_get_event_data - Get WMI data associated with an event
 *
 * @event: Event to find
 * @out: Buffer to hold event data. out->pointer should be freed with kfree()
 *
 * Returns extra data associated with an event in WMI.
 */
acpi_status wmi_get_event_data(u32 event, struct acpi_buffer *out)
{
	struct acpi_object_list input;
	union acpi_object params[1];
	struct guid_block *gblock;
	struct wmi_block *wblock;
	struct list_head *p;

	input.count = 1;
	input.pointer = params;
	params[0].type = ACPI_TYPE_INTEGER;
	params[0].integer.value = event;

	list_for_each(p, &wmi_block_list) {
		wblock = list_entry(p, struct wmi_block, list);
		gblock = &wblock->gblock;

		if ((gblock->flags & ACPI_WMI_EVENT) &&
			(gblock->notify_id == event))
			return acpi_evaluate_object(wblock->handle, "_WED",
				&input, out);
	}

	return AE_NOT_FOUND;
=======
 * wmi_get_event_data - Get WMI data associated with an event (deprecated)
 *
 * @event: Event to find
 * @out: Buffer to hold event data
 *
 * Get extra data associated with an WMI event, the caller needs to free @out.
 *
 * Return: acpi_status signaling success or error.
 */
acpi_status wmi_get_event_data(u32 event, struct acpi_buffer *out)
{
	struct wmi_block *wblock;
	struct wmi_device *wdev;
	acpi_status status;

	wdev = wmi_find_event_by_notify_id(event);
	if (IS_ERR(wdev))
		return AE_NOT_FOUND;

	wblock = container_of(wdev, struct wmi_block, dev);
	status = get_event_data(wblock, out);

	wmi_device_put(wdev);

	return status;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(wmi_get_event_data);

/**
 * wmi_has_guid - Check if a GUID is available
 * @guid_string: 36 char string of the form fa50ff2b-f2e8-45de-83fa-65417f2f49ba
 *
<<<<<<< HEAD
 * Check if a given GUID is defined by _WDG
 */
bool wmi_has_guid(const char *guid_string)
{
	return find_guid(guid_string, NULL);
}
EXPORT_SYMBOL_GPL(wmi_has_guid);

=======
 * Check if a given GUID is defined by _WDG.
 *
 * Return: True if GUID is available, false otherwise.
 */
bool wmi_has_guid(const char *guid_string)
{
	struct wmi_device *wdev;

	wdev = wmi_find_device_by_guid(guid_string);
	if (IS_ERR(wdev))
		return false;

	wmi_device_put(wdev);

	return true;
}
EXPORT_SYMBOL_GPL(wmi_has_guid);

/**
 * wmi_get_acpi_device_uid() - Get _UID name of ACPI device that defines GUID (deprecated)
 * @guid_string: 36 char string of the form fa50ff2b-f2e8-45de-83fa-65417f2f49ba
 *
 * Find the _UID of ACPI device associated with this WMI GUID.
 *
 * Return: The ACPI _UID field value or NULL if the WMI GUID was not found.
 */
char *wmi_get_acpi_device_uid(const char *guid_string)
{
	struct wmi_block *wblock;
	struct wmi_device *wdev;
	char *uid;

	wdev = wmi_find_device_by_guid(guid_string);
	if (IS_ERR(wdev))
		return NULL;

	wblock = container_of(wdev, struct wmi_block, dev);
	uid = acpi_device_uid(wblock->acpi_device);

	wmi_device_put(wdev);

	return uid;
}
EXPORT_SYMBOL_GPL(wmi_get_acpi_device_uid);

static inline struct wmi_driver *drv_to_wdrv(struct device_driver *drv)
{
	return container_of(drv, struct wmi_driver, driver);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * sysfs interface
 */
static ssize_t modalias_show(struct device *dev, struct device_attribute *attr,
			     char *buf)
{
<<<<<<< HEAD
	char guid_string[37];
	struct wmi_block *wblock;

	wblock = dev_get_drvdata(dev);
	if (!wblock)
		return -ENOMEM;

	wmi_gtoa(wblock->gblock.guid, guid_string);

	return sprintf(buf, "wmi:%s\n", guid_string);
}

static struct device_attribute wmi_dev_attrs[] = {
	__ATTR_RO(modalias),
	__ATTR_NULL
};

static int wmi_dev_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	char guid_string[37];

	struct wmi_block *wblock;

	if (add_uevent_var(env, "MODALIAS="))
		return -ENOMEM;

	wblock = dev_get_drvdata(dev);
	if (!wblock)
		return -ENOMEM;

	wmi_gtoa(wblock->gblock.guid, guid_string);

	strcpy(&env->buf[env->buflen - 1], "wmi:");
	memcpy(&env->buf[env->buflen - 1 + 4], guid_string, 36);
	env->buflen += 40;

	return 0;
}

static void wmi_dev_free(struct device *dev)
{
	struct wmi_block *wmi_block = container_of(dev, struct wmi_block, dev);

	kfree(wmi_block);
}

static struct class wmi_class = {
	.name = "wmi",
	.dev_release = wmi_dev_free,
	.dev_uevent = wmi_dev_uevent,
	.dev_attrs = wmi_dev_attrs,
};

static int wmi_create_device(const struct guid_block *gblock,
			     struct wmi_block *wblock, acpi_handle handle)
{
	char guid_string[37];

	wblock->dev.class = &wmi_class;

	wmi_gtoa(gblock->guid, guid_string);
	dev_set_name(&wblock->dev, guid_string);

	dev_set_drvdata(&wblock->dev, wblock);

	return device_register(&wblock->dev);
}

static void wmi_free_devices(void)
{
	struct wmi_block *wblock, *next;

	/* Delete devices for all the GUIDs */
	list_for_each_entry_safe(wblock, next, &wmi_block_list, list) {
		list_del(&wblock->list);
		if (wblock->dev.class)
			device_unregister(&wblock->dev);
		else
			kfree(wblock);
	}
}

static bool guid_already_parsed(const char *guid_string)
{
	struct wmi_block *wblock;

	list_for_each_entry(wblock, &wmi_block_list, list)
		if (memcmp(wblock->gblock.guid, guid_string, 16) == 0)
			return true;

	return false;
=======
	struct wmi_block *wblock = dev_to_wblock(dev);

	return sysfs_emit(buf, "wmi:%pUL\n", &wblock->gblock.guid);
}
static DEVICE_ATTR_RO(modalias);

static ssize_t guid_show(struct device *dev, struct device_attribute *attr,
			 char *buf)
{
	struct wmi_block *wblock = dev_to_wblock(dev);

	return sysfs_emit(buf, "%pUL\n", &wblock->gblock.guid);
}
static DEVICE_ATTR_RO(guid);

static ssize_t instance_count_show(struct device *dev,
				   struct device_attribute *attr, char *buf)
{
	struct wmi_block *wblock = dev_to_wblock(dev);

	return sysfs_emit(buf, "%d\n", (int)wblock->gblock.instance_count);
}
static DEVICE_ATTR_RO(instance_count);

static ssize_t expensive_show(struct device *dev,
			      struct device_attribute *attr, char *buf)
{
	struct wmi_block *wblock = dev_to_wblock(dev);

	return sysfs_emit(buf, "%d\n",
			  (wblock->gblock.flags & ACPI_WMI_EXPENSIVE) != 0);
}
static DEVICE_ATTR_RO(expensive);

static struct attribute *wmi_attrs[] = {
	&dev_attr_modalias.attr,
	&dev_attr_guid.attr,
	&dev_attr_instance_count.attr,
	&dev_attr_expensive.attr,
	NULL
};
ATTRIBUTE_GROUPS(wmi);

static ssize_t notify_id_show(struct device *dev, struct device_attribute *attr,
			      char *buf)
{
	struct wmi_block *wblock = dev_to_wblock(dev);

	return sysfs_emit(buf, "%02X\n", (unsigned int)wblock->gblock.notify_id);
}
static DEVICE_ATTR_RO(notify_id);

static struct attribute *wmi_event_attrs[] = {
	&dev_attr_notify_id.attr,
	NULL
};
ATTRIBUTE_GROUPS(wmi_event);

static ssize_t object_id_show(struct device *dev, struct device_attribute *attr,
			      char *buf)
{
	struct wmi_block *wblock = dev_to_wblock(dev);

	return sysfs_emit(buf, "%c%c\n", wblock->gblock.object_id[0],
			  wblock->gblock.object_id[1]);
}
static DEVICE_ATTR_RO(object_id);

static ssize_t setable_show(struct device *dev, struct device_attribute *attr,
			    char *buf)
{
	struct wmi_device *wdev = dev_to_wdev(dev);

	return sysfs_emit(buf, "%d\n", (int)wdev->setable);
}
static DEVICE_ATTR_RO(setable);

static struct attribute *wmi_data_attrs[] = {
	&dev_attr_object_id.attr,
	&dev_attr_setable.attr,
	NULL
};
ATTRIBUTE_GROUPS(wmi_data);

static struct attribute *wmi_method_attrs[] = {
	&dev_attr_object_id.attr,
	NULL
};
ATTRIBUTE_GROUPS(wmi_method);

static int wmi_dev_uevent(const struct device *dev, struct kobj_uevent_env *env)
{
	const struct wmi_block *wblock = dev_to_wblock(dev);

	if (add_uevent_var(env, "MODALIAS=wmi:%pUL", &wblock->gblock.guid))
		return -ENOMEM;

	if (add_uevent_var(env, "WMI_GUID=%pUL", &wblock->gblock.guid))
		return -ENOMEM;

	return 0;
}

static void wmi_dev_release(struct device *dev)
{
	struct wmi_block *wblock = dev_to_wblock(dev);

	kfree(wblock);
}

static int wmi_dev_match(struct device *dev, struct device_driver *driver)
{
	struct wmi_driver *wmi_driver = drv_to_wdrv(driver);
	struct wmi_block *wblock = dev_to_wblock(dev);
	const struct wmi_device_id *id = wmi_driver->id_table;

	if (id == NULL)
		return 0;

	while (*id->guid_string) {
		if (guid_parse_and_compare(id->guid_string, &wblock->gblock.guid))
			return 1;

		id++;
	}

	return 0;
}

static int wmi_dev_probe(struct device *dev)
{
	struct wmi_block *wblock = dev_to_wblock(dev);
	struct wmi_driver *wdriver = drv_to_wdrv(dev->driver);
	int ret = 0;

	/* Some older WMI drivers will break if instantiated multiple times,
	 * so they are blocked from probing WMI devices with a duplicated GUID.
	 *
	 * New WMI drivers should support being instantiated multiple times.
	 */
	if (test_bit(WMI_GUID_DUPLICATED, &wblock->flags) && !wdriver->no_singleton) {
		dev_warn(dev, "Legacy driver %s cannot be instantiated multiple times\n",
			 dev->driver->name);

		return -ENODEV;
	}

	if (wdriver->notify) {
		if (test_bit(WMI_NO_EVENT_DATA, &wblock->flags) && !wdriver->no_notify_data)
			return -ENODEV;
	}

	if (ACPI_FAILURE(wmi_method_enable(wblock, true)))
		dev_warn(dev, "failed to enable device -- probing anyway\n");

	if (wdriver->probe) {
		ret = wdriver->probe(dev_to_wdev(dev),
				find_guid_context(wblock, wdriver));
		if (ret) {
			if (ACPI_FAILURE(wmi_method_enable(wblock, false)))
				dev_warn(dev, "Failed to disable device\n");

			return ret;
		}
	}

	down_write(&wblock->notify_lock);
	wblock->driver_ready = true;
	up_write(&wblock->notify_lock);

	return 0;
}

static void wmi_dev_remove(struct device *dev)
{
	struct wmi_block *wblock = dev_to_wblock(dev);
	struct wmi_driver *wdriver = drv_to_wdrv(dev->driver);

	down_write(&wblock->notify_lock);
	wblock->driver_ready = false;
	up_write(&wblock->notify_lock);

	if (wdriver->remove)
		wdriver->remove(dev_to_wdev(dev));

	if (ACPI_FAILURE(wmi_method_enable(wblock, false)))
		dev_warn(dev, "failed to disable device\n");
}

static struct class wmi_bus_class = {
	.name = "wmi_bus",
};

static const struct bus_type wmi_bus_type = {
	.name = "wmi",
	.dev_groups = wmi_groups,
	.match = wmi_dev_match,
	.uevent = wmi_dev_uevent,
	.probe = wmi_dev_probe,
	.remove = wmi_dev_remove,
};

static const struct device_type wmi_type_event = {
	.name = "event",
	.groups = wmi_event_groups,
	.release = wmi_dev_release,
};

static const struct device_type wmi_type_method = {
	.name = "method",
	.groups = wmi_method_groups,
	.release = wmi_dev_release,
};

static const struct device_type wmi_type_data = {
	.name = "data",
	.groups = wmi_data_groups,
	.release = wmi_dev_release,
};

/*
 * _WDG is a static list that is only parsed at startup,
 * so it's safe to count entries without extra protection.
 */
static int guid_count(const guid_t *guid)
{
	struct wmi_block *wblock;
	int count = 0;

	list_for_each_entry(wblock, &wmi_block_list, list) {
		if (guid_equal(&wblock->gblock.guid, guid))
			count++;
	}

	return count;
}

static int wmi_create_device(struct device *wmi_bus_dev,
			     struct wmi_block *wblock,
			     struct acpi_device *device)
{
	char method[WMI_ACPI_METHOD_NAME_SIZE];
	struct acpi_device_info *info;
	acpi_handle method_handle;
	acpi_status status;
	uint count;

	if (wblock->gblock.flags & ACPI_WMI_EVENT) {
		wblock->dev.dev.type = &wmi_type_event;
		goto out_init;
	}

	if (wblock->gblock.flags & ACPI_WMI_METHOD) {
		get_acpi_method_name(wblock, 'M', method);
		if (!acpi_has_method(device->handle, method)) {
			dev_warn(wmi_bus_dev,
				 FW_BUG "%s method block execution control method not found\n",
				 method);

			return -ENXIO;
		}

		wblock->dev.dev.type = &wmi_type_method;
		goto out_init;
	}

	/*
	 * Data Block Query Control Method (WQxx by convention) is
	 * required per the WMI documentation. If it is not present,
	 * we ignore this data block.
	 */
	get_acpi_method_name(wblock, 'Q', method);
	status = acpi_get_handle(device->handle, method, &method_handle);
	if (ACPI_FAILURE(status)) {
		dev_warn(wmi_bus_dev,
			 FW_BUG "%s data block query control method not found\n",
			 method);

		return -ENXIO;
	}

	status = acpi_get_object_info(method_handle, &info);
	if (ACPI_FAILURE(status))
		return -EIO;

	wblock->dev.dev.type = &wmi_type_data;

	/*
	 * The Microsoft documentation specifically states:
	 *
	 *   Data blocks registered with only a single instance
	 *   can ignore the parameter.
	 *
	 * ACPICA will get mad at us if we call the method with the wrong number
	 * of arguments, so check what our method expects.  (On some Dell
	 * laptops, WQxx may not be a method at all.)
	 */
	if (info->type != ACPI_TYPE_METHOD || info->param_count == 0)
		set_bit(WMI_READ_TAKES_NO_ARGS, &wblock->flags);

	kfree(info);

	get_acpi_method_name(wblock, 'S', method);
	if (acpi_has_method(device->handle, method))
		wblock->dev.setable = true;

 out_init:
	init_rwsem(&wblock->notify_lock);
	wblock->driver_ready = false;
	wblock->dev.dev.bus = &wmi_bus_type;
	wblock->dev.dev.parent = wmi_bus_dev;

	count = guid_count(&wblock->gblock.guid);
	if (count) {
		dev_set_name(&wblock->dev.dev, "%pUL-%d", &wblock->gblock.guid, count);
		set_bit(WMI_GUID_DUPLICATED, &wblock->flags);
	} else {
		dev_set_name(&wblock->dev.dev, "%pUL", &wblock->gblock.guid);
	}

	device_initialize(&wblock->dev.dev);

	return 0;
}

static int wmi_add_device(struct platform_device *pdev, struct wmi_device *wdev)
{
	struct device_link *link;

	/*
	 * Many aggregate WMI drivers do not use -EPROBE_DEFER when they
	 * are unable to find a WMI device during probe, instead they require
	 * all WMI devices associated with an platform device to become available
	 * at once. This device link thus prevents WMI drivers from probing until
	 * the associated platform device has finished probing (and has registered
	 * all discovered WMI devices).
	 */

	link = device_link_add(&wdev->dev, &pdev->dev, DL_FLAG_AUTOREMOVE_SUPPLIER);
	if (!link)
		return -EINVAL;

	return device_add(&wdev->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Parse the _WDG method for the GUID data blocks
 */
<<<<<<< HEAD
static acpi_status parse_wdg(acpi_handle handle)
{
	struct acpi_buffer out = {ACPI_ALLOCATE_BUFFER, NULL};
	union acpi_object *obj;
	const struct guid_block *gblock;
	struct wmi_block *wblock;
	acpi_status status;
	int retval;
	u32 i, total;

	status = acpi_evaluate_object(handle, "_WDG", NULL, &out);
	if (ACPI_FAILURE(status))
		return -ENXIO;

	obj = (union acpi_object *) out.pointer;
=======
static int parse_wdg(struct device *wmi_bus_dev, struct platform_device *pdev)
{
	struct acpi_device *device = ACPI_COMPANION(&pdev->dev);
	struct acpi_buffer out = {ACPI_ALLOCATE_BUFFER, NULL};
	const struct guid_block *gblock;
	bool event_data_available;
	struct wmi_block *wblock;
	union acpi_object *obj;
	acpi_status status;
	u32 i, total;
	int retval;

	status = acpi_evaluate_object(device->handle, "_WDG", NULL, &out);
	if (ACPI_FAILURE(status))
		return -ENXIO;

	obj = out.pointer;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!obj)
		return -ENXIO;

	if (obj->type != ACPI_TYPE_BUFFER) {
<<<<<<< HEAD
		retval = -ENXIO;
		goto out_free_pointer;
	}

=======
		kfree(obj);
		return -ENXIO;
	}

	event_data_available = acpi_has_method(device->handle, "_WED");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	gblock = (const struct guid_block *)obj->buffer.pointer;
	total = obj->buffer.length / sizeof(struct guid_block);

	for (i = 0; i < total; i++) {
<<<<<<< HEAD
		if (debug_dump_wdg)
			wmi_dump_wdg(&gblock[i]);

		wblock = kzalloc(sizeof(struct wmi_block), GFP_KERNEL);
		if (!wblock)
			return AE_NO_MEMORY;

		wblock->handle = handle;
		wblock->gblock = gblock[i];

		/*
		  Some WMI devices, like those for nVidia hooks, have a
		  duplicate GUID. It's not clear what we should do in this
		  case yet, so for now, we'll just ignore the duplicate
		  for device creation.
		*/
		if (!guid_already_parsed(gblock[i].guid)) {
			retval = wmi_create_device(&gblock[i], wblock, handle);
			if (retval) {
				wmi_free_devices();
				goto out_free_pointer;
			}
=======
		if (!gblock[i].instance_count) {
			dev_info(wmi_bus_dev, FW_INFO "%pUL has zero instances\n", &gblock[i].guid);
			continue;
		}

		wblock = kzalloc(sizeof(*wblock), GFP_KERNEL);
		if (!wblock)
			continue;

		wblock->acpi_device = device;
		wblock->gblock = gblock[i];
		if (gblock[i].flags & ACPI_WMI_EVENT && !event_data_available)
			set_bit(WMI_NO_EVENT_DATA, &wblock->flags);

		retval = wmi_create_device(wmi_bus_dev, wblock, device);
		if (retval) {
			kfree(wblock);
			continue;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		list_add_tail(&wblock->list, &wmi_block_list);

<<<<<<< HEAD
		if (debug_event) {
			wblock->handler = wmi_notify_debug;
			wmi_method_enable(wblock, 1);
		}
	}

	retval = 0;

out_free_pointer:
	kfree(out.pointer);

	return retval;
=======
		retval = wmi_add_device(pdev, &wblock->dev);
		if (retval) {
			dev_err(wmi_bus_dev, "failed to register %pUL\n",
				&wblock->gblock.guid);

			list_del(&wblock->list);
			put_device(&wblock->dev.dev);
		}
	}

	kfree(obj);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * WMI can have EmbeddedControl access regions. In which case, we just want to
 * hand these off to the EC driver.
 */
static acpi_status
acpi_wmi_ec_space_handler(u32 function, acpi_physical_address address,
<<<<<<< HEAD
		      u32 bits, u64 *value,
		      void *handler_context, void *region_context)
{
	int result = 0, i = 0;
=======
			  u32 bits, u64 *value,
			  void *handler_context, void *region_context)
{
	int result = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u8 temp = 0;

	if ((address > 0xFF) || !value)
		return AE_BAD_PARAMETER;

	if (function != ACPI_READ && function != ACPI_WRITE)
		return AE_BAD_PARAMETER;

	if (bits != 8)
		return AE_BAD_PARAMETER;

	if (function == ACPI_READ) {
		result = ec_read(address, &temp);
<<<<<<< HEAD
		(*value) |= ((u64)temp) << i;
	} else {
		temp = 0xff & ((*value) >> i);
=======
		*value = temp;
	} else {
		temp = 0xff & *value;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		result = ec_write(address, temp);
	}

	switch (result) {
	case -EINVAL:
		return AE_BAD_PARAMETER;
<<<<<<< HEAD
		break;
	case -ENODEV:
		return AE_NOT_FOUND;
		break;
	case -ETIME:
		return AE_TIME;
		break;
=======
	case -ENODEV:
		return AE_NOT_FOUND;
	case -ETIME:
		return AE_TIME;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		return AE_OK;
	}
}

<<<<<<< HEAD
static void acpi_wmi_notify(struct acpi_device *device, u32 event)
{
	struct guid_block *block;
	struct wmi_block *wblock;
	struct list_head *p;
	char guid_string[37];

	list_for_each(p, &wmi_block_list) {
		wblock = list_entry(p, struct wmi_block, list);
		block = &wblock->gblock;

		if ((block->flags & ACPI_WMI_EVENT) &&
			(block->notify_id == event)) {
			if (wblock->handler)
				wblock->handler(event, wblock->handler_data);
			if (debug_event) {
				wmi_gtoa(wblock->gblock.guid, guid_string);
				pr_info("DEBUG Event GUID: %s\n", guid_string);
			}

			acpi_bus_generate_netlink_event(
				device->pnp.device_class, dev_name(&device->dev),
				event, 0);
			break;
		}
	}
}

static int acpi_wmi_remove(struct acpi_device *device, int type)
{
	acpi_remove_address_space_handler(device->handle,
				ACPI_ADR_SPACE_EC, &acpi_wmi_ec_space_handler);
	wmi_free_devices();
=======
static int wmi_get_notify_data(struct wmi_block *wblock, union acpi_object **obj)
{
	struct acpi_buffer data = { ACPI_ALLOCATE_BUFFER, NULL };
	acpi_status status;

	if (test_bit(WMI_NO_EVENT_DATA, &wblock->flags)) {
		*obj = NULL;
		return 0;
	}

	status = get_event_data(wblock, &data);
	if (ACPI_FAILURE(status)) {
		dev_warn(&wblock->dev.dev, "Failed to get event data\n");
		return -EIO;
	}

	*obj = data.pointer;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int acpi_wmi_add(struct acpi_device *device)
{
	acpi_status status;
	int error;

	status = acpi_install_address_space_handler(device->handle,
=======
static void wmi_notify_driver(struct wmi_block *wblock, union acpi_object *obj)
{
	struct wmi_driver *driver = drv_to_wdrv(wblock->dev.dev.driver);

	if (!obj && !driver->no_notify_data) {
		dev_warn(&wblock->dev.dev, "Event contains no event data\n");
		return;
	}

	if (driver->notify)
		driver->notify(&wblock->dev, obj);
}

static int wmi_notify_device(struct device *dev, void *data)
{
	struct wmi_block *wblock = dev_to_wblock(dev);
	union acpi_object *obj;
	u32 *event = data;
	int ret;

	if (!(wblock->gblock.flags & ACPI_WMI_EVENT && wblock->gblock.notify_id == *event))
		return 0;

	down_read(&wblock->notify_lock);
	/* The WMI driver notify handler conflicts with the legacy WMI handler.
	 * Because of this the WMI driver notify handler takes precedence.
	 */
	if (wblock->dev.dev.driver && wblock->driver_ready) {
		ret = wmi_get_notify_data(wblock, &obj);
		if (ret >= 0) {
			wmi_notify_driver(wblock, obj);
			kfree(obj);
		}
	} else {
		if (wblock->handler) {
			wblock->handler(*event, wblock->handler_data);
		} else {
			/* The ACPI WMI specification says that _WED should be
			 * evaluated every time an notification is received, even
			 * if no consumers are present.
			 *
			 * Some firmware implementations actually depend on this
			 * by using a queue for events which will fill up if the
			 * WMI driver core stops evaluating _WED due to missing
			 * WMI event consumers.
			 *
			 * Because of this we need this seemingly useless call to
			 * wmi_get_notify_data() which in turn evaluates _WED.
			 */
			ret = wmi_get_notify_data(wblock, &obj);
			if (ret >= 0)
				kfree(obj);
		}

	}
	up_read(&wblock->notify_lock);

	acpi_bus_generate_netlink_event("wmi", acpi_dev_name(wblock->acpi_device), *event, 0);

	return -EBUSY;
}

static void acpi_wmi_notify_handler(acpi_handle handle, u32 event, void *context)
{
	struct device *wmi_bus_dev = context;

	device_for_each_child(wmi_bus_dev, &event, wmi_notify_device);
}

static int wmi_remove_device(struct device *dev, void *data)
{
	struct wmi_block *wblock = dev_to_wblock(dev);

	list_del(&wblock->list);
	device_unregister(dev);

	return 0;
}

static void acpi_wmi_remove(struct platform_device *device)
{
	struct device *wmi_bus_device = dev_get_drvdata(&device->dev);

	device_for_each_child_reverse(wmi_bus_device, NULL, wmi_remove_device);
}

static void acpi_wmi_remove_notify_handler(void *data)
{
	struct acpi_device *acpi_device = data;

	acpi_remove_notify_handler(acpi_device->handle, ACPI_ALL_NOTIFY, acpi_wmi_notify_handler);
}

static void acpi_wmi_remove_address_space_handler(void *data)
{
	struct acpi_device *acpi_device = data;

	acpi_remove_address_space_handler(acpi_device->handle, ACPI_ADR_SPACE_EC,
					  &acpi_wmi_ec_space_handler);
}

static void acpi_wmi_remove_bus_device(void *data)
{
	struct device *wmi_bus_dev = data;

	device_unregister(wmi_bus_dev);
}

static int acpi_wmi_probe(struct platform_device *device)
{
	struct acpi_device *acpi_device;
	struct device *wmi_bus_dev;
	acpi_status status;
	int error;

	acpi_device = ACPI_COMPANION(&device->dev);
	if (!acpi_device) {
		dev_err(&device->dev, "ACPI companion is missing\n");
		return -ENODEV;
	}

	wmi_bus_dev = device_create(&wmi_bus_class, &device->dev, MKDEV(0, 0), NULL, "wmi_bus-%s",
				    dev_name(&device->dev));
	if (IS_ERR(wmi_bus_dev))
		return PTR_ERR(wmi_bus_dev);

	error = devm_add_action_or_reset(&device->dev, acpi_wmi_remove_bus_device, wmi_bus_dev);
	if (error < 0)
		return error;

	dev_set_drvdata(&device->dev, wmi_bus_dev);

	status = acpi_install_address_space_handler(acpi_device->handle,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
						    ACPI_ADR_SPACE_EC,
						    &acpi_wmi_ec_space_handler,
						    NULL, NULL);
	if (ACPI_FAILURE(status)) {
<<<<<<< HEAD
		pr_err("Error installing EC region handler\n");
		return -ENODEV;
	}

	error = parse_wdg(device->handle);
	if (error) {
		acpi_remove_address_space_handler(device->handle,
						  ACPI_ADR_SPACE_EC,
						  &acpi_wmi_ec_space_handler);
		pr_err("Failed to parse WDG method\n");
=======
		dev_err(&device->dev, "Error installing EC region handler\n");
		return -ENODEV;
	}
	error = devm_add_action_or_reset(&device->dev, acpi_wmi_remove_address_space_handler,
					 acpi_device);
	if (error < 0)
		return error;

	status = acpi_install_notify_handler(acpi_device->handle, ACPI_ALL_NOTIFY,
					     acpi_wmi_notify_handler, wmi_bus_dev);
	if (ACPI_FAILURE(status)) {
		dev_err(&device->dev, "Error installing notify handler\n");
		return -ENODEV;
	}
	error = devm_add_action_or_reset(&device->dev, acpi_wmi_remove_notify_handler,
					 acpi_device);
	if (error < 0)
		return error;

	error = parse_wdg(wmi_bus_dev, device);
	if (error) {
		dev_err(&device->dev, "Failed to parse _WDG method\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return error;
	}

	return 0;
}

<<<<<<< HEAD
=======
int __must_check __wmi_driver_register(struct wmi_driver *driver,
				       struct module *owner)
{
	driver->driver.owner = owner;
	driver->driver.bus = &wmi_bus_type;

	return driver_register(&driver->driver);
}
EXPORT_SYMBOL(__wmi_driver_register);

/**
 * wmi_driver_unregister() - Unregister a WMI driver
 * @driver: WMI driver to unregister
 *
 * Unregisters a WMI driver from the WMI bus.
 */
void wmi_driver_unregister(struct wmi_driver *driver)
{
	driver_unregister(&driver->driver);
}
EXPORT_SYMBOL(wmi_driver_unregister);

static struct platform_driver acpi_wmi_driver = {
	.driver = {
		.name = "acpi-wmi",
		.acpi_match_table = wmi_device_ids,
	},
	.probe = acpi_wmi_probe,
	.remove_new = acpi_wmi_remove,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int __init acpi_wmi_init(void)
{
	int error;

	if (acpi_disabled)
		return -ENODEV;

<<<<<<< HEAD
	error = class_register(&wmi_class);
	if (error)
		return error;

	error = acpi_bus_register_driver(&acpi_wmi_driver);
	if (error) {
		pr_err("Error loading mapper\n");
		class_unregister(&wmi_class);
		return error;
	}

	pr_info("Mapper loaded\n");
	return 0;
=======
	error = class_register(&wmi_bus_class);
	if (error)
		return error;

	error = bus_register(&wmi_bus_type);
	if (error)
		goto err_unreg_class;

	error = platform_driver_register(&acpi_wmi_driver);
	if (error) {
		pr_err("Error loading mapper\n");
		goto err_unreg_bus;
	}

	return 0;

err_unreg_bus:
	bus_unregister(&wmi_bus_type);

err_unreg_class:
	class_unregister(&wmi_bus_class);

	return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __exit acpi_wmi_exit(void)
{
<<<<<<< HEAD
	acpi_bus_unregister_driver(&acpi_wmi_driver);
	class_unregister(&wmi_class);

	pr_info("Mapper unloaded\n");
}

subsys_initcall(acpi_wmi_init);
=======
	platform_driver_unregister(&acpi_wmi_driver);
	bus_unregister(&wmi_bus_type);
	class_unregister(&wmi_bus_class);
}

subsys_initcall_sync(acpi_wmi_init);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
module_exit(acpi_wmi_exit);
