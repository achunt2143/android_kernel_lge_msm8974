/*
 * perf.c
 *
 * Performance analysis utility.
 *
 * This is the main hub from which the sub-commands (perf stat,
 * perf top, perf record, perf report, etc.) are started.
 */
#include "builtin.h"
<<<<<<< HEAD

#include "util/exec_cmd.h"
#include "util/cache.h"
#include "util/quote.h"
#include "util/run-command.h"
#include "util/parse-events.h"
#include "util/debugfs.h"

const char perf_usage_string[] =
	"perf [--version] [--help] COMMAND [ARGS]";

const char perf_more_info_string[] =
	"See 'perf help COMMAND' for more information on a specific command.";

int use_browser = -1;
static int use_pager = -1;
=======
#include "perf.h"

#include "util/build-id.h"
#include "util/cache.h"
#include "util/env.h"
#include <internal/lib.h> // page_size
#include <subcmd/exec-cmd.h>
#include "util/config.h"
#include <subcmd/run-command.h>
#include "util/parse-events.h"
#include <subcmd/parse-options.h>
#include "util/debug.h"
#include "util/event.h"
#include "util/util.h" // usage()
#include "ui/ui.h"
#include "perf-sys.h"
#include <api/fs/fs.h>
#include <api/fs/tracing_path.h>
#include <perf/core.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/zalloc.h>

static int use_pager = -1;
static FILE *debug_fp = NULL;

struct cmd_struct {
	const char *cmd;
	int (*fn)(int, const char **);
	int option;
};

static struct cmd_struct commands[] = {
	{ "archive",	NULL,	0 },
	{ "buildid-cache", cmd_buildid_cache, 0 },
	{ "buildid-list", cmd_buildid_list, 0 },
	{ "config",	cmd_config,	0 },
	{ "c2c",	cmd_c2c,	0 },
	{ "diff",	cmd_diff,	0 },
	{ "evlist",	cmd_evlist,	0 },
	{ "help",	cmd_help,	0 },
	{ "iostat",	NULL,	0 },
	{ "kallsyms",	cmd_kallsyms,	0 },
	{ "list",	cmd_list,	0 },
	{ "record",	cmd_record,	0 },
	{ "report",	cmd_report,	0 },
	{ "bench",	cmd_bench,	0 },
	{ "stat",	cmd_stat,	0 },
#ifdef HAVE_LIBTRACEEVENT
	{ "timechart",	cmd_timechart,	0 },
#endif
	{ "top",	cmd_top,	0 },
	{ "annotate",	cmd_annotate,	0 },
	{ "version",	cmd_version,	0 },
	{ "script",	cmd_script,	0 },
#ifdef HAVE_LIBTRACEEVENT
	{ "sched",	cmd_sched,	0 },
#endif
#ifdef HAVE_LIBELF_SUPPORT
	{ "probe",	cmd_probe,	0 },
#endif
#ifdef HAVE_LIBTRACEEVENT
	{ "kmem",	cmd_kmem,	0 },
	{ "lock",	cmd_lock,	0 },
#endif
	{ "kvm",	cmd_kvm,	0 },
	{ "test",	cmd_test,	0 },
#if defined(HAVE_LIBTRACEEVENT) && (defined(HAVE_LIBAUDIT_SUPPORT) || defined(HAVE_SYSCALL_TABLE_SUPPORT))
	{ "trace",	cmd_trace,	0 },
#endif
	{ "inject",	cmd_inject,	0 },
	{ "mem",	cmd_mem,	0 },
	{ "data",	cmd_data,	0 },
	{ "ftrace",	cmd_ftrace,	0 },
	{ "daemon",	cmd_daemon,	0 },
#ifdef HAVE_LIBTRACEEVENT
	{ "kwork",	cmd_kwork,	0 },
#endif
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct pager_config {
	const char *cmd;
	int val;
};

<<<<<<< HEAD
static int pager_command_config(const char *var, const char *value, void *data)
{
	struct pager_config *c = data;
	if (!prefixcmp(var, "pager.") && !strcmp(var + 6, c->cmd))
=======
static bool same_cmd_with_prefix(const char *var, struct pager_config *c,
				  const char *header)
{
	return (strstarts(var, header) && !strcmp(var + strlen(header), c->cmd));
}

static int pager_command_config(const char *var, const char *value, void *data)
{
	struct pager_config *c = data;
	if (same_cmd_with_prefix(var, c, "pager."))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		c->val = perf_config_bool(var, value);
	return 0;
}

/* returns 0 for "no pager", 1 for "use pager", and -1 for "not specified" */
<<<<<<< HEAD
int check_pager_config(const char *cmd)
{
	struct pager_config c;
	c.cmd = cmd;
	c.val = -1;
	perf_config(pager_command_config, &c);
	return c.val;
}

static int tui_command_config(const char *var, const char *value, void *data)
{
	struct pager_config *c = data;
	if (!prefixcmp(var, "tui.") && !strcmp(var + 4, c->cmd))
		c->val = perf_config_bool(var, value);
	return 0;
}

/* returns 0 for "no tui", 1 for "use tui", and -1 for "not specified" */
static int check_tui_config(const char *cmd)
{
	struct pager_config c;
	c.cmd = cmd;
	c.val = -1;
	perf_config(tui_command_config, &c);
	return c.val;
=======
static int check_pager_config(const char *cmd)
{
	int err;
	struct pager_config c;
	c.cmd = cmd;
	c.val = -1;
	err = perf_config(pager_command_config, &c);
	return err ?: c.val;
}

static int browser_command_config(const char *var, const char *value, void *data)
{
	struct pager_config *c = data;
	if (same_cmd_with_prefix(var, c, "tui."))
		c->val = perf_config_bool(var, value);
	if (same_cmd_with_prefix(var, c, "gtk."))
		c->val = perf_config_bool(var, value) ? 2 : 0;
	return 0;
}

/*
 * returns 0 for "no tui", 1 for "use tui", 2 for "use gtk",
 * and -1 for "not specified"
 */
static int check_browser_config(const char *cmd)
{
	int err;
	struct pager_config c;
	c.cmd = cmd;
	c.val = -1;
	err = perf_config(browser_command_config, &c);
	return err ?: c.val;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void commit_pager_choice(void)
{
	switch (use_pager) {
	case 0:
<<<<<<< HEAD
		setenv("PERF_PAGER", "cat", 1);
=======
		setenv(PERF_PAGER_ENVIRONMENT, "cat", 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case 1:
		/* setup_pager(); */
		break;
	default:
		break;
	}
}

<<<<<<< HEAD
=======
static int set_debug_file(const char *path)
{
	debug_fp = fopen(path, "w");
	if (!debug_fp) {
		fprintf(stderr, "Open debug file '%s' failed: %s\n",
			path, strerror(errno));
		return -1;
	}

	debug_set_file(debug_fp);
	return 0;
}

struct option options[] = {
	OPT_ARGUMENT("help", "help"),
	OPT_ARGUMENT("version", "version"),
	OPT_ARGUMENT("exec-path", "exec-path"),
	OPT_ARGUMENT("html-path", "html-path"),
	OPT_ARGUMENT("paginate", "paginate"),
	OPT_ARGUMENT("no-pager", "no-pager"),
	OPT_ARGUMENT("debugfs-dir", "debugfs-dir"),
	OPT_ARGUMENT("buildid-dir", "buildid-dir"),
	OPT_ARGUMENT("list-cmds", "list-cmds"),
	OPT_ARGUMENT("list-opts", "list-opts"),
	OPT_ARGUMENT("debug", "debug"),
	OPT_ARGUMENT("debug-file", "debug-file"),
	OPT_END()
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int handle_options(const char ***argv, int *argc, int *envchanged)
{
	int handled = 0;

	while (*argc > 0) {
		const char *cmd = (*argv)[0];
		if (cmd[0] != '-')
			break;

		/*
		 * For legacy reasons, the "version" and "help"
		 * commands can be written with "--" prepended
		 * to make them look like flags.
		 */
		if (!strcmp(cmd, "--help") || !strcmp(cmd, "--version"))
			break;

		/*
<<<<<<< HEAD
		 * Check remaining flags.
		 */
		if (!prefixcmp(cmd, CMD_EXEC_PATH)) {
			cmd += strlen(CMD_EXEC_PATH);
			if (*cmd == '=')
				perf_set_argv_exec_path(cmd + 1);
			else {
				puts(perf_exec_path());
=======
		 * Shortcut for '-h' and '-v' options to invoke help
		 * and version command.
		 */
		if (!strcmp(cmd, "-h")) {
			(*argv)[0] = "--help";
			break;
		}

		if (!strcmp(cmd, "-v")) {
			(*argv)[0] = "--version";
			break;
		}

		if (!strcmp(cmd, "-vv")) {
			(*argv)[0] = "version";
			verbose = 1;
			break;
		}

		/*
		 * Check remaining flags.
		 */
		if (strstarts(cmd, CMD_EXEC_PATH)) {
			cmd += strlen(CMD_EXEC_PATH);
			if (*cmd == '=')
				set_argv_exec_path(cmd + 1);
			else {
				puts(get_argv_exec_path());
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				exit(0);
			}
		} else if (!strcmp(cmd, "--html-path")) {
			puts(system_path(PERF_HTML_PATH));
			exit(0);
		} else if (!strcmp(cmd, "-p") || !strcmp(cmd, "--paginate")) {
			use_pager = 1;
		} else if (!strcmp(cmd, "--no-pager")) {
			use_pager = 0;
			if (envchanged)
				*envchanged = 1;
<<<<<<< HEAD
		} else if (!strcmp(cmd, "--perf-dir")) {
			if (*argc < 2) {
				fprintf(stderr, "No directory given for --perf-dir.\n");
				usage(perf_usage_string);
			}
			setenv(PERF_DIR_ENVIRONMENT, (*argv)[1], 1);
			if (envchanged)
				*envchanged = 1;
			(*argv)++;
			(*argc)--;
			handled++;
		} else if (!prefixcmp(cmd, CMD_PERF_DIR)) {
			setenv(PERF_DIR_ENVIRONMENT, cmd + strlen(CMD_PERF_DIR), 1);
			if (envchanged)
				*envchanged = 1;
		} else if (!strcmp(cmd, "--work-tree")) {
			if (*argc < 2) {
				fprintf(stderr, "No directory given for --work-tree.\n");
				usage(perf_usage_string);
			}
			setenv(PERF_WORK_TREE_ENVIRONMENT, (*argv)[1], 1);
			if (envchanged)
				*envchanged = 1;
			(*argv)++;
			(*argc)--;
		} else if (!prefixcmp(cmd, CMD_WORK_TREE)) {
			setenv(PERF_WORK_TREE_ENVIRONMENT, cmd + strlen(CMD_WORK_TREE), 1);
			if (envchanged)
				*envchanged = 1;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		} else if (!strcmp(cmd, "--debugfs-dir")) {
			if (*argc < 2) {
				fprintf(stderr, "No directory given for --debugfs-dir.\n");
				usage(perf_usage_string);
			}
<<<<<<< HEAD
			debugfs_set_path((*argv)[1]);
=======
			tracing_path_set((*argv)[1]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (envchanged)
				*envchanged = 1;
			(*argv)++;
			(*argc)--;
<<<<<<< HEAD
		} else if (!prefixcmp(cmd, CMD_DEBUGFS_DIR)) {
			debugfs_set_path(cmd + strlen(CMD_DEBUGFS_DIR));
			fprintf(stderr, "dir: %s\n", debugfs_mountpoint);
			if (envchanged)
				*envchanged = 1;
=======
		} else if (!strcmp(cmd, "--buildid-dir")) {
			if (*argc < 2) {
				fprintf(stderr, "No directory given for --buildid-dir.\n");
				usage(perf_usage_string);
			}
			set_buildid_dir((*argv)[1]);
			if (envchanged)
				*envchanged = 1;
			(*argv)++;
			(*argc)--;
		} else if (strstarts(cmd, CMD_DEBUGFS_DIR)) {
			tracing_path_set(cmd + strlen(CMD_DEBUGFS_DIR));
			fprintf(stderr, "dir: %s\n", tracing_path_mount());
			if (envchanged)
				*envchanged = 1;
		} else if (!strcmp(cmd, "--list-cmds")) {
			unsigned int i;

			for (i = 0; i < ARRAY_SIZE(commands); i++) {
				struct cmd_struct *p = commands+i;
				printf("%s ", p->cmd);
			}
			putchar('\n');
			exit(0);
		} else if (!strcmp(cmd, "--list-opts")) {
			unsigned int i;

			for (i = 0; i < ARRAY_SIZE(options)-1; i++) {
				struct option *p = options+i;
				printf("--%s ", p->long_name);
			}
			putchar('\n');
			exit(0);
		} else if (!strcmp(cmd, "--debug")) {
			if (*argc < 2) {
				fprintf(stderr, "No variable specified for --debug.\n");
				usage(perf_usage_string);
			}
			if (perf_debug_option((*argv)[1]))
				usage(perf_usage_string);

			(*argv)++;
			(*argc)--;
		} else if (!strcmp(cmd, "--debug-file")) {
			if (*argc < 2) {
				fprintf(stderr, "No path given for --debug-file.\n");
				usage(perf_usage_string);
			}

			if (set_debug_file((*argv)[1]))
				usage(perf_usage_string);

			(*argv)++;
			(*argc)--;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		} else {
			fprintf(stderr, "Unknown option: %s\n", cmd);
			usage(perf_usage_string);
		}

		(*argv)++;
		(*argc)--;
		handled++;
	}
	return handled;
}

<<<<<<< HEAD
static int handle_alias(int *argcp, const char ***argv)
{
	int envchanged = 0, ret = 0, saved_errno = errno;
	int count, option_count;
	const char **new_argv;
	const char *alias_command;
	char *alias_string;

	alias_command = (*argv)[0];
	alias_string = alias_lookup(alias_command);
	if (alias_string) {
		if (alias_string[0] == '!') {
			if (*argcp > 1) {
				struct strbuf buf;

				strbuf_init(&buf, PATH_MAX);
				strbuf_addstr(&buf, alias_string);
				sq_quote_argv(&buf, (*argv) + 1, PATH_MAX);
				free(alias_string);
				alias_string = buf.buf;
			}
			ret = system(alias_string + 1);
			if (ret >= 0 && WIFEXITED(ret) &&
			    WEXITSTATUS(ret) != 127)
				exit(WEXITSTATUS(ret));
			die("Failed to run '%s' when expanding alias '%s'",
			    alias_string + 1, alias_command);
		}
		count = split_cmdline(alias_string, &new_argv);
		if (count < 0)
			die("Bad alias.%s string", alias_command);
		option_count = handle_options(&new_argv, &count, &envchanged);
		if (envchanged)
			die("alias '%s' changes environment variables\n"
				 "You can use '!perf' in the alias to do this.",
				 alias_command);
		memmove(new_argv - option_count, new_argv,
				count * sizeof(char *));
		new_argv -= option_count;

		if (count < 1)
			die("empty alias for %s", alias_command);

		if (!strcmp(alias_command, new_argv[0]))
			die("recursive alias: %s", alias_command);

		new_argv = realloc(new_argv, sizeof(char *) *
				    (count + *argcp + 1));
		/* insert after command name */
		memcpy(new_argv + count, *argv + 1, sizeof(char *) * *argcp);
		new_argv[count + *argcp] = NULL;

		*argv = new_argv;
		*argcp += count - 1;

		ret = 1;
	}

	errno = saved_errno;

	return ret;
}

const char perf_version_string[] = PERF_VERSION;

#define RUN_SETUP	(1<<0)
#define USE_PAGER	(1<<1)
/*
 * require working tree to be present -- anything uses this needs
 * RUN_SETUP for reading from the configuration file.
 */
#define NEED_WORK_TREE	(1<<2)

struct cmd_struct {
	const char *cmd;
	int (*fn)(int, const char **, const char *);
	int option;
};
=======
#define RUN_SETUP	(1<<0)
#define USE_PAGER	(1<<1)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int run_builtin(struct cmd_struct *p, int argc, const char **argv)
{
	int status;
	struct stat st;
<<<<<<< HEAD
	const char *prefix;

	prefix = NULL;
	if (p->option & RUN_SETUP)
		prefix = NULL; /* setup_perf_directory(); */

	if (use_browser == -1)
		use_browser = check_tui_config(p->cmd);
=======
	char sbuf[STRERR_BUFSIZE];

	if (use_browser == -1)
		use_browser = check_browser_config(p->cmd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (use_pager == -1 && p->option & RUN_SETUP)
		use_pager = check_pager_config(p->cmd);
	if (use_pager == -1 && p->option & USE_PAGER)
		use_pager = 1;
	commit_pager_choice();

<<<<<<< HEAD
	status = p->fn(argc, argv, prefix);
	exit_browser(status);
=======
	perf_env__init(&perf_env);
	perf_env__set_cmdline(&perf_env, argc, argv);
	status = p->fn(argc, argv);
	perf_config__exit();
	exit_browser(status);
	perf_env__exit(&perf_env);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (status)
		return status & 0xff;

	/* Somebody closed stdout? */
	if (fstat(fileno(stdout), &st))
		return 0;
	/* Ignore write errors for pipes and sockets.. */
	if (S_ISFIFO(st.st_mode) || S_ISSOCK(st.st_mode))
		return 0;

<<<<<<< HEAD
	/* Check for ENOSPC and EIO errors.. */
	if (fflush(stdout))
		die("write failure on standard output: %s", strerror(errno));
	if (ferror(stdout))
		die("unknown write failure on standard output");
	if (fclose(stdout))
		die("close failed on standard output: %s", strerror(errno));
	return 0;
=======
	status = 1;
	/* Check for ENOSPC and EIO errors.. */
	if (fflush(stdout)) {
		fprintf(stderr, "write failure on standard output: %s",
			str_error_r(errno, sbuf, sizeof(sbuf)));
		goto out;
	}
	if (ferror(stdout)) {
		fprintf(stderr, "unknown write failure on standard output");
		goto out;
	}
	if (fclose(stdout)) {
		fprintf(stderr, "close failed on standard output: %s",
			str_error_r(errno, sbuf, sizeof(sbuf)));
		goto out;
	}
	status = 0;
out:
	return status;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void handle_internal_command(int argc, const char **argv)
{
	const char *cmd = argv[0];
<<<<<<< HEAD
	static struct cmd_struct commands[] = {
		{ "buildid-cache", cmd_buildid_cache, 0 },
		{ "buildid-list", cmd_buildid_list, 0 },
		{ "diff",	cmd_diff,	0 },
		{ "evlist",	cmd_evlist,	0 },
		{ "help",	cmd_help,	0 },
		{ "list",	cmd_list,	0 },
		{ "record",	cmd_record,	0 },
		{ "report",	cmd_report,	0 },
		{ "bench",	cmd_bench,	0 },
		{ "stat",	cmd_stat,	0 },
		{ "periodic",   cmd_periodic,   0 },
		{ "timechart",	cmd_timechart,	0 },
		{ "top",	cmd_top,	0 },
		{ "annotate",	cmd_annotate,	0 },
		{ "version",	cmd_version,	0 },
		{ "script",	cmd_script,	0 },
		{ "sched",	cmd_sched,	0 },
		{ "probe",	cmd_probe,	0 },
		{ "kmem",	cmd_kmem,	0 },
		{ "lock",	cmd_lock,	0 },
		{ "kvm",	cmd_kvm,	0 },
		{ "test",	cmd_test,	0 },
		{ "inject",	cmd_inject,	0 },
	};
	unsigned int i;
	static const char ext[] = STRIP_EXTENSION;

	if (sizeof(ext) > 1) {
		i = strlen(argv[0]) - strlen(ext);
		if (i > 0 && !strcmp(argv[0] + i, ext)) {
			char *argv0 = strdup(argv[0]);
			argv[0] = cmd = argv0;
			argv0[i] = '\0';
		}
	}
=======
	unsigned int i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Turn "perf cmd --help" into "perf help cmd" */
	if (argc > 1 && !strcmp(argv[1], "--help")) {
		argv[1] = argv[0];
		argv[0] = cmd = "help";
	}

	for (i = 0; i < ARRAY_SIZE(commands); i++) {
		struct cmd_struct *p = commands+i;
<<<<<<< HEAD
=======
		if (p->fn == NULL)
			continue;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (strcmp(p->cmd, cmd))
			continue;
		exit(run_builtin(p, argc, argv));
	}
}

static void execv_dashed_external(const char **argv)
{
<<<<<<< HEAD
	struct strbuf cmd = STRBUF_INIT;
	const char *tmp;
	int status;

	strbuf_addf(&cmd, "perf-%s", argv[0]);
=======
	char *cmd;
	const char *tmp;
	int status;

	if (asprintf(&cmd, "perf-%s", argv[0]) < 0)
		goto do_die;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * argv[0] must be the perf command, but the argv array
	 * belongs to the caller, and may be reused in
	 * subsequent loop iterations. Save argv[0] and
	 * restore it on error.
	 */
	tmp = argv[0];
<<<<<<< HEAD
	argv[0] = cmd.buf;
=======
	argv[0] = cmd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * if we fail because the command is not found, it is
	 * OK to return. Otherwise, we just pass along the status code.
	 */
	status = run_command_v_opt(argv, 0);
	if (status != -ERR_RUN_COMMAND_EXEC) {
<<<<<<< HEAD
		if (IS_RUN_COMMAND_ERR(status))
			die("unable to run '%s'", argv[0]);
=======
		if (IS_RUN_COMMAND_ERR(status)) {
do_die:
			pr_err("FATAL: unable to run '%s'", argv[0]);
			status = -128;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		exit(-status);
	}
	errno = ENOENT; /* as if we called execvp */

	argv[0] = tmp;
<<<<<<< HEAD

	strbuf_release(&cmd);
=======
	zfree(&cmd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int run_argv(int *argcp, const char ***argv)
{
<<<<<<< HEAD
	int done_alias = 0;

	while (1) {
		/* See if it's an internal command */
		handle_internal_command(*argcp, *argv);

		/* .. then try the external ones */
		execv_dashed_external(*argv);

		/* It could be an alias -- this works around the insanity
		 * of overriding "perf log" with "perf show" by having
		 * alias.log = show
		 */
		if (done_alias || !handle_alias(argcp, argv))
			break;
		done_alias = 1;
	}

	return done_alias;
}

static void pthread__block_sigwinch(void)
{
	sigset_t set;

	sigemptyset(&set);
	sigaddset(&set, SIGWINCH);
	pthread_sigmask(SIG_BLOCK, &set, NULL);
}

void pthread__unblock_sigwinch(void)
{
	sigset_t set;

	sigemptyset(&set);
	sigaddset(&set, SIGWINCH);
	pthread_sigmask(SIG_UNBLOCK, &set, NULL);
=======
	/* See if it's an internal command */
	handle_internal_command(*argcp, *argv);

	/* .. then try the external ones */
	execv_dashed_external(*argv);
	return 0;
}

static int libperf_print(enum libperf_print_level level,
			 const char *fmt, va_list ap)
{
	return veprintf(level, verbose, fmt, ap);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int main(int argc, const char **argv)
{
<<<<<<< HEAD
	const char *cmd;

	cmd = perf_extract_argv0_path(argv[0]);
	if (!cmd)
		cmd = "perf-help";
	/* get debugfs mount point from /proc/mounts */
	debugfs_mount(NULL);
=======
	int err;
	const char *cmd;
	char sbuf[STRERR_BUFSIZE];

	perf_debug_setup();

	/* libsubcmd init */
	exec_cmd_init("perf", PREFIX, PERF_EXEC_PATH, EXEC_PATH_ENVIRONMENT);
	pager_init(PERF_PAGER_ENVIRONMENT);

	libperf_init(libperf_print);

	cmd = extract_argv0_path(argv[0]);
	if (!cmd)
		cmd = "perf-help";

	srandom(time(NULL));

	/* Setting $PERF_CONFIG makes perf read _only_ the given config file. */
	config_exclusive_filename = getenv("PERF_CONFIG");

	err = perf_config(perf_default_config, NULL);
	if (err)
		return err;
	set_buildid_dir(NULL);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * "perf-xxxx" is the same as "perf xxxx", but we obviously:
	 *
	 *  - cannot take flags in between the "perf" and the "xxxx".
	 *  - cannot execute it externally (since it would just do
	 *    the same thing over again)
	 *
<<<<<<< HEAD
	 * So we just directly call the internal command handler, and
	 * die if that one cannot handle it.
	 */
	if (!prefixcmp(cmd, "perf-")) {
		cmd += 5;
		argv[0] = cmd;
		handle_internal_command(argc, argv);
		die("cannot handle %s internally", cmd);
	}

=======
	 * So we just directly call the internal command handler. If that one
	 * fails to handle this, then maybe we just run a renamed perf binary
	 * that contains a dash in its name. To handle this scenario, we just
	 * fall through and ignore the "xxxx" part of the command string.
	 */
	if (strstarts(cmd, "perf-")) {
		cmd += 5;
		argv[0] = cmd;
		handle_internal_command(argc, argv);
		/*
		 * If the command is handled, the above function does not
		 * return undo changes and fall through in such a case.
		 */
		cmd -= 5;
		argv[0] = cmd;
	}
	if (strstarts(cmd, "trace")) {
#ifndef HAVE_LIBTRACEEVENT
		fprintf(stderr,
			"trace command not available: missing libtraceevent devel package at build time.\n");
		goto out;
#elif !defined(HAVE_LIBAUDIT_SUPPORT) && !defined(HAVE_SYSCALL_TABLE_SUPPORT)
		fprintf(stderr,
			"trace command not available: missing audit-libs devel package at build time.\n");
		goto out;
#else
		setup_path();
		argv[0] = "trace";
		return cmd_trace(argc, argv);
#endif
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Look for flags.. */
	argv++;
	argc--;
	handle_options(&argv, &argc, NULL);
	commit_pager_choice();
<<<<<<< HEAD
	set_buildid_dir();

	if (argc > 0) {
		if (!prefixcmp(argv[0], "--"))
=======

	if (argc > 0) {
		if (strstarts(argv[0], "--"))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			argv[0] += 2;
	} else {
		/* The user didn't specify a command; give them help */
		printf("\n usage: %s\n\n", perf_usage_string);
		list_common_cmds_help();
		printf("\n %s\n\n", perf_more_info_string);
<<<<<<< HEAD
		exit(1);
	}
	cmd = argv[0];

=======
		goto out;
	}
	cmd = argv[0];

	test_attr__init();

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * We use PATH to find perf commands, but we prepend some higher
	 * precedence paths: the "--exec-path" option, the PERF_EXEC_PATH
	 * environment, and the $(perfexecdir) from the Makefile at build
	 * time.
	 */
	setup_path();
	/*
	 * Block SIGWINCH notifications so that the thread that wants it can
	 * unblock and get syscalls like select interrupted instead of waiting
	 * forever while the signal goes to some other non interested thread.
	 */
	pthread__block_sigwinch();

	while (1) {
		static int done_help;
<<<<<<< HEAD
		static int was_alias;

		was_alias = run_argv(&argc, &argv);
		if (errno != ENOENT)
			break;

		if (was_alias) {
			fprintf(stderr, "Expansion of alias '%s' failed; "
				"'%s' is not a perf-command\n",
				cmd, argv[0]);
			exit(1);
		}
=======

		run_argv(&argc, &argv);

		if (errno != ENOENT)
			break;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!done_help) {
			cmd = argv[0] = help_unknown_cmd(cmd);
			done_help = 1;
		} else
			break;
	}

	fprintf(stderr, "Failed to run command '%s': %s\n",
<<<<<<< HEAD
		cmd, strerror(errno));
=======
		cmd, str_error_r(errno, sbuf, sizeof(sbuf)));
out:
	if (debug_fp)
		fclose(debug_fp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 1;
}
