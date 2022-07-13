/*
 * Copyright 2008 Advanced Micro Devices, Inc.
 * Copyright 2008 Red Hat Inc.
 * Copyright 2009 Christian König.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors: Christian König
 */
<<<<<<< HEAD
#include "drmP.h"
#include "radeon_drm.h"
#include "radeon.h"
#include "radeon_asic.h"
=======
#include <linux/hdmi.h>
#include <linux/gcd.h>

#include <drm/radeon_drm.h>
#include "radeon.h"
#include "radeon_asic.h"
#include "radeon_audio.h"
#include "r600.h"
#include "r600d.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "atom.h"

/*
 * HDMI color format
 */
enum r600_hdmi_color_format {
	RGB = 0,
	YCC_422 = 1,
	YCC_444 = 2
};

/*
 * IEC60958 status bits
 */
enum r600_hdmi_iec_status_bits {
	AUDIO_STATUS_DIG_ENABLE   = 0x01,
	AUDIO_STATUS_V            = 0x02,
	AUDIO_STATUS_VCFG         = 0x04,
	AUDIO_STATUS_EMPHASIS     = 0x08,
	AUDIO_STATUS_COPYRIGHT    = 0x10,
	AUDIO_STATUS_NONAUDIO     = 0x20,
	AUDIO_STATUS_PROFESSIONAL = 0x40,
	AUDIO_STATUS_LEVEL        = 0x80
};

<<<<<<< HEAD
struct {
	uint32_t Clock;

	int N_32kHz;
	int CTS_32kHz;

	int N_44_1kHz;
	int CTS_44_1kHz;

	int N_48kHz;
	int CTS_48kHz;

} r600_hdmi_ACR[] = {
    /*	     32kHz	  44.1kHz	48kHz    */
    /* Clock      N     CTS      N     CTS      N     CTS */
    {  25174,  4576,  28125,  7007,  31250,  6864,  28125 }, /*  25,20/1.001 MHz */
    {  25200,  4096,  25200,  6272,  28000,  6144,  25200 }, /*  25.20       MHz */
    {  27000,  4096,  27000,  6272,  30000,  6144,  27000 }, /*  27.00       MHz */
    {  27027,  4096,  27027,  6272,  30030,  6144,  27027 }, /*  27.00*1.001 MHz */
    {  54000,  4096,  54000,  6272,  60000,  6144,  54000 }, /*  54.00       MHz */
    {  54054,  4096,  54054,  6272,  60060,  6144,  54054 }, /*  54.00*1.001 MHz */
    {  74175, 11648, 210937, 17836, 234375, 11648, 140625 }, /*  74.25/1.001 MHz */
    {  74250,  4096,  74250,  6272,  82500,  6144,  74250 }, /*  74.25       MHz */
    { 148351, 11648, 421875,  8918, 234375,  5824, 140625 }, /* 148.50/1.001 MHz */
    { 148500,  4096, 148500,  6272, 165000,  6144, 148500 }, /* 148.50       MHz */
    {      0,  4096,      0,  6272,      0,  6144,      0 }  /* Other */
};

/*
 * calculate CTS value if it's not found in the table
 */
static void r600_hdmi_calc_CTS(uint32_t clock, int *CTS, int N, int freq)
{
	if (*CTS == 0)
		*CTS = clock * N / (128 * freq) * 1000;
	DRM_DEBUG("Using ACR timing N=%d CTS=%d for frequency %d\n",
		  N, *CTS, freq);
}

/*
 * update the N and CTS parameters for a given pixel clock rate
 */
static void r600_hdmi_update_ACR(struct drm_encoder *encoder, uint32_t clock)
{
	struct drm_device *dev = encoder->dev;
	struct radeon_device *rdev = dev->dev_private;
	uint32_t offset = to_radeon_encoder(encoder)->hdmi_offset;
	int CTS;
	int N;
	int i;

	for (i = 0; r600_hdmi_ACR[i].Clock != clock && r600_hdmi_ACR[i].Clock != 0; i++);

	CTS = r600_hdmi_ACR[i].CTS_32kHz;
	N = r600_hdmi_ACR[i].N_32kHz;
	r600_hdmi_calc_CTS(clock, &CTS, N, 32000);
	WREG32(offset+R600_HDMI_32kHz_CTS, CTS << 12);
	WREG32(offset+R600_HDMI_32kHz_N, N);

	CTS = r600_hdmi_ACR[i].CTS_44_1kHz;
	N = r600_hdmi_ACR[i].N_44_1kHz;
	r600_hdmi_calc_CTS(clock, &CTS, N, 44100);
	WREG32(offset+R600_HDMI_44_1kHz_CTS, CTS << 12);
	WREG32(offset+R600_HDMI_44_1kHz_N, N);

	CTS = r600_hdmi_ACR[i].CTS_48kHz;
	N = r600_hdmi_ACR[i].N_48kHz;
	r600_hdmi_calc_CTS(clock, &CTS, N, 48000);
	WREG32(offset+R600_HDMI_48kHz_CTS, CTS << 12);
	WREG32(offset+R600_HDMI_48kHz_N, N);
}

/*
 * calculate the crc for a given info frame
 */
static void r600_hdmi_infoframe_checksum(uint8_t packetType,
					 uint8_t versionNumber,
					 uint8_t length,
					 uint8_t *frame)
{
	int i;
	frame[0] = packetType + versionNumber + length;
	for (i = 1; i <= length; i++)
		frame[0] += frame[i];
	frame[0] = 0x100 - frame[0];
=======
static struct r600_audio_pin r600_audio_status(struct radeon_device *rdev)
{
	struct r600_audio_pin status = {};
	uint32_t value;

	value = RREG32(R600_AUDIO_RATE_BPS_CHANNEL);

	/* number of channels */
	status.channels = (value & 0x7) + 1;

	/* bits per sample */
	switch ((value & 0xF0) >> 4) {
	case 0x0:
		status.bits_per_sample = 8;
		break;
	case 0x1:
		status.bits_per_sample = 16;
		break;
	case 0x2:
		status.bits_per_sample = 20;
		break;
	case 0x3:
		status.bits_per_sample = 24;
		break;
	case 0x4:
		status.bits_per_sample = 32;
		break;
	default:
		dev_err(rdev->dev, "Unknown bits per sample 0x%x, using 16\n",
			(int)value);
		status.bits_per_sample = 16;
	}

	/* current sampling rate in HZ */
	if (value & 0x4000)
		status.rate = 44100;
	else
		status.rate = 48000;
	status.rate *= ((value >> 11) & 0x7) + 1;
	status.rate /= ((value >> 8) & 0x7) + 1;

	value = RREG32(R600_AUDIO_STATUS_BITS);

	/* iec 60958 status bits */
	status.status_bits = value & 0xff;

	/* iec 60958 category code */
	status.category_code = (value >> 8) & 0xff;

	return status;
}

/*
 * update all hdmi interfaces with current audio parameters
 */
void r600_audio_update_hdmi(struct work_struct *work)
{
	struct radeon_device *rdev = container_of(work, struct radeon_device,
						  audio_work);
	struct drm_device *dev = rdev->ddev;
	struct r600_audio_pin audio_status = r600_audio_status(rdev);
	struct drm_encoder *encoder;
	bool changed = false;

	if (rdev->audio.pin[0].channels != audio_status.channels ||
	    rdev->audio.pin[0].rate != audio_status.rate ||
	    rdev->audio.pin[0].bits_per_sample != audio_status.bits_per_sample ||
	    rdev->audio.pin[0].status_bits != audio_status.status_bits ||
	    rdev->audio.pin[0].category_code != audio_status.category_code) {
		rdev->audio.pin[0] = audio_status;
		changed = true;
	}

	list_for_each_entry(encoder, &dev->mode_config.encoder_list, head) {
		if (!radeon_encoder_is_digital(encoder))
			continue;
		if (changed || r600_hdmi_buffer_status_changed(encoder))
			r600_hdmi_update_audio_settings(encoder);
	}
}

/* enable the audio stream */
void r600_audio_enable(struct radeon_device *rdev,
		       struct r600_audio_pin *pin,
		       u8 enable_mask)
{
	u32 tmp = RREG32(AZ_HOT_PLUG_CONTROL);

	if (!pin)
		return;

	if (enable_mask) {
		tmp |= AUDIO_ENABLED;
		if (enable_mask & 1)
			tmp |= PIN0_AUDIO_ENABLED;
		if (enable_mask & 2)
			tmp |= PIN1_AUDIO_ENABLED;
		if (enable_mask & 4)
			tmp |= PIN2_AUDIO_ENABLED;
		if (enable_mask & 8)
			tmp |= PIN3_AUDIO_ENABLED;
	} else {
		tmp &= ~(AUDIO_ENABLED |
			 PIN0_AUDIO_ENABLED |
			 PIN1_AUDIO_ENABLED |
			 PIN2_AUDIO_ENABLED |
			 PIN3_AUDIO_ENABLED);
	}

	WREG32(AZ_HOT_PLUG_CONTROL, tmp);
}

struct r600_audio_pin *r600_audio_get_pin(struct radeon_device *rdev)
{
	/* only one pin on 6xx-NI */
	return &rdev->audio.pin[0];
}

void r600_hdmi_update_acr(struct drm_encoder *encoder, long offset,
	const struct radeon_hdmi_acr *acr)
{
	struct drm_device *dev = encoder->dev;
	struct radeon_device *rdev = dev->dev_private;

	/* DCE 3.0 uses register that's normally for CRC_CONTROL */
	uint32_t acr_ctl = ASIC_IS_DCE3(rdev) ? DCE3_HDMI0_ACR_PACKET_CONTROL :
				       HDMI0_ACR_PACKET_CONTROL;
	WREG32_P(acr_ctl + offset,
		HDMI0_ACR_SOURCE |		/* select SW CTS value */
		HDMI0_ACR_AUTO_SEND,	/* allow hw to sent ACR packets when required */
		~(HDMI0_ACR_SOURCE |
		HDMI0_ACR_AUTO_SEND));

	WREG32_P(HDMI0_ACR_32_0 + offset,
		HDMI0_ACR_CTS_32(acr->cts_32khz),
		~HDMI0_ACR_CTS_32_MASK);
	WREG32_P(HDMI0_ACR_32_1 + offset,
		HDMI0_ACR_N_32(acr->n_32khz),
		~HDMI0_ACR_N_32_MASK);

	WREG32_P(HDMI0_ACR_44_0 + offset,
		HDMI0_ACR_CTS_44(acr->cts_44_1khz),
		~HDMI0_ACR_CTS_44_MASK);
	WREG32_P(HDMI0_ACR_44_1 + offset,
		HDMI0_ACR_N_44(acr->n_44_1khz),
		~HDMI0_ACR_N_44_MASK);

	WREG32_P(HDMI0_ACR_48_0 + offset,
		HDMI0_ACR_CTS_48(acr->cts_48khz),
		~HDMI0_ACR_CTS_48_MASK);
	WREG32_P(HDMI0_ACR_48_1 + offset,
		HDMI0_ACR_N_48(acr->n_48khz),
		~HDMI0_ACR_N_48_MASK);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * build a HDMI Video Info Frame
 */
<<<<<<< HEAD
static void r600_hdmi_videoinfoframe(
	struct drm_encoder *encoder,
	enum r600_hdmi_color_format color_format,
	int active_information_present,
	uint8_t active_format_aspect_ratio,
	uint8_t scan_information,
	uint8_t colorimetry,
	uint8_t ex_colorimetry,
	uint8_t quantization,
	int ITC,
	uint8_t picture_aspect_ratio,
	uint8_t video_format_identification,
	uint8_t pixel_repetition,
	uint8_t non_uniform_picture_scaling,
	uint8_t bar_info_data_valid,
	uint16_t top_bar,
	uint16_t bottom_bar,
	uint16_t left_bar,
	uint16_t right_bar
)
{
	struct drm_device *dev = encoder->dev;
	struct radeon_device *rdev = dev->dev_private;
	uint32_t offset = to_radeon_encoder(encoder)->hdmi_offset;

	uint8_t frame[14];

	frame[0x0] = 0;
	frame[0x1] =
		(scan_information & 0x3) |
		((bar_info_data_valid & 0x3) << 2) |
		((active_information_present & 0x1) << 4) |
		((color_format & 0x3) << 5);
	frame[0x2] =
		(active_format_aspect_ratio & 0xF) |
		((picture_aspect_ratio & 0x3) << 4) |
		((colorimetry & 0x3) << 6);
	frame[0x3] =
		(non_uniform_picture_scaling & 0x3) |
		((quantization & 0x3) << 2) |
		((ex_colorimetry & 0x7) << 4) |
		((ITC & 0x1) << 7);
	frame[0x4] = (video_format_identification & 0x7F);
	frame[0x5] = (pixel_repetition & 0xF);
	frame[0x6] = (top_bar & 0xFF);
	frame[0x7] = (top_bar >> 8);
	frame[0x8] = (bottom_bar & 0xFF);
	frame[0x9] = (bottom_bar >> 8);
	frame[0xA] = (left_bar & 0xFF);
	frame[0xB] = (left_bar >> 8);
	frame[0xC] = (right_bar & 0xFF);
	frame[0xD] = (right_bar >> 8);

	r600_hdmi_infoframe_checksum(0x82, 0x02, 0x0D, frame);
	/* Our header values (type, version, length) should be alright, Intel
	 * is using the same. Checksum function also seems to be OK, it works
	 * fine for audio infoframe. However calculated value is always lower
	 * by 2 in comparison to fglrx. It breaks displaying anything in case
	 * of TVs that strictly check the checksum. Hack it manually here to
	 * workaround this issue. */
	frame[0x0] += 2;

	WREG32(offset+R600_HDMI_VIDEOINFOFRAME_0,
		frame[0x0] | (frame[0x1] << 8) | (frame[0x2] << 16) | (frame[0x3] << 24));
	WREG32(offset+R600_HDMI_VIDEOINFOFRAME_1,
		frame[0x4] | (frame[0x5] << 8) | (frame[0x6] << 16) | (frame[0x7] << 24));
	WREG32(offset+R600_HDMI_VIDEOINFOFRAME_2,
		frame[0x8] | (frame[0x9] << 8) | (frame[0xA] << 16) | (frame[0xB] << 24));
	WREG32(offset+R600_HDMI_VIDEOINFOFRAME_3,
		frame[0xC] | (frame[0xD] << 8));
=======
void r600_set_avi_packet(struct radeon_device *rdev, u32 offset,
			 unsigned char *buffer, size_t size)
{
	uint8_t *frame = buffer + 3;

	WREG32(HDMI0_AVI_INFO0 + offset,
		frame[0x0] | (frame[0x1] << 8) | (frame[0x2] << 16) | (frame[0x3] << 24));
	WREG32(HDMI0_AVI_INFO1 + offset,
		frame[0x4] | (frame[0x5] << 8) | (frame[0x6] << 16) | (frame[0x7] << 24));
	WREG32(HDMI0_AVI_INFO2 + offset,
		frame[0x8] | (frame[0x9] << 8) | (frame[0xA] << 16) | (frame[0xB] << 24));
	WREG32(HDMI0_AVI_INFO3 + offset,
		frame[0xC] | (frame[0xD] << 8) | (buffer[1] << 24));

	WREG32_OR(HDMI0_INFOFRAME_CONTROL1 + offset,
		  HDMI0_AVI_INFO_LINE(2));	/* anything other than 0 */

	WREG32_OR(HDMI0_INFOFRAME_CONTROL0 + offset,
		  HDMI0_AVI_INFO_SEND |	/* enable AVI info frames */
		  HDMI0_AVI_INFO_CONT);	/* send AVI info frames every frame/field */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * build a Audio Info Frame
 */
<<<<<<< HEAD
static void r600_hdmi_audioinfoframe(
	struct drm_encoder *encoder,
	uint8_t channel_count,
	uint8_t coding_type,
	uint8_t sample_size,
	uint8_t sample_frequency,
	uint8_t format,
	uint8_t channel_allocation,
	uint8_t level_shift,
	int downmix_inhibit
)
{
	struct drm_device *dev = encoder->dev;
	struct radeon_device *rdev = dev->dev_private;
	uint32_t offset = to_radeon_encoder(encoder)->hdmi_offset;

	uint8_t frame[11];

	frame[0x0] = 0;
	frame[0x1] = (channel_count & 0x7) | ((coding_type & 0xF) << 4);
	frame[0x2] = (sample_size & 0x3) | ((sample_frequency & 0x7) << 2);
	frame[0x3] = format;
	frame[0x4] = channel_allocation;
	frame[0x5] = ((level_shift & 0xF) << 3) | ((downmix_inhibit & 0x1) << 7);
	frame[0x6] = 0;
	frame[0x7] = 0;
	frame[0x8] = 0;
	frame[0x9] = 0;
	frame[0xA] = 0;

	r600_hdmi_infoframe_checksum(0x84, 0x01, 0x0A, frame);

	WREG32(offset+R600_HDMI_AUDIOINFOFRAME_0,
		frame[0x0] | (frame[0x1] << 8) | (frame[0x2] << 16) | (frame[0x3] << 24));
	WREG32(offset+R600_HDMI_AUDIOINFOFRAME_1,
=======
static void r600_hdmi_update_audio_infoframe(struct drm_encoder *encoder,
					     const void *buffer, size_t size)
{
	struct drm_device *dev = encoder->dev;
	struct radeon_device *rdev = dev->dev_private;
	struct radeon_encoder *radeon_encoder = to_radeon_encoder(encoder);
	struct radeon_encoder_atom_dig *dig = radeon_encoder->enc_priv;
	uint32_t offset = dig->afmt->offset;
	const u8 *frame = buffer + 3;

	WREG32(HDMI0_AUDIO_INFO0 + offset,
		frame[0x0] | (frame[0x1] << 8) | (frame[0x2] << 16) | (frame[0x3] << 24));
	WREG32(HDMI0_AUDIO_INFO1 + offset,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		frame[0x4] | (frame[0x5] << 8) | (frame[0x6] << 16) | (frame[0x8] << 24));
}

/*
 * test if audio buffer is filled enough to start playing
 */
<<<<<<< HEAD
static int r600_hdmi_is_audio_buffer_filled(struct drm_encoder *encoder)
{
	struct drm_device *dev = encoder->dev;
	struct radeon_device *rdev = dev->dev_private;
	uint32_t offset = to_radeon_encoder(encoder)->hdmi_offset;

	return (RREG32(offset+R600_HDMI_STATUS) & 0x10) != 0;
=======
static bool r600_hdmi_is_audio_buffer_filled(struct drm_encoder *encoder)
{
	struct drm_device *dev = encoder->dev;
	struct radeon_device *rdev = dev->dev_private;
	struct radeon_encoder *radeon_encoder = to_radeon_encoder(encoder);
	struct radeon_encoder_atom_dig *dig = radeon_encoder->enc_priv;
	uint32_t offset = dig->afmt->offset;

	return (RREG32(HDMI0_STATUS + offset) & 0x10) != 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * have buffer status changed since last call?
 */
int r600_hdmi_buffer_status_changed(struct drm_encoder *encoder)
{
	struct radeon_encoder *radeon_encoder = to_radeon_encoder(encoder);
<<<<<<< HEAD
	int status, result;

	if (!radeon_encoder->hdmi_offset)
		return 0;

	status = r600_hdmi_is_audio_buffer_filled(encoder);
	result = radeon_encoder->hdmi_buffer_status != status;
	radeon_encoder->hdmi_buffer_status = status;
=======
	struct radeon_encoder_atom_dig *dig = radeon_encoder->enc_priv;
	int status, result;

	if (!dig->afmt || !dig->afmt->enabled)
		return 0;

	status = r600_hdmi_is_audio_buffer_filled(encoder);
	result = dig->afmt->last_buffer_filled_status != status;
	dig->afmt->last_buffer_filled_status = status;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return result;
}

/*
 * write the audio workaround status to the hardware
 */
void r600_hdmi_audio_workaround(struct drm_encoder *encoder)
{
	struct drm_device *dev = encoder->dev;
	struct radeon_device *rdev = dev->dev_private;
	struct radeon_encoder *radeon_encoder = to_radeon_encoder(encoder);
<<<<<<< HEAD
	uint32_t offset = radeon_encoder->hdmi_offset;

	if (!offset)
		return;

	if (!radeon_encoder->hdmi_audio_workaround ||
		r600_hdmi_is_audio_buffer_filled(encoder)) {

		/* disable audio workaround */
		WREG32_P(offset+R600_HDMI_CNTL, 0x00000001, ~0x00001001);

	} else {
		/* enable audio workaround */
		WREG32_P(offset+R600_HDMI_CNTL, 0x00001001, ~0x00001001);
	}
}


/*
 * update the info frames with the data from the current display mode
 */
void r600_hdmi_setmode(struct drm_encoder *encoder, struct drm_display_mode *mode)
{
	struct drm_device *dev = encoder->dev;
	struct radeon_device *rdev = dev->dev_private;
	uint32_t offset = to_radeon_encoder(encoder)->hdmi_offset;

	if (ASIC_IS_DCE5(rdev))
		return;

	if (!offset)
		return;

	r600_audio_set_clock(encoder, mode->clock);

	WREG32(offset+R600_HDMI_UNKNOWN_0, 0x1000);
	WREG32(offset+R600_HDMI_UNKNOWN_1, 0x0);
	WREG32(offset+R600_HDMI_UNKNOWN_2, 0x1000);

	r600_hdmi_update_ACR(encoder, mode->clock);

	WREG32(offset+R600_HDMI_VIDEOCNTL, 0x13);

	WREG32(offset+R600_HDMI_VERSION, 0x202);

	r600_hdmi_videoinfoframe(encoder, RGB, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	/* it's unknown what these bits do excatly, but it's indeed quite useful for debugging */
	WREG32(offset+R600_HDMI_AUDIO_DEBUG_0, 0x00FFFFFF);
	WREG32(offset+R600_HDMI_AUDIO_DEBUG_1, 0x007FFFFF);
	WREG32(offset+R600_HDMI_AUDIO_DEBUG_2, 0x00000001);
	WREG32(offset+R600_HDMI_AUDIO_DEBUG_3, 0x00000001);

	r600_hdmi_audio_workaround(encoder);

	/* audio packets per line, does anyone know how to calc this ? */
	WREG32_P(offset+R600_HDMI_CNTL, 0x00040000, ~0x001F0000);
}

/*
 * update settings with current parameters from audio engine
=======
	struct radeon_encoder_atom_dig *dig = radeon_encoder->enc_priv;
	uint32_t offset = dig->afmt->offset;
	bool hdmi_audio_workaround = false; /* FIXME */
	u32 value;

	if (!hdmi_audio_workaround ||
	    r600_hdmi_is_audio_buffer_filled(encoder))
		value = 0; /* disable workaround */
	else
		value = HDMI0_AUDIO_TEST_EN; /* enable workaround */
	WREG32_P(HDMI0_AUDIO_PACKET_CONTROL + offset,
		 value, ~HDMI0_AUDIO_TEST_EN);
}

void r600_hdmi_audio_set_dto(struct radeon_device *rdev,
			     struct radeon_crtc *crtc, unsigned int clock)
{
	struct radeon_encoder *radeon_encoder;
	struct radeon_encoder_atom_dig *dig;

	if (!crtc)
		return;

	radeon_encoder = to_radeon_encoder(crtc->encoder);
	dig = radeon_encoder->enc_priv;

	if (!dig)
		return;

	if (dig->dig_encoder == 0) {
		WREG32(DCCG_AUDIO_DTO0_PHASE, 24000 * 100);
		WREG32(DCCG_AUDIO_DTO0_MODULE, clock * 100);
		WREG32(DCCG_AUDIO_DTO_SELECT, 0); /* select DTO0 */
	} else {
		WREG32(DCCG_AUDIO_DTO1_PHASE, 24000 * 100);
		WREG32(DCCG_AUDIO_DTO1_MODULE, clock * 100);
		WREG32(DCCG_AUDIO_DTO_SELECT, 1); /* select DTO1 */
	}
}

void r600_set_vbi_packet(struct drm_encoder *encoder, u32 offset)
{
	struct drm_device *dev = encoder->dev;
	struct radeon_device *rdev = dev->dev_private;

	WREG32_OR(HDMI0_VBI_PACKET_CONTROL + offset,
		HDMI0_NULL_SEND |	/* send null packets when required */
		HDMI0_GC_SEND |		/* send general control packets */
		HDMI0_GC_CONT);		/* send general control packets every frame */
}

void r600_set_audio_packet(struct drm_encoder *encoder, u32 offset)
{
	struct drm_device *dev = encoder->dev;
	struct radeon_device *rdev = dev->dev_private;

	WREG32_P(HDMI0_AUDIO_PACKET_CONTROL + offset,
		HDMI0_AUDIO_SAMPLE_SEND |			/* send audio packets */
		HDMI0_AUDIO_DELAY_EN(1) |			/* default audio delay */
		HDMI0_AUDIO_PACKETS_PER_LINE(3) |	/* should be suffient for all audio modes and small enough for all hblanks */
		HDMI0_60958_CS_UPDATE,				/* allow 60958 channel status fields to be updated */
		~(HDMI0_AUDIO_SAMPLE_SEND |
		HDMI0_AUDIO_DELAY_EN_MASK |
		HDMI0_AUDIO_PACKETS_PER_LINE_MASK |
		HDMI0_60958_CS_UPDATE));

	WREG32_OR(HDMI0_INFOFRAME_CONTROL0 + offset,
		HDMI0_AUDIO_INFO_SEND |		/* enable audio info frames (frames won't be set until audio is enabled) */
		HDMI0_AUDIO_INFO_UPDATE);	/* required for audio info values to be updated */

	WREG32_P(HDMI0_INFOFRAME_CONTROL1 + offset,
		HDMI0_AUDIO_INFO_LINE(2),	/* anything other than 0 */
		~HDMI0_AUDIO_INFO_LINE_MASK);

	WREG32_AND(HDMI0_GENERIC_PACKET_CONTROL + offset,
		~(HDMI0_GENERIC0_SEND |
		HDMI0_GENERIC0_CONT |
		HDMI0_GENERIC0_UPDATE |
		HDMI0_GENERIC1_SEND |
		HDMI0_GENERIC1_CONT |
		HDMI0_GENERIC0_LINE_MASK |
		HDMI0_GENERIC1_LINE_MASK));

	WREG32_P(HDMI0_60958_0 + offset,
		HDMI0_60958_CS_CHANNEL_NUMBER_L(1),
		~(HDMI0_60958_CS_CHANNEL_NUMBER_L_MASK |
		HDMI0_60958_CS_CLOCK_ACCURACY_MASK));

	WREG32_P(HDMI0_60958_1 + offset,
		HDMI0_60958_CS_CHANNEL_NUMBER_R(2),
		~HDMI0_60958_CS_CHANNEL_NUMBER_R_MASK);
}

void r600_set_mute(struct drm_encoder *encoder, u32 offset, bool mute)
{
	struct drm_device *dev = encoder->dev;
	struct radeon_device *rdev = dev->dev_private;

	if (mute)
		WREG32_OR(HDMI0_GC + offset, HDMI0_GC_AVMUTE);
	else
		WREG32_AND(HDMI0_GC + offset, ~HDMI0_GC_AVMUTE);
}

/**
 * r600_hdmi_update_audio_settings - Update audio infoframe
 *
 * @encoder: drm encoder
 *
 * Gets info about current audio stream and updates audio infoframe.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
void r600_hdmi_update_audio_settings(struct drm_encoder *encoder)
{
	struct drm_device *dev = encoder->dev;
	struct radeon_device *rdev = dev->dev_private;
<<<<<<< HEAD
	uint32_t offset = to_radeon_encoder(encoder)->hdmi_offset;

	int channels = r600_audio_channels(rdev);
	int rate = r600_audio_rate(rdev);
	int bps = r600_audio_bits_per_sample(rdev);
	uint8_t status_bits = r600_audio_status_bits(rdev);
	uint8_t category_code = r600_audio_category_code(rdev);

	uint32_t iec;

	if (!offset)
		return;

	DRM_DEBUG("%s with %d channels, %d Hz sampling rate, %d bits per sample,\n",
		 r600_hdmi_is_audio_buffer_filled(encoder) ? "playing" : "stopped",
		channels, rate, bps);
	DRM_DEBUG("0x%02X IEC60958 status bits and 0x%02X category code\n",
		  (int)status_bits, (int)category_code);

	iec = 0;
	if (status_bits & AUDIO_STATUS_PROFESSIONAL)
		iec |= 1 << 0;
	if (status_bits & AUDIO_STATUS_NONAUDIO)
		iec |= 1 << 1;
	if (status_bits & AUDIO_STATUS_COPYRIGHT)
		iec |= 1 << 2;
	if (status_bits & AUDIO_STATUS_EMPHASIS)
		iec |= 1 << 3;

	iec |= category_code << 8;

	switch (rate) {
	case  32000: iec |= 0x3 << 24; break;
	case  44100: iec |= 0x0 << 24; break;
	case  88200: iec |= 0x8 << 24; break;
	case 176400: iec |= 0xc << 24; break;
	case  48000: iec |= 0x2 << 24; break;
	case  96000: iec |= 0xa << 24; break;
	case 192000: iec |= 0xe << 24; break;
	}

	WREG32(offset+R600_HDMI_IEC60958_1, iec);

	iec = 0;
	switch (bps) {
	case 16: iec |= 0x2; break;
	case 20: iec |= 0x3; break;
	case 24: iec |= 0xb; break;
	}
	if (status_bits & AUDIO_STATUS_V)
		iec |= 0x5 << 16;

	WREG32_P(offset+R600_HDMI_IEC60958_2, iec, ~0x5000f);

	/* 0x021 or 0x031 sets the audio frame length */
	WREG32(offset+R600_HDMI_AUDIOCNTL, 0x31);
	r600_hdmi_audioinfoframe(encoder, channels-1, 0, 0, 0, 0, 0, 0, 0);

	r600_hdmi_audio_workaround(encoder);
}

static int r600_hdmi_find_free_block(struct drm_device *dev)
{
	struct radeon_device *rdev = dev->dev_private;
	struct drm_encoder *encoder;
	struct radeon_encoder *radeon_encoder;
	bool free_blocks[3] = { true, true, true };

	list_for_each_entry(encoder, &dev->mode_config.encoder_list, head) {
		radeon_encoder = to_radeon_encoder(encoder);
		switch (radeon_encoder->hdmi_offset) {
		case R600_HDMI_BLOCK1:
			free_blocks[0] = false;
			break;
		case R600_HDMI_BLOCK2:
			free_blocks[1] = false;
			break;
		case R600_HDMI_BLOCK3:
			free_blocks[2] = false;
			break;
		}
	}

	if (rdev->family == CHIP_RS600 || rdev->family == CHIP_RS690 ||
	    rdev->family == CHIP_RS740) {
		return free_blocks[0] ? R600_HDMI_BLOCK1 : 0;
	} else if (rdev->family >= CHIP_R600) {
		if (free_blocks[0])
			return R600_HDMI_BLOCK1;
		else if (free_blocks[1])
			return R600_HDMI_BLOCK2;
	}
	return 0;
}

static void r600_hdmi_assign_block(struct drm_encoder *encoder)
{
	struct drm_device *dev = encoder->dev;
	struct radeon_device *rdev = dev->dev_private;
	struct radeon_encoder *radeon_encoder = to_radeon_encoder(encoder);
	struct radeon_encoder_atom_dig *dig = radeon_encoder->enc_priv;

	u16 eg_offsets[] = {
		EVERGREEN_CRTC0_REGISTER_OFFSET,
		EVERGREEN_CRTC1_REGISTER_OFFSET,
		EVERGREEN_CRTC2_REGISTER_OFFSET,
		EVERGREEN_CRTC3_REGISTER_OFFSET,
		EVERGREEN_CRTC4_REGISTER_OFFSET,
		EVERGREEN_CRTC5_REGISTER_OFFSET,
	};

	if (!dig) {
		dev_err(rdev->dev, "Enabling HDMI on non-dig encoder\n");
		return;
	}

	if (ASIC_IS_DCE5(rdev)) {
		/* TODO */
	} else if (ASIC_IS_DCE4(rdev)) {
		if (dig->dig_encoder >= ARRAY_SIZE(eg_offsets)) {
			dev_err(rdev->dev, "Enabling HDMI on unknown dig\n");
			return;
		}
		radeon_encoder->hdmi_offset = EVERGREEN_HDMI_BASE +
						eg_offsets[dig->dig_encoder];
		radeon_encoder->hdmi_config_offset = radeon_encoder->hdmi_offset
						+ EVERGREEN_HDMI_CONFIG_OFFSET;
	} else if (ASIC_IS_DCE3(rdev)) {
		radeon_encoder->hdmi_offset = dig->dig_encoder ?
			R600_HDMI_BLOCK3 : R600_HDMI_BLOCK1;
		if (ASIC_IS_DCE32(rdev))
			radeon_encoder->hdmi_config_offset = dig->dig_encoder ?
				R600_HDMI_CONFIG2 : R600_HDMI_CONFIG1;
	} else if (rdev->family >= CHIP_R600 || rdev->family == CHIP_RS600 ||
		   rdev->family == CHIP_RS690 || rdev->family == CHIP_RS740) {
		radeon_encoder->hdmi_offset = r600_hdmi_find_free_block(dev);
	}
=======
	struct radeon_encoder *radeon_encoder = to_radeon_encoder(encoder);
	struct radeon_encoder_atom_dig *dig = radeon_encoder->enc_priv;
	struct r600_audio_pin audio = r600_audio_status(rdev);
	uint8_t buffer[HDMI_INFOFRAME_HEADER_SIZE + HDMI_AUDIO_INFOFRAME_SIZE];
	struct hdmi_audio_infoframe frame;
	uint32_t offset;
	uint32_t value;
	ssize_t err;

	if (!dig->afmt || !dig->afmt->enabled)
		return;
	offset = dig->afmt->offset;

	DRM_DEBUG("%s with %d channels, %d Hz sampling rate, %d bits per sample,\n",
		 r600_hdmi_is_audio_buffer_filled(encoder) ? "playing" : "stopped",
		  audio.channels, audio.rate, audio.bits_per_sample);
	DRM_DEBUG("0x%02X IEC60958 status bits and 0x%02X category code\n",
		  (int)audio.status_bits, (int)audio.category_code);

	err = hdmi_audio_infoframe_init(&frame);
	if (err < 0) {
		DRM_ERROR("failed to setup audio infoframe\n");
		return;
	}

	frame.channels = audio.channels;

	err = hdmi_audio_infoframe_pack(&frame, buffer, sizeof(buffer));
	if (err < 0) {
		DRM_ERROR("failed to pack audio infoframe\n");
		return;
	}

	value = RREG32(HDMI0_AUDIO_PACKET_CONTROL + offset);
	if (value & HDMI0_AUDIO_TEST_EN)
		WREG32(HDMI0_AUDIO_PACKET_CONTROL + offset,
		       value & ~HDMI0_AUDIO_TEST_EN);

	WREG32_OR(HDMI0_CONTROL + offset,
		  HDMI0_ERROR_ACK);

	WREG32_AND(HDMI0_INFOFRAME_CONTROL0 + offset,
		   ~HDMI0_AUDIO_INFO_SOURCE);

	r600_hdmi_update_audio_infoframe(encoder, buffer, sizeof(buffer));

	WREG32_OR(HDMI0_INFOFRAME_CONTROL0 + offset,
		  HDMI0_AUDIO_INFO_CONT |
		  HDMI0_AUDIO_INFO_UPDATE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * enable the HDMI engine
 */
<<<<<<< HEAD
void r600_hdmi_enable(struct drm_encoder *encoder)
=======
void r600_hdmi_enable(struct drm_encoder *encoder, bool enable)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct drm_device *dev = encoder->dev;
	struct radeon_device *rdev = dev->dev_private;
	struct radeon_encoder *radeon_encoder = to_radeon_encoder(encoder);
<<<<<<< HEAD
	uint32_t offset;

	if (ASIC_IS_DCE5(rdev))
		return;

	if (!radeon_encoder->hdmi_offset) {
		r600_hdmi_assign_block(encoder);
		if (!radeon_encoder->hdmi_offset) {
			dev_warn(rdev->dev, "Could not find HDMI block for "
				"0x%x encoder\n", radeon_encoder->encoder_id);
			return;
		}
	}

	offset = radeon_encoder->hdmi_offset;
	if (ASIC_IS_DCE5(rdev)) {
		/* TODO */
	} else if (ASIC_IS_DCE4(rdev)) {
		WREG32_P(radeon_encoder->hdmi_config_offset + 0xc, 0x1, ~0x1);
	} else if (ASIC_IS_DCE32(rdev)) {
		WREG32_P(radeon_encoder->hdmi_config_offset + 0x4, 0x1, ~0x1);
	} else if (ASIC_IS_DCE3(rdev)) {
		/* TODO */
	} else if (ASIC_IS_DCE2(rdev)) {
		switch (radeon_encoder->encoder_id) {
		case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_TMDS1:
			WREG32_P(AVIVO_TMDSA_CNTL, AVIVO_TMDSA_CNTL_HDMI_EN,
				 ~AVIVO_TMDSA_CNTL_HDMI_EN);
			WREG32(offset + R600_HDMI_ENABLE, 0x101);
			break;
		case ENCODER_OBJECT_ID_INTERNAL_LVTM1:
			WREG32_P(AVIVO_LVTMA_CNTL, AVIVO_LVTMA_CNTL_HDMI_EN,
				 ~AVIVO_LVTMA_CNTL_HDMI_EN);
			WREG32(offset + R600_HDMI_ENABLE, 0x105);
			break;
		default:
			dev_err(rdev->dev, "Unknown HDMI output type\n");
			break;
		}
	}

	if (rdev->irq.installed
	    && rdev->family != CHIP_RS600
	    && rdev->family != CHIP_RS690
	    && rdev->family != CHIP_RS740
	    && !ASIC_IS_DCE4(rdev)) {
		/* if irq is available use it */
		rdev->irq.hdmi[offset == R600_HDMI_BLOCK1 ? 0 : 1] = true;
		radeon_irq_set(rdev);

		r600_audio_disable_polling(encoder);
	} else {
		/* if not fallback to polling */
		r600_audio_enable_polling(encoder);
	}

	DRM_DEBUG("Enabling HDMI interface @ 0x%04X for encoder 0x%x\n",
		radeon_encoder->hdmi_offset, radeon_encoder->encoder_id);
}

/*
 * disable the HDMI engine
 */
void r600_hdmi_disable(struct drm_encoder *encoder)
{
	struct drm_device *dev = encoder->dev;
	struct radeon_device *rdev = dev->dev_private;
	struct radeon_encoder *radeon_encoder = to_radeon_encoder(encoder);
	uint32_t offset;

	if (ASIC_IS_DCE5(rdev))
		return;

	offset = radeon_encoder->hdmi_offset;
	if (!offset) {
		dev_err(rdev->dev, "Disabling not enabled HDMI\n");
		return;
	}

	DRM_DEBUG("Disabling HDMI interface @ 0x%04X for encoder 0x%x\n",
		offset, radeon_encoder->encoder_id);

	/* disable irq */
	rdev->irq.hdmi[offset == R600_HDMI_BLOCK1 ? 0 : 1] = false;
	radeon_irq_set(rdev);

	/* disable polling */
	r600_audio_disable_polling(encoder);

	if (ASIC_IS_DCE5(rdev)) {
		/* TODO */
	} else if (ASIC_IS_DCE4(rdev)) {
		WREG32_P(radeon_encoder->hdmi_config_offset + 0xc, 0, ~0x1);
	} else if (ASIC_IS_DCE32(rdev)) {
		WREG32_P(radeon_encoder->hdmi_config_offset + 0x4, 0, ~0x1);
	} else if (ASIC_IS_DCE2(rdev) && !ASIC_IS_DCE3(rdev)) {
		switch (radeon_encoder->encoder_id) {
		case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_TMDS1:
			WREG32_P(AVIVO_TMDSA_CNTL, 0,
				 ~AVIVO_TMDSA_CNTL_HDMI_EN);
			WREG32(offset + R600_HDMI_ENABLE, 0);
			break;
		case ENCODER_OBJECT_ID_INTERNAL_LVTM1:
			WREG32_P(AVIVO_LVTMA_CNTL, 0,
				 ~AVIVO_LVTMA_CNTL_HDMI_EN);
			WREG32(offset + R600_HDMI_ENABLE, 0);
			break;
		default:
			dev_err(rdev->dev, "Unknown HDMI output type\n");
			break;
		}
	}

	radeon_encoder->hdmi_offset = 0;
	radeon_encoder->hdmi_config_offset = 0;
}
=======
	struct radeon_encoder_atom_dig *dig = radeon_encoder->enc_priv;
	u32 hdmi = HDMI0_ERROR_ACK;

	if (!dig || !dig->afmt)
		return;

	/* Older chipsets require setting HDMI and routing manually */
	if (!ASIC_IS_DCE3(rdev)) {
		if (enable)
			hdmi |= HDMI0_ENABLE;
		switch (radeon_encoder->encoder_id) {
		case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_TMDS1:
			if (enable) {
				WREG32_OR(AVIVO_TMDSA_CNTL, AVIVO_TMDSA_CNTL_HDMI_EN);
				hdmi |= HDMI0_STREAM(HDMI0_STREAM_TMDSA);
			} else {
				WREG32_AND(AVIVO_TMDSA_CNTL, ~AVIVO_TMDSA_CNTL_HDMI_EN);
			}
			break;
		case ENCODER_OBJECT_ID_INTERNAL_LVTM1:
			if (enable) {
				WREG32_OR(AVIVO_LVTMA_CNTL, AVIVO_LVTMA_CNTL_HDMI_EN);
				hdmi |= HDMI0_STREAM(HDMI0_STREAM_LVTMA);
			} else {
				WREG32_AND(AVIVO_LVTMA_CNTL, ~AVIVO_LVTMA_CNTL_HDMI_EN);
			}
			break;
		case ENCODER_OBJECT_ID_INTERNAL_DDI:
			if (enable) {
				WREG32_OR(DDIA_CNTL, DDIA_HDMI_EN);
				hdmi |= HDMI0_STREAM(HDMI0_STREAM_DDIA);
			} else {
				WREG32_AND(DDIA_CNTL, ~DDIA_HDMI_EN);
			}
			break;
		case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DVO1:
			if (enable)
				hdmi |= HDMI0_STREAM(HDMI0_STREAM_DVOA);
			break;
		default:
			dev_err(rdev->dev, "Invalid encoder for HDMI: 0x%X\n",
				radeon_encoder->encoder_id);
			break;
		}
		WREG32(HDMI0_CONTROL + dig->afmt->offset, hdmi);
	}

	if (rdev->irq.installed) {
		/* if irq is available use it */
		/* XXX: shouldn't need this on any asics.  Double check DCE2/3 */
		if (enable)
			radeon_irq_kms_enable_afmt(rdev, dig->afmt->id);
		else
			radeon_irq_kms_disable_afmt(rdev, dig->afmt->id);
	}

	dig->afmt->enabled = enable;

	DRM_DEBUG("%sabling HDMI interface @ 0x%04X for encoder 0x%x\n",
		  enable ? "En" : "Dis", dig->afmt->offset, radeon_encoder->encoder_id);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
