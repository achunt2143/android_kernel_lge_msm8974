<<<<<<< HEAD
#include "../../../include/linux/hw_breakpoint.h"
#include "util.h"
#include "../perf.h"
#include "evlist.h"
#include "evsel.h"
#include "parse-options.h"
#include "parse-events.h"
#include "exec_cmd.h"
#include "string.h"
#include "symbol.h"
#include "cache.h"
#include "header.h"
#include "debugfs.h"
#include "parse-events-flex.h"
#include "pmu.h"

#define MAX_NAME_LEN 100

struct event_symbol {
	u8		type;
	u64		config;
	const char	*symbol;
	const char	*alias;
};

int parse_events_parse(struct list_head *list, struct list_head *list_tmp,
		       int *idx);

#define CHW(x) .type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_##x
#define CSW(x) .type = PERF_TYPE_SOFTWARE, .config = PERF_COUNT_SW_##x

static struct event_symbol event_symbols[] = {
  { CHW(CPU_CYCLES),			"cpu-cycles",			"cycles"		},
  { CHW(STALLED_CYCLES_FRONTEND),	"stalled-cycles-frontend",	"idle-cycles-frontend"	},
  { CHW(STALLED_CYCLES_BACKEND),	"stalled-cycles-backend",	"idle-cycles-backend"	},
  { CHW(INSTRUCTIONS),			"instructions",			""			},
  { CHW(CACHE_REFERENCES),		"cache-references",		""			},
  { CHW(CACHE_MISSES),			"cache-misses",			""			},
  { CHW(BRANCH_INSTRUCTIONS),		"branch-instructions",		"branches"		},
  { CHW(BRANCH_MISSES),			"branch-misses",		""			},
  { CHW(BUS_CYCLES),			"bus-cycles",			""			},
  { CHW(REF_CPU_CYCLES),		"ref-cycles",			""			},

  { CSW(CPU_CLOCK),			"cpu-clock",			""			},
  { CSW(TASK_CLOCK),			"task-clock",			""			},
  { CSW(PAGE_FAULTS),			"page-faults",			"faults"		},
  { CSW(PAGE_FAULTS_MIN),		"minor-faults",			""			},
  { CSW(PAGE_FAULTS_MAJ),		"major-faults",			""			},
  { CSW(CONTEXT_SWITCHES),		"context-switches",		"cs"			},
  { CSW(CPU_MIGRATIONS),		"cpu-migrations",		"migrations"		},
  { CSW(ALIGNMENT_FAULTS),		"alignment-faults",		""			},
  { CSW(EMULATION_FAULTS),		"emulation-faults",		""			},
};

#define __PERF_EVENT_FIELD(config, name) \
	((config & PERF_EVENT_##name##_MASK) >> PERF_EVENT_##name##_SHIFT)

#define PERF_EVENT_RAW(config)		__PERF_EVENT_FIELD(config, RAW)
#define PERF_EVENT_CONFIG(config)	__PERF_EVENT_FIELD(config, CONFIG)
#define PERF_EVENT_TYPE(config)		__PERF_EVENT_FIELD(config, TYPE)
#define PERF_EVENT_ID(config)		__PERF_EVENT_FIELD(config, EVENT)

static const char *hw_event_names[PERF_COUNT_HW_MAX] = {
	"cycles",
	"instructions",
	"cache-references",
	"cache-misses",
	"branches",
	"branch-misses",
	"bus-cycles",
	"stalled-cycles-frontend",
	"stalled-cycles-backend",
	"ref-cycles",
};

static const char *sw_event_names[PERF_COUNT_SW_MAX] = {
	"cpu-clock",
	"task-clock",
	"page-faults",
	"context-switches",
	"CPU-migrations",
	"minor-faults",
	"major-faults",
	"alignment-faults",
	"emulation-faults",
};

#define MAX_ALIASES 8

static const char *hw_cache[PERF_COUNT_HW_CACHE_MAX][MAX_ALIASES] = {
 { "L1-dcache",	"l1-d",		"l1d",		"L1-data",		},
 { "L1-icache",	"l1-i",		"l1i",		"L1-instruction",	},
 { "LLC",	"L2",							},
 { "dTLB",	"d-tlb",	"Data-TLB",				},
 { "iTLB",	"i-tlb",	"Instruction-TLB",			},
 { "branch",	"branches",	"bpu",		"btb",		"bpc",	},
 { "node",								},
};

static const char *hw_cache_op[PERF_COUNT_HW_CACHE_OP_MAX][MAX_ALIASES] = {
 { "load",	"loads",	"read",					},
 { "store",	"stores",	"write",				},
 { "prefetch",	"prefetches",	"speculative-read", "speculative-load",	},
};

static const char *hw_cache_result[PERF_COUNT_HW_CACHE_RESULT_MAX]
				  [MAX_ALIASES] = {
 { "refs",	"Reference",	"ops",		"access",		},
 { "misses",	"miss",							},
};

#define C(x)		PERF_COUNT_HW_CACHE_##x
#define CACHE_READ	(1 << C(OP_READ))
#define CACHE_WRITE	(1 << C(OP_WRITE))
#define CACHE_PREFETCH	(1 << C(OP_PREFETCH))
#define COP(x)		(1 << x)

/*
 * cache operartion stat
 * L1I : Read and prefetch only
 * ITLB and BPU : Read-only
 */
static unsigned long hw_cache_stat[C(MAX)] = {
 [C(L1D)]	= (CACHE_READ | CACHE_WRITE | CACHE_PREFETCH),
 [C(L1I)]	= (CACHE_READ | CACHE_PREFETCH),
 [C(LL)]	= (CACHE_READ | CACHE_WRITE | CACHE_PREFETCH),
 [C(DTLB)]	= (CACHE_READ | CACHE_WRITE | CACHE_PREFETCH),
 [C(ITLB)]	= (CACHE_READ),
 [C(BPU)]	= (CACHE_READ),
 [C(NODE)]	= (CACHE_READ | CACHE_WRITE | CACHE_PREFETCH),
};

#define for_each_subsystem(sys_dir, sys_dirent, sys_next)	       \
	while (!readdir_r(sys_dir, &sys_dirent, &sys_next) && sys_next)	       \
	if (sys_dirent.d_type == DT_DIR &&				       \
	   (strcmp(sys_dirent.d_name, ".")) &&				       \
	   (strcmp(sys_dirent.d_name, "..")))

static int tp_event_has_id(struct dirent *sys_dir, struct dirent *evt_dir)
{
	char evt_path[MAXPATHLEN];
	int fd;

	snprintf(evt_path, MAXPATHLEN, "%s/%s/%s/id", tracing_events_path,
			sys_dir->d_name, evt_dir->d_name);
	fd = open(evt_path, O_RDONLY);
	if (fd < 0)
		return -EINVAL;
	close(fd);

	return 0;
}

#define for_each_event(sys_dirent, evt_dir, evt_dirent, evt_next)	       \
	while (!readdir_r(evt_dir, &evt_dirent, &evt_next) && evt_next)        \
	if (evt_dirent.d_type == DT_DIR &&				       \
	   (strcmp(evt_dirent.d_name, ".")) &&				       \
	   (strcmp(evt_dirent.d_name, "..")) &&				       \
	   (!tp_event_has_id(&sys_dirent, &evt_dirent)))

#define MAX_EVENT_LENGTH 512


struct tracepoint_path *tracepoint_id_to_path(u64 config)
{
	struct tracepoint_path *path = NULL;
	DIR *sys_dir, *evt_dir;
	struct dirent *sys_next, *evt_next, sys_dirent, evt_dirent;
	char id_buf[24];
	int fd;
	u64 id;
	char evt_path[MAXPATHLEN];
	char dir_path[MAXPATHLEN];

	if (debugfs_valid_mountpoint(tracing_events_path))
		return NULL;

	sys_dir = opendir(tracing_events_path);
	if (!sys_dir)
		return NULL;

	for_each_subsystem(sys_dir, sys_dirent, sys_next) {

		snprintf(dir_path, MAXPATHLEN, "%s/%s", tracing_events_path,
			 sys_dirent.d_name);
		evt_dir = opendir(dir_path);
		if (!evt_dir)
			continue;

		for_each_event(sys_dirent, evt_dir, evt_dirent, evt_next) {

			snprintf(evt_path, MAXPATHLEN, "%s/%s/id", dir_path,
				 evt_dirent.d_name);
			fd = open(evt_path, O_RDONLY);
			if (fd < 0)
				continue;
			if (read(fd, id_buf, sizeof(id_buf)) < 0) {
				close(fd);
				continue;
			}
			close(fd);
			id = atoll(id_buf);
			if (id == config) {
				closedir(evt_dir);
				closedir(sys_dir);
				path = zalloc(sizeof(*path));
				path->system = malloc(MAX_EVENT_LENGTH);
				if (!path->system) {
					free(path);
					return NULL;
				}
				path->name = malloc(MAX_EVENT_LENGTH);
				if (!path->name) {
					free(path->system);
					free(path);
					return NULL;
				}
				strncpy(path->system, sys_dirent.d_name,
					MAX_EVENT_LENGTH);
				strncpy(path->name, evt_dirent.d_name,
					MAX_EVENT_LENGTH);
				return path;
			}
		}
		closedir(evt_dir);
	}

	closedir(sys_dir);
	return NULL;
}

#define TP_PATH_LEN (MAX_EVENT_LENGTH * 2 + 1)
static const char *tracepoint_id_to_name(u64 config)
{
	static char buf[TP_PATH_LEN];
	struct tracepoint_path *path;

	path = tracepoint_id_to_path(config);
	if (path) {
		snprintf(buf, TP_PATH_LEN, "%s:%s", path->system, path->name);
		free(path->name);
		free(path->system);
		free(path);
	} else
		snprintf(buf, TP_PATH_LEN, "%s:%s", "unknown", "unknown");

	return buf;
}

static int is_cache_op_valid(u8 cache_type, u8 cache_op)
{
	if (hw_cache_stat[cache_type] & COP(cache_op))
		return 1;	/* valid */
	else
		return 0;	/* invalid */
}

static char *event_cache_name(u8 cache_type, u8 cache_op, u8 cache_result)
{
	static char name[50];

	if (cache_result) {
		sprintf(name, "%s-%s-%s", hw_cache[cache_type][0],
			hw_cache_op[cache_op][0],
			hw_cache_result[cache_result][0]);
	} else {
		sprintf(name, "%s-%s", hw_cache[cache_type][0],
			hw_cache_op[cache_op][1]);
	}

	return name;
}

=======
// SPDX-License-Identifier: GPL-2.0
#include <linux/hw_breakpoint.h>
#include <linux/err.h>
#include <linux/list_sort.h>
#include <linux/zalloc.h>
#include <dirent.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/param.h>
#include "term.h"
#include "evlist.h"
#include "evsel.h"
#include <subcmd/parse-options.h>
#include "parse-events.h"
#include "string2.h"
#include "strbuf.h"
#include "debug.h"
#include <api/fs/tracing_path.h>
#include <perf/cpumap.h>
#include <util/parse-events-bison.h>
#include <util/parse-events-flex.h>
#include "pmu.h"
#include "pmus.h"
#include "asm/bug.h"
#include "util/parse-branch-options.h"
#include "util/evsel_config.h"
#include "util/event.h"
#include "util/bpf-filter.h"
#include "util/util.h"
#include "tracepoint.h"

#define MAX_NAME_LEN 100

#ifdef PARSER_DEBUG
extern int parse_events_debug;
#endif
static int get_config_terms(struct parse_events_terms *head_config, struct list_head *head_terms);
static int parse_events_terms__copy(const struct parse_events_terms *src,
				    struct parse_events_terms *dest);

struct event_symbol event_symbols_hw[PERF_COUNT_HW_MAX] = {
	[PERF_COUNT_HW_CPU_CYCLES] = {
		.symbol = "cpu-cycles",
		.alias  = "cycles",
	},
	[PERF_COUNT_HW_INSTRUCTIONS] = {
		.symbol = "instructions",
		.alias  = "",
	},
	[PERF_COUNT_HW_CACHE_REFERENCES] = {
		.symbol = "cache-references",
		.alias  = "",
	},
	[PERF_COUNT_HW_CACHE_MISSES] = {
		.symbol = "cache-misses",
		.alias  = "",
	},
	[PERF_COUNT_HW_BRANCH_INSTRUCTIONS] = {
		.symbol = "branch-instructions",
		.alias  = "branches",
	},
	[PERF_COUNT_HW_BRANCH_MISSES] = {
		.symbol = "branch-misses",
		.alias  = "",
	},
	[PERF_COUNT_HW_BUS_CYCLES] = {
		.symbol = "bus-cycles",
		.alias  = "",
	},
	[PERF_COUNT_HW_STALLED_CYCLES_FRONTEND] = {
		.symbol = "stalled-cycles-frontend",
		.alias  = "idle-cycles-frontend",
	},
	[PERF_COUNT_HW_STALLED_CYCLES_BACKEND] = {
		.symbol = "stalled-cycles-backend",
		.alias  = "idle-cycles-backend",
	},
	[PERF_COUNT_HW_REF_CPU_CYCLES] = {
		.symbol = "ref-cycles",
		.alias  = "",
	},
};

struct event_symbol event_symbols_sw[PERF_COUNT_SW_MAX] = {
	[PERF_COUNT_SW_CPU_CLOCK] = {
		.symbol = "cpu-clock",
		.alias  = "",
	},
	[PERF_COUNT_SW_TASK_CLOCK] = {
		.symbol = "task-clock",
		.alias  = "",
	},
	[PERF_COUNT_SW_PAGE_FAULTS] = {
		.symbol = "page-faults",
		.alias  = "faults",
	},
	[PERF_COUNT_SW_CONTEXT_SWITCHES] = {
		.symbol = "context-switches",
		.alias  = "cs",
	},
	[PERF_COUNT_SW_CPU_MIGRATIONS] = {
		.symbol = "cpu-migrations",
		.alias  = "migrations",
	},
	[PERF_COUNT_SW_PAGE_FAULTS_MIN] = {
		.symbol = "minor-faults",
		.alias  = "",
	},
	[PERF_COUNT_SW_PAGE_FAULTS_MAJ] = {
		.symbol = "major-faults",
		.alias  = "",
	},
	[PERF_COUNT_SW_ALIGNMENT_FAULTS] = {
		.symbol = "alignment-faults",
		.alias  = "",
	},
	[PERF_COUNT_SW_EMULATION_FAULTS] = {
		.symbol = "emulation-faults",
		.alias  = "",
	},
	[PERF_COUNT_SW_DUMMY] = {
		.symbol = "dummy",
		.alias  = "",
	},
	[PERF_COUNT_SW_BPF_OUTPUT] = {
		.symbol = "bpf-output",
		.alias  = "",
	},
	[PERF_COUNT_SW_CGROUP_SWITCHES] = {
		.symbol = "cgroup-switches",
		.alias  = "",
	},
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
const char *event_type(int type)
{
	switch (type) {
	case PERF_TYPE_HARDWARE:
		return "hardware";

	case PERF_TYPE_SOFTWARE:
		return "software";

	case PERF_TYPE_TRACEPOINT:
		return "tracepoint";

	case PERF_TYPE_HW_CACHE:
		return "hardware-cache";

	default:
		break;
	}

	return "unknown";
}

<<<<<<< HEAD
const char *event_name(struct perf_evsel *evsel)
{
	u64 config = evsel->attr.config;
	int type = evsel->attr.type;
	char *buf;
	size_t buf_sz;

	if (evsel->name) {
		/* Make new space for the modifier bits. */
		buf_sz = strlen(evsel->name) + 3;
		buf = malloc(buf_sz);
		if (!buf)
			/*
			 * Always return what was already in 'name'.
			 */
			return evsel->name;

		strlcpy(buf, evsel->name, buf_sz);

		free(evsel->name);

		evsel->name = buf;

		/* User mode profiling. */
		if (!evsel->attr.exclude_user && evsel->attr.exclude_kernel)
			strlcpy(&evsel->name[strlen(evsel->name)], ":u",
					buf_sz);
		/* Kernel mode profiling. */
		else if (!evsel->attr.exclude_kernel &&
				evsel->attr.exclude_user)
			strlcpy(&evsel->name[strlen(evsel->name)], ":k",
					buf_sz);

		return evsel->name;
	}

	return __event_name(type, config, NULL);
}

const char *__event_name(int type, u64 config, char *pmu_name)
{
	static char buf[32];

	if (!pmu_name && type == PERF_TYPE_RAW) {
		sprintf(buf, "raw 0x%" PRIx64, config);
		return buf;
	}

	switch (type) {
	case PERF_TYPE_HARDWARE:
		if (config < PERF_COUNT_HW_MAX && hw_event_names[config])
			return hw_event_names[config];
		return "unknown-hardware";

	case PERF_TYPE_HW_CACHE: {
		u8 cache_type, cache_op, cache_result;

		cache_type   = (config >>  0) & 0xff;
		if (cache_type > PERF_COUNT_HW_CACHE_MAX)
			return "unknown-ext-hardware-cache-type";

		cache_op     = (config >>  8) & 0xff;
		if (cache_op > PERF_COUNT_HW_CACHE_OP_MAX)
			return "unknown-ext-hardware-cache-op";

		cache_result = (config >> 16) & 0xff;
		if (cache_result > PERF_COUNT_HW_CACHE_RESULT_MAX)
			return "unknown-ext-hardware-cache-result";

		if (!is_cache_op_valid(cache_type, cache_op))
			return "invalid-cache";

		return event_cache_name(cache_type, cache_op, cache_result);
	}

	case PERF_TYPE_SOFTWARE:
		if (config < PERF_COUNT_SW_MAX && sw_event_names[config])
			return sw_event_names[config];
		return "unknown-software";

	case PERF_TYPE_TRACEPOINT:
		return tracepoint_id_to_name(config);

	default:
		if (pmu_name) {
			snprintf(buf, sizeof(buf), "%s 0x%" PRIx64, pmu_name,
					config);
			return buf;
		} else
			break;
	}

	return "unknown";
}

static int add_event(struct list_head *list, int *idx,
		     struct perf_event_attr *attr, char *name)
{
	struct perf_evsel *evsel;

	event_attr_init(attr);

	evsel = perf_evsel__new(attr, (*idx)++);
	if (!evsel)
		return -ENOMEM;

	list_add_tail(&evsel->node, list);

	evsel->name = strdup(name);
	return 0;
}

static int parse_aliases(char *str, const char *names[][MAX_ALIASES], int size)
{
	int i, j;
	int n, longest = -1;

	for (i = 0; i < size; i++) {
		for (j = 0; j < MAX_ALIASES && names[i][j]; j++) {
			n = strlen(names[i][j]);
			if (n > longest && !strncasecmp(str, names[i][j], n))
				longest = n;
		}
		if (longest > 0)
=======
static char *get_config_str(struct parse_events_terms *head_terms,
			    enum parse_events__term_type type_term)
{
	struct parse_events_term *term;

	if (!head_terms)
		return NULL;

	list_for_each_entry(term, &head_terms->terms, list)
		if (term->type_term == type_term)
			return term->val.str;

	return NULL;
}

static char *get_config_metric_id(struct parse_events_terms *head_terms)
{
	return get_config_str(head_terms, PARSE_EVENTS__TERM_TYPE_METRIC_ID);
}

static char *get_config_name(struct parse_events_terms *head_terms)
{
	return get_config_str(head_terms, PARSE_EVENTS__TERM_TYPE_NAME);
}

/**
 * fix_raw - For each raw term see if there is an event (aka alias) in pmu that
 *           matches the raw's string value. If the string value matches an
 *           event then change the term to be an event, if not then change it to
 *           be a config term. For example, "read" may be an event of the PMU or
 *           a raw hex encoding of 0xead. The fix-up is done late so the PMU of
 *           the event can be determined and we don't need to scan all PMUs
 *           ahead-of-time.
 * @config_terms: the list of terms that may contain a raw term.
 * @pmu: the PMU to scan for events from.
 */
static void fix_raw(struct parse_events_terms *config_terms, struct perf_pmu *pmu)
{
	struct parse_events_term *term;

	list_for_each_entry(term, &config_terms->terms, list) {
		u64 num;

		if (term->type_term != PARSE_EVENTS__TERM_TYPE_RAW)
			continue;

		if (perf_pmu__have_event(pmu, term->val.str)) {
			zfree(&term->config);
			term->config = term->val.str;
			term->type_val = PARSE_EVENTS__TERM_TYPE_NUM;
			term->type_term = PARSE_EVENTS__TERM_TYPE_USER;
			term->val.num = 1;
			term->no_value = true;
			continue;
		}

		zfree(&term->config);
		term->config = strdup("config");
		errno = 0;
		num = strtoull(term->val.str + 1, NULL, 16);
		assert(errno == 0);
		free(term->val.str);
		term->type_val = PARSE_EVENTS__TERM_TYPE_NUM;
		term->type_term = PARSE_EVENTS__TERM_TYPE_CONFIG;
		term->val.num = num;
		term->no_value = false;
	}
}

static struct evsel *
__add_event(struct list_head *list, int *idx,
	    struct perf_event_attr *attr,
	    bool init_attr,
	    const char *name, const char *metric_id, struct perf_pmu *pmu,
	    struct list_head *config_terms, bool auto_merge_stats,
	    const char *cpu_list)
{
	struct evsel *evsel;
	struct perf_cpu_map *cpus = pmu ? perf_cpu_map__get(pmu->cpus) :
			       cpu_list ? perf_cpu_map__new(cpu_list) : NULL;

	if (pmu)
		perf_pmu__warn_invalid_formats(pmu);

	if (pmu && (attr->type == PERF_TYPE_RAW || attr->type >= PERF_TYPE_MAX)) {
		perf_pmu__warn_invalid_config(pmu, attr->config, name,
					      PERF_PMU_FORMAT_VALUE_CONFIG, "config");
		perf_pmu__warn_invalid_config(pmu, attr->config1, name,
					      PERF_PMU_FORMAT_VALUE_CONFIG1, "config1");
		perf_pmu__warn_invalid_config(pmu, attr->config2, name,
					      PERF_PMU_FORMAT_VALUE_CONFIG2, "config2");
		perf_pmu__warn_invalid_config(pmu, attr->config3, name,
					      PERF_PMU_FORMAT_VALUE_CONFIG3, "config3");
	}
	if (init_attr)
		event_attr_init(attr);

	evsel = evsel__new_idx(attr, *idx);
	if (!evsel) {
		perf_cpu_map__put(cpus);
		return NULL;
	}

	(*idx)++;
	evsel->core.cpus = cpus;
	evsel->core.own_cpus = perf_cpu_map__get(cpus);
	evsel->core.requires_cpu = pmu ? pmu->is_uncore : false;
	evsel->core.is_pmu_core = pmu ? pmu->is_core : false;
	evsel->auto_merge_stats = auto_merge_stats;
	evsel->pmu = pmu;
	evsel->pmu_name = pmu ? strdup(pmu->name) : NULL;

	if (name)
		evsel->name = strdup(name);

	if (metric_id)
		evsel->metric_id = strdup(metric_id);

	if (config_terms)
		list_splice_init(config_terms, &evsel->config_terms);

	if (list)
		list_add_tail(&evsel->core.node, list);

	return evsel;
}

struct evsel *parse_events__add_event(int idx, struct perf_event_attr *attr,
				      const char *name, const char *metric_id,
				      struct perf_pmu *pmu)
{
	return __add_event(/*list=*/NULL, &idx, attr, /*init_attr=*/false, name,
			   metric_id, pmu, /*config_terms=*/NULL,
			   /*auto_merge_stats=*/false, /*cpu_list=*/NULL);
}

static int add_event(struct list_head *list, int *idx,
		     struct perf_event_attr *attr, const char *name,
		     const char *metric_id, struct list_head *config_terms)
{
	return __add_event(list, idx, attr, /*init_attr*/true, name, metric_id,
			   /*pmu=*/NULL, config_terms,
			   /*auto_merge_stats=*/false, /*cpu_list=*/NULL) ? 0 : -ENOMEM;
}

static int add_event_tool(struct list_head *list, int *idx,
			  enum perf_tool_event tool_event)
{
	struct evsel *evsel;
	struct perf_event_attr attr = {
		.type = PERF_TYPE_SOFTWARE,
		.config = PERF_COUNT_SW_DUMMY,
	};

	evsel = __add_event(list, idx, &attr, /*init_attr=*/true, /*name=*/NULL,
			    /*metric_id=*/NULL, /*pmu=*/NULL,
			    /*config_terms=*/NULL, /*auto_merge_stats=*/false,
			    /*cpu_list=*/"0");
	if (!evsel)
		return -ENOMEM;
	evsel->tool_event = tool_event;
	if (tool_event == PERF_TOOL_DURATION_TIME
	    || tool_event == PERF_TOOL_USER_TIME
	    || tool_event == PERF_TOOL_SYSTEM_TIME) {
		free((char *)evsel->unit);
		evsel->unit = strdup("ns");
	}
	return 0;
}

/**
 * parse_aliases - search names for entries beginning or equalling str ignoring
 *                 case. If mutliple entries in names match str then the longest
 *                 is chosen.
 * @str: The needle to look for.
 * @names: The haystack to search.
 * @size: The size of the haystack.
 * @longest: Out argument giving the length of the matching entry.
 */
static int parse_aliases(const char *str, const char *const names[][EVSEL__MAX_ALIASES], int size,
			 int *longest)
{
	*longest = -1;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < EVSEL__MAX_ALIASES && names[i][j]; j++) {
			int n = strlen(names[i][j]);

			if (n > *longest && !strncasecmp(str, names[i][j], n))
				*longest = n;
		}
		if (*longest > 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return i;
	}

	return -1;
}

<<<<<<< HEAD
int parse_events_add_cache(struct list_head *list, int *idx,
			   char *type, char *op_result1, char *op_result2)
{
	struct perf_event_attr attr;
	char name[MAX_NAME_LEN];
	int cache_type = -1, cache_op = -1, cache_result = -1;
	char *op_result[2] = { op_result1, op_result2 };
	int i, n;

	/*
	 * No fallback - if we cannot get a clear cache type
	 * then bail out:
	 */
	cache_type = parse_aliases(type, hw_cache,
				   PERF_COUNT_HW_CACHE_MAX);
	if (cache_type == -1)
		return -EINVAL;

	n = snprintf(name, MAX_NAME_LEN, "%s", type);

	for (i = 0; (i < 2) && (op_result[i]); i++) {
		char *str = op_result[i];

		n += snprintf(name + n, MAX_NAME_LEN - n, "-%s", str);

		if (cache_op == -1) {
			cache_op = parse_aliases(str, hw_cache_op,
						 PERF_COUNT_HW_CACHE_OP_MAX);
			if (cache_op >= 0) {
				if (!is_cache_op_valid(cache_type, cache_op))
					return -EINVAL;
				continue;
			}
		}

		if (cache_result == -1) {
			cache_result = parse_aliases(str, hw_cache_result,
						PERF_COUNT_HW_CACHE_RESULT_MAX);
			if (cache_result >= 0)
				continue;
=======
typedef int config_term_func_t(struct perf_event_attr *attr,
			       struct parse_events_term *term,
			       struct parse_events_error *err);
static int config_term_common(struct perf_event_attr *attr,
			      struct parse_events_term *term,
			      struct parse_events_error *err);
static int config_attr(struct perf_event_attr *attr,
		       struct parse_events_terms *head,
		       struct parse_events_error *err,
		       config_term_func_t config_term);

/**
 * parse_events__decode_legacy_cache - Search name for the legacy cache event
 *                                     name composed of 1, 2 or 3 hyphen
 *                                     separated sections. The first section is
 *                                     the cache type while the others are the
 *                                     optional op and optional result. To make
 *                                     life hard the names in the table also
 *                                     contain hyphens and the longest name
 *                                     should always be selected.
 */
int parse_events__decode_legacy_cache(const char *name, int extended_pmu_type, __u64 *config)
{
	int len, cache_type = -1, cache_op = -1, cache_result = -1;
	const char *name_end = &name[strlen(name) + 1];
	const char *str = name;

	cache_type = parse_aliases(str, evsel__hw_cache, PERF_COUNT_HW_CACHE_MAX, &len);
	if (cache_type == -1)
		return -EINVAL;
	str += len + 1;

	if (str < name_end) {
		cache_op = parse_aliases(str, evsel__hw_cache_op,
					PERF_COUNT_HW_CACHE_OP_MAX, &len);
		if (cache_op >= 0) {
			if (!evsel__is_cache_op_valid(cache_type, cache_op))
				return -EINVAL;
			str += len + 1;
		} else {
			cache_result = parse_aliases(str, evsel__hw_cache_result,
						PERF_COUNT_HW_CACHE_RESULT_MAX, &len);
			if (cache_result >= 0)
				str += len + 1;
		}
	}
	if (str < name_end) {
		if (cache_op < 0) {
			cache_op = parse_aliases(str, evsel__hw_cache_op,
						PERF_COUNT_HW_CACHE_OP_MAX, &len);
			if (cache_op >= 0) {
				if (!evsel__is_cache_op_valid(cache_type, cache_op))
					return -EINVAL;
			}
		} else if (cache_result < 0) {
			cache_result = parse_aliases(str, evsel__hw_cache_result,
						PERF_COUNT_HW_CACHE_RESULT_MAX, &len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	/*
	 * Fall back to reads:
	 */
	if (cache_op == -1)
		cache_op = PERF_COUNT_HW_CACHE_OP_READ;

	/*
	 * Fall back to accesses:
	 */
	if (cache_result == -1)
		cache_result = PERF_COUNT_HW_CACHE_RESULT_ACCESS;

<<<<<<< HEAD
	memset(&attr, 0, sizeof(attr));
	attr.config = cache_type | (cache_op << 8) | (cache_result << 16);
	attr.type = PERF_TYPE_HW_CACHE;
	return add_event(list, idx, &attr, name);
}

static int add_tracepoint(struct list_head *list, int *idx,
			  char *sys_name, char *evt_name)
{
	struct perf_event_attr attr;
	char name[MAX_NAME_LEN];
	char evt_path[MAXPATHLEN];
	char id_buf[4];
	u64 id;
	int fd;

	snprintf(evt_path, MAXPATHLEN, "%s/%s/%s/id", tracing_events_path,
		 sys_name, evt_name);

	fd = open(evt_path, O_RDONLY);
	if (fd < 0)
		return -1;

	if (read(fd, id_buf, sizeof(id_buf)) < 0) {
		close(fd);
		return -1;
	}

	close(fd);
	id = atoll(id_buf);

	memset(&attr, 0, sizeof(attr));
	attr.config = id;
	attr.type = PERF_TYPE_TRACEPOINT;
	attr.sample_type |= PERF_SAMPLE_RAW;
	attr.sample_type |= PERF_SAMPLE_TIME;
	attr.sample_type |= PERF_SAMPLE_CPU;
	attr.sample_period = 1;

	snprintf(name, MAX_NAME_LEN, "%s:%s", sys_name, evt_name);
	return add_event(list, idx, &attr, name);
}

static int add_tracepoint_multi(struct list_head *list, int *idx,
				char *sys_name, char *evt_name)
{
	char evt_path[MAXPATHLEN];
	struct dirent *evt_ent;
	DIR *evt_dir;
	int ret = 0;

	snprintf(evt_path, MAXPATHLEN, "%s/%s", tracing_events_path, sys_name);
	evt_dir = opendir(evt_path);
	if (!evt_dir) {
		perror("Can't open event dir");
=======
	*config = cache_type | (cache_op << 8) | (cache_result << 16);
	if (perf_pmus__supports_extended_type())
		*config |= (__u64)extended_pmu_type << PERF_PMU_TYPE_SHIFT;
	return 0;
}

/**
 * parse_events__filter_pmu - returns false if a wildcard PMU should be
 *                            considered, true if it should be filtered.
 */
bool parse_events__filter_pmu(const struct parse_events_state *parse_state,
			      const struct perf_pmu *pmu)
{
	if (parse_state->pmu_filter == NULL)
		return false;

	return strcmp(parse_state->pmu_filter, pmu->name) != 0;
}

int parse_events_add_cache(struct list_head *list, int *idx, const char *name,
			   struct parse_events_state *parse_state,
			   struct parse_events_terms *head_config)
{
	struct perf_pmu *pmu = NULL;
	bool found_supported = false;
	const char *config_name = get_config_name(head_config);
	const char *metric_id = get_config_metric_id(head_config);

	/* Legacy cache events are only supported by core PMUs. */
	while ((pmu = perf_pmus__scan_core(pmu)) != NULL) {
		LIST_HEAD(config_terms);
		struct perf_event_attr attr;
		int ret;

		if (parse_events__filter_pmu(parse_state, pmu))
			continue;

		memset(&attr, 0, sizeof(attr));
		attr.type = PERF_TYPE_HW_CACHE;

		ret = parse_events__decode_legacy_cache(name, pmu->type, &attr.config);
		if (ret)
			return ret;

		found_supported = true;

		if (head_config) {
			if (config_attr(&attr, head_config, parse_state->error, config_term_common))
				return -EINVAL;

			if (get_config_terms(head_config, &config_terms))
				return -ENOMEM;
		}

		if (__add_event(list, idx, &attr, /*init_attr*/true, config_name ?: name,
				metric_id, pmu, &config_terms, /*auto_merge_stats=*/false,
				/*cpu_list=*/NULL) == NULL)
			return -ENOMEM;

		free_config_terms(&config_terms);
	}
	return found_supported ? 0 : -EINVAL;
}

#ifdef HAVE_LIBTRACEEVENT
static void tracepoint_error(struct parse_events_error *e, int err,
			     const char *sys, const char *name, int column)
{
	const char *str;
	char help[BUFSIZ];

	if (!e)
		return;

	/*
	 * We get error directly from syscall errno ( > 0),
	 * or from encoded pointer's error ( < 0).
	 */
	err = abs(err);

	switch (err) {
	case EACCES:
		str = "can't access trace events";
		break;
	case ENOENT:
		str = "unknown tracepoint";
		break;
	default:
		str = "failed to add tracepoint";
		break;
	}

	tracing_path__strerror_open_tp(err, help, sizeof(help), sys, name);
	parse_events_error__handle(e, column, strdup(str), strdup(help));
}

static int add_tracepoint(struct list_head *list, int *idx,
			  const char *sys_name, const char *evt_name,
			  struct parse_events_error *err,
			  struct parse_events_terms *head_config, void *loc_)
{
	YYLTYPE *loc = loc_;
	struct evsel *evsel = evsel__newtp_idx(sys_name, evt_name, (*idx)++);

	if (IS_ERR(evsel)) {
		tracepoint_error(err, PTR_ERR(evsel), sys_name, evt_name, loc->first_column);
		return PTR_ERR(evsel);
	}

	if (head_config) {
		LIST_HEAD(config_terms);

		if (get_config_terms(head_config, &config_terms))
			return -ENOMEM;
		list_splice(&config_terms, &evsel->config_terms);
	}

	list_add_tail(&evsel->core.node, list);
	return 0;
}

static int add_tracepoint_multi_event(struct list_head *list, int *idx,
				      const char *sys_name, const char *evt_name,
				      struct parse_events_error *err,
				      struct parse_events_terms *head_config, YYLTYPE *loc)
{
	char *evt_path;
	struct dirent *evt_ent;
	DIR *evt_dir;
	int ret = 0, found = 0;

	evt_path = get_events_file(sys_name);
	if (!evt_path) {
		tracepoint_error(err, errno, sys_name, evt_name, loc->first_column);
		return -1;
	}
	evt_dir = opendir(evt_path);
	if (!evt_dir) {
		put_events_file(evt_path);
		tracepoint_error(err, errno, sys_name, evt_name, loc->first_column);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -1;
	}

	while (!ret && (evt_ent = readdir(evt_dir))) {
		if (!strcmp(evt_ent->d_name, ".")
		    || !strcmp(evt_ent->d_name, "..")
		    || !strcmp(evt_ent->d_name, "enable")
		    || !strcmp(evt_ent->d_name, "filter"))
			continue;

		if (!strglobmatch(evt_ent->d_name, evt_name))
			continue;

<<<<<<< HEAD
		ret = add_tracepoint(list, idx, sys_name, evt_ent->d_name);
	}

	return ret;
}

int parse_events_add_tracepoint(struct list_head *list, int *idx,
				char *sys, char *event)
{
	int ret;

	ret = debugfs_valid_mountpoint(tracing_events_path);
	if (ret)
		return ret;

	return strpbrk(event, "*?") ?
	       add_tracepoint_multi(list, idx, sys, event) :
	       add_tracepoint(list, idx, sys, event);
}

=======
		found++;

		ret = add_tracepoint(list, idx, sys_name, evt_ent->d_name,
				     err, head_config, loc);
	}

	if (!found) {
		tracepoint_error(err, ENOENT, sys_name, evt_name, loc->first_column);
		ret = -1;
	}

	put_events_file(evt_path);
	closedir(evt_dir);
	return ret;
}

static int add_tracepoint_event(struct list_head *list, int *idx,
				const char *sys_name, const char *evt_name,
				struct parse_events_error *err,
				struct parse_events_terms *head_config, YYLTYPE *loc)
{
	return strpbrk(evt_name, "*?") ?
		add_tracepoint_multi_event(list, idx, sys_name, evt_name,
					   err, head_config, loc) :
		add_tracepoint(list, idx, sys_name, evt_name,
			       err, head_config, loc);
}

static int add_tracepoint_multi_sys(struct list_head *list, int *idx,
				    const char *sys_name, const char *evt_name,
				    struct parse_events_error *err,
				    struct parse_events_terms *head_config, YYLTYPE *loc)
{
	struct dirent *events_ent;
	DIR *events_dir;
	int ret = 0;

	events_dir = tracing_events__opendir();
	if (!events_dir) {
		tracepoint_error(err, errno, sys_name, evt_name, loc->first_column);
		return -1;
	}

	while (!ret && (events_ent = readdir(events_dir))) {
		if (!strcmp(events_ent->d_name, ".")
		    || !strcmp(events_ent->d_name, "..")
		    || !strcmp(events_ent->d_name, "enable")
		    || !strcmp(events_ent->d_name, "header_event")
		    || !strcmp(events_ent->d_name, "header_page"))
			continue;

		if (!strglobmatch(events_ent->d_name, sys_name))
			continue;

		ret = add_tracepoint_event(list, idx, events_ent->d_name,
					   evt_name, err, head_config, loc);
	}

	closedir(events_dir);
	return ret;
}
#endif /* HAVE_LIBTRACEEVENT */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int
parse_breakpoint_type(const char *type, struct perf_event_attr *attr)
{
	int i;

	for (i = 0; i < 3; i++) {
		if (!type || !type[i])
			break;

<<<<<<< HEAD
		switch (type[i]) {
		case 'r':
			attr->bp_type |= HW_BREAKPOINT_R;
			break;
		case 'w':
			attr->bp_type |= HW_BREAKPOINT_W;
			break;
		case 'x':
			attr->bp_type |= HW_BREAKPOINT_X;
=======
#define CHECK_SET_TYPE(bit)		\
do {					\
	if (attr->bp_type & bit)	\
		return -EINVAL;		\
	else				\
		attr->bp_type |= bit;	\
} while (0)

		switch (type[i]) {
		case 'r':
			CHECK_SET_TYPE(HW_BREAKPOINT_R);
			break;
		case 'w':
			CHECK_SET_TYPE(HW_BREAKPOINT_W);
			break;
		case 'x':
			CHECK_SET_TYPE(HW_BREAKPOINT_X);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		default:
			return -EINVAL;
		}
	}

<<<<<<< HEAD
=======
#undef CHECK_SET_TYPE

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!attr->bp_type) /* Default */
		attr->bp_type = HW_BREAKPOINT_R | HW_BREAKPOINT_W;

	return 0;
}

<<<<<<< HEAD
int parse_events_add_breakpoint(struct list_head *list, int *idx,
				void *ptr, char *type)
{
	struct perf_event_attr attr;
	char name[MAX_NAME_LEN];

	memset(&attr, 0, sizeof(attr));
	attr.bp_addr = (unsigned long) ptr;
=======
int parse_events_add_breakpoint(struct parse_events_state *parse_state,
				struct list_head *list,
				u64 addr, char *type, u64 len,
				struct parse_events_terms *head_config)
{
	struct perf_event_attr attr;
	LIST_HEAD(config_terms);
	const char *name;

	memset(&attr, 0, sizeof(attr));
	attr.bp_addr = addr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (parse_breakpoint_type(type, &attr))
		return -EINVAL;

<<<<<<< HEAD
	/*
	 * We should find a nice way to override the access length
	 * Provide some defaults for now
	 */
	if (attr.bp_type == HW_BREAKPOINT_X)
		attr.bp_len = sizeof(long);
	else
		attr.bp_len = HW_BREAKPOINT_LEN_4;

	attr.type = PERF_TYPE_BREAKPOINT;

	snprintf(name, MAX_NAME_LEN, "mem:%p:%s", ptr, type ? type : "rw");
	return add_event(list, idx, &attr, name);
}

static int config_term(struct perf_event_attr *attr,
		       struct parse_events__term *term)
{
	switch (term->type) {
	case PARSE_EVENTS__TERM_TYPE_CONFIG:
		attr->config = term->val.num;
		break;
	case PARSE_EVENTS__TERM_TYPE_CONFIG1:
		attr->config1 = term->val.num;
		break;
	case PARSE_EVENTS__TERM_TYPE_CONFIG2:
		attr->config2 = term->val.num;
		break;
	case PARSE_EVENTS__TERM_TYPE_SAMPLE_PERIOD:
		attr->sample_period = term->val.num;
		break;
	case PARSE_EVENTS__TERM_TYPE_BRANCH_SAMPLE_TYPE:
		/*
		 * TODO uncomment when the field is available
		 * attr->branch_sample_type = term->val.num;
		 */
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static int config_attr(struct perf_event_attr *attr,
		       struct list_head *head, int fail)
{
	struct parse_events__term *term;

	list_for_each_entry(term, head, list)
		if (config_term(attr, term) && fail)
=======
	/* Provide some defaults if len is not specified */
	if (!len) {
		if (attr.bp_type == HW_BREAKPOINT_X)
			len = sizeof(long);
		else
			len = HW_BREAKPOINT_LEN_4;
	}

	attr.bp_len = len;

	attr.type = PERF_TYPE_BREAKPOINT;
	attr.sample_period = 1;

	if (head_config) {
		if (config_attr(&attr, head_config, parse_state->error,
				config_term_common))
			return -EINVAL;

		if (get_config_terms(head_config, &config_terms))
			return -ENOMEM;
	}

	name = get_config_name(head_config);

	return add_event(list, &parse_state->idx, &attr, name, /*mertic_id=*/NULL,
			 &config_terms);
}

static int check_type_val(struct parse_events_term *term,
			  struct parse_events_error *err,
			  enum parse_events__term_val_type type)
{
	if (type == term->type_val)
		return 0;

	if (err) {
		parse_events_error__handle(err, term->err_val,
					type == PARSE_EVENTS__TERM_TYPE_NUM
					? strdup("expected numeric value")
					: strdup("expected string value"),
					NULL);
	}
	return -EINVAL;
}

static bool config_term_shrinked;

static const char *config_term_name(enum parse_events__term_type term_type)
{
	/*
	 * Update according to parse-events.l
	 */
	static const char *config_term_names[__PARSE_EVENTS__TERM_TYPE_NR] = {
		[PARSE_EVENTS__TERM_TYPE_USER]			= "<sysfs term>",
		[PARSE_EVENTS__TERM_TYPE_CONFIG]		= "config",
		[PARSE_EVENTS__TERM_TYPE_CONFIG1]		= "config1",
		[PARSE_EVENTS__TERM_TYPE_CONFIG2]		= "config2",
		[PARSE_EVENTS__TERM_TYPE_CONFIG3]		= "config3",
		[PARSE_EVENTS__TERM_TYPE_NAME]			= "name",
		[PARSE_EVENTS__TERM_TYPE_SAMPLE_PERIOD]		= "period",
		[PARSE_EVENTS__TERM_TYPE_SAMPLE_FREQ]		= "freq",
		[PARSE_EVENTS__TERM_TYPE_BRANCH_SAMPLE_TYPE]	= "branch_type",
		[PARSE_EVENTS__TERM_TYPE_TIME]			= "time",
		[PARSE_EVENTS__TERM_TYPE_CALLGRAPH]		= "call-graph",
		[PARSE_EVENTS__TERM_TYPE_STACKSIZE]		= "stack-size",
		[PARSE_EVENTS__TERM_TYPE_NOINHERIT]		= "no-inherit",
		[PARSE_EVENTS__TERM_TYPE_INHERIT]		= "inherit",
		[PARSE_EVENTS__TERM_TYPE_MAX_STACK]		= "max-stack",
		[PARSE_EVENTS__TERM_TYPE_MAX_EVENTS]		= "nr",
		[PARSE_EVENTS__TERM_TYPE_OVERWRITE]		= "overwrite",
		[PARSE_EVENTS__TERM_TYPE_NOOVERWRITE]		= "no-overwrite",
		[PARSE_EVENTS__TERM_TYPE_DRV_CFG]		= "driver-config",
		[PARSE_EVENTS__TERM_TYPE_PERCORE]		= "percore",
		[PARSE_EVENTS__TERM_TYPE_AUX_OUTPUT]		= "aux-output",
		[PARSE_EVENTS__TERM_TYPE_AUX_SAMPLE_SIZE]	= "aux-sample-size",
		[PARSE_EVENTS__TERM_TYPE_METRIC_ID]		= "metric-id",
		[PARSE_EVENTS__TERM_TYPE_RAW]                   = "raw",
		[PARSE_EVENTS__TERM_TYPE_LEGACY_CACHE]          = "legacy-cache",
		[PARSE_EVENTS__TERM_TYPE_HARDWARE]              = "hardware",
	};
	if ((unsigned int)term_type >= __PARSE_EVENTS__TERM_TYPE_NR)
		return "unknown term";

	return config_term_names[term_type];
}

static bool
config_term_avail(enum parse_events__term_type term_type, struct parse_events_error *err)
{
	char *err_str;

	if (term_type < 0 || term_type >= __PARSE_EVENTS__TERM_TYPE_NR) {
		parse_events_error__handle(err, -1,
					strdup("Invalid term_type"), NULL);
		return false;
	}
	if (!config_term_shrinked)
		return true;

	switch (term_type) {
	case PARSE_EVENTS__TERM_TYPE_CONFIG:
	case PARSE_EVENTS__TERM_TYPE_CONFIG1:
	case PARSE_EVENTS__TERM_TYPE_CONFIG2:
	case PARSE_EVENTS__TERM_TYPE_CONFIG3:
	case PARSE_EVENTS__TERM_TYPE_NAME:
	case PARSE_EVENTS__TERM_TYPE_METRIC_ID:
	case PARSE_EVENTS__TERM_TYPE_SAMPLE_PERIOD:
	case PARSE_EVENTS__TERM_TYPE_PERCORE:
		return true;
	case PARSE_EVENTS__TERM_TYPE_USER:
	case PARSE_EVENTS__TERM_TYPE_SAMPLE_FREQ:
	case PARSE_EVENTS__TERM_TYPE_BRANCH_SAMPLE_TYPE:
	case PARSE_EVENTS__TERM_TYPE_TIME:
	case PARSE_EVENTS__TERM_TYPE_CALLGRAPH:
	case PARSE_EVENTS__TERM_TYPE_STACKSIZE:
	case PARSE_EVENTS__TERM_TYPE_NOINHERIT:
	case PARSE_EVENTS__TERM_TYPE_INHERIT:
	case PARSE_EVENTS__TERM_TYPE_MAX_STACK:
	case PARSE_EVENTS__TERM_TYPE_MAX_EVENTS:
	case PARSE_EVENTS__TERM_TYPE_NOOVERWRITE:
	case PARSE_EVENTS__TERM_TYPE_OVERWRITE:
	case PARSE_EVENTS__TERM_TYPE_DRV_CFG:
	case PARSE_EVENTS__TERM_TYPE_AUX_OUTPUT:
	case PARSE_EVENTS__TERM_TYPE_AUX_SAMPLE_SIZE:
	case PARSE_EVENTS__TERM_TYPE_RAW:
	case PARSE_EVENTS__TERM_TYPE_LEGACY_CACHE:
	case PARSE_EVENTS__TERM_TYPE_HARDWARE:
	default:
		if (!err)
			return false;

		/* term_type is validated so indexing is safe */
		if (asprintf(&err_str, "'%s' is not usable in 'perf stat'",
			     config_term_name(term_type)) >= 0)
			parse_events_error__handle(err, -1, err_str, NULL);
		return false;
	}
}

void parse_events__shrink_config_terms(void)
{
	config_term_shrinked = true;
}

static int config_term_common(struct perf_event_attr *attr,
			      struct parse_events_term *term,
			      struct parse_events_error *err)
{
#define CHECK_TYPE_VAL(type)						   \
do {									   \
	if (check_type_val(term, err, PARSE_EVENTS__TERM_TYPE_ ## type)) \
		return -EINVAL;						   \
} while (0)

	switch (term->type_term) {
	case PARSE_EVENTS__TERM_TYPE_CONFIG:
		CHECK_TYPE_VAL(NUM);
		attr->config = term->val.num;
		break;
	case PARSE_EVENTS__TERM_TYPE_CONFIG1:
		CHECK_TYPE_VAL(NUM);
		attr->config1 = term->val.num;
		break;
	case PARSE_EVENTS__TERM_TYPE_CONFIG2:
		CHECK_TYPE_VAL(NUM);
		attr->config2 = term->val.num;
		break;
	case PARSE_EVENTS__TERM_TYPE_CONFIG3:
		CHECK_TYPE_VAL(NUM);
		attr->config3 = term->val.num;
		break;
	case PARSE_EVENTS__TERM_TYPE_SAMPLE_PERIOD:
		CHECK_TYPE_VAL(NUM);
		break;
	case PARSE_EVENTS__TERM_TYPE_SAMPLE_FREQ:
		CHECK_TYPE_VAL(NUM);
		break;
	case PARSE_EVENTS__TERM_TYPE_BRANCH_SAMPLE_TYPE:
		CHECK_TYPE_VAL(STR);
		if (strcmp(term->val.str, "no") &&
		    parse_branch_str(term->val.str,
				    &attr->branch_sample_type)) {
			parse_events_error__handle(err, term->err_val,
					strdup("invalid branch sample type"),
					NULL);
			return -EINVAL;
		}
		break;
	case PARSE_EVENTS__TERM_TYPE_TIME:
		CHECK_TYPE_VAL(NUM);
		if (term->val.num > 1) {
			parse_events_error__handle(err, term->err_val,
						strdup("expected 0 or 1"),
						NULL);
			return -EINVAL;
		}
		break;
	case PARSE_EVENTS__TERM_TYPE_CALLGRAPH:
		CHECK_TYPE_VAL(STR);
		break;
	case PARSE_EVENTS__TERM_TYPE_STACKSIZE:
		CHECK_TYPE_VAL(NUM);
		break;
	case PARSE_EVENTS__TERM_TYPE_INHERIT:
		CHECK_TYPE_VAL(NUM);
		break;
	case PARSE_EVENTS__TERM_TYPE_NOINHERIT:
		CHECK_TYPE_VAL(NUM);
		break;
	case PARSE_EVENTS__TERM_TYPE_OVERWRITE:
		CHECK_TYPE_VAL(NUM);
		break;
	case PARSE_EVENTS__TERM_TYPE_NOOVERWRITE:
		CHECK_TYPE_VAL(NUM);
		break;
	case PARSE_EVENTS__TERM_TYPE_NAME:
		CHECK_TYPE_VAL(STR);
		break;
	case PARSE_EVENTS__TERM_TYPE_METRIC_ID:
		CHECK_TYPE_VAL(STR);
		break;
	case PARSE_EVENTS__TERM_TYPE_RAW:
		CHECK_TYPE_VAL(STR);
		break;
	case PARSE_EVENTS__TERM_TYPE_MAX_STACK:
		CHECK_TYPE_VAL(NUM);
		break;
	case PARSE_EVENTS__TERM_TYPE_MAX_EVENTS:
		CHECK_TYPE_VAL(NUM);
		break;
	case PARSE_EVENTS__TERM_TYPE_PERCORE:
		CHECK_TYPE_VAL(NUM);
		if ((unsigned int)term->val.num > 1) {
			parse_events_error__handle(err, term->err_val,
						strdup("expected 0 or 1"),
						NULL);
			return -EINVAL;
		}
		break;
	case PARSE_EVENTS__TERM_TYPE_AUX_OUTPUT:
		CHECK_TYPE_VAL(NUM);
		break;
	case PARSE_EVENTS__TERM_TYPE_AUX_SAMPLE_SIZE:
		CHECK_TYPE_VAL(NUM);
		if (term->val.num > UINT_MAX) {
			parse_events_error__handle(err, term->err_val,
						strdup("too big"),
						NULL);
			return -EINVAL;
		}
		break;
	case PARSE_EVENTS__TERM_TYPE_DRV_CFG:
	case PARSE_EVENTS__TERM_TYPE_USER:
	case PARSE_EVENTS__TERM_TYPE_LEGACY_CACHE:
	case PARSE_EVENTS__TERM_TYPE_HARDWARE:
	default:
		parse_events_error__handle(err, term->err_term,
					strdup(config_term_name(term->type_term)),
					parse_events_formats_error_string(NULL));
		return -EINVAL;
	}

	/*
	 * Check term availability after basic checking so
	 * PARSE_EVENTS__TERM_TYPE_USER can be found and filtered.
	 *
	 * If check availability at the entry of this function,
	 * user will see "'<sysfs term>' is not usable in 'perf stat'"
	 * if an invalid config term is provided for legacy events
	 * (for example, instructions/badterm/...), which is confusing.
	 */
	if (!config_term_avail(term->type_term, err))
		return -EINVAL;
	return 0;
#undef CHECK_TYPE_VAL
}

static int config_term_pmu(struct perf_event_attr *attr,
			   struct parse_events_term *term,
			   struct parse_events_error *err)
{
	if (term->type_term == PARSE_EVENTS__TERM_TYPE_LEGACY_CACHE) {
		struct perf_pmu *pmu = perf_pmus__find_by_type(attr->type);

		if (!pmu) {
			char *err_str;

			if (asprintf(&err_str, "Failed to find PMU for type %d", attr->type) >= 0)
				parse_events_error__handle(err, term->err_term,
							   err_str, /*help=*/NULL);
			return -EINVAL;
		}
		/*
		 * Rewrite the PMU event to a legacy cache one unless the PMU
		 * doesn't support legacy cache events or the event is present
		 * within the PMU.
		 */
		if (perf_pmu__supports_legacy_cache(pmu) &&
		    !perf_pmu__have_event(pmu, term->config)) {
			attr->type = PERF_TYPE_HW_CACHE;
			return parse_events__decode_legacy_cache(term->config, pmu->type,
								 &attr->config);
		} else {
			term->type_term = PARSE_EVENTS__TERM_TYPE_USER;
			term->no_value = true;
		}
	}
	if (term->type_term == PARSE_EVENTS__TERM_TYPE_HARDWARE) {
		struct perf_pmu *pmu = perf_pmus__find_by_type(attr->type);

		if (!pmu) {
			char *err_str;

			if (asprintf(&err_str, "Failed to find PMU for type %d", attr->type) >= 0)
				parse_events_error__handle(err, term->err_term,
							   err_str, /*help=*/NULL);
			return -EINVAL;
		}
		/*
		 * If the PMU has a sysfs or json event prefer it over
		 * legacy. ARM requires this.
		 */
		if (perf_pmu__have_event(pmu, term->config)) {
			term->type_term = PARSE_EVENTS__TERM_TYPE_USER;
			term->no_value = true;
		} else {
			attr->type = PERF_TYPE_HARDWARE;
			attr->config = term->val.num;
			if (perf_pmus__supports_extended_type())
				attr->config |= (__u64)pmu->type << PERF_PMU_TYPE_SHIFT;
		}
		return 0;
	}
	if (term->type_term == PARSE_EVENTS__TERM_TYPE_USER ||
	    term->type_term == PARSE_EVENTS__TERM_TYPE_DRV_CFG) {
		/*
		 * Always succeed for sysfs terms, as we dont know
		 * at this point what type they need to have.
		 */
		return 0;
	}
	return config_term_common(attr, term, err);
}

#ifdef HAVE_LIBTRACEEVENT
static int config_term_tracepoint(struct perf_event_attr *attr,
				  struct parse_events_term *term,
				  struct parse_events_error *err)
{
	switch (term->type_term) {
	case PARSE_EVENTS__TERM_TYPE_CALLGRAPH:
	case PARSE_EVENTS__TERM_TYPE_STACKSIZE:
	case PARSE_EVENTS__TERM_TYPE_INHERIT:
	case PARSE_EVENTS__TERM_TYPE_NOINHERIT:
	case PARSE_EVENTS__TERM_TYPE_MAX_STACK:
	case PARSE_EVENTS__TERM_TYPE_MAX_EVENTS:
	case PARSE_EVENTS__TERM_TYPE_OVERWRITE:
	case PARSE_EVENTS__TERM_TYPE_NOOVERWRITE:
	case PARSE_EVENTS__TERM_TYPE_AUX_OUTPUT:
	case PARSE_EVENTS__TERM_TYPE_AUX_SAMPLE_SIZE:
		return config_term_common(attr, term, err);
	case PARSE_EVENTS__TERM_TYPE_USER:
	case PARSE_EVENTS__TERM_TYPE_CONFIG:
	case PARSE_EVENTS__TERM_TYPE_CONFIG1:
	case PARSE_EVENTS__TERM_TYPE_CONFIG2:
	case PARSE_EVENTS__TERM_TYPE_CONFIG3:
	case PARSE_EVENTS__TERM_TYPE_NAME:
	case PARSE_EVENTS__TERM_TYPE_SAMPLE_PERIOD:
	case PARSE_EVENTS__TERM_TYPE_SAMPLE_FREQ:
	case PARSE_EVENTS__TERM_TYPE_BRANCH_SAMPLE_TYPE:
	case PARSE_EVENTS__TERM_TYPE_TIME:
	case PARSE_EVENTS__TERM_TYPE_DRV_CFG:
	case PARSE_EVENTS__TERM_TYPE_PERCORE:
	case PARSE_EVENTS__TERM_TYPE_METRIC_ID:
	case PARSE_EVENTS__TERM_TYPE_RAW:
	case PARSE_EVENTS__TERM_TYPE_LEGACY_CACHE:
	case PARSE_EVENTS__TERM_TYPE_HARDWARE:
	default:
		if (err) {
			parse_events_error__handle(err, term->err_term,
						   strdup(config_term_name(term->type_term)),
				strdup("valid terms: call-graph,stack-size\n"));
		}
		return -EINVAL;
	}

	return 0;
}
#endif

static int config_attr(struct perf_event_attr *attr,
		       struct parse_events_terms *head,
		       struct parse_events_error *err,
		       config_term_func_t config_term)
{
	struct parse_events_term *term;

	list_for_each_entry(term, &head->terms, list)
		if (config_term(attr, term, err))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EINVAL;

	return 0;
}

<<<<<<< HEAD
int parse_events_add_numeric_legacy(struct list_head *list, int *idx,
			     const char *name, unsigned long config,
			     struct list_head *head_config)
{
	struct perf_event_attr attr;
	struct perf_pmu *pmu;
	char *pmu_name = strdup(name);

	memset(&attr, 0, sizeof(attr));

	pmu = perf_pmu__find(pmu_name);

	if (!pmu)
		return -EINVAL;

	attr.type = pmu->type;
	attr.config = config;

	if (head_config &&
	    config_attr(&attr, head_config, 1))
		return -EINVAL;

	return add_event(list, idx, &attr,
			 (char *) __event_name(pmu->type, config, pmu_name));
}

int parse_events_add_numeric(struct list_head *list, int *idx,
			     unsigned long type, unsigned long config,
			     struct list_head *head_config)
{
	struct perf_event_attr attr;
=======
static int get_config_terms(struct parse_events_terms *head_config, struct list_head *head_terms)
{
#define ADD_CONFIG_TERM(__type, __weak)				\
	struct evsel_config_term *__t;			\
								\
	__t = zalloc(sizeof(*__t));				\
	if (!__t)						\
		return -ENOMEM;					\
								\
	INIT_LIST_HEAD(&__t->list);				\
	__t->type       = EVSEL__CONFIG_TERM_ ## __type;	\
	__t->weak	= __weak;				\
	list_add_tail(&__t->list, head_terms)

#define ADD_CONFIG_TERM_VAL(__type, __name, __val, __weak)	\
do {								\
	ADD_CONFIG_TERM(__type, __weak);			\
	__t->val.__name = __val;				\
} while (0)

#define ADD_CONFIG_TERM_STR(__type, __val, __weak)		\
do {								\
	ADD_CONFIG_TERM(__type, __weak);			\
	__t->val.str = strdup(__val);				\
	if (!__t->val.str) {					\
		zfree(&__t);					\
		return -ENOMEM;					\
	}							\
	__t->free_str = true;					\
} while (0)

	struct parse_events_term *term;

	list_for_each_entry(term, &head_config->terms, list) {
		switch (term->type_term) {
		case PARSE_EVENTS__TERM_TYPE_SAMPLE_PERIOD:
			ADD_CONFIG_TERM_VAL(PERIOD, period, term->val.num, term->weak);
			break;
		case PARSE_EVENTS__TERM_TYPE_SAMPLE_FREQ:
			ADD_CONFIG_TERM_VAL(FREQ, freq, term->val.num, term->weak);
			break;
		case PARSE_EVENTS__TERM_TYPE_TIME:
			ADD_CONFIG_TERM_VAL(TIME, time, term->val.num, term->weak);
			break;
		case PARSE_EVENTS__TERM_TYPE_CALLGRAPH:
			ADD_CONFIG_TERM_STR(CALLGRAPH, term->val.str, term->weak);
			break;
		case PARSE_EVENTS__TERM_TYPE_BRANCH_SAMPLE_TYPE:
			ADD_CONFIG_TERM_STR(BRANCH, term->val.str, term->weak);
			break;
		case PARSE_EVENTS__TERM_TYPE_STACKSIZE:
			ADD_CONFIG_TERM_VAL(STACK_USER, stack_user,
					    term->val.num, term->weak);
			break;
		case PARSE_EVENTS__TERM_TYPE_INHERIT:
			ADD_CONFIG_TERM_VAL(INHERIT, inherit,
					    term->val.num ? 1 : 0, term->weak);
			break;
		case PARSE_EVENTS__TERM_TYPE_NOINHERIT:
			ADD_CONFIG_TERM_VAL(INHERIT, inherit,
					    term->val.num ? 0 : 1, term->weak);
			break;
		case PARSE_EVENTS__TERM_TYPE_MAX_STACK:
			ADD_CONFIG_TERM_VAL(MAX_STACK, max_stack,
					    term->val.num, term->weak);
			break;
		case PARSE_EVENTS__TERM_TYPE_MAX_EVENTS:
			ADD_CONFIG_TERM_VAL(MAX_EVENTS, max_events,
					    term->val.num, term->weak);
			break;
		case PARSE_EVENTS__TERM_TYPE_OVERWRITE:
			ADD_CONFIG_TERM_VAL(OVERWRITE, overwrite,
					    term->val.num ? 1 : 0, term->weak);
			break;
		case PARSE_EVENTS__TERM_TYPE_NOOVERWRITE:
			ADD_CONFIG_TERM_VAL(OVERWRITE, overwrite,
					    term->val.num ? 0 : 1, term->weak);
			break;
		case PARSE_EVENTS__TERM_TYPE_DRV_CFG:
			ADD_CONFIG_TERM_STR(DRV_CFG, term->val.str, term->weak);
			break;
		case PARSE_EVENTS__TERM_TYPE_PERCORE:
			ADD_CONFIG_TERM_VAL(PERCORE, percore,
					    term->val.num ? true : false, term->weak);
			break;
		case PARSE_EVENTS__TERM_TYPE_AUX_OUTPUT:
			ADD_CONFIG_TERM_VAL(AUX_OUTPUT, aux_output,
					    term->val.num ? 1 : 0, term->weak);
			break;
		case PARSE_EVENTS__TERM_TYPE_AUX_SAMPLE_SIZE:
			ADD_CONFIG_TERM_VAL(AUX_SAMPLE_SIZE, aux_sample_size,
					    term->val.num, term->weak);
			break;
		case PARSE_EVENTS__TERM_TYPE_USER:
		case PARSE_EVENTS__TERM_TYPE_CONFIG:
		case PARSE_EVENTS__TERM_TYPE_CONFIG1:
		case PARSE_EVENTS__TERM_TYPE_CONFIG2:
		case PARSE_EVENTS__TERM_TYPE_CONFIG3:
		case PARSE_EVENTS__TERM_TYPE_NAME:
		case PARSE_EVENTS__TERM_TYPE_METRIC_ID:
		case PARSE_EVENTS__TERM_TYPE_RAW:
		case PARSE_EVENTS__TERM_TYPE_LEGACY_CACHE:
		case PARSE_EVENTS__TERM_TYPE_HARDWARE:
		default:
			break;
		}
	}
	return 0;
}

/*
 * Add EVSEL__CONFIG_TERM_CFG_CHG where cfg_chg will have a bit set for
 * each bit of attr->config that the user has changed.
 */
static int get_config_chgs(struct perf_pmu *pmu, struct parse_events_terms *head_config,
			   struct list_head *head_terms)
{
	struct parse_events_term *term;
	u64 bits = 0;
	int type;

	list_for_each_entry(term, &head_config->terms, list) {
		switch (term->type_term) {
		case PARSE_EVENTS__TERM_TYPE_USER:
			type = perf_pmu__format_type(pmu, term->config);
			if (type != PERF_PMU_FORMAT_VALUE_CONFIG)
				continue;
			bits |= perf_pmu__format_bits(pmu, term->config);
			break;
		case PARSE_EVENTS__TERM_TYPE_CONFIG:
			bits = ~(u64)0;
			break;
		case PARSE_EVENTS__TERM_TYPE_CONFIG1:
		case PARSE_EVENTS__TERM_TYPE_CONFIG2:
		case PARSE_EVENTS__TERM_TYPE_CONFIG3:
		case PARSE_EVENTS__TERM_TYPE_NAME:
		case PARSE_EVENTS__TERM_TYPE_SAMPLE_PERIOD:
		case PARSE_EVENTS__TERM_TYPE_SAMPLE_FREQ:
		case PARSE_EVENTS__TERM_TYPE_BRANCH_SAMPLE_TYPE:
		case PARSE_EVENTS__TERM_TYPE_TIME:
		case PARSE_EVENTS__TERM_TYPE_CALLGRAPH:
		case PARSE_EVENTS__TERM_TYPE_STACKSIZE:
		case PARSE_EVENTS__TERM_TYPE_NOINHERIT:
		case PARSE_EVENTS__TERM_TYPE_INHERIT:
		case PARSE_EVENTS__TERM_TYPE_MAX_STACK:
		case PARSE_EVENTS__TERM_TYPE_MAX_EVENTS:
		case PARSE_EVENTS__TERM_TYPE_NOOVERWRITE:
		case PARSE_EVENTS__TERM_TYPE_OVERWRITE:
		case PARSE_EVENTS__TERM_TYPE_DRV_CFG:
		case PARSE_EVENTS__TERM_TYPE_PERCORE:
		case PARSE_EVENTS__TERM_TYPE_AUX_OUTPUT:
		case PARSE_EVENTS__TERM_TYPE_AUX_SAMPLE_SIZE:
		case PARSE_EVENTS__TERM_TYPE_METRIC_ID:
		case PARSE_EVENTS__TERM_TYPE_RAW:
		case PARSE_EVENTS__TERM_TYPE_LEGACY_CACHE:
		case PARSE_EVENTS__TERM_TYPE_HARDWARE:
		default:
			break;
		}
	}

	if (bits)
		ADD_CONFIG_TERM_VAL(CFG_CHG, cfg_chg, bits, false);

#undef ADD_CONFIG_TERM
	return 0;
}

int parse_events_add_tracepoint(struct list_head *list, int *idx,
				const char *sys, const char *event,
				struct parse_events_error *err,
				struct parse_events_terms *head_config, void *loc_)
{
	YYLTYPE *loc = loc_;
#ifdef HAVE_LIBTRACEEVENT
	if (head_config) {
		struct perf_event_attr attr;

		if (config_attr(&attr, head_config, err,
				config_term_tracepoint))
			return -EINVAL;
	}

	if (strpbrk(sys, "*?"))
		return add_tracepoint_multi_sys(list, idx, sys, event,
						err, head_config, loc);
	else
		return add_tracepoint_event(list, idx, sys, event,
					    err, head_config, loc);
#else
	(void)list;
	(void)idx;
	(void)sys;
	(void)event;
	(void)head_config;
	parse_events_error__handle(err, loc->first_column, strdup("unsupported tracepoint"),
				strdup("libtraceevent is necessary for tracepoint support"));
	return -1;
#endif
}

static int __parse_events_add_numeric(struct parse_events_state *parse_state,
				struct list_head *list,
				struct perf_pmu *pmu, u32 type, u32 extended_type,
				u64 config, struct parse_events_terms *head_config)
{
	struct perf_event_attr attr;
	LIST_HEAD(config_terms);
	const char *name, *metric_id;
	int ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	memset(&attr, 0, sizeof(attr));
	attr.type = type;
	attr.config = config;
<<<<<<< HEAD

	if (head_config &&
	    config_attr(&attr, head_config, 1))
		return -EINVAL;

	return add_event(list, idx, &attr,
			 (char *) __event_name(type, config, NULL));
}

int parse_events_add_pmu(struct list_head *list, int *idx,
			 char *name, struct list_head *head_config)
{
	struct perf_event_attr attr;
	struct perf_pmu *pmu;
	char *ev_name;

	pmu = perf_pmu__find(name);
	if (!pmu)
		return -EINVAL;

	memset(&attr, 0, sizeof(attr));

	/*
	 * Configure hardcoded terms first, no need to check
	 * return value when called with fail == 0 ;)
	 */
	config_attr(&attr, head_config, 0);

	if (perf_pmu__config(pmu, &attr, head_config))
		return -EINVAL;

	ev_name = (char *) __event_name(attr.type, attr.config, pmu->name);

	return add_event(list, idx, &attr, ev_name);
}

=======
	if (extended_type && (type == PERF_TYPE_HARDWARE || type == PERF_TYPE_HW_CACHE)) {
		assert(perf_pmus__supports_extended_type());
		attr.config |= (u64)extended_type << PERF_PMU_TYPE_SHIFT;
	}

	if (head_config) {
		if (config_attr(&attr, head_config, parse_state->error,
				config_term_common))
			return -EINVAL;

		if (get_config_terms(head_config, &config_terms))
			return -ENOMEM;
	}

	name = get_config_name(head_config);
	metric_id = get_config_metric_id(head_config);
	ret = __add_event(list, &parse_state->idx, &attr, /*init_attr*/true, name,
			metric_id, pmu, &config_terms, /*auto_merge_stats=*/false,
			/*cpu_list=*/NULL) ? 0 : -ENOMEM;
	free_config_terms(&config_terms);
	return ret;
}

int parse_events_add_numeric(struct parse_events_state *parse_state,
			     struct list_head *list,
			     u32 type, u64 config,
			     struct parse_events_terms *head_config,
			     bool wildcard)
{
	struct perf_pmu *pmu = NULL;
	bool found_supported = false;

	/* Wildcards on numeric values are only supported by core PMUs. */
	if (wildcard && perf_pmus__supports_extended_type()) {
		while ((pmu = perf_pmus__scan_core(pmu)) != NULL) {
			int ret;

			found_supported = true;
			if (parse_events__filter_pmu(parse_state, pmu))
				continue;

			ret = __parse_events_add_numeric(parse_state, list, pmu,
							 type, pmu->type,
							 config, head_config);
			if (ret)
				return ret;
		}
		if (found_supported)
			return 0;
	}
	return __parse_events_add_numeric(parse_state, list, perf_pmus__find_by_type(type),
					type, /*extended_type=*/0, config, head_config);
}

int parse_events_add_tool(struct parse_events_state *parse_state,
			  struct list_head *list,
			  int tool_event)
{
	return add_event_tool(list, &parse_state->idx, tool_event);
}

static bool config_term_percore(struct list_head *config_terms)
{
	struct evsel_config_term *term;

	list_for_each_entry(term, config_terms, list) {
		if (term->type == EVSEL__CONFIG_TERM_PERCORE)
			return term->val.percore;
	}

	return false;
}

int parse_events_add_pmu(struct parse_events_state *parse_state,
			 struct list_head *list, const char *name,
			 const struct parse_events_terms *const_parsed_terms,
			 bool auto_merge_stats, void *loc_)
{
	struct perf_event_attr attr;
	struct perf_pmu_info info;
	struct perf_pmu *pmu;
	struct evsel *evsel;
	struct parse_events_error *err = parse_state->error;
	YYLTYPE *loc = loc_;
	LIST_HEAD(config_terms);
	struct parse_events_terms parsed_terms;
	bool alias_rewrote_terms = false;

	pmu = parse_state->fake_pmu ?: perf_pmus__find(name);

	if (!pmu) {
		char *err_str;

		if (asprintf(&err_str,
				"Cannot find PMU `%s'. Missing kernel support?",
				name) >= 0)
			parse_events_error__handle(err, loc->first_column, err_str, NULL);
		return -EINVAL;
	}

	parse_events_terms__init(&parsed_terms);
	if (const_parsed_terms) {
		int ret = parse_events_terms__copy(const_parsed_terms, &parsed_terms);

		if (ret)
			return ret;
	}

	if (verbose > 1) {
		struct strbuf sb;

		strbuf_init(&sb, /*hint=*/ 0);
		if (pmu->selectable && list_empty(&parsed_terms.terms)) {
			strbuf_addf(&sb, "%s//", name);
		} else {
			strbuf_addf(&sb, "%s/", name);
			parse_events_terms__to_strbuf(&parsed_terms, &sb);
			strbuf_addch(&sb, '/');
		}
		fprintf(stderr, "Attempt to add: %s\n", sb.buf);
		strbuf_release(&sb);
	}
	fix_raw(&parsed_terms, pmu);

	memset(&attr, 0, sizeof(attr));
	if (pmu->perf_event_attr_init_default)
		pmu->perf_event_attr_init_default(pmu, &attr);

	attr.type = pmu->type;

	if (list_empty(&parsed_terms.terms)) {
		evsel = __add_event(list, &parse_state->idx, &attr,
				    /*init_attr=*/true, /*name=*/NULL,
				    /*metric_id=*/NULL, pmu,
				    /*config_terms=*/NULL, auto_merge_stats,
				    /*cpu_list=*/NULL);
		return evsel ? 0 : -ENOMEM;
	}

	/* Configure attr/terms with a known PMU, this will set hardcoded terms. */
	if (config_attr(&attr, &parsed_terms, parse_state->error, config_term_pmu)) {
		parse_events_terms__exit(&parsed_terms);
		return -EINVAL;
	}

	/* Look for event names in the terms and rewrite into format based terms. */
	if (!parse_state->fake_pmu && perf_pmu__check_alias(pmu, &parsed_terms,
							    &info, &alias_rewrote_terms, err)) {
		parse_events_terms__exit(&parsed_terms);
		return -EINVAL;
	}

	if (verbose > 1) {
		struct strbuf sb;

		strbuf_init(&sb, /*hint=*/ 0);
		parse_events_terms__to_strbuf(&parsed_terms, &sb);
		fprintf(stderr, "..after resolving event: %s/%s/\n", name, sb.buf);
		strbuf_release(&sb);
	}

	/* Configure attr/terms again if an alias was expanded. */
	if (alias_rewrote_terms &&
	    config_attr(&attr, &parsed_terms, parse_state->error, config_term_pmu)) {
		parse_events_terms__exit(&parsed_terms);
		return -EINVAL;
	}

	if (get_config_terms(&parsed_terms, &config_terms)) {
		parse_events_terms__exit(&parsed_terms);
		return -ENOMEM;
	}

	/*
	 * When using default config, record which bits of attr->config were
	 * changed by the user.
	 */
	if (pmu->perf_event_attr_init_default &&
	    get_config_chgs(pmu, &parsed_terms, &config_terms)) {
		parse_events_terms__exit(&parsed_terms);
		return -ENOMEM;
	}

	if (!parse_state->fake_pmu &&
	    perf_pmu__config(pmu, &attr, &parsed_terms, parse_state->error)) {
		free_config_terms(&config_terms);
		parse_events_terms__exit(&parsed_terms);
		return -EINVAL;
	}

	evsel = __add_event(list, &parse_state->idx, &attr, /*init_attr=*/true,
			    get_config_name(&parsed_terms),
			    get_config_metric_id(&parsed_terms), pmu,
			    &config_terms, auto_merge_stats, /*cpu_list=*/NULL);
	if (!evsel) {
		parse_events_terms__exit(&parsed_terms);
		return -ENOMEM;
	}

	if (evsel->name)
		evsel->use_config_name = true;

	evsel->percore = config_term_percore(&evsel->config_terms);

	if (parse_state->fake_pmu) {
		parse_events_terms__exit(&parsed_terms);
		return 0;
	}

	parse_events_terms__exit(&parsed_terms);
	free((char *)evsel->unit);
	evsel->unit = strdup(info.unit);
	evsel->scale = info.scale;
	evsel->per_pkg = info.per_pkg;
	evsel->snapshot = info.snapshot;
	return 0;
}

int parse_events_multi_pmu_add(struct parse_events_state *parse_state,
			       const char *event_name,
			       const struct parse_events_terms *const_parsed_terms,
			       struct list_head **listp, void *loc_)
{
	struct parse_events_term *term;
	struct list_head *list = NULL;
	struct perf_pmu *pmu = NULL;
	YYLTYPE *loc = loc_;
	int ok = 0;
	const char *config;
	struct parse_events_terms parsed_terms;

	*listp = NULL;

	parse_events_terms__init(&parsed_terms);
	if (const_parsed_terms) {
		int ret = parse_events_terms__copy(const_parsed_terms, &parsed_terms);

		if (ret)
			return ret;
	}

	config = strdup(event_name);
	if (!config)
		goto out_err;

	if (parse_events_term__num(&term,
				   PARSE_EVENTS__TERM_TYPE_USER,
				   config, /*num=*/1, /*novalue=*/true,
				   loc, /*loc_val=*/NULL) < 0) {
		zfree(&config);
		goto out_err;
	}
	list_add_tail(&term->list, &parsed_terms.terms);

	/* Add it for all PMUs that support the alias */
	list = malloc(sizeof(struct list_head));
	if (!list)
		goto out_err;

	INIT_LIST_HEAD(list);

	while ((pmu = perf_pmus__scan(pmu)) != NULL) {
		bool auto_merge_stats;

		if (parse_events__filter_pmu(parse_state, pmu))
			continue;

		if (!perf_pmu__have_event(pmu, event_name))
			continue;

		auto_merge_stats = perf_pmu__auto_merge_stats(pmu);
		if (!parse_events_add_pmu(parse_state, list, pmu->name,
					  &parsed_terms, auto_merge_stats, loc)) {
			struct strbuf sb;

			strbuf_init(&sb, /*hint=*/ 0);
			parse_events_terms__to_strbuf(&parsed_terms, &sb);
			pr_debug("%s -> %s/%s/\n", event_name, pmu->name, sb.buf);
			strbuf_release(&sb);
			ok++;
		}
	}

	if (parse_state->fake_pmu) {
		if (!parse_events_add_pmu(parse_state, list, event_name, &parsed_terms,
					  /*auto_merge_stats=*/true, loc)) {
			struct strbuf sb;

			strbuf_init(&sb, /*hint=*/ 0);
			parse_events_terms__to_strbuf(&parsed_terms, &sb);
			pr_debug("%s -> %s/%s/\n", event_name, "fake_pmu", sb.buf);
			strbuf_release(&sb);
			ok++;
		}
	}

out_err:
	parse_events_terms__exit(&parsed_terms);
	if (ok)
		*listp = list;
	else
		free(list);

	return ok ? 0 : -1;
}

int parse_events__modifier_group(struct list_head *list,
				 char *event_mod)
{
	return parse_events__modifier_event(list, event_mod, true);
}

void parse_events__set_leader(char *name, struct list_head *list)
{
	struct evsel *leader;

	if (list_empty(list)) {
		WARN_ONCE(true, "WARNING: failed to set leader: empty list");
		return;
	}

	leader = list_first_entry(list, struct evsel, core.node);
	__perf_evlist__set_leader(list, &leader->core);
	leader->group_name = name;
}

/* list_event is assumed to point to malloc'ed memory */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void parse_events_update_lists(struct list_head *list_event,
			       struct list_head *list_all)
{
	/*
	 * Called for single event definition. Update the
<<<<<<< HEAD
	 * 'all event' list, and reinit the 'signle event'
	 * list, for next event definition.
	 */
	list_splice_tail(list_event, list_all);
	INIT_LIST_HEAD(list_event);
}

int parse_events_modifier(struct list_head *list, char *str)
{
	struct perf_evsel *evsel;
	int exclude = 0, exclude_GH = 0;
	int eu = 0, ek = 0, eh = 0, eH = 0, eG = 0, precise = 0;

	if (str == NULL)
		return 0;
=======
	 * 'all event' list, and reinit the 'single event'
	 * list, for next event definition.
	 */
	list_splice_tail(list_event, list_all);
	free(list_event);
}

struct event_modifier {
	int eu;
	int ek;
	int eh;
	int eH;
	int eG;
	int eI;
	int precise;
	int precise_max;
	int exclude_GH;
	int sample_read;
	int pinned;
	int weak;
	int exclusive;
	int bpf_counter;
};

static int get_event_modifier(struct event_modifier *mod, char *str,
			       struct evsel *evsel)
{
	int eu = evsel ? evsel->core.attr.exclude_user : 0;
	int ek = evsel ? evsel->core.attr.exclude_kernel : 0;
	int eh = evsel ? evsel->core.attr.exclude_hv : 0;
	int eH = evsel ? evsel->core.attr.exclude_host : 0;
	int eG = evsel ? evsel->core.attr.exclude_guest : 0;
	int eI = evsel ? evsel->core.attr.exclude_idle : 0;
	int precise = evsel ? evsel->core.attr.precise_ip : 0;
	int precise_max = 0;
	int sample_read = 0;
	int pinned = evsel ? evsel->core.attr.pinned : 0;
	int exclusive = evsel ? evsel->core.attr.exclusive : 0;

	int exclude = eu | ek | eh;
	int exclude_GH = evsel ? evsel->exclude_GH : 0;
	int weak = 0;
	int bpf_counter = 0;

	memset(mod, 0, sizeof(*mod));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	while (*str) {
		if (*str == 'u') {
			if (!exclude)
				exclude = eu = ek = eh = 1;
<<<<<<< HEAD
=======
			if (!exclude_GH && !perf_guest)
				eG = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			eu = 0;
		} else if (*str == 'k') {
			if (!exclude)
				exclude = eu = ek = eh = 1;
			ek = 0;
		} else if (*str == 'h') {
			if (!exclude)
				exclude = eu = ek = eh = 1;
			eh = 0;
		} else if (*str == 'G') {
			if (!exclude_GH)
				exclude_GH = eG = eH = 1;
			eG = 0;
		} else if (*str == 'H') {
			if (!exclude_GH)
				exclude_GH = eG = eH = 1;
			eH = 0;
<<<<<<< HEAD
		} else if (*str == 'p') {
			precise++;
=======
		} else if (*str == 'I') {
			eI = 1;
		} else if (*str == 'p') {
			precise++;
			/* use of precise requires exclude_guest */
			if (!exclude_GH)
				eG = 1;
		} else if (*str == 'P') {
			precise_max = 1;
		} else if (*str == 'S') {
			sample_read = 1;
		} else if (*str == 'D') {
			pinned = 1;
		} else if (*str == 'e') {
			exclusive = 1;
		} else if (*str == 'W') {
			weak = 1;
		} else if (*str == 'b') {
			bpf_counter = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		} else
			break;

		++str;
	}

	/*
	 * precise ip:
	 *
	 *  0 - SAMPLE_IP can have arbitrary skid
	 *  1 - SAMPLE_IP must have constant skid
	 *  2 - SAMPLE_IP requested to have 0 skid
	 *  3 - SAMPLE_IP must have 0 skid
	 *
	 *  See also PERF_RECORD_MISC_EXACT_IP
	 */
	if (precise > 3)
		return -EINVAL;

<<<<<<< HEAD
	list_for_each_entry(evsel, list, node) {
		evsel->attr.exclude_user   = eu;
		evsel->attr.exclude_kernel = ek;
		evsel->attr.exclude_hv     = eh;
		evsel->attr.precise_ip     = precise;
		evsel->attr.exclude_host   = eH;
		evsel->attr.exclude_guest  = eG;
=======
	mod->eu = eu;
	mod->ek = ek;
	mod->eh = eh;
	mod->eH = eH;
	mod->eG = eG;
	mod->eI = eI;
	mod->precise = precise;
	mod->precise_max = precise_max;
	mod->exclude_GH = exclude_GH;
	mod->sample_read = sample_read;
	mod->pinned = pinned;
	mod->weak = weak;
	mod->bpf_counter = bpf_counter;
	mod->exclusive = exclusive;

	return 0;
}

/*
 * Basic modifier sanity check to validate it contains only one
 * instance of any modifier (apart from 'p') present.
 */
static int check_modifier(char *str)
{
	char *p = str;

	/* The sizeof includes 0 byte as well. */
	if (strlen(str) > (sizeof("ukhGHpppPSDIWeb") - 1))
		return -1;

	while (*p) {
		if (*p != 'p' && strchr(p + 1, *p))
			return -1;
		p++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}

<<<<<<< HEAD
int parse_events(struct perf_evlist *evlist, const char *str, int unset __used)
{
	LIST_HEAD(list);
	LIST_HEAD(list_tmp);
	YY_BUFFER_STATE buffer;
	int ret, idx = evlist->nr_entries;

	buffer = parse_events__scan_string(str);

	ret = parse_events_parse(&list, &list_tmp, &idx);

	parse_events__flush_buffer(buffer);
	parse_events__delete_buffer(buffer);

	if (!ret) {
		int entries = idx - evlist->nr_entries;
		perf_evlist__splice_list_tail(evlist, &list, entries);
=======
int parse_events__modifier_event(struct list_head *list, char *str, bool add)
{
	struct evsel *evsel;
	struct event_modifier mod;

	if (str == NULL)
		return 0;

	if (check_modifier(str))
		return -EINVAL;

	if (!add && get_event_modifier(&mod, str, NULL))
		return -EINVAL;

	__evlist__for_each_entry(list, evsel) {
		if (add && get_event_modifier(&mod, str, evsel))
			return -EINVAL;

		evsel->core.attr.exclude_user   = mod.eu;
		evsel->core.attr.exclude_kernel = mod.ek;
		evsel->core.attr.exclude_hv     = mod.eh;
		evsel->core.attr.precise_ip     = mod.precise;
		evsel->core.attr.exclude_host   = mod.eH;
		evsel->core.attr.exclude_guest  = mod.eG;
		evsel->core.attr.exclude_idle   = mod.eI;
		evsel->exclude_GH          = mod.exclude_GH;
		evsel->sample_read         = mod.sample_read;
		evsel->precise_max         = mod.precise_max;
		evsel->weak_group	   = mod.weak;
		evsel->bpf_counter	   = mod.bpf_counter;

		if (evsel__is_group_leader(evsel)) {
			evsel->core.attr.pinned = mod.pinned;
			evsel->core.attr.exclusive = mod.exclusive;
		}
	}

	return 0;
}

int parse_events_name(struct list_head *list, const char *name)
{
	struct evsel *evsel;

	__evlist__for_each_entry(list, evsel) {
		if (!evsel->name) {
			evsel->name = strdup(name);
			if (!evsel->name)
				return -ENOMEM;
		}
	}

	return 0;
}

static int parse_events__scanner(const char *str,
				 FILE *input,
				 struct parse_events_state *parse_state)
{
	YY_BUFFER_STATE buffer;
	void *scanner;
	int ret;

	ret = parse_events_lex_init_extra(parse_state, &scanner);
	if (ret)
		return ret;

	if (str)
		buffer = parse_events__scan_string(str, scanner);
	else
	        parse_events_set_in(input, scanner);

#ifdef PARSER_DEBUG
	parse_events_debug = 1;
	parse_events_set_debug(1, scanner);
#endif
	ret = parse_events_parse(parse_state, scanner);

	if (str) {
		parse_events__flush_buffer(buffer, scanner);
		parse_events__delete_buffer(buffer, scanner);
	}
	parse_events_lex_destroy(scanner);
	return ret;
}

/*
 * parse event config string, return a list of event terms.
 */
int parse_events_terms(struct parse_events_terms *terms, const char *str, FILE *input)
{
	struct parse_events_state parse_state = {
		.terms  = NULL,
		.stoken = PE_START_TERMS,
	};
	int ret;

	ret = parse_events__scanner(str, input, &parse_state);
	if (!ret)
		list_splice(&parse_state.terms->terms, &terms->terms);

	zfree(&parse_state.terms);
	return ret;
}

static int evsel__compute_group_pmu_name(struct evsel *evsel,
					  const struct list_head *head)
{
	struct evsel *leader = evsel__leader(evsel);
	struct evsel *pos;
	const char *group_pmu_name;
	struct perf_pmu *pmu = evsel__find_pmu(evsel);

	if (!pmu) {
		/*
		 * For PERF_TYPE_HARDWARE and PERF_TYPE_HW_CACHE types the PMU
		 * is a core PMU, but in heterogeneous systems this is
		 * unknown. For now pick the first core PMU.
		 */
		pmu = perf_pmus__scan_core(NULL);
	}
	if (!pmu) {
		pr_debug("No PMU found for '%s'\n", evsel__name(evsel));
		return -EINVAL;
	}
	group_pmu_name = pmu->name;
	/*
	 * Software events may be in a group with other uncore PMU events. Use
	 * the pmu_name of the first non-software event to avoid breaking the
	 * software event out of the group.
	 *
	 * Aux event leaders, like intel_pt, expect a group with events from
	 * other PMUs, so substitute the AUX event's PMU in this case.
	 */
	if (perf_pmu__is_software(pmu) || evsel__is_aux_event(leader)) {
		struct perf_pmu *leader_pmu = evsel__find_pmu(leader);

		if (!leader_pmu) {
			/* As with determining pmu above. */
			leader_pmu = perf_pmus__scan_core(NULL);
		}
		/*
		 * Starting with the leader, find the first event with a named
		 * non-software PMU. for_each_group_(member|evsel) isn't used as
		 * the list isn't yet sorted putting evsel's in the same group
		 * together.
		 */
		if (leader_pmu && !perf_pmu__is_software(leader_pmu)) {
			group_pmu_name = leader_pmu->name;
		} else if (leader->core.nr_members > 1) {
			list_for_each_entry(pos, head, core.node) {
				struct perf_pmu *pos_pmu;

				if (pos == leader || evsel__leader(pos) != leader)
					continue;
				pos_pmu = evsel__find_pmu(pos);
				if (!pos_pmu) {
					/* As with determining pmu above. */
					pos_pmu = perf_pmus__scan_core(NULL);
				}
				if (pos_pmu && !perf_pmu__is_software(pos_pmu)) {
					group_pmu_name = pos_pmu->name;
					break;
				}
			}
		}
	}
	/* Assign the actual name taking care that the fake PMU lacks a name. */
	evsel->group_pmu_name = strdup(group_pmu_name ?: "fake");
	return evsel->group_pmu_name ? 0 : -ENOMEM;
}

__weak int arch_evlist__cmp(const struct evsel *lhs, const struct evsel *rhs)
{
	/* Order by insertion index. */
	return lhs->core.idx - rhs->core.idx;
}

static int evlist__cmp(void *_fg_idx, const struct list_head *l, const struct list_head *r)
{
	const struct perf_evsel *lhs_core = container_of(l, struct perf_evsel, node);
	const struct evsel *lhs = container_of(lhs_core, struct evsel, core);
	const struct perf_evsel *rhs_core = container_of(r, struct perf_evsel, node);
	const struct evsel *rhs = container_of(rhs_core, struct evsel, core);
	int *force_grouped_idx = _fg_idx;
	int lhs_sort_idx, rhs_sort_idx, ret;
	const char *lhs_pmu_name, *rhs_pmu_name;
	bool lhs_has_group, rhs_has_group;

	/*
	 * First sort by grouping/leader. Read the leader idx only if the evsel
	 * is part of a group, by default ungrouped events will be sorted
	 * relative to grouped events based on where the first ungrouped event
	 * occurs. If both events don't have a group we want to fall-through to
	 * the arch specific sorting, that can reorder and fix things like
	 * Intel's topdown events.
	 */
	if (lhs_core->leader != lhs_core || lhs_core->nr_members > 1) {
		lhs_has_group = true;
		lhs_sort_idx = lhs_core->leader->idx;
	} else {
		lhs_has_group = false;
		lhs_sort_idx = *force_grouped_idx != -1 && arch_evsel__must_be_in_group(lhs)
			? *force_grouped_idx
			: lhs_core->idx;
	}
	if (rhs_core->leader != rhs_core || rhs_core->nr_members > 1) {
		rhs_has_group = true;
		rhs_sort_idx = rhs_core->leader->idx;
	} else {
		rhs_has_group = false;
		rhs_sort_idx = *force_grouped_idx != -1 && arch_evsel__must_be_in_group(rhs)
			? *force_grouped_idx
			: rhs_core->idx;
	}

	if (lhs_sort_idx != rhs_sort_idx)
		return lhs_sort_idx - rhs_sort_idx;

	/* Group by PMU if there is a group. Groups can't span PMUs. */
	if (lhs_has_group && rhs_has_group) {
		lhs_pmu_name = lhs->group_pmu_name;
		rhs_pmu_name = rhs->group_pmu_name;
		ret = strcmp(lhs_pmu_name, rhs_pmu_name);
		if (ret)
			return ret;
	}

	/* Architecture specific sorting. */
	return arch_evlist__cmp(lhs, rhs);
}

static int parse_events__sort_events_and_fix_groups(struct list_head *list)
{
	int idx = 0, force_grouped_idx = -1;
	struct evsel *pos, *cur_leader = NULL;
	struct perf_evsel *cur_leaders_grp = NULL;
	bool idx_changed = false, cur_leader_force_grouped = false;
	int orig_num_leaders = 0, num_leaders = 0;
	int ret;

	/*
	 * Compute index to insert ungrouped events at. Place them where the
	 * first ungrouped event appears.
	 */
	list_for_each_entry(pos, list, core.node) {
		const struct evsel *pos_leader = evsel__leader(pos);

		ret = evsel__compute_group_pmu_name(pos, list);
		if (ret)
			return ret;

		if (pos == pos_leader)
			orig_num_leaders++;

		/*
		 * Ensure indexes are sequential, in particular for multiple
		 * event lists being merged. The indexes are used to detect when
		 * the user order is modified.
		 */
		pos->core.idx = idx++;

		/* Remember an index to sort all forced grouped events together to. */
		if (force_grouped_idx == -1 && pos == pos_leader && pos->core.nr_members < 2 &&
		    arch_evsel__must_be_in_group(pos))
			force_grouped_idx = pos->core.idx;
	}

	/* Sort events. */
	list_sort(&force_grouped_idx, list, evlist__cmp);

	/*
	 * Recompute groups, splitting for PMUs and adding groups for events
	 * that require them.
	 */
	idx = 0;
	list_for_each_entry(pos, list, core.node) {
		const struct evsel *pos_leader = evsel__leader(pos);
		const char *pos_pmu_name = pos->group_pmu_name;
		const char *cur_leader_pmu_name;
		bool pos_force_grouped = force_grouped_idx != -1 &&
			arch_evsel__must_be_in_group(pos);

		/* Reset index and nr_members. */
		if (pos->core.idx != idx)
			idx_changed = true;
		pos->core.idx = idx++;
		pos->core.nr_members = 0;

		/*
		 * Set the group leader respecting the given groupings and that
		 * groups can't span PMUs.
		 */
		if (!cur_leader)
			cur_leader = pos;

		cur_leader_pmu_name = cur_leader->group_pmu_name;
		if ((cur_leaders_grp != pos->core.leader &&
		     (!pos_force_grouped || !cur_leader_force_grouped)) ||
		    strcmp(cur_leader_pmu_name, pos_pmu_name)) {
			/* Event is for a different group/PMU than last. */
			cur_leader = pos;
			/*
			 * Remember the leader's group before it is overwritten,
			 * so that later events match as being in the same
			 * group.
			 */
			cur_leaders_grp = pos->core.leader;
			/*
			 * Avoid forcing events into groups with events that
			 * don't need to be in the group.
			 */
			cur_leader_force_grouped = pos_force_grouped;
		}
		if (pos_leader != cur_leader) {
			/* The leader changed so update it. */
			evsel__set_leader(pos, cur_leader);
		}
	}
	list_for_each_entry(pos, list, core.node) {
		struct evsel *pos_leader = evsel__leader(pos);

		if (pos == pos_leader)
			num_leaders++;
		pos_leader->core.nr_members++;
	}
	return (idx_changed || num_leaders != orig_num_leaders) ? 1 : 0;
}

int __parse_events(struct evlist *evlist, const char *str, const char *pmu_filter,
		   struct parse_events_error *err, struct perf_pmu *fake_pmu,
		   bool warn_if_reordered)
{
	struct parse_events_state parse_state = {
		.list	  = LIST_HEAD_INIT(parse_state.list),
		.idx	  = evlist->core.nr_entries,
		.error	  = err,
		.stoken	  = PE_START_EVENTS,
		.fake_pmu = fake_pmu,
		.pmu_filter = pmu_filter,
		.match_legacy_cache_terms = true,
	};
	int ret, ret2;

	ret = parse_events__scanner(str, /*input=*/ NULL, &parse_state);

	if (!ret && list_empty(&parse_state.list)) {
		WARN_ONCE(true, "WARNING: event parser found nothing\n");
		return -1;
	}

	ret2 = parse_events__sort_events_and_fix_groups(&parse_state.list);
	if (ret2 < 0)
		return ret;

	if (ret2 && warn_if_reordered && !parse_state.wild_card_pmus)
		pr_warning("WARNING: events were regrouped to match PMUs\n");

	/*
	 * Add list to the evlist even with errors to allow callers to clean up.
	 */
	evlist__splice_list_tail(evlist, &parse_state.list);

	if (!ret) {
		struct evsel *last;

		last = evlist__last(evlist);
		last->cmdline_group_boundary = true;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	}

	/*
	 * There are 2 users - builtin-record and builtin-test objects.
<<<<<<< HEAD
	 * Both call perf_evlist__delete in case of error, so we dont
	 * need to bother.
	 */
	fprintf(stderr, "invalid or unsupported event: '%s'\n", str);
	fprintf(stderr, "Run 'perf list' for a list of valid events\n");
	return ret;
}

int parse_events_option(const struct option *opt, const char *str,
			int unset __used)
{
	struct perf_evlist *evlist = *(struct perf_evlist **)opt->value;
	return parse_events(evlist, str, unset);
}

int parse_filter(const struct option *opt, const char *str,
		 int unset __used)
{
	struct perf_evlist *evlist = *(struct perf_evlist **)opt->value;
	struct perf_evsel *last = NULL;

	if (evlist->nr_entries > 0)
		last = list_entry(evlist->entries.prev, struct perf_evsel, node);

	if (last == NULL || last->attr.type != PERF_TYPE_TRACEPOINT) {
		fprintf(stderr,
			"-F option should follow a -e tracepoint option\n");
		return -1;
	}

	last->filter = strdup(str);
	if (last->filter == NULL) {
		fprintf(stderr, "not enough memory to hold filter string\n");
=======
	 * Both call evlist__delete in case of error, so we dont
	 * need to bother.
	 */
	return ret;
}

int parse_event(struct evlist *evlist, const char *str)
{
	struct parse_events_error err;
	int ret;

	parse_events_error__init(&err);
	ret = parse_events(evlist, str, &err);
	parse_events_error__exit(&err);
	return ret;
}

struct parse_events_error_entry {
	/** @list: The list the error is part of. */
	struct list_head list;
	/** @idx: index in the parsed string */
	int   idx;
	/** @str: string to display at the index */
	char *str;
	/** @help: optional help string */
	char *help;
};

void parse_events_error__init(struct parse_events_error *err)
{
	INIT_LIST_HEAD(&err->list);
}

void parse_events_error__exit(struct parse_events_error *err)
{
	struct parse_events_error_entry *pos, *tmp;

	list_for_each_entry_safe(pos, tmp, &err->list, list) {
		zfree(&pos->str);
		zfree(&pos->help);
		list_del_init(&pos->list);
		free(pos);
	}
}

void parse_events_error__handle(struct parse_events_error *err, int idx,
				char *str, char *help)
{
	struct parse_events_error_entry *entry;

	if (WARN(!str || !err, "WARNING: failed to provide error string or struct\n"))
		goto out_free;

	entry = zalloc(sizeof(*entry));
	if (!entry) {
		pr_err("Failed to allocate memory for event parsing error: %s (%s)\n",
			str, help ?: "<no help>");
		goto out_free;
	}
	entry->idx = idx;
	entry->str = str;
	entry->help = help;
	list_add(&entry->list, &err->list);
	return;
out_free:
	free(str);
	free(help);
}

#define MAX_WIDTH 1000
static int get_term_width(void)
{
	struct winsize ws;

	get_term_dimensions(&ws);
	return ws.ws_col > MAX_WIDTH ? MAX_WIDTH : ws.ws_col;
}

static void __parse_events_error__print(int err_idx, const char *err_str,
					const char *err_help, const char *event)
{
	const char *str = "invalid or unsupported event: ";
	char _buf[MAX_WIDTH];
	char *buf = (char *) event;
	int idx = 0;
	if (err_str) {
		/* -2 for extra '' in the final fprintf */
		int width       = get_term_width() - 2;
		int len_event   = strlen(event);
		int len_str, max_len, cut = 0;

		/*
		 * Maximum error index indent, we will cut
		 * the event string if it's bigger.
		 */
		int max_err_idx = 13;

		/*
		 * Let's be specific with the message when
		 * we have the precise error.
		 */
		str     = "event syntax error: ";
		len_str = strlen(str);
		max_len = width - len_str;

		buf = _buf;

		/* We're cutting from the beginning. */
		if (err_idx > max_err_idx)
			cut = err_idx - max_err_idx;

		strncpy(buf, event + cut, max_len);

		/* Mark cut parts with '..' on both sides. */
		if (cut)
			buf[0] = buf[1] = '.';

		if ((len_event - cut) > max_len) {
			buf[max_len - 1] = buf[max_len - 2] = '.';
			buf[max_len] = 0;
		}

		idx = len_str + err_idx - cut;
	}

	fprintf(stderr, "%s'%s'\n", str, buf);
	if (idx) {
		fprintf(stderr, "%*s\\___ %s\n", idx + 1, "", err_str);
		if (err_help)
			fprintf(stderr, "\n%s\n", err_help);
	}
}

void parse_events_error__print(const struct parse_events_error *err,
			       const char *event)
{
	struct parse_events_error_entry *pos;
	bool first = true;

	list_for_each_entry(pos, &err->list, list) {
		if (!first)
			fputs("\n", stderr);
		__parse_events_error__print(pos->idx, pos->str, pos->help, event);
		first = false;
	}
}

/*
 * In the list of errors err, do any of the error strings (str) contain the
 * given needle string?
 */
bool parse_events_error__contains(const struct parse_events_error *err,
				  const char *needle)
{
	struct parse_events_error_entry *pos;

	list_for_each_entry(pos, &err->list, list) {
		if (strstr(pos->str, needle) != NULL)
			return true;
	}
	return false;
}

#undef MAX_WIDTH

int parse_events_option(const struct option *opt, const char *str,
			int unset __maybe_unused)
{
	struct parse_events_option_args *args = opt->value;
	struct parse_events_error err;
	int ret;

	parse_events_error__init(&err);
	ret = __parse_events(*args->evlistp, str, args->pmu_filter, &err,
			     /*fake_pmu=*/NULL, /*warn_if_reordered=*/true);

	if (ret) {
		parse_events_error__print(&err, str);
		fprintf(stderr, "Run 'perf list' for a list of valid events\n");
	}
	parse_events_error__exit(&err);

	return ret;
}

int parse_events_option_new_evlist(const struct option *opt, const char *str, int unset)
{
	struct parse_events_option_args *args = opt->value;
	int ret;

	if (*args->evlistp == NULL) {
		*args->evlistp = evlist__new();

		if (*args->evlistp == NULL) {
			fprintf(stderr, "Not enough memory to create evlist\n");
			return -1;
		}
	}
	ret = parse_events_option(opt, str, unset);
	if (ret) {
		evlist__delete(*args->evlistp);
		*args->evlistp = NULL;
	}

	return ret;
}

static int
foreach_evsel_in_last_glob(struct evlist *evlist,
			   int (*func)(struct evsel *evsel,
				       const void *arg),
			   const void *arg)
{
	struct evsel *last = NULL;
	int err;

	/*
	 * Don't return when list_empty, give func a chance to report
	 * error when it found last == NULL.
	 *
	 * So no need to WARN here, let *func do this.
	 */
	if (evlist->core.nr_entries > 0)
		last = evlist__last(evlist);

	do {
		err = (*func)(last, arg);
		if (err)
			return -1;
		if (!last)
			return 0;

		if (last->core.node.prev == &evlist->core.entries)
			return 0;
		last = list_entry(last->core.node.prev, struct evsel, core.node);
	} while (!last->cmdline_group_boundary);

	return 0;
}

static int set_filter(struct evsel *evsel, const void *arg)
{
	const char *str = arg;
	bool found = false;
	int nr_addr_filters = 0;
	struct perf_pmu *pmu = NULL;

	if (evsel == NULL) {
		fprintf(stderr,
			"--filter option should follow a -e tracepoint or HW tracer option\n");
		return -1;
	}

	if (evsel->core.attr.type == PERF_TYPE_TRACEPOINT) {
		if (evsel__append_tp_filter(evsel, str) < 0) {
			fprintf(stderr,
				"not enough memory to hold filter string\n");
			return -1;
		}

		return 0;
	}

	while ((pmu = perf_pmus__scan(pmu)) != NULL)
		if (pmu->type == evsel->core.attr.type) {
			found = true;
			break;
		}

	if (found)
		perf_pmu__scan_file(pmu, "nr_addr_filters",
				    "%d", &nr_addr_filters);

	if (!nr_addr_filters)
		return perf_bpf_filter__parse(&evsel->bpf_filters, str);

	if (evsel__append_addr_filter(evsel, str) < 0) {
		fprintf(stderr,
			"not enough memory to hold filter string\n");
		return -1;
	}

	return 0;
}

int parse_filter(const struct option *opt, const char *str,
		 int unset __maybe_unused)
{
	struct evlist *evlist = *(struct evlist **)opt->value;

	return foreach_evsel_in_last_glob(evlist, set_filter,
					  (const void *)str);
}

static int add_exclude_perf_filter(struct evsel *evsel,
				   const void *arg __maybe_unused)
{
	char new_filter[64];

	if (evsel == NULL || evsel->core.attr.type != PERF_TYPE_TRACEPOINT) {
		fprintf(stderr,
			"--exclude-perf option should follow a -e tracepoint option\n");
		return -1;
	}

	snprintf(new_filter, sizeof(new_filter), "common_pid != %d", getpid());

	if (evsel__append_tp_filter(evsel, new_filter) < 0) {
		fprintf(stderr,
			"not enough memory to hold filter string\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -1;
	}

	return 0;
}

<<<<<<< HEAD
static const char * const event_type_descriptors[] = {
	"Hardware event",
	"Software event",
	"Tracepoint event",
	"Hardware cache event",
	"Raw hardware event descriptor",
	"Hardware breakpoint",
};

/*
 * Print the events from <debugfs_mount_point>/tracing/events
 */

void print_tracepoint_events(const char *subsys_glob, const char *event_glob)
{
	DIR *sys_dir, *evt_dir;
	struct dirent *sys_next, *evt_next, sys_dirent, evt_dirent;
	char evt_path[MAXPATHLEN];
	char dir_path[MAXPATHLEN];

	if (debugfs_valid_mountpoint(tracing_events_path))
		return;

	sys_dir = opendir(tracing_events_path);
	if (!sys_dir)
		return;

	for_each_subsystem(sys_dir, sys_dirent, sys_next) {
		if (subsys_glob != NULL && 
		    !strglobmatch(sys_dirent.d_name, subsys_glob))
			continue;

		snprintf(dir_path, MAXPATHLEN, "%s/%s", tracing_events_path,
			 sys_dirent.d_name);
		evt_dir = opendir(dir_path);
		if (!evt_dir)
			continue;

		for_each_event(sys_dirent, evt_dir, evt_dirent, evt_next) {
			if (event_glob != NULL && 
			    !strglobmatch(evt_dirent.d_name, event_glob))
				continue;

			snprintf(evt_path, MAXPATHLEN, "%s:%s",
				 sys_dirent.d_name, evt_dirent.d_name);
			printf("  %-50s [%s]\n", evt_path,
				event_type_descriptors[PERF_TYPE_TRACEPOINT]);
		}
		closedir(evt_dir);
	}
	closedir(sys_dir);
}

/*
 * Check whether event is in <debugfs_mount_point>/tracing/events
 */

int is_valid_tracepoint(const char *event_string)
{
	DIR *sys_dir, *evt_dir;
	struct dirent *sys_next, *evt_next, sys_dirent, evt_dirent;
	char evt_path[MAXPATHLEN];
	char dir_path[MAXPATHLEN];

	if (debugfs_valid_mountpoint(tracing_events_path))
		return 0;

	sys_dir = opendir(tracing_events_path);
	if (!sys_dir)
		return 0;

	for_each_subsystem(sys_dir, sys_dirent, sys_next) {

		snprintf(dir_path, MAXPATHLEN, "%s/%s", tracing_events_path,
			 sys_dirent.d_name);
		evt_dir = opendir(dir_path);
		if (!evt_dir)
			continue;

		for_each_event(sys_dirent, evt_dir, evt_dirent, evt_next) {
			snprintf(evt_path, MAXPATHLEN, "%s:%s",
				 sys_dirent.d_name, evt_dirent.d_name);
			if (!strcmp(evt_path, event_string)) {
				closedir(evt_dir);
				closedir(sys_dir);
				return 1;
			}
		}
		closedir(evt_dir);
	}
	closedir(sys_dir);
	return 0;
}

void print_events_type(u8 type)
{
	struct event_symbol *syms = event_symbols;
	unsigned int i;
	char name[64];

	for (i = 0; i < ARRAY_SIZE(event_symbols); i++, syms++) {
		if (type != syms->type)
			continue;

		if (strlen(syms->alias))
			snprintf(name, sizeof(name),  "%s OR %s",
				 syms->symbol, syms->alias);
		else
			snprintf(name, sizeof(name), "%s", syms->symbol);

		printf("  %-50s [%s]\n", name,
			event_type_descriptors[type]);
	}
}

int print_hwcache_events(const char *event_glob)
{
	unsigned int type, op, i, printed = 0;

	for (type = 0; type < PERF_COUNT_HW_CACHE_MAX; type++) {
		for (op = 0; op < PERF_COUNT_HW_CACHE_OP_MAX; op++) {
			/* skip invalid cache type */
			if (!is_cache_op_valid(type, op))
				continue;

			for (i = 0; i < PERF_COUNT_HW_CACHE_RESULT_MAX; i++) {
				char *name = event_cache_name(type, op, i);

				if (event_glob != NULL && !strglobmatch(name, event_glob))
					continue;

				printf("  %-50s [%s]\n", name,
					event_type_descriptors[PERF_TYPE_HW_CACHE]);
				++printed;
			}
		}
	}

	return printed;
}

/*
 * Print the help text for the event symbols:
 */
void print_events(const char *event_glob)
{
	unsigned int i, type, prev_type = -1, printed = 0, ntypes_printed = 0;
	struct event_symbol *syms = event_symbols;
	char name[MAX_NAME_LEN];

	printf("\n");
	printf("List of pre-defined events (to be used in -e):\n");

	for (i = 0; i < ARRAY_SIZE(event_symbols); i++, syms++) {
		type = syms->type;

		if (type != prev_type && printed) {
			printf("\n");
			printed = 0;
			ntypes_printed++;
		}

		if (event_glob != NULL && 
		    !(strglobmatch(syms->symbol, event_glob) ||
		      (syms->alias && strglobmatch(syms->alias, event_glob))))
			continue;

		if (strlen(syms->alias))
			snprintf(name, MAX_NAME_LEN, "%s OR %s", syms->symbol, syms->alias);
		else
			strncpy(name, syms->symbol, MAX_NAME_LEN);
		printf("  %-50s [%s]\n", name,
			event_type_descriptors[type]);

		prev_type = type;
		++printed;
	}

	if (ntypes_printed) {
		printed = 0;
		printf("\n");
	}
	print_hwcache_events(event_glob);

	if (event_glob != NULL)
		return;

	printf("\n");
	printf("  %-50s [%s]\n",
	       "rNNN",
	       event_type_descriptors[PERF_TYPE_RAW]);
	printf("  %-50s [%s]\n",
	       "cpu/t1=v1[,t2=v2,t3 ...]/modifier",
	       event_type_descriptors[PERF_TYPE_RAW]);
	printf("   (see 'perf list --help' on how to encode it)\n");
	printf("\n");

	printf("  %-50s [%s]\n",
			"mem:<addr>[:access]",
			event_type_descriptors[PERF_TYPE_BREAKPOINT]);
	printf("\n");

	print_tracepoint_events(NULL, NULL);
}

int parse_events__is_hardcoded_term(struct parse_events__term *term)
{
	return term->type <= PARSE_EVENTS__TERM_TYPE_HARDCODED_MAX;
}

int parse_events__new_term(struct parse_events__term **_term, int type,
			   char *config, char *str, long num)
{
	struct parse_events__term *term;

	term = zalloc(sizeof(*term));
	if (!term)
		return -ENOMEM;

	INIT_LIST_HEAD(&term->list);
	term->type = type;
	term->config = config;

	switch (type) {
	case PARSE_EVENTS__TERM_TYPE_CONFIG:
	case PARSE_EVENTS__TERM_TYPE_CONFIG1:
	case PARSE_EVENTS__TERM_TYPE_CONFIG2:
	case PARSE_EVENTS__TERM_TYPE_SAMPLE_PERIOD:
	case PARSE_EVENTS__TERM_TYPE_BRANCH_SAMPLE_TYPE:
=======
int exclude_perf(const struct option *opt,
		 const char *arg __maybe_unused,
		 int unset __maybe_unused)
{
	struct evlist *evlist = *(struct evlist **)opt->value;

	return foreach_evsel_in_last_glob(evlist, add_exclude_perf_filter,
					  NULL);
}

int parse_events__is_hardcoded_term(struct parse_events_term *term)
{
	return term->type_term != PARSE_EVENTS__TERM_TYPE_USER;
}

static int new_term(struct parse_events_term **_term,
		    struct parse_events_term *temp,
		    char *str, u64 num)
{
	struct parse_events_term *term;

	term = malloc(sizeof(*term));
	if (!term)
		return -ENOMEM;

	*term = *temp;
	INIT_LIST_HEAD(&term->list);
	term->weak = false;

	switch (term->type_val) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case PARSE_EVENTS__TERM_TYPE_NUM:
		term->val.num = num;
		break;
	case PARSE_EVENTS__TERM_TYPE_STR:
		term->val.str = str;
		break;
	default:
<<<<<<< HEAD
=======
		free(term);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	}

	*_term = term;
	return 0;
}

<<<<<<< HEAD
void parse_events__free_terms(struct list_head *terms)
{
	struct parse_events__term *term, *h;

	list_for_each_entry_safe(term, h, terms, list)
		free(term);

	free(terms);
}
=======
int parse_events_term__num(struct parse_events_term **term,
			   enum parse_events__term_type type_term,
			   const char *config, u64 num,
			   bool no_value,
			   void *loc_term_, void *loc_val_)
{
	YYLTYPE *loc_term = loc_term_;
	YYLTYPE *loc_val = loc_val_;

	struct parse_events_term temp = {
		.type_val  = PARSE_EVENTS__TERM_TYPE_NUM,
		.type_term = type_term,
		.config    = config ? : strdup(config_term_name(type_term)),
		.no_value  = no_value,
		.err_term  = loc_term ? loc_term->first_column : 0,
		.err_val   = loc_val  ? loc_val->first_column  : 0,
	};

	return new_term(term, &temp, /*str=*/NULL, num);
}

int parse_events_term__str(struct parse_events_term **term,
			   enum parse_events__term_type type_term,
			   char *config, char *str,
			   void *loc_term_, void *loc_val_)
{
	YYLTYPE *loc_term = loc_term_;
	YYLTYPE *loc_val = loc_val_;

	struct parse_events_term temp = {
		.type_val  = PARSE_EVENTS__TERM_TYPE_STR,
		.type_term = type_term,
		.config    = config,
		.err_term  = loc_term ? loc_term->first_column : 0,
		.err_val   = loc_val  ? loc_val->first_column  : 0,
	};

	return new_term(term, &temp, str, /*num=*/0);
}

int parse_events_term__term(struct parse_events_term **term,
			    enum parse_events__term_type term_lhs,
			    enum parse_events__term_type term_rhs,
			    void *loc_term, void *loc_val)
{
	return parse_events_term__str(term, term_lhs, NULL,
				      strdup(config_term_name(term_rhs)),
				      loc_term, loc_val);
}

int parse_events_term__clone(struct parse_events_term **new,
			     struct parse_events_term *term)
{
	char *str;
	struct parse_events_term temp = *term;

	temp.used = false;
	if (term->config) {
		temp.config = strdup(term->config);
		if (!temp.config)
			return -ENOMEM;
	}
	if (term->type_val == PARSE_EVENTS__TERM_TYPE_NUM)
		return new_term(new, &temp, /*str=*/NULL, term->val.num);

	str = strdup(term->val.str);
	if (!str) {
		zfree(&temp.config);
		return -ENOMEM;
	}
	return new_term(new, &temp, str, /*num=*/0);
}

void parse_events_term__delete(struct parse_events_term *term)
{
	if (term->type_val != PARSE_EVENTS__TERM_TYPE_NUM)
		zfree(&term->val.str);

	zfree(&term->config);
	free(term);
}

static int parse_events_terms__copy(const struct parse_events_terms *src,
				    struct parse_events_terms *dest)
{
	struct parse_events_term *term;

	list_for_each_entry (term, &src->terms, list) {
		struct parse_events_term *n;
		int ret;

		ret = parse_events_term__clone(&n, term);
		if (ret)
			return ret;

		list_add_tail(&n->list, &dest->terms);
	}
	return 0;
}

void parse_events_terms__init(struct parse_events_terms *terms)
{
	INIT_LIST_HEAD(&terms->terms);
}

void parse_events_terms__exit(struct parse_events_terms *terms)
{
	struct parse_events_term *term, *h;

	list_for_each_entry_safe(term, h, &terms->terms, list) {
		list_del_init(&term->list);
		parse_events_term__delete(term);
	}
}

void parse_events_terms__delete(struct parse_events_terms *terms)
{
	if (!terms)
		return;
	parse_events_terms__exit(terms);
	free(terms);
}

int parse_events_terms__to_strbuf(const struct parse_events_terms *terms, struct strbuf *sb)
{
	struct parse_events_term *term;
	bool first = true;

	if (!terms)
		return 0;

	list_for_each_entry(term, &terms->terms, list) {
		int ret;

		if (!first) {
			ret = strbuf_addch(sb, ',');
			if (ret < 0)
				return ret;
		}
		first = false;

		if (term->type_val == PARSE_EVENTS__TERM_TYPE_NUM)
			if (term->no_value) {
				assert(term->val.num == 1);
				ret = strbuf_addf(sb, "%s", term->config);
			} else
				ret = strbuf_addf(sb, "%s=%#"PRIx64, term->config, term->val.num);
		else if (term->type_val == PARSE_EVENTS__TERM_TYPE_STR) {
			if (term->config) {
				ret = strbuf_addf(sb, "%s=", term->config);
				if (ret < 0)
					return ret;
			} else if ((unsigned int)term->type_term < __PARSE_EVENTS__TERM_TYPE_NR) {
				ret = strbuf_addf(sb, "%s=", config_term_name(term->type_term));
				if (ret < 0)
					return ret;
			}
			assert(!term->no_value);
			ret = strbuf_addf(sb, "%s", term->val.str);
		}
		if (ret < 0)
			return ret;
	}
	return 0;
}

void parse_events_evlist_error(struct parse_events_state *parse_state,
			       int idx, const char *str)
{
	if (!parse_state->error)
		return;

	parse_events_error__handle(parse_state->error, idx, strdup(str), NULL);
}

static void config_terms_list(char *buf, size_t buf_sz)
{
	int i;
	bool first = true;

	buf[0] = '\0';
	for (i = 0; i < __PARSE_EVENTS__TERM_TYPE_NR; i++) {
		const char *name = config_term_name(i);

		if (!config_term_avail(i, NULL))
			continue;
		if (!name)
			continue;
		if (name[0] == '<')
			continue;

		if (strlen(buf) + strlen(name) + 2 >= buf_sz)
			return;

		if (!first)
			strcat(buf, ",");
		else
			first = false;
		strcat(buf, name);
	}
}

/*
 * Return string contains valid config terms of an event.
 * @additional_terms: For terms such as PMU sysfs terms.
 */
char *parse_events_formats_error_string(char *additional_terms)
{
	char *str;
	/* "no-overwrite" is the longest name */
	char static_terms[__PARSE_EVENTS__TERM_TYPE_NR *
			  (sizeof("no-overwrite") - 1)];

	config_terms_list(static_terms, sizeof(static_terms));
	/* valid terms */
	if (additional_terms) {
		if (asprintf(&str, "valid terms: %s,%s",
			     additional_terms, static_terms) < 0)
			goto fail;
	} else {
		if (asprintf(&str, "valid terms: %s", static_terms) < 0)
			goto fail;
	}
	return str;

fail:
	return NULL;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
