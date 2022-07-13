<<<<<<< HEAD
/*******************************************************************************

  Intel 82599 Virtual Function driver
  Copyright(c) 1999 - 2012 Intel Corporation.

  This program is free software; you can redistribute it and/or modify it
  under the terms and conditions of the GNU General Public License,
  version 2, as published by the Free Software Foundation.

  This program is distributed in the hope it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.

  The full GNU General Public License is included in this distribution in
  the file called "COPYING".

  Contact Information:
  e1000-devel Mailing List <e1000-devel@lists.sourceforge.net>
  Intel Corporation, 5200 N.E. Elam Young Parkway, Hillsboro, OR 97124-6497

*******************************************************************************/
=======
// SPDX-License-Identifier: GPL-2.0
/* Copyright(c) 1999 - 2018 Intel Corporation. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "mbx.h"
#include "ixgbevf.h"

/**
 *  ixgbevf_poll_for_msg - Wait for message notification
 *  @hw: pointer to the HW structure
 *
 *  returns 0 if it successfully received a message notification
 **/
static s32 ixgbevf_poll_for_msg(struct ixgbe_hw *hw)
{
	struct ixgbe_mbx_info *mbx = &hw->mbx;
	int countdown = mbx->timeout;

<<<<<<< HEAD
=======
	if (!countdown || !mbx->ops.check_for_msg)
		return IXGBE_ERR_CONFIG;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	while (countdown && mbx->ops.check_for_msg(hw)) {
		countdown--;
		udelay(mbx->udelay);
	}

<<<<<<< HEAD
	/* if we failed, all future posted messages fail until reset */
	if (!countdown)
		mbx->timeout = 0;

	return countdown ? 0 : IXGBE_ERR_MBX;
}

/**
 *  ixgbevf_poll_for_ack - Wait for message acknowledgement
 *  @hw: pointer to the HW structure
 *
 *  returns 0 if it successfully received a message acknowledgement
=======
	return countdown ? 0 : IXGBE_ERR_TIMEOUT;
}

/**
 *  ixgbevf_poll_for_ack - Wait for message acknowledgment
 *  @hw: pointer to the HW structure
 *
 *  returns 0 if it successfully received a message acknowledgment
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static s32 ixgbevf_poll_for_ack(struct ixgbe_hw *hw)
{
	struct ixgbe_mbx_info *mbx = &hw->mbx;
	int countdown = mbx->timeout;

<<<<<<< HEAD
=======
	if (!countdown || !mbx->ops.check_for_ack)
		return IXGBE_ERR_CONFIG;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	while (countdown && mbx->ops.check_for_ack(hw)) {
		countdown--;
		udelay(mbx->udelay);
	}

<<<<<<< HEAD
	/* if we failed, all future posted messages fail until reset */
	if (!countdown)
		mbx->timeout = 0;

	return countdown ? 0 : IXGBE_ERR_MBX;
}

/**
 *  ixgbevf_read_posted_mbx - Wait for message notification and receive message
 *  @hw: pointer to the HW structure
 *  @msg: The message buffer
 *  @size: Length of buffer
 *
 *  returns 0 if it successfully received a message notification and
 *  copied it into the receive buffer.
 **/
static s32 ixgbevf_read_posted_mbx(struct ixgbe_hw *hw, u32 *msg, u16 size)
{
	struct ixgbe_mbx_info *mbx = &hw->mbx;
	s32 ret_val = IXGBE_ERR_MBX;

	ret_val = ixgbevf_poll_for_msg(hw);

	/* if ack received read message, otherwise we timed out */
	if (!ret_val)
		ret_val = mbx->ops.read(hw, msg, size);

	return ret_val;
}

/**
 *  ixgbevf_write_posted_mbx - Write a message to the mailbox, wait for ack
 *  @hw: pointer to the HW structure
 *  @msg: The message buffer
 *  @size: Length of buffer
 *
 *  returns 0 if it successfully copied message into the buffer and
 *  received an ack to that message within delay * timeout period
 **/
static s32 ixgbevf_write_posted_mbx(struct ixgbe_hw *hw, u32 *msg, u16 size)
{
	struct ixgbe_mbx_info *mbx = &hw->mbx;
	s32 ret_val;

	/* send msg */
	ret_val = mbx->ops.write(hw, msg, size);

	/* if msg sent wait until we receive an ack */
	if (!ret_val)
		ret_val = ixgbevf_poll_for_ack(hw);

	return ret_val;
}

/**
 *  ixgbevf_read_v2p_mailbox - read v2p mailbox
 *  @hw: pointer to the HW structure
 *
 *  This function is used to read the v2p mailbox without losing the read to
 *  clear status bits.
 **/
static u32 ixgbevf_read_v2p_mailbox(struct ixgbe_hw *hw)
{
	u32 v2p_mailbox = IXGBE_READ_REG(hw, IXGBE_VFMAILBOX);

	v2p_mailbox |= hw->mbx.v2p_mailbox;
	hw->mbx.v2p_mailbox |= v2p_mailbox & IXGBE_VFMAILBOX_R2C_BITS;

	return v2p_mailbox;
=======
	return countdown ? 0 : IXGBE_ERR_TIMEOUT;
}

/**
 * ixgbevf_read_mailbox_vf - read VF's mailbox register
 * @hw: pointer to the HW structure
 *
 * This function is used to read the mailbox register dedicated for VF without
 * losing the read to clear status bits.
 **/
static u32 ixgbevf_read_mailbox_vf(struct ixgbe_hw *hw)
{
	u32 vf_mailbox = IXGBE_READ_REG(hw, IXGBE_VFMAILBOX);

	vf_mailbox |= hw->mbx.vf_mailbox;
	hw->mbx.vf_mailbox |= vf_mailbox & IXGBE_VFMAILBOX_R2C_BITS;

	return vf_mailbox;
}

/**
 * ixgbevf_clear_msg_vf - clear PF status bit
 * @hw: pointer to the HW structure
 *
 * This function is used to clear PFSTS bit in the VFMAILBOX register
 **/
static void ixgbevf_clear_msg_vf(struct ixgbe_hw *hw)
{
	u32 vf_mailbox = ixgbevf_read_mailbox_vf(hw);

	if (vf_mailbox & IXGBE_VFMAILBOX_PFSTS) {
		hw->mbx.stats.reqs++;
		hw->mbx.vf_mailbox &= ~IXGBE_VFMAILBOX_PFSTS;
	}
}

/**
 * ixgbevf_clear_ack_vf - clear PF ACK bit
 * @hw: pointer to the HW structure
 *
 * This function is used to clear PFACK bit in the VFMAILBOX register
 **/
static void ixgbevf_clear_ack_vf(struct ixgbe_hw *hw)
{
	u32 vf_mailbox = ixgbevf_read_mailbox_vf(hw);

	if (vf_mailbox & IXGBE_VFMAILBOX_PFACK) {
		hw->mbx.stats.acks++;
		hw->mbx.vf_mailbox &= ~IXGBE_VFMAILBOX_PFACK;
	}
}

/**
 * ixgbevf_clear_rst_vf - clear PF reset bit
 * @hw: pointer to the HW structure
 *
 * This function is used to clear reset indication and reset done bit in
 * VFMAILBOX register after reset the shared resources and the reset sequence.
 **/
static void ixgbevf_clear_rst_vf(struct ixgbe_hw *hw)
{
	u32 vf_mailbox = ixgbevf_read_mailbox_vf(hw);

	if (vf_mailbox & (IXGBE_VFMAILBOX_RSTI | IXGBE_VFMAILBOX_RSTD)) {
		hw->mbx.stats.rsts++;
		hw->mbx.vf_mailbox &= ~(IXGBE_VFMAILBOX_RSTI |
					IXGBE_VFMAILBOX_RSTD);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 *  ixgbevf_check_for_bit_vf - Determine if a status bit was set
 *  @hw: pointer to the HW structure
 *  @mask: bitmask for bits to be tested and cleared
 *
 *  This function is used to check for the read to clear bits within
 *  the V2P mailbox.
 **/
static s32 ixgbevf_check_for_bit_vf(struct ixgbe_hw *hw, u32 mask)
{
<<<<<<< HEAD
	u32 v2p_mailbox = ixgbevf_read_v2p_mailbox(hw);
	s32 ret_val = IXGBE_ERR_MBX;

	if (v2p_mailbox & mask)
		ret_val = 0;

	hw->mbx.v2p_mailbox &= ~mask;

=======
	u32 vf_mailbox = ixgbevf_read_mailbox_vf(hw);
	s32 ret_val = IXGBE_ERR_MBX;

	if (vf_mailbox & mask)
		ret_val = 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret_val;
}

/**
 *  ixgbevf_check_for_msg_vf - checks to see if the PF has sent mail
 *  @hw: pointer to the HW structure
 *
 *  returns 0 if the PF has set the Status bit or else ERR_MBX
 **/
static s32 ixgbevf_check_for_msg_vf(struct ixgbe_hw *hw)
{
	s32 ret_val = IXGBE_ERR_MBX;

	if (!ixgbevf_check_for_bit_vf(hw, IXGBE_VFMAILBOX_PFSTS)) {
		ret_val = 0;
		hw->mbx.stats.reqs++;
	}

	return ret_val;
}

/**
 *  ixgbevf_check_for_ack_vf - checks to see if the PF has ACK'd
 *  @hw: pointer to the HW structure
 *
 *  returns 0 if the PF has set the ACK bit or else ERR_MBX
 **/
static s32 ixgbevf_check_for_ack_vf(struct ixgbe_hw *hw)
{
	s32 ret_val = IXGBE_ERR_MBX;

	if (!ixgbevf_check_for_bit_vf(hw, IXGBE_VFMAILBOX_PFACK)) {
		ret_val = 0;
<<<<<<< HEAD
=======
		ixgbevf_clear_ack_vf(hw);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		hw->mbx.stats.acks++;
	}

	return ret_val;
}

/**
 *  ixgbevf_check_for_rst_vf - checks to see if the PF has reset
 *  @hw: pointer to the HW structure
 *
 *  returns true if the PF has set the reset done bit or else false
 **/
static s32 ixgbevf_check_for_rst_vf(struct ixgbe_hw *hw)
{
	s32 ret_val = IXGBE_ERR_MBX;

	if (!ixgbevf_check_for_bit_vf(hw, (IXGBE_VFMAILBOX_RSTD |
<<<<<<< HEAD
					 IXGBE_VFMAILBOX_RSTI))) {
		ret_val = 0;
=======
					   IXGBE_VFMAILBOX_RSTI))) {
		ret_val = 0;
		ixgbevf_clear_rst_vf(hw);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		hw->mbx.stats.rsts++;
	}

	return ret_val;
}

/**
 *  ixgbevf_obtain_mbx_lock_vf - obtain mailbox lock
 *  @hw: pointer to the HW structure
 *
 *  return 0 if we obtained the mailbox lock
 **/
static s32 ixgbevf_obtain_mbx_lock_vf(struct ixgbe_hw *hw)
{
<<<<<<< HEAD
	s32 ret_val = IXGBE_ERR_MBX;

	/* Take ownership of the buffer */
	IXGBE_WRITE_REG(hw, IXGBE_VFMAILBOX, IXGBE_VFMAILBOX_VFU);

	/* reserve mailbox for vf use */
	if (ixgbevf_read_v2p_mailbox(hw) & IXGBE_VFMAILBOX_VFU)
		ret_val = 0;
=======
	struct ixgbe_mbx_info *mbx = &hw->mbx;
	s32 ret_val = IXGBE_ERR_CONFIG;
	int countdown = mbx->timeout;
	u32 vf_mailbox;

	if (!mbx->timeout)
		return ret_val;

	while (countdown--) {
		/* Reserve mailbox for VF use */
		vf_mailbox = ixgbevf_read_mailbox_vf(hw);
		vf_mailbox |= IXGBE_VFMAILBOX_VFU;
		IXGBE_WRITE_REG(hw, IXGBE_VFMAILBOX, vf_mailbox);

		/* Verify that VF is the owner of the lock */
		if (ixgbevf_read_mailbox_vf(hw) & IXGBE_VFMAILBOX_VFU) {
			ret_val = 0;
			break;
		}

		/* Wait a bit before trying again */
		udelay(mbx->udelay);
	}

	if (ret_val)
		ret_val = IXGBE_ERR_TIMEOUT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret_val;
}

/**
<<<<<<< HEAD
=======
 * ixgbevf_release_mbx_lock_vf - release mailbox lock
 * @hw: pointer to the HW structure
 **/
static void ixgbevf_release_mbx_lock_vf(struct ixgbe_hw *hw)
{
	u32 vf_mailbox;

	/* Return ownership of the buffer */
	vf_mailbox = ixgbevf_read_mailbox_vf(hw);
	vf_mailbox &= ~IXGBE_VFMAILBOX_VFU;
	IXGBE_WRITE_REG(hw, IXGBE_VFMAILBOX, vf_mailbox);
}

/**
 * ixgbevf_release_mbx_lock_vf_legacy - release mailbox lock
 * @hw: pointer to the HW structure
 **/
static void ixgbevf_release_mbx_lock_vf_legacy(struct ixgbe_hw *__always_unused hw)
{
}

/**
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *  ixgbevf_write_mbx_vf - Write a message to the mailbox
 *  @hw: pointer to the HW structure
 *  @msg: The message buffer
 *  @size: Length of buffer
 *
 *  returns 0 if it successfully copied message into the buffer
 **/
static s32 ixgbevf_write_mbx_vf(struct ixgbe_hw *hw, u32 *msg, u16 size)
{
<<<<<<< HEAD
	s32 ret_val;
	u16 i;


	/* lock the mailbox to prevent pf/vf race condition */
=======
	u32 vf_mailbox;
	s32 ret_val;
	u16 i;

	/* lock the mailbox to prevent PF/VF race condition */
	ret_val = ixgbevf_obtain_mbx_lock_vf(hw);
	if (ret_val)
		goto out_no_write;

	/* flush msg and acks as we are overwriting the message buffer */
	ixgbevf_clear_msg_vf(hw);
	ixgbevf_clear_ack_vf(hw);

	/* copy the caller specified message to the mailbox memory buffer */
	for (i = 0; i < size; i++)
		IXGBE_WRITE_REG_ARRAY(hw, IXGBE_VFMBMEM, i, msg[i]);

	/* update stats */
	hw->mbx.stats.msgs_tx++;

	/* interrupt the PF to tell it a message has been sent */
	vf_mailbox = ixgbevf_read_mailbox_vf(hw);
	vf_mailbox |= IXGBE_VFMAILBOX_REQ;
	IXGBE_WRITE_REG(hw, IXGBE_VFMAILBOX, vf_mailbox);

	/* if msg sent wait until we receive an ack */
	ret_val = ixgbevf_poll_for_ack(hw);

out_no_write:
	hw->mbx.ops.release(hw);

	return ret_val;
}

/**
 *  ixgbevf_write_mbx_vf_legacy - Write a message to the mailbox
 *  @hw: pointer to the HW structure
 *  @msg: The message buffer
 *  @size: Length of buffer
 *
 *  returns 0 if it successfully copied message into the buffer
 **/
static s32 ixgbevf_write_mbx_vf_legacy(struct ixgbe_hw *hw, u32 *msg, u16 size)
{
	s32 ret_val;
	u16 i;

	/* lock the mailbox to prevent PF/VF race condition */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret_val = ixgbevf_obtain_mbx_lock_vf(hw);
	if (ret_val)
		goto out_no_write;

	/* flush msg and acks as we are overwriting the message buffer */
	ixgbevf_check_for_msg_vf(hw);
<<<<<<< HEAD
	ixgbevf_check_for_ack_vf(hw);
=======
	ixgbevf_clear_msg_vf(hw);
	ixgbevf_check_for_ack_vf(hw);
	ixgbevf_clear_ack_vf(hw);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* copy the caller specified message to the mailbox memory buffer */
	for (i = 0; i < size; i++)
		IXGBE_WRITE_REG_ARRAY(hw, IXGBE_VFMBMEM, i, msg[i]);

	/* update stats */
	hw->mbx.stats.msgs_tx++;

	/* Drop VFU and interrupt the PF to tell it a message has been sent */
	IXGBE_WRITE_REG(hw, IXGBE_VFMAILBOX, IXGBE_VFMAILBOX_REQ);

out_no_write:
	return ret_val;
}

/**
<<<<<<< HEAD
 *  ixgbevf_read_mbx_vf - Reads a message from the inbox intended for vf
=======
 *  ixgbevf_read_mbx_vf - Reads a message from the inbox intended for VF
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *  @hw: pointer to the HW structure
 *  @msg: The message buffer
 *  @size: Length of buffer
 *
 *  returns 0 if it successfully read message from buffer
 **/
static s32 ixgbevf_read_mbx_vf(struct ixgbe_hw *hw, u32 *msg, u16 size)
{
<<<<<<< HEAD
	s32 ret_val = 0;
	u16 i;

	/* lock the mailbox to prevent pf/vf race condition */
=======
	u32 vf_mailbox;
	s32 ret_val;
	u16 i;

	/* check if there is a message from PF */
	ret_val = ixgbevf_check_for_msg_vf(hw);
	if (ret_val)
		return ret_val;

	ixgbevf_clear_msg_vf(hw);

	/* copy the message from the mailbox memory buffer */
	for (i = 0; i < size; i++)
		msg[i] = IXGBE_READ_REG_ARRAY(hw, IXGBE_VFMBMEM, i);

	/* Acknowledge receipt */
	vf_mailbox = ixgbevf_read_mailbox_vf(hw);
	vf_mailbox |= IXGBE_VFMAILBOX_ACK;
	IXGBE_WRITE_REG(hw, IXGBE_VFMAILBOX, vf_mailbox);

	/* update stats */
	hw->mbx.stats.msgs_rx++;

	return ret_val;
}

/**
 *  ixgbevf_read_mbx_vf_legacy - Reads a message from the inbox intended for VF
 *  @hw: pointer to the HW structure
 *  @msg: The message buffer
 *  @size: Length of buffer
 *
 *  returns 0 if it successfully read message from buffer
 **/
static s32 ixgbevf_read_mbx_vf_legacy(struct ixgbe_hw *hw, u32 *msg, u16 size)
{
	s32 ret_val = 0;
	u16 i;

	/* lock the mailbox to prevent PF/VF race condition */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret_val = ixgbevf_obtain_mbx_lock_vf(hw);
	if (ret_val)
		goto out_no_read;

	/* copy the message from the mailbox memory buffer */
	for (i = 0; i < size; i++)
		msg[i] = IXGBE_READ_REG_ARRAY(hw, IXGBE_VFMBMEM, i);

	/* Acknowledge receipt and release mailbox, then we're done */
	IXGBE_WRITE_REG(hw, IXGBE_VFMAILBOX, IXGBE_VFMAILBOX_ACK);

	/* update stats */
	hw->mbx.stats.msgs_rx++;

out_no_read:
	return ret_val;
}

/**
<<<<<<< HEAD
 *  ixgbevf_init_mbx_params_vf - set initial values for vf mailbox
 *  @hw: pointer to the HW structure
 *
 *  Initializes the hw->mbx struct to correct values for vf mailbox
=======
 *  ixgbevf_init_mbx_params_vf - set initial values for VF mailbox
 *  @hw: pointer to the HW structure
 *
 *  Initializes the hw->mbx struct to correct values for VF mailbox
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static s32 ixgbevf_init_mbx_params_vf(struct ixgbe_hw *hw)
{
	struct ixgbe_mbx_info *mbx = &hw->mbx;

	/* start mailbox as timed out and let the reset_hw call set the timeout
<<<<<<< HEAD
	 * value to begin communications */
	mbx->timeout = 0;
=======
	 * value to begin communications
	 */
	mbx->timeout = IXGBE_VF_MBX_INIT_TIMEOUT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mbx->udelay = IXGBE_VF_MBX_INIT_DELAY;

	mbx->size = IXGBE_VFMAILBOX_SIZE;

	mbx->stats.msgs_tx = 0;
	mbx->stats.msgs_rx = 0;
	mbx->stats.reqs = 0;
	mbx->stats.acks = 0;
	mbx->stats.rsts = 0;

	return 0;
}

<<<<<<< HEAD
const struct ixgbe_mbx_operations ixgbevf_mbx_ops = {
	.init_params   = ixgbevf_init_mbx_params_vf,
	.read          = ixgbevf_read_mbx_vf,
	.write         = ixgbevf_write_mbx_vf,
	.read_posted   = ixgbevf_read_posted_mbx,
	.write_posted  = ixgbevf_write_posted_mbx,
	.check_for_msg = ixgbevf_check_for_msg_vf,
	.check_for_ack = ixgbevf_check_for_ack_vf,
	.check_for_rst = ixgbevf_check_for_rst_vf,
};

=======
/**
 * ixgbevf_poll_mbx - Wait for message and read it from the mailbox
 * @hw: pointer to the HW structure
 * @msg: The message buffer
 * @size: Length of buffer
 *
 * returns 0 if it successfully read message from buffer
 **/
s32 ixgbevf_poll_mbx(struct ixgbe_hw *hw, u32 *msg, u16 size)
{
	struct ixgbe_mbx_info *mbx = &hw->mbx;
	s32 ret_val = IXGBE_ERR_CONFIG;

	if (!mbx->ops.read || !mbx->ops.check_for_msg || !mbx->timeout)
		return ret_val;

	/* limit read to size of mailbox */
	if (size > mbx->size)
		size = mbx->size;

	ret_val = ixgbevf_poll_for_msg(hw);
	/* if ack received read message, otherwise we timed out */
	if (!ret_val)
		ret_val = mbx->ops.read(hw, msg, size);

	return ret_val;
}

/**
 * ixgbevf_write_mbx - Write a message to the mailbox and wait for ACK
 * @hw: pointer to the HW structure
 * @msg: The message buffer
 * @size: Length of buffer
 *
 * returns 0 if it successfully copied message into the buffer and
 * received an ACK to that message within specified period
 **/
s32 ixgbevf_write_mbx(struct ixgbe_hw *hw, u32 *msg, u16 size)
{
	struct ixgbe_mbx_info *mbx = &hw->mbx;
	s32 ret_val = IXGBE_ERR_CONFIG;

	/**
	 * exit if either we can't write, release
	 * or there is no timeout defined
	 */
	if (!mbx->ops.write || !mbx->ops.check_for_ack || !mbx->ops.release ||
	    !mbx->timeout)
		return ret_val;

	if (size > mbx->size)
		ret_val = IXGBE_ERR_PARAM;
	else
		ret_val = mbx->ops.write(hw, msg, size);

	return ret_val;
}

const struct ixgbe_mbx_operations ixgbevf_mbx_ops = {
	.init_params	= ixgbevf_init_mbx_params_vf,
	.release	= ixgbevf_release_mbx_lock_vf,
	.read		= ixgbevf_read_mbx_vf,
	.write		= ixgbevf_write_mbx_vf,
	.check_for_msg	= ixgbevf_check_for_msg_vf,
	.check_for_ack	= ixgbevf_check_for_ack_vf,
	.check_for_rst	= ixgbevf_check_for_rst_vf,
};

const struct ixgbe_mbx_operations ixgbevf_mbx_ops_legacy = {
	.init_params	= ixgbevf_init_mbx_params_vf,
	.release	= ixgbevf_release_mbx_lock_vf_legacy,
	.read		= ixgbevf_read_mbx_vf_legacy,
	.write		= ixgbevf_write_mbx_vf_legacy,
	.check_for_msg	= ixgbevf_check_for_msg_vf,
	.check_for_ack	= ixgbevf_check_for_ack_vf,
	.check_for_rst	= ixgbevf_check_for_rst_vf,
};

/* Mailbox operations when running on Hyper-V.
 * On Hyper-V, PF/VF communication is not through the
 * hardware mailbox; this communication is through
 * a software mediated path.
 * Most mail box operations are noop while running on
 * Hyper-V.
 */
const struct ixgbe_mbx_operations ixgbevf_hv_mbx_ops = {
	.init_params	= ixgbevf_init_mbx_params_vf,
	.check_for_rst	= ixgbevf_check_for_rst_vf,
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
