<<<<<<< HEAD
#include <sys/types.h>
=======
// SPDX-License-Identifier: GPL-2.0
#include <sys/types.h>
#include <errno.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
<<<<<<< HEAD

#include "../../util/header.h"

#define __stringify_1(x)        #x
#define __stringify(x)          __stringify_1(x)

#define mfspr(rn)       ({unsigned long rval; \
			 asm volatile("mfspr %0," __stringify(rn) \
				      : "=r" (rval)); rval; })

#define SPRN_PVR        0x11F	/* Processor Version Register */
#define PVR_VER(pvr)    (((pvr) >>  16) & 0xFFFF) /* Version field */
#define PVR_REV(pvr)    (((pvr) >>   0) & 0xFFFF) /* Revison field */
=======
#include <linux/stringify.h>
#include "header.h"
#include "utils_header.h"
#include "metricgroup.h"
#include <api/fs/fs.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int
get_cpuid(char *buffer, size_t sz)
{
	unsigned long pvr;
	int nb;

	pvr = mfspr(SPRN_PVR);

	nb = scnprintf(buffer, sz, "%lu,%lu$", PVR_VER(pvr), PVR_REV(pvr));

	/* look for end marker to ensure the entire data fit */
	if (strchr(buffer, '$')) {
		buffer[nb-1] = '\0';
		return 0;
	}
<<<<<<< HEAD
	return -1;
=======
	return ENOBUFS;
}

char *
get_cpuid_str(struct perf_pmu *pmu __maybe_unused)
{
	char *bufp;

	if (asprintf(&bufp, "0x%.8lx", mfspr(SPRN_PVR)) < 0)
		bufp = NULL;

	return bufp;
}

int arch_get_runtimeparam(const struct pmu_metric *pm)
{
	int count;
	char path[PATH_MAX] = "/devices/hv_24x7/interface/";

	strcat(path, pm->aggr_mode == PerChip ? "sockets" : "coresperchip");
	return sysfs__read_int(path, &count) < 0 ? 1 : count;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
