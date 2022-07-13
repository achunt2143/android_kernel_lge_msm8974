<<<<<<< HEAD

=======
// SPDX-License-Identifier: BSD-3-Clause OR GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/******************************************************************************
 *
 * Module Name: exsystem - Interface to OS services
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
#include "acinterp.h"

#define _COMPONENT          ACPI_EXECUTER
ACPI_MODULE_NAME("exsystem")

/*******************************************************************************
 *
 * FUNCTION:    acpi_ex_system_wait_semaphore
 *
<<<<<<< HEAD
 * PARAMETERS:  Semaphore       - Semaphore to wait on
 *              Timeout         - Max time to wait
=======
 * PARAMETERS:  semaphore       - Semaphore to wait on
 *              timeout         - Max time to wait
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Implements a semaphore wait with a check to see if the
<<<<<<< HEAD
 *              semaphore is available immediately.  If it is not, the
=======
 *              semaphore is available immediately. If it is not, the
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *              interpreter is released before waiting.
 *
 ******************************************************************************/
acpi_status acpi_ex_system_wait_semaphore(acpi_semaphore semaphore, u16 timeout)
{
	acpi_status status;

	ACPI_FUNCTION_TRACE(ex_system_wait_semaphore);

	status = acpi_os_wait_semaphore(semaphore, 1, ACPI_DO_NOT_WAIT);
	if (ACPI_SUCCESS(status)) {
		return_ACPI_STATUS(status);
	}

	if (status == AE_TIME) {

		/* We must wait, so unlock the interpreter */

<<<<<<< HEAD
		acpi_ex_relinquish_interpreter();

=======
		acpi_ex_exit_interpreter();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		status = acpi_os_wait_semaphore(semaphore, 1, timeout);

		ACPI_DEBUG_PRINT((ACPI_DB_EXEC,
				  "*** Thread awake after blocking, %s\n",
				  acpi_format_exception(status)));

		/* Reacquire the interpreter */

<<<<<<< HEAD
		acpi_ex_reacquire_interpreter();
=======
		acpi_ex_enter_interpreter();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return_ACPI_STATUS(status);
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_ex_system_wait_mutex
 *
<<<<<<< HEAD
 * PARAMETERS:  Mutex           - Mutex to wait on
 *              Timeout         - Max time to wait
=======
 * PARAMETERS:  mutex           - Mutex to wait on
 *              timeout         - Max time to wait
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Implements a mutex wait with a check to see if the
<<<<<<< HEAD
 *              mutex is available immediately.  If it is not, the
=======
 *              mutex is available immediately. If it is not, the
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *              interpreter is released before waiting.
 *
 ******************************************************************************/

acpi_status acpi_ex_system_wait_mutex(acpi_mutex mutex, u16 timeout)
{
	acpi_status status;

	ACPI_FUNCTION_TRACE(ex_system_wait_mutex);

	status = acpi_os_acquire_mutex(mutex, ACPI_DO_NOT_WAIT);
	if (ACPI_SUCCESS(status)) {
		return_ACPI_STATUS(status);
	}

	if (status == AE_TIME) {

		/* We must wait, so unlock the interpreter */

<<<<<<< HEAD
		acpi_ex_relinquish_interpreter();

=======
		acpi_ex_exit_interpreter();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		status = acpi_os_acquire_mutex(mutex, timeout);

		ACPI_DEBUG_PRINT((ACPI_DB_EXEC,
				  "*** Thread awake after blocking, %s\n",
				  acpi_format_exception(status)));

		/* Reacquire the interpreter */

<<<<<<< HEAD
		acpi_ex_reacquire_interpreter();
=======
		acpi_ex_enter_interpreter();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return_ACPI_STATUS(status);
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_ex_system_do_stall
 *
<<<<<<< HEAD
 * PARAMETERS:  how_long        - The amount of time to stall,
=======
 * PARAMETERS:  how_long_us     - The amount of time to stall,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *                                in microseconds
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Suspend running thread for specified amount of time.
 *              Note: ACPI specification requires that Stall() does not
 *              relinquish the processor, and delays longer than 100 usec
<<<<<<< HEAD
 *              should use Sleep() instead.  We allow stalls up to 255 usec
=======
 *              should use Sleep() instead. We allow stalls up to 255 usec
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *              for compatibility with other interpreters and existing BIOSs.
 *
 ******************************************************************************/

<<<<<<< HEAD
acpi_status acpi_ex_system_do_stall(u32 how_long)
=======
acpi_status acpi_ex_system_do_stall(u32 how_long_us)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	acpi_status status = AE_OK;

	ACPI_FUNCTION_ENTRY();

<<<<<<< HEAD
	if (how_long > 255) {	/* 255 microseconds */
		/*
		 * Longer than 255 usec, this is an error
=======
	if (how_long_us > 255) {
		/*
		 * Longer than 255 microseconds, this is an error
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 *
		 * (ACPI specifies 100 usec as max, but this gives some slack in
		 * order to support existing BIOSs)
		 */
<<<<<<< HEAD
		ACPI_ERROR((AE_INFO, "Time parameter is too large (%u)",
			    how_long));
		status = AE_AML_OPERAND_VALUE;
	} else {
		acpi_os_stall(how_long);
=======
		ACPI_ERROR((AE_INFO,
			    "Time parameter is too large (%u)", how_long_us));
		status = AE_AML_OPERAND_VALUE;
	} else {
		if (how_long_us > 100) {
			ACPI_WARNING((AE_INFO,
				      "Time parameter %u us > 100 us violating ACPI spec, please fix the firmware.",
				      how_long_us));
		}
		acpi_os_stall(how_long_us);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return (status);
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_ex_system_do_sleep
 *
<<<<<<< HEAD
 * PARAMETERS:  how_long        - The amount of time to sleep,
=======
 * PARAMETERS:  how_long_ms     - The amount of time to sleep,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *                                in milliseconds
 *
 * RETURN:      None
 *
 * DESCRIPTION: Sleep the running thread for specified amount of time.
 *
 ******************************************************************************/

<<<<<<< HEAD
acpi_status acpi_ex_system_do_sleep(u64 how_long)
=======
acpi_status acpi_ex_system_do_sleep(u64 how_long_ms)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	ACPI_FUNCTION_ENTRY();

	/* Since this thread will sleep, we must release the interpreter */

<<<<<<< HEAD
	acpi_ex_relinquish_interpreter();
=======
	acpi_ex_exit_interpreter();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * For compatibility with other ACPI implementations and to prevent
	 * accidental deep sleeps, limit the sleep time to something reasonable.
	 */
<<<<<<< HEAD
	if (how_long > ACPI_MAX_SLEEP) {
		how_long = ACPI_MAX_SLEEP;
	}

	acpi_os_sleep(how_long);

	/* And now we must get the interpreter again */

	acpi_ex_reacquire_interpreter();
=======
	if (how_long_ms > ACPI_MAX_SLEEP) {
		how_long_ms = ACPI_MAX_SLEEP;
	}

	acpi_os_sleep(how_long_ms);

	/* And now we must get the interpreter again */

	acpi_ex_enter_interpreter();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return (AE_OK);
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_ex_system_signal_event
 *
 * PARAMETERS:  obj_desc        - The object descriptor for this op
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Provides an access point to perform synchronization operations
 *              within the AML.
 *
 ******************************************************************************/

acpi_status acpi_ex_system_signal_event(union acpi_operand_object * obj_desc)
{
	acpi_status status = AE_OK;

	ACPI_FUNCTION_TRACE(ex_system_signal_event);

	if (obj_desc) {
		status =
		    acpi_os_signal_semaphore(obj_desc->event.os_semaphore, 1);
	}

	return_ACPI_STATUS(status);
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_ex_system_wait_event
 *
 * PARAMETERS:  time_desc       - The 'time to delay' object descriptor
 *              obj_desc        - The object descriptor for this op
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Provides an access point to perform synchronization operations
<<<<<<< HEAD
 *              within the AML.  This operation is a request to wait for an
=======
 *              within the AML. This operation is a request to wait for an
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *              event.
 *
 ******************************************************************************/

acpi_status
acpi_ex_system_wait_event(union acpi_operand_object *time_desc,
			  union acpi_operand_object *obj_desc)
{
	acpi_status status = AE_OK;

	ACPI_FUNCTION_TRACE(ex_system_wait_event);

	if (obj_desc) {
		status =
		    acpi_ex_system_wait_semaphore(obj_desc->event.os_semaphore,
						  (u16) time_desc->integer.
						  value);
	}

	return_ACPI_STATUS(status);
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_ex_system_reset_event
 *
 * PARAMETERS:  obj_desc        - The object descriptor for this op
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Reset an event to a known state.
 *
 ******************************************************************************/

acpi_status acpi_ex_system_reset_event(union acpi_operand_object *obj_desc)
{
	acpi_status status = AE_OK;
	acpi_semaphore temp_semaphore;

	ACPI_FUNCTION_ENTRY();

	/*
	 * We are going to simply delete the existing semaphore and
	 * create a new one!
	 */
	status =
	    acpi_os_create_semaphore(ACPI_NO_UNIT_LIMIT, 0, &temp_semaphore);
	if (ACPI_SUCCESS(status)) {
		(void)acpi_os_delete_semaphore(obj_desc->event.os_semaphore);
		obj_desc->event.os_semaphore = temp_semaphore;
	}

	return (status);
}
