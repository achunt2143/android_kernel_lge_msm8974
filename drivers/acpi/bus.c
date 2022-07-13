<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  acpi_bus.c - ACPI Bus Driver ($Revision: 80 $)
 *
 *  Copyright (C) 2001, 2002 Paul Diefenbaugh <paul.s.diefenbaugh@intel.com>
<<<<<<< HEAD
 *
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
 */

=======
 */

#define pr_fmt(fmt) "ACPI: " fmt

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/sched.h>
#include <linux/pm.h>
#include <linux/device.h>
#include <linux/proc_fs.h>
#include <linux/acpi.h>
#include <linux/slab.h>
#include <linux/regulator/machine.h>
<<<<<<< HEAD
#ifdef CONFIG_X86
#include <asm/mpspec.h>
#endif
#include <linux/pci.h>
#include <acpi/acpi_bus.h>
#include <acpi/acpi_drivers.h>
#include <acpi/apei.h>
#include <linux/dmi.h>
#include <linux/suspend.h>

#include "internal.h"

#define _COMPONENT		ACPI_BUS_COMPONENT
ACPI_MODULE_NAME("bus");

=======
#include <linux/workqueue.h>
#include <linux/reboot.h>
#include <linux/delay.h>
#ifdef CONFIG_X86
#include <asm/mpspec.h>
#include <linux/dmi.h>
#endif
#include <linux/acpi_viot.h>
#include <linux/pci.h>
#include <acpi/apei.h>
#include <linux/suspend.h>
#include <linux/prmt.h>

#include "internal.h"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct acpi_device *acpi_root;
struct proc_dir_entry *acpi_root_dir;
EXPORT_SYMBOL(acpi_root_dir);

<<<<<<< HEAD
#define STRUCT_TO_INT(s)	(*((int*)&s))


=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_X86
#ifdef CONFIG_ACPI_CUSTOM_DSDT
static inline int set_copy_dsdt(const struct dmi_system_id *id)
{
	return 0;
}
#else
static int set_copy_dsdt(const struct dmi_system_id *id)
{
<<<<<<< HEAD
	printk(KERN_NOTICE "%s detected - "
		"force copy of DSDT to local memory\n", id->ident);
=======
	pr_notice("%s detected - force copy of DSDT to local memory\n", id->ident);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	acpi_gbl_copy_dsdt_locally = 1;
	return 0;
}
#endif

<<<<<<< HEAD
static struct dmi_system_id dsdt_dmi_table[] __initdata = {
=======
static const struct dmi_system_id dsdt_dmi_table[] __initconst = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Invoke DSDT corruption work-around on all Toshiba Satellite.
	 * https://bugzilla.kernel.org/show_bug.cgi?id=14679
	 */
	{
	 .callback = set_copy_dsdt,
	 .ident = "TOSHIBA Satellite",
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "TOSHIBA"),
		DMI_MATCH(DMI_PRODUCT_NAME, "Satellite"),
		},
	},
	{}
};
<<<<<<< HEAD
#else
static struct dmi_system_id dsdt_dmi_table[] __initdata = {
	{}
};
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

/* --------------------------------------------------------------------------
                                Device Management
   -------------------------------------------------------------------------- */

<<<<<<< HEAD
int acpi_bus_get_device(acpi_handle handle, struct acpi_device **device)
{
	acpi_status status = AE_OK;


	if (!device)
		return -EINVAL;

	/* TBD: Support fixed-feature devices */

	status = acpi_get_data(handle, acpi_bus_data_handler, (void **)device);
	if (ACPI_FAILURE(status) || !*device) {
		ACPI_DEBUG_PRINT((ACPI_DB_INFO, "No context for object [%p]\n",
				  handle));
		return -ENODEV;
	}

	return 0;
}

EXPORT_SYMBOL(acpi_bus_get_device);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
acpi_status acpi_bus_get_status_handle(acpi_handle handle,
				       unsigned long long *sta)
{
	acpi_status status;

	status = acpi_evaluate_integer(handle, "_STA", NULL, sta);
	if (ACPI_SUCCESS(status))
		return AE_OK;

	if (status == AE_NOT_FOUND) {
		*sta = ACPI_STA_DEVICE_PRESENT | ACPI_STA_DEVICE_ENABLED |
		       ACPI_STA_DEVICE_UI      | ACPI_STA_DEVICE_FUNCTIONING;
		return AE_OK;
	}
	return status;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(acpi_bus_get_status_handle);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int acpi_bus_get_status(struct acpi_device *device)
{
	acpi_status status;
	unsigned long long sta;

<<<<<<< HEAD
=======
	if (acpi_device_override_status(device, &sta)) {
		acpi_set_device_status(device, sta);
		return 0;
	}

	/* Battery devices must have their deps met before calling _STA */
	if (acpi_device_is_battery(device) && device->dep_unmet) {
		acpi_set_device_status(device, 0);
		return 0;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	status = acpi_bus_get_status_handle(device->handle, &sta);
	if (ACPI_FAILURE(status))
		return -ENODEV;

<<<<<<< HEAD
	STRUCT_TO_INT(device->status) = (int) sta;

	if (device->status.functional && !device->status.present) {
		ACPI_DEBUG_PRINT((ACPI_DB_INFO, "Device [%s] status [%08x]: "
		       "functional but not present;\n",
			device->pnp.bus_id,
			(u32) STRUCT_TO_INT(device->status)));
	}

	ACPI_DEBUG_PRINT((ACPI_DB_INFO, "Device [%s] status [%08x]\n",
			  device->pnp.bus_id,
			  (u32) STRUCT_TO_INT(device->status)));
=======
	acpi_set_device_status(device, sta);

	if (device->status.functional && !device->status.present) {
		pr_debug("Device [%s] status [%08x]: functional but not present\n",
			 device->pnp.bus_id, (u32)sta);
	}

	pr_debug("Device [%s] status [%08x]\n", device->pnp.bus_id, (u32)sta);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
EXPORT_SYMBOL(acpi_bus_get_status);

void acpi_bus_private_data_handler(acpi_handle handle,
				   void *context)
{
	return;
}
EXPORT_SYMBOL(acpi_bus_private_data_handler);

<<<<<<< HEAD
int acpi_bus_get_private_data(acpi_handle handle, void **data)
{
	acpi_status status = AE_OK;

	if (!*data)
		return -EINVAL;

	status = acpi_get_data(handle, acpi_bus_private_data_handler, data);
	if (ACPI_FAILURE(status) || !*data) {
		ACPI_DEBUG_PRINT((ACPI_DB_INFO, "No context for object [%p]\n",
				handle));
=======
int acpi_bus_attach_private_data(acpi_handle handle, void *data)
{
	acpi_status status;

	status = acpi_attach_data(handle,
			acpi_bus_private_data_handler, data);
	if (ACPI_FAILURE(status)) {
		acpi_handle_debug(handle, "Error attaching device data\n");
		return -ENODEV;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(acpi_bus_attach_private_data);

int acpi_bus_get_private_data(acpi_handle handle, void **data)
{
	acpi_status status;

	if (!data)
		return -EINVAL;

	status = acpi_get_data(handle, acpi_bus_private_data_handler, data);
	if (ACPI_FAILURE(status)) {
		acpi_handle_debug(handle, "No context for object\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENODEV;
	}

	return 0;
}
<<<<<<< HEAD
EXPORT_SYMBOL(acpi_bus_get_private_data);

/* --------------------------------------------------------------------------
                                 Power Management
   -------------------------------------------------------------------------- */

static int __acpi_bus_get_power(struct acpi_device *device, int *state)
{
	int result = 0;
	acpi_status status = 0;
	unsigned long long psc = 0;

	if (!device || !state)
		return -EINVAL;

	*state = ACPI_STATE_UNKNOWN;

	if (device->flags.power_manageable) {
		/*
		 * Get the device's power state either directly (via _PSC) or
		 * indirectly (via power resources).
		 */
		if (device->power.flags.power_resources) {
			result = acpi_power_get_inferred_state(device, state);
			if (result)
				return result;
		} else if (device->power.flags.explicit_get) {
			status = acpi_evaluate_integer(device->handle, "_PSC",
						       NULL, &psc);
			if (ACPI_FAILURE(status))
				return -ENODEV;
			*state = (int)psc;
		}
	} else {
		/* TBD: Non-recursive algorithm for walking up hierarchy. */
		*state = device->parent ?
			device->parent->power.state : ACPI_STATE_D0;
	}

	ACPI_DEBUG_PRINT((ACPI_DB_INFO, "Device [%s] power state is D%d\n",
			  device->pnp.bus_id, *state));

	return 0;
}


static int __acpi_bus_set_power(struct acpi_device *device, int state)
{
	int result = 0;
	acpi_status status = AE_OK;
	char object_name[5] = { '_', 'P', 'S', '0' + state, '\0' };

	if (!device || (state < ACPI_STATE_D0) || (state > ACPI_STATE_D3_COLD))
		return -EINVAL;

	/* Make sure this is a valid target state */

	if (state == device->power.state) {
		ACPI_DEBUG_PRINT((ACPI_DB_INFO, "Device is already at D%d\n",
				  state));
		return 0;
	}

	if (!device->power.states[state].flags.valid) {
		printk(KERN_WARNING PREFIX "Device does not support D%d\n", state);
		return -ENODEV;
	}
	if (device->parent && (state < device->parent->power.state)) {
		printk(KERN_WARNING PREFIX
			      "Cannot set device to a higher-powered"
			      " state than parent\n");
		return -ENODEV;
	}

	/* For D3cold we should execute _PS3, not _PS4. */
	if (state == ACPI_STATE_D3_COLD)
		object_name[3] = '3';

	/*
	 * Transition Power
	 * ----------------
	 * On transitions to a high-powered state we first apply power (via
	 * power resources) then evalute _PSx.  Conversly for transitions to
	 * a lower-powered state.
	 */
	if (state < device->power.state) {
		if (device->power.flags.power_resources) {
			result = acpi_power_transition(device, state);
			if (result)
				goto end;
		}
		if (device->power.states[state].flags.explicit_set) {
			status = acpi_evaluate_object(device->handle,
						      object_name, NULL, NULL);
			if (ACPI_FAILURE(status)) {
				result = -ENODEV;
				goto end;
			}
		}
	} else {
		if (device->power.states[state].flags.explicit_set) {
			status = acpi_evaluate_object(device->handle,
						      object_name, NULL, NULL);
			if (ACPI_FAILURE(status)) {
				result = -ENODEV;
				goto end;
			}
		}
		if (device->power.flags.power_resources) {
			result = acpi_power_transition(device, state);
			if (result)
				goto end;
		}
	}

      end:
	if (result)
		printk(KERN_WARNING PREFIX
			      "Device [%s] failed to transition to D%d\n",
			      device->pnp.bus_id, state);
	else {
		device->power.state = state;
		ACPI_DEBUG_PRINT((ACPI_DB_INFO,
				  "Device [%s] transitioned to D%d\n",
				  device->pnp.bus_id, state));
	}

	return result;
}


int acpi_bus_set_power(acpi_handle handle, int state)
{
	struct acpi_device *device;
	int result;

	result = acpi_bus_get_device(handle, &device);
	if (result)
		return result;

	if (!device->flags.power_manageable) {
		ACPI_DEBUG_PRINT((ACPI_DB_INFO,
				"Device [%s] is not power manageable\n",
				dev_name(&device->dev)));
		return -ENODEV;
	}

	return __acpi_bus_set_power(device, state);
}
EXPORT_SYMBOL(acpi_bus_set_power);


int acpi_bus_init_power(struct acpi_device *device)
{
	int state;
	int result;

	if (!device)
		return -EINVAL;

	device->power.state = ACPI_STATE_UNKNOWN;

	result = __acpi_bus_get_power(device, &state);
	if (result)
		return result;

	if (device->power.flags.power_resources)
		result = acpi_power_on_resources(device, state);

	if (!result)
		device->power.state = state;

	return result;
}


int acpi_bus_update_power(acpi_handle handle, int *state_p)
{
	struct acpi_device *device;
	int state;
	int result;

	result = acpi_bus_get_device(handle, &device);
	if (result)
		return result;

	result = __acpi_bus_get_power(device, &state);
	if (result)
		return result;

	result = __acpi_bus_set_power(device, state);
	if (!result && state_p)
		*state_p = state;

	return result;
}
EXPORT_SYMBOL_GPL(acpi_bus_update_power);


bool acpi_bus_power_manageable(acpi_handle handle)
{
	struct acpi_device *device;
	int result;

	result = acpi_bus_get_device(handle, &device);
	return result ? false : device->flags.power_manageable;
}

EXPORT_SYMBOL(acpi_bus_power_manageable);

bool acpi_bus_can_wakeup(acpi_handle handle)
{
	struct acpi_device *device;
	int result;

	result = acpi_bus_get_device(handle, &device);
	return result ? false : device->wakeup.flags.valid;
}

EXPORT_SYMBOL(acpi_bus_can_wakeup);

static void acpi_print_osc_error(acpi_handle handle,
	struct acpi_osc_context *context, char *error)
{
	struct acpi_buffer buffer = {ACPI_ALLOCATE_BUFFER};
	int i;

	if (ACPI_FAILURE(acpi_get_name(handle, ACPI_FULL_PATHNAME, &buffer)))
		printk(KERN_DEBUG "%s\n", error);
	else {
		printk(KERN_DEBUG "%s:%s\n", (char *)buffer.pointer, error);
		kfree(buffer.pointer);
	}
	printk(KERN_DEBUG"_OSC request data:");
	for (i = 0; i < context->cap.length; i += sizeof(u32))
		printk("%x ", *((u32 *)(context->cap.pointer + i)));
	printk("\n");
}

static acpi_status acpi_str_to_uuid(char *str, u8 *uuid)
{
	int i;
	static int opc_map_to_uuid[16] = {6, 4, 2, 0, 11, 9, 16, 14, 19, 21,
		24, 26, 28, 30, 32, 34};

	if (strlen(str) != 36)
		return AE_BAD_PARAMETER;
	for (i = 0; i < 36; i++) {
		if (i == 8 || i == 13 || i == 18 || i == 23) {
			if (str[i] != '-')
				return AE_BAD_PARAMETER;
		} else if (!isxdigit(str[i]))
			return AE_BAD_PARAMETER;
	}
	for (i = 0; i < 16; i++) {
		uuid[i] = hex_to_bin(str[opc_map_to_uuid[i]]) << 4;
		uuid[i] |= hex_to_bin(str[opc_map_to_uuid[i] + 1]);
	}
	return AE_OK;
=======
EXPORT_SYMBOL_GPL(acpi_bus_get_private_data);

void acpi_bus_detach_private_data(acpi_handle handle)
{
	acpi_detach_data(handle, acpi_bus_private_data_handler);
}
EXPORT_SYMBOL_GPL(acpi_bus_detach_private_data);

static void acpi_print_osc_error(acpi_handle handle,
				 struct acpi_osc_context *context, char *error)
{
	int i;

	acpi_handle_debug(handle, "(%s): %s\n", context->uuid_str, error);

	pr_debug("_OSC request data:");
	for (i = 0; i < context->cap.length; i += sizeof(u32))
		pr_debug(" %x", *((u32 *)(context->cap.pointer + i)));

	pr_debug("\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

acpi_status acpi_run_osc(acpi_handle handle, struct acpi_osc_context *context)
{
	acpi_status status;
	struct acpi_object_list input;
	union acpi_object in_params[4];
	union acpi_object *out_obj;
<<<<<<< HEAD
	u8 uuid[16];
=======
	guid_t guid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32 errors;
	struct acpi_buffer output = {ACPI_ALLOCATE_BUFFER, NULL};

	if (!context)
		return AE_ERROR;
<<<<<<< HEAD
	if (ACPI_FAILURE(acpi_str_to_uuid(context->uuid_str, uuid)))
=======
	if (guid_parse(context->uuid_str, &guid))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return AE_ERROR;
	context->ret.length = ACPI_ALLOCATE_BUFFER;
	context->ret.pointer = NULL;

	/* Setting up input parameters */
	input.count = 4;
	input.pointer = in_params;
	in_params[0].type 		= ACPI_TYPE_BUFFER;
	in_params[0].buffer.length 	= 16;
<<<<<<< HEAD
	in_params[0].buffer.pointer	= uuid;
=======
	in_params[0].buffer.pointer	= (u8 *)&guid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	in_params[1].type 		= ACPI_TYPE_INTEGER;
	in_params[1].integer.value 	= context->rev;
	in_params[2].type 		= ACPI_TYPE_INTEGER;
	in_params[2].integer.value	= context->cap.length/sizeof(u32);
	in_params[3].type		= ACPI_TYPE_BUFFER;
	in_params[3].buffer.length 	= context->cap.length;
	in_params[3].buffer.pointer 	= context->cap.pointer;

	status = acpi_evaluate_object(handle, "_OSC", &input, &output);
	if (ACPI_FAILURE(status))
		return status;

	if (!output.length)
		return AE_NULL_OBJECT;

	out_obj = output.pointer;
	if (out_obj->type != ACPI_TYPE_BUFFER
		|| out_obj->buffer.length != context->cap.length) {
		acpi_print_osc_error(handle, context,
			"_OSC evaluation returned wrong type");
		status = AE_TYPE;
		goto out_kfree;
	}
	/* Need to ignore the bit0 in result code */
	errors = *((u32 *)out_obj->buffer.pointer) & ~(1 << 0);
	if (errors) {
		if (errors & OSC_REQUEST_ERROR)
			acpi_print_osc_error(handle, context,
				"_OSC request failed");
		if (errors & OSC_INVALID_UUID_ERROR)
			acpi_print_osc_error(handle, context,
				"_OSC invalid UUID");
		if (errors & OSC_INVALID_REVISION_ERROR)
			acpi_print_osc_error(handle, context,
				"_OSC invalid revision");
		if (errors & OSC_CAPABILITIES_MASK_ERROR) {
<<<<<<< HEAD
			if (((u32 *)context->cap.pointer)[OSC_QUERY_TYPE]
=======
			if (((u32 *)context->cap.pointer)[OSC_QUERY_DWORD]
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			    & OSC_QUERY_ENABLE)
				goto out_success;
			status = AE_SUPPORT;
			goto out_kfree;
		}
		status = AE_ERROR;
		goto out_kfree;
	}
out_success:
	context->ret.length = out_obj->buffer.length;
<<<<<<< HEAD
	context->ret.pointer = kmalloc(context->ret.length, GFP_KERNEL);
=======
	context->ret.pointer = kmemdup(out_obj->buffer.pointer,
				       context->ret.length, GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!context->ret.pointer) {
		status =  AE_NO_MEMORY;
		goto out_kfree;
	}
<<<<<<< HEAD
	memcpy(context->ret.pointer, out_obj->buffer.pointer,
		context->ret.length);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	status =  AE_OK;

out_kfree:
	kfree(output.pointer);
<<<<<<< HEAD
	if (status != AE_OK)
		context->ret.pointer = NULL;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return status;
}
EXPORT_SYMBOL(acpi_run_osc);

bool osc_sb_apei_support_acked;
<<<<<<< HEAD
static u8 sb_uuid_str[] = "0811B06E-4A27-44F9-8D60-3CBBC22E7B48";
static void acpi_bus_osc_support(void)
{
	u32 capbuf[2];
=======

/*
 * ACPI 6.0 Section 8.4.4.2 Idle State Coordination
 * OSPM supports platform coordinated low power idle(LPI) states
 */
bool osc_pc_lpi_support_confirmed;
EXPORT_SYMBOL_GPL(osc_pc_lpi_support_confirmed);

/*
 * ACPI 6.2 Section 6.2.11.2 'Platform-Wide OSPM Capabilities':
 *   Starting with ACPI Specification 6.2, all _CPC registers can be in
 *   PCC, System Memory, System IO, or Functional Fixed Hardware address
 *   spaces. OSPM support for this more flexible register space scheme is
 *   indicated by the “Flexible Address Space for CPPC Registers” _OSC bit.
 *
 * Otherwise (cf ACPI 6.1, s8.4.7.1.1.X), _CPC registers must be in:
 * - PCC or Functional Fixed Hardware address space if defined
 * - SystemMemory address space (NULL register) if not defined
 */
bool osc_cpc_flexible_adr_space_confirmed;
EXPORT_SYMBOL_GPL(osc_cpc_flexible_adr_space_confirmed);

/*
 * ACPI 6.4 Operating System Capabilities for USB.
 */
bool osc_sb_native_usb4_support_confirmed;
EXPORT_SYMBOL_GPL(osc_sb_native_usb4_support_confirmed);

bool osc_sb_cppc2_support_acked;

static u8 sb_uuid_str[] = "0811B06E-4A27-44F9-8D60-3CBBC22E7B48";
static void acpi_bus_osc_negotiate_platform_control(void)
{
	u32 capbuf[2], *capbuf_ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct acpi_osc_context context = {
		.uuid_str = sb_uuid_str,
		.rev = 1,
		.cap.length = 8,
		.cap.pointer = capbuf,
	};
	acpi_handle handle;

<<<<<<< HEAD
	capbuf[OSC_QUERY_TYPE] = OSC_QUERY_ENABLE;
	capbuf[OSC_SUPPORT_TYPE] = OSC_SB_PR3_SUPPORT; /* _PR3 is in use */
#if defined(CONFIG_ACPI_PROCESSOR_AGGREGATOR) ||\
			defined(CONFIG_ACPI_PROCESSOR_AGGREGATOR_MODULE)
	capbuf[OSC_SUPPORT_TYPE] |= OSC_SB_PAD_SUPPORT;
#endif

#if defined(CONFIG_ACPI_PROCESSOR) || defined(CONFIG_ACPI_PROCESSOR_MODULE)
	capbuf[OSC_SUPPORT_TYPE] |= OSC_SB_PPC_OST_SUPPORT;
#endif

	if (!ghes_disable)
		capbuf[OSC_SUPPORT_TYPE] |= OSC_SB_APEI_SUPPORT;
	if (ACPI_FAILURE(acpi_get_handle(NULL, "\\_SB", &handle)))
		return;
	if (ACPI_SUCCESS(acpi_run_osc(handle, &context))) {
		u32 *capbuf_ret = context.ret.pointer;
		if (context.ret.length > OSC_SUPPORT_TYPE)
			osc_sb_apei_support_acked =
				capbuf_ret[OSC_SUPPORT_TYPE] & OSC_SB_APEI_SUPPORT;
		kfree(context.ret.pointer);
	}
	/* do we need to check other returned cap? Sounds no */
}

/* --------------------------------------------------------------------------
                                Event Management
   -------------------------------------------------------------------------- */

#ifdef CONFIG_ACPI_PROC_EVENT
static DEFINE_SPINLOCK(acpi_bus_event_lock);

LIST_HEAD(acpi_bus_event_list);
DECLARE_WAIT_QUEUE_HEAD(acpi_bus_event_queue);

extern int event_is_open;

int acpi_bus_generate_proc_event4(const char *device_class, const char *bus_id, u8 type, int data)
{
	struct acpi_bus_event *event;
	unsigned long flags = 0;

	/* drop event on the floor if no one's listening */
	if (!event_is_open)
		return 0;

	event = kzalloc(sizeof(struct acpi_bus_event), GFP_ATOMIC);
	if (!event)
		return -ENOMEM;

	strcpy(event->device_class, device_class);
	strcpy(event->bus_id, bus_id);
	event->type = type;
	event->data = data;

	spin_lock_irqsave(&acpi_bus_event_lock, flags);
	list_add_tail(&event->node, &acpi_bus_event_list);
	spin_unlock_irqrestore(&acpi_bus_event_lock, flags);

	wake_up_interruptible(&acpi_bus_event_queue);

	return 0;

}

EXPORT_SYMBOL_GPL(acpi_bus_generate_proc_event4);

int acpi_bus_generate_proc_event(struct acpi_device *device, u8 type, int data)
{
	if (!device)
		return -EINVAL;
	return acpi_bus_generate_proc_event4(device->pnp.device_class,
					     device->pnp.bus_id, type, data);
}

EXPORT_SYMBOL(acpi_bus_generate_proc_event);

int acpi_bus_receive_event(struct acpi_bus_event *event)
{
	unsigned long flags = 0;
	struct acpi_bus_event *entry = NULL;

	DECLARE_WAITQUEUE(wait, current);


	if (!event)
		return -EINVAL;

	if (list_empty(&acpi_bus_event_list)) {

		set_current_state(TASK_INTERRUPTIBLE);
		add_wait_queue(&acpi_bus_event_queue, &wait);

		if (list_empty(&acpi_bus_event_list))
			schedule();

		remove_wait_queue(&acpi_bus_event_queue, &wait);
		set_current_state(TASK_RUNNING);

		if (signal_pending(current))
			return -ERESTARTSYS;
	}

	spin_lock_irqsave(&acpi_bus_event_lock, flags);
	if (!list_empty(&acpi_bus_event_list)) {
		entry = list_entry(acpi_bus_event_list.next,
				   struct acpi_bus_event, node);
		list_del(&entry->node);
	}
	spin_unlock_irqrestore(&acpi_bus_event_lock, flags);

	if (!entry)
		return -ENODEV;

	memcpy(event, entry, sizeof(struct acpi_bus_event));

	kfree(entry);

	return 0;
}

#endif	/* CONFIG_ACPI_PROC_EVENT */
=======
	capbuf[OSC_QUERY_DWORD] = OSC_QUERY_ENABLE;
	capbuf[OSC_SUPPORT_DWORD] = OSC_SB_PR3_SUPPORT; /* _PR3 is in use */
	if (IS_ENABLED(CONFIG_ACPI_PROCESSOR_AGGREGATOR))
		capbuf[OSC_SUPPORT_DWORD] |= OSC_SB_PAD_SUPPORT;
	if (IS_ENABLED(CONFIG_ACPI_PROCESSOR))
		capbuf[OSC_SUPPORT_DWORD] |= OSC_SB_PPC_OST_SUPPORT;

	capbuf[OSC_SUPPORT_DWORD] |= OSC_SB_HOTPLUG_OST_SUPPORT;
	capbuf[OSC_SUPPORT_DWORD] |= OSC_SB_PCLPI_SUPPORT;
	if (IS_ENABLED(CONFIG_ACPI_PRMT))
		capbuf[OSC_SUPPORT_DWORD] |= OSC_SB_PRM_SUPPORT;
	if (IS_ENABLED(CONFIG_ACPI_FFH))
		capbuf[OSC_SUPPORT_DWORD] |= OSC_SB_FFH_OPR_SUPPORT;

#ifdef CONFIG_ARM64
	capbuf[OSC_SUPPORT_DWORD] |= OSC_SB_GENERIC_INITIATOR_SUPPORT;
#endif
#ifdef CONFIG_X86
	capbuf[OSC_SUPPORT_DWORD] |= OSC_SB_GENERIC_INITIATOR_SUPPORT;
#endif

#ifdef CONFIG_ACPI_CPPC_LIB
	capbuf[OSC_SUPPORT_DWORD] |= OSC_SB_CPC_SUPPORT;
	capbuf[OSC_SUPPORT_DWORD] |= OSC_SB_CPCV2_SUPPORT;
#endif

	capbuf[OSC_SUPPORT_DWORD] |= OSC_SB_CPC_FLEXIBLE_ADR_SPACE;

	if (IS_ENABLED(CONFIG_SCHED_MC_PRIO))
		capbuf[OSC_SUPPORT_DWORD] |= OSC_SB_CPC_DIVERSE_HIGH_SUPPORT;

	if (IS_ENABLED(CONFIG_USB4))
		capbuf[OSC_SUPPORT_DWORD] |= OSC_SB_NATIVE_USB4_SUPPORT;

	if (!ghes_disable)
		capbuf[OSC_SUPPORT_DWORD] |= OSC_SB_APEI_SUPPORT;
	if (ACPI_FAILURE(acpi_get_handle(NULL, "\\_SB", &handle)))
		return;

	if (ACPI_FAILURE(acpi_run_osc(handle, &context)))
		return;

	capbuf_ret = context.ret.pointer;
	if (context.ret.length <= OSC_SUPPORT_DWORD) {
		kfree(context.ret.pointer);
		return;
	}

	/*
	 * Now run _OSC again with query flag clear and with the caps
	 * supported by both the OS and the platform.
	 */
	capbuf[OSC_QUERY_DWORD] = 0;
	capbuf[OSC_SUPPORT_DWORD] = capbuf_ret[OSC_SUPPORT_DWORD];
	kfree(context.ret.pointer);

	if (ACPI_FAILURE(acpi_run_osc(handle, &context)))
		return;

	capbuf_ret = context.ret.pointer;
	if (context.ret.length > OSC_SUPPORT_DWORD) {
#ifdef CONFIG_ACPI_CPPC_LIB
		osc_sb_cppc2_support_acked = capbuf_ret[OSC_SUPPORT_DWORD] & OSC_SB_CPCV2_SUPPORT;
#endif

		osc_sb_apei_support_acked =
			capbuf_ret[OSC_SUPPORT_DWORD] & OSC_SB_APEI_SUPPORT;
		osc_pc_lpi_support_confirmed =
			capbuf_ret[OSC_SUPPORT_DWORD] & OSC_SB_PCLPI_SUPPORT;
		osc_sb_native_usb4_support_confirmed =
			capbuf_ret[OSC_SUPPORT_DWORD] & OSC_SB_NATIVE_USB4_SUPPORT;
		osc_cpc_flexible_adr_space_confirmed =
			capbuf_ret[OSC_SUPPORT_DWORD] & OSC_SB_CPC_FLEXIBLE_ADR_SPACE;
	}

	kfree(context.ret.pointer);
}

/*
 * Native control of USB4 capabilities. If any of the tunneling bits is
 * set it means OS is in control and we use software based connection
 * manager.
 */
u32 osc_sb_native_usb4_control;
EXPORT_SYMBOL_GPL(osc_sb_native_usb4_control);

static void acpi_bus_decode_usb_osc(const char *msg, u32 bits)
{
	pr_info("%s USB3%c DisplayPort%c PCIe%c XDomain%c\n", msg,
	       (bits & OSC_USB_USB3_TUNNELING) ? '+' : '-',
	       (bits & OSC_USB_DP_TUNNELING) ? '+' : '-',
	       (bits & OSC_USB_PCIE_TUNNELING) ? '+' : '-',
	       (bits & OSC_USB_XDOMAIN) ? '+' : '-');
}

static u8 sb_usb_uuid_str[] = "23A0D13A-26AB-486C-9C5F-0FFA525A575A";
static void acpi_bus_osc_negotiate_usb_control(void)
{
	u32 capbuf[3], *capbuf_ret;
	struct acpi_osc_context context = {
		.uuid_str = sb_usb_uuid_str,
		.rev = 1,
		.cap.length = sizeof(capbuf),
		.cap.pointer = capbuf,
	};
	acpi_handle handle;
	acpi_status status;
	u32 control;

	if (!osc_sb_native_usb4_support_confirmed)
		return;

	if (ACPI_FAILURE(acpi_get_handle(NULL, "\\_SB", &handle)))
		return;

	control = OSC_USB_USB3_TUNNELING | OSC_USB_DP_TUNNELING |
		  OSC_USB_PCIE_TUNNELING | OSC_USB_XDOMAIN;

	/*
	 * Run _OSC first with query bit set, trying to get control over
	 * all tunneling. The platform can then clear out bits in the
	 * control dword that it does not want to grant to the OS.
	 */
	capbuf[OSC_QUERY_DWORD] = OSC_QUERY_ENABLE;
	capbuf[OSC_SUPPORT_DWORD] = 0;
	capbuf[OSC_CONTROL_DWORD] = control;

	status = acpi_run_osc(handle, &context);
	if (ACPI_FAILURE(status))
		return;

	if (context.ret.length != sizeof(capbuf)) {
		pr_info("USB4 _OSC: returned invalid length buffer\n");
		goto out_free;
	}

	/*
	 * Run _OSC again now with query bit clear and the control dword
	 * matching what the platform granted (which may not have all
	 * the control bits set).
	 */
	capbuf_ret = context.ret.pointer;

	capbuf[OSC_QUERY_DWORD] = 0;
	capbuf[OSC_CONTROL_DWORD] = capbuf_ret[OSC_CONTROL_DWORD];

	kfree(context.ret.pointer);

	status = acpi_run_osc(handle, &context);
	if (ACPI_FAILURE(status))
		return;

	if (context.ret.length != sizeof(capbuf)) {
		pr_info("USB4 _OSC: returned invalid length buffer\n");
		goto out_free;
	}

	osc_sb_native_usb4_control =
		control & acpi_osc_ctx_get_pci_control(&context);

	acpi_bus_decode_usb_osc("USB4 _OSC: OS supports", control);
	acpi_bus_decode_usb_osc("USB4 _OSC: OS controls",
				osc_sb_native_usb4_control);

out_free:
	kfree(context.ret.pointer);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* --------------------------------------------------------------------------
                             Notification Handling
   -------------------------------------------------------------------------- */

<<<<<<< HEAD
static void acpi_bus_check_device(acpi_handle handle)
{
	struct acpi_device *device;
	acpi_status status;
	struct acpi_device_status old_status;

	if (acpi_bus_get_device(handle, &device))
		return;
	if (!device)
		return;

	old_status = device->status;

	/*
	 * Make sure this device's parent is present before we go about
	 * messing with the device.
	 */
	if (device->parent && !device->parent->status.present) {
		device->status = device->parent->status;
		return;
	}

	status = acpi_bus_get_status(device);
	if (ACPI_FAILURE(status))
		return;

	if (STRUCT_TO_INT(old_status) == STRUCT_TO_INT(device->status))
		return;

	/*
	 * Device Insertion/Removal
	 */
	if ((device->status.present) && !(old_status.present)) {
		ACPI_DEBUG_PRINT((ACPI_DB_INFO, "Device insertion detected\n"));
		/* TBD: Handle device insertion */
	} else if (!(device->status.present) && (old_status.present)) {
		ACPI_DEBUG_PRINT((ACPI_DB_INFO, "Device removal detected\n"));
		/* TBD: Handle device removal */
	}
}

static void acpi_bus_check_scope(acpi_handle handle)
{
	/* Status Change? */
	acpi_bus_check_device(handle);

	/*
	 * TBD: Enumerate child devices within this device's scope and
	 *       run acpi_bus_check_device()'s on them.
	 */
}

static BLOCKING_NOTIFIER_HEAD(acpi_bus_notify_list);
int register_acpi_bus_notifier(struct notifier_block *nb)
{
	return blocking_notifier_chain_register(&acpi_bus_notify_list, nb);
}
EXPORT_SYMBOL_GPL(register_acpi_bus_notifier);

void unregister_acpi_bus_notifier(struct notifier_block *nb)
{
	blocking_notifier_chain_unregister(&acpi_bus_notify_list, nb);
}
EXPORT_SYMBOL_GPL(unregister_acpi_bus_notifier);

/**
 * acpi_bus_notify
 * ---------------
 * Callback for all 'system-level' device notifications (values 0x00-0x7F).
 */
static void acpi_bus_notify(acpi_handle handle, u32 type, void *data)
{
	struct acpi_device *device = NULL;
	struct acpi_driver *driver;

	ACPI_DEBUG_PRINT((ACPI_DB_INFO, "Notification %#02x to handle %p\n",
			  type, handle));

	blocking_notifier_call_chain(&acpi_bus_notify_list,
		type, (void *)handle);

	switch (type) {

	case ACPI_NOTIFY_BUS_CHECK:
		acpi_bus_check_scope(handle);
		/*
		 * TBD: We'll need to outsource certain events to non-ACPI
		 *      drivers via the device manager (device.c).
		 */
		break;

	case ACPI_NOTIFY_DEVICE_CHECK:
		acpi_bus_check_device(handle);
		/*
		 * TBD: We'll need to outsource certain events to non-ACPI
		 *      drivers via the device manager (device.c).
		 */
		break;

	case ACPI_NOTIFY_DEVICE_WAKE:
		/* TBD */
		break;

	case ACPI_NOTIFY_EJECT_REQUEST:
		/* TBD */
		break;

	case ACPI_NOTIFY_DEVICE_CHECK_LIGHT:
		/* TBD: Exactly what does 'light' mean? */
		break;

	case ACPI_NOTIFY_FREQUENCY_MISMATCH:
		/* TBD */
		break;

	case ACPI_NOTIFY_BUS_MODE_MISMATCH:
		/* TBD */
		break;

	case ACPI_NOTIFY_POWER_FAULT:
		/* TBD */
		break;

	default:
		ACPI_DEBUG_PRINT((ACPI_DB_INFO,
				  "Received unknown/unsupported notification [%08x]\n",
				  type));
		break;
	}

	acpi_bus_get_device(handle, &device);
	if (device) {
		driver = device->driver;
		if (driver && driver->ops.notify &&
		    (driver->flags & ACPI_DRIVER_ALL_NOTIFY_EVENTS))
			driver->ops.notify(device, type);
	}
}

=======
/**
 * acpi_bus_notify - Global system-level (0x00-0x7F) notifications handler
 * @handle: Target ACPI object.
 * @type: Notification type.
 * @data: Ignored.
 *
 * This only handles notifications related to device hotplug.
 */
static void acpi_bus_notify(acpi_handle handle, u32 type, void *data)
{
	struct acpi_device *adev;

	switch (type) {
	case ACPI_NOTIFY_BUS_CHECK:
		acpi_handle_debug(handle, "ACPI_NOTIFY_BUS_CHECK event\n");
		break;

	case ACPI_NOTIFY_DEVICE_CHECK:
		acpi_handle_debug(handle, "ACPI_NOTIFY_DEVICE_CHECK event\n");
		break;

	case ACPI_NOTIFY_DEVICE_WAKE:
		acpi_handle_debug(handle, "ACPI_NOTIFY_DEVICE_WAKE event\n");
		return;

	case ACPI_NOTIFY_EJECT_REQUEST:
		acpi_handle_debug(handle, "ACPI_NOTIFY_EJECT_REQUEST event\n");
		break;

	case ACPI_NOTIFY_DEVICE_CHECK_LIGHT:
		acpi_handle_debug(handle, "ACPI_NOTIFY_DEVICE_CHECK_LIGHT event\n");
		/* TBD: Exactly what does 'light' mean? */
		return;

	case ACPI_NOTIFY_FREQUENCY_MISMATCH:
		acpi_handle_err(handle, "Device cannot be configured due "
				"to a frequency mismatch\n");
		return;

	case ACPI_NOTIFY_BUS_MODE_MISMATCH:
		acpi_handle_err(handle, "Device cannot be configured due "
				"to a bus mode mismatch\n");
		return;

	case ACPI_NOTIFY_POWER_FAULT:
		acpi_handle_err(handle, "Device has suffered a power fault\n");
		return;

	default:
		acpi_handle_debug(handle, "Unknown event type 0x%x\n", type);
		return;
	}

	adev = acpi_get_acpi_dev(handle);

	if (adev && ACPI_SUCCESS(acpi_hotplug_schedule(adev, type)))
		return;

	acpi_put_acpi_dev(adev);

	acpi_evaluate_ost(handle, type, ACPI_OST_SC_NON_SPECIFIC_FAILURE, NULL);
}

static void acpi_notify_device(acpi_handle handle, u32 event, void *data)
{
	struct acpi_device *device = data;
	struct acpi_driver *acpi_drv = to_acpi_driver(device->dev.driver);

	acpi_drv->ops.notify(device, event);
}

static int acpi_device_install_notify_handler(struct acpi_device *device,
					      struct acpi_driver *acpi_drv)
{
	u32 type = acpi_drv->flags & ACPI_DRIVER_ALL_NOTIFY_EVENTS ?
				ACPI_ALL_NOTIFY : ACPI_DEVICE_NOTIFY;
	acpi_status status;

	status = acpi_install_notify_handler(device->handle, type,
					     acpi_notify_device, device);
	if (ACPI_FAILURE(status))
		return -EINVAL;

	return 0;
}

static void acpi_device_remove_notify_handler(struct acpi_device *device,
					      struct acpi_driver *acpi_drv)
{
	u32 type = acpi_drv->flags & ACPI_DRIVER_ALL_NOTIFY_EVENTS ?
				ACPI_ALL_NOTIFY : ACPI_DEVICE_NOTIFY;

	acpi_remove_notify_handler(device->handle, type,
				   acpi_notify_device);

	acpi_os_wait_events_complete();
}

int acpi_dev_install_notify_handler(struct acpi_device *adev,
				    u32 handler_type,
				    acpi_notify_handler handler, void *context)
{
	acpi_status status;

	status = acpi_install_notify_handler(adev->handle, handler_type,
					     handler, context);
	if (ACPI_FAILURE(status))
		return -ENODEV;

	return 0;
}
EXPORT_SYMBOL_GPL(acpi_dev_install_notify_handler);

void acpi_dev_remove_notify_handler(struct acpi_device *adev,
				    u32 handler_type,
				    acpi_notify_handler handler)
{
	acpi_remove_notify_handler(adev->handle, handler_type, handler);
	acpi_os_wait_events_complete();
}
EXPORT_SYMBOL_GPL(acpi_dev_remove_notify_handler);

/* Handle events targeting \_SB device (at present only graceful shutdown) */

#define ACPI_SB_NOTIFY_SHUTDOWN_REQUEST 0x81
#define ACPI_SB_INDICATE_INTERVAL	10000

static void sb_notify_work(struct work_struct *dummy)
{
	acpi_handle sb_handle;

	orderly_poweroff(true);

	/*
	 * After initiating graceful shutdown, the ACPI spec requires OSPM
	 * to evaluate _OST method once every 10seconds to indicate that
	 * the shutdown is in progress
	 */
	acpi_get_handle(NULL, "\\_SB", &sb_handle);
	while (1) {
		pr_info("Graceful shutdown in progress.\n");
		acpi_evaluate_ost(sb_handle, ACPI_OST_EC_OSPM_SHUTDOWN,
				ACPI_OST_SC_OS_SHUTDOWN_IN_PROGRESS, NULL);
		msleep(ACPI_SB_INDICATE_INTERVAL);
	}
}

static void acpi_sb_notify(acpi_handle handle, u32 event, void *data)
{
	static DECLARE_WORK(acpi_sb_work, sb_notify_work);

	if (event == ACPI_SB_NOTIFY_SHUTDOWN_REQUEST) {
		if (!work_busy(&acpi_sb_work))
			schedule_work(&acpi_sb_work);
	} else {
		pr_warn("event %x is not supported by \\_SB device\n", event);
	}
}

static int __init acpi_setup_sb_notify_handler(void)
{
	acpi_handle sb_handle;

	if (ACPI_FAILURE(acpi_get_handle(NULL, "\\_SB", &sb_handle)))
		return -ENXIO;

	if (ACPI_FAILURE(acpi_install_notify_handler(sb_handle, ACPI_DEVICE_NOTIFY,
						acpi_sb_notify, NULL)))
		return -EINVAL;

	return 0;
}

/* --------------------------------------------------------------------------
                             Device Matching
   -------------------------------------------------------------------------- */

/**
 * acpi_get_first_physical_node - Get first physical node of an ACPI device
 * @adev:	ACPI device in question
 *
 * Return: First physical node of ACPI device @adev
 */
struct device *acpi_get_first_physical_node(struct acpi_device *adev)
{
	struct mutex *physical_node_lock = &adev->physical_node_lock;
	struct device *phys_dev;

	mutex_lock(physical_node_lock);
	if (list_empty(&adev->physical_node_list)) {
		phys_dev = NULL;
	} else {
		const struct acpi_device_physical_node *node;

		node = list_first_entry(&adev->physical_node_list,
					struct acpi_device_physical_node, node);

		phys_dev = node->dev;
	}
	mutex_unlock(physical_node_lock);
	return phys_dev;
}
EXPORT_SYMBOL_GPL(acpi_get_first_physical_node);

static struct acpi_device *acpi_primary_dev_companion(struct acpi_device *adev,
						      const struct device *dev)
{
	const struct device *phys_dev = acpi_get_first_physical_node(adev);

	return phys_dev && phys_dev == dev ? adev : NULL;
}

/**
 * acpi_device_is_first_physical_node - Is given dev first physical node
 * @adev: ACPI companion device
 * @dev: Physical device to check
 *
 * Function checks if given @dev is the first physical devices attached to
 * the ACPI companion device. This distinction is needed in some cases
 * where the same companion device is shared between many physical devices.
 *
 * Note that the caller have to provide valid @adev pointer.
 */
bool acpi_device_is_first_physical_node(struct acpi_device *adev,
					const struct device *dev)
{
	return !!acpi_primary_dev_companion(adev, dev);
}

/*
 * acpi_companion_match() - Can we match via ACPI companion device
 * @dev: Device in question
 *
 * Check if the given device has an ACPI companion and if that companion has
 * a valid list of PNP IDs, and if the device is the first (primary) physical
 * device associated with it.  Return the companion pointer if that's the case
 * or NULL otherwise.
 *
 * If multiple physical devices are attached to a single ACPI companion, we need
 * to be careful.  The usage scenario for this kind of relationship is that all
 * of the physical devices in question use resources provided by the ACPI
 * companion.  A typical case is an MFD device where all the sub-devices share
 * the parent's ACPI companion.  In such cases we can only allow the primary
 * (first) physical device to be matched with the help of the companion's PNP
 * IDs.
 *
 * Additional physical devices sharing the ACPI companion can still use
 * resources available from it but they will be matched normally using functions
 * provided by their bus types (and analogously for their modalias).
 */
const struct acpi_device *acpi_companion_match(const struct device *dev)
{
	struct acpi_device *adev;

	adev = ACPI_COMPANION(dev);
	if (!adev)
		return NULL;

	if (list_empty(&adev->pnp.ids))
		return NULL;

	return acpi_primary_dev_companion(adev, dev);
}

/**
 * acpi_of_match_device - Match device object using the "compatible" property.
 * @adev: ACPI device object to match.
 * @of_match_table: List of device IDs to match against.
 * @of_id: OF ID if matched
 *
 * If @dev has an ACPI companion which has ACPI_DT_NAMESPACE_HID in its list of
 * identifiers and a _DSD object with the "compatible" property, use that
 * property to match against the given list of identifiers.
 */
static bool acpi_of_match_device(const struct acpi_device *adev,
				 const struct of_device_id *of_match_table,
				 const struct of_device_id **of_id)
{
	const union acpi_object *of_compatible, *obj;
	int i, nval;

	if (!adev)
		return false;

	of_compatible = adev->data.of_compatible;
	if (!of_match_table || !of_compatible)
		return false;

	if (of_compatible->type == ACPI_TYPE_PACKAGE) {
		nval = of_compatible->package.count;
		obj = of_compatible->package.elements;
	} else { /* Must be ACPI_TYPE_STRING. */
		nval = 1;
		obj = of_compatible;
	}
	/* Now we can look for the driver DT compatible strings */
	for (i = 0; i < nval; i++, obj++) {
		const struct of_device_id *id;

		for (id = of_match_table; id->compatible[0]; id++)
			if (!strcasecmp(obj->string.pointer, id->compatible)) {
				if (of_id)
					*of_id = id;
				return true;
			}
	}

	return false;
}

static bool acpi_of_modalias(struct acpi_device *adev,
			     char *modalias, size_t len)
{
	const union acpi_object *of_compatible;
	const union acpi_object *obj;
	const char *str, *chr;

	of_compatible = adev->data.of_compatible;
	if (!of_compatible)
		return false;

	if (of_compatible->type == ACPI_TYPE_PACKAGE)
		obj = of_compatible->package.elements;
	else /* Must be ACPI_TYPE_STRING. */
		obj = of_compatible;

	str = obj->string.pointer;
	chr = strchr(str, ',');
	strscpy(modalias, chr ? chr + 1 : str, len);

	return true;
}

/**
 * acpi_set_modalias - Set modalias using "compatible" property or supplied ID
 * @adev:	ACPI device object to match
 * @default_id:	ID string to use as default if no compatible string found
 * @modalias:   Pointer to buffer that modalias value will be copied into
 * @len:	Length of modalias buffer
 *
 * This is a counterpart of of_alias_from_compatible() for struct acpi_device
 * objects. If there is a compatible string for @adev, it will be copied to
 * @modalias with the vendor prefix stripped; otherwise, @default_id will be
 * used.
 */
void acpi_set_modalias(struct acpi_device *adev, const char *default_id,
		       char *modalias, size_t len)
{
	if (!acpi_of_modalias(adev, modalias, len))
		strscpy(modalias, default_id, len);
}
EXPORT_SYMBOL_GPL(acpi_set_modalias);

static bool __acpi_match_device_cls(const struct acpi_device_id *id,
				    struct acpi_hardware_id *hwid)
{
	int i, msk, byte_shift;
	char buf[3];

	if (!id->cls)
		return false;

	/* Apply class-code bitmask, before checking each class-code byte */
	for (i = 1; i <= 3; i++) {
		byte_shift = 8 * (3 - i);
		msk = (id->cls_msk >> byte_shift) & 0xFF;
		if (!msk)
			continue;

		sprintf(buf, "%02x", (id->cls >> byte_shift) & msk);
		if (strncmp(buf, &hwid->id[(i - 1) * 2], 2))
			return false;
	}
	return true;
}

static bool __acpi_match_device(const struct acpi_device *device,
				const struct acpi_device_id *acpi_ids,
				const struct of_device_id *of_ids,
				const struct acpi_device_id **acpi_id,
				const struct of_device_id **of_id)
{
	const struct acpi_device_id *id;
	struct acpi_hardware_id *hwid;

	/*
	 * If the device is not present, it is unnecessary to load device
	 * driver for it.
	 */
	if (!device || !device->status.present)
		return false;

	list_for_each_entry(hwid, &device->pnp.ids, list) {
		/* First, check the ACPI/PNP IDs provided by the caller. */
		if (acpi_ids) {
			for (id = acpi_ids; id->id[0] || id->cls; id++) {
				if (id->id[0] && !strcmp((char *)id->id, hwid->id))
					goto out_acpi_match;
				if (id->cls && __acpi_match_device_cls(id, hwid))
					goto out_acpi_match;
			}
		}

		/*
		 * Next, check ACPI_DT_NAMESPACE_HID and try to match the
		 * "compatible" property if found.
		 */
		if (!strcmp(ACPI_DT_NAMESPACE_HID, hwid->id))
			return acpi_of_match_device(device, of_ids, of_id);
	}
	return false;

out_acpi_match:
	if (acpi_id)
		*acpi_id = id;
	return true;
}

/**
 * acpi_match_acpi_device - Match an ACPI device against a given list of ACPI IDs
 * @ids: Array of struct acpi_device_id objects to match against.
 * @adev: The ACPI device pointer to match.
 *
 * Match the ACPI device @adev against a given list of ACPI IDs @ids.
 *
 * Return:
 * a pointer to the first matching ACPI ID on success or %NULL on failure.
 */
const struct acpi_device_id *acpi_match_acpi_device(const struct acpi_device_id *ids,
						    const struct acpi_device *adev)
{
	const struct acpi_device_id *id = NULL;

	__acpi_match_device(adev, ids, NULL, &id, NULL);
	return id;
}
EXPORT_SYMBOL_GPL(acpi_match_acpi_device);

/**
 * acpi_match_device - Match a struct device against a given list of ACPI IDs
 * @ids: Array of struct acpi_device_id object to match against.
 * @dev: The device structure to match.
 *
 * Check if @dev has a valid ACPI handle and if there is a struct acpi_device
 * object for that handle and use that object to match against a given list of
 * device IDs.
 *
 * Return a pointer to the first matching ID on success or %NULL on failure.
 */
const struct acpi_device_id *acpi_match_device(const struct acpi_device_id *ids,
					       const struct device *dev)
{
	return acpi_match_acpi_device(ids, acpi_companion_match(dev));
}
EXPORT_SYMBOL_GPL(acpi_match_device);

static const void *acpi_of_device_get_match_data(const struct device *dev)
{
	struct acpi_device *adev = ACPI_COMPANION(dev);
	const struct of_device_id *match = NULL;

	if (!acpi_of_match_device(adev, dev->driver->of_match_table, &match))
		return NULL;

	return match->data;
}

const void *acpi_device_get_match_data(const struct device *dev)
{
	const struct acpi_device_id *acpi_ids = dev->driver->acpi_match_table;
	const struct acpi_device_id *match;

	if (!acpi_ids)
		return acpi_of_device_get_match_data(dev);

	match = acpi_match_device(acpi_ids, dev);
	if (!match)
		return NULL;

	return (const void *)match->driver_data;
}
EXPORT_SYMBOL_GPL(acpi_device_get_match_data);

int acpi_match_device_ids(struct acpi_device *device,
			  const struct acpi_device_id *ids)
{
	return __acpi_match_device(device, ids, NULL, NULL, NULL) ? 0 : -ENOENT;
}
EXPORT_SYMBOL(acpi_match_device_ids);

bool acpi_driver_match_device(struct device *dev,
			      const struct device_driver *drv)
{
	const struct acpi_device_id *acpi_ids = drv->acpi_match_table;
	const struct of_device_id *of_ids = drv->of_match_table;

	if (!acpi_ids)
		return acpi_of_match_device(ACPI_COMPANION(dev), of_ids, NULL);

	return __acpi_match_device(acpi_companion_match(dev), acpi_ids, of_ids, NULL, NULL);
}
EXPORT_SYMBOL_GPL(acpi_driver_match_device);

/* --------------------------------------------------------------------------
                              ACPI Driver Management
   -------------------------------------------------------------------------- */

/**
 * acpi_bus_register_driver - register a driver with the ACPI bus
 * @driver: driver being registered
 *
 * Registers a driver with the ACPI bus.  Searches the namespace for all
 * devices that match the driver's criteria and binds.  Returns zero for
 * success or a negative error status for failure.
 */
int acpi_bus_register_driver(struct acpi_driver *driver)
{
	if (acpi_disabled)
		return -ENODEV;
	driver->drv.name = driver->name;
	driver->drv.bus = &acpi_bus_type;
	driver->drv.owner = driver->owner;

	return driver_register(&driver->drv);
}

EXPORT_SYMBOL(acpi_bus_register_driver);

/**
 * acpi_bus_unregister_driver - unregisters a driver with the ACPI bus
 * @driver: driver to unregister
 *
 * Unregisters a driver with the ACPI bus.  Searches the namespace for all
 * devices that match the driver's criteria and unbinds.
 */
void acpi_bus_unregister_driver(struct acpi_driver *driver)
{
	driver_unregister(&driver->drv);
}

EXPORT_SYMBOL(acpi_bus_unregister_driver);

/* --------------------------------------------------------------------------
                              ACPI Bus operations
   -------------------------------------------------------------------------- */

static int acpi_bus_match(struct device *dev, struct device_driver *drv)
{
	struct acpi_device *acpi_dev = to_acpi_device(dev);
	struct acpi_driver *acpi_drv = to_acpi_driver(drv);

	return acpi_dev->flags.match_driver
		&& !acpi_match_device_ids(acpi_dev, acpi_drv->ids);
}

static int acpi_device_uevent(const struct device *dev, struct kobj_uevent_env *env)
{
	return __acpi_device_uevent_modalias(to_acpi_device(dev), env);
}

static int acpi_device_probe(struct device *dev)
{
	struct acpi_device *acpi_dev = to_acpi_device(dev);
	struct acpi_driver *acpi_drv = to_acpi_driver(dev->driver);
	int ret;

	if (acpi_dev->handler && !acpi_is_pnp_device(acpi_dev))
		return -EINVAL;

	if (!acpi_drv->ops.add)
		return -ENOSYS;

	ret = acpi_drv->ops.add(acpi_dev);
	if (ret) {
		acpi_dev->driver_data = NULL;
		return ret;
	}

	pr_debug("Driver [%s] successfully bound to device [%s]\n",
		 acpi_drv->name, acpi_dev->pnp.bus_id);

	if (acpi_drv->ops.notify) {
		ret = acpi_device_install_notify_handler(acpi_dev, acpi_drv);
		if (ret) {
			if (acpi_drv->ops.remove)
				acpi_drv->ops.remove(acpi_dev);

			acpi_dev->driver_data = NULL;
			return ret;
		}
	}

	pr_debug("Found driver [%s] for device [%s]\n", acpi_drv->name,
		 acpi_dev->pnp.bus_id);

	get_device(dev);
	return 0;
}

static void acpi_device_remove(struct device *dev)
{
	struct acpi_device *acpi_dev = to_acpi_device(dev);
	struct acpi_driver *acpi_drv = to_acpi_driver(dev->driver);

	if (acpi_drv->ops.notify)
		acpi_device_remove_notify_handler(acpi_dev, acpi_drv);

	if (acpi_drv->ops.remove)
		acpi_drv->ops.remove(acpi_dev);

	acpi_dev->driver_data = NULL;

	put_device(dev);
}

const struct bus_type acpi_bus_type = {
	.name		= "acpi",
	.match		= acpi_bus_match,
	.probe		= acpi_device_probe,
	.remove		= acpi_device_remove,
	.uevent		= acpi_device_uevent,
};

int acpi_bus_for_each_dev(int (*fn)(struct device *, void *), void *data)
{
	return bus_for_each_dev(&acpi_bus_type, NULL, data, fn);
}
EXPORT_SYMBOL_GPL(acpi_bus_for_each_dev);

struct acpi_dev_walk_context {
	int (*fn)(struct acpi_device *, void *);
	void *data;
};

static int acpi_dev_for_one_check(struct device *dev, void *context)
{
	struct acpi_dev_walk_context *adwc = context;

	if (dev->bus != &acpi_bus_type)
		return 0;

	return adwc->fn(to_acpi_device(dev), adwc->data);
}
EXPORT_SYMBOL_GPL(acpi_dev_for_each_child);

int acpi_dev_for_each_child(struct acpi_device *adev,
			    int (*fn)(struct acpi_device *, void *), void *data)
{
	struct acpi_dev_walk_context adwc = {
		.fn = fn,
		.data = data,
	};

	return device_for_each_child(&adev->dev, &adwc, acpi_dev_for_one_check);
}

int acpi_dev_for_each_child_reverse(struct acpi_device *adev,
				    int (*fn)(struct acpi_device *, void *),
				    void *data)
{
	struct acpi_dev_walk_context adwc = {
		.fn = fn,
		.data = data,
	};

	return device_for_each_child_reverse(&adev->dev, &adwc, acpi_dev_for_one_check);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* --------------------------------------------------------------------------
                             Initialization/Cleanup
   -------------------------------------------------------------------------- */

static int __init acpi_bus_init_irq(void)
{
<<<<<<< HEAD
	acpi_status status = AE_OK;
	union acpi_object arg = { ACPI_TYPE_INTEGER };
	struct acpi_object_list arg_list = { 1, &arg };
=======
	acpi_status status;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char *message = NULL;


	/*
	 * Let the system know what interrupt model we are using by
	 * evaluating the \_PIC object, if exists.
	 */

	switch (acpi_irq_model) {
	case ACPI_IRQ_MODEL_PIC:
		message = "PIC";
		break;
	case ACPI_IRQ_MODEL_IOAPIC:
		message = "IOAPIC";
		break;
	case ACPI_IRQ_MODEL_IOSAPIC:
		message = "IOSAPIC";
		break;
<<<<<<< HEAD
	case ACPI_IRQ_MODEL_PLATFORM:
		message = "platform specific model";
		break;
	default:
		printk(KERN_WARNING PREFIX "Unknown interrupt routing model\n");
		return -ENODEV;
	}

	printk(KERN_INFO PREFIX "Using %s for interrupt routing\n", message);

	arg.integer.value = acpi_irq_model;

	status = acpi_evaluate_object(NULL, "\\_PIC", &arg_list, NULL);
	if (ACPI_FAILURE(status) && (status != AE_NOT_FOUND)) {
		ACPI_EXCEPTION((AE_INFO, status, "Evaluating _PIC"));
=======
	case ACPI_IRQ_MODEL_GIC:
		message = "GIC";
		break;
	case ACPI_IRQ_MODEL_PLATFORM:
		message = "platform specific model";
		break;
	case ACPI_IRQ_MODEL_LPIC:
		message = "LPIC";
		break;
	default:
		pr_info("Unknown interrupt routing model\n");
		return -ENODEV;
	}

	pr_info("Using %s for interrupt routing\n", message);

	status = acpi_execute_simple_method(NULL, "\\_PIC", acpi_irq_model);
	if (ACPI_FAILURE(status) && (status != AE_NOT_FOUND)) {
		pr_info("_PIC evaluation failed: %s\n", acpi_format_exception(status));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENODEV;
	}

	return 0;
}

<<<<<<< HEAD
u8 acpi_gbl_permanent_mmap;


void __init acpi_early_init(void)
{
	acpi_status status = AE_OK;
=======
/**
 * acpi_early_init - Initialize ACPICA and populate the ACPI namespace.
 *
 * The ACPI tables are accessible after this, but the handling of events has not
 * been initialized and the global lock is not available yet, so AML should not
 * be executed at this point.
 *
 * Doing this before switching the EFI runtime services to virtual mode allows
 * the EfiBootServices memory to be freed slightly earlier on boot.
 */
void __init acpi_early_init(void)
{
	acpi_status status;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (acpi_disabled)
		return;

<<<<<<< HEAD
	printk(KERN_INFO PREFIX "Core revision %08x\n", ACPI_CA_VERSION);
=======
	pr_info("Core revision %08x\n", ACPI_CA_VERSION);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* enable workarounds, unless strict ACPI spec. compliance */
	if (!acpi_strict)
		acpi_gbl_enable_interpreter_slack = TRUE;

<<<<<<< HEAD
	acpi_gbl_permanent_mmap = 1;

	/*
	 * If the machine falls into the DMI check table,
	 * DSDT will be copied to memory
	 */
	dmi_check_system(dsdt_dmi_table);

	status = acpi_reallocate_root_table();
	if (ACPI_FAILURE(status)) {
		printk(KERN_ERR PREFIX
		       "Unable to reallocate ACPI tables\n");
=======
	acpi_permanent_mmap = true;

#ifdef CONFIG_X86
	/*
	 * If the machine falls into the DMI check table,
	 * DSDT will be copied to memory.
	 * Note that calling dmi_check_system() here on other architectures
	 * would not be OK because only x86 initializes dmi early enough.
	 * Thankfully only x86 systems need such quirks for now.
	 */
	dmi_check_system(dsdt_dmi_table);
#endif

	status = acpi_reallocate_root_table();
	if (ACPI_FAILURE(status)) {
		pr_err("Unable to reallocate ACPI tables\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto error0;
	}

	status = acpi_initialize_subsystem();
	if (ACPI_FAILURE(status)) {
<<<<<<< HEAD
		printk(KERN_ERR PREFIX
		       "Unable to initialize the ACPI Interpreter\n");
		goto error0;
	}

	status = acpi_load_tables();
	if (ACPI_FAILURE(status)) {
		printk(KERN_ERR PREFIX
		       "Unable to load the System Description Tables\n");
=======
		pr_err("Unable to initialize the ACPI Interpreter\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto error0;
	}

#ifdef CONFIG_X86
	if (!acpi_ioapic) {
		/* compatible (0) means level (3) */
		if (!(acpi_sci_flags & ACPI_MADT_TRIGGER_MASK)) {
			acpi_sci_flags &= ~ACPI_MADT_TRIGGER_MASK;
			acpi_sci_flags |= ACPI_MADT_TRIGGER_LEVEL;
		}
		/* Set PIC-mode SCI trigger type */
		acpi_pic_sci_set_trigger(acpi_gbl_FADT.sci_interrupt,
					 (acpi_sci_flags & ACPI_MADT_TRIGGER_MASK) >> 2);
	} else {
		/*
		 * now that acpi_gbl_FADT is initialized,
		 * update it with result from INT_SRC_OVR parsing
		 */
		acpi_gbl_FADT.sci_interrupt = acpi_sci_override_gsi;
	}
#endif
<<<<<<< HEAD

	status = acpi_enable_subsystem(~ACPI_NO_ACPI_ENABLE);
	if (ACPI_FAILURE(status)) {
		printk(KERN_ERR PREFIX "Unable to enable ACPI\n");
		goto error0;
	}

	/*
	 * If the system is using ACPI then we can be reasonably
	 * confident that any regulators are managed by the firmware
	 * so tell the regulator core it has everything it needs to
	 * know.
	 */
	regulator_has_full_constraints();

	return;

      error0:
	disable_acpi();
	return;
=======
	return;

 error0:
	disable_acpi();
}

/**
 * acpi_subsystem_init - Finalize the early initialization of ACPI.
 *
 * Switch over the platform to the ACPI mode (if possible).
 *
 * Doing this too early is generally unsafe, but at the same time it needs to be
 * done before all things that really depend on ACPI.  The right spot appears to
 * be before finalizing the EFI initialization.
 */
void __init acpi_subsystem_init(void)
{
	acpi_status status;

	if (acpi_disabled)
		return;

	status = acpi_enable_subsystem(~ACPI_NO_ACPI_ENABLE);
	if (ACPI_FAILURE(status)) {
		pr_err("Unable to enable ACPI\n");
		disable_acpi();
	} else {
		/*
		 * If the system is using ACPI then we can be reasonably
		 * confident that any regulators are managed by the firmware
		 * so tell the regulator core it has everything it needs to
		 * know.
		 */
		regulator_has_full_constraints();
	}
}

static acpi_status acpi_bus_table_handler(u32 event, void *table, void *context)
{
	if (event == ACPI_TABLE_EVENT_LOAD)
		acpi_scan_table_notify();

	return acpi_sysfs_table_handler(event, table, context);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int __init acpi_bus_init(void)
{
<<<<<<< HEAD
	int result = 0;
	acpi_status status = AE_OK;
	extern acpi_status acpi_os_initialize1(void);

	acpi_os_initialize1();

	status = acpi_enable_subsystem(ACPI_NO_ACPI_ENABLE);
	if (ACPI_FAILURE(status)) {
		printk(KERN_ERR PREFIX
		       "Unable to start the ACPI Interpreter\n");
=======
	int result;
	acpi_status status;

	acpi_os_initialize1();

	status = acpi_load_tables();
	if (ACPI_FAILURE(status)) {
		pr_err("Unable to load the System Description Tables\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto error1;
	}

	/*
<<<<<<< HEAD
	 * ACPI 2.0 requires the EC driver to be loaded and work before
	 * the EC device is found in the namespace (i.e. before acpi_initialize_objects()
	 * is called).
	 *
	 * This is accomplished by looking for the ECDT table, and getting
	 * the EC parameters out of that.
	 */
	status = acpi_ec_ecdt_probe();
	/* Ignore result. Not having an ECDT is not fatal. */

	status = acpi_initialize_objects(ACPI_FULL_INITIALIZATION);
	if (ACPI_FAILURE(status)) {
		printk(KERN_ERR PREFIX "Unable to initialize ACPI objects\n");
=======
	 * ACPI 2.0 requires the EC driver to be loaded and work before the EC
	 * device is found in the namespace.
	 *
	 * This is accomplished by looking for the ECDT table and getting the EC
	 * parameters out of that.
	 *
	 * Do that before calling acpi_initialize_objects() which may trigger EC
	 * address space accesses.
	 */
	acpi_ec_ecdt_probe();

	status = acpi_enable_subsystem(ACPI_NO_ACPI_ENABLE);
	if (ACPI_FAILURE(status)) {
		pr_err("Unable to start the ACPI Interpreter\n");
		goto error1;
	}

	status = acpi_initialize_objects(ACPI_FULL_INITIALIZATION);
	if (ACPI_FAILURE(status)) {
		pr_err("Unable to initialize ACPI objects\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto error1;
	}

	/*
	 * _OSC method may exist in module level code,
	 * so it must be run after ACPI_FULL_INITIALIZATION
	 */
<<<<<<< HEAD
	acpi_bus_osc_support();
=======
	acpi_bus_osc_negotiate_platform_control();
	acpi_bus_osc_negotiate_usb_control();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * _PDC control method may load dynamic SSDT tables,
	 * and we need to install the table handler before that.
	 */
<<<<<<< HEAD
	acpi_sysfs_init();

	acpi_early_processor_set_pdc();
=======
	status = acpi_install_table_handler(acpi_bus_table_handler, NULL);

	acpi_sysfs_init();

	acpi_early_processor_control_setup();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Maybe EC region is required at bus_scan/acpi_get_devices. So it
	 * is necessary to enable it as early as possible.
	 */
<<<<<<< HEAD
	acpi_boot_ec_enable();

	printk(KERN_INFO PREFIX "Interpreter enabled\n");
=======
	acpi_ec_dsdt_probe();

	pr_info("Interpreter enabled\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Initialize sleep structures */
	acpi_sleep_init();

	/*
	 * Get the system interrupt model and evaluate \_PIC.
	 */
	result = acpi_bus_init_irq();
	if (result)
		goto error1;

	/*
	 * Register the for all standard device notifications.
	 */
	status =
	    acpi_install_notify_handler(ACPI_ROOT_OBJECT, ACPI_SYSTEM_NOTIFY,
					&acpi_bus_notify, NULL);
	if (ACPI_FAILURE(status)) {
<<<<<<< HEAD
		printk(KERN_ERR PREFIX
		       "Unable to register for device notifications\n");
=======
		pr_err("Unable to register for system notifications\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto error1;
	}

	/*
	 * Create the top ACPI proc directory
	 */
	acpi_root_dir = proc_mkdir(ACPI_BUS_FILE_ROOT, NULL);

<<<<<<< HEAD
	return 0;
=======
	result = bus_register(&acpi_bus_type);
	if (!result)
		return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Mimic structured exception handling */
      error1:
	acpi_terminate();
	return -ENODEV;
}

struct kobject *acpi_kobj;
EXPORT_SYMBOL_GPL(acpi_kobj);

static int __init acpi_init(void)
{
	int result;

	if (acpi_disabled) {
<<<<<<< HEAD
		printk(KERN_INFO PREFIX "Interpreter disabled.\n");
=======
		pr_info("Interpreter disabled.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENODEV;
	}

	acpi_kobj = kobject_create_and_add("acpi", firmware_kobj);
<<<<<<< HEAD
	if (!acpi_kobj) {
		printk(KERN_WARNING "%s: kset create error\n", __func__);
		acpi_kobj = NULL;
	}

	init_acpi_device_notify();
	result = acpi_bus_init();
	if (result) {
		disable_acpi();
		return result;
	}

	pci_mmcfg_late_init();
=======
	if (!acpi_kobj)
		pr_debug("%s: kset create error\n", __func__);

	init_prmt();
	acpi_init_pcc();
	result = acpi_bus_init();
	if (result) {
		kobject_put(acpi_kobj);
		disable_acpi();
		return result;
	}
	acpi_init_ffh();

	pci_mmcfg_late_init();
	acpi_viot_early_init();
	acpi_hest_init();
	acpi_ghes_init();
	acpi_arm_init();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	acpi_scan_init();
	acpi_ec_init();
	acpi_debugfs_init();
	acpi_sleep_proc_init();
	acpi_wakeup_device_init();
<<<<<<< HEAD
=======
	acpi_debugger_init();
	acpi_setup_sb_notify_handler();
	acpi_viot_init();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

subsys_initcall(acpi_init);
