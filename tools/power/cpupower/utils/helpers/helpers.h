<<<<<<< HEAD
/*
 *  (C) 2010,2011       Thomas Renninger <trenn@suse.de>, Novell Inc.
 *
 *  Licensed under the terms of the GNU GPL License version 2.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 *  (C) 2010,2011       Thomas Renninger <trenn@suse.de>, Novell Inc.
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Miscellaneous helpers which do not fit or are worth
 * to put into separate headers
 */

#ifndef __CPUPOWERUTILS_HELPERS__
#define __CPUPOWERUTILS_HELPERS__

#include <libintl.h>
#include <locale.h>
<<<<<<< HEAD

#include "helpers/bitmask.h"
=======
#include <stdbool.h>

#include "helpers/bitmask.h"
#include <cpupower.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Internationalization ****************************/
#ifdef NLS

#define _(String) gettext(String)
#ifndef gettext_noop
#define gettext_noop(String) String
#endif
#define N_(String) gettext_noop(String)

#else /* !NLS */

#define _(String) String
#define N_(String) String

#endif
/* Internationalization ****************************/

extern int run_as_root;
<<<<<<< HEAD
=======
extern int base_cpu;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern struct bitmask *cpus_chosen;

/* Global verbose (-d) stuff *********************************/
/*
 * define DEBUG via global Makefile variable
 * Debug output is sent to stderr, do:
 * cpupower monitor 2>/tmp/debug
 * to split debug output away from normal output
*/
#ifdef DEBUG
extern int be_verbose;

#define dprint(fmt, ...) {					\
		if (be_verbose) {				\
			fprintf(stderr, "%s: " fmt,		\
				__func__, ##__VA_ARGS__);	\
		}						\
	}
#else
static inline void dprint(const char *fmt, ...) { }
#endif
extern int be_verbose;
/* Global verbose (-v) stuff *********************************/

/* cpuid and cpuinfo helpers  **************************/
enum cpupower_cpu_vendor {X86_VENDOR_UNKNOWN = 0, X86_VENDOR_INTEL,
<<<<<<< HEAD
			  X86_VENDOR_AMD, X86_VENDOR_MAX};

#define CPUPOWER_CAP_INV_TSC		0x00000001
#define CPUPOWER_CAP_APERF		0x00000002
#define CPUPOWER_CAP_AMD_CBP		0x00000004
=======
			  X86_VENDOR_AMD, X86_VENDOR_HYGON, X86_VENDOR_MAX};

#define CPUPOWER_CAP_INV_TSC		0x00000001
#define CPUPOWER_CAP_APERF		0x00000002
#define CPUPOWER_CAP_AMD_CPB		0x00000004
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define CPUPOWER_CAP_PERF_BIAS		0x00000008
#define CPUPOWER_CAP_HAS_TURBO_RATIO	0x00000010
#define CPUPOWER_CAP_IS_SNB		0x00000020
#define CPUPOWER_CAP_INTEL_IDA		0x00000040
<<<<<<< HEAD
=======
#define CPUPOWER_CAP_AMD_RDPRU		0x00000080
#define CPUPOWER_CAP_AMD_HW_PSTATE	0x00000100
#define CPUPOWER_CAP_AMD_PSTATEDEF	0x00000200
#define CPUPOWER_CAP_AMD_CPB_MSR	0x00000400
#define CPUPOWER_CAP_AMD_PSTATE		0x00000800

#define CPUPOWER_AMD_CPBDIS		0x02000000
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define MAX_HW_PSTATES 10

struct cpupower_cpu_info {
	enum cpupower_cpu_vendor vendor;
	unsigned int family;
	unsigned int model;
	unsigned int stepping;
	/* CPU capabilities read out from cpuid */
	unsigned long long caps;
};

/* get_cpu_info
 *
 * Extract CPU vendor, family, model, stepping info from /proc/cpuinfo
 *
<<<<<<< HEAD
 * Returns 0 on success or a negativ error code
 * Only used on x86, below global's struct values are zero/unknown on
 * other archs
 */
extern int get_cpu_info(unsigned int cpu, struct cpupower_cpu_info *cpu_info);
extern struct cpupower_cpu_info cpupower_cpu_info;
/* cpuid and cpuinfo helpers  **************************/


/* CPU topology/hierarchy parsing ******************/
struct cpupower_topology {
	/* Amount of CPU cores, packages and threads per core in the system */
	unsigned int cores;
	unsigned int pkgs;
	unsigned int threads; /* per core */

	/* Array gets mallocated with cores entries, holding per core info */
	struct {
		int pkg;
		int core;
		int cpu;

		/* flags */
		unsigned int is_online:1;
	} *core_info;
};

extern int get_cpu_topology(struct cpupower_topology *cpu_top);
extern void cpu_topology_release(struct cpupower_topology cpu_top);
/* CPU topology/hierarchy parsing ******************/

=======
 * Returns 0 on success or a negative error code
 * Only used on x86, below global's struct values are zero/unknown on
 * other archs
 */
extern int get_cpu_info(struct cpupower_cpu_info *cpu_info);
extern struct cpupower_cpu_info cpupower_cpu_info;


/* cpuid and cpuinfo helpers  **************************/

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* X86 ONLY ****************************************/
#if defined(__i386__) || defined(__x86_64__)

#include <pci/pci.h>

/* Read/Write msr ****************************/
extern int read_msr(int cpu, unsigned int idx, unsigned long long *val);
extern int write_msr(int cpu, unsigned int idx, unsigned long long val);

<<<<<<< HEAD
extern int msr_intel_set_perf_bias(unsigned int cpu, unsigned int val);
extern int msr_intel_get_perf_bias(unsigned int cpu);
extern unsigned long long msr_intel_get_turbo_ratio(unsigned int cpu);

=======
extern int cpupower_intel_set_perf_bias(unsigned int cpu, unsigned int val);
extern int cpupower_intel_get_perf_bias(unsigned int cpu);
extern unsigned long long msr_intel_get_turbo_ratio(unsigned int cpu);

extern int cpupower_set_epp(unsigned int cpu, char *epp);
extern int cpupower_set_amd_pstate_mode(char *mode);
extern int cpupower_set_turbo_boost(int turbo_boost);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Read/Write msr ****************************/

/* PCI stuff ****************************/
extern int amd_pci_get_num_boost_states(int *active, int *states);
extern struct pci_dev *pci_acc_init(struct pci_access **pacc, int domain,
				    int bus, int slot, int func, int vendor,
				    int dev);
extern struct pci_dev *pci_slot_func_init(struct pci_access **pacc,
					      int slot, int func);

/* PCI stuff ****************************/

/* AMD HW pstate decoding **************************/

<<<<<<< HEAD
extern int decode_pstates(unsigned int cpu, unsigned int cpu_family,
			  int boost_states, unsigned long *pstates, int *no);
=======
extern int decode_pstates(unsigned int cpu, int boost_states,
			  unsigned long *pstates, int *no);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* AMD HW pstate decoding **************************/

extern int cpufreq_has_boost_support(unsigned int cpu, int *support,
				     int *active, int * states);
<<<<<<< HEAD
=======

/* AMD P-State stuff **************************/
bool cpupower_amd_pstate_enabled(void);
void amd_pstate_boost_init(unsigned int cpu,
			   int *support, int *active);
void amd_pstate_show_perf_and_freq(unsigned int cpu,
				   int no_rounding);

/* AMD P-State stuff **************************/

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * CPUID functions returning a single datum
 */
unsigned int cpuid_eax(unsigned int op);
unsigned int cpuid_ebx(unsigned int op);
unsigned int cpuid_ecx(unsigned int op);
unsigned int cpuid_edx(unsigned int op);

/* cpuid and cpuinfo helpers  **************************/
/* X86 ONLY ********************************************/
#else
<<<<<<< HEAD
static inline int decode_pstates(unsigned int cpu, unsigned int cpu_family,
				 int boost_states, unsigned long *pstates,
				 int *no)
=======
static inline int decode_pstates(unsigned int cpu, int boost_states,
				 unsigned long *pstates, int *no)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{ return -1; };

static inline int read_msr(int cpu, unsigned int idx, unsigned long long *val)
{ return -1; };
static inline int write_msr(int cpu, unsigned int idx, unsigned long long val)
{ return -1; };
<<<<<<< HEAD
static inline int msr_intel_set_perf_bias(unsigned int cpu, unsigned int val)
{ return -1; };
static inline int msr_intel_get_perf_bias(unsigned int cpu)
=======
static inline int cpupower_intel_set_perf_bias(unsigned int cpu, unsigned int val)
{ return -1; };
static inline int cpupower_intel_get_perf_bias(unsigned int cpu)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{ return -1; };
static inline unsigned long long msr_intel_get_turbo_ratio(unsigned int cpu)
{ return 0; };

<<<<<<< HEAD
=======
static inline int cpupower_set_epp(unsigned int cpu, char *epp)
{ return -1; };
static inline int cpupower_set_amd_pstate_mode(char *mode)
{ return -1; };
static inline int cpupower_set_turbo_boost(int turbo_boost)
{ return -1; };

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Read/Write msr ****************************/

static inline int cpufreq_has_boost_support(unsigned int cpu, int *support,
					    int *active, int * states)
{ return -1; }

<<<<<<< HEAD
=======
static inline bool cpupower_amd_pstate_enabled(void)
{ return false; }
static inline void amd_pstate_boost_init(unsigned int cpu, int *support,
					 int *active)
{}
static inline void amd_pstate_show_perf_and_freq(unsigned int cpu,
						 int no_rounding)
{}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* cpuid and cpuinfo helpers  **************************/

static inline unsigned int cpuid_eax(unsigned int op) { return 0; };
static inline unsigned int cpuid_ebx(unsigned int op) { return 0; };
static inline unsigned int cpuid_ecx(unsigned int op) { return 0; };
static inline unsigned int cpuid_edx(unsigned int op) { return 0; };
#endif /* defined(__i386__) || defined(__x86_64__) */

<<<<<<< HEAD
=======
/*
 * CPU State related functions
 */
extern struct bitmask *online_cpus;
extern struct bitmask *offline_cpus;

void get_cpustate(void);
void print_online_cpus(void);
void print_offline_cpus(void);
void print_speed(unsigned long speed, int no_rounding);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __CPUPOWERUTILS_HELPERS__ */
