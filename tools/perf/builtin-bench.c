<<<<<<< HEAD
/*
 *
 * builtin-bench.c
 *
 * General benchmarking subsystem provided by perf
 *
 * Copyright (C) 2009, Hitoshi Mitake <mitake@dcl.info.waseda.ac.jp>
 *
 */

/*
 *
 * Available subsystem list:
 *  sched ... scheduler and IPC mechanism
 *  mem   ... memory access performance
 *
 */

#include "perf.h"
#include "util/util.h"
#include "util/parse-options.h"
#include "builtin.h"
#include "bench/bench.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct bench_suite {
	const char *name;
	const char *summary;
	int (*fn)(int, const char **, const char *);
};
						\
/* sentinel: easy for help */
#define suite_all { "all", "test all suite (pseudo suite)", NULL }

static struct bench_suite sched_suites[] = {
	{ "messaging",
	  "Benchmark for scheduler and IPC mechanisms",
	  bench_sched_messaging },
	{ "pipe",
	  "Flood of communication over pipe() between two processes",
	  bench_sched_pipe      },
	suite_all,
	{ NULL,
	  NULL,
	  NULL                  }
};

static struct bench_suite mem_suites[] = {
	{ "memcpy",
	  "Simple memory copy in various ways",
	  bench_mem_memcpy },
	{ "memset",
	  "Simple memory set in various ways",
	  bench_mem_memset },
	suite_all,
	{ NULL,
	  NULL,
	  NULL             }
};

struct bench_subsys {
	const char *name;
	const char *summary;
	struct bench_suite *suites;
};

static struct bench_subsys subsystems[] = {
	{ "sched",
	  "scheduler and IPC mechanism",
	  sched_suites },
	{ "mem",
	  "memory access performance",
	  mem_suites },
	{ "all",		/* sentinel: easy for help */
	  "test all subsystem (pseudo subsystem)",
	  NULL },
	{ NULL,
	  NULL,
	  NULL       }
};

static void dump_suites(int subsys_index)
{
	int i;

	printf("# List of available suites for %s...\n\n",
	       subsystems[subsys_index].name);

	for (i = 0; subsystems[subsys_index].suites[i].name; i++)
		printf("%14s: %s\n",
		       subsystems[subsys_index].suites[i].name,
		       subsystems[subsys_index].suites[i].summary);

	printf("\n");
	return;
}

static const char *bench_format_str;
int bench_format = BENCH_FORMAT_DEFAULT;

static const struct option bench_options[] = {
	OPT_STRING('f', "format", &bench_format_str, "default",
		    "Specify format style"),
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * builtin-bench.c
 *
 * General benchmarking collections provided by perf
 *
 * Copyright (C) 2009, Hitoshi Mitake <mitake@dcl.info.waseda.ac.jp>
 */

/*
 * Available benchmark collection list:
 *
 *  sched ... scheduler and IPC performance
 *  syscall ... System call performance
 *  mem   ... memory access performance
 *  numa  ... NUMA scheduling and MM performance
 *  futex ... Futex performance
 *  epoll ... Event poll performance
 */
#include <subcmd/parse-options.h>
#include "builtin.h"
#include "bench/bench.h"

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <linux/zalloc.h>

typedef int (*bench_fn_t)(int argc, const char **argv);

struct bench {
	const char	*name;
	const char	*summary;
	bench_fn_t	fn;
};

#ifdef HAVE_LIBNUMA_SUPPORT
static struct bench numa_benchmarks[] = {
	{ "mem",	"Benchmark for NUMA workloads",			bench_numa		},
	{ "all",	"Run all NUMA benchmarks",			NULL			},
	{ NULL,		NULL,						NULL			}
};
#endif

static struct bench sched_benchmarks[] = {
	{ "messaging",	"Benchmark for scheduling and IPC",		bench_sched_messaging	},
	{ "pipe",	"Benchmark for pipe() between two processes",	bench_sched_pipe	},
	{ "seccomp-notify",	"Benchmark for seccomp user notify",	bench_sched_seccomp_notify},
	{ "all",	"Run all scheduler benchmarks",		NULL			},
	{ NULL,		NULL,						NULL			}
};

static struct bench syscall_benchmarks[] = {
	{ "basic",	"Benchmark for basic getppid(2) calls",		bench_syscall_basic	},
	{ "getpgid",	"Benchmark for getpgid(2) calls",		bench_syscall_getpgid	},
	{ "fork",	"Benchmark for fork(2) calls",			bench_syscall_fork	},
	{ "execve",	"Benchmark for execve(2) calls",		bench_syscall_execve	},
	{ "all",	"Run all syscall benchmarks",			NULL			},
	{ NULL,		NULL,						NULL			},
};

static struct bench mem_benchmarks[] = {
	{ "memcpy",	"Benchmark for memcpy() functions",		bench_mem_memcpy	},
	{ "memset",	"Benchmark for memset() functions",		bench_mem_memset	},
	{ "find_bit",	"Benchmark for find_bit() functions",		bench_mem_find_bit	},
	{ "all",	"Run all memory access benchmarks",		NULL			},
	{ NULL,		NULL,						NULL			}
};

static struct bench futex_benchmarks[] = {
	{ "hash",	"Benchmark for futex hash table",               bench_futex_hash	},
	{ "wake",	"Benchmark for futex wake calls",               bench_futex_wake	},
	{ "wake-parallel", "Benchmark for parallel futex wake calls",   bench_futex_wake_parallel },
	{ "requeue",	"Benchmark for futex requeue calls",            bench_futex_requeue	},
	/* pi-futexes */
	{ "lock-pi",	"Benchmark for futex lock_pi calls",            bench_futex_lock_pi	},
	{ "all",	"Run all futex benchmarks",			NULL			},
	{ NULL,		NULL,						NULL			}
};

#ifdef HAVE_EVENTFD_SUPPORT
static struct bench epoll_benchmarks[] = {
	{ "wait",	"Benchmark epoll concurrent epoll_waits",       bench_epoll_wait	},
	{ "ctl",	"Benchmark epoll concurrent epoll_ctls",        bench_epoll_ctl		},
	{ "all",	"Run all futex benchmarks",			NULL			},
	{ NULL,		NULL,						NULL			}
};
#endif // HAVE_EVENTFD_SUPPORT

static struct bench internals_benchmarks[] = {
	{ "synthesize", "Benchmark perf event synthesis",	bench_synthesize	},
	{ "kallsyms-parse", "Benchmark kallsyms parsing",	bench_kallsyms_parse	},
	{ "inject-build-id", "Benchmark build-id injection",	bench_inject_build_id	},
	{ "evlist-open-close", "Benchmark evlist open and close",	bench_evlist_open_close	},
	{ "pmu-scan", "Benchmark sysfs PMU info scanning",	bench_pmu_scan		},
	{ NULL,		NULL,					NULL			}
};

static struct bench breakpoint_benchmarks[] = {
	{ "thread", "Benchmark thread start/finish with breakpoints", bench_breakpoint_thread},
	{ "enable", "Benchmark breakpoint enable/disable", bench_breakpoint_enable},
	{ "all", "Run all breakpoint benchmarks", NULL},
	{ NULL,	NULL, NULL },
};

static struct bench uprobe_benchmarks[] = {
	{ "baseline",	"Baseline libc usleep(1000) call",				bench_uprobe_baseline,	},
	{ "empty",	"Attach empty BPF prog to uprobe on usleep, system wide",	bench_uprobe_empty,	},
	{ "trace_printk", "Attach trace_printk BPF prog to uprobe on usleep syswide",	bench_uprobe_trace_printk,	},
	{ NULL,	NULL, NULL },
};

struct collection {
	const char	*name;
	const char	*summary;
	struct bench	*benchmarks;
};

static struct collection collections[] = {
	{ "sched",	"Scheduler and IPC benchmarks",			sched_benchmarks	},
	{ "syscall",	"System call benchmarks",			syscall_benchmarks	},
	{ "mem",	"Memory access benchmarks",			mem_benchmarks		},
#ifdef HAVE_LIBNUMA_SUPPORT
	{ "numa",	"NUMA scheduling and MM benchmarks",		numa_benchmarks		},
#endif
	{"futex",       "Futex stressing benchmarks",                   futex_benchmarks        },
#ifdef HAVE_EVENTFD_SUPPORT
	{"epoll",       "Epoll stressing benchmarks",                   epoll_benchmarks        },
#endif
	{ "internals",	"Perf-internals benchmarks",			internals_benchmarks	},
	{ "breakpoint",	"Breakpoint benchmarks",			breakpoint_benchmarks	},
	{ "uprobe",	"uprobe benchmarks",				uprobe_benchmarks	},
	{ "all",	"All benchmarks",				NULL			},
	{ NULL,		NULL,						NULL			}
};

/* Iterate over all benchmark collections: */
#define for_each_collection(coll) \
	for (coll = collections; coll->name; coll++)

/* Iterate over all benchmarks within a collection: */
#define for_each_bench(coll, bench) \
	for (bench = coll->benchmarks; bench && bench->name; bench++)

static void dump_benchmarks(struct collection *coll)
{
	struct bench *bench;

	printf("\n        # List of available benchmarks for collection '%s':\n\n", coll->name);

	for_each_bench(coll, bench)
		printf("%14s: %s\n", bench->name, bench->summary);

	printf("\n");
}

static const char *bench_format_str;

/* Output/formatting style, exported to benchmark modules: */
int bench_format = BENCH_FORMAT_DEFAULT;
unsigned int bench_repeat = 10; /* default number of times to repeat the run */

static const struct option bench_options[] = {
	OPT_STRING('f', "format", &bench_format_str, "default|simple", "Specify the output formatting style"),
	OPT_UINTEGER('r', "repeat",  &bench_repeat,   "Specify number of times to repeat the run"),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	OPT_END()
};

static const char * const bench_usage[] = {
<<<<<<< HEAD
	"perf bench [<common options>] <subsystem> <suite> [<options>]",
=======
	"perf bench [<common options>] <collection> <benchmark> [<options>]",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	NULL
};

static void print_usage(void)
{
<<<<<<< HEAD
=======
	struct collection *coll;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int i;

	printf("Usage: \n");
	for (i = 0; bench_usage[i]; i++)
		printf("\t%s\n", bench_usage[i]);
	printf("\n");

<<<<<<< HEAD
	printf("# List of available subsystems...\n\n");

	for (i = 0; subsystems[i].name; i++)
		printf("%14s: %s\n",
		       subsystems[i].name, subsystems[i].summary);
=======
	printf("        # List of all available benchmark collections:\n\n");

	for_each_collection(coll)
		printf("%14s: %s\n", coll->name, coll->summary);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	printf("\n");
}

static int bench_str2int(const char *str)
{
	if (!str)
		return BENCH_FORMAT_DEFAULT;

	if (!strcmp(str, BENCH_FORMAT_DEFAULT_STR))
		return BENCH_FORMAT_DEFAULT;
	else if (!strcmp(str, BENCH_FORMAT_SIMPLE_STR))
		return BENCH_FORMAT_SIMPLE;

	return BENCH_FORMAT_UNKNOWN;
}

<<<<<<< HEAD
static void all_suite(struct bench_subsys *subsys)	  /* FROM HERE */
{
	int i;
	const char *argv[2];
	struct bench_suite *suites = subsys->suites;
=======
/*
 * Run a specific benchmark but first rename the running task's ->comm[]
 * to something meaningful:
 */
static int run_bench(const char *coll_name, const char *bench_name, bench_fn_t fn,
		     int argc, const char **argv)
{
	int size;
	char *name;
	int ret;

	size = strlen(coll_name) + 1 + strlen(bench_name) + 1;

	name = zalloc(size);
	BUG_ON(!name);

	scnprintf(name, size, "%s-%s", coll_name, bench_name);

	prctl(PR_SET_NAME, name);
	argv[0] = name;

	ret = fn(argc, argv);

	free(name);

	return ret;
}

static void run_collection(struct collection *coll)
{
	struct bench *bench;
	const char *argv[2];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	argv[1] = NULL;
	/*
	 * TODO:
<<<<<<< HEAD
	 * preparing preset parameters for
	 * embedded, ordinary PC, HPC, etc...
	 * will be helpful
	 */
	for (i = 0; suites[i].fn; i++) {
		printf("# Running %s/%s benchmark...\n",
		       subsys->name,
		       suites[i].name);

		argv[1] = suites[i].name;
		suites[i].fn(1, argv, NULL);
=======
	 *
	 * Preparing preset parameters for
	 * embedded, ordinary PC, HPC, etc...
	 * would be helpful.
	 */
	for_each_bench(coll, bench) {
		if (!bench->fn)
			break;
		printf("# Running %s/%s benchmark...\n", coll->name, bench->name);

		argv[1] = bench->name;
		run_bench(coll->name, bench->name, bench->fn, 1, argv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		printf("\n");
	}
}

<<<<<<< HEAD
static void all_subsystem(void)
{
	int i;
	for (i = 0; subsystems[i].suites; i++)
		all_suite(&subsystems[i]);
}

int cmd_bench(int argc, const char **argv, const char *prefix __used)
{
	int i, j, status = 0;

	if (argc < 2) {
		/* No subsystem specified. */
=======
static void run_all_collections(void)
{
	struct collection *coll;

	for_each_collection(coll)
		run_collection(coll);
}

int cmd_bench(int argc, const char **argv)
{
	struct collection *coll;
	int ret = 0;

	/* Unbuffered output */
	setvbuf(stdout, NULL, _IONBF, 0);
	setlocale(LC_ALL, "");

	if (argc < 2) {
		/* No collection specified. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		print_usage();
		goto end;
	}

	argc = parse_options(argc, argv, bench_options, bench_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	bench_format = bench_str2int(bench_format_str);
	if (bench_format == BENCH_FORMAT_UNKNOWN) {
<<<<<<< HEAD
		printf("Unknown format descriptor:%s\n", bench_format_str);
=======
		printf("Unknown format descriptor: '%s'\n", bench_format_str);
		goto end;
	}

	if (bench_repeat == 0) {
		printf("Invalid repeat option: Must specify a positive value\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto end;
	}

	if (argc < 1) {
		print_usage();
		goto end;
	}

	if (!strcmp(argv[0], "all")) {
<<<<<<< HEAD
		all_subsystem();
		goto end;
	}

	for (i = 0; subsystems[i].name; i++) {
		if (strcmp(subsystems[i].name, argv[0]))
			continue;

		if (argc < 2) {
			/* No suite specified. */
			dump_suites(i);
=======
		run_all_collections();
		goto end;
	}

	for_each_collection(coll) {
		struct bench *bench;

		if (strcmp(coll->name, argv[0]))
			continue;

		if (argc < 2) {
			/* No bench specified. */
			dump_benchmarks(coll);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto end;
		}

		if (!strcmp(argv[1], "all")) {
<<<<<<< HEAD
			all_suite(&subsystems[i]);
			goto end;
		}

		for (j = 0; subsystems[i].suites[j].name; j++) {
			if (strcmp(subsystems[i].suites[j].name, argv[1]))
				continue;

			if (bench_format == BENCH_FORMAT_DEFAULT)
				printf("# Running %s/%s benchmark...\n",
				       subsystems[i].name,
				       subsystems[i].suites[j].name);
			status = subsystems[i].suites[j].fn(argc - 1,
							    argv + 1, prefix);
=======
			run_collection(coll);
			goto end;
		}

		for_each_bench(coll, bench) {
			if (strcmp(bench->name, argv[1]))
				continue;

			if (bench_format == BENCH_FORMAT_DEFAULT)
				printf("# Running '%s/%s' benchmark:\n", coll->name, bench->name);
			ret = run_bench(coll->name, bench->name, bench->fn, argc-1, argv+1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto end;
		}

		if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
<<<<<<< HEAD
			dump_suites(i);
			goto end;
		}

		printf("Unknown suite:%s for %s\n", argv[1], argv[0]);
		status = 1;
		goto end;
	}

	printf("Unknown subsystem:%s\n", argv[0]);
	status = 1;

end:
	return status;
=======
			dump_benchmarks(coll);
			goto end;
		}

		printf("Unknown benchmark: '%s' for collection '%s'\n", argv[1], argv[0]);
		ret = 1;
		goto end;
	}

	printf("Unknown collection: '%s'\n", argv[0]);
	ret = 1;

end:
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
