<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef BUILTIN_H
#define BUILTIN_H

extern int cmd_set(int argc, const char **argv);
extern int cmd_info(int argc, const char **argv);
extern int cmd_freq_set(int argc, const char **argv);
extern int cmd_freq_info(int argc, const char **argv);
<<<<<<< HEAD
extern int cmd_idle_info(int argc, const char **argv);
=======
extern int cmd_idle_set(int argc, const char **argv);
extern int cmd_idle_info(int argc, const char **argv);
extern int cmd_cap_info(int argc, const char **argv);
extern int cmd_cap_set(int argc, const char **argv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern int cmd_monitor(int argc, const char **argv);

#endif
