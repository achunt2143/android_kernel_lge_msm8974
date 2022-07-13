<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  Copyright (c) by Jaroslav Kysela <perex@perex.cz>
 * 
 *  Misc memory accessors
<<<<<<< HEAD
 *
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */

#include <linux/export.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <sound/core.h>
=======
 */

#include <linux/export.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <sound/core.h>
#include <sound/pcm.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * copy_to_user_fromio - copy data from mmio-space to user-space
 * @dst: the destination pointer on user-space
 * @src: the source pointer on mmio
 * @count: the data size to copy in bytes
 *
 * Copies the data from mmio-space to user-space.
 *
<<<<<<< HEAD
 * Returns zero if successful, or non-zero on failure.
 */
int copy_to_user_fromio(void __user *dst, const volatile void __iomem *src, size_t count)
{
#if defined(__i386__) || defined(CONFIG_SPARC32)
	return copy_to_user(dst, (const void __force*)src, count) ? -EFAULT : 0;
=======
 * Return: Zero if successful, or non-zero on failure.
 */
int copy_to_user_fromio(void __user *dst, const volatile void __iomem *src, size_t count)
{
	struct iov_iter iter;

	if (import_ubuf(ITER_DEST, dst, count, &iter))
		return -EFAULT;
	return copy_to_iter_fromio(&iter, (const void __iomem *)src, count);
}
EXPORT_SYMBOL(copy_to_user_fromio);

/**
 * copy_to_iter_fromio - copy data from mmio-space to iov_iter
 * @dst: the destination iov_iter
 * @src: the source pointer on mmio
 * @count: the data size to copy in bytes
 *
 * Copies the data from mmio-space to iov_iter.
 *
 * Return: Zero if successful, or non-zero on failure.
 */
int copy_to_iter_fromio(struct iov_iter *dst, const void __iomem *src,
			size_t count)
{
#if defined(__i386__) || defined(CONFIG_SPARC32)
	return copy_to_iter((const void __force *)src, count, dst) == count ? 0 : -EFAULT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else
	char buf[256];
	while (count) {
		size_t c = count;
		if (c > sizeof(buf))
			c = sizeof(buf);
		memcpy_fromio(buf, (void __iomem *)src, c);
<<<<<<< HEAD
		if (copy_to_user(dst, buf, c))
			return -EFAULT;
		count -= c;
		dst += c;
=======
		if (copy_to_iter(buf, c, dst) != c)
			return -EFAULT;
		count -= c;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		src += c;
	}
	return 0;
#endif
}
<<<<<<< HEAD

EXPORT_SYMBOL(copy_to_user_fromio);
=======
EXPORT_SYMBOL(copy_to_iter_fromio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * copy_from_user_toio - copy data from user-space to mmio-space
 * @dst: the destination pointer on mmio-space
 * @src: the source pointer on user-space
 * @count: the data size to copy in bytes
 *
 * Copies the data from user-space to mmio-space.
 *
<<<<<<< HEAD
 * Returns zero if successful, or non-zero on failure.
 */
int copy_from_user_toio(volatile void __iomem *dst, const void __user *src, size_t count)
{
#if defined(__i386__) || defined(CONFIG_SPARC32)
	return copy_from_user((void __force *)dst, src, count) ? -EFAULT : 0;
=======
 * Return: Zero if successful, or non-zero on failure.
 */
int copy_from_user_toio(volatile void __iomem *dst, const void __user *src, size_t count)
{
	struct iov_iter iter;

	if (import_ubuf(ITER_SOURCE, (void __user *)src, count, &iter))
		return -EFAULT;
	return copy_from_iter_toio((void __iomem *)dst, &iter, count);
}
EXPORT_SYMBOL(copy_from_user_toio);

/**
 * copy_from_iter_toio - copy data from iov_iter to mmio-space
 * @dst: the destination pointer on mmio-space
 * @src: the source iov_iter
 * @count: the data size to copy in bytes
 *
 * Copies the data from iov_iter to mmio-space.
 *
 * Return: Zero if successful, or non-zero on failure.
 */
int copy_from_iter_toio(void __iomem *dst, struct iov_iter *src, size_t count)
{
#if defined(__i386__) || defined(CONFIG_SPARC32)
	return copy_from_iter((void __force *)dst, count, src) == count ? 0 : -EFAULT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else
	char buf[256];
	while (count) {
		size_t c = count;
		if (c > sizeof(buf))
			c = sizeof(buf);
<<<<<<< HEAD
		if (copy_from_user(buf, src, c))
=======
		if (copy_from_iter(buf, c, src) != c)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EFAULT;
		memcpy_toio(dst, buf, c);
		count -= c;
		dst += c;
<<<<<<< HEAD
		src += c;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return 0;
#endif
}
<<<<<<< HEAD

EXPORT_SYMBOL(copy_from_user_toio);
=======
EXPORT_SYMBOL(copy_from_iter_toio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
