<<<<<<< HEAD

=======
// SPDX-License-Identifier: BSD-3-Clause OR GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/******************************************************************************
 *
 * Module Name: exregion - ACPI default op_region (address space) handlers
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
ACPI_MODULE_NAME("exregion")

/*******************************************************************************
 *
 * FUNCTION:    acpi_ex_system_memory_space_handler
 *
<<<<<<< HEAD
 * PARAMETERS:  Function            - Read or Write operation
 *              Address             - Where in the space to read or write
 *              bit_width           - Field width in bits (8, 16, or 32)
 *              Value               - Pointer to in or out value
=======
 * PARAMETERS:  function            - Read or Write operation
 *              address             - Where in the space to read or write
 *              bit_width           - Field width in bits (8, 16, or 32)
 *              value               - Pointer to in or out value
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *              handler_context     - Pointer to Handler's context
 *              region_context      - Pointer to context specific to the
 *                                    accessed region
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Handler for the System Memory address space (Op Region)
 *
 ******************************************************************************/
acpi_status
acpi_ex_system_memory_space_handler(u32 function,
				    acpi_physical_address address,
				    u32 bit_width,
				    u64 *value,
				    void *handler_context, void *region_context)
{
	acpi_status status = AE_OK;
	void *logical_addr_ptr = NULL;
	struct acpi_mem_space_context *mem_info = region_context;
<<<<<<< HEAD
=======
	struct acpi_mem_mapping *mm = mem_info->cur_mm;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32 length;
	acpi_size map_length;
	acpi_size page_boundary_map_length;
#ifdef ACPI_MISALIGNMENT_NOT_SUPPORTED
	u32 remainder;
#endif

	ACPI_FUNCTION_TRACE(ex_system_memory_space_handler);

	/* Validate and translate the bit width */

	switch (bit_width) {
	case 8:
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		length = 1;
		break;

	case 16:
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		length = 2;
		break;

	case 32:
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		length = 4;
		break;

	case 64:
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		length = 8;
		break;

	default:
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ACPI_ERROR((AE_INFO, "Invalid SystemMemory width %u",
			    bit_width));
		return_ACPI_STATUS(AE_AML_OPERAND_VALUE);
	}

#ifdef ACPI_MISALIGNMENT_NOT_SUPPORTED
	/*
	 * Hardware does not support non-aligned data transfers, we must verify
	 * the request.
	 */
	(void)acpi_ut_short_divide((u64) address, length, NULL, &remainder);
	if (remainder != 0) {
		return_ACPI_STATUS(AE_AML_ALIGNMENT);
	}
#endif

	/*
	 * Does the request fit into the cached memory mapping?
	 * Is 1) Address below the current mapping? OR
	 *    2) Address beyond the current mapping?
	 */
<<<<<<< HEAD
	if ((address < mem_info->mapped_physical_address) ||
	    (((u64) address + length) > ((u64)
					 mem_info->mapped_physical_address +
					 mem_info->mapped_length))) {
		/*
		 * The request cannot be resolved by the current memory mapping;
		 * Delete the existing mapping and create a new one.
		 */
		if (mem_info->mapped_length) {

			/* Valid mapping, delete it */

			acpi_os_unmap_memory(mem_info->mapped_logical_address,
					     mem_info->mapped_length);
		}

		/*
		 * Attempt to map from the requested address to the end of the region.
		 * However, we will never map more than one page, nor will we cross
		 * a page boundary.
=======
	if (!mm || (address < mm->physical_address) ||
	    ((u64) address + length > (u64) mm->physical_address + mm->length)) {
		/*
		 * The request cannot be resolved by the current memory mapping.
		 *
		 * Look for an existing saved mapping covering the address range
		 * at hand.  If found, save it as the current one and carry out
		 * the access.
		 */
		for (mm = mem_info->first_mm; mm; mm = mm->next_mm) {
			if (mm == mem_info->cur_mm)
				continue;

			if (address < mm->physical_address)
				continue;

			if ((u64) address + length >
					(u64) mm->physical_address + mm->length)
				continue;

			mem_info->cur_mm = mm;
			goto access;
		}

		/* Create a new mappings list entry */
		mm = ACPI_ALLOCATE_ZEROED(sizeof(*mm));
		if (!mm) {
			ACPI_ERROR((AE_INFO,
				    "Unable to save memory mapping at 0x%8.8X%8.8X, size %u",
				    ACPI_FORMAT_UINT64(address), length));
			return_ACPI_STATUS(AE_NO_MEMORY);
		}

		/*
		 * October 2009: Attempt to map from the requested address to the
		 * end of the region. However, we will never map more than one
		 * page, nor will we cross a page boundary.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 */
		map_length = (acpi_size)
		    ((mem_info->address + mem_info->length) - address);

		/*
		 * If mapping the entire remaining portion of the region will cross
		 * a page boundary, just map up to the page boundary, do not cross.
		 * On some systems, crossing a page boundary while mapping regions
		 * can cause warnings if the pages have different attributes
<<<<<<< HEAD
		 * due to resource management
		 */
		page_boundary_map_length =
		    ACPI_ROUND_UP(address, ACPI_DEFAULT_PAGE_SIZE) - address;

		if (!page_boundary_map_length) {
=======
		 * due to resource management.
		 *
		 * This has the added benefit of constraining a single mapping to
		 * one page, which is similar to the original code that used a 4k
		 * maximum window.
		 */
		page_boundary_map_length = (acpi_size)
		    (ACPI_ROUND_UP(address, ACPI_DEFAULT_PAGE_SIZE) - address);
		if (page_boundary_map_length == 0) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			page_boundary_map_length = ACPI_DEFAULT_PAGE_SIZE;
		}

		if (map_length > page_boundary_map_length) {
			map_length = page_boundary_map_length;
		}

		/* Create a new mapping starting at the address given */

<<<<<<< HEAD
		mem_info->mapped_logical_address = acpi_os_map_memory((acpi_physical_address) address, map_length);
		if (!mem_info->mapped_logical_address) {
			ACPI_ERROR((AE_INFO,
				    "Could not map memory at 0x%8.8X%8.8X, size %u",
				    ACPI_FORMAT_NATIVE_UINT(address),
				    (u32) map_length));
			mem_info->mapped_length = 0;
=======
		logical_addr_ptr = acpi_os_map_memory(address, map_length);
		if (!logical_addr_ptr) {
			ACPI_ERROR((AE_INFO,
				    "Could not map memory at 0x%8.8X%8.8X, size %u",
				    ACPI_FORMAT_UINT64(address),
				    (u32)map_length));
			ACPI_FREE(mm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return_ACPI_STATUS(AE_NO_MEMORY);
		}

		/* Save the physical address and mapping size */

<<<<<<< HEAD
		mem_info->mapped_physical_address = address;
		mem_info->mapped_length = map_length;
	}

=======
		mm->logical_address = logical_addr_ptr;
		mm->physical_address = address;
		mm->length = map_length;

		/*
		 * Add the new entry to the mappigs list and save it as the
		 * current mapping.
		 */
		mm->next_mm = mem_info->first_mm;
		mem_info->first_mm = mm;

		mem_info->cur_mm = mm;
	}

access:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Generate a logical pointer corresponding to the address we want to
	 * access
	 */
<<<<<<< HEAD
	logical_addr_ptr = mem_info->mapped_logical_address +
	    ((u64) address - (u64) mem_info->mapped_physical_address);

	ACPI_DEBUG_PRINT((ACPI_DB_INFO,
			  "System-Memory (width %u) R/W %u Address=%8.8X%8.8X\n",
			  bit_width, function,
			  ACPI_FORMAT_NATIVE_UINT(address)));
=======
	logical_addr_ptr = mm->logical_address +
		((u64) address - (u64) mm->physical_address);

	ACPI_DEBUG_PRINT((ACPI_DB_INFO,
			  "System-Memory (width %u) R/W %u Address=%8.8X%8.8X\n",
			  bit_width, function, ACPI_FORMAT_UINT64(address)));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Perform the memory read or write
	 *
	 * Note: For machines that do not support non-aligned transfers, the target
<<<<<<< HEAD
	 * address was checked for alignment above.  We do not attempt to break the
=======
	 * address was checked for alignment above. We do not attempt to break the
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * transfer up into smaller (byte-size) chunks because the AML specifically
	 * asked for a transfer width that the hardware may require.
	 */
	switch (function) {
	case ACPI_READ:

		*value = 0;
		switch (bit_width) {
		case 8:
<<<<<<< HEAD
			*value = (u64) ACPI_GET8(logical_addr_ptr);
			break;

		case 16:
			*value = (u64) ACPI_GET16(logical_addr_ptr);
			break;

		case 32:
			*value = (u64) ACPI_GET32(logical_addr_ptr);
			break;

		case 64:
			*value = (u64) ACPI_GET64(logical_addr_ptr);
			break;

		default:
			/* bit_width was already validated */
=======

			*value = (u64)ACPI_GET8(logical_addr_ptr);
			break;

		case 16:

			*value = (u64)ACPI_GET16(logical_addr_ptr);
			break;

		case 32:

			*value = (u64)ACPI_GET32(logical_addr_ptr);
			break;

		case 64:

			*value = (u64)ACPI_GET64(logical_addr_ptr);
			break;

		default:

			/* bit_width was already validated */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}
		break;

	case ACPI_WRITE:

		switch (bit_width) {
		case 8:
<<<<<<< HEAD
			ACPI_SET8(logical_addr_ptr) = (u8) * value;
			break;

		case 16:
			ACPI_SET16(logical_addr_ptr) = (u16) * value;
			break;

		case 32:
			ACPI_SET32(logical_addr_ptr) = (u32) * value;
			break;

		case 64:
			ACPI_SET64(logical_addr_ptr) = (u64) * value;
			break;

		default:
			/* bit_width was already validated */
=======

			ACPI_SET8(logical_addr_ptr, *value);
			break;

		case 16:

			ACPI_SET16(logical_addr_ptr, *value);
			break;

		case 32:

			ACPI_SET32(logical_addr_ptr, *value);
			break;

		case 64:

			ACPI_SET64(logical_addr_ptr, *value);
			break;

		default:

			/* bit_width was already validated */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}
		break;

	default:
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		status = AE_BAD_PARAMETER;
		break;
	}

	return_ACPI_STATUS(status);
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_ex_system_io_space_handler
 *
<<<<<<< HEAD
 * PARAMETERS:  Function            - Read or Write operation
 *              Address             - Where in the space to read or write
 *              bit_width           - Field width in bits (8, 16, or 32)
 *              Value               - Pointer to in or out value
=======
 * PARAMETERS:  function            - Read or Write operation
 *              address             - Where in the space to read or write
 *              bit_width           - Field width in bits (8, 16, or 32)
 *              value               - Pointer to in or out value
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *              handler_context     - Pointer to Handler's context
 *              region_context      - Pointer to context specific to the
 *                                    accessed region
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Handler for the System IO address space (Op Region)
 *
 ******************************************************************************/

acpi_status
acpi_ex_system_io_space_handler(u32 function,
				acpi_physical_address address,
				u32 bit_width,
				u64 *value,
				void *handler_context, void *region_context)
{
	acpi_status status = AE_OK;
	u32 value32;

	ACPI_FUNCTION_TRACE(ex_system_io_space_handler);

	ACPI_DEBUG_PRINT((ACPI_DB_INFO,
			  "System-IO (width %u) R/W %u Address=%8.8X%8.8X\n",
<<<<<<< HEAD
			  bit_width, function,
			  ACPI_FORMAT_NATIVE_UINT(address)));
=======
			  bit_width, function, ACPI_FORMAT_UINT64(address)));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Decode the function parameter */

	switch (function) {
	case ACPI_READ:

<<<<<<< HEAD
		status = acpi_hw_read_port((acpi_io_address) address,
=======
		status = acpi_hw_read_port((acpi_io_address)address,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					   &value32, bit_width);
		*value = value32;
		break;

	case ACPI_WRITE:

<<<<<<< HEAD
		status = acpi_hw_write_port((acpi_io_address) address,
					    (u32) * value, bit_width);
		break;

	default:
=======
		status = acpi_hw_write_port((acpi_io_address)address,
					    (u32)*value, bit_width);
		break;

	default:

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		status = AE_BAD_PARAMETER;
		break;
	}

	return_ACPI_STATUS(status);
}

<<<<<<< HEAD
=======
#ifdef ACPI_PCI_CONFIGURED
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*******************************************************************************
 *
 * FUNCTION:    acpi_ex_pci_config_space_handler
 *
<<<<<<< HEAD
 * PARAMETERS:  Function            - Read or Write operation
 *              Address             - Where in the space to read or write
 *              bit_width           - Field width in bits (8, 16, or 32)
 *              Value               - Pointer to in or out value
=======
 * PARAMETERS:  function            - Read or Write operation
 *              address             - Where in the space to read or write
 *              bit_width           - Field width in bits (8, 16, or 32)
 *              value               - Pointer to in or out value
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *              handler_context     - Pointer to Handler's context
 *              region_context      - Pointer to context specific to the
 *                                    accessed region
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Handler for the PCI Config address space (Op Region)
 *
 ******************************************************************************/

acpi_status
acpi_ex_pci_config_space_handler(u32 function,
				 acpi_physical_address address,
				 u32 bit_width,
				 u64 *value,
				 void *handler_context, void *region_context)
{
	acpi_status status = AE_OK;
	struct acpi_pci_id *pci_id;
	u16 pci_register;

	ACPI_FUNCTION_TRACE(ex_pci_config_space_handler);

	/*
	 *  The arguments to acpi_os(Read|Write)pci_configuration are:
	 *
	 *  pci_segment is the PCI bus segment range 0-31
	 *  pci_bus     is the PCI bus number range 0-255
	 *  pci_device  is the PCI device number range 0-31
	 *  pci_function is the PCI device function number
	 *  pci_register is the Config space register range 0-255 bytes
	 *
<<<<<<< HEAD
	 *  Value - input value for write, output address for read
=======
	 *  value - input value for write, output address for read
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 *
	 */
	pci_id = (struct acpi_pci_id *)region_context;
	pci_register = (u16) (u32) address;

	ACPI_DEBUG_PRINT((ACPI_DB_INFO,
<<<<<<< HEAD
			  "Pci-Config %u (%u) Seg(%04x) Bus(%04x) Dev(%04x) Func(%04x) Reg(%04x)\n",
=======
			  "Pci-Config %u (%u) Seg(%04x) Bus(%04x) "
			  "Dev(%04x) Func(%04x) Reg(%04x)\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			  function, bit_width, pci_id->segment, pci_id->bus,
			  pci_id->device, pci_id->function, pci_register));

	switch (function) {
	case ACPI_READ:

<<<<<<< HEAD
		status = acpi_os_read_pci_configuration(pci_id, pci_register,
							value, bit_width);
=======
		*value = 0;
		status =
		    acpi_os_read_pci_configuration(pci_id, pci_register, value,
						   bit_width);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;

	case ACPI_WRITE:

<<<<<<< HEAD
		status = acpi_os_write_pci_configuration(pci_id, pci_register,
							 *value, bit_width);
=======
		status =
		    acpi_os_write_pci_configuration(pci_id, pci_register,
						    *value, bit_width);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;

	default:

		status = AE_BAD_PARAMETER;
		break;
	}

	return_ACPI_STATUS(status);
}
<<<<<<< HEAD
=======
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*******************************************************************************
 *
 * FUNCTION:    acpi_ex_cmos_space_handler
 *
<<<<<<< HEAD
 * PARAMETERS:  Function            - Read or Write operation
 *              Address             - Where in the space to read or write
 *              bit_width           - Field width in bits (8, 16, or 32)
 *              Value               - Pointer to in or out value
=======
 * PARAMETERS:  function            - Read or Write operation
 *              address             - Where in the space to read or write
 *              bit_width           - Field width in bits (8, 16, or 32)
 *              value               - Pointer to in or out value
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *              handler_context     - Pointer to Handler's context
 *              region_context      - Pointer to context specific to the
 *                                    accessed region
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Handler for the CMOS address space (Op Region)
 *
 ******************************************************************************/

acpi_status
acpi_ex_cmos_space_handler(u32 function,
			   acpi_physical_address address,
			   u32 bit_width,
			   u64 *value,
			   void *handler_context, void *region_context)
{
	acpi_status status = AE_OK;

	ACPI_FUNCTION_TRACE(ex_cmos_space_handler);

	return_ACPI_STATUS(status);
}

<<<<<<< HEAD
=======
#ifdef ACPI_PCI_CONFIGURED
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*******************************************************************************
 *
 * FUNCTION:    acpi_ex_pci_bar_space_handler
 *
<<<<<<< HEAD
 * PARAMETERS:  Function            - Read or Write operation
 *              Address             - Where in the space to read or write
 *              bit_width           - Field width in bits (8, 16, or 32)
 *              Value               - Pointer to in or out value
=======
 * PARAMETERS:  function            - Read or Write operation
 *              address             - Where in the space to read or write
 *              bit_width           - Field width in bits (8, 16, or 32)
 *              value               - Pointer to in or out value
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *              handler_context     - Pointer to Handler's context
 *              region_context      - Pointer to context specific to the
 *                                    accessed region
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Handler for the PCI bar_target address space (Op Region)
 *
 ******************************************************************************/

acpi_status
acpi_ex_pci_bar_space_handler(u32 function,
			      acpi_physical_address address,
			      u32 bit_width,
			      u64 *value,
			      void *handler_context, void *region_context)
{
	acpi_status status = AE_OK;

	ACPI_FUNCTION_TRACE(ex_pci_bar_space_handler);

	return_ACPI_STATUS(status);
}
<<<<<<< HEAD
=======
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*******************************************************************************
 *
 * FUNCTION:    acpi_ex_data_table_space_handler
 *
<<<<<<< HEAD
 * PARAMETERS:  Function            - Read or Write operation
 *              Address             - Where in the space to read or write
 *              bit_width           - Field width in bits (8, 16, or 32)
 *              Value               - Pointer to in or out value
=======
 * PARAMETERS:  function            - Read or Write operation
 *              address             - Where in the space to read or write
 *              bit_width           - Field width in bits (8, 16, or 32)
 *              value               - Pointer to in or out value
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *              handler_context     - Pointer to Handler's context
 *              region_context      - Pointer to context specific to the
 *                                    accessed region
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Handler for the Data Table address space (Op Region)
 *
 ******************************************************************************/

acpi_status
acpi_ex_data_table_space_handler(u32 function,
				 acpi_physical_address address,
				 u32 bit_width,
				 u64 *value,
				 void *handler_context, void *region_context)
{
<<<<<<< HEAD
	ACPI_FUNCTION_TRACE(ex_data_table_space_handler);

=======
	struct acpi_data_table_mapping *mapping;
	char *pointer;

	ACPI_FUNCTION_TRACE(ex_data_table_space_handler);

	mapping = (struct acpi_data_table_mapping *) region_context;
	pointer = ACPI_CAST_PTR(char, mapping->pointer) +
	    (address - ACPI_PTR_TO_PHYSADDR(mapping->pointer));

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Perform the memory read or write. The bit_width was already
	 * validated.
	 */
	switch (function) {
	case ACPI_READ:

<<<<<<< HEAD
		ACPI_MEMCPY(ACPI_CAST_PTR(char, value),
			    ACPI_PHYSADDR_TO_PTR(address),
			    ACPI_DIV_8(bit_width));
=======
		memcpy(ACPI_CAST_PTR(char, value), pointer,
		       ACPI_DIV_8(bit_width));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;

	case ACPI_WRITE:

<<<<<<< HEAD
		ACPI_MEMCPY(ACPI_PHYSADDR_TO_PTR(address),
			    ACPI_CAST_PTR(char, value), ACPI_DIV_8(bit_width));
=======
		memcpy(pointer, ACPI_CAST_PTR(char, value),
		       ACPI_DIV_8(bit_width));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;

	default:

		return_ACPI_STATUS(AE_BAD_PARAMETER);
	}

	return_ACPI_STATUS(AE_OK);
}
