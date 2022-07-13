<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __PERF_LEVENSHTEIN_H
#define __PERF_LEVENSHTEIN_H

int levenshtein(const char *string1, const char *string2,
	int swap_penalty, int substition_penalty,
	int insertion_penalty, int deletion_penalty);

#endif /* __PERF_LEVENSHTEIN_H */
