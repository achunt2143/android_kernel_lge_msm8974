<<<<<<< HEAD
#ifdef CONFIG_ATAGS_PROC
extern void save_atags(struct tag *tags);
#else
static inline void save_atags(struct tag *tags) { }
=======
/* SPDX-License-Identifier: GPL-2.0 */
void convert_to_tag_list(struct tag *tags);

#ifdef CONFIG_ATAGS
const struct machine_desc *setup_machine_tags(void *__atags_vaddr,
	unsigned int machine_nr);
#else
static inline const struct machine_desc * __init __noreturn
setup_machine_tags(void *__atags_vaddr, unsigned int machine_nr)
{
	early_print("no ATAGS support: can't continue\n");
	while (true);
	unreachable();
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
