/*
 * User address space access functions.
 *
 *  For licencing details see kernel-base/COPYING
 */

<<<<<<< HEAD
#include <linux/highmem.h>
#include <linux/module.h>

#include <asm/word-at-a-time.h>

/*
 * best effort, GUP based copy_from_user() that is NMI-safe
=======
#include <linux/uaccess.h>
#include <linux/export.h>
#include <linux/instrumented.h>

#include <asm/tlbflush.h>

/**
 * copy_from_user_nmi - NMI safe copy from user
 * @to:		Pointer to the destination buffer
 * @from:	Pointer to a user space address of the current task
 * @n:		Number of bytes to copy
 *
 * Returns: The number of not copied bytes. 0 is success, i.e. all bytes copied
 *
 * Contrary to other copy_from_user() variants this function can be called
 * from NMI context. Despite the name it is not restricted to be called
 * from NMI context. It is safe to be called from any other context as
 * well. It disables pagefaults across the copy which means a fault will
 * abort the copy.
 *
 * For NMI context invocations this relies on the nested NMI work to allow
 * atomic faults from the NMI path; the nested NMI paths are careful to
 * preserve CR2.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
unsigned long
copy_from_user_nmi(void *to, const void __user *from, unsigned long n)
{
<<<<<<< HEAD
	unsigned long offset, addr = (unsigned long)from;
	unsigned long size, len = 0;
	struct page *page;
	void *map;
	int ret;

	do {
		ret = __get_user_pages_fast(addr, 1, 0, &page);
		if (!ret)
			break;

		offset = addr & (PAGE_SIZE - 1);
		size = min(PAGE_SIZE - offset, n - len);

		map = kmap_atomic(page);
		memcpy(to, map+offset, size);
		kunmap_atomic(map);
		put_page(page);

		len  += size;
		to   += size;
		addr += size;

	} while (len < n);

	return len;
}
EXPORT_SYMBOL_GPL(copy_from_user_nmi);

static inline unsigned long count_bytes(unsigned long mask)
{
	mask = (mask - 1) & ~mask;
	mask >>= 7;
	return count_masked_bytes(mask);
}

/*
 * Do a strncpy, return length of string without final '\0'.
 * 'count' is the user-supplied count (return 'count' if we
 * hit it), 'max' is the address space maximum (and we return
 * -EFAULT if we hit it).
 */
static inline long do_strncpy_from_user(char *dst, const char __user *src, long count, unsigned long max)
{
	long res = 0;

	/*
	 * Truncate 'max' to the user-specified limit, so that
	 * we only have one limit we need to check in the loop
	 */
	if (max > count)
		max = count;

	while (max >= sizeof(unsigned long)) {
		unsigned long c;

		/* Fall back to byte-at-a-time if we get a page fault */
		if (unlikely(__get_user(c,(unsigned long __user *)(src+res))))
			break;
		/* This can write a few bytes past the NUL character, but that's ok */
		*(unsigned long *)(dst+res) = c;
		c = has_zero(c);
		if (c)
			return res + count_bytes(c);
		res += sizeof(unsigned long);
		max -= sizeof(unsigned long);
	}

	while (max) {
		char c;

		if (unlikely(__get_user(c,src+res)))
			return -EFAULT;
		dst[res] = c;
		if (!c)
			return res;
		res++;
		max--;
	}

	/*
	 * Uhhuh. We hit 'max'. But was that the user-specified maximum
	 * too? If so, that's ok - we got as much as the user asked for.
	 */
	if (res >= count)
		return res;

	/*
	 * Nope: we hit the address space limit, and we still had more
	 * characters the caller would have wanted. That's an EFAULT.
	 */
	return -EFAULT;
}

/**
 * strncpy_from_user: - Copy a NUL terminated string from userspace.
 * @dst:   Destination address, in kernel space.  This buffer must be at
 *         least @count bytes long.
 * @src:   Source address, in user space.
 * @count: Maximum number of bytes to copy, including the trailing NUL.
 *
 * Copies a NUL-terminated string from userspace to kernel space.
 *
 * On success, returns the length of the string (not including the trailing
 * NUL).
 *
 * If access to userspace fails, returns -EFAULT (some data may have been
 * copied).
 *
 * If @count is smaller than the length of the string, copies @count bytes
 * and returns @count.
 */
long
strncpy_from_user(char *dst, const char __user *src, long count)
{
	unsigned long max_addr, src_addr;

	if (unlikely(count <= 0))
		return 0;

	max_addr = current_thread_info()->addr_limit.seg;
	src_addr = (unsigned long)src;
	if (likely(src_addr < max_addr)) {
		unsigned long max = max_addr - src_addr;
		return do_strncpy_from_user(dst, src, count, max);
	}
	return -EFAULT;
}
EXPORT_SYMBOL(strncpy_from_user);
=======
	unsigned long ret;

	if (!__access_ok(from, n))
		return n;

	if (!nmi_uaccess_okay())
		return n;

	/*
	 * Even though this function is typically called from NMI/IRQ context
	 * disable pagefaults so that its behaviour is consistent even when
	 * called from other contexts.
	 */
	pagefault_disable();
	instrument_copy_from_user_before(to, from, n);
	ret = raw_copy_from_user(to, from, n);
	instrument_copy_from_user_after(to, from, n, ret);
	pagefault_enable();

	return ret;
}
EXPORT_SYMBOL_GPL(copy_from_user_nmi);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
