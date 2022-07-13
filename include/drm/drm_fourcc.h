/*
<<<<<<< HEAD
 * Copyright 2011 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * VA LINUX SYSTEMS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef DRM_FOURCC_H
#define DRM_FOURCC_H

#include <linux/types.h>

#define fourcc_code(a, b, c, d) ((__u32)(a) | ((__u32)(b) << 8) | \
				 ((__u32)(c) << 16) | ((__u32)(d) << 24))

#define DRM_FORMAT_BIG_ENDIAN (1<<31) /* format is big endian instead of little endian */

/* color index */
#define DRM_FORMAT_C8		fourcc_code('C', '8', ' ', ' ') /* [7:0] C */

/* 8 bpp RGB */
#define DRM_FORMAT_RGB332	fourcc_code('R', 'G', 'B', '8') /* [7:0] R:G:B 3:3:2 */
#define DRM_FORMAT_BGR233	fourcc_code('B', 'G', 'R', '8') /* [7:0] B:G:R 2:3:3 */

/* 16 bpp RGB */
#define DRM_FORMAT_XRGB4444	fourcc_code('X', 'R', '1', '2') /* [15:0] x:R:G:B 4:4:4:4 little endian */
#define DRM_FORMAT_XBGR4444	fourcc_code('X', 'B', '1', '2') /* [15:0] x:B:G:R 4:4:4:4 little endian */
#define DRM_FORMAT_RGBX4444	fourcc_code('R', 'X', '1', '2') /* [15:0] R:G:B:x 4:4:4:4 little endian */
#define DRM_FORMAT_BGRX4444	fourcc_code('B', 'X', '1', '2') /* [15:0] B:G:R:x 4:4:4:4 little endian */

#define DRM_FORMAT_ARGB4444	fourcc_code('A', 'R', '1', '2') /* [15:0] A:R:G:B 4:4:4:4 little endian */
#define DRM_FORMAT_ABGR4444	fourcc_code('A', 'B', '1', '2') /* [15:0] A:B:G:R 4:4:4:4 little endian */
#define DRM_FORMAT_RGBA4444	fourcc_code('R', 'A', '1', '2') /* [15:0] R:G:B:A 4:4:4:4 little endian */
#define DRM_FORMAT_BGRA4444	fourcc_code('B', 'A', '1', '2') /* [15:0] B:G:R:A 4:4:4:4 little endian */

#define DRM_FORMAT_XRGB1555	fourcc_code('X', 'R', '1', '5') /* [15:0] x:R:G:B 1:5:5:5 little endian */
#define DRM_FORMAT_XBGR1555	fourcc_code('X', 'B', '1', '5') /* [15:0] x:B:G:R 1:5:5:5 little endian */
#define DRM_FORMAT_RGBX5551	fourcc_code('R', 'X', '1', '5') /* [15:0] R:G:B:x 5:5:5:1 little endian */
#define DRM_FORMAT_BGRX5551	fourcc_code('B', 'X', '1', '5') /* [15:0] B:G:R:x 5:5:5:1 little endian */

#define DRM_FORMAT_ARGB1555	fourcc_code('A', 'R', '1', '5') /* [15:0] A:R:G:B 1:5:5:5 little endian */
#define DRM_FORMAT_ABGR1555	fourcc_code('A', 'B', '1', '5') /* [15:0] A:B:G:R 1:5:5:5 little endian */
#define DRM_FORMAT_RGBA5551	fourcc_code('R', 'A', '1', '5') /* [15:0] R:G:B:A 5:5:5:1 little endian */
#define DRM_FORMAT_BGRA5551	fourcc_code('B', 'A', '1', '5') /* [15:0] B:G:R:A 5:5:5:1 little endian */

#define DRM_FORMAT_RGB565	fourcc_code('R', 'G', '1', '6') /* [15:0] R:G:B 5:6:5 little endian */
#define DRM_FORMAT_BGR565	fourcc_code('B', 'G', '1', '6') /* [15:0] B:G:R 5:6:5 little endian */

/* 24 bpp RGB */
#define DRM_FORMAT_RGB888	fourcc_code('R', 'G', '2', '4') /* [23:0] R:G:B little endian */
#define DRM_FORMAT_BGR888	fourcc_code('B', 'G', '2', '4') /* [23:0] B:G:R little endian */

/* 32 bpp RGB */
#define DRM_FORMAT_XRGB8888	fourcc_code('X', 'R', '2', '4') /* [31:0] x:R:G:B 8:8:8:8 little endian */
#define DRM_FORMAT_XBGR8888	fourcc_code('X', 'B', '2', '4') /* [31:0] x:B:G:R 8:8:8:8 little endian */
#define DRM_FORMAT_RGBX8888	fourcc_code('R', 'X', '2', '4') /* [31:0] R:G:B:x 8:8:8:8 little endian */
#define DRM_FORMAT_BGRX8888	fourcc_code('B', 'X', '2', '4') /* [31:0] B:G:R:x 8:8:8:8 little endian */

#define DRM_FORMAT_ARGB8888	fourcc_code('A', 'R', '2', '4') /* [31:0] A:R:G:B 8:8:8:8 little endian */
#define DRM_FORMAT_ABGR8888	fourcc_code('A', 'B', '2', '4') /* [31:0] A:B:G:R 8:8:8:8 little endian */
#define DRM_FORMAT_RGBA8888	fourcc_code('R', 'A', '2', '4') /* [31:0] R:G:B:A 8:8:8:8 little endian */
#define DRM_FORMAT_BGRA8888	fourcc_code('B', 'A', '2', '4') /* [31:0] B:G:R:A 8:8:8:8 little endian */

#define DRM_FORMAT_XRGB2101010	fourcc_code('X', 'R', '3', '0') /* [31:0] x:R:G:B 2:10:10:10 little endian */
#define DRM_FORMAT_XBGR2101010	fourcc_code('X', 'B', '3', '0') /* [31:0] x:B:G:R 2:10:10:10 little endian */
#define DRM_FORMAT_RGBX1010102	fourcc_code('R', 'X', '3', '0') /* [31:0] R:G:B:x 10:10:10:2 little endian */
#define DRM_FORMAT_BGRX1010102	fourcc_code('B', 'X', '3', '0') /* [31:0] B:G:R:x 10:10:10:2 little endian */

#define DRM_FORMAT_ARGB2101010	fourcc_code('A', 'R', '3', '0') /* [31:0] A:R:G:B 2:10:10:10 little endian */
#define DRM_FORMAT_ABGR2101010	fourcc_code('A', 'B', '3', '0') /* [31:0] A:B:G:R 2:10:10:10 little endian */
#define DRM_FORMAT_RGBA1010102	fourcc_code('R', 'A', '3', '0') /* [31:0] R:G:B:A 10:10:10:2 little endian */
#define DRM_FORMAT_BGRA1010102	fourcc_code('B', 'A', '3', '0') /* [31:0] B:G:R:A 10:10:10:2 little endian */

/* packed YCbCr */
#define DRM_FORMAT_YUYV		fourcc_code('Y', 'U', 'Y', 'V') /* [31:0] Cr0:Y1:Cb0:Y0 8:8:8:8 little endian */
#define DRM_FORMAT_YVYU		fourcc_code('Y', 'V', 'Y', 'U') /* [31:0] Cb0:Y1:Cr0:Y0 8:8:8:8 little endian */
#define DRM_FORMAT_UYVY		fourcc_code('U', 'Y', 'V', 'Y') /* [31:0] Y1:Cr0:Y0:Cb0 8:8:8:8 little endian */
#define DRM_FORMAT_VYUY		fourcc_code('V', 'Y', 'U', 'Y') /* [31:0] Y1:Cb0:Y0:Cr0 8:8:8:8 little endian */

#define DRM_FORMAT_AYUV		fourcc_code('A', 'Y', 'U', 'V') /* [31:0] A:Y:Cb:Cr 8:8:8:8 little endian */

/*
 * 2 plane YCbCr
 * index 0 = Y plane, [7:0] Y
 * index 1 = Cr:Cb plane, [15:0] Cr:Cb little endian
 * or
 * index 1 = Cb:Cr plane, [15:0] Cb:Cr little endian
 */
#define DRM_FORMAT_NV12		fourcc_code('N', 'V', '1', '2') /* 2x2 subsampled Cr:Cb plane */
#define DRM_FORMAT_NV21		fourcc_code('N', 'V', '2', '1') /* 2x2 subsampled Cb:Cr plane */
#define DRM_FORMAT_NV16		fourcc_code('N', 'V', '1', '6') /* 2x1 subsampled Cr:Cb plane */
#define DRM_FORMAT_NV61		fourcc_code('N', 'V', '6', '1') /* 2x1 subsampled Cb:Cr plane */

/* 2 non contiguous plane YCbCr */
#define DRM_FORMAT_NV12M	fourcc_code('N', 'M', '1', '2') /* 2x2 subsampled Cr:Cb plane */
#define DRM_FORMAT_NV12MT	fourcc_code('T', 'M', '1', '2') /* 2x2 subsampled Cr:Cb plane 64x32 macroblocks */

/*
 * 3 plane YCbCr
 * index 0: Y plane, [7:0] Y
 * index 1: Cb plane, [7:0] Cb
 * index 2: Cr plane, [7:0] Cr
 * or
 * index 1: Cr plane, [7:0] Cr
 * index 2: Cb plane, [7:0] Cb
 */
#define DRM_FORMAT_YUV410	fourcc_code('Y', 'U', 'V', '9') /* 4x4 subsampled Cb (1) and Cr (2) planes */
#define DRM_FORMAT_YVU410	fourcc_code('Y', 'V', 'U', '9') /* 4x4 subsampled Cr (1) and Cb (2) planes */
#define DRM_FORMAT_YUV411	fourcc_code('Y', 'U', '1', '1') /* 4x1 subsampled Cb (1) and Cr (2) planes */
#define DRM_FORMAT_YVU411	fourcc_code('Y', 'V', '1', '1') /* 4x1 subsampled Cr (1) and Cb (2) planes */
#define DRM_FORMAT_YUV420	fourcc_code('Y', 'U', '1', '2') /* 2x2 subsampled Cb (1) and Cr (2) planes */
#define DRM_FORMAT_YVU420	fourcc_code('Y', 'V', '1', '2') /* 2x2 subsampled Cr (1) and Cb (2) planes */
#define DRM_FORMAT_YUV422	fourcc_code('Y', 'U', '1', '6') /* 2x1 subsampled Cb (1) and Cr (2) planes */
#define DRM_FORMAT_YVU422	fourcc_code('Y', 'V', '1', '6') /* 2x1 subsampled Cr (1) and Cb (2) planes */
#define DRM_FORMAT_YUV444	fourcc_code('Y', 'U', '2', '4') /* non-subsampled Cb (1) and Cr (2) planes */
#define DRM_FORMAT_YVU444	fourcc_code('Y', 'V', '2', '4') /* non-subsampled Cr (1) and Cb (2) planes */

/* 3 non contiguous plane YCbCr */
#define DRM_FORMAT_YUV420M	fourcc_code('Y', 'M', '1', '2') /* 2x2 subsampled Cb (1) and Cr (2) planes */

#endif /* DRM_FOURCC_H */
=======
 * Copyright (c) 2016 Laurent Pinchart <laurent.pinchart@ideasonboard.com>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the name of the copyright holders not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  The copyright holders make no representations
 * about the suitability of this software for any purpose.  It is provided "as
 * is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 */
#ifndef __DRM_FOURCC_H__
#define __DRM_FOURCC_H__

#include <linux/math.h>
#include <linux/types.h>
#include <uapi/drm/drm_fourcc.h>

/**
 * DRM_FORMAT_MAX_PLANES - maximum number of planes a DRM format can have
 */
#define DRM_FORMAT_MAX_PLANES	4u

/*
 * DRM formats are little endian.  Define host endian variants for the
 * most common formats here, to reduce the #ifdefs needed in drivers.
 *
 * Note that the DRM_FORMAT_BIG_ENDIAN flag should only be used in
 * case the format can't be specified otherwise, so we don't end up
 * with two values describing the same format.
 */
#ifdef __BIG_ENDIAN
# define DRM_FORMAT_HOST_XRGB1555     (DRM_FORMAT_XRGB1555         |	\
				       DRM_FORMAT_BIG_ENDIAN)
# define DRM_FORMAT_HOST_RGB565       (DRM_FORMAT_RGB565           |	\
				       DRM_FORMAT_BIG_ENDIAN)
# define DRM_FORMAT_HOST_XRGB8888     DRM_FORMAT_BGRX8888
# define DRM_FORMAT_HOST_ARGB8888     DRM_FORMAT_BGRA8888
#else
# define DRM_FORMAT_HOST_XRGB1555     DRM_FORMAT_XRGB1555
# define DRM_FORMAT_HOST_RGB565       DRM_FORMAT_RGB565
# define DRM_FORMAT_HOST_XRGB8888     DRM_FORMAT_XRGB8888
# define DRM_FORMAT_HOST_ARGB8888     DRM_FORMAT_ARGB8888
#endif

struct drm_device;
struct drm_mode_fb_cmd2;

/**
 * struct drm_format_info - information about a DRM format
 */
struct drm_format_info {
	/** @format: 4CC format identifier (DRM_FORMAT_*) */
	u32 format;

	/**
	 * @depth:
	 *
	 * Color depth (number of bits per pixel excluding padding bits),
	 * valid for a subset of RGB formats only. This is a legacy field, do
	 * not use in new code and set to 0 for new formats.
	 */
	u8 depth;

	/** @num_planes: Number of color planes (1 to 3) */
	u8 num_planes;

	union {
		/**
		 * @cpp:
		 *
		 * Number of bytes per pixel (per plane), this is aliased with
		 * @char_per_block. It is deprecated in favour of using the
		 * triplet @char_per_block, @block_w, @block_h for better
		 * describing the pixel format.
		 */
		u8 cpp[DRM_FORMAT_MAX_PLANES];

		/**
		 * @char_per_block:
		 *
		 * Number of bytes per block (per plane), where blocks are
		 * defined as a rectangle of pixels which are stored next to
		 * each other in a byte aligned memory region. Together with
		 * @block_w and @block_h this is used to properly describe tiles
		 * in tiled formats or to describe groups of pixels in packed
		 * formats for which the memory needed for a single pixel is not
		 * byte aligned.
		 *
		 * @cpp has been kept for historical reasons because there are
		 * a lot of places in drivers where it's used. In drm core for
		 * generic code paths the preferred way is to use
		 * @char_per_block, drm_format_info_block_width() and
		 * drm_format_info_block_height() which allows handling both
		 * block and non-block formats in the same way.
		 *
		 * For formats that are intended to be used only with non-linear
		 * modifiers both @cpp and @char_per_block must be 0 in the
		 * generic format table. Drivers could supply accurate
		 * information from their drm_mode_config.get_format_info hook
		 * if they want the core to be validating the pitch.
		 */
		u8 char_per_block[DRM_FORMAT_MAX_PLANES];
	};

	/**
	 * @block_w:
	 *
	 * Block width in pixels, this is intended to be accessed through
	 * drm_format_info_block_width()
	 */
	u8 block_w[DRM_FORMAT_MAX_PLANES];

	/**
	 * @block_h:
	 *
	 * Block height in pixels, this is intended to be accessed through
	 * drm_format_info_block_height()
	 */
	u8 block_h[DRM_FORMAT_MAX_PLANES];

	/** @hsub: Horizontal chroma subsampling factor */
	u8 hsub;
	/** @vsub: Vertical chroma subsampling factor */
	u8 vsub;

	/** @has_alpha: Does the format embeds an alpha component? */
	bool has_alpha;

	/** @is_yuv: Is it a YUV format? */
	bool is_yuv;

	/** @is_color_indexed: Is it a color-indexed format? */
	bool is_color_indexed;
};

/**
 * drm_format_info_is_yuv_packed - check that the format info matches a YUV
 * format with data laid in a single plane
 * @info: format info
 *
 * Returns:
 * A boolean indicating whether the format info matches a packed YUV format.
 */
static inline bool
drm_format_info_is_yuv_packed(const struct drm_format_info *info)
{
	return info->is_yuv && info->num_planes == 1;
}

/**
 * drm_format_info_is_yuv_semiplanar - check that the format info matches a YUV
 * format with data laid in two planes (luminance and chrominance)
 * @info: format info
 *
 * Returns:
 * A boolean indicating whether the format info matches a semiplanar YUV format.
 */
static inline bool
drm_format_info_is_yuv_semiplanar(const struct drm_format_info *info)
{
	return info->is_yuv && info->num_planes == 2;
}

/**
 * drm_format_info_is_yuv_planar - check that the format info matches a YUV
 * format with data laid in three planes (one for each YUV component)
 * @info: format info
 *
 * Returns:
 * A boolean indicating whether the format info matches a planar YUV format.
 */
static inline bool
drm_format_info_is_yuv_planar(const struct drm_format_info *info)
{
	return info->is_yuv && info->num_planes == 3;
}

/**
 * drm_format_info_is_yuv_sampling_410 - check that the format info matches a
 * YUV format with 4:1:0 sub-sampling
 * @info: format info
 *
 * Returns:
 * A boolean indicating whether the format info matches a YUV format with 4:1:0
 * sub-sampling.
 */
static inline bool
drm_format_info_is_yuv_sampling_410(const struct drm_format_info *info)
{
	return info->is_yuv && info->hsub == 4 && info->vsub == 4;
}

/**
 * drm_format_info_is_yuv_sampling_411 - check that the format info matches a
 * YUV format with 4:1:1 sub-sampling
 * @info: format info
 *
 * Returns:
 * A boolean indicating whether the format info matches a YUV format with 4:1:1
 * sub-sampling.
 */
static inline bool
drm_format_info_is_yuv_sampling_411(const struct drm_format_info *info)
{
	return info->is_yuv && info->hsub == 4 && info->vsub == 1;
}

/**
 * drm_format_info_is_yuv_sampling_420 - check that the format info matches a
 * YUV format with 4:2:0 sub-sampling
 * @info: format info
 *
 * Returns:
 * A boolean indicating whether the format info matches a YUV format with 4:2:0
 * sub-sampling.
 */
static inline bool
drm_format_info_is_yuv_sampling_420(const struct drm_format_info *info)
{
	return info->is_yuv && info->hsub == 2 && info->vsub == 2;
}

/**
 * drm_format_info_is_yuv_sampling_422 - check that the format info matches a
 * YUV format with 4:2:2 sub-sampling
 * @info: format info
 *
 * Returns:
 * A boolean indicating whether the format info matches a YUV format with 4:2:2
 * sub-sampling.
 */
static inline bool
drm_format_info_is_yuv_sampling_422(const struct drm_format_info *info)
{
	return info->is_yuv && info->hsub == 2 && info->vsub == 1;
}

/**
 * drm_format_info_is_yuv_sampling_444 - check that the format info matches a
 * YUV format with 4:4:4 sub-sampling
 * @info: format info
 *
 * Returns:
 * A boolean indicating whether the format info matches a YUV format with 4:4:4
 * sub-sampling.
 */
static inline bool
drm_format_info_is_yuv_sampling_444(const struct drm_format_info *info)
{
	return info->is_yuv && info->hsub == 1 && info->vsub == 1;
}

/**
 * drm_format_info_plane_width - width of the plane given the first plane
 * @info: pixel format info
 * @width: width of the first plane
 * @plane: plane index
 *
 * Returns:
 * The width of @plane, given that the width of the first plane is @width.
 */
static inline
int drm_format_info_plane_width(const struct drm_format_info *info, int width,
				int plane)
{
	if (!info || plane >= info->num_planes)
		return 0;

	if (plane == 0)
		return width;

	return DIV_ROUND_UP(width, info->hsub);
}

/**
 * drm_format_info_plane_height - height of the plane given the first plane
 * @info: pixel format info
 * @height: height of the first plane
 * @plane: plane index
 *
 * Returns:
 * The height of @plane, given that the height of the first plane is @height.
 */
static inline
int drm_format_info_plane_height(const struct drm_format_info *info, int height,
				 int plane)
{
	if (!info || plane >= info->num_planes)
		return 0;

	if (plane == 0)
		return height;

	return DIV_ROUND_UP(height, info->vsub);
}

const struct drm_format_info *__drm_format_info(u32 format);
const struct drm_format_info *drm_format_info(u32 format);
const struct drm_format_info *
drm_get_format_info(struct drm_device *dev,
		    const struct drm_mode_fb_cmd2 *mode_cmd);
uint32_t drm_mode_legacy_fb_format(uint32_t bpp, uint32_t depth);
uint32_t drm_driver_legacy_fb_format(struct drm_device *dev,
				     uint32_t bpp, uint32_t depth);
unsigned int drm_format_info_block_width(const struct drm_format_info *info,
					 int plane);
unsigned int drm_format_info_block_height(const struct drm_format_info *info,
					  int plane);
unsigned int drm_format_info_bpp(const struct drm_format_info *info, int plane);
uint64_t drm_format_info_min_pitch(const struct drm_format_info *info,
				   int plane, unsigned int buffer_width);

#endif /* __DRM_FOURCC_H__ */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
