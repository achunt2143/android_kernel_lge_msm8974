<<<<<<< HEAD
#ifndef TARGET_CORE_BASE_H
#define TARGET_CORE_BASE_H

#include <linux/in.h>
#include <linux/configfs.h>
#include <linux/dma-mapping.h>
#include <linux/blkdev.h>
#include <scsi/scsi_cmnd.h>
#include <net/sock.h>
#include <net/tcp.h>

#define TARGET_CORE_MOD_VERSION		"v4.1.0-rc2-ml"
#define TARGET_CORE_VERSION		TARGET_CORE_MOD_VERSION

/* Maximum Number of LUNs per Target Portal Group */
/* Don't raise above 511 or REPORT_LUNS needs to handle >1 page */
#define TRANSPORT_MAX_LUNS_PER_TPG		256
/*
 * By default we use 32-byte CDBs in TCM Core and subsystem plugin code.
 *
 * Note that both include/scsi/scsi_cmnd.h:MAX_COMMAND_SIZE and
 * include/linux/blkdev.h:BLOCK_MAX_CDB as of v2.6.36-rc4 still use
 * 16-byte CDBs by default and require an extra allocation for
 * 32-byte CDBs to because of legacy issues.
 *
 * Within TCM Core there are no such legacy limitiations, so we go ahead
 * use 32-byte CDBs by default and use include/scsi/scsi.h:scsi_command_size()
 * within all TCM Core and subsystem plugin code.
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef TARGET_CORE_BASE_H
#define TARGET_CORE_BASE_H

#include <linux/configfs.h>      /* struct config_group */
#include <linux/dma-direction.h> /* enum dma_data_direction */
#include <linux/sbitmap.h>
#include <linux/percpu-refcount.h>
#include <linux/semaphore.h>     /* struct semaphore */
#include <linux/completion.h>

#define TARGET_CORE_VERSION		"v5.0"

/*
 * Maximum size of a CDB that can be stored in se_cmd without allocating
 * memory dynamically for the CDB.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#define TCM_MAX_COMMAND_SIZE			32
/*
 * From include/scsi/scsi_cmnd.h:SCSI_SENSE_BUFFERSIZE, currently
 * defined 96, but the real limit is 252 (or 260 including the header)
 */
<<<<<<< HEAD
#define TRANSPORT_SENSE_BUFFER			SCSI_SENSE_BUFFERSIZE
/* Used by transport_send_check_condition_and_sense() */
#define SPC_SENSE_KEY_OFFSET			2
#define SPC_ADD_SENSE_LEN_OFFSET		7
=======
#define TRANSPORT_SENSE_BUFFER			96
/* Used by transport_send_check_condition_and_sense() */
#define SPC_SENSE_KEY_OFFSET			2
#define SPC_ADD_SENSE_LEN_OFFSET		7
#define SPC_DESC_TYPE_OFFSET			8
#define SPC_ADDITIONAL_DESC_LEN_OFFSET		9
#define SPC_VALIDITY_OFFSET			10
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SPC_ASC_KEY_OFFSET			12
#define SPC_ASCQ_KEY_OFFSET			13
#define TRANSPORT_IQN_LEN			224
/* Used by target_core_store_alua_lu_gp() and target_core_alua_lu_gp_show_attr_members() */
#define LU_GROUP_NAME_BUF			256
/* Used by core_alua_store_tg_pt_gp_info() and target_core_alua_tg_pt_gp_show_attr_members() */
#define TG_PT_GROUP_NAME_BUF			256
/* Used to parse VPD into struct t10_vpd */
<<<<<<< HEAD
#define VPD_TMP_BUF_SIZE			128
=======
#define VPD_TMP_BUF_SIZE			254
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Used by transport_generic_cmd_sequencer() */
#define READ_BLOCK_LEN          		6
#define READ_CAP_LEN            		8
#define READ_POSITION_LEN       		20
#define INQUIRY_LEN				36
/* Used by transport_get_inquiry_vpd_serial() */
#define INQUIRY_VPD_SERIAL_LEN			254
/* Used by transport_get_inquiry_vpd_device_ident() */
#define INQUIRY_VPD_DEVICE_IDENTIFIER_LEN	254

<<<<<<< HEAD
=======
#define INQUIRY_VENDOR_LEN			8
#define INQUIRY_MODEL_LEN			16
#define INQUIRY_REVISION_LEN			4

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Attempts before moving from SHORT to LONG */
#define PYX_TRANSPORT_WINDOW_CLOSED_THRESHOLD	3
#define PYX_TRANSPORT_WINDOW_CLOSED_WAIT_SHORT	3  /* In milliseconds */
#define PYX_TRANSPORT_WINDOW_CLOSED_WAIT_LONG	10 /* In milliseconds */

#define PYX_TRANSPORT_STATUS_INTERVAL		5 /* In seconds */

<<<<<<< HEAD
/*
 * struct se_subsystem_dev->su_dev_flags
*/
#define SDF_FIRMWARE_VPD_UNIT_SERIAL		0x00000001
#define SDF_EMULATED_VPD_UNIT_SERIAL		0x00000002
#define SDF_USING_UDEV_PATH			0x00000004
#define SDF_USING_ALIAS				0x00000008

/*
 * struct se_device->dev_flags
 */
#define DF_READ_ONLY				0x00000001
#define DF_SPC2_RESERVATIONS			0x00000002
#define DF_SPC2_RESERVATIONS_WITH_ISID		0x00000004

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* struct se_dev_attrib sanity values */
/* Default max_unmap_lba_count */
#define DA_MAX_UNMAP_LBA_COUNT			0
/* Default max_unmap_block_desc_count */
#define DA_MAX_UNMAP_BLOCK_DESC_COUNT		0
/* Default unmap_granularity */
#define DA_UNMAP_GRANULARITY_DEFAULT		0
/* Default unmap_granularity_alignment */
#define DA_UNMAP_GRANULARITY_ALIGNMENT_DEFAULT	0
<<<<<<< HEAD
/* Default max transfer length */
#define DA_FABRIC_MAX_SECTORS			8192
/* Emulation for Direct Page Out */
#define DA_EMULATE_DPO				0
/* Emulation for Forced Unit Access WRITEs */
#define DA_EMULATE_FUA_WRITE			1
/* Emulation for Forced Unit Access READs */
#define DA_EMULATE_FUA_READ			0
/* Emulation for WriteCache and SYNCHRONIZE_CACHE */
#define DA_EMULATE_WRITE_CACHE			0
/* Emulation for UNIT ATTENTION Interlock Control */
#define DA_EMULATE_UA_INTLLCK_CTRL		0
=======
/* Default unmap_zeroes_data */
#define DA_UNMAP_ZEROES_DATA_DEFAULT		0
/* Default max_write_same_len, disabled by default */
#define DA_MAX_WRITE_SAME_LEN			0
/* Use a model alias based on the configfs backend device name */
#define DA_EMULATE_MODEL_ALIAS			0
/* Emulation for WriteCache and SYNCHRONIZE_CACHE */
#define DA_EMULATE_WRITE_CACHE			0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Emulation for TASK_ABORTED status (TAS) by default */
#define DA_EMULATE_TAS				1
/* Emulation for Thin Provisioning UNMAP using block/blk-lib.c:blkdev_issue_discard() */
#define DA_EMULATE_TPU				0
/*
 * Emulation for Thin Provisioning WRITE_SAME w/ UNMAP=1 bit using
 * block/blk-lib.c:blkdev_issue_discard()
 */
#define DA_EMULATE_TPWS				0
<<<<<<< HEAD
/* No Emulation for PSCSI by default */
#define DA_EMULATE_RESERVATIONS			0
/* No Emulation for PSCSI by default */
#define DA_EMULATE_ALUA				0
/* Enforce SCSI Initiator Port TransportID with 'ISID' for PR */
#define DA_ENFORCE_PR_ISIDS			1
=======
/* Emulation for CompareAndWrite (AtomicTestandSet) by default */
#define DA_EMULATE_CAW				1
/* Emulation for 3rd Party Copy (ExtendedCopy) by default */
#define DA_EMULATE_3PC				1
/* No Emulation for PSCSI by default */
#define DA_EMULATE_ALUA				0
/* Emulate SCSI2 RESERVE/RELEASE and Persistent Reservations by default */
#define DA_EMULATE_PR				1
/* Emulation for REPORT SUPPORTED OPERATION CODES */
#define DA_EMULATE_RSOC				1
/* Enforce SCSI Initiator Port TransportID with 'ISID' for PR */
#define DA_ENFORCE_PR_ISIDS			1
/* Force SPC-3 PR Activate Persistence across Target Power Loss */
#define DA_FORCE_PR_APTPL			0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define DA_STATUS_MAX_SECTORS_MIN		16
#define DA_STATUS_MAX_SECTORS_MAX		8192
/* By default don't report non-rotating (solid state) medium */
#define DA_IS_NONROT				0
/* Queue Algorithm Modifier default for restricted reordering in control mode page */
#define DA_EMULATE_REST_REORD			0

<<<<<<< HEAD
#define SE_INQUIRY_BUF				512
#define SE_MODE_PAGE_BUF			512
=======
#define SE_INQUIRY_BUF				1024
#define SE_MODE_PAGE_BUF			512
#define SE_SENSE_BUF				96

enum target_submit_type {
	/* Use the fabric driver's default submission type */
	TARGET_FABRIC_DEFAULT_SUBMIT,
	/* Submit from the calling context */
	TARGET_DIRECT_SUBMIT,
	/* Defer submission to the LIO workqueue */
	TARGET_QUEUE_SUBMIT,
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* struct se_hba->hba_flags */
enum hba_flags_table {
	HBA_FLAGS_INTERNAL_USE	= 0x01,
	HBA_FLAGS_PSCSI_MODE	= 0x02,
};

<<<<<<< HEAD
/* struct se_lun->lun_status */
enum transport_lun_status_table {
	TRANSPORT_LUN_STATUS_FREE = 0,
	TRANSPORT_LUN_STATUS_ACTIVE = 1,
};

/* struct se_portal_group->se_tpg_type */
enum transport_tpg_type_table {
	TRANSPORT_TPG_TYPE_NORMAL = 0,
	TRANSPORT_TPG_TYPE_DISCOVERY = 1,
};

/* struct se_task->task_flags */
enum se_task_flags {
	TF_ACTIVE		= (1 << 0),
	TF_SENT			= (1 << 1),
	TF_REQUEST_STOP		= (1 << 2),
	TF_HAS_SENSE		= (1 << 3),
};

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Special transport agnostic struct se_cmd->t_states */
enum transport_state_table {
	TRANSPORT_NO_STATE	= 0,
	TRANSPORT_NEW_CMD	= 1,
	TRANSPORT_WRITE_PENDING	= 3,
<<<<<<< HEAD
	TRANSPORT_PROCESS_WRITE	= 4,
	TRANSPORT_PROCESSING	= 5,
	TRANSPORT_COMPLETE	= 6,
	TRANSPORT_PROCESS_TMR	= 9,
	TRANSPORT_ISTATE_PROCESSING = 11,
	TRANSPORT_NEW_CMD_MAP	= 16,
	TRANSPORT_COMPLETE_QF_WP = 18,
	TRANSPORT_COMPLETE_QF_OK = 19,
=======
	TRANSPORT_PROCESSING	= 5,
	TRANSPORT_COMPLETE	= 6,
	TRANSPORT_ISTATE_PROCESSING = 11,
	TRANSPORT_COMPLETE_QF_WP = 18,
	TRANSPORT_COMPLETE_QF_OK = 19,
	TRANSPORT_COMPLETE_QF_ERR = 20,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* Used for struct se_cmd->se_cmd_flags */
enum se_cmd_flags_table {
<<<<<<< HEAD
	SCF_SUPPORTED_SAM_OPCODE	= 0x00000001,
	SCF_TRANSPORT_TASK_SENSE	= 0x00000002,
	SCF_EMULATED_TASK_SENSE		= 0x00000004,
	SCF_SCSI_DATA_SG_IO_CDB		= 0x00000008,
	SCF_SCSI_CONTROL_SG_IO_CDB	= 0x00000010,
	SCF_SCSI_NON_DATA_CDB		= 0x00000020,
	SCF_SCSI_TMR_CDB		= 0x00000040,
	SCF_SCSI_CDB_EXCEPTION		= 0x00000080,
	SCF_SCSI_RESERVATION_CONFLICT	= 0x00000100,
	SCF_FUA				= 0x00000200,
	SCF_SE_LUN_CMD			= 0x00000800,
	SCF_SE_ALLOW_EOO		= 0x00001000,
	SCF_BIDI			= 0x00002000,
	SCF_SENT_CHECK_CONDITION	= 0x00004000,
	SCF_OVERFLOW_BIT		= 0x00008000,
	SCF_UNDERFLOW_BIT		= 0x00010000,
	SCF_SENT_DELAYED_TAS		= 0x00020000,
	SCF_ALUA_NON_OPTIMIZED		= 0x00040000,
	SCF_DELAYED_CMD_FROM_SAM_ATTR	= 0x00080000,
	SCF_UNUSED			= 0x00100000,
	SCF_PASSTHROUGH_SG_TO_MEM_NOALLOC = 0x00200000,
	SCF_ACK_KREF			= 0x00400000,
};

/* struct se_dev_entry->lun_flags and struct se_lun->lun_access */
enum transport_lunflags_table {
	TRANSPORT_LUNFLAGS_NO_ACCESS		= 0x00,
	TRANSPORT_LUNFLAGS_INITIATOR_ACCESS	= 0x01,
	TRANSPORT_LUNFLAGS_READ_ONLY		= 0x02,
	TRANSPORT_LUNFLAGS_READ_WRITE		= 0x04,
};

/* struct se_device->dev_status */
enum transport_device_status_table {
	TRANSPORT_DEVICE_ACTIVATED		= 0x01,
	TRANSPORT_DEVICE_DEACTIVATED		= 0x02,
	TRANSPORT_DEVICE_QUEUE_FULL		= 0x04,
	TRANSPORT_DEVICE_SHUTDOWN		= 0x08,
	TRANSPORT_DEVICE_OFFLINE_ACTIVATED	= 0x10,
	TRANSPORT_DEVICE_OFFLINE_DEACTIVATED	= 0x20,
};

/*
 * Used by transport_send_check_condition_and_sense() and se_cmd->scsi_sense_reason
 * to signal which ASC/ASCQ sense payload should be built.
 */
enum tcm_sense_reason_table {
	TCM_NON_EXISTENT_LUN			= 0x01,
	TCM_UNSUPPORTED_SCSI_OPCODE		= 0x02,
	TCM_INCORRECT_AMOUNT_OF_DATA		= 0x03,
	TCM_UNEXPECTED_UNSOLICITED_DATA		= 0x04,
	TCM_SERVICE_CRC_ERROR			= 0x05,
	TCM_SNACK_REJECTED			= 0x06,
	TCM_SECTOR_COUNT_TOO_MANY		= 0x07,
	TCM_INVALID_CDB_FIELD			= 0x08,
	TCM_INVALID_PARAMETER_LIST		= 0x09,
	TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE	= 0x0a,
	TCM_UNKNOWN_MODE_PAGE			= 0x0b,
	TCM_WRITE_PROTECTED			= 0x0c,
	TCM_CHECK_CONDITION_ABORT_CMD		= 0x0d,
	TCM_CHECK_CONDITION_UNIT_ATTENTION	= 0x0e,
	TCM_CHECK_CONDITION_NOT_READY		= 0x0f,
	TCM_RESERVATION_CONFLICT		= 0x10,
	TCM_ADDRESS_OUT_OF_RANGE		= 0x11,
=======
	SCF_SUPPORTED_SAM_OPCODE		= (1 << 0),
	SCF_TRANSPORT_TASK_SENSE		= (1 << 1),
	SCF_EMULATED_TASK_SENSE			= (1 << 2),
	SCF_SCSI_DATA_CDB			= (1 << 3),
	SCF_SCSI_TMR_CDB			= (1 << 4),
	SCF_FUA					= (1 << 5),
	SCF_SE_LUN_CMD				= (1 << 6),
	SCF_BIDI				= (1 << 7),
	SCF_SENT_CHECK_CONDITION		= (1 << 8),
	SCF_OVERFLOW_BIT			= (1 << 9),
	SCF_UNDERFLOW_BIT			= (1 << 10),
	SCF_ALUA_NON_OPTIMIZED			= (1 << 11),
	SCF_PASSTHROUGH_SG_TO_MEM_NOALLOC	= (1 << 12),
	SCF_COMPARE_AND_WRITE			= (1 << 13),
	SCF_PASSTHROUGH_PROT_SG_TO_MEM_NOALLOC	= (1 << 14),
	SCF_ACK_KREF				= (1 << 15),
	SCF_USE_CPUID				= (1 << 16),
	SCF_TASK_ATTR_SET			= (1 << 17),
	SCF_TREAT_READ_AS_NORMAL		= (1 << 18),
};

/*
 * Used by transport_send_check_condition_and_sense()
 * to signal which ASC/ASCQ sense payload should be built.
 */
typedef unsigned __bitwise sense_reason_t;

enum tcm_sense_reason_table {
#define R(x)	(__force sense_reason_t )(x)
	TCM_NO_SENSE				= R(0x00),
	TCM_NON_EXISTENT_LUN			= R(0x01),
	TCM_UNSUPPORTED_SCSI_OPCODE		= R(0x02),
	TCM_INCORRECT_AMOUNT_OF_DATA		= R(0x03),
	TCM_UNEXPECTED_UNSOLICITED_DATA		= R(0x04),
	TCM_SERVICE_CRC_ERROR			= R(0x05),
	TCM_SNACK_REJECTED			= R(0x06),
	TCM_SECTOR_COUNT_TOO_MANY		= R(0x07),
	TCM_INVALID_CDB_FIELD			= R(0x08),
	TCM_INVALID_PARAMETER_LIST		= R(0x09),
	TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE	= R(0x0a),
	TCM_UNKNOWN_MODE_PAGE			= R(0x0b),
	TCM_WRITE_PROTECTED			= R(0x0c),
	TCM_CHECK_CONDITION_ABORT_CMD		= R(0x0d),
	TCM_CHECK_CONDITION_UNIT_ATTENTION	= R(0x0e),

	TCM_RESERVATION_CONFLICT		= R(0x10),
	TCM_ADDRESS_OUT_OF_RANGE		= R(0x11),
	TCM_OUT_OF_RESOURCES			= R(0x12),
	TCM_PARAMETER_LIST_LENGTH_ERROR		= R(0x13),
	TCM_MISCOMPARE_VERIFY			= R(0x14),
	TCM_LOGICAL_BLOCK_GUARD_CHECK_FAILED	= R(0x15),
	TCM_LOGICAL_BLOCK_APP_TAG_CHECK_FAILED	= R(0x16),
	TCM_LOGICAL_BLOCK_REF_TAG_CHECK_FAILED	= R(0x17),
	TCM_COPY_TARGET_DEVICE_NOT_REACHABLE	= R(0x18),
	TCM_TOO_MANY_TARGET_DESCS		= R(0x19),
	TCM_UNSUPPORTED_TARGET_DESC_TYPE_CODE	= R(0x1a),
	TCM_TOO_MANY_SEGMENT_DESCS		= R(0x1b),
	TCM_UNSUPPORTED_SEGMENT_DESC_TYPE_CODE	= R(0x1c),
	TCM_INSUFFICIENT_REGISTRATION_RESOURCES	= R(0x1d),
	TCM_LUN_BUSY				= R(0x1e),
	TCM_INVALID_FIELD_IN_COMMAND_IU         = R(0x1f),
	TCM_ALUA_TG_PT_STANDBY			= R(0x20),
	TCM_ALUA_TG_PT_UNAVAILABLE		= R(0x21),
	TCM_ALUA_STATE_TRANSITION		= R(0x22),
	TCM_ALUA_OFFLINE			= R(0x23),
#undef R
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

enum target_sc_flags_table {
	TARGET_SCF_BIDI_OP		= 0x01,
	TARGET_SCF_ACK_KREF		= 0x02,
	TARGET_SCF_UNKNOWN_SIZE		= 0x04,
<<<<<<< HEAD
=======
	TARGET_SCF_USE_CPUID		= 0x08,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* fabric independent task management function values */
enum tcm_tmreq_table {
	TMR_ABORT_TASK		= 1,
	TMR_ABORT_TASK_SET	= 2,
	TMR_CLEAR_ACA		= 3,
	TMR_CLEAR_TASK_SET	= 4,
	TMR_LUN_RESET		= 5,
	TMR_TARGET_WARM_RESET	= 6,
	TMR_TARGET_COLD_RESET	= 7,
<<<<<<< HEAD
	TMR_FABRIC_TMR		= 255,
=======
	TMR_LUN_RESET_PRO	= 0x80,
	TMR_UNKNOWN		= 0xff,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* fabric independent task management response values */
enum tcm_tmrsp_table {
<<<<<<< HEAD
	TMR_FUNCTION_COMPLETE		= 0,
	TMR_TASK_DOES_NOT_EXIST		= 1,
	TMR_LUN_DOES_NOT_EXIST		= 2,
	TMR_TASK_STILL_ALLEGIANT	= 3,
	TMR_TASK_FAILOVER_NOT_SUPPORTED	= 4,
	TMR_TASK_MGMT_FUNCTION_NOT_SUPPORTED	= 5,
	TMR_FUNCTION_AUTHORIZATION_FAILED = 6,
	TMR_FUNCTION_REJECTED		= 255,
};

struct se_obj {
	atomic_t obj_access_count;
};

/*
 * Used by TCM Core internally to signal if ALUA emulation is enabled or
 * disabled, or running in with TCM/pSCSI passthrough mode
 */
typedef enum {
	SPC_ALUA_PASSTHROUGH,
	SPC2_ALUA_DISABLED,
	SPC3_ALUA_EMULATED
} t10_alua_index_t;

/*
 * Used by TCM Core internally to signal if SAM Task Attribute emulation
 * is enabled or disabled, or running in with TCM/pSCSI passthrough mode
 */
typedef enum {
	SAM_TASK_ATTR_PASSTHROUGH,
	SAM_TASK_ATTR_UNTAGGED,
	SAM_TASK_ATTR_EMULATED
} t10_task_attr_index_t;

=======
	TMR_FUNCTION_FAILED		= 0,
	TMR_FUNCTION_COMPLETE		= 1,
	TMR_TASK_DOES_NOT_EXIST		= 2,
	TMR_LUN_DOES_NOT_EXIST		= 3,
	TMR_TASK_MGMT_FUNCTION_NOT_SUPPORTED	= 4,
	TMR_FUNCTION_REJECTED		= 5,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Used for target SCSI statistics
 */
typedef enum {
	SCSI_INST_INDEX,
<<<<<<< HEAD
	SCSI_DEVICE_INDEX,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	SCSI_AUTH_INTR_INDEX,
	SCSI_INDEX_TYPE_MAX
} scsi_index_t;

struct se_cmd;

<<<<<<< HEAD
struct t10_alua {
	t10_alua_index_t alua_type;
	/* ALUA Target Port Group ID */
	u16	alua_tg_pt_gps_counter;
	u32	alua_tg_pt_gps_count;
	spinlock_t tg_pt_gps_lock;
	struct se_subsystem_dev *t10_sub_dev;
=======
struct t10_alua_lba_map_member {
	struct list_head lba_map_mem_list;
	int lba_map_mem_alua_state;
	int lba_map_mem_alua_pg_id;
};

struct t10_alua_lba_map {
	u64 lba_map_first_lba;
	u64 lba_map_last_lba;
	struct list_head lba_map_list;
	struct list_head lba_map_mem_list;
};

struct t10_alua {
	/* ALUA Target Port Group ID */
	u16	alua_tg_pt_gps_counter;
	u32	alua_tg_pt_gps_count;
	/* Referrals support */
	spinlock_t lba_map_lock;
	u32     lba_map_segment_size;
	u32     lba_map_segment_multiplier;
	struct list_head lba_map_list;
	spinlock_t tg_pt_gps_lock;
	struct se_device *t10_dev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Used for default ALUA Target Port Group */
	struct t10_alua_tg_pt_gp *default_tg_pt_gp;
	/* Used for default ALUA Target Port Group ConfigFS group */
	struct config_group alua_tg_pt_gps_group;
<<<<<<< HEAD
	int (*alua_state_check)(struct se_cmd *, unsigned char *, u8 *);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct list_head tg_pt_gps_list;
};

struct t10_alua_lu_gp {
	u16	lu_gp_id;
	int	lu_gp_valid_id;
	u32	lu_gp_members;
	atomic_t lu_gp_ref_cnt;
	spinlock_t lu_gp_lock;
	struct config_group lu_gp_group;
	struct list_head lu_gp_node;
	struct list_head lu_gp_mem_list;
};

struct t10_alua_lu_gp_member {
	bool lu_gp_assoc;
	atomic_t lu_gp_mem_ref_cnt;
	spinlock_t lu_gp_mem_lock;
	struct t10_alua_lu_gp *lu_gp;
	struct se_device *lu_gp_mem_dev;
	struct list_head lu_gp_mem_list;
};

struct t10_alua_tg_pt_gp {
	u16	tg_pt_gp_id;
	int	tg_pt_gp_valid_id;
<<<<<<< HEAD
=======
	int	tg_pt_gp_alua_supported_states;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int	tg_pt_gp_alua_access_status;
	int	tg_pt_gp_alua_access_type;
	int	tg_pt_gp_nonop_delay_msecs;
	int	tg_pt_gp_trans_delay_msecs;
<<<<<<< HEAD
	int	tg_pt_gp_pref;
	int	tg_pt_gp_write_metadata;
	/* Used by struct t10_alua_tg_pt_gp->tg_pt_gp_md_buf_len */
#define ALUA_MD_BUF_LEN				1024
	u32	tg_pt_gp_md_buf_len;
	u32	tg_pt_gp_members;
	atomic_t tg_pt_gp_alua_access_state;
	atomic_t tg_pt_gp_ref_cnt;
	spinlock_t tg_pt_gp_lock;
	struct mutex tg_pt_gp_md_mutex;
	struct se_subsystem_dev *tg_pt_gp_su_dev;
	struct config_group tg_pt_gp_group;
	struct list_head tg_pt_gp_list;
	struct list_head tg_pt_gp_mem_list;
};

struct t10_alua_tg_pt_gp_member {
	bool tg_pt_gp_assoc;
	atomic_t tg_pt_gp_mem_ref_cnt;
	spinlock_t tg_pt_gp_mem_lock;
	struct t10_alua_tg_pt_gp *tg_pt_gp;
	struct se_port *tg_pt;
	struct list_head tg_pt_gp_mem_list;
=======
	int	tg_pt_gp_implicit_trans_secs;
	int	tg_pt_gp_pref;
	int	tg_pt_gp_write_metadata;
	u32	tg_pt_gp_members;
	int	tg_pt_gp_alua_access_state;
	atomic_t tg_pt_gp_ref_cnt;
	spinlock_t tg_pt_gp_lock;
	struct mutex tg_pt_gp_transition_mutex;
	struct se_device *tg_pt_gp_dev;
	struct config_group tg_pt_gp_group;
	struct list_head tg_pt_gp_list;
	struct list_head tg_pt_gp_lun_list;
	struct se_lun *tg_pt_gp_alua_lun;
	struct se_node_acl *tg_pt_gp_alua_nacl;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct t10_vpd {
	unsigned char device_identifier[INQUIRY_VPD_DEVICE_IDENTIFIER_LEN];
	int protocol_identifier_set;
	u32 protocol_identifier;
	u32 device_identifier_code_set;
	u32 association;
	u32 device_identifier_type;
	struct list_head vpd_list;
};

struct t10_wwn {
<<<<<<< HEAD
	char vendor[8];
	char model[16];
	char revision[4];
	char unit_serial[INQUIRY_VPD_SERIAL_LEN];
	spinlock_t t10_vpd_lock;
	struct se_subsystem_dev *t10_sub_dev;
=======
	/*
	 * SCSI left aligned strings may not be null terminated. +1 to ensure a
	 * null terminator is always present.
	 */
	char vendor[INQUIRY_VENDOR_LEN + 1];
	char model[INQUIRY_MODEL_LEN + 1];
	char revision[INQUIRY_REVISION_LEN + 1];
	char unit_serial[INQUIRY_VPD_SERIAL_LEN];
	u32 company_id;
	spinlock_t t10_vpd_lock;
	struct se_device *t10_dev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct config_group t10_wwn_group;
	struct list_head t10_vpd_list;
};

<<<<<<< HEAD

/*
 * Used by TCM Core internally to signal if >= SPC-3 persistent reservations
 * emulation is enabled or disabled, or running in with TCM/pSCSI passthrough
 * mode
 */
typedef enum {
	SPC_PASSTHROUGH,
	SPC2_RESERVATIONS,
	SPC3_PERSISTENT_RESERVATIONS
} t10_reservations_index_t;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct t10_pr_registration {
	/* Used for fabrics that contain WWN+ISID */
#define PR_REG_ISID_LEN				16
	/* PR_REG_ISID_LEN + ',i,0x' */
#define PR_REG_ISID_ID_LEN			(PR_REG_ISID_LEN + 5)
	char pr_reg_isid[PR_REG_ISID_LEN];
	/* Used during APTPL metadata reading */
#define PR_APTPL_MAX_IPORT_LEN			256
	unsigned char pr_iport[PR_APTPL_MAX_IPORT_LEN];
	/* Used during APTPL metadata reading */
#define PR_APTPL_MAX_TPORT_LEN			256
	unsigned char pr_tport[PR_APTPL_MAX_TPORT_LEN];
<<<<<<< HEAD
	/* For writing out live meta data */
	unsigned char *pr_aptpl_buf;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u16 pr_aptpl_rpti;
	u16 pr_reg_tpgt;
	/* Reservation effects all target ports */
	int pr_reg_all_tg_pt;
	/* Activate Persistence across Target Power Loss */
	int pr_reg_aptpl;
	int pr_res_holder;
	int pr_res_type;
	int pr_res_scope;
	/* Used for fabric initiator WWPNs using a ISID */
	bool isid_present_at_reg;
<<<<<<< HEAD
	u32 pr_res_mapped_lun;
	u32 pr_aptpl_target_lun;
=======
	u64 pr_res_mapped_lun;
	u64 pr_aptpl_target_lun;
	u16 tg_pt_sep_rtpi;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32 pr_res_generation;
	u64 pr_reg_bin_isid;
	u64 pr_res_key;
	atomic_t pr_res_holders;
	struct se_node_acl *pr_reg_nacl;
<<<<<<< HEAD
	struct se_dev_entry *pr_reg_deve;
	struct se_lun *pr_reg_tg_pt_lun;
=======
	/* Used by ALL_TG_PT=1 registration with deve->pr_ref taken */
	struct se_dev_entry *pr_reg_deve;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct list_head pr_reg_list;
	struct list_head pr_reg_abort_list;
	struct list_head pr_reg_aptpl_list;
	struct list_head pr_reg_atp_list;
	struct list_head pr_reg_atp_mem_list;
};

<<<<<<< HEAD
/*
 * This set of function pointer ops is set based upon SPC3_PERSISTENT_RESERVATIONS,
 * SPC2_RESERVATIONS or SPC_PASSTHROUGH in drivers/target/target_core_pr.c:
 * core_setup_reservations()
 */
struct t10_reservation_ops {
	int (*t10_reservation_check)(struct se_cmd *, u32 *);
	int (*t10_seq_non_holder)(struct se_cmd *, unsigned char *, u32);
	int (*t10_pr_register)(struct se_cmd *);
	int (*t10_pr_clear)(struct se_cmd *);
};

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct t10_reservation {
	/* Reservation effects all target ports */
	int pr_all_tg_pt;
	/* Activate Persistence across Target Power Loss enabled
	 * for SCSI device */
	int pr_aptpl_active;
<<<<<<< HEAD
	/* Used by struct t10_reservation->pr_aptpl_buf_len */
#define PR_APTPL_BUF_LEN			8192
	u32 pr_aptpl_buf_len;
	u32 pr_generation;
	t10_reservations_index_t res_type;
=======
#define PR_APTPL_BUF_LEN			262144
	u32 pr_generation;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spinlock_t registration_lock;
	spinlock_t aptpl_reg_lock;
	/*
	 * This will always be set by one individual I_T Nexus.
	 * However with all_tg_pt=1, other I_T Nexus from the
	 * same initiator can access PR reg/res info on a different
	 * target port.
	 *
	 * There is also the 'All Registrants' case, where there is
	 * a single *pr_res_holder of the reservation, but all
	 * registrations are considered reservation holders.
	 */
	struct se_node_acl *pr_res_holder;
	struct list_head registration_list;
	struct list_head aptpl_reg_list;
<<<<<<< HEAD
	struct t10_reservation_ops pr_ops;
};

struct se_queue_obj {
	atomic_t		queue_cnt;
	spinlock_t		cmd_queue_lock;
	struct list_head	qobj_list;
	wait_queue_head_t	thread_wq;
};

struct se_task {
	unsigned long long	task_lba;
	u32			task_sectors;
	u32			task_size;
	struct se_cmd		*task_se_cmd;
	struct scatterlist	*task_sg;
	u32			task_sg_nents;
	u16			task_flags;
	u8			task_scsi_status;
	enum dma_data_direction	task_data_direction;
	struct list_head	t_list;
	struct list_head	t_execute_list;
	struct list_head	t_state_list;
	bool			t_state_active;
	struct completion	task_stop_comp;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct se_tmr_req {
	/* Task Management function to be performed */
	u8			function;
	/* Task Management response to send */
	u8			response;
	int			call_transport;
	/* Reference to ITT that Task Mgmt should be performed */
<<<<<<< HEAD
	u32			ref_task_tag;
	/* 64-bit encoded SAM LUN from $FABRIC_MOD TMR header */
	u64			ref_task_lun;
	void 			*fabric_tmr_ptr;
	struct se_cmd		*task_cmd;
	struct se_cmd		*ref_cmd;
	struct se_device	*tmr_dev;
	struct se_lun		*tmr_lun;
	struct list_head	tmr_list;
};

struct se_cmd {
	/* SAM response code being sent to initiator */
	u8			scsi_status;
	u8			scsi_asc;
	u8			scsi_ascq;
	u8			scsi_sense_reason;
	u16			scsi_sense_length;
=======
	u64			ref_task_tag;
	void 			*fabric_tmr_ptr;
	struct se_cmd		*task_cmd;
	struct se_device	*tmr_dev;
	struct list_head	tmr_list;
};

enum target_prot_op {
	TARGET_PROT_NORMAL	= 0,
	TARGET_PROT_DIN_INSERT	= (1 << 0),
	TARGET_PROT_DOUT_INSERT	= (1 << 1),
	TARGET_PROT_DIN_STRIP	= (1 << 2),
	TARGET_PROT_DOUT_STRIP	= (1 << 3),
	TARGET_PROT_DIN_PASS	= (1 << 4),
	TARGET_PROT_DOUT_PASS	= (1 << 5),
};

#define TARGET_PROT_ALL	TARGET_PROT_DIN_INSERT | TARGET_PROT_DOUT_INSERT | \
			TARGET_PROT_DIN_STRIP | TARGET_PROT_DOUT_STRIP | \
			TARGET_PROT_DIN_PASS | TARGET_PROT_DOUT_PASS

enum target_prot_type {
	TARGET_DIF_TYPE0_PROT,
	TARGET_DIF_TYPE1_PROT,
	TARGET_DIF_TYPE2_PROT,
	TARGET_DIF_TYPE3_PROT,
};

/* Emulation for UNIT ATTENTION Interlock Control */
enum target_ua_intlck_ctrl {
	TARGET_UA_INTLCK_CTRL_CLEAR = 0,
	TARGET_UA_INTLCK_CTRL_NO_CLEAR = 1,
	TARGET_UA_INTLCK_CTRL_ESTABLISH_UA = 2,
};

enum target_core_dif_check {
	TARGET_DIF_CHECK_GUARD  = 0x1 << 0,
	TARGET_DIF_CHECK_APPTAG = 0x1 << 1,
	TARGET_DIF_CHECK_REFTAG = 0x1 << 2,
};

/* for sam_task_attr */
#define TCM_SIMPLE_TAG	0x20
#define TCM_HEAD_TAG	0x21
#define TCM_ORDERED_TAG	0x22
#define TCM_ACA_TAG	0x24

struct se_cmd {
	/* Used for fail with specific sense codes */
	sense_reason_t		sense_reason;
	/* SAM response code being sent to initiator */
	u8			scsi_status;
	u16			scsi_sense_length;
	unsigned		unknown_data_length:1;
	bool			state_active:1;
	u64			tag; /* SAM command identifier aka task tag */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Delay for ALUA Active/NonOptimized state access in milliseconds */
	int			alua_nonop_delay;
	/* See include/linux/dma-mapping.h */
	enum dma_data_direction	data_direction;
	/* For SAM Task Attribute */
	int			sam_task_attr;
<<<<<<< HEAD
	/* Transport protocol dependent state, see transport_state_table */
	enum transport_state_table t_state;
	/* Used to signal cmd->se_tfo->check_release_cmd() usage per cmd */
	unsigned		check_release:1;
	unsigned		cmd_wait_set:1;
	unsigned		unknown_data_length:1;
	/* See se_cmd_flags_table */
	u32			se_cmd_flags;
	u32			se_ordered_id;
	/* Total size in bytes associated with command */
	u32			data_length;
	/* SCSI Presented Data Transfer Length */
	u32			cmd_spdtl;
	u32			residual_count;
	u32			orig_fe_lun;
=======
	/* Used for se_sess->sess_tag_pool */
	unsigned int		map_tag;
	int			map_cpu;
	/* Transport protocol dependent state, see transport_state_table */
	enum transport_state_table t_state;
	/* See se_cmd_flags_table */
	u32			se_cmd_flags;
	/* Total size in bytes associated with command */
	u32			data_length;
	u32			residual_count;
	u64			orig_fe_lun;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Persistent Reservation key */
	u64			pr_res_key;
	/* Used for sense data */
	void			*sense_buffer;
	struct list_head	se_delayed_node;
<<<<<<< HEAD
	struct list_head	se_lun_node;
	struct list_head	se_qf_node;
	struct se_device      *se_dev;
	struct se_dev_entry   *se_deve;
	struct se_lun		*se_lun;
	/* Only used for internal passthrough and legacy TCM fabric modules */
	struct se_session	*se_sess;
	struct se_tmr_req	*se_tmr_req;
	struct list_head	se_queue_node;
	struct list_head	se_cmd_list;
	struct completion	cmd_wait_comp;
	struct kref		cmd_kref;
	struct target_core_fabric_ops *se_tfo;
	int (*execute_task)(struct se_task *);
	void (*transport_complete_callback)(struct se_cmd *);
=======
	struct list_head	se_qf_node;
	struct se_device      *se_dev;
	struct se_lun		*se_lun;
	/* Only used for internal passthrough and legacy TCM fabric modules */
	struct se_session	*se_sess;
	struct target_cmd_counter *cmd_cnt;
	struct se_tmr_req	*se_tmr_req;
	struct llist_node	se_cmd_list;
	struct completion	*free_compl;
	struct completion	*abrt_compl;
	const struct target_core_fabric_ops *se_tfo;
	sense_reason_t		(*execute_cmd)(struct se_cmd *);
	sense_reason_t (*transport_complete_callback)(struct se_cmd *, bool, int *);
	void			*protocol_data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	unsigned char		*t_task_cdb;
	unsigned char		__t_task_cdb[TCM_MAX_COMMAND_SIZE];
	unsigned long long	t_task_lba;
<<<<<<< HEAD
	u32			t_tasks_sg_chained_no;
	atomic_t		t_fe_count;
	atomic_t		t_se_count;
	atomic_t		t_task_cdbs_left;
	atomic_t		t_task_cdbs_ex_left;
	atomic_t		t_task_cdbs_sent;
=======
	unsigned int		t_task_nolb;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int		transport_state;
#define CMD_T_ABORTED		(1 << 0)
#define CMD_T_ACTIVE		(1 << 1)
#define CMD_T_COMPLETE		(1 << 2)
<<<<<<< HEAD
#define CMD_T_QUEUED		(1 << 3)
#define CMD_T_SENT		(1 << 4)
#define CMD_T_STOP		(1 << 5)
#define CMD_T_FAILED		(1 << 6)
#define CMD_T_LUN_STOP		(1 << 7)
#define CMD_T_LUN_FE_STOP	(1 << 8)
#define CMD_T_DEV_ACTIVE	(1 << 9)
	spinlock_t		t_state_lock;
	struct completion	t_transport_stop_comp;
	struct completion	transport_lun_fe_stop_comp;
	struct completion	transport_lun_stop_comp;
	struct scatterlist	*t_tasks_sg_chained;
=======
#define CMD_T_SENT		(1 << 4)
#define CMD_T_STOP		(1 << 5)
#define CMD_T_TAS		(1 << 10)
#define CMD_T_FABRIC_STOP	(1 << 11)
	spinlock_t		t_state_lock;
	struct kref		cmd_kref;
	struct completion	t_transport_stop_comp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	struct work_struct	work;

	struct scatterlist	*t_data_sg;
<<<<<<< HEAD
	unsigned int		t_data_nents;
=======
	struct scatterlist	*t_data_sg_orig;
	unsigned int		t_data_nents;
	unsigned int		t_data_nents_orig;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	void			*t_data_vmap;
	struct scatterlist	*t_bidi_data_sg;
	unsigned int		t_bidi_data_nents;

<<<<<<< HEAD
	/* Used for BIDI READ */
	struct list_head	t_task_list;
	u32			t_task_list_num;

=======
	/* Used for lun->lun_ref counting */
	int			lun_ref_active;

	struct list_head	state_list;

	/* backend private data */
	void			*priv;

	/* DIF related members */
	enum target_prot_op	prot_op;
	enum target_prot_type	prot_type;
	u8			prot_checks;
	bool			prot_pto;
	u32			prot_length;
	u32			reftag_seed;
	struct scatterlist	*t_prot_sg;
	unsigned int		t_prot_nents;
	sense_reason_t		pi_err;
	u64			sense_info;
	/*
	 * CPU LIO will execute the cmd on. Defaults to the CPU the cmd is
	 * initialized on. Drivers can override.
	 */
	int			cpuid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct se_ua {
	u8			ua_asc;
	u8			ua_ascq;
<<<<<<< HEAD
	struct se_node_acl	*ua_nacl;
	struct list_head	ua_dev_list;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct list_head	ua_nacl_list;
};

struct se_node_acl {
	char			initiatorname[TRANSPORT_IQN_LEN];
	/* Used to signal demo mode created ACL, disabled by default */
	bool			dynamic_node_acl;
<<<<<<< HEAD
	bool			acl_stop:1;
	u32			queue_depth;
	u32			acl_index;
	u64			num_cmds;
	u64			read_bytes;
	u64			write_bytes;
	spinlock_t		stats_lock;
	/* Used for PR SPEC_I_PT=1 and REGISTER_AND_MOVE */
	atomic_t		acl_pr_ref_count;
	struct se_dev_entry	**device_list;
	struct se_session	*nacl_sess;
	struct se_portal_group *se_tpg;
	spinlock_t		device_list_lock;
=======
	bool			dynamic_stop;
	u32			queue_depth;
	u32			acl_index;
	enum target_prot_type	saved_prot_type;
#define MAX_ACL_TAG_SIZE 64
	char			acl_tag[MAX_ACL_TAG_SIZE];
	/* Used for PR SPEC_I_PT=1 and REGISTER_AND_MOVE */
	atomic_t		acl_pr_ref_count;
	struct hlist_head	lun_entry_hlist;
	struct se_session	*nacl_sess;
	struct se_portal_group *se_tpg;
	struct mutex		lun_entry_mutex;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spinlock_t		nacl_sess_lock;
	struct config_group	acl_group;
	struct config_group	acl_attrib_group;
	struct config_group	acl_auth_group;
	struct config_group	acl_param_group;
	struct config_group	acl_fabric_stat_group;
<<<<<<< HEAD
	struct config_group	*acl_default_groups[5];
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct list_head	acl_list;
	struct list_head	acl_sess_list;
	struct completion	acl_free_comp;
	struct kref		acl_kref;
};

<<<<<<< HEAD
struct se_session {
	unsigned		sess_tearing_down:1;
	u64			sess_bin_isid;
=======
static inline struct se_node_acl *acl_to_nacl(struct config_item *item)
{
	return container_of(to_config_group(item), struct se_node_acl,
			acl_group);
}

static inline struct se_node_acl *attrib_to_nacl(struct config_item *item)
{
	return container_of(to_config_group(item), struct se_node_acl,
			acl_attrib_group);
}

static inline struct se_node_acl *auth_to_nacl(struct config_item *item)
{
	return container_of(to_config_group(item), struct se_node_acl,
			acl_auth_group);
}

static inline struct se_node_acl *param_to_nacl(struct config_item *item)
{
	return container_of(to_config_group(item), struct se_node_acl,
			acl_param_group);
}

static inline struct se_node_acl *fabric_stat_to_nacl(struct config_item *item)
{
	return container_of(to_config_group(item), struct se_node_acl,
			acl_fabric_stat_group);
}

struct target_cmd_counter {
	struct percpu_ref	refcnt;
	wait_queue_head_t	refcnt_wq;
	struct completion	stop_done;
	atomic_t		stopped;
};

struct se_session {
	u64			sess_bin_isid;
	enum target_prot_op	sup_prot_ops;
	enum target_prot_type	sess_prot_type;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct se_node_acl	*se_node_acl;
	struct se_portal_group *se_tpg;
	void			*fabric_sess_ptr;
	struct list_head	sess_list;
	struct list_head	sess_acl_list;
<<<<<<< HEAD
	struct list_head	sess_cmd_list;
	struct list_head	sess_wait_list;
	spinlock_t		sess_cmd_lock;
	struct kref		sess_kref;
=======
	spinlock_t		sess_cmd_lock;
	void			*sess_cmd_map;
	struct sbitmap_queue	sess_tag_pool;
	struct target_cmd_counter *cmd_cnt;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct se_device;
struct se_transform_info;
struct scatterlist;

struct se_ml_stat_grps {
	struct config_group	stat_group;
	struct config_group	scsi_auth_intr_group;
	struct config_group	scsi_att_intr_port_group;
};

struct se_lun_acl {
<<<<<<< HEAD
	char			initiatorname[TRANSPORT_IQN_LEN];
	u32			mapped_lun;
	struct se_node_acl	*se_lun_nacl;
	struct se_lun		*se_lun;
	struct list_head	lacl_list;
=======
	u64			mapped_lun;
	struct se_node_acl	*se_lun_nacl;
	struct se_lun		*se_lun;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct config_group	se_lun_group;
	struct se_ml_stat_grps	ml_stat_grps;
};

struct se_dev_entry {
<<<<<<< HEAD
	bool			def_pr_registered;
	/* See transport_lunflags_table */
	u32			lun_flags;
	u32			deve_cmds;
	u32			mapped_lun;
	u32			average_bytes;
	u32			last_byte_count;
	u32			total_cmds;
	u32			total_bytes;
	u64			pr_res_key;
	u64			creation_time;
	u32			attach_count;
	u64			read_bytes;
	u64			write_bytes;
	atomic_t		ua_count;
	/* Used for PR SPEC_I_PT=1 and REGISTER_AND_MOVE */
	atomic_t		pr_ref_count;
	struct se_lun_acl	*se_lun_acl;
	spinlock_t		ua_lock;
	struct se_lun		*se_lun;
	struct list_head	alua_port_list;
	struct list_head	ua_list;
};

struct se_dev_limits {
	/* Max supported HW queue depth */
	u32		hw_queue_depth;
	/* Max supported virtual queue depth */
	u32		queue_depth;
	/* From include/linux/blkdev.h for the other HW/SW limits. */
	struct queue_limits limits;
};

struct se_dev_attrib {
	int		emulate_dpo;
	int		emulate_fua_write;
	int		emulate_fua_read;
	int		emulate_write_cache;
	int		emulate_ua_intlck_ctrl;
	int		emulate_tas;
	int		emulate_tpu;
	int		emulate_tpws;
	int		emulate_reservations;
	int		emulate_alua;
	int		enforce_pr_isids;
	int		is_nonrot;
	int		emulate_rest_reord;
	u32		hw_block_size;
	u32		block_size;
	u32		hw_max_sectors;
	u32		max_sectors;
	u32		fabric_max_sectors;
=======
	u64			mapped_lun;
	u64			pr_res_key;
	u64			creation_time;
	bool			lun_access_ro;
	u32			attach_count;
	atomic_long_t		total_cmds;
	atomic_long_t		read_bytes;
	atomic_long_t		write_bytes;
	/* Used for PR SPEC_I_PT=1 and REGISTER_AND_MOVE */
	struct kref		pr_kref;
	struct completion	pr_comp;
	struct se_lun_acl	*se_lun_acl;
	spinlock_t		ua_lock;
	struct se_lun		*se_lun;
#define DEF_PR_REG_ACTIVE		1
	unsigned long		deve_flags;
	struct list_head	alua_port_list;
	struct list_head	lun_link;
	struct list_head	ua_list;
	struct hlist_node	link;
	struct rcu_head		rcu_head;
};

struct se_dev_attrib {
	bool		emulate_model_alias;
	bool		emulate_dpo;		/* deprecated */
	bool		emulate_fua_write;
	bool		emulate_fua_read;	/* deprecated */
	bool		emulate_write_cache;
	enum target_ua_intlck_ctrl emulate_ua_intlck_ctrl;
	bool		emulate_tas;
	bool		emulate_tpu;
	bool		emulate_tpws;
	bool		emulate_caw;
	bool		emulate_3pc;
	bool		emulate_pr;
	bool		emulate_rsoc;
	enum target_prot_type pi_prot_type;
	enum target_prot_type hw_pi_prot_type;
	bool		pi_prot_verify;
	bool		enforce_pr_isids;
	bool		force_pr_aptpl;
	bool		is_nonrot;
	bool		emulate_rest_reord;
	bool		unmap_zeroes_data;
	u32		hw_block_size;
	u32		block_size;
	u32		hw_max_sectors;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32		optimal_sectors;
	u32		hw_queue_depth;
	u32		queue_depth;
	u32		max_unmap_lba_count;
	u32		max_unmap_block_desc_count;
	u32		unmap_granularity;
	u32		unmap_granularity_alignment;
<<<<<<< HEAD
	struct se_subsystem_dev *da_sub_dev;
	struct config_group da_group;
};

=======
	u32		max_write_same_len;
	u8		submit_type;
	struct se_device *da_dev;
	struct config_group da_group;
};

struct se_port_stat_grps {
	struct config_group stat_group;
	struct config_group scsi_port_group;
	struct config_group scsi_tgt_port_group;
	struct config_group scsi_transport_group;
};

struct scsi_port_stats {
	atomic_long_t	cmd_pdus;
	atomic_long_t	tx_data_octets;
	atomic_long_t	rx_data_octets;
};

struct se_lun {
	u64			unpacked_lun;
	bool			lun_shutdown;
	bool			lun_access_ro;
	u32			lun_index;

	atomic_t		lun_acl_count;
	struct se_device __rcu	*lun_se_dev;

	struct list_head	lun_deve_list;
	spinlock_t		lun_deve_lock;

	/* ALUA state */
	int			lun_tg_pt_secondary_stat;
	int			lun_tg_pt_secondary_write_md;
	atomic_t		lun_tg_pt_secondary_offline;
	struct mutex		lun_tg_pt_md_mutex;

	/* ALUA target port group linkage */
	struct list_head	lun_tg_pt_gp_link;
	struct t10_alua_tg_pt_gp __rcu *lun_tg_pt_gp;
	spinlock_t		lun_tg_pt_gp_lock;

	struct se_portal_group	*lun_tpg;
	struct scsi_port_stats	lun_stats;
	struct config_group	lun_group;
	struct se_port_stat_grps port_stat_grps;
	struct completion	lun_shutdown_comp;
	struct percpu_ref	lun_ref;
	struct list_head	lun_dev_link;
	struct hlist_node	link;
	struct rcu_head		rcu_head;
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct se_dev_stat_grps {
	struct config_group stat_group;
	struct config_group scsi_dev_group;
	struct config_group scsi_tgt_dev_group;
	struct config_group scsi_lu_group;
};

<<<<<<< HEAD
struct se_subsystem_dev {
/* Used for struct se_subsystem_dev-->se_dev_alias, must be less than PAGE_SIZE */
#define SE_DEV_ALIAS_LEN		512
	unsigned char	se_dev_alias[SE_DEV_ALIAS_LEN];
/* Used for struct se_subsystem_dev->se_dev_udev_path[], must be less than PAGE_SIZE */
#define SE_UDEV_PATH_LEN		512
	unsigned char	se_dev_udev_path[SE_UDEV_PATH_LEN];
	u32		su_dev_flags;
	struct se_hba *se_dev_hba;
	struct se_device *se_dev_ptr;
	struct se_dev_attrib se_dev_attrib;
	/* T10 Asymmetric Logical Unit Assignment for Target Ports */
	struct t10_alua	t10_alua;
	/* T10 Inquiry and VPD WWN Information */
	struct t10_wwn	t10_wwn;
	/* T10 SPC-2 + SPC-3 Reservations */
	struct t10_reservation t10_pr;
	spinlock_t      se_dev_lock;
	void            *se_dev_su_ptr;
	struct config_group se_dev_group;
	/* For T10 Reservations */
	struct config_group se_dev_pr_group;
	/* For target_core_stat.c groups */
	struct se_dev_stat_grps dev_stat_grps;
};

struct se_device {
#define SE_DEV_LINK_MAGIC			0xfeeddeef
	u32			dev_link_magic;
	/* RELATIVE TARGET PORT IDENTIFER Counter */
	u16			dev_rpti_counter;
	/* Used for SAM Task Attribute ordering */
	u32			dev_cur_ordered_id;
	u32			dev_flags;
	u32			dev_port_count;
	/* See transport_device_status_table */
	u32			dev_status;
=======
struct se_cmd_queue {
	struct llist_head	cmd_list;
	struct work_struct	work;
};

struct se_dev_plug {
	struct se_device	*se_dev;
};

struct se_device_queue {
	struct list_head	state_list;
	spinlock_t		lock;
	struct se_cmd_queue	sq;
};

struct se_device {
	/* Used for SAM Task Attribute ordering */
	u32			dev_cur_ordered_id;
	u32			dev_flags;
#define DF_CONFIGURED				0x00000001
#define DF_FIRMWARE_VPD_UNIT_SERIAL		0x00000002
#define DF_EMULATED_VPD_UNIT_SERIAL		0x00000004
#define DF_USING_UDEV_PATH			0x00000008
#define DF_USING_ALIAS				0x00000010
#define DF_READ_ONLY				0x00000020
	u8			transport_flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Physical device queue depth */
	u32			queue_depth;
	/* Used for SPC-2 reservations enforce of ISIDs */
	u64			dev_res_bin_isid;
<<<<<<< HEAD
	t10_task_attr_index_t	dev_task_attr_type;
	/* Pointer to transport specific device structure */
	void 			*dev_ptr;
	u32			dev_index;
	u64			creation_time;
	u32			num_resets;
	u64			num_cmds;
	u64			read_bytes;
	u64			write_bytes;
	spinlock_t		stats_lock;
	/* Active commands on this virtual SE device */
	atomic_t		simple_cmds;
	atomic_t		dev_ordered_id;
	atomic_t		execute_tasks;
	atomic_t		dev_ordered_sync;
	atomic_t		dev_qf_count;
	struct se_obj		dev_obj;
	struct se_obj		dev_access_obj;
	struct se_obj		dev_export_obj;
	struct se_queue_obj	dev_queue_obj;
	spinlock_t		delayed_cmd_lock;
	spinlock_t		execute_task_lock;
	spinlock_t		dev_reservation_lock;
	spinlock_t		dev_status_lock;
	spinlock_t		se_port_lock;
	spinlock_t		se_tmr_lock;
	spinlock_t		qf_cmd_lock;
	/* Used for legacy SPC-2 reservationsa */
	struct se_node_acl	*dev_reserved_node_acl;
=======
	/* Pointer to transport specific device structure */
	u32			dev_index;
	u64			creation_time;
	atomic_long_t		num_resets;
	atomic_long_t		aborts_complete;
	atomic_long_t		aborts_no_task;
	atomic_long_t		num_cmds;
	atomic_long_t		read_bytes;
	atomic_long_t		write_bytes;
	/* Active commands on this virtual SE device */
	atomic_t		non_ordered;
	bool			ordered_sync_in_progress;
	atomic_t		delayed_cmd_count;
	atomic_t		dev_qf_count;
	u32			export_count;
	spinlock_t		delayed_cmd_lock;
	spinlock_t		dev_reservation_lock;
	unsigned int		dev_reservation_flags;
#define DRF_SPC2_RESERVATIONS			0x00000001
#define DRF_SPC2_RESERVATIONS_WITH_ISID		0x00000002
	spinlock_t		se_port_lock;
	spinlock_t		se_tmr_lock;
	spinlock_t		qf_cmd_lock;
	struct semaphore	caw_sem;
	/* Used for legacy SPC-2 reservations */
	struct se_session	*reservation_holder;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Used for ALUA Logical Unit Group membership */
	struct t10_alua_lu_gp_member *dev_alua_lu_gp_mem;
	/* Used for SPC-3 Persistent Reservations */
	struct t10_pr_registration *dev_pr_res_holder;
	struct list_head	dev_sep_list;
	struct list_head	dev_tmr_list;
<<<<<<< HEAD
	/* Pointer to descriptor for processing thread */
	struct task_struct	*process_thread;
	struct work_struct	qf_work_queue;
	struct list_head	delayed_cmd_list;
	struct list_head	execute_task_list;
	struct list_head	state_task_list;
	struct list_head	qf_cmd_list;
	/* Pointer to associated SE HBA */
	struct se_hba		*se_hba;
	struct se_subsystem_dev *se_sub_dev;
	/* Pointer to template of function pointers for transport */
	struct se_subsystem_api *transport;
	/* Linked list for struct se_hba struct se_device list */
	struct list_head	dev_list;
=======
	struct work_struct	qf_work_queue;
	struct work_struct	delayed_cmd_work;
	struct list_head	delayed_cmd_list;
	struct list_head	qf_cmd_list;
	/* Pointer to associated SE HBA */
	struct se_hba		*se_hba;
	/* T10 Inquiry and VPD WWN Information */
	struct t10_wwn		t10_wwn;
	/* T10 Asymmetric Logical Unit Assignment for Target Ports */
	struct t10_alua		t10_alua;
	/* T10 SPC-2 + SPC-3 Reservations */
	struct t10_reservation	t10_pr;
	struct se_dev_attrib	dev_attrib;
	struct config_group	dev_action_group;
	struct config_group	dev_group;
	struct config_group	dev_pr_group;
	struct se_dev_stat_grps dev_stat_grps;
#define SE_DEV_ALIAS_LEN 512		/* must be less than PAGE_SIZE */
	unsigned char		dev_alias[SE_DEV_ALIAS_LEN];
#define SE_UDEV_PATH_LEN 512		/* must be less than PAGE_SIZE */
	unsigned char		udev_path[SE_UDEV_PATH_LEN];
	/* Pointer to template of function pointers for transport */
	const struct target_backend_ops *transport;
	struct se_lun		xcopy_lun;
	/* Protection Information */
	int			prot_length;
	/* For se_lun->lun_se_dev RCU read-side critical access */
	u32			hba_index;
	struct rcu_head		rcu_head;
	int			queue_cnt;
	struct se_device_queue	*queues;
	struct mutex		lun_reset_mutex;
};

struct target_opcode_descriptor {
	u8			support:3;
	u8			serv_action_valid:1;
	u8			opcode;
	u16			service_action;
	u32			cdb_size;
	u8			specific_timeout;
	u16			nominal_timeout;
	u16			recommended_timeout;
	bool			(*enabled)(struct target_opcode_descriptor *descr,
					   struct se_cmd *cmd);
	void			(*update_usage_bits)(u8 *usage_bits,
						     struct se_device *dev);
	u8			usage_bits[];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct se_hba {
	u16			hba_tpgt;
	u32			hba_id;
	/* See hba_flags_table */
	u32			hba_flags;
	/* Virtual iSCSI devices attached. */
	u32			dev_count;
	u32			hba_index;
	/* Pointer to transport specific host structure. */
	void			*hba_ptr;
<<<<<<< HEAD
	/* Linked list for struct se_device */
	struct list_head	hba_dev_list;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct list_head	hba_node;
	spinlock_t		device_lock;
	struct config_group	hba_group;
	struct mutex		hba_access_mutex;
<<<<<<< HEAD
	struct se_subsystem_api *transport;
};

struct se_port_stat_grps {
	struct config_group stat_group;
	struct config_group scsi_port_group;
	struct config_group scsi_tgt_port_group;
	struct config_group scsi_transport_group;
};

struct se_lun {
#define SE_LUN_LINK_MAGIC			0xffff7771
	u32			lun_link_magic;
	/* See transport_lun_status_table */
	enum transport_lun_status_table lun_status;
	u32			lun_access;
	u32			lun_flags;
	u32			unpacked_lun;
	atomic_t		lun_acl_count;
	spinlock_t		lun_acl_lock;
	spinlock_t		lun_cmd_lock;
	spinlock_t		lun_sep_lock;
	struct completion	lun_shutdown_comp;
	struct list_head	lun_cmd_list;
	struct list_head	lun_acl_list;
	struct se_device	*lun_se_dev;
	struct se_port		*lun_sep;
	struct config_group	lun_group;
	struct se_port_stat_grps port_stat_grps;
};

struct scsi_port_stats {
       u64     cmd_pdus;
       u64     tx_data_octets;
       u64     rx_data_octets;
};

struct se_port {
	/* RELATIVE TARGET PORT IDENTIFER */
	u16		sep_rtpi;
	int		sep_tg_pt_secondary_stat;
	int		sep_tg_pt_secondary_write_md;
	u32		sep_index;
	struct scsi_port_stats sep_stats;
	/* Used for ALUA Target Port Groups membership */
	atomic_t	sep_tg_pt_secondary_offline;
	/* Used for PR ALL_TG_PT=1 */
	atomic_t	sep_tg_pt_ref_cnt;
	spinlock_t	sep_alua_lock;
	struct mutex	sep_tg_pt_md_mutex;
	struct t10_alua_tg_pt_gp_member *sep_alua_tg_pt_gp_mem;
	struct se_lun *sep_lun;
	struct se_portal_group *sep_tpg;
	struct list_head sep_alua_list;
	struct list_head sep_list;
=======
	struct target_backend	*backend;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct se_tpg_np {
	struct se_portal_group *tpg_np_parent;
	struct config_group	tpg_np_group;
};

<<<<<<< HEAD
struct se_portal_group {
	/* Type of target portal group, see transport_tpg_type_table */
	enum transport_tpg_type_table se_tpg_type;
	/* Number of ACLed Initiator Nodes for this TPG */
	u32			num_node_acls;
	/* Used for PR SPEC_I_PT=1 and REGISTER_AND_MOVE */
	atomic_t		tpg_pr_ref_count;
	/* Spinlock for adding/removing ACLed Nodes */
	spinlock_t		acl_node_lock;
	/* Spinlock for adding/removing sessions */
	spinlock_t		session_lock;
	spinlock_t		tpg_lun_lock;
	/* Pointer to $FABRIC_MOD portal group */
	void			*se_tpg_fabric_ptr;
	struct list_head	se_tpg_node;
	/* linked list for initiator ACL list */
	struct list_head	acl_node_list;
	struct se_lun		**tpg_lun_list;
	struct se_lun		tpg_virt_lun0;
	/* List of TCM sessions associated wth this TPG */
	struct list_head	tpg_sess_list;
	/* Pointer to $FABRIC_MOD dependent code */
	struct target_core_fabric_ops *se_tpg_tfo;
	struct se_wwn		*se_tpg_wwn;
	struct config_group	tpg_group;
	struct config_group	*tpg_default_groups[6];
=======
static inline struct se_tpg_np *to_tpg_np(struct config_item *item)
{
	return container_of(to_config_group(item), struct se_tpg_np,
			tpg_np_group);
}

struct se_portal_group {
	/*
	 * PROTOCOL IDENTIFIER value per SPC4, 7.5.1.
	 *
	 * Negative values can be used by fabric drivers for internal use TPGs.
	 */
	int			proto_id;
	bool			enabled;
	/* RELATIVE TARGET PORT IDENTIFIER */
	u16			tpg_rtpi;
	bool			rtpi_manual;
	/* Used for PR SPEC_I_PT=1 and REGISTER_AND_MOVE */
	atomic_t		tpg_pr_ref_count;
	/* Spinlock for adding/removing ACLed Nodes */
	struct mutex		acl_node_mutex;
	/* Spinlock for adding/removing sessions */
	spinlock_t		session_lock;
	struct mutex		tpg_lun_mutex;
	/* linked list for initiator ACL list */
	struct list_head	acl_node_list;
	struct hlist_head	tpg_lun_hlist;
	struct se_lun		*tpg_virt_lun0;
	/* List of TCM sessions associated wth this TPG */
	struct list_head	tpg_sess_list;
	/* Pointer to $FABRIC_MOD dependent code */
	const struct target_core_fabric_ops *se_tpg_tfo;
	struct se_wwn		*se_tpg_wwn;
	struct config_group	tpg_group;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct config_group	tpg_lun_group;
	struct config_group	tpg_np_group;
	struct config_group	tpg_acl_group;
	struct config_group	tpg_attrib_group;
<<<<<<< HEAD
	struct config_group	tpg_param_group;
};

struct se_wwn {
	struct target_fabric_configfs *wwn_tf;
	struct config_group	wwn_group;
	struct config_group	*wwn_default_groups[2];
	struct config_group	fabric_stat_group;
};

=======
	struct config_group	tpg_auth_group;
	struct config_group	tpg_param_group;
};

static inline struct se_portal_group *to_tpg(struct config_item *item)
{
	return container_of(to_config_group(item), struct se_portal_group,
			tpg_group);
}

static inline struct se_portal_group *attrib_to_tpg(struct config_item *item)
{
	return container_of(to_config_group(item), struct se_portal_group,
			tpg_attrib_group);
}

static inline struct se_portal_group *auth_to_tpg(struct config_item *item)
{
	return container_of(to_config_group(item), struct se_portal_group,
			tpg_auth_group);
}

static inline struct se_portal_group *param_to_tpg(struct config_item *item)
{
	return container_of(to_config_group(item), struct se_portal_group,
			tpg_param_group);
}

enum {
	/* Use se_cmd's cpuid for completion */
	SE_COMPL_AFFINITY_CPUID		= -1,
	/* Complete on current CPU */
	SE_COMPL_AFFINITY_CURR_CPU	= -2,
};

struct se_wwn {
	struct target_fabric_configfs *wwn_tf;
	void			*priv;
	struct config_group	wwn_group;
	struct config_group	fabric_stat_group;
	struct config_group	param_group;
	int			cmd_compl_affinity;
};

static inline void atomic_inc_mb(atomic_t *v)
{
	smp_mb__before_atomic();
	atomic_inc(v);
	smp_mb__after_atomic();
}

static inline void atomic_dec_mb(atomic_t *v)
{
	smp_mb__before_atomic();
	atomic_dec(v);
	smp_mb__after_atomic();
}

static inline void target_free_tag(struct se_session *sess, struct se_cmd *cmd)
{
	sbitmap_queue_clear(&sess->sess_tag_pool, cmd->map_tag, cmd->map_cpu);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* TARGET_CORE_BASE_H */
