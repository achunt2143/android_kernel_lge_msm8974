<<<<<<< HEAD
#include "util.h"
#include "string.h"

#define K 1024LL
=======
// SPDX-License-Identifier: GPL-2.0
#include "string2.h"
#include <linux/kernel.h>
#include <linux/string.h>
#include <stdlib.h>

#include <linux/ctype.h>

const char *graph_dotted_line =
	"---------------------------------------------------------------------"
	"---------------------------------------------------------------------"
	"---------------------------------------------------------------------";
const char *dots =
	"....................................................................."
	"....................................................................."
	".....................................................................";

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * perf_atoll()
 * Parse (\d+)(b|B|kb|KB|mb|MB|gb|GB|tb|TB) (e.g. "256MB")
 * and return its numeric value
 */
s64 perf_atoll(const char *str)
{
<<<<<<< HEAD
	unsigned int i;
	s64 length = -1, unit = 1;
=======
	s64 length;
	char *p;
	char c;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!isdigit(str[0]))
		goto out_err;

<<<<<<< HEAD
	for (i = 1; i < strlen(str); i++) {
		switch (str[i]) {
		case 'B':
		case 'b':
			break;
		case 'K':
			if (str[i + 1] != 'B')
				goto out_err;
			else
				goto kilo;
		case 'k':
			if (str[i + 1] != 'b')
				goto out_err;
kilo:
			unit = K;
			break;
		case 'M':
			if (str[i + 1] != 'B')
				goto out_err;
			else
				goto mega;
		case 'm':
			if (str[i + 1] != 'b')
				goto out_err;
mega:
			unit = K * K;
			break;
		case 'G':
			if (str[i + 1] != 'B')
				goto out_err;
			else
				goto giga;
		case 'g':
			if (str[i + 1] != 'b')
				goto out_err;
giga:
			unit = K * K * K;
			break;
		case 'T':
			if (str[i + 1] != 'B')
				goto out_err;
			else
				goto tera;
		case 't':
			if (str[i + 1] != 'b')
				goto out_err;
tera:
			unit = K * K * K * K;
			break;
		case '\0':	/* only specified figures */
			unit = 1;
			break;
		default:
			if (!isdigit(str[i]))
				goto out_err;
			break;
		}
	}

	length = atoll(str) * unit;
	goto out;

out_err:
	length = -1;
out:
	return length;
}

/*
 * Helper function for splitting a string into an argv-like array.
 * originally copied from lib/argv_split.c
 */
static const char *skip_sep(const char *cp)
{
	while (*cp && isspace(*cp))
		cp++;

	return cp;
}

static const char *skip_arg(const char *cp)
{
	while (*cp && !isspace(*cp))
		cp++;

	return cp;
}

static int count_argc(const char *str)
{
	int count = 0;

	while (*str) {
		str = skip_sep(str);
		if (*str) {
			count++;
			str = skip_arg(str);
		}
	}

	return count;
}

/**
 * argv_free - free an argv
 * @argv - the argument vector to be freed
 *
 * Frees an argv and the strings it points to.
 */
void argv_free(char **argv)
{
	char **p;
	for (p = argv; *p; p++)
		free(*p);

	free(argv);
}

/**
 * argv_split - split a string at whitespace, returning an argv
 * @str: the string to be split
 * @argcp: returned argument count
 *
 * Returns an array of pointers to strings which are split out from
 * @str.  This is performed by strictly splitting on white-space; no
 * quote processing is performed.  Multiple whitespace characters are
 * considered to be a single argument separator.  The returned array
 * is always NULL-terminated.  Returns NULL on memory allocation
 * failure.
 */
char **argv_split(const char *str, int *argcp)
{
	int argc = count_argc(str);
	char **argv = zalloc(sizeof(*argv) * (argc+1));
	char **argvp;

	if (argv == NULL)
		goto out;

	if (argcp)
		*argcp = argc;

	argvp = argv;

	while (*str) {
		str = skip_sep(str);

		if (*str) {
			const char *p = str;
			char *t;

			str = skip_arg(str);

			t = strndup(p, str-p);
			if (t == NULL)
				goto fail;
			*argvp++ = t;
		}
	}
	*argvp = NULL;

out:
	return argv;

fail:
	argv_free(argv);
	return NULL;
=======
	length = strtoll(str, &p, 10);
	switch (c = *p++) {
		case 'b': case 'B':
			if (*p)
				goto out_err;

			fallthrough;
		case '\0':
			return length;
		default:
			goto out_err;
		/* two-letter suffices */
		case 'k': case 'K':
			length <<= 10;
			break;
		case 'm': case 'M':
			length <<= 20;
			break;
		case 'g': case 'G':
			length <<= 30;
			break;
		case 't': case 'T':
			length <<= 40;
			break;
	}
	/* we want the cases to match */
	if (islower(c)) {
		if (strcmp(p, "b") != 0)
			goto out_err;
	} else {
		if (strcmp(p, "B") != 0)
			goto out_err;
	}
	return length;

out_err:
	return -1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Character class matching */
static bool __match_charclass(const char *pat, char c, const char **npat)
{
	bool complement = false, ret = true;

	if (*pat == '!') {
		complement = true;
		pat++;
	}
	if (*pat++ == c)	/* First character is special */
		goto end;

	while (*pat && *pat != ']') {	/* Matching */
		if (*pat == '-' && *(pat + 1) != ']') {	/* Range */
			if (*(pat - 1) <= c && c <= *(pat + 1))
				goto end;
			if (*(pat - 1) > *(pat + 1))
				goto error;
			pat += 2;
		} else if (*pat++ == c)
			goto end;
	}
	if (!*pat)
		goto error;
	ret = false;

end:
	while (*pat && *pat != ']')	/* Searching closing */
		pat++;
	if (!*pat)
		goto error;
	*npat = pat + 1;
	return complement ? !ret : ret;

error:
	return false;
}

/* Glob/lazy pattern matching */
<<<<<<< HEAD
static bool __match_glob(const char *str, const char *pat, bool ignore_space)
=======
static bool __match_glob(const char *str, const char *pat, bool ignore_space,
			bool case_ins)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	while (*str && *pat && *pat != '*') {
		if (ignore_space) {
			/* Ignore spaces for lazy matching */
			if (isspace(*str)) {
				str++;
				continue;
			}
			if (isspace(*pat)) {
				pat++;
				continue;
			}
		}
		if (*pat == '?') {	/* Matches any single character */
			str++;
			pat++;
			continue;
		} else if (*pat == '[')	/* Character classes/Ranges */
			if (__match_charclass(pat + 1, *str, &pat)) {
				str++;
				continue;
			} else
				return false;
		else if (*pat == '\\') /* Escaped char match as normal char */
			pat++;
<<<<<<< HEAD
		if (*str++ != *pat++)
			return false;
=======
		if (case_ins) {
			if (tolower(*str) != tolower(*pat))
				return false;
		} else if (*str != *pat)
			return false;
		str++;
		pat++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	/* Check wild card */
	if (*pat == '*') {
		while (*pat == '*')
			pat++;
		if (!*pat)	/* Tail wild card matches all */
			return true;
		while (*str)
<<<<<<< HEAD
			if (__match_glob(str++, pat, ignore_space))
=======
			if (__match_glob(str++, pat, ignore_space, case_ins))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				return true;
	}
	return !*str && !*pat;
}

/**
 * strglobmatch - glob expression pattern matching
 * @str: the target string to match
 * @pat: the pattern string to match
 *
 * This returns true if the @str matches @pat. @pat can includes wildcards
 * ('*','?') and character classes ([CHARS], complementation and ranges are
 * also supported). Also, this supports escape character ('\') to use special
 * characters as normal character.
 *
 * Note: if @pat syntax is broken, this always returns false.
 */
bool strglobmatch(const char *str, const char *pat)
{
<<<<<<< HEAD
	return __match_glob(str, pat, false);
=======
	return __match_glob(str, pat, false, false);
}

bool strglobmatch_nocase(const char *str, const char *pat)
{
	return __match_glob(str, pat, false, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * strlazymatch - matching pattern strings lazily with glob pattern
 * @str: the target string to match
 * @pat: the pattern string to match
 *
 * This is similar to strglobmatch, except this ignores spaces in
 * the target string.
 */
bool strlazymatch(const char *str, const char *pat)
{
<<<<<<< HEAD
	return __match_glob(str, pat, true);
=======
	return __match_glob(str, pat, true, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * strtailcmp - Compare the tail of two strings
 * @s1: 1st string to be compared
 * @s2: 2nd string to be compared
 *
 * Return 0 if whole of either string is same as another's tail part.
 */
int strtailcmp(const char *s1, const char *s2)
{
	int i1 = strlen(s1);
	int i2 = strlen(s2);
	while (--i1 >= 0 && --i2 >= 0) {
		if (s1[i1] != s2[i2])
			return s1[i1] - s2[i2];
	}
	return 0;
}

<<<<<<< HEAD
=======
char *asprintf_expr_inout_ints(const char *var, bool in, size_t nints, int *ints)
{
	/*
	 * FIXME: replace this with an expression using log10() when we
	 * find a suitable implementation, maybe the one in the dvb drivers...
	 *
	 * "%s == %d || " = log10(MAXINT) * 2 + 8 chars for the operators
	 */
	size_t size = nints * 28 + 1; /* \0 */
	size_t i, printed = 0;
	char *expr = malloc(size);

	if (expr) {
		const char *or_and = "||", *eq_neq = "==";
		char *e = expr;

		if (!in) {
			or_and = "&&";
			eq_neq = "!=";
		}

		for (i = 0; i < nints; ++i) {
			if (printed == size)
				goto out_err_overflow;

			if (i > 0)
				printed += scnprintf(e + printed, size - printed, " %s ", or_and);
			printed += scnprintf(e + printed, size - printed,
					     "%s %s %d", var, eq_neq, ints[i]);
		}
	}

	return expr;

out_err_overflow:
	free(expr);
	return NULL;
}

/* Like strpbrk(), but not break if it is right after a backslash (escaped) */
char *strpbrk_esc(char *str, const char *stopset)
{
	char *ptr;

	do {
		ptr = strpbrk(str, stopset);
		if (ptr == str ||
		    (ptr == str + 1 && *(ptr - 1) != '\\'))
			break;
		str = ptr + 1;
	} while (ptr && *(ptr - 1) == '\\' && *(ptr - 2) != '\\');

	return ptr;
}

/* Like strdup, but do not copy a single backslash */
char *strdup_esc(const char *str)
{
	char *s, *d, *p, *ret = strdup(str);

	if (!ret)
		return NULL;

	d = strchr(ret, '\\');
	if (!d)
		return ret;

	s = d + 1;
	do {
		if (*s == '\0') {
			*d = '\0';
			break;
		}
		p = strchr(s + 1, '\\');
		if (p) {
			memmove(d, s, p - s);
			d += p - s;
			s = p + 1;
		} else
			memmove(d, s, strlen(s) + 1);
	} while (p);

	return ret;
}

unsigned int hex(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	return c - 'A' + 10;
}

/*
 * Replace all occurrences of character 'needle' in string 'haystack' with
 * string 'replace'
 *
 * The new string could be longer so a new string is returned which must be
 * freed.
 */
char *strreplace_chars(char needle, const char *haystack, const char *replace)
{
	int replace_len = strlen(replace);
	char *new_s, *to;
	const char *loc = strchr(haystack, needle);
	const char *from = haystack;
	int num = 0;

	/* Count occurrences */
	while (loc) {
		loc = strchr(loc + 1, needle);
		num++;
	}

	/* Allocate enough space for replacements and reset first location */
	new_s = malloc(strlen(haystack) + (num * (replace_len - 1) + 1));
	if (!new_s)
		return NULL;
	loc = strchr(haystack, needle);
	to = new_s;

	while (loc) {
		/* Copy original string up to found char and update positions */
		memcpy(to, from, 1 + loc - from);
		to += loc - from;
		from = loc + 1;

		/* Copy replacement string and update positions */
		memcpy(to, replace, replace_len);
		to += replace_len;

		/* needle next occurrence or end of string */
		loc = strchr(from, needle);
	}

	/* Copy any remaining chars + null */
	strcpy(to, from);

	return new_s;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
