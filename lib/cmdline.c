<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/lib/cmdline.c
 * Helper functions generally used for parsing kernel command line
 * and module options.
 *
 * Code and copyrights come from init/main.c and arch/i386/kernel/setup.c.
 *
<<<<<<< HEAD
 * This source code is licensed under the GNU General Public License,
 * Version 2.  See the file COPYING for more details.
 *
 * GNU Indent formatting options for this file: -kr -i8 -npsl -pcs
 *
=======
 * GNU Indent formatting options for this file: -kr -i8 -npsl -pcs
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/export.h>
#include <linux/kernel.h>
#include <linux/string.h>
<<<<<<< HEAD
=======
#include <linux/ctype.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 *	If a hyphen was found in get_option, this will handle the
 *	range of numbers, M-N.  This will expand the range and insert
 *	the values[M, M+1, ..., N] into the ints array in get_options.
 */

<<<<<<< HEAD
static int get_range(char **str, int *pint)
=======
static int get_range(char **str, int *pint, int n)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int x, inc_counter, upper_range;

	(*str)++;
	upper_range = simple_strtol((*str), NULL, 0);
	inc_counter = upper_range - *pint;
<<<<<<< HEAD
	for (x = *pint; x < upper_range; x++)
=======
	for (x = *pint; n && x < upper_range; x++, n--)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		*pint++ = x;
	return inc_counter;
}

/**
 *	get_option - Parse integer from an option string
 *	@str: option string
<<<<<<< HEAD
 *	@pint: (output) integer value parsed from @str
=======
 *	@pint: (optional output) integer value parsed from @str
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 *	Read an int from an option string; if available accept a subsequent
 *	comma as well.
 *
<<<<<<< HEAD
=======
 *	When @pint is NULL the function can be used as a validator of
 *	the current option in the string.
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	Return values:
 *	0 - no int in string
 *	1 - int found, no subsequent comma
 *	2 - int found including a subsequent comma
 *	3 - hyphen found to denote a range
<<<<<<< HEAD
 */

int get_option (char **str, int *pint)
{
	char *cur = *str;

	if (!cur || !(*cur))
		return 0;
	*pint = simple_strtol (cur, str, 0);
=======
 *
 *	Leading hyphen without integer is no integer case, but we consume it
 *	for the sake of simplification.
 */

int get_option(char **str, int *pint)
{
	char *cur = *str;
	int value;

	if (!cur || !(*cur))
		return 0;
	if (*cur == '-')
		value = -simple_strtoull(++cur, str, 0);
	else
		value = simple_strtoull(cur, str, 0);
	if (pint)
		*pint = value;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (cur == *str)
		return 0;
	if (**str == ',') {
		(*str)++;
		return 2;
	}
	if (**str == '-')
		return 3;

	return 1;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL(get_option);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 *	get_options - Parse a string into a list of integers
 *	@str: String to be parsed
 *	@nints: size of integer array
<<<<<<< HEAD
 *	@ints: integer array
=======
 *	@ints: integer array (must have room for at least one element)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 *	This function parses a string containing a comma-separated
 *	list of integers, a hyphen-separated range of _positive_ integers,
 *	or a combination of both.  The parse halts when the array is
 *	full, or when no more numbers can be retrieved from the
 *	string.
 *
<<<<<<< HEAD
=======
 *	When @nints is 0, the function just validates the given @str and
 *	returns the amount of parseable integers as described below.
 *
 *	Returns:
 *
 *	The first element is filled by the number of collected integers
 *	in the range. The rest is what was parsed from the @str.
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	Return value is the character in the string which caused
 *	the parse to end (typically a null terminator, if @str is
 *	completely parseable).
 */
<<<<<<< HEAD
 
char *get_options(const char *str, int nints, int *ints)
{
	int res, i = 1;

	while (i < nints) {
		res = get_option ((char **)&str, ints + i);
		if (res == 0)
			break;
		if (res == 3) {
			int range_nums;
			range_nums = get_range((char **)&str, ints + i);
=======

char *get_options(const char *str, int nints, int *ints)
{
	bool validate = (nints == 0);
	int res, i = 1;

	while (i < nints || validate) {
		int *pint = validate ? ints : ints + i;

		res = get_option((char **)&str, pint);
		if (res == 0)
			break;
		if (res == 3) {
			int n = validate ? 0 : nints - i;
			int range_nums;

			range_nums = get_range((char **)&str, pint, n);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (range_nums < 0)
				break;
			/*
			 * Decrement the result by one to leave out the
			 * last number in the range.  The next iteration
			 * will handle the upper number in the range
			 */
			i += (range_nums - 1);
		}
		i++;
		if (res == 1)
			break;
	}
	ints[0] = i - 1;
	return (char *)str;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL(get_options);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 *	memparse - parse a string with mem suffixes into a number
 *	@ptr: Where parse begins
 *	@retptr: (output) Optional pointer to next char after parse completes
 *
 *	Parses a string into a number.  The number stored at @ptr is
<<<<<<< HEAD
 *	potentially suffixed with %K (for kilobytes, or 1024 bytes),
 *	%M (for megabytes, or 1048576 bytes), or %G (for gigabytes, or
 *	1073741824).  If the number is suffixed with K, M, or G, then
 *	the return value is the number multiplied by one kilobyte, one
 *	megabyte, or one gigabyte, respectively.
=======
 *	potentially suffixed with K, M, G, T, P, E.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

unsigned long long memparse(const char *ptr, char **retptr)
{
	char *endptr;	/* local pointer to end of parsed string */

	unsigned long long ret = simple_strtoull(ptr, &endptr, 0);

	switch (*endptr) {
<<<<<<< HEAD
	case 'G':
	case 'g':
		ret <<= 10;
	case 'M':
	case 'm':
		ret <<= 10;
=======
	case 'E':
	case 'e':
		ret <<= 10;
		fallthrough;
	case 'P':
	case 'p':
		ret <<= 10;
		fallthrough;
	case 'T':
	case 't':
		ret <<= 10;
		fallthrough;
	case 'G':
	case 'g':
		ret <<= 10;
		fallthrough;
	case 'M':
	case 'm':
		ret <<= 10;
		fallthrough;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case 'K':
	case 'k':
		ret <<= 10;
		endptr++;
<<<<<<< HEAD
=======
		fallthrough;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		break;
	}

	if (retptr)
		*retptr = endptr;

	return ret;
}
<<<<<<< HEAD


EXPORT_SYMBOL(memparse);
EXPORT_SYMBOL(get_option);
EXPORT_SYMBOL(get_options);
=======
EXPORT_SYMBOL(memparse);

/**
 *	parse_option_str - Parse a string and check an option is set or not
 *	@str: String to be parsed
 *	@option: option name
 *
 *	This function parses a string containing a comma-separated list of
 *	strings like a=b,c.
 *
 *	Return true if there's such option in the string, or return false.
 */
bool parse_option_str(const char *str, const char *option)
{
	while (*str) {
		if (!strncmp(str, option, strlen(option))) {
			str += strlen(option);
			if (!*str || *str == ',')
				return true;
		}

		while (*str && *str != ',')
			str++;

		if (*str == ',')
			str++;
	}

	return false;
}

/*
 * Parse a string to get a param value pair.
 * You can use " around spaces, but can't escape ".
 * Hyphens and underscores equivalent in parameter names.
 */
char *next_arg(char *args, char **param, char **val)
{
	unsigned int i, equals = 0;
	int in_quote = 0, quoted = 0;

	if (*args == '"') {
		args++;
		in_quote = 1;
		quoted = 1;
	}

	for (i = 0; args[i]; i++) {
		if (isspace(args[i]) && !in_quote)
			break;
		if (equals == 0) {
			if (args[i] == '=')
				equals = i;
		}
		if (args[i] == '"')
			in_quote = !in_quote;
	}

	*param = args;
	if (!equals)
		*val = NULL;
	else {
		args[equals] = '\0';
		*val = args + equals + 1;

		/* Don't include quotes in value. */
		if (**val == '"') {
			(*val)++;
			if (args[i-1] == '"')
				args[i-1] = '\0';
		}
	}
	if (quoted && i > 0 && args[i-1] == '"')
		args[i-1] = '\0';

	if (args[i]) {
		args[i] = '\0';
		args += i + 1;
	} else
		args += i;

	/* Chew up trailing spaces. */
	return skip_spaces(args);
}
EXPORT_SYMBOL(next_arg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
