<<<<<<< HEAD
=======
/* SPDX-License-Identifier: BSD-3-Clause OR GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/******************************************************************************
 *
 * Name: actbl3.h - ACPI Table Definitions
 *
<<<<<<< HEAD
 *****************************************************************************/

/*
 * Copyright (C) 2000 - 2011, Intel Corp.
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
#ifndef __ACTBL3_H__
#define __ACTBL3_H__

/*******************************************************************************
 *
<<<<<<< HEAD
 * Additional ACPI Tables (3)
=======
 * Additional ACPI Tables
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * These tables are not consumed directly by the ACPICA subsystem, but are
 * included here to support device drivers and the AML disassembler.
 *
<<<<<<< HEAD
 * The tables in this file are fully defined within the ACPI specification.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 ******************************************************************************/

/*
 * Values for description table header signatures for tables defined in this
 * file. Useful because they make it more difficult to inadvertently type in
 * the wrong signature.
 */
<<<<<<< HEAD
#define ACPI_SIG_BGRT           "BGRT"	/* Boot Graphics Resource Table */
#define ACPI_SIG_DRTM           "DRTM"	/* Dynamic Root of Trust for Measurement table */
#define ACPI_SIG_FPDT           "FPDT"	/* Firmware Performance Data Table */
#define ACPI_SIG_GTDT           "GTDT"	/* Generic Timer Description Table */
#define ACPI_SIG_MPST           "MPST"	/* Memory Power State Table */
#define ACPI_SIG_PCCT           "PCCT"	/* Platform Communications Channel Table */
#define ACPI_SIG_PMTT           "PMTT"	/* Platform Memory Topology Table */
#define ACPI_SIG_RASF           "RASF"	/* RAS Feature table */

#define ACPI_SIG_S3PT           "S3PT"	/* S3 Performance (sub)Table */
#define ACPI_SIG_PCCS           "PCC"	/* PCC Shared Memory Region */

/* Reserved table signatures */

#define ACPI_SIG_CSRT           "CSRT"	/* Core System Resources Table */
#define ACPI_SIG_DBG2           "DBG2"	/* Debug Port table 2 */
#define ACPI_SIG_MATR           "MATR"	/* Memory Address Translation Table */
#define ACPI_SIG_MSDM           "MSDM"	/* Microsoft Data Management Table */
#define ACPI_SIG_WPBT           "WPBT"	/* Windows Platform Binary Table */
=======
#define ACPI_SIG_SLIC           "SLIC"	/* Software Licensing Description Table */
#define ACPI_SIG_SLIT           "SLIT"	/* System Locality Distance Information Table */
#define ACPI_SIG_SPCR           "SPCR"	/* Serial Port Console Redirection table */
#define ACPI_SIG_SPMI           "SPMI"	/* Server Platform Management Interface table */
#define ACPI_SIG_SRAT           "SRAT"	/* System Resource Affinity Table */
#define ACPI_SIG_STAO           "STAO"	/* Status Override table */
#define ACPI_SIG_TCPA           "TCPA"	/* Trusted Computing Platform Alliance table */
#define ACPI_SIG_TPM2           "TPM2"	/* Trusted Platform Module 2.0 H/W interface table */
#define ACPI_SIG_UEFI           "UEFI"	/* Uefi Boot Optimization Table */
#define ACPI_SIG_VIOT           "VIOT"	/* Virtual I/O Translation Table */
#define ACPI_SIG_WAET           "WAET"	/* Windows ACPI Emulated devices Table */
#define ACPI_SIG_WDAT           "WDAT"	/* Watchdog Action Table */
#define ACPI_SIG_WDDT           "WDDT"	/* Watchdog Timer Description Table */
#define ACPI_SIG_WDRT           "WDRT"	/* Watchdog Resource Table */
#define ACPI_SIG_WPBT           "WPBT"	/* Windows Platform Binary Table */
#define ACPI_SIG_WSMT           "WSMT"	/* Windows SMM Security Mitigations Table */
#define ACPI_SIG_XENV           "XENV"	/* Xen Environment table */
#define ACPI_SIG_XXXX           "XXXX"	/* Intermediate AML header for ASL/ASL+ converter */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * All tables must be byte-packed to match the ACPI specification, since
 * the tables are provided by the system BIOS.
 */
#pragma pack(1)

/*
<<<<<<< HEAD
 * Note about bitfields: The u8 type is used for bitfields in ACPI tables.
 * This is the only type that is even remotely portable. Anything else is not
 * portable, so do not use any other bitfield types.
=======
 * Note: C bitfields are not used for this reason:
 *
 * "Bitfields are great and easy to read, but unfortunately the C language
 * does not specify the layout of bitfields in memory, which means they are
 * essentially useless for dealing with packed data in on-disk formats or
 * binary wire protocols." (Or ACPI tables and buffers.) "If you ask me,
 * this decision was a design error in C. Ritchie could have picked an order
 * and stuck with it." Norman Ramsey.
 * See http://stackoverflow.com/a/1053662/41661
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

/*******************************************************************************
 *
<<<<<<< HEAD
 * BGRT - Boot Graphics Resource Table (ACPI 5.0)
 *        Version 1
 *
 ******************************************************************************/

struct acpi_table_bgrt {
	struct acpi_table_header header;	/* Common ACPI table header */
	u16 version;
	u8 status;
	u8 image_type;
	u64 image_address;
	u32 image_offset_x;
	u32 image_offset_y;
=======
 * SLIC - Software Licensing Description Table
 *
 * Conforms to "Microsoft Software Licensing Tables (SLIC and MSDM)",
 * November 29, 2011. Copyright 2011 Microsoft
 *
 ******************************************************************************/

/* Basic SLIC table is only the common ACPI header */

struct acpi_table_slic {
	struct acpi_table_header header;	/* Common ACPI table header */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*******************************************************************************
 *
<<<<<<< HEAD
 * DRTM - Dynamic Root of Trust for Measurement table
 *
 ******************************************************************************/

struct acpi_table_drtm {
	struct acpi_table_header header;	/* Common ACPI table header */
	u64 entry_base_address;
	u64 entry_length;
	u32 entry_address32;
	u64 entry_address64;
	u64 exit_address;
	u64 log_area_address;
	u32 log_area_length;
	u64 arch_dependent_address;
	u32 flags;
};

/* 1) Validated Tables List */

struct acpi_drtm_vtl_list {
	u32 validated_table_list_count;
};

/* 2) Resources List */

struct acpi_drtm_resource_list {
	u32 resource_list_count;
};

/* 3) Platform-specific Identifiers List */

struct acpi_drtm_id_list {
	u32 id_list_count;
};

/*******************************************************************************
 *
 * FPDT - Firmware Performance Data Table (ACPI 5.0)
=======
 * SLIT - System Locality Distance Information Table
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *        Version 1
 *
 ******************************************************************************/

<<<<<<< HEAD
struct acpi_table_fpdt {
	struct acpi_table_header header;	/* Common ACPI table header */
};

/* FPDT subtable header */

struct acpi_fpdt_header {
	u16 type;
	u8 length;
	u8 revision;
};

/* Values for Type field above */

enum acpi_fpdt_type {
	ACPI_FPDT_TYPE_BOOT = 0,
	ACPI_FPDT_TYPE_S3PERF = 1,
};

/*
 * FPDT subtables
 */

/* 0: Firmware Basic Boot Performance Record */

struct acpi_fpdt_boot {
	struct acpi_fpdt_header header;
	u8 reserved[4];
	u64 reset_end;
	u64 load_start;
	u64 startup_start;
	u64 exit_services_entry;
	u64 exit_services_exit;
};

/* 1: S3 Performance Table Pointer Record */

struct acpi_fpdt_s3pt_ptr {
	struct acpi_fpdt_header header;
	u8 reserved[4];
	u64 address;
};

/*
 * S3PT - S3 Performance Table. This table is pointed to by the
 * FPDT S3 Pointer Record above.
 */
struct acpi_table_s3pt {
	u8 signature[4];	/* "S3PT" */
	u32 length;
};

/*
 * S3PT Subtables
 */
struct acpi_s3pt_header {
	u16 type;
	u8 length;
	u8 revision;
};

/* Values for Type field above */

enum acpi_s3pt_type {
	ACPI_S3PT_TYPE_RESUME = 0,
	ACPI_S3PT_TYPE_SUSPEND = 1,
};

struct acpi_s3pt_resume {
	struct acpi_s3pt_header header;
	u32 resume_count;
	u64 full_resume;
	u64 average_resume;
};

struct acpi_s3pt_suspend {
	struct acpi_s3pt_header header;
	u64 suspend_start;
	u64 suspend_end;
=======
struct acpi_table_slit {
	struct acpi_table_header header;	/* Common ACPI table header */
	u64 locality_count;
	u8 entry[];				/* Real size = localities^2 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*******************************************************************************
 *
<<<<<<< HEAD
 * GTDT - Generic Timer Description Table (ACPI 5.0)
 *        Version 1
 *
 ******************************************************************************/

struct acpi_table_gtdt {
	struct acpi_table_header header;	/* Common ACPI table header */
	u64 address;
	u32 flags;
	u32 secure_pl1_interrupt;
	u32 secure_pl1_flags;
	u32 non_secure_pl1_interrupt;
	u32 non_secure_pl1_flags;
	u32 virtual_timer_interrupt;
	u32 virtual_timer_flags;
	u32 non_secure_pl2_interrupt;
	u32 non_secure_pl2_flags;
};

/* Values for Flags field above */

#define ACPI_GTDT_MAPPED_BLOCK_PRESENT      1

/* Values for all "TimerFlags" fields above */

#define ACPI_GTDT_INTERRUPT_MODE            1
#define ACPI_GTDT_INTERRUPT_POLARITY        2

/*******************************************************************************
 *
 * MPST - Memory Power State Table (ACPI 5.0)
 *        Version 1
 *
 ******************************************************************************/

#define ACPI_MPST_CHANNEL_INFO \
	u16                             reserved1; \
	u8                              channel_id; \
	u8                              reserved2; \
	u16                             power_node_count;

/* Main table */

struct acpi_table_mpst {
	struct acpi_table_header header;	/* Common ACPI table header */
	 ACPI_MPST_CHANNEL_INFO	/* Platform Communication Channel */
};

/* Memory Platform Communication Channel Info */

struct acpi_mpst_channel {
	ACPI_MPST_CHANNEL_INFO	/* Platform Communication Channel */
};

/* Memory Power Node Structure */

struct acpi_mpst_power_node {
	u8 flags;
	u8 reserved1;
	u16 node_id;
	u32 length;
	u64 range_address;
	u64 range_length;
	u8 num_power_states;
	u8 num_physical_components;
	u16 reserved2;
};

/* Values for Flags field above */

#define ACPI_MPST_ENABLED               1
#define ACPI_MPST_POWER_MANAGED         2
#define ACPI_MPST_HOT_PLUG_CAPABLE      4

/* Memory Power State Structure (follows POWER_NODE above) */

struct acpi_mpst_power_state {
	u8 power_state;
	u8 info_index;
};

/* Physical Component ID Structure (follows POWER_STATE above) */

struct acpi_mpst_component {
	u16 component_id;
};

/* Memory Power State Characteristics Structure (follows all POWER_NODEs) */

struct acpi_mpst_data_hdr {
	u16 characteristics_count;
};

struct acpi_mpst_power_data {
	u8 revision;
	u8 flags;
	u16 reserved1;
	u32 average_power;
	u32 power_saving;
	u64 exit_latency;
	u64 reserved2;
};

/* Values for Flags field above */

#define ACPI_MPST_PRESERVE              1
#define ACPI_MPST_AUTOENTRY             2
#define ACPI_MPST_AUTOEXIT              4

/* Shared Memory Region (not part of an ACPI table) */

struct acpi_mpst_shared {
	u32 signature;
	u16 pcc_command;
	u16 pcc_status;
	u16 command_register;
	u16 status_register;
	u16 power_state_id;
	u16 power_node_id;
	u64 energy_consumed;
	u64 average_power;
=======
 * SPCR - Serial Port Console Redirection table
 *        Version 2
 *
 * Conforms to "Serial Port Console Redirection Table",
 * Version 1.03, August 10, 2015
 *
 ******************************************************************************/

struct acpi_table_spcr {
	struct acpi_table_header header;	/* Common ACPI table header */
	u8 interface_type;	/* 0=full 16550, 1=subset of 16550 */
	u8 reserved[3];
	struct acpi_generic_address serial_port;
	u8 interrupt_type;
	u8 pc_interrupt;
	u32 interrupt;
	u8 baud_rate;
	u8 parity;
	u8 stop_bits;
	u8 flow_control;
	u8 terminal_type;
	u8 reserved1;
	u16 pci_device_id;
	u16 pci_vendor_id;
	u8 pci_bus;
	u8 pci_device;
	u8 pci_function;
	u32 pci_flags;
	u8 pci_segment;
	u32 reserved2;
};

/* Masks for pci_flags field above */

#define ACPI_SPCR_DO_NOT_DISABLE    (1)

/* Values for Interface Type: See the definition of the DBG2 table */

/*******************************************************************************
 *
 * SPMI - Server Platform Management Interface table
 *        Version 5
 *
 * Conforms to "Intelligent Platform Management Interface Specification
 * Second Generation v2.0", Document Revision 1.0, February 12, 2004 with
 * June 12, 2009 markup.
 *
 ******************************************************************************/

struct acpi_table_spmi {
	struct acpi_table_header header;	/* Common ACPI table header */
	u8 interface_type;
	u8 reserved;		/* Must be 1 */
	u16 spec_revision;	/* Version of IPMI */
	u8 interrupt_type;
	u8 gpe_number;		/* GPE assigned */
	u8 reserved1;
	u8 pci_device_flag;
	u32 interrupt;
	struct acpi_generic_address ipmi_register;
	u8 pci_segment;
	u8 pci_bus;
	u8 pci_device;
	u8 pci_function;
	u8 reserved2;
};

/* Values for interface_type above */

enum acpi_spmi_interface_types {
	ACPI_SPMI_NOT_USED = 0,
	ACPI_SPMI_KEYBOARD = 1,
	ACPI_SPMI_SMI = 2,
	ACPI_SPMI_BLOCK_TRANSFER = 3,
	ACPI_SPMI_SMBUS = 4,
	ACPI_SPMI_RESERVED = 5	/* 5 and above are reserved */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*******************************************************************************
 *
<<<<<<< HEAD
 * PCCT - Platform Communications Channel Table (ACPI 5.0)
 *        Version 1
 *
 ******************************************************************************/

struct acpi_table_pcct {
	struct acpi_table_header header;	/* Common ACPI table header */
	u32 flags;
	u32 latency;
	u32 reserved;
};

/* Values for Flags field above */

#define ACPI_PCCT_DOORBELL              1

/*
 * PCCT subtables
 */

/* 0: Generic Communications Subspace */

struct acpi_pcct_subspace {
	struct acpi_subtable_header header;
	u8 reserved[6];
	u64 base_address;
	u64 length;
	struct acpi_generic_address doorbell_register;
	u64 preserve_mask;
	u64 write_mask;
};

/*
 * PCC memory structures (not part of the ACPI table)
 */

/* Shared Memory Region */

struct acpi_pcct_shared_memory {
	u32 signature;
	u16 command;
	u16 status;
=======
 * SRAT - System Resource Affinity Table
 *        Version 3
 *
 ******************************************************************************/

struct acpi_table_srat {
	struct acpi_table_header header;	/* Common ACPI table header */
	u32 table_revision;	/* Must be value '1' */
	u64 reserved;		/* Reserved, must be zero */
};

/* Values for subtable type in struct acpi_subtable_header */

enum acpi_srat_type {
	ACPI_SRAT_TYPE_CPU_AFFINITY = 0,
	ACPI_SRAT_TYPE_MEMORY_AFFINITY = 1,
	ACPI_SRAT_TYPE_X2APIC_CPU_AFFINITY = 2,
	ACPI_SRAT_TYPE_GICC_AFFINITY = 3,
	ACPI_SRAT_TYPE_GIC_ITS_AFFINITY = 4,	/* ACPI 6.2 */
	ACPI_SRAT_TYPE_GENERIC_AFFINITY = 5,	/* ACPI 6.3 */
	ACPI_SRAT_TYPE_GENERIC_PORT_AFFINITY = 6,	/* ACPI 6.4 */
	ACPI_SRAT_TYPE_RESERVED = 7	/* 7 and greater are reserved */
};

/*
 * SRAT Subtables, correspond to Type in struct acpi_subtable_header
 */

/* 0: Processor Local APIC/SAPIC Affinity */

struct acpi_srat_cpu_affinity {
	struct acpi_subtable_header header;
	u8 proximity_domain_lo;
	u8 apic_id;
	u32 flags;
	u8 local_sapic_eid;
	u8 proximity_domain_hi[3];
	u32 clock_domain;
};

/* Flags */

#define ACPI_SRAT_CPU_USE_AFFINITY  (1)	/* 00: Use affinity structure */

/* 1: Memory Affinity */

struct acpi_srat_mem_affinity {
	struct acpi_subtable_header header;
	u32 proximity_domain;
	u16 reserved;		/* Reserved, must be zero */
	u64 base_address;
	u64 length;
	u32 reserved1;
	u32 flags;
	u64 reserved2;		/* Reserved, must be zero */
};

/* Flags */

#define ACPI_SRAT_MEM_ENABLED       (1)	/* 00: Use affinity structure */
#define ACPI_SRAT_MEM_HOT_PLUGGABLE (1<<1)	/* 01: Memory region is hot pluggable */
#define ACPI_SRAT_MEM_NON_VOLATILE  (1<<2)	/* 02: Memory region is non-volatile */

/* 2: Processor Local X2_APIC Affinity (ACPI 4.0) */

struct acpi_srat_x2apic_cpu_affinity {
	struct acpi_subtable_header header;
	u16 reserved;		/* Reserved, must be zero */
	u32 proximity_domain;
	u32 apic_id;
	u32 flags;
	u32 clock_domain;
	u32 reserved2;
};

/* Flags for struct acpi_srat_cpu_affinity and struct acpi_srat_x2apic_cpu_affinity */

#define ACPI_SRAT_CPU_ENABLED       (1)	/* 00: Use affinity structure */

/* 3: GICC Affinity (ACPI 5.1) */

struct acpi_srat_gicc_affinity {
	struct acpi_subtable_header header;
	u32 proximity_domain;
	u32 acpi_processor_uid;
	u32 flags;
	u32 clock_domain;
};

/* Flags for struct acpi_srat_gicc_affinity */

#define ACPI_SRAT_GICC_ENABLED     (1)	/* 00: Use affinity structure */

/* 4: GCC ITS Affinity (ACPI 6.2) */

struct acpi_srat_gic_its_affinity {
	struct acpi_subtable_header header;
	u32 proximity_domain;
	u16 reserved;
	u32 its_id;
};

/*
 * Common structure for SRAT subtable types:
 * 5: ACPI_SRAT_TYPE_GENERIC_AFFINITY
 * 6: ACPI_SRAT_TYPE_GENERIC_PORT_AFFINITY
 */

#define ACPI_SRAT_DEVICE_HANDLE_SIZE	16

struct acpi_srat_generic_affinity {
	struct acpi_subtable_header header;
	u8 reserved;
	u8 device_handle_type;
	u32 proximity_domain;
	u8 device_handle[ACPI_SRAT_DEVICE_HANDLE_SIZE];
	u32 flags;
	u32 reserved1;
};

/* Flags for struct acpi_srat_generic_affinity */

#define ACPI_SRAT_GENERIC_AFFINITY_ENABLED     (1)	/* 00: Use affinity structure */
#define ACPI_SRAT_ARCHITECTURAL_TRANSACTIONS   (1<<1)	/* ACPI 6.4 */

/*******************************************************************************
 *
 * STAO - Status Override Table (_STA override) - ACPI 6.0
 *        Version 1
 *
 * Conforms to "ACPI Specification for Status Override Table"
 * 6 January 2015
 *
 ******************************************************************************/

struct acpi_table_stao {
	struct acpi_table_header header;	/* Common ACPI table header */
	u8 ignore_uart;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*******************************************************************************
 *
<<<<<<< HEAD
 * PMTT - Platform Memory Topology Table (ACPI 5.0)
=======
 * TCPA - Trusted Computing Platform Alliance table
 *        Version 2
 *
 * TCG Hardware Interface Table for TPM 1.2 Clients and Servers
 *
 * Conforms to "TCG ACPI Specification, Family 1.2 and 2.0",
 * Version 1.2, Revision 8
 * February 27, 2017
 *
 * NOTE: There are two versions of the table with the same signature --
 * the client version and the server version. The common platform_class
 * field is used to differentiate the two types of tables.
 *
 ******************************************************************************/

struct acpi_table_tcpa_hdr {
	struct acpi_table_header header;	/* Common ACPI table header */
	u16 platform_class;
};

/*
 * Values for platform_class above.
 * This is how the client and server subtables are differentiated
 */
#define ACPI_TCPA_CLIENT_TABLE          0
#define ACPI_TCPA_SERVER_TABLE          1

struct acpi_table_tcpa_client {
	u32 minimum_log_length;	/* Minimum length for the event log area */
	u64 log_address;	/* Address of the event log area */
};

struct acpi_table_tcpa_server {
	u16 reserved;
	u64 minimum_log_length;	/* Minimum length for the event log area */
	u64 log_address;	/* Address of the event log area */
	u16 spec_revision;
	u8 device_flags;
	u8 interrupt_flags;
	u8 gpe_number;
	u8 reserved2[3];
	u32 global_interrupt;
	struct acpi_generic_address address;
	u32 reserved3;
	struct acpi_generic_address config_address;
	u8 group;
	u8 bus;			/* PCI Bus/Segment/Function numbers */
	u8 device;
	u8 function;
};

/* Values for device_flags above */

#define ACPI_TCPA_PCI_DEVICE            (1)
#define ACPI_TCPA_BUS_PNP               (1<<1)
#define ACPI_TCPA_ADDRESS_VALID         (1<<2)

/* Values for interrupt_flags above */

#define ACPI_TCPA_INTERRUPT_MODE        (1)
#define ACPI_TCPA_INTERRUPT_POLARITY    (1<<1)
#define ACPI_TCPA_SCI_VIA_GPE           (1<<2)
#define ACPI_TCPA_GLOBAL_INTERRUPT      (1<<3)

/*******************************************************************************
 *
 * TPM2 - Trusted Platform Module (TPM) 2.0 Hardware Interface Table
 *        Version 4
 *
 * TCG Hardware Interface Table for TPM 2.0 Clients and Servers
 *
 * Conforms to "TCG ACPI Specification, Family 1.2 and 2.0",
 * Version 1.2, Revision 8
 * February 27, 2017
 *
 ******************************************************************************/

/* Revision 3 */

struct acpi_table_tpm23 {
	struct acpi_table_header header;	/* Common ACPI table header */
	u32 reserved;
	u64 control_address;
	u32 start_method;
};

/* Value for start_method above */

#define ACPI_TPM23_ACPI_START_METHOD                 2

/*
 * Optional trailer for revision 3. If start method is 2, there is a 4 byte
 * reserved area of all zeros.
 */
struct acpi_tmp23_trailer {
	u32 reserved;
};

/* Revision 4 */

struct acpi_table_tpm2 {
	struct acpi_table_header header;	/* Common ACPI table header */
	u16 platform_class;
	u16 reserved;
	u64 control_address;
	u32 start_method;

	/* Platform-specific data follows */
};

/* Optional trailer for revision 4 holding platform-specific data */
struct acpi_tpm2_phy {
	u8  start_method_specific[12];
	u32 log_area_minimum_length;
	u64 log_area_start_address;
};

/* Values for start_method above */

#define ACPI_TPM2_NOT_ALLOWED                       0
#define ACPI_TPM2_RESERVED1                         1
#define ACPI_TPM2_START_METHOD                      2
#define ACPI_TPM2_RESERVED3                         3
#define ACPI_TPM2_RESERVED4                         4
#define ACPI_TPM2_RESERVED5                         5
#define ACPI_TPM2_MEMORY_MAPPED                     6
#define ACPI_TPM2_COMMAND_BUFFER                    7
#define ACPI_TPM2_COMMAND_BUFFER_WITH_START_METHOD  8
#define ACPI_TPM2_RESERVED9                         9
#define ACPI_TPM2_RESERVED10                        10
#define ACPI_TPM2_COMMAND_BUFFER_WITH_ARM_SMC       11	/* V1.2 Rev 8 */
#define ACPI_TPM2_RESERVED                          12
#define ACPI_TPM2_COMMAND_BUFFER_WITH_PLUTON        13

/* Optional trailer appears after any start_method subtables */

struct acpi_tpm2_trailer {
	u8 method_parameters[12];
	u32 minimum_log_length;	/* Minimum length for the event log area */
	u64 log_address;	/* Address of the event log area */
};

/*
 * Subtables (start_method-specific)
 */

/* 11: Start Method for ARM SMC (V1.2 Rev 8) */

struct acpi_tpm2_arm_smc {
	u32 global_interrupt;
	u8 interrupt_flags;
	u8 operation_flags;
	u16 reserved;
	u32 function_id;
};

/* Values for interrupt_flags above */

#define ACPI_TPM2_INTERRUPT_SUPPORT     (1)

/* Values for operation_flags above */

#define ACPI_TPM2_IDLE_SUPPORT          (1)

/*******************************************************************************
 *
 * UEFI - UEFI Boot optimization Table
 *        Version 1
 *
 * Conforms to "Unified Extensible Firmware Interface Specification",
 * Version 2.3, May 8, 2009
 *
 ******************************************************************************/

struct acpi_table_uefi {
	struct acpi_table_header header;	/* Common ACPI table header */
	u8 identifier[16];	/* UUID identifier */
	u16 data_offset;	/* Offset of remaining data in table */
};

/*******************************************************************************
 *
 * VIOT - Virtual I/O Translation Table
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *        Version 1
 *
 ******************************************************************************/

<<<<<<< HEAD
struct acpi_table_pmtt {
	struct acpi_table_header header;	/* Common ACPI table header */
	u32 reserved;
};

/* Common header for PMTT subtables that follow main table */

struct acpi_pmtt_header {
	u8 type;
	u8 reserved1;
	u16 length;
	u16 flags;
	u16 reserved2;
=======
struct acpi_table_viot {
	struct acpi_table_header header;	/* Common ACPI table header */
	u16 node_count;
	u16 node_offset;
	u8 reserved[8];
};

/* VIOT subtable header */

struct acpi_viot_header {
	u8 type;
	u8 reserved;
	u16 length;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* Values for Type field above */

<<<<<<< HEAD
#define ACPI_PMTT_TYPE_SOCKET           0
#define ACPI_PMTT_TYPE_CONTROLLER       1
#define ACPI_PMTT_TYPE_DIMM             2
#define ACPI_PMTT_TYPE_RESERVED         3	/* 0x03-0xFF are reserved */

/* Values for Flags field above */

#define ACPI_PMTT_TOP_LEVEL             0x0001
#define ACPI_PMTT_PHYSICAL              0x0002
#define ACPI_PMTT_MEMORY_TYPE           0x000C

/*
 * PMTT subtables, correspond to Type in struct acpi_pmtt_header
 */

/* 0: Socket Structure */

struct acpi_pmtt_socket {
	struct acpi_pmtt_header header;
	u16 socket_id;
	u16 reserved;
};

/* 1: Memory Controller subtable */

struct acpi_pmtt_controller {
	struct acpi_pmtt_header header;
	u32 read_latency;
	u32 write_latency;
	u32 read_bandwidth;
	u32 write_bandwidth;
	u16 access_width;
	u16 alignment;
	u16 reserved;
	u16 domain_count;
};

/* 1a: Proximity Domain substructure */

struct acpi_pmtt_domain {
	u32 proximity_domain;
};

/* 2: Physical Component Identifier (DIMM) */

struct acpi_pmtt_physical_component {
	struct acpi_pmtt_header header;
	u16 component_id;
	u16 reserved;
	u32 memory_size;
	u32 bios_handle;
=======
enum acpi_viot_node_type {
	ACPI_VIOT_NODE_PCI_RANGE = 0x01,
	ACPI_VIOT_NODE_MMIO = 0x02,
	ACPI_VIOT_NODE_VIRTIO_IOMMU_PCI = 0x03,
	ACPI_VIOT_NODE_VIRTIO_IOMMU_MMIO = 0x04,
	ACPI_VIOT_RESERVED = 0x05
};

/* VIOT subtables */

struct acpi_viot_pci_range {
	struct acpi_viot_header header;
	u32 endpoint_start;
	u16 segment_start;
	u16 segment_end;
	u16 bdf_start;
	u16 bdf_end;
	u16 output_node;
	u8 reserved[6];
};

struct acpi_viot_mmio {
	struct acpi_viot_header header;
	u32 endpoint;
	u64 base_address;
	u16 output_node;
	u8 reserved[6];
};

struct acpi_viot_virtio_iommu_pci {
	struct acpi_viot_header header;
	u16 segment;
	u16 bdf;
	u8 reserved[8];
};

struct acpi_viot_virtio_iommu_mmio {
	struct acpi_viot_header header;
	u8 reserved[4];
	u64 base_address;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*******************************************************************************
 *
<<<<<<< HEAD
 * RASF - RAS Feature Table (ACPI 5.0)
 *        Version 1
 *
 ******************************************************************************/

struct acpi_table_rasf {
	struct acpi_table_header header;	/* Common ACPI table header */
	u8 channel_id[12];
};

/* RASF Platform Communication Channel Shared Memory Region */

struct acpi_rasf_shared_memory {
	u32 signature;
	u16 command;
	u16 status;
	u64 requested_address;
	u64 requested_length;
	u64 actual_address;
	u64 actual_length;
	u16 flags;
	u8 speed;
};

/* Masks for Flags and Speed fields above */

#define ACPI_RASF_SCRUBBER_RUNNING      1
#define ACPI_RASF_SPEED                 (7<<1)

/* Channel Commands */

enum acpi_rasf_commands {
	ACPI_RASF_GET_RAS_CAPABILITIES = 1,
	ACPI_RASF_GET_PATROL_PARAMETERS = 2,
	ACPI_RASF_START_PATROL_SCRUBBER = 3,
	ACPI_RASF_STOP_PATROL_SCRUBBER = 4
};

/* Channel Command flags */

#define ACPI_RASF_GENERATE_SCI          (1<<15)

/* Status values */

enum acpi_rasf_status {
	ACPI_RASF_SUCCESS = 0,
	ACPI_RASF_NOT_VALID = 1,
	ACPI_RASF_NOT_SUPPORTED = 2,
	ACPI_RASF_BUSY = 3,
	ACPI_RASF_FAILED = 4,
	ACPI_RASF_ABORTED = 5,
	ACPI_RASF_INVALID_DATA = 6
};

/* Status flags */

#define ACPI_RASF_COMMAND_COMPLETE      (1)
#define ACPI_RASF_SCI_DOORBELL          (1<<1)
#define ACPI_RASF_ERROR                 (1<<2)
#define ACPI_RASF_STATUS                (0x1F<<3)
=======
 * WAET - Windows ACPI Emulated devices Table
 *        Version 1
 *
 * Conforms to "Windows ACPI Emulated Devices Table", version 1.0, April 6, 2009
 *
 ******************************************************************************/

struct acpi_table_waet {
	struct acpi_table_header header;	/* Common ACPI table header */
	u32 flags;
};

/* Masks for Flags field above */

#define ACPI_WAET_RTC_NO_ACK        (1)	/* RTC requires no int acknowledge */
#define ACPI_WAET_TIMER_ONE_READ    (1<<1)	/* PM timer requires only one read */

/*******************************************************************************
 *
 * WDAT - Watchdog Action Table
 *        Version 1
 *
 * Conforms to "Hardware Watchdog Timers Design Specification",
 * Copyright 2006 Microsoft Corporation.
 *
 ******************************************************************************/

struct acpi_table_wdat {
	struct acpi_table_header header;	/* Common ACPI table header */
	u32 header_length;	/* Watchdog Header Length */
	u16 pci_segment;	/* PCI Segment number */
	u8 pci_bus;		/* PCI Bus number */
	u8 pci_device;		/* PCI Device number */
	u8 pci_function;	/* PCI Function number */
	u8 reserved[3];
	u32 timer_period;	/* Period of one timer count (msec) */
	u32 max_count;		/* Maximum counter value supported */
	u32 min_count;		/* Minimum counter value */
	u8 flags;
	u8 reserved2[3];
	u32 entries;		/* Number of watchdog entries that follow */
};

/* Masks for Flags field above */

#define ACPI_WDAT_ENABLED           (1)
#define ACPI_WDAT_STOPPED           0x80

/* WDAT Instruction Entries (actions) */

struct acpi_wdat_entry {
	u8 action;
	u8 instruction;
	u16 reserved;
	struct acpi_generic_address register_region;
	u32 value;		/* Value used with Read/Write register */
	u32 mask;		/* Bitmask required for this register instruction */
};

/* Values for Action field above */

enum acpi_wdat_actions {
	ACPI_WDAT_RESET = 1,
	ACPI_WDAT_GET_CURRENT_COUNTDOWN = 4,
	ACPI_WDAT_GET_COUNTDOWN = 5,
	ACPI_WDAT_SET_COUNTDOWN = 6,
	ACPI_WDAT_GET_RUNNING_STATE = 8,
	ACPI_WDAT_SET_RUNNING_STATE = 9,
	ACPI_WDAT_GET_STOPPED_STATE = 10,
	ACPI_WDAT_SET_STOPPED_STATE = 11,
	ACPI_WDAT_GET_REBOOT = 16,
	ACPI_WDAT_SET_REBOOT = 17,
	ACPI_WDAT_GET_SHUTDOWN = 18,
	ACPI_WDAT_SET_SHUTDOWN = 19,
	ACPI_WDAT_GET_STATUS = 32,
	ACPI_WDAT_SET_STATUS = 33,
	ACPI_WDAT_ACTION_RESERVED = 34	/* 34 and greater are reserved */
};

/* Values for Instruction field above */

enum acpi_wdat_instructions {
	ACPI_WDAT_READ_VALUE = 0,
	ACPI_WDAT_READ_COUNTDOWN = 1,
	ACPI_WDAT_WRITE_VALUE = 2,
	ACPI_WDAT_WRITE_COUNTDOWN = 3,
	ACPI_WDAT_INSTRUCTION_RESERVED = 4,	/* 4 and greater are reserved */
	ACPI_WDAT_PRESERVE_REGISTER = 0x80	/* Except for this value */
};

/*******************************************************************************
 *
 * WDDT - Watchdog Descriptor Table
 *        Version 1
 *
 * Conforms to "Using the Intel ICH Family Watchdog Timer (WDT)",
 * Version 001, September 2002
 *
 ******************************************************************************/

struct acpi_table_wddt {
	struct acpi_table_header header;	/* Common ACPI table header */
	u16 spec_version;
	u16 table_version;
	u16 pci_vendor_id;
	struct acpi_generic_address address;
	u16 max_count;		/* Maximum counter value supported */
	u16 min_count;		/* Minimum counter value supported */
	u16 period;
	u16 status;
	u16 capability;
};

/* Flags for Status field above */

#define ACPI_WDDT_AVAILABLE     (1)
#define ACPI_WDDT_ACTIVE        (1<<1)
#define ACPI_WDDT_TCO_OS_OWNED  (1<<2)
#define ACPI_WDDT_USER_RESET    (1<<11)
#define ACPI_WDDT_WDT_RESET     (1<<12)
#define ACPI_WDDT_POWER_FAIL    (1<<13)
#define ACPI_WDDT_UNKNOWN_RESET (1<<14)

/* Flags for Capability field above */

#define ACPI_WDDT_AUTO_RESET    (1)
#define ACPI_WDDT_ALERT_SUPPORT (1<<1)

/*******************************************************************************
 *
 * WDRT - Watchdog Resource Table
 *        Version 1
 *
 * Conforms to "Watchdog Timer Hardware Requirements for Windows Server 2003",
 * Version 1.01, August 28, 2006
 *
 ******************************************************************************/

struct acpi_table_wdrt {
	struct acpi_table_header header;	/* Common ACPI table header */
	struct acpi_generic_address control_register;
	struct acpi_generic_address count_register;
	u16 pci_device_id;
	u16 pci_vendor_id;
	u8 pci_bus;		/* PCI Bus number */
	u8 pci_device;		/* PCI Device number */
	u8 pci_function;	/* PCI Function number */
	u8 pci_segment;		/* PCI Segment number */
	u16 max_count;		/* Maximum counter value supported */
	u8 units;
};

/*******************************************************************************
 *
 * WPBT - Windows Platform Environment Table (ACPI 6.0)
 *        Version 1
 *
 * Conforms to "Windows Platform Binary Table (WPBT)" 29 November 2011
 *
 ******************************************************************************/

struct acpi_table_wpbt {
	struct acpi_table_header header;	/* Common ACPI table header */
	u32 handoff_size;
	u64 handoff_address;
	u8 layout;
	u8 type;
	u16 arguments_length;
};

struct acpi_wpbt_unicode {
	u16 *unicode_string;
};

/*******************************************************************************
 *
 * WSMT - Windows SMM Security Mitigations Table
 *        Version 1
 *
 * Conforms to "Windows SMM Security Mitigations Table",
 * Version 1.0, April 18, 2016
 *
 ******************************************************************************/

struct acpi_table_wsmt {
	struct acpi_table_header header;	/* Common ACPI table header */
	u32 protection_flags;
};

/* Flags for protection_flags field above */

#define ACPI_WSMT_FIXED_COMM_BUFFERS                (1)
#define ACPI_WSMT_COMM_BUFFER_NESTED_PTR_PROTECTION (2)
#define ACPI_WSMT_SYSTEM_RESOURCE_PROTECTION        (4)

/*******************************************************************************
 *
 * XENV - Xen Environment Table (ACPI 6.0)
 *        Version 1
 *
 * Conforms to "ACPI Specification for Xen Environment Table" 4 January 2015
 *
 ******************************************************************************/

struct acpi_table_xenv {
	struct acpi_table_header header;	/* Common ACPI table header */
	u64 grant_table_address;
	u64 grant_table_size;
	u32 event_interrupt;
	u8 event_flags;
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Reset to default packing */

#pragma pack()

#endif				/* __ACTBL3_H__ */
