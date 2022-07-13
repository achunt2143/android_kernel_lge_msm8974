<<<<<<< HEAD
#ifndef _ASM_X86_INTEL_SCU_IPC_H_
#define  _ASM_X86_INTEL_SCU_IPC_H_

#include <linux/notifier.h>

#define IPCMSG_WARM_RESET	0xF0
#define IPCMSG_COLD_RESET	0xF1
#define IPCMSG_SOFT_RESET	0xF2
#define IPCMSG_COLD_BOOT	0xF3

#define IPCMSG_VRTC		0xFA	 /* Set vRTC device */
	/* Command id associated with message IPCMSG_VRTC */
	#define IPC_CMD_VRTC_SETTIME      1 /* Set time */
	#define IPC_CMD_VRTC_SETALARM     2 /* Set alarm */

/* Read single register */
int intel_scu_ipc_ioread8(u16 addr, u8 *data);

/* Read two sequential registers */
int intel_scu_ipc_ioread16(u16 addr, u16 *data);

/* Read four sequential registers */
int intel_scu_ipc_ioread32(u16 addr, u32 *data);

/* Read a vector */
int intel_scu_ipc_readv(u16 *addr, u8 *data, int len);

/* Write single register */
int intel_scu_ipc_iowrite8(u16 addr, u8 data);

/* Write two sequential registers */
int intel_scu_ipc_iowrite16(u16 addr, u16 data);

/* Write four sequential registers */
int intel_scu_ipc_iowrite32(u16 addr, u32 data);

/* Write a vector */
int intel_scu_ipc_writev(u16 *addr, u8 *data, int len);

/* Update single register based on the mask */
int intel_scu_ipc_update_register(u16 addr, u8 data, u8 mask);

/* Issue commands to the SCU with or without data */
int intel_scu_ipc_simple_command(int cmd, int sub);
int intel_scu_ipc_command(int cmd, int sub, u32 *in, int inlen,
							u32 *out, int outlen);
/* I2C control api */
int intel_scu_ipc_i2c_cntrl(u32 addr, u32 *data);

/* Update FW version */
int intel_scu_ipc_fw_update(u8 *buffer, u32 length);

extern struct blocking_notifier_head intel_scu_notifier;

static inline void intel_scu_notifier_add(struct notifier_block *nb)
{
	blocking_notifier_chain_register(&intel_scu_notifier, nb);
}

static inline void intel_scu_notifier_remove(struct notifier_block *nb)
{
	blocking_notifier_chain_unregister(&intel_scu_notifier, nb);
}

static inline int intel_scu_notifier_post(unsigned long v, void *p)
{
	return blocking_notifier_call_chain(&intel_scu_notifier, v, p);
}

#define		SCU_AVAILABLE		1
#define		SCU_DOWN		2

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_INTEL_SCU_IPC_H_
#define  _ASM_X86_INTEL_SCU_IPC_H_

#include <linux/ioport.h>

struct device;
struct intel_scu_ipc_dev;

/**
 * struct intel_scu_ipc_data - Data used to configure SCU IPC
 * @mem: Base address of SCU IPC MMIO registers
 * @irq: The IRQ number used for SCU (optional)
 */
struct intel_scu_ipc_data {
	struct resource mem;
	int irq;
};

struct intel_scu_ipc_dev *
__intel_scu_ipc_register(struct device *parent,
			 const struct intel_scu_ipc_data *scu_data,
			 struct module *owner);

#define intel_scu_ipc_register(parent, scu_data)  \
	__intel_scu_ipc_register(parent, scu_data, THIS_MODULE)

void intel_scu_ipc_unregister(struct intel_scu_ipc_dev *scu);

struct intel_scu_ipc_dev *
__devm_intel_scu_ipc_register(struct device *parent,
			      const struct intel_scu_ipc_data *scu_data,
			      struct module *owner);

#define devm_intel_scu_ipc_register(parent, scu_data)  \
	__devm_intel_scu_ipc_register(parent, scu_data, THIS_MODULE)

struct intel_scu_ipc_dev *intel_scu_ipc_dev_get(void);
void intel_scu_ipc_dev_put(struct intel_scu_ipc_dev *scu);
struct intel_scu_ipc_dev *devm_intel_scu_ipc_dev_get(struct device *dev);

int intel_scu_ipc_dev_ioread8(struct intel_scu_ipc_dev *scu, u16 addr,
			      u8 *data);
int intel_scu_ipc_dev_iowrite8(struct intel_scu_ipc_dev *scu, u16 addr,
			       u8 data);
int intel_scu_ipc_dev_readv(struct intel_scu_ipc_dev *scu, u16 *addr,
			    u8 *data, size_t len);
int intel_scu_ipc_dev_writev(struct intel_scu_ipc_dev *scu, u16 *addr,
			     u8 *data, size_t len);

int intel_scu_ipc_dev_update(struct intel_scu_ipc_dev *scu, u16 addr,
			     u8 data, u8 mask);

int intel_scu_ipc_dev_simple_command(struct intel_scu_ipc_dev *scu, int cmd,
				     int sub);
int intel_scu_ipc_dev_command_with_size(struct intel_scu_ipc_dev *scu, int cmd,
					int sub, const void *in, size_t inlen,
					size_t size, void *out, size_t outlen);

static inline int intel_scu_ipc_dev_command(struct intel_scu_ipc_dev *scu, int cmd,
					    int sub, const void *in, size_t inlen,
					    void *out, size_t outlen)
{
	return intel_scu_ipc_dev_command_with_size(scu, cmd, sub, in, inlen,
						   inlen, out, outlen);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
