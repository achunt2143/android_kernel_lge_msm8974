<<<<<<< HEAD
=======
// SPDX-License-Identifier: BSD-3-Clause OR GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/******************************************************************************
 *
 * Module Name: evmisc - Miscellaneous event manager support functions
 *
<<<<<<< HEAD
 *****************************************************************************/

/*
 * Copyright (C) 2000 - 2012, Intel Corp.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce at minimum a disclaimer
 *    substantially similar to the "NO WARRANTY" disclaimer below
 *    ("Disclaimer") and any redistribution must be conditioned upon
 *    including a substantially similar Disclaimer requirement for further
 *    binary redistribution.
 * 3. Neither the names of the above-listed copyright holders nor the names
 *    of any contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * NO WARRANTY
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 */

=======
 * Copyright (C) 2000 - 2023, Intel Corp.
 *
 *****************************************************************************/

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <acpi/acpi.h>
#include "accommon.h"
#include "acevents.h"
#include "acnamesp.h"

#define _COMPONENT          ACPI_EVENTS
ACPI_MODULE_NAME("evmisc")

/* Local prototypes */
static void ACPI_SYSTEM_XFACE acpi_ev_notify_dispatch(void *context);

/*******************************************************************************
 *
 * FUNCTION:    acpi_ev_is_notify_object
 *
<<<<<<< HEAD
 * PARAMETERS:  Node            - Node to check
=======
 * PARAMETERS:  node            - Node to check
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * RETURN:      TRUE if notifies allowed on this object
 *
 * DESCRIPTION: Check type of node for a object that supports notifies.
 *
 *              TBD: This could be replaced by a flag bit in the node.
 *
 ******************************************************************************/

u8 acpi_ev_is_notify_object(struct acpi_namespace_node *node)
{
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (node->type) {
	case ACPI_TYPE_DEVICE:
	case ACPI_TYPE_PROCESSOR:
	case ACPI_TYPE_THERMAL:
		/*
		 * These are the ONLY objects that can receive ACPI notifications
		 */
		return (TRUE);

	default:
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return (FALSE);
	}
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_ev_queue_notify_request
 *
<<<<<<< HEAD
 * PARAMETERS:  Node            - NS node for the notified object
=======
 * PARAMETERS:  node            - NS node for the notified object
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *              notify_value    - Value from the Notify() request
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Dispatch a device notification event to a previously
 *              installed handler.
 *
 ******************************************************************************/

acpi_status
<<<<<<< HEAD
acpi_ev_queue_notify_request(struct acpi_namespace_node * node,
			     u32 notify_value)
{
	union acpi_operand_object *obj_desc;
	union acpi_operand_object *handler_obj = NULL;
	union acpi_generic_state *notify_info;
=======
acpi_ev_queue_notify_request(struct acpi_namespace_node *node, u32 notify_value)
{
	union acpi_operand_object *obj_desc;
	union acpi_operand_object *handler_list_head = NULL;
	union acpi_generic_state *info;
	u8 handler_list_id = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	acpi_status status = AE_OK;

	ACPI_FUNCTION_NAME(ev_queue_notify_request);

<<<<<<< HEAD
	/*
	 * For value 0x03 (Ejection Request), may need to run a device method.
	 * For value 0x02 (Device Wake), if _PRW exists, may need to run
	 *   the _PS0 method.
	 * For value 0x80 (Status Change) on the power button or sleep button,
	 *   initiate soft-off or sleep operation.
	 *
	 * For all cases, simply dispatch the notify to the handler.
	 */
	ACPI_DEBUG_PRINT((ACPI_DB_INFO,
			  "Dispatching Notify on [%4.4s] (%s) Value 0x%2.2X (%s) Node %p\n",
			  acpi_ut_get_node_name(node),
			  acpi_ut_get_type_name(node->type), notify_value,
			  acpi_ut_get_notify_name(notify_value), node));

	/* Get the notify object attached to the NS Node */
=======
	/* Are Notifies allowed on this object? */

	if (!acpi_ev_is_notify_object(node)) {
		return (AE_TYPE);
	}

	/* Get the correct notify list type (System or Device) */

	if (notify_value <= ACPI_MAX_SYS_NOTIFY) {
		handler_list_id = ACPI_SYSTEM_HANDLER_LIST;
	} else {
		handler_list_id = ACPI_DEVICE_HANDLER_LIST;
	}

	/* Get the notify object attached to the namespace Node */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	obj_desc = acpi_ns_get_attached_object(node);
	if (obj_desc) {

<<<<<<< HEAD
		/* We have the notify object, Get the correct handler */

		switch (node->type) {

			/* Notify is allowed only on these types */

		case ACPI_TYPE_DEVICE:
		case ACPI_TYPE_THERMAL:
		case ACPI_TYPE_PROCESSOR:

			if (notify_value <= ACPI_MAX_SYS_NOTIFY) {
				handler_obj =
				    obj_desc->common_notify.system_notify;
			} else {
				handler_obj =
				    obj_desc->common_notify.device_notify;
			}
			break;

		default:

			/* All other types are not supported */

			return (AE_TYPE);
		}
	}

	/*
	 * If there is a handler to run, schedule the dispatcher.
	 * Check for:
	 * 1) Global system notify handler
	 * 2) Global device notify handler
	 * 3) Per-device notify handler
	 */
	if ((acpi_gbl_system_notify.handler &&
	     (notify_value <= ACPI_MAX_SYS_NOTIFY)) ||
	    (acpi_gbl_device_notify.handler &&
	     (notify_value > ACPI_MAX_SYS_NOTIFY)) || handler_obj) {
		notify_info = acpi_ut_create_generic_state();
		if (!notify_info) {
			return (AE_NO_MEMORY);
		}

		if (!handler_obj) {
			ACPI_DEBUG_PRINT((ACPI_DB_INFO,
					  "Executing system notify handler for Notify (%4.4s, %X) "
					  "node %p\n",
					  acpi_ut_get_node_name(node),
					  notify_value, node));
		}

		notify_info->common.descriptor_type =
		    ACPI_DESC_TYPE_STATE_NOTIFY;
		notify_info->notify.node = node;
		notify_info->notify.value = (u16) notify_value;
		notify_info->notify.handler_obj = handler_obj;

		status =
		    acpi_os_execute(OSL_NOTIFY_HANDLER, acpi_ev_notify_dispatch,
				    notify_info);
		if (ACPI_FAILURE(status)) {
			acpi_ut_delete_generic_state(notify_info);
		}
	} else {
		/* There is no notify handler (per-device or system) for this device */

		ACPI_DEBUG_PRINT((ACPI_DB_INFO,
				  "No notify handler for Notify (%4.4s, %X) node %p\n",
				  acpi_ut_get_node_name(node), notify_value,
				  node));
=======
		/* We have an attached object, Get the correct handler list */

		handler_list_head =
		    obj_desc->common_notify.notify_list[handler_list_id];
	}

	/*
	 * If there is no notify handler (Global or Local)
	 * for this object, just ignore the notify
	 */
	if (!acpi_gbl_global_notify[handler_list_id].handler
	    && !handler_list_head) {
		ACPI_DEBUG_PRINT((ACPI_DB_INFO,
				  "No notify handler for Notify, ignoring (%4.4s, %X) node %p\n",
				  acpi_ut_get_node_name(node), notify_value,
				  node));

		return (AE_OK);
	}

	/* Setup notify info and schedule the notify dispatcher */

	info = acpi_ut_create_generic_state();
	if (!info) {
		return (AE_NO_MEMORY);
	}

	info->common.descriptor_type = ACPI_DESC_TYPE_STATE_NOTIFY;

	info->notify.node = node;
	info->notify.value = (u16)notify_value;
	info->notify.handler_list_id = handler_list_id;
	info->notify.handler_list_head = handler_list_head;
	info->notify.global = &acpi_gbl_global_notify[handler_list_id];

	ACPI_DEBUG_PRINT((ACPI_DB_INFO,
			  "Dispatching Notify on [%4.4s] (%s) Value 0x%2.2X (%s) Node %p\n",
			  acpi_ut_get_node_name(node),
			  acpi_ut_get_type_name(node->type), notify_value,
			  acpi_ut_get_notify_name(notify_value, ACPI_TYPE_ANY),
			  node));

	status = acpi_os_execute(OSL_NOTIFY_HANDLER,
				 acpi_ev_notify_dispatch, info);
	if (ACPI_FAILURE(status)) {
		acpi_ut_delete_generic_state(info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return (status);
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_ev_notify_dispatch
 *
<<<<<<< HEAD
 * PARAMETERS:  Context         - To be passed to the notify handler
=======
 * PARAMETERS:  context         - To be passed to the notify handler
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * RETURN:      None.
 *
 * DESCRIPTION: Dispatch a device notification event to a previously
 *              installed handler.
 *
 ******************************************************************************/

static void ACPI_SYSTEM_XFACE acpi_ev_notify_dispatch(void *context)
{
<<<<<<< HEAD
	union acpi_generic_state *notify_info =
	    (union acpi_generic_state *)context;
	acpi_notify_handler global_handler = NULL;
	void *global_context = NULL;
=======
	union acpi_generic_state *info = (union acpi_generic_state *)context;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	union acpi_operand_object *handler_obj;

	ACPI_FUNCTION_ENTRY();

<<<<<<< HEAD
	/*
	 * We will invoke a global notify handler if installed. This is done
	 * _before_ we invoke the per-device handler attached to the device.
	 */
	if (notify_info->notify.value <= ACPI_MAX_SYS_NOTIFY) {

		/* Global system notification handler */

		if (acpi_gbl_system_notify.handler) {
			global_handler = acpi_gbl_system_notify.handler;
			global_context = acpi_gbl_system_notify.context;
		}
	} else {
		/* Global driver notification handler */

		if (acpi_gbl_device_notify.handler) {
			global_handler = acpi_gbl_device_notify.handler;
			global_context = acpi_gbl_device_notify.context;
		}
	}

	/* Invoke the system handler first, if present */

	if (global_handler) {
		global_handler(notify_info->notify.node,
			       notify_info->notify.value, global_context);
	}

	/* Now invoke the per-device handler, if present */

	handler_obj = notify_info->notify.handler_obj;
	if (handler_obj) {
		struct acpi_object_notify_handler *notifier;

		notifier = &handler_obj->notify;
		while (notifier) {
			notifier->handler(notify_info->notify.node,
					  notify_info->notify.value,
					  notifier->context);
			notifier = notifier->next;
		}
=======
	/* Invoke a global notify handler if installed */

	if (info->notify.global->handler) {
		info->notify.global->handler(info->notify.node,
					     info->notify.value,
					     info->notify.global->context);
	}

	/* Now invoke the local notify handler(s) if any are installed */

	handler_obj = info->notify.handler_list_head;
	while (handler_obj) {
		handler_obj->notify.handler(info->notify.node,
					    info->notify.value,
					    handler_obj->notify.context);

		handler_obj =
		    handler_obj->notify.next[info->notify.handler_list_id];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* All done with the info object */

<<<<<<< HEAD
	acpi_ut_delete_generic_state(notify_info);
=======
	acpi_ut_delete_generic_state(info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#if (!ACPI_REDUCED_HARDWARE)
/******************************************************************************
 *
 * FUNCTION:    acpi_ev_terminate
 *
 * PARAMETERS:  none
 *
 * RETURN:      none
 *
 * DESCRIPTION: Disable events and free memory allocated for table storage.
 *
 ******************************************************************************/

void acpi_ev_terminate(void)
{
	u32 i;
	acpi_status status;

	ACPI_FUNCTION_TRACE(ev_terminate);

	if (acpi_gbl_events_initialized) {
		/*
		 * Disable all event-related functionality. In all cases, on error,
		 * print a message but obviously we don't abort.
		 */

		/* Disable all fixed events */

		for (i = 0; i < ACPI_NUM_FIXED_EVENTS; i++) {
			status = acpi_disable_event(i, 0);
			if (ACPI_FAILURE(status)) {
				ACPI_ERROR((AE_INFO,
					    "Could not disable fixed event %u",
					    (u32) i));
			}
		}

		/* Disable all GPEs in all GPE blocks */

		status = acpi_ev_walk_gpe_list(acpi_hw_disable_gpe_block, NULL);
<<<<<<< HEAD

		/* Remove SCI handler */

		status = acpi_ev_remove_sci_handler();
		if (ACPI_FAILURE(status)) {
			ACPI_ERROR((AE_INFO, "Could not remove SCI handler"));
=======
		if (ACPI_FAILURE(status)) {
			ACPI_EXCEPTION((AE_INFO, status,
					"Could not disable GPEs in GPE block"));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		status = acpi_ev_remove_global_lock_handler();
		if (ACPI_FAILURE(status)) {
<<<<<<< HEAD
			ACPI_ERROR((AE_INFO,
				    "Could not remove Global Lock handler"));
		}
=======
			ACPI_EXCEPTION((AE_INFO, status,
					"Could not remove Global Lock handler"));
		}

		acpi_gbl_events_initialized = FALSE;
	}

	/* Remove SCI handlers */

	status = acpi_ev_remove_all_sci_handlers();
	if (ACPI_FAILURE(status)) {
		ACPI_ERROR((AE_INFO, "Could not remove SCI handler"));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* Deallocate all handler objects installed within GPE info structs */

	status = acpi_ev_walk_gpe_list(acpi_ev_delete_gpe_handlers, NULL);
<<<<<<< HEAD
=======
	if (ACPI_FAILURE(status)) {
		ACPI_EXCEPTION((AE_INFO, status,
				"Could not delete GPE handlers"));
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Return to original mode if necessary */

	if (acpi_gbl_original_mode == ACPI_SYS_MODE_LEGACY) {
		status = acpi_disable();
		if (ACPI_FAILURE(status)) {
			ACPI_WARNING((AE_INFO, "AcpiDisable failed"));
		}
	}
	return_VOID;
}

#endif				/* !ACPI_REDUCED_HARDWARE */
