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
#include "helpers/bitmask.h"

static struct option set_opts[] = {
<<<<<<< HEAD
	{ .name = "perf-bias",	.has_arg = required_argument,	.flag = NULL,	.val = 'b'},
	{ .name = "sched-mc",	.has_arg = required_argument,	.flag = NULL,	.val = 'm'},
	{ .name = "sched-smt",	.has_arg = required_argument,	.flag = NULL,	.val = 's'},
=======
	{"perf-bias", required_argument, NULL, 'b'},
	{"epp", required_argument, NULL, 'e'},
	{"amd-pstate-mode", required_argument, NULL, 'm'},
	{"turbo-boost", required_argument, NULL, 't'},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ },
};

static void print_wrong_arg_exit(void)
{
	printf(_("invalid or unknown argument\n"));
	exit(EXIT_FAILURE);
}

int cmd_set(int argc, char **argv)
{
	extern char *optarg;
	extern int optind, opterr, optopt;
	unsigned int cpu;
<<<<<<< HEAD

	union {
		struct {
			int sched_mc:1;
			int sched_smt:1;
			int perf_bias:1;
		};
		int params;
	} params;
	int sched_mc = 0, sched_smt = 0, perf_bias = 0;
	int ret = 0;
=======
	struct utsname uts;

	union {
		struct {
			int perf_bias:1;
			int epp:1;
			int mode:1;
			int turbo_boost:1;
		};
		int params;
	} params;
	int perf_bias = 0, turbo_boost = 1;
	int ret = 0;
	char epp[30], mode[20];

	ret = uname(&uts);
	if (!ret && (!strcmp(uts.machine, "ppc64le") ||
		     !strcmp(uts.machine, "ppc64"))) {
		fprintf(stderr, _("Subcommand not supported on POWER.\n"));
		return ret;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	setlocale(LC_ALL, "");
	textdomain(PACKAGE);

	params.params = 0;
	/* parameter parsing */
<<<<<<< HEAD
	while ((ret = getopt_long(argc, argv, "m:s:b:",
=======
	while ((ret = getopt_long(argc, argv, "b:e:m:",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
						set_opts, NULL)) != -1) {
		switch (ret) {
		case 'b':
			if (params.perf_bias)
				print_wrong_arg_exit();
			perf_bias = atoi(optarg);
			if (perf_bias < 0 || perf_bias > 15) {
				printf(_("--perf-bias param out "
					 "of range [0-%d]\n"), 15);
				print_wrong_arg_exit();
			}
			params.perf_bias = 1;
			break;
<<<<<<< HEAD
		case 'm':
			if (params.sched_mc)
				print_wrong_arg_exit();
			sched_mc = atoi(optarg);
			if (sched_mc < 0 || sched_mc > 2) {
				printf(_("--sched-mc param out "
					 "of range [0-%d]\n"), 2);
				print_wrong_arg_exit();
			}
			params.sched_mc = 1;
			break;
		case 's':
			if (params.sched_smt)
				print_wrong_arg_exit();
			sched_smt = atoi(optarg);
			if (sched_smt < 0 || sched_smt > 2) {
				printf(_("--sched-smt param out "
					 "of range [0-%d]\n"), 2);
				print_wrong_arg_exit();
			}
			params.sched_smt = 1;
			break;
		default:
			print_wrong_arg_exit();
		}
	};
=======
		case 'e':
			if (params.epp)
				print_wrong_arg_exit();
			if (sscanf(optarg, "%29s", epp) != 1) {
				print_wrong_arg_exit();
				return -EINVAL;
			}
			params.epp = 1;
			break;
		case 'm':
			if (cpupower_cpu_info.vendor != X86_VENDOR_AMD)
				print_wrong_arg_exit();
			if (params.mode)
				print_wrong_arg_exit();
			if (sscanf(optarg, "%19s", mode) != 1) {
				print_wrong_arg_exit();
				return -EINVAL;
			}
			params.mode = 1;
			break;
		case 't':
			if (params.turbo_boost)
				print_wrong_arg_exit();
			turbo_boost = atoi(optarg);
			if (turbo_boost < 0 || turbo_boost > 1) {
				printf("--turbo-boost param out of range [0-1]\n");
				print_wrong_arg_exit();
			}
			params.turbo_boost = 1;
			break;


		default:
			print_wrong_arg_exit();
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!params.params)
		print_wrong_arg_exit();

<<<<<<< HEAD
	if (params.sched_mc) {
		ret = sysfs_set_sched("mc", sched_mc);
		if (ret)
			fprintf(stderr, _("Error setting sched-mc %s\n"),
				(ret == -ENODEV) ? "not supported" : "");
	}
	if (params.sched_smt) {
		ret = sysfs_set_sched("smt", sched_smt);
		if (ret)
			fprintf(stderr, _("Error setting sched-smt %s\n"),
				(ret == -ENODEV) ? "not supported" : "");
=======
	if (params.mode) {
		ret = cpupower_set_amd_pstate_mode(mode);
		if (ret)
			fprintf(stderr, "Error setting mode\n");
	}

	if (params.turbo_boost) {
		ret = cpupower_set_turbo_boost(turbo_boost);
		if (ret)
			fprintf(stderr, "Error setting turbo-boost\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* Default is: set all CPUs */
	if (bitmask_isallclear(cpus_chosen))
		bitmask_setall(cpus_chosen);

	/* loop over CPUs */
	for (cpu = bitmask_first(cpus_chosen);
	     cpu <= bitmask_last(cpus_chosen); cpu++) {

<<<<<<< HEAD
		if (!bitmask_isbitset(cpus_chosen, cpu) ||
		    cpufreq_cpu_exists(cpu))
			continue;

		if (params.perf_bias) {
			ret = msr_intel_set_perf_bias(cpu, perf_bias);
=======
		if (!bitmask_isbitset(cpus_chosen, cpu))
			continue;

		if (sysfs_is_cpu_online(cpu) != 1){
			fprintf(stderr, _("Cannot set values on CPU %d:"), cpu);
			fprintf(stderr, _(" *is offline\n"));
			continue;
		}

		if (params.perf_bias) {
			ret = cpupower_intel_set_perf_bias(cpu, perf_bias);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (ret) {
				fprintf(stderr, _("Error setting perf-bias "
						  "value on CPU %d\n"), cpu);
				break;
			}
		}
<<<<<<< HEAD
=======

		if (params.epp) {
			ret = cpupower_set_epp(cpu, epp);
			if (ret) {
				fprintf(stderr,
					"Error setting epp value on CPU %d\n", cpu);
				break;
			}
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return ret;
}
