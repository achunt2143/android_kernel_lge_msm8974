<<<<<<< HEAD
/*
 *  linux/include/linux/mmc/host.h
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 *  linux/include/linux/mmc/host.h
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *  Host driver specific definitions.
 */
#ifndef LINUX_MMC_HOST_H
#define LINUX_MMC_HOST_H

<<<<<<< HEAD
#include <linux/leds.h>
#include <linux/sched.h>
#include <linux/device.h>
#include <linux/fault-inject.h>
#include <linux/wakelock.h>

#include <linux/mmc/core.h>
#include <linux/mmc/pm.h>

struct mmc_ios {
	unsigned int	clock;			/* clock rate */
	unsigned int	old_rate;       /* saved clock rate */
	unsigned long	clk_ts;         /* time stamp of last updated clock */
	unsigned short	vdd;
=======
#include <linux/sched.h>
#include <linux/device.h>
#include <linux/fault-inject.h>

#include <linux/mmc/core.h>
#include <linux/mmc/card.h>
#include <linux/mmc/pm.h>
#include <linux/dma-direction.h>
#include <linux/blk-crypto-profile.h>

struct mmc_ios {
	unsigned int	clock;			/* clock rate */
	unsigned short	vdd;
	unsigned int	power_delay_ms;		/* waiting for stable power */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* vdd stores the bit number of the selected voltage range from below. */

	unsigned char	bus_mode;		/* command output mode */

#define MMC_BUSMODE_OPENDRAIN	1
#define MMC_BUSMODE_PUSHPULL	2

	unsigned char	chip_select;		/* SPI chip select */

#define MMC_CS_DONTCARE		0
#define MMC_CS_HIGH		1
#define MMC_CS_LOW		2

	unsigned char	power_mode;		/* power supply mode */

#define MMC_POWER_OFF		0
#define MMC_POWER_UP		1
#define MMC_POWER_ON		2
<<<<<<< HEAD
=======
#define MMC_POWER_UNDEFINED	3
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	unsigned char	bus_width;		/* data bus width */

#define MMC_BUS_WIDTH_1		0
#define MMC_BUS_WIDTH_4		2
#define MMC_BUS_WIDTH_8		3

	unsigned char	timing;			/* timing specification used */

#define MMC_TIMING_LEGACY	0
#define MMC_TIMING_MMC_HS	1
#define MMC_TIMING_SD_HS	2
<<<<<<< HEAD
#define MMC_TIMING_UHS_SDR12	MMC_TIMING_LEGACY
#define MMC_TIMING_UHS_SDR25	MMC_TIMING_SD_HS
#define MMC_TIMING_UHS_SDR50	3
#define MMC_TIMING_UHS_SDR104	4
#define MMC_TIMING_UHS_DDR50	5
#define MMC_TIMING_MMC_HS200	6
#define MMC_TIMING_MMC_HS400	7

#define MMC_SDR_MODE		0
#define MMC_1_2V_DDR_MODE	1
#define MMC_1_8V_DDR_MODE	2
#define MMC_1_2V_SDR_MODE	3
#define MMC_1_8V_SDR_MODE	4
=======
#define MMC_TIMING_UHS_SDR12	3
#define MMC_TIMING_UHS_SDR25	4
#define MMC_TIMING_UHS_SDR50	5
#define MMC_TIMING_UHS_SDR104	6
#define MMC_TIMING_UHS_DDR50	7
#define MMC_TIMING_MMC_DDR52	8
#define MMC_TIMING_MMC_HS200	9
#define MMC_TIMING_MMC_HS400	10
#define MMC_TIMING_SD_EXP	11
#define MMC_TIMING_SD_EXP_1_2V	12
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	unsigned char	signal_voltage;		/* signalling voltage (1.8V or 3.3V) */

#define MMC_SIGNAL_VOLTAGE_330	0
#define MMC_SIGNAL_VOLTAGE_180	1
#define MMC_SIGNAL_VOLTAGE_120	2

	unsigned char	drv_type;		/* driver type (A, B, C, D) */

#define MMC_SET_DRIVER_TYPE_B	0
#define MMC_SET_DRIVER_TYPE_A	1
#define MMC_SET_DRIVER_TYPE_C	2
#define MMC_SET_DRIVER_TYPE_D	3
<<<<<<< HEAD
};

/* states to represent load on the host */
enum mmc_load {
	MMC_LOAD_HIGH,
	MMC_LOAD_LOW,
=======

	bool enhanced_strobe;			/* hs400es selection */
};

struct mmc_clk_phase {
	bool valid;
	u16 in_deg;
	u16 out_deg;
};

#define MMC_NUM_CLK_PHASES (MMC_TIMING_MMC_HS400 + 1)
struct mmc_clk_phase_map {
	struct mmc_clk_phase phase[MMC_NUM_CLK_PHASES];
};

struct mmc_host;

enum mmc_err_stat {
	MMC_ERR_CMD_TIMEOUT,
	MMC_ERR_CMD_CRC,
	MMC_ERR_DAT_TIMEOUT,
	MMC_ERR_DAT_CRC,
	MMC_ERR_AUTO_CMD,
	MMC_ERR_ADMA,
	MMC_ERR_TUNING,
	MMC_ERR_CMDQ_RED,
	MMC_ERR_CMDQ_GCE,
	MMC_ERR_CMDQ_ICCE,
	MMC_ERR_REQ_TIMEOUT,
	MMC_ERR_CMDQ_REQ_TIMEOUT,
	MMC_ERR_ICE_CFG,
	MMC_ERR_CTRL_TIMEOUT,
	MMC_ERR_UNEXPECTED_IRQ,
	MMC_ERR_MAX,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct mmc_host_ops {
	/*
<<<<<<< HEAD
	 * 'enable' is called when the host is claimed and 'disable' is called
	 * when the host is released. 'enable' and 'disable' are deprecated.
	 */
	int (*enable)(struct mmc_host *host);
	int (*disable)(struct mmc_host *host);
	/*
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * It is optional for the host to implement pre_req and post_req in
	 * order to support double buffering of requests (prepare one
	 * request while another request is active).
	 * pre_req() must always be followed by a post_req().
	 * To undo a call made to pre_req(), call post_req() with
	 * a nonzero err condition.
	 */
	void	(*post_req)(struct mmc_host *host, struct mmc_request *req,
			    int err);
<<<<<<< HEAD
	void	(*pre_req)(struct mmc_host *host, struct mmc_request *req,
			   bool is_first_req);
	void	(*request)(struct mmc_host *host, struct mmc_request *req);
	/*
	 * Avoid calling these three functions too often or in a "fast path",
	 * since underlaying controller might implement them in an expensive
	 * and/or slow way.
	 *
	 * Also note that these functions might sleep, so don't call them
	 * in the atomic contexts!
	 *
=======
	void	(*pre_req)(struct mmc_host *host, struct mmc_request *req);
	void	(*request)(struct mmc_host *host, struct mmc_request *req);
	/* Submit one request to host in atomic context. */
	int	(*request_atomic)(struct mmc_host *host,
				  struct mmc_request *req);

	/*
	 * Avoid calling the next three functions too often or in a "fast
	 * path", since underlaying controller might implement them in an
	 * expensive and/or slow way. Also note that these functions might
	 * sleep, so don't call them in the atomic contexts!
	 */

	/*
	 * Notes to the set_ios callback:
	 * ios->clock might be 0. For some controllers, setting 0Hz
	 * as any other frequency works. However, some controllers
	 * explicitly need to disable the clock. Otherwise e.g. voltage
	 * switching might fail because the SDCLK is not really quiet.
	 */
	void	(*set_ios)(struct mmc_host *host, struct mmc_ios *ios);

	/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * Return values for the get_ro callback should be:
	 *   0 for a read/write card
	 *   1 for a read-only card
	 *   -ENOSYS when not supported (equal to NULL callback)
	 *   or a negative errno value when something bad happened
<<<<<<< HEAD
	 *
=======
	 */
	int	(*get_ro)(struct mmc_host *host);

	/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * Return values for the get_cd callback should be:
	 *   0 for a absent card
	 *   1 for a present card
	 *   -ENOSYS when not supported (equal to NULL callback)
	 *   or a negative errno value when something bad happened
	 */
<<<<<<< HEAD
	void	(*set_ios)(struct mmc_host *host, struct mmc_ios *ios);
	int	(*get_ro)(struct mmc_host *host);
	int	(*get_cd)(struct mmc_host *host);

	void	(*enable_sdio_irq)(struct mmc_host *host, int enable);
=======
	int	(*get_cd)(struct mmc_host *host);

	void	(*enable_sdio_irq)(struct mmc_host *host, int enable);
	/* Mandatory callback when using MMC_CAP2_SDIO_IRQ_NOTHREAD. */
	void	(*ack_sdio_irq)(struct mmc_host *host);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* optional callback for HC quirks */
	void	(*init_card)(struct mmc_host *host, struct mmc_card *card);

	int	(*start_signal_voltage_switch)(struct mmc_host *host, struct mmc_ios *ios);

<<<<<<< HEAD
	/* The tuning command opcode value is different for SD and eMMC cards */
	int	(*execute_tuning)(struct mmc_host *host, u32 opcode);
	void	(*enable_preset_value)(struct mmc_host *host, bool enable);
	int	(*select_drive_strength)(unsigned int max_dtr, int host_drv, int card_drv);
	void	(*hw_reset)(struct mmc_host *host);
	unsigned long (*get_max_frequency)(struct mmc_host *host);
	unsigned long (*get_min_frequency)(struct mmc_host *host);
	int	(*notify_load)(struct mmc_host *, enum mmc_load);
	int	(*stop_request)(struct mmc_host *host);
	unsigned int	(*get_xfer_remain)(struct mmc_host *host);
};

struct mmc_card;
struct device;
=======
	/* Check if the card is pulling dat[0] low */
	int	(*card_busy)(struct mmc_host *host);

	/* The tuning command opcode value is different for SD and eMMC cards */
	int	(*execute_tuning)(struct mmc_host *host, u32 opcode);

	/* Prepare HS400 target operating frequency depending host driver */
	int	(*prepare_hs400_tuning)(struct mmc_host *host, struct mmc_ios *ios);

	/* Execute HS400 tuning depending host driver */
	int	(*execute_hs400_tuning)(struct mmc_host *host, struct mmc_card *card);

	/* Optional callback to prepare for SD high-speed tuning */
	int	(*prepare_sd_hs_tuning)(struct mmc_host *host, struct mmc_card *card);

	/* Optional callback to execute SD high-speed tuning */
	int	(*execute_sd_hs_tuning)(struct mmc_host *host, struct mmc_card *card);

	/* Prepare switch to DDR during the HS400 init sequence */
	int	(*hs400_prepare_ddr)(struct mmc_host *host);

	/* Prepare for switching from HS400 to HS200 */
	void	(*hs400_downgrade)(struct mmc_host *host);

	/* Complete selection of HS400 */
	void	(*hs400_complete)(struct mmc_host *host);

	/* Prepare enhanced strobe depending host driver */
	void	(*hs400_enhanced_strobe)(struct mmc_host *host,
					 struct mmc_ios *ios);
	int	(*select_drive_strength)(struct mmc_card *card,
					 unsigned int max_dtr, int host_drv,
					 int card_drv, int *drv_type);
	/* Reset the eMMC card via RST_n */
	void	(*card_hw_reset)(struct mmc_host *host);
	void	(*card_event)(struct mmc_host *host);

	/*
	 * Optional callback to support controllers with HW issues for multiple
	 * I/O. Returns the number of supported blocks for the request.
	 */
	int	(*multi_io_quirk)(struct mmc_card *card,
				  unsigned int direction, int blk_size);

	/* Initialize an SD express card, mandatory for MMC_CAP2_SD_EXP. */
	int	(*init_sd_express)(struct mmc_host *host, struct mmc_ios *ios);
};

struct mmc_cqe_ops {
	/* Allocate resources, and make the CQE operational */
	int	(*cqe_enable)(struct mmc_host *host, struct mmc_card *card);
	/* Free resources, and make the CQE non-operational */
	void	(*cqe_disable)(struct mmc_host *host);
	/*
	 * Issue a read, write or DCMD request to the CQE. Also deal with the
	 * effect of ->cqe_off().
	 */
	int	(*cqe_request)(struct mmc_host *host, struct mmc_request *mrq);
	/* Free resources (e.g. DMA mapping) associated with the request */
	void	(*cqe_post_req)(struct mmc_host *host, struct mmc_request *mrq);
	/*
	 * Prepare the CQE and host controller to accept non-CQ commands. There
	 * is no corresponding ->cqe_on(), instead ->cqe_request() is required
	 * to deal with that.
	 */
	void	(*cqe_off)(struct mmc_host *host);
	/*
	 * Wait for all CQE tasks to complete. Return an error if recovery
	 * becomes necessary.
	 */
	int	(*cqe_wait_for_idle)(struct mmc_host *host);
	/*
	 * Notify CQE that a request has timed out. Return false if the request
	 * completed or true if a timeout happened in which case indicate if
	 * recovery is needed.
	 */
	bool	(*cqe_timeout)(struct mmc_host *host, struct mmc_request *mrq,
			       bool *recovery_needed);
	/*
	 * Stop all CQE activity and prepare the CQE and host controller to
	 * accept recovery commands.
	 */
	void	(*cqe_recovery_start)(struct mmc_host *host);
	/*
	 * Clear the queue and call mmc_cqe_request_done() on all requests.
	 * Requests that errored will have the error set on the mmc_request
	 * (data->error or cmd->error for DCMD).  Requests that did not error
	 * will have zero data bytes transferred.
	 */
	void	(*cqe_recovery_finish)(struct mmc_host *host);
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct mmc_async_req {
	/* active mmc request */
	struct mmc_request	*mrq;
<<<<<<< HEAD
	unsigned int cmd_flags; /* copied from struct request */

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Check error status of completed mmc request.
	 * Returns 0 if success otherwise non zero.
	 */
<<<<<<< HEAD
	int (*err_check) (struct mmc_card *, struct mmc_async_req *);
	/* Reinserts request back to the block layer */
	void (*reinsert_req) (struct mmc_async_req *);
	/* update what part of request is not done (packed_fail_idx) */
	int (*update_interrupted_req) (struct mmc_card *,
			struct mmc_async_req *);
=======
	enum mmc_blk_status (*err_check)(struct mmc_card *, struct mmc_async_req *);
};

/**
 * struct mmc_slot - MMC slot functions
 *
 * @cd_irq:		MMC/SD-card slot hotplug detection IRQ or -EINVAL
 * @handler_priv:	MMC/SD-card slot context
 *
 * Some MMC/SD host controllers implement slot-functions like card and
 * write-protect detection natively. However, a large number of controllers
 * leave these functions to the CPU. This struct provides a hook to attach
 * such slot-function drivers.
 */
struct mmc_slot {
	int cd_irq;
	bool cd_wake_enabled;
	void *handler_priv;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/**
 * mmc_context_info - synchronization details for mmc context
 * @is_done_rcv		wake up reason was done request
 * @is_new_req		wake up reason was new request
<<<<<<< HEAD
 * @is_waiting_last_req	is true, when 1 request running on the bus and
 *			NULL fetched as second request. MMC_BLK_NEW_REQUEST
 *			notification will wake up mmc thread from waiting.
 * @is_urgent		wake up reason was urgent request
 * @wait		wait queue
 * @lock		lock to protect data fields
=======
 * @is_waiting_last_req	mmc context waiting for single running request
 * @wait		wait queue
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
struct mmc_context_info {
	bool			is_done_rcv;
	bool			is_new_req;
	bool			is_waiting_last_req;
<<<<<<< HEAD
	bool			is_urgent;
	wait_queue_head_t	wait;
	spinlock_t		lock;
};

struct mmc_hotplug {
	unsigned int irq;
	void *handler_priv;
};

enum dev_state {
	DEV_SUSPENDING = 1,
	DEV_SUSPENDED,
	DEV_RESUMED,
};

 #ifdef CONFIG_MACH_LGE
/* LGE_CHANGE, 2014/01/16, B2-BSP-FS@lge.com
 * extern int mmc_cd_get_status( )
 */
extern int mmc_cd_get_status(struct mmc_host *host);
#endif

#ifdef CONFIG_MACH_LGE
/* LGE_CHANGE
 * define enumeration for mmc-host-driver-index.
 * 2014-01-16, B2-BSP-FS@lge.com
 */
enum mmc_host_driver_index {
	MMC_HOST_DRIVER_INDEX_MMC0 = 0,
	MMC_HOST_DRIVER_INDEX_MMC1,
	MMC_HOST_DRIVER_INDEX_MMC2,
	MMC_HOST_DRIVER_INDEX_MMC3
};

enum mmc_sdcc_controller_index {
	MMC_SDCC_CONTROLLER_INDEX_SDCC1 = 1,
	MMC_SDCC_CONTROLLER_INDEX_SDCC2,
	MMC_SDCC_CONTROLLER_INDEX_SDCC3,
	MMC_SDCC_CONTROLLER_INDEX_SDCC4
};
#endif

=======
	wait_queue_head_t	wait;
};

struct regulator;
struct mmc_pwrseq;

struct mmc_supply {
	struct regulator *vmmc;		/* Card power supply */
	struct regulator *vqmmc;	/* Optional Vccq supply */
};

struct mmc_ctx {
	struct task_struct *task;
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct mmc_host {
	struct device		*parent;
	struct device		class_dev;
	int			index;
	const struct mmc_host_ops *ops;
<<<<<<< HEAD
=======
	struct mmc_pwrseq	*pwrseq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int		f_min;
	unsigned int		f_max;
	unsigned int		f_init;
	u32			ocr_avail;
	u32			ocr_avail_sdio;	/* SDIO-specific OCR */
	u32			ocr_avail_sd;	/* SD-specific OCR */
	u32			ocr_avail_mmc;	/* MMC-specific OCR */
<<<<<<< HEAD
	struct notifier_block	pm_notify;
=======
	struct wakeup_source	*ws;		/* Enable consume of uevents */
	u32			max_current_330;
	u32			max_current_300;
	u32			max_current_180;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define MMC_VDD_165_195		0x00000080	/* VDD voltage 1.65 - 1.95 */
#define MMC_VDD_20_21		0x00000100	/* VDD voltage 2.0 ~ 2.1 */
#define MMC_VDD_21_22		0x00000200	/* VDD voltage 2.1 ~ 2.2 */
#define MMC_VDD_22_23		0x00000400	/* VDD voltage 2.2 ~ 2.3 */
#define MMC_VDD_23_24		0x00000800	/* VDD voltage 2.3 ~ 2.4 */
#define MMC_VDD_24_25		0x00001000	/* VDD voltage 2.4 ~ 2.5 */
#define MMC_VDD_25_26		0x00002000	/* VDD voltage 2.5 ~ 2.6 */
#define MMC_VDD_26_27		0x00004000	/* VDD voltage 2.6 ~ 2.7 */
#define MMC_VDD_27_28		0x00008000	/* VDD voltage 2.7 ~ 2.8 */
#define MMC_VDD_28_29		0x00010000	/* VDD voltage 2.8 ~ 2.9 */
#define MMC_VDD_29_30		0x00020000	/* VDD voltage 2.9 ~ 3.0 */
#define MMC_VDD_30_31		0x00040000	/* VDD voltage 3.0 ~ 3.1 */
#define MMC_VDD_31_32		0x00080000	/* VDD voltage 3.1 ~ 3.2 */
#define MMC_VDD_32_33		0x00100000	/* VDD voltage 3.2 ~ 3.3 */
#define MMC_VDD_33_34		0x00200000	/* VDD voltage 3.3 ~ 3.4 */
#define MMC_VDD_34_35		0x00400000	/* VDD voltage 3.4 ~ 3.5 */
#define MMC_VDD_35_36		0x00800000	/* VDD voltage 3.5 ~ 3.6 */

<<<<<<< HEAD
	unsigned long		caps;		/* Host capabilities */
=======
	u32			caps;		/* Host capabilities */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define MMC_CAP_4_BIT_DATA	(1 << 0)	/* Can the host do 4 bit transfers */
#define MMC_CAP_MMC_HIGHSPEED	(1 << 1)	/* Can do MMC high-speed timing */
#define MMC_CAP_SD_HIGHSPEED	(1 << 2)	/* Can do SD high-speed timing */
#define MMC_CAP_SDIO_IRQ	(1 << 3)	/* Can signal pending SDIO IRQs */
#define MMC_CAP_SPI		(1 << 4)	/* Talks only SPI protocols */
#define MMC_CAP_NEEDS_POLL	(1 << 5)	/* Needs polling for card-detection */
#define MMC_CAP_8_BIT_DATA	(1 << 6)	/* Can the host do 8 bit transfers */
<<<<<<< HEAD

#define MMC_CAP_NONREMOVABLE	(1 << 8)	/* Nonremovable e.g. eMMC */
#define MMC_CAP_WAIT_WHILE_BUSY	(1 << 9)	/* Waits while card is busy */
#define MMC_CAP_ERASE		(1 << 10)	/* Allow erase/trim commands */
#define MMC_CAP_1_8V_DDR	(1 << 11)	/* can support */
						/* DDR mode at 1.8V */
#define MMC_CAP_1_2V_DDR	(1 << 12)	/* can support */
						/* DDR mode at 1.2V */
#define MMC_CAP_HSDDR		(MMC_CAP_1_8V_DDR | MMC_CAP_1_2V_DDR)
#define MMC_CAP_POWER_OFF_CARD	(1 << 13)	/* Can power off after boot */
#define MMC_CAP_BUS_WIDTH_TEST	(1 << 14)	/* CMD14/CMD19 bus width ok */
#define MMC_CAP_UHS_SDR12	(1 << 15)	/* Host supports UHS SDR12 mode */
#define MMC_CAP_UHS_SDR25	(1 << 16)	/* Host supports UHS SDR25 mode */
#define MMC_CAP_UHS_SDR50	(1 << 17)	/* Host supports UHS SDR50 mode */
#define MMC_CAP_UHS_SDR104	(1 << 18)	/* Host supports UHS SDR104 mode */
#define MMC_CAP_UHS_DDR50	(1 << 19)	/* Host supports UHS DDR50 mode */
#define MMC_CAP_SET_XPC_330	(1 << 20)	/* Host supports >150mA current at 3.3V */
#define MMC_CAP_SET_XPC_300	(1 << 21)	/* Host supports >150mA current at 3.0V */
#define MMC_CAP_SET_XPC_180	(1 << 22)	/* Host supports >150mA current at 1.8V */
#define MMC_CAP_DRIVER_TYPE_A	(1 << 23)	/* Host supports Driver Type A */
#define MMC_CAP_DRIVER_TYPE_C	(1 << 24)	/* Host supports Driver Type C */
#define MMC_CAP_DRIVER_TYPE_D	(1 << 25)	/* Host supports Driver Type D */
#define MMC_CAP_MAX_CURRENT_200	(1 << 26)	/* Host max current limit is 200mA */
#define MMC_CAP_MAX_CURRENT_400	(1 << 27)	/* Host max current limit is 400mA */
#define MMC_CAP_MAX_CURRENT_600	(1 << 28)	/* Host max current limit is 600mA */
#define MMC_CAP_MAX_CURRENT_800	(1 << 29)	/* Host max current limit is 800mA */
#define MMC_CAP_CMD23		(1 << 30)	/* CMD23 supported. */
#define MMC_CAP_HW_RESET	(1 << 31)	/* Hardware reset */

	unsigned int		caps2;		/* More host capabilities */

#define MMC_CAP2_BOOTPART_NOACC	(1 << 0)	/* Boot partition no access */
#define MMC_CAP2_CACHE_CTRL	(1 << 1)	/* Allow cache control */
#define MMC_CAP2_POWEROFF_NOTIFY (1 << 2)	/* Notify poweroff supported */
#define MMC_CAP2_NO_MULTI_READ	(1 << 3)	/* Multiblock reads don't work */
#define MMC_CAP2_NO_SLEEP_CMD	(1 << 4)	/* Don't allow sleep command */
=======
#define MMC_CAP_AGGRESSIVE_PM	(1 << 7)	/* Suspend (e)MMC/SD at idle  */
#define MMC_CAP_NONREMOVABLE	(1 << 8)	/* Nonremovable e.g. eMMC */
#define MMC_CAP_WAIT_WHILE_BUSY	(1 << 9)	/* Waits while card is busy */
#define MMC_CAP_3_3V_DDR	(1 << 11)	/* Host supports eMMC DDR 3.3V */
#define MMC_CAP_1_8V_DDR	(1 << 12)	/* Host supports eMMC DDR 1.8V */
#define MMC_CAP_1_2V_DDR	(1 << 13)	/* Host supports eMMC DDR 1.2V */
#define MMC_CAP_DDR		(MMC_CAP_3_3V_DDR | MMC_CAP_1_8V_DDR | \
				 MMC_CAP_1_2V_DDR)
#define MMC_CAP_POWER_OFF_CARD	(1 << 14)	/* Can power off after boot */
#define MMC_CAP_BUS_WIDTH_TEST	(1 << 15)	/* CMD14/CMD19 bus width ok */
#define MMC_CAP_UHS_SDR12	(1 << 16)	/* Host supports UHS SDR12 mode */
#define MMC_CAP_UHS_SDR25	(1 << 17)	/* Host supports UHS SDR25 mode */
#define MMC_CAP_UHS_SDR50	(1 << 18)	/* Host supports UHS SDR50 mode */
#define MMC_CAP_UHS_SDR104	(1 << 19)	/* Host supports UHS SDR104 mode */
#define MMC_CAP_UHS_DDR50	(1 << 20)	/* Host supports UHS DDR50 mode */
#define MMC_CAP_UHS		(MMC_CAP_UHS_SDR12 | MMC_CAP_UHS_SDR25 | \
				 MMC_CAP_UHS_SDR50 | MMC_CAP_UHS_SDR104 | \
				 MMC_CAP_UHS_DDR50)
#define MMC_CAP_SYNC_RUNTIME_PM	(1 << 21)	/* Synced runtime PM suspends. */
#define MMC_CAP_NEED_RSP_BUSY	(1 << 22)	/* Commands with R1B can't use R1. */
#define MMC_CAP_DRIVER_TYPE_A	(1 << 23)	/* Host supports Driver Type A */
#define MMC_CAP_DRIVER_TYPE_C	(1 << 24)	/* Host supports Driver Type C */
#define MMC_CAP_DRIVER_TYPE_D	(1 << 25)	/* Host supports Driver Type D */
#define MMC_CAP_DONE_COMPLETE	(1 << 27)	/* RW reqs can be completed within mmc_request_done() */
#define MMC_CAP_CD_WAKE		(1 << 28)	/* Enable card detect wake */
#define MMC_CAP_CMD_DURING_TFR	(1 << 29)	/* Commands during data transfer */
#define MMC_CAP_CMD23		(1 << 30)	/* CMD23 supported. */
#define MMC_CAP_HW_RESET	(1 << 31)	/* Reset the eMMC card via RST_n */

	u32			caps2;		/* More host capabilities */

#define MMC_CAP2_BOOTPART_NOACC	(1 << 0)	/* Boot partition no access */
#define MMC_CAP2_FULL_PWR_CYCLE	(1 << 2)	/* Can do full power cycle */
#define MMC_CAP2_FULL_PWR_CYCLE_IN_SUSPEND (1 << 3) /* Can do full power cycle in suspend */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define MMC_CAP2_HS200_1_8V_SDR	(1 << 5)        /* can support */
#define MMC_CAP2_HS200_1_2V_SDR	(1 << 6)        /* can support */
#define MMC_CAP2_HS200		(MMC_CAP2_HS200_1_8V_SDR | \
				 MMC_CAP2_HS200_1_2V_SDR)
<<<<<<< HEAD
#define MMC_CAP2_BROKEN_VOLTAGE	(1 << 7)	/* Use the broken voltage */
#define MMC_CAP2_DETECT_ON_ERR	(1 << 8)	/* On I/O err check card removal */
#define MMC_CAP2_HC_ERASE_SZ	(1 << 9)	/* High-capacity erase size */
#define MMC_CAP2_CD_ACTIVE_HIGH (1 << 10) /* Card-detect signal active high */

#define MMC_CAP2_PACKED_RD	(1 << 12)	/* Allow packed read */
#define MMC_CAP2_PACKED_WR	(1 << 13)	/* Allow packed write */
#define MMC_CAP2_PACKED_CMD	(MMC_CAP2_PACKED_RD | \
				 MMC_CAP2_PACKED_WR) /* Allow packed commands */
#define MMC_CAP2_PACKED_WR_CONTROL (1 << 14) /* Allow write packing control */

#define MMC_CAP2_SANITIZE	(1 << 15)		/* Support Sanitize */
#define MMC_CAP2_INIT_BKOPS	    (1 << 16)	/* Need to set BKOPS_EN */
#define MMC_CAP2_CLK_SCALE	(1 << 17)	/* Allow dynamic clk scaling */
#define MMC_CAP2_STOP_REQUEST	(1 << 18)	/* Allow stop ongoing request */
/* Use runtime PM framework provided by MMC core */
#define MMC_CAP2_CORE_RUNTIME_PM (1 << 19)
/* Allows Asynchronous SDIO irq while card is in 4-bit mode */
#define MMC_CAP2_ASYNC_SDIO_IRQ_4BIT_MODE (1 << 20)

#define MMC_CAP2_HS400_1_8V	(1 << 21)        /* can support */
#define MMC_CAP2_HS400_1_2V	(1 << 22)        /* can support */
#define MMC_CAP2_CORE_PM	(1 << 23)       /* use PM framework */
#define MMC_CAP2_HS400		(MMC_CAP2_HS400_1_8V | \
				 MMC_CAP2_HS400_1_2V)
	mmc_pm_flag_t		pm_caps;	/* supported pm features */

	int			clk_requests;	/* internal reference counter */
	unsigned int		clk_delay;	/* number of MCI clk hold cycles */
	bool			clk_gated;	/* clock gated */
	struct delayed_work	clk_gate_work; /* delayed clock gate */
	unsigned int		clk_old;	/* old clock value cache */
	spinlock_t		clk_lock;	/* lock for clk fields */
	struct mutex		clk_gate_mutex;	/* mutex for clock gating */
	struct device_attribute clkgate_delay_attr;
	unsigned long           clkgate_delay;
=======
#define MMC_CAP2_SD_EXP		(1 << 7)	/* SD express via PCIe */
#define MMC_CAP2_SD_EXP_1_2V	(1 << 8)	/* SD express 1.2V */
#define MMC_CAP2_CD_ACTIVE_HIGH	(1 << 10)	/* Card-detect signal active high */
#define MMC_CAP2_RO_ACTIVE_HIGH	(1 << 11)	/* Write-protect signal active high */
#define MMC_CAP2_NO_PRESCAN_POWERUP (1 << 14)	/* Don't power up before scan */
#define MMC_CAP2_HS400_1_8V	(1 << 15)	/* Can support HS400 1.8V */
#define MMC_CAP2_HS400_1_2V	(1 << 16)	/* Can support HS400 1.2V */
#define MMC_CAP2_HS400		(MMC_CAP2_HS400_1_8V | \
				 MMC_CAP2_HS400_1_2V)
#define MMC_CAP2_HSX00_1_8V	(MMC_CAP2_HS200_1_8V_SDR | MMC_CAP2_HS400_1_8V)
#define MMC_CAP2_HSX00_1_2V	(MMC_CAP2_HS200_1_2V_SDR | MMC_CAP2_HS400_1_2V)
#define MMC_CAP2_SDIO_IRQ_NOTHREAD (1 << 17)
#define MMC_CAP2_NO_WRITE_PROTECT (1 << 18)	/* No physical write protect pin, assume that card is always read-write */
#define MMC_CAP2_NO_SDIO	(1 << 19)	/* Do not send SDIO commands during initialization */
#define MMC_CAP2_HS400_ES	(1 << 20)	/* Host supports enhanced strobe */
#define MMC_CAP2_NO_SD		(1 << 21)	/* Do not send SD commands during initialization */
#define MMC_CAP2_NO_MMC		(1 << 22)	/* Do not send (e)MMC commands during initialization */
#define MMC_CAP2_CQE		(1 << 23)	/* Has eMMC command queue engine */
#define MMC_CAP2_CQE_DCMD	(1 << 24)	/* CQE can issue a direct command */
#define MMC_CAP2_AVOID_3_3V	(1 << 25)	/* Host must negotiate down from 3.3V */
#define MMC_CAP2_MERGE_CAPABLE	(1 << 26)	/* Host can merge a segment over the segment size */
#ifdef CONFIG_MMC_CRYPTO
#define MMC_CAP2_CRYPTO		(1 << 27)	/* Host supports inline encryption */
#else
#define MMC_CAP2_CRYPTO		0
#endif
#define MMC_CAP2_ALT_GPT_TEGRA	(1 << 28)	/* Host with eMMC that has GPT entry at a non-standard location */

	int			fixed_drv_type;	/* fixed driver type for non-removable media */

	mmc_pm_flag_t		pm_caps;	/* supported pm features */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* host specific block data */
	unsigned int		max_seg_size;	/* see blk_queue_max_segment_size */
	unsigned short		max_segs;	/* see blk_queue_max_segments */
	unsigned short		unused;
	unsigned int		max_req_size;	/* maximum number of bytes in one req */
	unsigned int		max_blk_size;	/* maximum size of one mmc block */
	unsigned int		max_blk_count;	/* maximum number of blocks in one req */
<<<<<<< HEAD
	unsigned int		max_discard_to;	/* max. discard timeout in ms */
=======
	unsigned int		max_busy_timeout; /* max busy timeout in ms */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* private data */
	spinlock_t		lock;		/* lock for claim and bus ops */

	struct mmc_ios		ios;		/* current io bus settings */
<<<<<<< HEAD
	u32			ocr;		/* the current OCR setting */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* group bitfields together to minimize padding */
	unsigned int		use_spi_crc:1;
	unsigned int		claimed:1;	/* host exclusively claimed */
<<<<<<< HEAD
	unsigned int		bus_dead:1;	/* bus has been released */
#ifdef CONFIG_MMC_DEBUG
	unsigned int		removed:1;	/* host is being removed */
#endif

	int			rescan_disable;	/* disable card detection */
=======
	unsigned int		doing_init_tune:1; /* initial tuning in progress */
	unsigned int		can_retune:1;	/* re-tuning can be used */
	unsigned int		doing_retune:1;	/* re-tuning in progress */
	unsigned int		retune_now:1;	/* do re-tuning at next req */
	unsigned int		retune_paused:1; /* re-tuning is temporarily disabled */
	unsigned int		retune_crc_disable:1; /* don't trigger retune upon crc */
	unsigned int		can_dma_map_merge:1; /* merging can be used */
	unsigned int		vqmmc_enabled:1; /* vqmmc regulator is enabled */

	int			rescan_disable;	/* disable card detection */
	int			rescan_entered;	/* used with nonremovable devices */

	int			need_retune;	/* re-tuning is needed */
	int			hold_retune;	/* hold off re-tuning */
	unsigned int		retune_period;	/* re-tuning period in secs */
	struct timer_list	retune_timer;	/* for periodic re-tuning */

	bool			trigger_card_event; /* card_event necessary */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	struct mmc_card		*card;		/* device attached to this host */

	wait_queue_head_t	wq;
<<<<<<< HEAD
	struct task_struct	*claimer;	/* task that has host claimed */
	struct task_struct	*suspend_task;
	int			claim_cnt;	/* "claim" nesting count */

	struct delayed_work	detect;
	struct wake_lock	detect_wake_lock;
	const char		*wlock_name;
	int			detect_change;	/* card detect flag */
	struct mmc_hotplug	hotplug;

	const struct mmc_bus_ops *bus_ops;	/* current bus driver */
	unsigned int		bus_refs;	/* reference counter */

	unsigned int		bus_resume_flags;
#define MMC_BUSRESUME_MANUAL_RESUME	(1 << 0)
#define MMC_BUSRESUME_NEEDS_RESUME	(1 << 1)

	unsigned int		sdio_irqs;
	struct task_struct	*sdio_irq_thread;
=======
	struct mmc_ctx		*claimer;	/* context that has host claimed */
	int			claim_cnt;	/* "claim" nesting count */
	struct mmc_ctx		default_ctx;	/* default context */

	struct delayed_work	detect;
	int			detect_change;	/* card detect flag */
	struct mmc_slot		slot;

	const struct mmc_bus_ops *bus_ops;	/* current bus driver */

	unsigned int		sdio_irqs;
	struct task_struct	*sdio_irq_thread;
	struct work_struct	sdio_irq_work;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	bool			sdio_irq_pending;
	atomic_t		sdio_irq_thread_abort;

	mmc_pm_flag_t		pm_flags;	/* requested pm features */

<<<<<<< HEAD
#ifdef CONFIG_LEDS_TRIGGERS
	struct led_trigger	*led;		/* activity led */
#endif
=======
	struct led_trigger	*led;		/* activity led */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_REGULATOR
	bool			regulator_enabled; /* regulator state */
#endif
<<<<<<< HEAD

	struct dentry		*debugfs_root;

	struct mmc_async_req	*areq;		/* active async req */
	struct mmc_context_info	context_info;	/* async synchronization info */
=======
	struct mmc_supply	supply;

	struct dentry		*debugfs_root;

	/* Ongoing data transfer that allows commands during transfer */
	struct mmc_request	*ongoing_mrq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_FAIL_MMC_REQUEST
	struct fault_attr	fail_mmc_request;
#endif

	unsigned int		actual_clock;	/* Actual HC clock rate */

<<<<<<< HEAD
#ifdef CONFIG_MMC_EMBEDDED_SDIO
	struct {
		struct sdio_cis			*cis;
		struct sdio_cccr		*cccr;
		struct sdio_embedded_func	*funcs;
		int				num_funcs;
	} embedded_sdio_data;
#endif

#ifdef CONFIG_MMC_PERF_PROFILING
	struct {

		unsigned long rbytes_drv;  /* Rd bytes MMC Host  */
		unsigned long wbytes_drv;  /* Wr bytes MMC Host  */
		ktime_t rtime_drv;	   /* Rd time  MMC Host  */
		ktime_t wtime_drv;	   /* Wr time  MMC Host  */
		ktime_t start;
	} perf;
	bool perf_enable;
#endif
	struct mmc_ios saved_ios;
	struct {
		unsigned long	busy_time_us;
		unsigned long	window_time;
		unsigned long	curr_freq;
		unsigned long	polling_delay_ms;
		unsigned int	up_threshold;
		unsigned int	down_threshold;
		ktime_t		start_busy;
		bool		enable;
		bool		initialized;
		bool		in_progress;
		/* freq. transitions are not allowed in invalid state */
		bool		invalid_state;
		struct delayed_work work;
		enum mmc_load	state;
	} clk_scaling;
	enum dev_state dev_status;
	unsigned long		private[0] ____cacheline_aligned;
};

extern struct mmc_host *mmc_alloc_host(int extra, struct device *);
extern bool mmc_host_may_gate_card(struct mmc_card *);
extern int mmc_add_host(struct mmc_host *);
extern void mmc_remove_host(struct mmc_host *);
extern void mmc_free_host(struct mmc_host *);

#ifdef CONFIG_MMC_EMBEDDED_SDIO
extern void mmc_set_embedded_sdio_data(struct mmc_host *host,
				       struct sdio_cis *cis,
				       struct sdio_cccr *cccr,
				       struct sdio_embedded_func *funcs,
				       int num_funcs);
#endif
=======
	unsigned int		slotno;	/* used for sdio acpi binding */

	int			dsr_req;	/* DSR value is valid */
	u32			dsr;	/* optional driver stage (DSR) value */

	/* Command Queue Engine (CQE) support */
	const struct mmc_cqe_ops *cqe_ops;
	void			*cqe_private;
	int			cqe_qdepth;
	bool			cqe_enabled;
	bool			cqe_on;

	/* Inline encryption support */
#ifdef CONFIG_MMC_CRYPTO
	struct blk_crypto_profile crypto_profile;
#endif

	/* Host Software Queue support */
	bool			hsq_enabled;
	int			hsq_depth;

	u32			err_stats[MMC_ERR_MAX];
	unsigned long		private[] ____cacheline_aligned;
};

struct device_node;

struct mmc_host *mmc_alloc_host(int extra, struct device *);
struct mmc_host *devm_mmc_alloc_host(struct device *dev, int extra);
int mmc_add_host(struct mmc_host *);
void mmc_remove_host(struct mmc_host *);
void mmc_free_host(struct mmc_host *);
void mmc_of_parse_clk_phase(struct device *dev,
			    struct mmc_clk_phase_map *map);
int mmc_of_parse(struct mmc_host *host);
int mmc_of_parse_voltage(struct mmc_host *host, u32 *mask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline void *mmc_priv(struct mmc_host *host)
{
	return (void *)host->private;
}

<<<<<<< HEAD
=======
static inline struct mmc_host *mmc_from_priv(void *priv)
{
	return container_of(priv, struct mmc_host, private);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define mmc_host_is_spi(host)	((host)->caps & MMC_CAP_SPI)

#define mmc_dev(x)	((x)->parent)
#define mmc_classdev(x)	(&(x)->class_dev)
#define mmc_hostname(x)	(dev_name(&(x)->class_dev))
<<<<<<< HEAD
#define mmc_bus_needs_resume(host) ((host)->bus_resume_flags & MMC_BUSRESUME_NEEDS_RESUME)
#define mmc_bus_manual_resume(host) ((host)->bus_resume_flags & MMC_BUSRESUME_MANUAL_RESUME)

static inline void mmc_set_bus_resume_policy(struct mmc_host *host, int manual)
{
	if (manual)
		host->bus_resume_flags |= MMC_BUSRESUME_MANUAL_RESUME;
	else
		host->bus_resume_flags &= ~MMC_BUSRESUME_MANUAL_RESUME;
}

extern int mmc_resume_bus(struct mmc_host *host);

extern int mmc_suspend_host(struct mmc_host *);
extern int mmc_resume_host(struct mmc_host *);

extern int mmc_power_save_host(struct mmc_host *host);
extern int mmc_power_restore_host(struct mmc_host *host);

extern void mmc_detect_change(struct mmc_host *, unsigned long delay);
extern void mmc_request_done(struct mmc_host *, struct mmc_request *);

extern int mmc_cache_ctrl(struct mmc_host *, u8);

static inline void mmc_signal_sdio_irq(struct mmc_host *host)
{
	if (!host->sdio_irqs) {
		pr_err("%s: SDIO interrupt recieved without function driver claiming an irq\n",
				mmc_hostname(host));
		return;
	}

	host->ops->enable_sdio_irq(host, 0);
	host->sdio_irq_pending = true;
	wake_up_process(host->sdio_irq_thread);
}

struct regulator;

#ifdef CONFIG_REGULATOR
int mmc_regulator_get_ocrmask(struct regulator *supply);
int mmc_regulator_set_ocr(struct mmc_host *mmc,
			struct regulator *supply,
			unsigned short vdd_bit);
#else
static inline int mmc_regulator_get_ocrmask(struct regulator *supply)
{
	return 0;
}

=======

void mmc_detect_change(struct mmc_host *, unsigned long delay);
void mmc_request_done(struct mmc_host *, struct mmc_request *);
void mmc_command_done(struct mmc_host *host, struct mmc_request *mrq);

void mmc_cqe_request_done(struct mmc_host *host, struct mmc_request *mrq);

/*
 * May be called from host driver's system/runtime suspend/resume callbacks,
 * to know if SDIO IRQs has been claimed.
 */
static inline bool sdio_irq_claimed(struct mmc_host *host)
{
	return host->sdio_irqs > 0;
}

static inline void mmc_signal_sdio_irq(struct mmc_host *host)
{
	host->ops->enable_sdio_irq(host, 0);
	host->sdio_irq_pending = true;
	if (host->sdio_irq_thread)
		wake_up_process(host->sdio_irq_thread);
}

void sdio_signal_irq(struct mmc_host *host);

#ifdef CONFIG_REGULATOR
int mmc_regulator_set_ocr(struct mmc_host *mmc,
			struct regulator *supply,
			unsigned short vdd_bit);
int mmc_regulator_set_vqmmc(struct mmc_host *mmc, struct mmc_ios *ios);
#else
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int mmc_regulator_set_ocr(struct mmc_host *mmc,
				 struct regulator *supply,
				 unsigned short vdd_bit)
{
	return 0;
}
<<<<<<< HEAD
#endif

int mmc_card_awake(struct mmc_host *host);
int mmc_card_sleep(struct mmc_host *host);
int mmc_card_can_sleep(struct mmc_host *host);

int mmc_pm_notify(struct notifier_block *notify_block, unsigned long, void *);

/* Module parameter */
extern bool mmc_assume_removable;

static inline int mmc_card_is_removable(struct mmc_host *host)
{
	return !(host->caps & MMC_CAP_NONREMOVABLE) && mmc_assume_removable;
=======

static inline int mmc_regulator_set_vqmmc(struct mmc_host *mmc,
					  struct mmc_ios *ios)
{
	return -EINVAL;
}
#endif

int mmc_regulator_get_supply(struct mmc_host *mmc);
int mmc_regulator_enable_vqmmc(struct mmc_host *mmc);
void mmc_regulator_disable_vqmmc(struct mmc_host *mmc);

static inline int mmc_card_is_removable(struct mmc_host *host)
{
	return !(host->caps & MMC_CAP_NONREMOVABLE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int mmc_card_keep_power(struct mmc_host *host)
{
	return host->pm_flags & MMC_PM_KEEP_POWER;
}

static inline int mmc_card_wake_sdio_irq(struct mmc_host *host)
{
	return host->pm_flags & MMC_PM_WAKE_SDIO_IRQ;
}

<<<<<<< HEAD
static inline int mmc_host_cmd23(struct mmc_host *host)
{
	return host->caps & MMC_CAP_CMD23;
}

static inline int mmc_boot_partition_access(struct mmc_host *host)
{
	return !(host->caps2 & MMC_CAP2_BOOTPART_NOACC);
}

static inline int mmc_host_uhs(struct mmc_host *host)
{
	return host->caps &
		(MMC_CAP_UHS_SDR12 | MMC_CAP_UHS_SDR25 |
		 MMC_CAP_UHS_SDR50 | MMC_CAP_UHS_SDR104 |
		 MMC_CAP_UHS_DDR50);
}

#ifdef CONFIG_MMC_CLKGATE
void mmc_host_clk_hold(struct mmc_host *host);
void mmc_host_clk_release(struct mmc_host *host);
unsigned int mmc_host_clk_rate(struct mmc_host *host);

#else
static inline void mmc_host_clk_hold(struct mmc_host *host)
{
}

static inline void mmc_host_clk_release(struct mmc_host *host)
{
}

static inline unsigned int mmc_host_clk_rate(struct mmc_host *host)
{
	return host->ios.clock;
}
#endif

static inline int mmc_use_core_runtime_pm(struct mmc_host *host)
{
	return host->caps2 & MMC_CAP2_CORE_RUNTIME_PM;
}

static inline int mmc_use_core_pm(struct mmc_host *host)
{
	return host->caps2 & MMC_CAP2_CORE_PM;
}

=======
/* TODO: Move to private header */
static inline int mmc_card_hs(struct mmc_card *card)
{
	return card->host->ios.timing == MMC_TIMING_SD_HS ||
		card->host->ios.timing == MMC_TIMING_MMC_HS;
}

/* TODO: Move to private header */
static inline int mmc_card_uhs(struct mmc_card *card)
{
	return card->host->ios.timing >= MMC_TIMING_UHS_SDR12 &&
		card->host->ios.timing <= MMC_TIMING_UHS_DDR50;
}

void mmc_retune_timer_stop(struct mmc_host *host);

static inline void mmc_retune_needed(struct mmc_host *host)
{
	if (host->can_retune)
		host->need_retune = 1;
}

static inline bool mmc_can_retune(struct mmc_host *host)
{
	return host->can_retune == 1;
}

static inline bool mmc_doing_retune(struct mmc_host *host)
{
	return host->doing_retune == 1;
}

static inline bool mmc_doing_tune(struct mmc_host *host)
{
	return host->doing_retune == 1 || host->doing_init_tune == 1;
}

static inline enum dma_data_direction mmc_get_dma_dir(struct mmc_data *data)
{
	return data->flags & MMC_DATA_WRITE ? DMA_TO_DEVICE : DMA_FROM_DEVICE;
}

static inline void mmc_debugfs_err_stats_inc(struct mmc_host *host,
					     enum mmc_err_stat stat)
{
	host->err_stats[stat] += 1;
}

int mmc_sd_switch(struct mmc_card *card, int mode, int group, u8 value, u8 *resp);
int mmc_send_status(struct mmc_card *card, u32 *status);
int mmc_send_tuning(struct mmc_host *host, u32 opcode, int *cmd_error);
int mmc_send_abort_tuning(struct mmc_host *host, u32 opcode);
int mmc_get_ext_csd(struct mmc_card *card, u8 **new_ext_csd);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* LINUX_MMC_HOST_H */
