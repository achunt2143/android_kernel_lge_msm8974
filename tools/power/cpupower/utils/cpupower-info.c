<<<<<<< HEAD
/*
 *  (C) 2011 Thomas Renninger <trenn@suse.de>, Novell Inc.
 *
 *  Licensed under the terms of the GNU GPL License version 2.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 *  (C) 2011 Thomas Renninger <trenn@suse.de>, Novell Inc.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
<<<<<<< HEAD

#include <cpufreq.h>
=======
#include <sys/utsname.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "helpers/helpers.h"
#include "helpers/sysfs.h"

static struct option set_opts[] = {
<<<<<<< HEAD
	{ .name = "perf-bias",	.has_arg = optional_argument,	.flag = NULL,	.val = 'b'},
	{ .name = "sched-mc",	.has_arg = optional_argument,	.flag = NULL,	.val = 'm'},
	{ .name = "sched-smt",	.has_arg = optional_argument,	.flag = NULL,	.val = 's'},
	{ },
=======
     {"perf-bias", optional_argument, NULL, 'b'},
     { },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static void print_wrong_arg_exit(void)
{
	printf(_("invalid or unknown argument\n"));
	exit(EXIT_FAILURE);
}

int cmd_info(int argc, char **argv)
{
	extern char *optarg;
	extern int optind, opterr, optopt;
	unsigned int cpu;
<<<<<<< HEAD

	union {
		struct {
			int sched_mc:1;
			int sched_smt:1;
=======
	struct utsname uts;

	union {
		struct {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			int perf_bias:1;
		};
		int params;
	} params = {};
	int ret = 0;

<<<<<<< HEAD
=======
	ret = uname(&uts);
	if (!ret && (!strcmp(uts.machine, "ppc64le") ||
		     !strcmp(uts.machine, "ppc64"))) {
		fprintf(stderr, _("Subcommand not supported on POWER.\n"));
		return ret;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	setlocale(LC_ALL, "");
	textdomain(PACKAGE);

	/* parameter parsing */
<<<<<<< HEAD
	while ((ret = getopt_long(argc, argv, "msb", set_opts, NULL)) != -1) {
=======
	while ((ret = getopt_long(argc, argv, "b", set_opts, NULL)) != -1) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		switch (ret) {
		case 'b':
			if (params.perf_bias)
				print_wrong_arg_exit();
			params.perf_bias = 1;
			break;
<<<<<<< HEAD
		case 'm':
			if (params.sched_mc)
				print_wrong_arg_exit();
			params.sched_mc = 1;
			break;
		case 's':
			if (params.sched_smt)
				print_wrong_arg_exit();
			params.sched_smt = 1;
			break;
		default:
			print_wrong_arg_exit();
		}
	};
=======
		default:
			print_wrong_arg_exit();
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!params.params)
		params.params = 0x7;

<<<<<<< HEAD
	/* Default is: show output of CPU 0 only */
	if (bitmask_isallclear(cpus_chosen))
		bitmask_setbit(cpus_chosen, 0);

	if (params.sched_mc) {
		ret = sysfs_get_sched("mc");
		printf(_("System's multi core scheduler setting: "));
		if (ret < 0)
			/* if sysfs file is missing it's: errno == ENOENT */
			printf(_("not supported\n"));
		else
			printf("%d\n", ret);
	}
	if (params.sched_smt) {
		ret = sysfs_get_sched("smt");
		printf(_("System's thread sibling scheduler setting: "));
		if (ret < 0)
			/* if sysfs file is missing it's: errno == ENOENT */
			printf(_("not supported\n"));
		else
			printf("%d\n", ret);
	}
=======
	/* Default is: show output of base_cpu only */
	if (bitmask_isallclear(cpus_chosen))
		bitmask_setbit(cpus_chosen, base_cpu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Add more per cpu options here */
	if (!params.perf_bias)
		return ret;

	if (params.perf_bias) {
		if (!run_as_root) {
			params.perf_bias = 0;
			printf(_("Intel's performance bias setting needs root privileges\n"));
		} else if (!(cpupower_cpu_info.caps & CPUPOWER_CAP_PERF_BIAS)) {
			printf(_("System does not support Intel's performance"
				 " bias setting\n"));
			params.perf_bias = 0;
		}
	}

	/* loop over CPUs */
	for (cpu = bitmask_first(cpus_chosen);
	     cpu <= bitmask_last(cpus_chosen); cpu++) {

<<<<<<< HEAD
		if (!bitmask_isbitset(cpus_chosen, cpu) ||
		    cpufreq_cpu_exists(cpu))
=======
		if (!bitmask_isbitset(cpus_chosen, cpu))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			continue;

		printf(_("analyzing CPU %d:\n"), cpu);

<<<<<<< HEAD
		if (params.perf_bias) {
			ret = msr_intel_get_perf_bias(cpu);
			if (ret < 0) {
				printf(_("Could not read perf-bias value\n"));
				break;
=======
		if (sysfs_is_cpu_online(cpu) != 1){
			printf(_(" *is offline\n"));
			continue;
		}

		if (params.perf_bias) {
			ret = cpupower_intel_get_perf_bias(cpu);
			if (ret < 0) {
				fprintf(stderr,
			_("Could not read perf-bias value[%d]\n"), ret);
				exit(EXIT_FAILURE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			} else
				printf(_("perf-bias: %d\n"), ret);
		}
	}
<<<<<<< HEAD
	return ret;
=======
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
