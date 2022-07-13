<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#if defined(__i386__) || defined(__x86_64__)
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>

#include <pci/pci.h>

#include "helpers/helpers.h"
<<<<<<< HEAD

=======
#include "cpufreq.h"
#include "acpi_cppc.h"

/* ACPI P-States Helper Functions for AMD Processors ***************/
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define MSR_AMD_PSTATE_STATUS	0xc0010063
#define MSR_AMD_PSTATE		0xc0010064
#define MSR_AMD_PSTATE_LIMIT	0xc0010061

<<<<<<< HEAD
union msr_pstate {
=======
union core_pstate {
	/* pre fam 17h: */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct {
		unsigned fid:6;
		unsigned did:3;
		unsigned vid:7;
		unsigned res1:6;
		unsigned nbdid:1;
		unsigned res2:2;
		unsigned nbvid:7;
		unsigned iddval:8;
		unsigned idddiv:2;
		unsigned res3:21;
		unsigned en:1;
<<<<<<< HEAD
	} bits;
	unsigned long long val;
};

static int get_did(int family, union msr_pstate pstate)
{
	int t;

	if (family == 0x12)
		t = pstate.val & 0xf;
	else
		t = pstate.bits.did;
=======
	} pstate;
	/* since fam 17h: */
	struct {
		unsigned fid:8;
		unsigned did:6;
		unsigned vid:8;
		unsigned iddval:8;
		unsigned idddiv:2;
		unsigned res1:31;
		unsigned en:1;
	} pstatedef;
	unsigned long long val;
};

static int get_did(union core_pstate pstate)
{
	int t;

	if (cpupower_cpu_info.caps & CPUPOWER_CAP_AMD_PSTATEDEF)
		t = pstate.pstatedef.did;
	else if (cpupower_cpu_info.family == 0x12)
		t = pstate.val & 0xf;
	else
		t = pstate.pstate.did;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return t;
}

<<<<<<< HEAD
static int get_cof(int family, union msr_pstate pstate)
{
	int t;
	int fid, did;

	did = get_did(family, pstate);

	t = 0x10;
	fid = pstate.bits.fid;
	if (family == 0x11)
		t = 0x8;

	return (100 * (fid + t)) >> did;
=======
static int get_cof(union core_pstate pstate)
{
	int t;
	int fid, did, cof;

	did = get_did(pstate);
	if (cpupower_cpu_info.caps & CPUPOWER_CAP_AMD_PSTATEDEF) {
		fid = pstate.pstatedef.fid;
		cof = 200 * fid / did;
	} else {
		t = 0x10;
		fid = pstate.pstate.fid;
		if (cpupower_cpu_info.family == 0x11)
			t = 0x8;
		cof = (100 * (fid + t)) >> did;
	}
	return cof;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Needs:
 * cpu          -> the cpu that gets evaluated
<<<<<<< HEAD
 * cpu_family   -> The cpu's family (0x10, 0x12,...)
 * boots_states -> how much boost states the machines support
=======
 * boost_states -> how much boost states the machines support
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Fills up:
 * pstates -> a pointer to an array of size MAX_HW_PSTATES
 *            must be initialized with zeros.
 *            All available  HW pstates (including boost states)
 * no      -> amount of pstates above array got filled up with
 *
 * returns zero on success, -1 on failure
 */
<<<<<<< HEAD
int decode_pstates(unsigned int cpu, unsigned int cpu_family,
		   int boost_states, unsigned long *pstates, int *no)
{
	int i, psmax, pscur;
	union msr_pstate pstate;
	unsigned long long val;

	/* Only read out frequencies from HW when CPU might be boostable
	   to keep the code as short and clean as possible.
	   Otherwise frequencies are exported via ACPI tables.
	*/
	if (cpu_family < 0x10 || cpu_family == 0x14)
=======
int decode_pstates(unsigned int cpu, int boost_states,
		   unsigned long *pstates, int *no)
{
	int i, psmax;
	union core_pstate pstate;
	unsigned long long val;

	/* Only read out frequencies from HW if HW Pstate is supported,
	 * otherwise frequencies are exported via ACPI tables.
	 */
	if (!(cpupower_cpu_info.caps & CPUPOWER_CAP_AMD_HW_PSTATE))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -1;

	if (read_msr(cpu, MSR_AMD_PSTATE_LIMIT, &val))
		return -1;

	psmax = (val >> 4) & 0x7;
<<<<<<< HEAD

	if (read_msr(cpu, MSR_AMD_PSTATE_STATUS, &val))
		return -1;

	pscur = val & 0x7;

	pscur += boost_states;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	psmax += boost_states;
	for (i = 0; i <= psmax; i++) {
		if (i >= MAX_HW_PSTATES) {
			fprintf(stderr, "HW pstates [%d] exceeding max [%d]\n",
				psmax, MAX_HW_PSTATES);
			return -1;
		}
		if (read_msr(cpu, MSR_AMD_PSTATE + i, &pstate.val))
			return -1;
<<<<<<< HEAD
		pstates[i] = get_cof(cpu_family, pstate);
=======

		/* The enabled bit (bit 63) is common for all families */
		if (!pstate.pstatedef.en)
			continue;

		pstates[i] = get_cof(pstate);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	*no = i;
	return 0;
}

int amd_pci_get_num_boost_states(int *active, int *states)
{
	struct pci_access *pci_acc;
	struct pci_dev *device;
	uint8_t val = 0;

	*active = *states = 0;

	device = pci_slot_func_init(&pci_acc, 0x18, 4);

	if (device == NULL)
		return -ENODEV;

	val = pci_read_byte(device, 0x15c);
	if (val & 3)
		*active = 1;
	else
		*active = 0;
	*states = (val >> 2) & 7;

	pci_cleanup(pci_acc);
	return 0;
}
<<<<<<< HEAD
=======

/* ACPI P-States Helper Functions for AMD Processors ***************/

/* AMD P-State Helper Functions ************************************/
enum amd_pstate_value {
	AMD_PSTATE_HIGHEST_PERF,
	AMD_PSTATE_MAX_FREQ,
	AMD_PSTATE_LOWEST_NONLINEAR_FREQ,
	MAX_AMD_PSTATE_VALUE_READ_FILES,
};

static const char *amd_pstate_value_files[MAX_AMD_PSTATE_VALUE_READ_FILES] = {
	[AMD_PSTATE_HIGHEST_PERF] = "amd_pstate_highest_perf",
	[AMD_PSTATE_MAX_FREQ] = "amd_pstate_max_freq",
	[AMD_PSTATE_LOWEST_NONLINEAR_FREQ] = "amd_pstate_lowest_nonlinear_freq",
};

static unsigned long amd_pstate_get_data(unsigned int cpu,
					 enum amd_pstate_value value)
{
	return cpufreq_get_sysfs_value_from_table(cpu,
						  amd_pstate_value_files,
						  value,
						  MAX_AMD_PSTATE_VALUE_READ_FILES);
}

void amd_pstate_boost_init(unsigned int cpu, int *support, int *active)
{
	unsigned long highest_perf, nominal_perf, cpuinfo_min,
		      cpuinfo_max, amd_pstate_max;

	highest_perf = amd_pstate_get_data(cpu, AMD_PSTATE_HIGHEST_PERF);
	nominal_perf = acpi_cppc_get_data(cpu, NOMINAL_PERF);

	*support = highest_perf > nominal_perf ? 1 : 0;
	if (!(*support))
		return;

	cpufreq_get_hardware_limits(cpu, &cpuinfo_min, &cpuinfo_max);
	amd_pstate_max = amd_pstate_get_data(cpu, AMD_PSTATE_MAX_FREQ);

	*active = cpuinfo_max == amd_pstate_max ? 1 : 0;
}

void amd_pstate_show_perf_and_freq(unsigned int cpu, int no_rounding)
{
	printf(_("    AMD PSTATE Highest Performance: %lu. Maximum Frequency: "),
	       amd_pstate_get_data(cpu, AMD_PSTATE_HIGHEST_PERF));
	/*
	 * If boost isn't active, the cpuinfo_max doesn't indicate real max
	 * frequency. So we read it back from amd-pstate sysfs entry.
	 */
	print_speed(amd_pstate_get_data(cpu, AMD_PSTATE_MAX_FREQ), no_rounding);
	printf(".\n");

	printf(_("    AMD PSTATE Nominal Performance: %lu. Nominal Frequency: "),
	       acpi_cppc_get_data(cpu, NOMINAL_PERF));
	print_speed(acpi_cppc_get_data(cpu, NOMINAL_FREQ) * 1000,
		    no_rounding);
	printf(".\n");

	printf(_("    AMD PSTATE Lowest Non-linear Performance: %lu. Lowest Non-linear Frequency: "),
	       acpi_cppc_get_data(cpu, LOWEST_NONLINEAR_PERF));
	print_speed(amd_pstate_get_data(cpu, AMD_PSTATE_LOWEST_NONLINEAR_FREQ),
		    no_rounding);
	printf(".\n");

	printf(_("    AMD PSTATE Lowest Performance: %lu. Lowest Frequency: "),
	       acpi_cppc_get_data(cpu, LOWEST_PERF));
	print_speed(acpi_cppc_get_data(cpu, LOWEST_FREQ) * 1000, no_rounding);
	printf(".\n");
}

/* AMD P-State Helper Functions ************************************/
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* defined(__i386__) || defined(__x86_64__) */
