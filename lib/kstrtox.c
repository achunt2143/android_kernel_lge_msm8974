<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Convert integer string representation to an integer.
 * If an integer doesn't fit into specified type, -E is returned.
 *
 * Integer starts with optional sign.
 * kstrtou*() functions do not accept sign "-".
 *
 * Radix 0 means autodetection: leading "0x" implies radix 16,
 * leading "0" implies radix 8, otherwise radix is 10.
 * Autodetection hints work after optional sign, but not before.
 *
 * If -E is returned, result is not touched.
 */
#include <linux/ctype.h>
#include <linux/errno.h>
<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/math64.h>
#include <linux/export.h>
#include <linux/types.h>
#include <asm/uaccess.h>
#include "kstrtox.h"

=======
#include <linux/export.h>
#include <linux/kstrtox.h>
#include <linux/math64.h>
#include <linux/types.h>
#include <linux/uaccess.h>

#include "kstrtox.h"

noinline
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
const char *_parse_integer_fixup_radix(const char *s, unsigned int *base)
{
	if (*base == 0) {
		if (s[0] == '0') {
			if (_tolower(s[1]) == 'x' && isxdigit(s[2]))
				*base = 16;
			else
				*base = 8;
		} else
			*base = 10;
	}
	if (*base == 16 && s[0] == '0' && _tolower(s[1]) == 'x')
		s += 2;
	return s;
}

/*
 * Convert non-negative integer string representation in explicitly given radix
<<<<<<< HEAD
 * to an integer.
=======
 * to an integer. A maximum of max_chars characters will be converted.
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Return number of characters consumed maybe or-ed with overflow bit.
 * If overflow occurs, result integer (incorrect) is still returned.
 *
 * Don't you dare use this function.
 */
<<<<<<< HEAD
unsigned int _parse_integer(const char *s, unsigned int base, unsigned long long *p)
{
	unsigned long long res;
	unsigned int rv;
	int overflow;

	res = 0;
	rv = 0;
	overflow = 0;
	while (*s) {
		unsigned int val;

		if ('0' <= *s && *s <= '9')
			val = *s - '0';
		else if ('a' <= _tolower(*s) && _tolower(*s) <= 'f')
			val = _tolower(*s) - 'a' + 10;
=======
noinline
unsigned int _parse_integer_limit(const char *s, unsigned int base, unsigned long long *p,
				  size_t max_chars)
{
	unsigned long long res;
	unsigned int rv;

	res = 0;
	rv = 0;
	while (max_chars--) {
		unsigned int c = *s;
		unsigned int lc = _tolower(c);
		unsigned int val;

		if ('0' <= c && c <= '9')
			val = c - '0';
		else if ('a' <= lc && lc <= 'f')
			val = lc - 'a' + 10;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		else
			break;

		if (val >= base)
			break;
		/*
		 * Check for overflow only if we are within range of
		 * it in the max base we support (16)
		 */
		if (unlikely(res & (~0ull << 60))) {
			if (res > div_u64(ULLONG_MAX - val, base))
<<<<<<< HEAD
				overflow = 1;
=======
				rv |= KSTRTOX_OVERFLOW;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		res = res * base + val;
		rv++;
		s++;
	}
	*p = res;
<<<<<<< HEAD
	if (overflow)
		rv |= KSTRTOX_OVERFLOW;
	return rv;
}

=======
	return rv;
}

noinline
unsigned int _parse_integer(const char *s, unsigned int base, unsigned long long *p)
{
	return _parse_integer_limit(s, base, p, INT_MAX);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int _kstrtoull(const char *s, unsigned int base, unsigned long long *res)
{
	unsigned long long _res;
	unsigned int rv;

	s = _parse_integer_fixup_radix(s, &base);
	rv = _parse_integer(s, base, &_res);
	if (rv & KSTRTOX_OVERFLOW)
		return -ERANGE;
<<<<<<< HEAD
	rv &= ~KSTRTOX_OVERFLOW;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rv == 0)
		return -EINVAL;
	s += rv;
	if (*s == '\n')
		s++;
	if (*s)
		return -EINVAL;
	*res = _res;
	return 0;
}

<<<<<<< HEAD
=======
/**
 * kstrtoull - convert a string to an unsigned long long
 * @s: The start of the string. The string must be null-terminated, and may also
 *  include a single newline before its terminating null. The first character
 *  may also be a plus sign, but not a minus sign.
 * @base: The number base to use. The maximum supported base is 16. If base is
 *  given as 0, then the base of the string is automatically detected with the
 *  conventional semantics - If it begins with 0x the number will be parsed as a
 *  hexadecimal (case insensitive), if it otherwise begins with 0, it will be
 *  parsed as an octal number. Otherwise it will be parsed as a decimal.
 * @res: Where to write the result of the conversion on success.
 *
 * Returns 0 on success, -ERANGE on overflow and -EINVAL on parsing error.
 * Preferred over simple_strtoull(). Return code must be checked.
 */
noinline
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int kstrtoull(const char *s, unsigned int base, unsigned long long *res)
{
	if (s[0] == '+')
		s++;
	return _kstrtoull(s, base, res);
}
EXPORT_SYMBOL(kstrtoull);

<<<<<<< HEAD
=======
/**
 * kstrtoll - convert a string to a long long
 * @s: The start of the string. The string must be null-terminated, and may also
 *  include a single newline before its terminating null. The first character
 *  may also be a plus sign or a minus sign.
 * @base: The number base to use. The maximum supported base is 16. If base is
 *  given as 0, then the base of the string is automatically detected with the
 *  conventional semantics - If it begins with 0x the number will be parsed as a
 *  hexadecimal (case insensitive), if it otherwise begins with 0, it will be
 *  parsed as an octal number. Otherwise it will be parsed as a decimal.
 * @res: Where to write the result of the conversion on success.
 *
 * Returns 0 on success, -ERANGE on overflow and -EINVAL on parsing error.
 * Preferred over simple_strtoll(). Return code must be checked.
 */
noinline
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int kstrtoll(const char *s, unsigned int base, long long *res)
{
	unsigned long long tmp;
	int rv;

	if (s[0] == '-') {
		rv = _kstrtoull(s + 1, base, &tmp);
		if (rv < 0)
			return rv;
<<<<<<< HEAD
		if ((long long)(-tmp) >= 0)
=======
		if ((long long)-tmp > 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -ERANGE;
		*res = -tmp;
	} else {
		rv = kstrtoull(s, base, &tmp);
		if (rv < 0)
			return rv;
		if ((long long)tmp < 0)
			return -ERANGE;
		*res = tmp;
	}
	return 0;
}
EXPORT_SYMBOL(kstrtoll);

/* Internal, do not use. */
int _kstrtoul(const char *s, unsigned int base, unsigned long *res)
{
	unsigned long long tmp;
	int rv;

	rv = kstrtoull(s, base, &tmp);
	if (rv < 0)
		return rv;
<<<<<<< HEAD
	if (tmp != (unsigned long long)(unsigned long)tmp)
=======
	if (tmp != (unsigned long)tmp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ERANGE;
	*res = tmp;
	return 0;
}
EXPORT_SYMBOL(_kstrtoul);

/* Internal, do not use. */
int _kstrtol(const char *s, unsigned int base, long *res)
{
	long long tmp;
	int rv;

	rv = kstrtoll(s, base, &tmp);
	if (rv < 0)
		return rv;
<<<<<<< HEAD
	if (tmp != (long long)(long)tmp)
=======
	if (tmp != (long)tmp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ERANGE;
	*res = tmp;
	return 0;
}
EXPORT_SYMBOL(_kstrtol);

<<<<<<< HEAD
=======
/**
 * kstrtouint - convert a string to an unsigned int
 * @s: The start of the string. The string must be null-terminated, and may also
 *  include a single newline before its terminating null. The first character
 *  may also be a plus sign, but not a minus sign.
 * @base: The number base to use. The maximum supported base is 16. If base is
 *  given as 0, then the base of the string is automatically detected with the
 *  conventional semantics - If it begins with 0x the number will be parsed as a
 *  hexadecimal (case insensitive), if it otherwise begins with 0, it will be
 *  parsed as an octal number. Otherwise it will be parsed as a decimal.
 * @res: Where to write the result of the conversion on success.
 *
 * Returns 0 on success, -ERANGE on overflow and -EINVAL on parsing error.
 * Preferred over simple_strtoul(). Return code must be checked.
 */
noinline
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int kstrtouint(const char *s, unsigned int base, unsigned int *res)
{
	unsigned long long tmp;
	int rv;

	rv = kstrtoull(s, base, &tmp);
	if (rv < 0)
		return rv;
<<<<<<< HEAD
	if (tmp != (unsigned long long)(unsigned int)tmp)
=======
	if (tmp != (unsigned int)tmp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ERANGE;
	*res = tmp;
	return 0;
}
EXPORT_SYMBOL(kstrtouint);

<<<<<<< HEAD
=======
/**
 * kstrtoint - convert a string to an int
 * @s: The start of the string. The string must be null-terminated, and may also
 *  include a single newline before its terminating null. The first character
 *  may also be a plus sign or a minus sign.
 * @base: The number base to use. The maximum supported base is 16. If base is
 *  given as 0, then the base of the string is automatically detected with the
 *  conventional semantics - If it begins with 0x the number will be parsed as a
 *  hexadecimal (case insensitive), if it otherwise begins with 0, it will be
 *  parsed as an octal number. Otherwise it will be parsed as a decimal.
 * @res: Where to write the result of the conversion on success.
 *
 * Returns 0 on success, -ERANGE on overflow and -EINVAL on parsing error.
 * Preferred over simple_strtol(). Return code must be checked.
 */
noinline
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int kstrtoint(const char *s, unsigned int base, int *res)
{
	long long tmp;
	int rv;

	rv = kstrtoll(s, base, &tmp);
	if (rv < 0)
		return rv;
<<<<<<< HEAD
	if (tmp != (long long)(int)tmp)
=======
	if (tmp != (int)tmp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ERANGE;
	*res = tmp;
	return 0;
}
EXPORT_SYMBOL(kstrtoint);

<<<<<<< HEAD
=======
noinline
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int kstrtou16(const char *s, unsigned int base, u16 *res)
{
	unsigned long long tmp;
	int rv;

	rv = kstrtoull(s, base, &tmp);
	if (rv < 0)
		return rv;
<<<<<<< HEAD
	if (tmp != (unsigned long long)(u16)tmp)
=======
	if (tmp != (u16)tmp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ERANGE;
	*res = tmp;
	return 0;
}
EXPORT_SYMBOL(kstrtou16);

<<<<<<< HEAD
=======
noinline
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int kstrtos16(const char *s, unsigned int base, s16 *res)
{
	long long tmp;
	int rv;

	rv = kstrtoll(s, base, &tmp);
	if (rv < 0)
		return rv;
<<<<<<< HEAD
	if (tmp != (long long)(s16)tmp)
=======
	if (tmp != (s16)tmp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ERANGE;
	*res = tmp;
	return 0;
}
EXPORT_SYMBOL(kstrtos16);

<<<<<<< HEAD
=======
noinline
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int kstrtou8(const char *s, unsigned int base, u8 *res)
{
	unsigned long long tmp;
	int rv;

	rv = kstrtoull(s, base, &tmp);
	if (rv < 0)
		return rv;
<<<<<<< HEAD
	if (tmp != (unsigned long long)(u8)tmp)
=======
	if (tmp != (u8)tmp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ERANGE;
	*res = tmp;
	return 0;
}
EXPORT_SYMBOL(kstrtou8);

<<<<<<< HEAD
=======
noinline
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int kstrtos8(const char *s, unsigned int base, s8 *res)
{
	long long tmp;
	int rv;

	rv = kstrtoll(s, base, &tmp);
	if (rv < 0)
		return rv;
<<<<<<< HEAD
	if (tmp != (long long)(s8)tmp)
=======
	if (tmp != (s8)tmp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ERANGE;
	*res = tmp;
	return 0;
}
EXPORT_SYMBOL(kstrtos8);

<<<<<<< HEAD
=======
/**
 * kstrtobool - convert common user inputs into boolean values
 * @s: input string
 * @res: result
 *
 * This routine returns 0 iff the first character is one of 'YyTt1NnFf0', or
 * [oO][NnFf] for "on" and "off". Otherwise it will return -EINVAL.  Value
 * pointed to by res is updated upon finding a match.
 */
noinline
int kstrtobool(const char *s, bool *res)
{
	if (!s)
		return -EINVAL;

	switch (s[0]) {
	case 'y':
	case 'Y':
	case 't':
	case 'T':
	case '1':
		*res = true;
		return 0;
	case 'n':
	case 'N':
	case 'f':
	case 'F':
	case '0':
		*res = false;
		return 0;
	case 'o':
	case 'O':
		switch (s[1]) {
		case 'n':
		case 'N':
			*res = true;
			return 0;
		case 'f':
		case 'F':
			*res = false;
			return 0;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return -EINVAL;
}
EXPORT_SYMBOL(kstrtobool);

/*
 * Since "base" would be a nonsense argument, this open-codes the
 * _from_user helper instead of using the helper macro below.
 */
int kstrtobool_from_user(const char __user *s, size_t count, bool *res)
{
	/* Longest string needed to differentiate, newline, terminator */
	char buf[4];

	count = min(count, sizeof(buf) - 1);
	if (copy_from_user(buf, s, count))
		return -EFAULT;
	buf[count] = '\0';
	return kstrtobool(buf, res);
}
EXPORT_SYMBOL(kstrtobool_from_user);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define kstrto_from_user(f, g, type)					\
int f(const char __user *s, size_t count, unsigned int base, type *res)	\
{									\
	/* sign, base 2 representation, newline, terminator */		\
	char buf[1 + sizeof(type) * 8 + 1 + 1];				\
									\
	count = min(count, sizeof(buf) - 1);				\
	if (copy_from_user(buf, s, count))				\
		return -EFAULT;						\
	buf[count] = '\0';						\
	return g(buf, base, res);					\
}									\
EXPORT_SYMBOL(f)

kstrto_from_user(kstrtoull_from_user,	kstrtoull,	unsigned long long);
kstrto_from_user(kstrtoll_from_user,	kstrtoll,	long long);
kstrto_from_user(kstrtoul_from_user,	kstrtoul,	unsigned long);
kstrto_from_user(kstrtol_from_user,	kstrtol,	long);
kstrto_from_user(kstrtouint_from_user,	kstrtouint,	unsigned int);
kstrto_from_user(kstrtoint_from_user,	kstrtoint,	int);
kstrto_from_user(kstrtou16_from_user,	kstrtou16,	u16);
kstrto_from_user(kstrtos16_from_user,	kstrtos16,	s16);
kstrto_from_user(kstrtou8_from_user,	kstrtou8,	u8);
kstrto_from_user(kstrtos8_from_user,	kstrtos8,	s8);
