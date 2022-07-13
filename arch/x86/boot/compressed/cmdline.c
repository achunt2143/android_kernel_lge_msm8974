<<<<<<< HEAD
#include "misc.h"

=======
// SPDX-License-Identifier: GPL-2.0
#include "misc.h"

#include <asm/bootparam.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static unsigned long fs;
static inline void set_fs(unsigned long seg)
{
	fs = seg << 4;  /* shift it back */
}
typedef unsigned long addr_t;
static inline char rdfs8(addr_t addr)
{
	return *((char *)(fs + addr));
}
#include "../cmdline.c"
<<<<<<< HEAD
int cmdline_find_option(const char *option, char *buffer, int bufsize)
{
	return __cmdline_find_option(real_mode->hdr.cmd_line_ptr, option, buffer, bufsize);
}
int cmdline_find_option_bool(const char *option)
{
	return __cmdline_find_option_bool(real_mode->hdr.cmd_line_ptr, option);
=======
unsigned long get_cmd_line_ptr(void)
{
	unsigned long cmd_line_ptr = boot_params_ptr->hdr.cmd_line_ptr;

	cmd_line_ptr |= (u64)boot_params_ptr->ext_cmd_line_ptr << 32;

	return cmd_line_ptr;
}
int cmdline_find_option(const char *option, char *buffer, int bufsize)
{
	return __cmdline_find_option(get_cmd_line_ptr(), option, buffer, bufsize);
}
int cmdline_find_option_bool(const char *option)
{
	return __cmdline_find_option_bool(get_cmd_line_ptr(), option);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
