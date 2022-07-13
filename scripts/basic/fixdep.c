/*
 * "Optimize" a list of dependencies as spit out by gcc -MD
 * for the kernel build
 * ===========================================================================
 *
 * Author       Kai Germaschewski
 * Copyright    2002 by Kai Germaschewski  <kai.germaschewski@gmx.de>
 *
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 *
 * Introduction:
 *
 * gcc produces a very nice and correct list of dependencies which
 * tells make when to remake a file.
 *
 * To use this list as-is however has the drawback that virtually
 * every file in the kernel includes autoconf.h.
 *
 * If the user re-runs make *config, autoconf.h will be
 * regenerated.  make notices that and will rebuild every file which
 * includes autoconf.h, i.e. basically all files. This is extremely
 * annoying if the user just changed CONFIG_HIS_DRIVER from n to m.
 *
 * So we play the same trick that "mkdep" played before. We replace
 * the dependency on autoconf.h by a dependency on every config
<<<<<<< HEAD
 * option which is mentioned in any of the listed prequisites.
=======
 * option which is mentioned in any of the listed prerequisites.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * kconfig populates a tree in include/config/ with an empty file
 * for each config symbol and when the configuration is updated
 * the files representing changed config options are touched
 * which then let make pick up the changes and the files that use
 * the config symbols are rebuilt.
 *
 * So if the user changes his CONFIG_HIS_DRIVER option, only the objects
<<<<<<< HEAD
 * which depend on "include/linux/config/his/driver.h" will be rebuilt,
=======
 * which depend on "include/config/HIS_DRIVER" will be rebuilt,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * so most likely only his driver ;-)
 *
 * The idea above dates, by the way, back to Michael E Chastain, AFAIK.
 *
 * So to get dependencies right, there are two issues:
 * o if any of the files the compiler read changed, we need to rebuild
 * o if the command line given to the compile the file changed, we
 *   better rebuild as well.
 *
 * The former is handled by using the -MD output, the later by saving
 * the command line used to compile the old object and comparing it
 * to the one we would now use.
 *
 * Again, also this idea is pretty old and has been discussed on
 * kbuild-devel a long time ago. I don't have a sensibly working
 * internet connection right now, so I rather don't mention names
 * without double checking.
 *
 * This code here has been based partially based on mkdep.c, which
 * says the following about its history:
 *
 *   Copyright abandoned, Michael Chastain, <mailto:mec@shout.net>.
 *   This is a C version of syncdep.pl by Werner Almesberger.
 *
 *
 * It is invoked as
 *
 *   fixdep <depfile> <target> <cmdline>
 *
 * and will read the dependency file <depfile>
 *
 * The transformed dependency snipped is written to stdout.
 *
 * It first generates a line
 *
<<<<<<< HEAD
 *   cmd_<target> = <cmdline>
 *
 * and then basically copies the .<target>.d file to stdout, in the
 * process filtering out the dependency on autoconf.h and adding
 * dependencies on include/config/my/option.h for every
 * CONFIG_MY_OPTION encountered in any of the prequisites.
 *
 * It will also filter out all the dependencies on *.ver. We need
 * to make sure that the generated version checksum are globally up
 * to date before even starting the recursive build, so it's too late
 * at this point anyway.
 *
 * The algorithm to grep for "CONFIG_..." is bit unusual, but should
 * be fast ;-) We don't even try to really parse the header files, but
=======
 *   savedcmd_<target> = <cmdline>
 *
 * and then basically copies the .<target>.d file to stdout, in the
 * process filtering out the dependency on autoconf.h and adding
 * dependencies on include/config/MY_OPTION for every
 * CONFIG_MY_OPTION encountered in any of the prerequisites.
 *
 * We don't even try to really parse the header files, but
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * merely grep, i.e. if CONFIG_FOO is mentioned in a comment, it will
 * be picked up as well. It's not a problem with respect to
 * correctness, since that can only give too many dependencies, thus
 * we cannot miss a rebuild. Since people tend to not mention totally
 * unrelated CONFIG_ options all over the place, it's not an
 * efficiency problem either.
 *
 * (Note: it'd be easy to port over the complete mkdep state machine,
 *  but I don't think the added complexity is worth it)
 */
<<<<<<< HEAD
/*
 * Note 2: if somebody writes HELLO_CONFIG_BOOM in a file, it will depend onto
 * CONFIG_BOOM. This could seem a bug (not too hard to fix), but please do not
 * fix it! Some UserModeLinux files (look at arch/um/) call CONFIG_BOOM as
 * UML_CONFIG_BOOM, to avoid conflicts with /usr/include/linux/autoconf.h,
 * through arch/um/include/uml-config.h; this fixdep "bug" makes sure that
 * those files will have correct dependencies.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <arpa/inet.h>

#define INT_CONF ntohl(0x434f4e46)
#define INT_ONFI ntohl(0x4f4e4649)
#define INT_NFIG ntohl(0x4e464947)
#define INT_FIG_ ntohl(0x4649475f)

char *target;
char *depfile;
char *cmdline;
=======

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void usage(void)
{
	fprintf(stderr, "Usage: fixdep <depfile> <target> <cmdline>\n");
	exit(1);
}

<<<<<<< HEAD
/*
 * Print out the commandline prefixed with cmd_<target filename> :=
 */
static void print_cmdline(void)
{
	printf("cmd_%s := %s\n\n", target, cmdline);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct item {
	struct item	*next;
	unsigned int	len;
	unsigned int	hash;
<<<<<<< HEAD
	char		name[0];
};

#define HASHSZ 256
static struct item *hashtab[HASHSZ];
=======
	char		name[];
};

#define HASHSZ 256
static struct item *config_hashtab[HASHSZ], *file_hashtab[HASHSZ];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static unsigned int strhash(const char *str, unsigned int sz)
{
	/* fnv32 hash */
	unsigned int i, hash = 2166136261U;

	for (i = 0; i < sz; i++)
		hash = (hash ^ str[i]) * 0x01000193;
	return hash;
}

/*
<<<<<<< HEAD
 * Lookup a value in the configuration string.
 */
static int is_defined_config(const char *name, int len, unsigned int hash)
{
	struct item *aux;

	for (aux = hashtab[hash % HASHSZ]; aux; aux = aux->next) {
		if (aux->hash == hash && aux->len == len &&
		    memcmp(aux->name, name, len) == 0)
			return 1;
	}
	return 0;
}

/*
 * Add a new value to the configuration string.
 */
static void define_config(const char *name, int len, unsigned int hash)
=======
 * Add a new value to the configuration string.
 */
static void add_to_hashtable(const char *name, int len, unsigned int hash,
			     struct item *hashtab[])
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct item *aux = malloc(sizeof(*aux) + len);

	if (!aux) {
		perror("fixdep:malloc");
		exit(1);
	}
	memcpy(aux->name, name, len);
	aux->len = len;
	aux->hash = hash;
	aux->next = hashtab[hash % HASHSZ];
	hashtab[hash % HASHSZ] = aux;
}

/*
<<<<<<< HEAD
 * Clear the set of configuration strings.
 */
static void clear_config(void)
{
	struct item *aux, *next;
	unsigned int i;

	for (i = 0; i < HASHSZ; i++) {
		for (aux = hashtab[i]; aux; aux = next) {
			next = aux->next;
			free(aux);
		}
		hashtab[i] = NULL;
	}
=======
 * Lookup a string in the hash table. If found, just return true.
 * If not, add it to the hashtable and return false.
 */
static bool in_hashtable(const char *name, int len, struct item *hashtab[])
{
	struct item *aux;
	unsigned int hash = strhash(name, len);

	for (aux = hashtab[hash % HASHSZ]; aux; aux = aux->next) {
		if (aux->hash == hash && aux->len == len &&
		    memcmp(aux->name, name, len) == 0)
			return true;
	}

	add_to_hashtable(name, len, hash, hashtab);

	return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Record the use of a CONFIG_* word.
 */
static void use_config(const char *m, int slen)
{
<<<<<<< HEAD
	unsigned int hash = strhash(m, slen);
	int c, i;

	if (is_defined_config(m, slen, hash))
	    return;

	define_config(m, slen, hash);

	printf("    $(wildcard include/config/");
	for (i = 0; i < slen; i++) {
		c = m[i];
		if (c == '_')
			c = '/';
		else
			c = tolower(c);
		putchar(c);
	}
	printf(".h) \\\n");
}

static void parse_config_file(const char *map, size_t len)
{
	const int *end = (const int *) (map + len);
	/* start at +1, so that p can never be < map */
	const int *m   = (const int *) map + 1;
	const char *p, *q;

	for (; m < end; m++) {
		if (*m == INT_CONF) { p = (char *) m  ; goto conf; }
		if (*m == INT_ONFI) { p = (char *) m-1; goto conf; }
		if (*m == INT_NFIG) { p = (char *) m-2; goto conf; }
		if (*m == INT_FIG_) { p = (char *) m-3; goto conf; }
		continue;
	conf:
		if (p > map + len - 7)
			continue;
		if (memcmp(p, "CONFIG_", 7))
			continue;
		for (q = p + 7; q < map + len; q++) {
			if (!(isalnum(*q) || *q == '_'))
				goto found;
		}
		continue;

	found:
		if (!memcmp(q - 7, "_MODULE", 7))
			q -= 7;
		if( (q-p-7) < 0 )
			continue;
		use_config(p+7, q-p-7);
	}
}

/* test is s ends in sub */
static int strrcmp(char *s, char *sub)
{
	int slen = strlen(s);
	int sublen = strlen(sub);

	if (sublen > slen)
		return 1;

	return memcmp(s + slen - sublen, sub, sublen);
}

static void do_config_file(const char *filename)
{
	struct stat st;
	int fd;
	void *map;

	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "fixdep: error opening config file: ");
		perror(filename);
		exit(2);
	}
	fstat(fd, &st);
	if (st.st_size == 0) {
		close(fd);
		return;
	}
	map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if ((long) map == -1) {
		perror("fixdep: mmap");
		close(fd);
		return;
	}

	parse_config_file(map, st.st_size);

	munmap(map, st.st_size);

	close(fd);
=======
	if (in_hashtable(m, slen, config_hashtab))
		return;

	/* Print out a dependency path from a symbol name. */
	printf("    $(wildcard include/config/%.*s) \\\n", slen, m);
}

/* test if s ends in sub */
static int str_ends_with(const char *s, int slen, const char *sub)
{
	int sublen = strlen(sub);

	if (sublen > slen)
		return 0;

	return !memcmp(s + slen - sublen, sub, sublen);
}

static void parse_config_file(const char *p)
{
	const char *q, *r;
	const char *start = p;

	while ((p = strstr(p, "CONFIG_"))) {
		if (p > start && (isalnum(p[-1]) || p[-1] == '_')) {
			p += 7;
			continue;
		}
		p += 7;
		q = p;
		while (isalnum(*q) || *q == '_')
			q++;
		if (str_ends_with(p, q - p, "_MODULE"))
			r = q - 7;
		else
			r = q;
		if (r > p)
			use_config(p, r - p);
		p = q;
	}
}

static void *read_file(const char *filename)
{
	struct stat st;
	int fd;
	char *buf;

	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "fixdep: error opening file: ");
		perror(filename);
		exit(2);
	}
	if (fstat(fd, &st) < 0) {
		fprintf(stderr, "fixdep: error fstat'ing file: ");
		perror(filename);
		exit(2);
	}
	buf = malloc(st.st_size + 1);
	if (!buf) {
		perror("fixdep: malloc");
		exit(2);
	}
	if (read(fd, buf, st.st_size) != st.st_size) {
		perror("fixdep: read");
		exit(2);
	}
	buf[st.st_size] = '\0';
	close(fd);

	return buf;
}

/* Ignore certain dependencies */
static int is_ignored_file(const char *s, int len)
{
	return str_ends_with(s, len, "include/generated/autoconf.h");
}

/* Do not parse these files */
static int is_no_parse_file(const char *s, int len)
{
	/* rustc may list binary files in dep-info */
	return str_ends_with(s, len, ".rlib") ||
	       str_ends_with(s, len, ".rmeta") ||
	       str_ends_with(s, len, ".so");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Important: The below generated source_foo.o and deps_foo.o variable
 * assignments are parsed not only by make, but also by the rather simple
 * parser in scripts/mod/sumversion.c.
 */
<<<<<<< HEAD
static void parse_dep_file(void *map, size_t len)
{
	char *m = map;
	char *end = m + len;
	char *p;
	char s[PATH_MAX];
	int first;

	p = strchr(m, ':');
	if (!p) {
		fprintf(stderr, "fixdep: parse error\n");
		exit(1);
	}
	memcpy(s, m, p-m); s[p-m] = 0;
	m = p+1;

	clear_config();

	first = 1;
	while (m < end) {
		while (m < end && (*m == ' ' || *m == '\\' || *m == '\n'))
			m++;
		p = m;
		while (p < end && *p != ' ') p++;
		if (p == end) {
			do p--; while (!isalnum(*p));
			p++;
		}
		memcpy(s, m, p-m); s[p-m] = 0;
		if (strrcmp(s, "include/generated/autoconf.h") &&
		    strrcmp(s, "arch/um/include/uml-config.h") &&
		    strrcmp(s, "include/linux/kconfig.h") &&
		    strrcmp(s, ".ver")) {
			/*
			 * Do not list the source file as dependency, so that
			 * kbuild is not confused if a .c file is rewritten
			 * into .S or vice versa. Storing it in source_* is
			 * needed for modpost to compute srcversions.
			 */
			if (first) {
				printf("source_%s := %s\n\n", target, s);
				printf("deps_%s := \\\n", target);
			} else
				printf("  %s \\\n", s);
			do_config_file(s);
		}
		first = 0;
		m = p + 1;
	}
=======
static void parse_dep_file(char *p, const char *target)
{
	bool saw_any_target = false;
	bool is_target = true;
	bool is_source = false;
	bool need_parse;
	char *q, saved_c;

	while (*p) {
		/* handle some special characters first. */
		switch (*p) {
		case '#':
			/*
			 * skip comments.
			 * rustc may emit comments to dep-info.
			 */
			p++;
			while (*p != '\0' && *p != '\n') {
				/*
				 * escaped newlines continue the comment across
				 * multiple lines.
				 */
				if (*p == '\\')
					p++;
				p++;
			}
			continue;
		case ' ':
		case '\t':
			/* skip whitespaces */
			p++;
			continue;
		case '\\':
			/*
			 * backslash/newline combinations continue the
			 * statement. Skip it just like a whitespace.
			 */
			if (*(p + 1) == '\n') {
				p += 2;
				continue;
			}
			break;
		case '\n':
			/*
			 * Makefiles use a line-based syntax, where the newline
			 * is the end of a statement. After seeing a newline,
			 * we expect the next token is a target.
			 */
			p++;
			is_target = true;
			continue;
		case ':':
			/*
			 * assume the first dependency after a colon as the
			 * source file.
			 */
			p++;
			is_target = false;
			is_source = true;
			continue;
		}

		/* find the end of the token */
		q = p;
		while (*q != ' ' && *q != '\t' && *q != '\n' && *q != '#' && *q != ':') {
			if (*q == '\\') {
				/*
				 * backslash/newline combinations work like as
				 * a whitespace, so this is the end of token.
				 */
				if (*(q + 1) == '\n')
					break;

				/* escaped special characters */
				if (*(q + 1) == '#' || *(q + 1) == ':') {
					memmove(p + 1, p, q - p);
					p++;
				}

				q++;
			}

			if (*q == '\0')
				break;
			q++;
		}

		/* Just discard the target */
		if (is_target) {
			p = q;
			continue;
		}

		saved_c = *q;
		*q = '\0';
		need_parse = false;

		/*
		 * Do not list the source file as dependency, so that kbuild is
		 * not confused if a .c file is rewritten into .S or vice versa.
		 * Storing it in source_* is needed for modpost to compute
		 * srcversions.
		 */
		if (is_source) {
			/*
			 * The DT build rule concatenates multiple dep files.
			 * When processing them, only process the first source
			 * name, which will be the original one, and ignore any
			 * other source names, which will be intermediate
			 * temporary files.
			 *
			 * rustc emits the same dependency list for each
			 * emission type. It is enough to list the source name
			 * just once.
			 */
			if (!saw_any_target) {
				saw_any_target = true;
				printf("source_%s := %s\n\n", target, p);
				printf("deps_%s := \\\n", target);
				need_parse = true;
			}
		} else if (!is_ignored_file(p, q - p) &&
			   !in_hashtable(p, q - p, file_hashtab)) {
			printf("  %s \\\n", p);
			need_parse = true;
		}

		if (need_parse && !is_no_parse_file(p, q - p)) {
			void *buf;

			buf = read_file(p);
			parse_config_file(buf);
			free(buf);
		}

		is_source = false;
		*q = saved_c;
		p = q;
	}

	if (!saw_any_target) {
		fprintf(stderr, "fixdep: parse error; no targets found\n");
		exit(1);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	printf("\n%s: $(deps_%s)\n\n", target, target);
	printf("$(deps_%s):\n", target);
}

<<<<<<< HEAD
static void print_deps(void)
{
	struct stat st;
	int fd;
	void *map;

	fd = open(depfile, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "fixdep: error opening depfile: ");
		perror(depfile);
		exit(2);
	}
	if (fstat(fd, &st) < 0) {
                fprintf(stderr, "fixdep: error fstat'ing depfile: ");
                perror(depfile);
                exit(2);
        }
	if (st.st_size == 0) {
		fprintf(stderr,"fixdep: %s is empty\n",depfile);
		close(fd);
		return;
	}
	map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if ((long) map == -1) {
		perror("fixdep: mmap");
		close(fd);
		return;
	}

	parse_dep_file(map, st.st_size);

	munmap(map, st.st_size);

	close(fd);
}

static void traps(void)
{
	static char test[] __attribute__((aligned(sizeof(int)))) = "CONF";
	int *p = (int *)test;

	if (*p != INT_CONF) {
		fprintf(stderr, "fixdep: sizeof(int) != 4 or wrong endianess? %#x\n",
			*p);
		exit(2);
	}
}

int main(int argc, char *argv[])
{
	traps();
=======
int main(int argc, char *argv[])
{
	const char *depfile, *target, *cmdline;
	void *buf;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (argc != 4)
		usage();

	depfile = argv[1];
	target = argv[2];
	cmdline = argv[3];

<<<<<<< HEAD
	print_cmdline();
	print_deps();
=======
	printf("savedcmd_%s := %s\n\n", target, cmdline);

	buf = read_file(depfile);
	parse_dep_file(buf, target);
	free(buf);

	fflush(stdout);

	/*
	 * In the intended usage, the stdout is redirected to .*.cmd files.
	 * Call ferror() to catch errors such as "No space left on device".
	 */
	if (ferror(stdout)) {
		fprintf(stderr, "fixdep: not all data was written to the output\n");
		exit(1);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}
