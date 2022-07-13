<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __PERF_THREAD_MAP_H
#define __PERF_THREAD_MAP_H

#include <sys/types.h>
#include <stdio.h>
<<<<<<< HEAD

struct thread_map {
	int nr;
	int map[];
};

struct thread_map *thread_map__new_by_pid(pid_t pid);
struct thread_map *thread_map__new_by_tid(pid_t tid);
struct thread_map *thread_map__new_by_uid(uid_t uid);
struct thread_map *thread_map__new(pid_t pid, pid_t tid, uid_t uid);

struct thread_map *thread_map__new_str(const char *pid,
		const char *tid, uid_t uid);

void thread_map__delete(struct thread_map *threads);

size_t thread_map__fprintf(struct thread_map *threads, FILE *fp);

=======
#include <perf/threadmap.h>

struct perf_record_thread_map;

struct perf_thread_map *thread_map__new_dummy(void);
struct perf_thread_map *thread_map__new_by_pid(pid_t pid);
struct perf_thread_map *thread_map__new_by_tid(pid_t tid);
struct perf_thread_map *thread_map__new_by_uid(uid_t uid);
struct perf_thread_map *thread_map__new_all_cpus(void);
struct perf_thread_map *thread_map__new(pid_t pid, pid_t tid, uid_t uid);
struct perf_thread_map *thread_map__new_event(struct perf_record_thread_map *event);

struct perf_thread_map *thread_map__new_str(const char *pid,
		const char *tid, uid_t uid, bool all_threads);

struct perf_thread_map *thread_map__new_by_tid_str(const char *tid_str);

size_t thread_map__fprintf(struct perf_thread_map *threads, FILE *fp);

void thread_map__read_comms(struct perf_thread_map *threads);
bool thread_map__has(struct perf_thread_map *threads, pid_t pid);
int thread_map__remove(struct perf_thread_map *threads, int idx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif	/* __PERF_THREAD_MAP_H */
