<<<<<<< HEAD
/******************************************************************************
 * Xen balloon functionality
 */
=======
/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 * Xen balloon functionality
 */
#ifndef _XEN_BALLOON_H
#define _XEN_BALLOON_H
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define RETRY_UNLIMITED	0

struct balloon_stats {
	/* We aim for 'current allocation' == 'target allocation'. */
	unsigned long current_pages;
	unsigned long target_pages;
<<<<<<< HEAD
	/* Number of pages in high- and low-memory balloons. */
	unsigned long balloon_low;
	unsigned long balloon_high;
=======
	unsigned long target_unpopulated;
	/* Number of pages in high- and low-memory balloons. */
	unsigned long balloon_low;
	unsigned long balloon_high;
	unsigned long total_pages;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long schedule_delay;
	unsigned long max_schedule_delay;
	unsigned long retry_count;
	unsigned long max_retry_count;
<<<<<<< HEAD
#ifdef CONFIG_XEN_BALLOON_MEMORY_HOTPLUG
	unsigned long hotplug_pages;
	unsigned long balloon_hotplug;
#endif
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

extern struct balloon_stats balloon_stats;

void balloon_set_new_target(unsigned long target);

<<<<<<< HEAD
int alloc_xenballooned_pages(int nr_pages, struct page **pages,
		bool highmem);
void free_xenballooned_pages(int nr_pages, struct page **pages);

struct device;
#ifdef CONFIG_XEN_SELFBALLOONING
extern int register_xen_selfballooning(struct device *dev);
#else
static inline int register_xen_selfballooning(struct device *dev)
{
	return -ENOSYS;
}
#endif
=======
int xen_alloc_ballooned_pages(unsigned int nr_pages, struct page **pages);
void xen_free_ballooned_pages(unsigned int nr_pages, struct page **pages);

#ifdef CONFIG_XEN_BALLOON
void xen_balloon_init(void);
#else
static inline void xen_balloon_init(void)
{
}
#endif

#endif	/* _XEN_BALLOON_H */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
