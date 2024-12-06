<<<<<<< HEAD
#include <stdlib.h>

#include "util.h"
#include "values.h"

void perf_read_values_init(struct perf_read_values *values)
=======
// SPDX-License-Identifier: GPL-2.0
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <linux/zalloc.h>

#include "values.h"
#include "debug.h"

int perf_read_values_init(struct perf_read_values *values)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	values->threads_max = 16;
	values->pid = malloc(values->threads_max * sizeof(*values->pid));
	values->tid = malloc(values->threads_max * sizeof(*values->tid));
<<<<<<< HEAD
	values->value = malloc(values->threads_max * sizeof(*values->value));
	if (!values->pid || !values->tid || !values->value)
		die("failed to allocate read_values threads arrays");
=======
	values->value = zalloc(values->threads_max * sizeof(*values->value));
	if (!values->pid || !values->tid || !values->value) {
		pr_debug("failed to allocate read_values threads arrays");
		goto out_free_pid;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	values->threads = 0;

	values->counters_max = 16;
	values->counterrawid = malloc(values->counters_max
				      * sizeof(*values->counterrawid));
	values->countername = malloc(values->counters_max
				     * sizeof(*values->countername));
<<<<<<< HEAD
	if (!values->counterrawid || !values->countername)
		die("failed to allocate read_values counters arrays");
	values->counters = 0;
=======
	if (!values->counterrawid || !values->countername) {
		pr_debug("failed to allocate read_values counters arrays");
		goto out_free_counter;
	}
	values->counters = 0;

	return 0;

out_free_counter:
	zfree(&values->counterrawid);
	zfree(&values->countername);
out_free_pid:
	zfree(&values->pid);
	zfree(&values->tid);
	zfree(&values->value);
	return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void perf_read_values_destroy(struct perf_read_values *values)
{
	int i;

	if (!values->threads_max || !values->counters_max)
		return;

	for (i = 0; i < values->threads; i++)
<<<<<<< HEAD
		free(values->value[i]);
	free(values->value);
	free(values->pid);
	free(values->tid);
	free(values->counterrawid);
	for (i = 0; i < values->counters; i++)
		free(values->countername[i]);
	free(values->countername);
}

static void perf_read_values__enlarge_threads(struct perf_read_values *values)
{
	values->threads_max *= 2;
	values->pid = realloc(values->pid,
			      values->threads_max * sizeof(*values->pid));
	values->tid = realloc(values->tid,
			      values->threads_max * sizeof(*values->tid));
	values->value = realloc(values->value,
				values->threads_max * sizeof(*values->value));
	if (!values->pid || !values->tid || !values->value)
		die("failed to enlarge read_values threads arrays");
=======
		zfree(&values->value[i]);
	zfree(&values->value);
	zfree(&values->pid);
	zfree(&values->tid);
	zfree(&values->counterrawid);
	for (i = 0; i < values->counters; i++)
		zfree(&values->countername[i]);
	zfree(&values->countername);
}

static int perf_read_values__enlarge_threads(struct perf_read_values *values)
{
	int nthreads_max = values->threads_max * 2;
	void *npid = realloc(values->pid, nthreads_max * sizeof(*values->pid)),
	     *ntid = realloc(values->tid, nthreads_max * sizeof(*values->tid)),
	     *nvalue = realloc(values->value, nthreads_max * sizeof(*values->value));

	if (!npid || !ntid || !nvalue)
		goto out_err;

	values->threads_max = nthreads_max;
	values->pid = npid;
	values->tid = ntid;
	values->value = nvalue;
	return 0;
out_err:
	free(npid);
	free(ntid);
	free(nvalue);
	pr_debug("failed to enlarge read_values threads arrays");
	return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int perf_read_values__findnew_thread(struct perf_read_values *values,
					    u32 pid, u32 tid)
{
	int i;

	for (i = 0; i < values->threads; i++)
		if (values->pid[i] == pid && values->tid[i] == tid)
			return i;

<<<<<<< HEAD
	if (values->threads == values->threads_max)
		perf_read_values__enlarge_threads(values);

	i = values->threads++;
	values->pid[i] = pid;
	values->tid[i] = tid;
	values->value[i] = malloc(values->counters_max * sizeof(**values->value));
	if (!values->value[i])
		die("failed to allocate read_values counters array");
=======
	if (values->threads == values->threads_max) {
		i = perf_read_values__enlarge_threads(values);
		if (i < 0)
			return i;
	}

	i = values->threads;

	values->value[i] = zalloc(values->counters_max * sizeof(**values->value));
	if (!values->value[i]) {
		pr_debug("failed to allocate read_values counters array");
		return -ENOMEM;
	}
	values->pid[i] = pid;
	values->tid[i] = tid;
	values->threads = i + 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return i;
}

<<<<<<< HEAD
static void perf_read_values__enlarge_counters(struct perf_read_values *values)
{
	int i;

	values->counters_max *= 2;
	values->counterrawid = realloc(values->counterrawid,
				       values->counters_max * sizeof(*values->counterrawid));
	values->countername = realloc(values->countername,
				      values->counters_max * sizeof(*values->countername));
	if (!values->counterrawid || !values->countername)
		die("failed to enlarge read_values counters arrays");

	for (i = 0; i < values->threads; i++) {
		values->value[i] = realloc(values->value[i],
					   values->counters_max * sizeof(**values->value));
		if (!values->value[i])
			die("failed to enlarge read_values counters arrays");
	}
=======
static int perf_read_values__enlarge_counters(struct perf_read_values *values)
{
	char **countername;
	int i, counters_max = values->counters_max * 2;
	u64 *counterrawid = realloc(values->counterrawid, counters_max * sizeof(*values->counterrawid));

	if (!counterrawid) {
		pr_debug("failed to enlarge read_values rawid array");
		goto out_enomem;
	}

	countername = realloc(values->countername, counters_max * sizeof(*values->countername));
	if (!countername) {
		pr_debug("failed to enlarge read_values rawid array");
		goto out_free_rawid;
	}

	for (i = 0; i < values->threads; i++) {
		u64 *value = realloc(values->value[i], counters_max * sizeof(**values->value));
		int j;

		if (!value) {
			pr_debug("failed to enlarge read_values ->values array");
			goto out_free_name;
		}

		for (j = values->counters_max; j < counters_max; j++)
			value[j] = 0;

		values->value[i] = value;
	}

	values->counters_max = counters_max;
	values->counterrawid = counterrawid;
	values->countername  = countername;

	return 0;
out_free_name:
	free(countername);
out_free_rawid:
	free(counterrawid);
out_enomem:
	return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int perf_read_values__findnew_counter(struct perf_read_values *values,
					     u64 rawid, const char *name)
{
	int i;

	for (i = 0; i < values->counters; i++)
		if (values->counterrawid[i] == rawid)
			return i;

<<<<<<< HEAD
	if (values->counters == values->counters_max)
		perf_read_values__enlarge_counters(values);
=======
	if (values->counters == values->counters_max) {
		i = perf_read_values__enlarge_counters(values);
		if (i)
			return i;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	i = values->counters++;
	values->counterrawid[i] = rawid;
	values->countername[i] = strdup(name);

	return i;
}

<<<<<<< HEAD
void perf_read_values_add_value(struct perf_read_values *values,
=======
int perf_read_values_add_value(struct perf_read_values *values,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				u32 pid, u32 tid,
				u64 rawid, const char *name, u64 value)
{
	int tindex, cindex;

	tindex = perf_read_values__findnew_thread(values, pid, tid);
<<<<<<< HEAD
	cindex = perf_read_values__findnew_counter(values, rawid, name);

	values->value[tindex][cindex] = value;
=======
	if (tindex < 0)
		return tindex;
	cindex = perf_read_values__findnew_counter(values, rawid, name);
	if (cindex < 0)
		return cindex;

	values->value[tindex][cindex] += value;
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void perf_read_values__display_pretty(FILE *fp,
					     struct perf_read_values *values)
{
	int i, j;
	int pidwidth, tidwidth;
	int *counterwidth;

	counterwidth = malloc(values->counters * sizeof(*counterwidth));
<<<<<<< HEAD
	if (!counterwidth)
		die("failed to allocate counterwidth array");
=======
	if (!counterwidth) {
		fprintf(fp, "INTERNAL ERROR: Failed to allocate counterwidth array\n");
		return;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	tidwidth = 3;
	pidwidth = 3;
	for (j = 0; j < values->counters; j++)
		counterwidth[j] = strlen(values->countername[j]);
	for (i = 0; i < values->threads; i++) {
		int width;

		width = snprintf(NULL, 0, "%d", values->pid[i]);
		if (width > pidwidth)
			pidwidth = width;
		width = snprintf(NULL, 0, "%d", values->tid[i]);
		if (width > tidwidth)
			tidwidth = width;
		for (j = 0; j < values->counters; j++) {
			width = snprintf(NULL, 0, "%" PRIu64, values->value[i][j]);
			if (width > counterwidth[j])
				counterwidth[j] = width;
		}
	}

	fprintf(fp, "# %*s  %*s", pidwidth, "PID", tidwidth, "TID");
	for (j = 0; j < values->counters; j++)
		fprintf(fp, "  %*s", counterwidth[j], values->countername[j]);
	fprintf(fp, "\n");

	for (i = 0; i < values->threads; i++) {
		fprintf(fp, "  %*d  %*d", pidwidth, values->pid[i],
			tidwidth, values->tid[i]);
		for (j = 0; j < values->counters; j++)
			fprintf(fp, "  %*" PRIu64,
				counterwidth[j], values->value[i][j]);
		fprintf(fp, "\n");
	}
	free(counterwidth);
}

static void perf_read_values__display_raw(FILE *fp,
					  struct perf_read_values *values)
{
	int width, pidwidth, tidwidth, namewidth, rawwidth, countwidth;
	int i, j;

	tidwidth = 3; /* TID */
	pidwidth = 3; /* PID */
	namewidth = 4; /* "Name" */
	rawwidth = 3; /* "Raw" */
	countwidth = 5; /* "Count" */

	for (i = 0; i < values->threads; i++) {
		width = snprintf(NULL, 0, "%d", values->pid[i]);
		if (width > pidwidth)
			pidwidth = width;
		width = snprintf(NULL, 0, "%d", values->tid[i]);
		if (width > tidwidth)
			tidwidth = width;
	}
	for (j = 0; j < values->counters; j++) {
		width = strlen(values->countername[j]);
		if (width > namewidth)
			namewidth = width;
		width = snprintf(NULL, 0, "%" PRIx64, values->counterrawid[j]);
		if (width > rawwidth)
			rawwidth = width;
	}
	for (i = 0; i < values->threads; i++) {
		for (j = 0; j < values->counters; j++) {
			width = snprintf(NULL, 0, "%" PRIu64, values->value[i][j]);
			if (width > countwidth)
				countwidth = width;
		}
	}

	fprintf(fp, "# %*s  %*s  %*s  %*s  %*s\n",
		pidwidth, "PID", tidwidth, "TID",
		namewidth, "Name", rawwidth, "Raw",
		countwidth, "Count");
	for (i = 0; i < values->threads; i++)
		for (j = 0; j < values->counters; j++)
			fprintf(fp, "  %*d  %*d  %*s  %*" PRIx64 "  %*" PRIu64,
				pidwidth, values->pid[i],
				tidwidth, values->tid[i],
				namewidth, values->countername[j],
				rawwidth, values->counterrawid[j],
				countwidth, values->value[i][j]);
}

void perf_read_values_display(FILE *fp, struct perf_read_values *values, int raw)
{
	if (raw)
		perf_read_values__display_raw(fp, values);
	else
		perf_read_values__display_pretty(fp, values);
}
