<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#undef DEBUG

#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/ioport.h>
#include <linux/etherdevice.h>
#include <linux/of_address.h>
#include <asm/prom.h>

<<<<<<< HEAD
void of_parse_dma_window(struct device_node *dn, const void *dma_window_prop,
		unsigned long *busno, unsigned long *phys, unsigned long *size)
{
	const u32 *dma_window;
	u32 cells;
	const unsigned char *prop;

	dma_window = dma_window_prop;

	/* busno is always one cell */
	*busno = *(dma_window++);
=======
void of_parse_dma_window(struct device_node *dn, const __be32 *dma_window,
			 unsigned long *busno, unsigned long *phys,
			 unsigned long *size)
{
	u32 cells;
	const __be32 *prop;

	/* busno is always one cell */
	*busno = of_read_number(dma_window, 1);
	dma_window++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	prop = of_get_property(dn, "ibm,#dma-address-cells", NULL);
	if (!prop)
		prop = of_get_property(dn, "#address-cells", NULL);

<<<<<<< HEAD
	cells = prop ? *(u32 *)prop : of_n_addr_cells(dn);
=======
	cells = prop ? of_read_number(prop, 1) : of_n_addr_cells(dn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	*phys = of_read_number(dma_window, cells);

	dma_window += cells;

	prop = of_get_property(dn, "ibm,#dma-size-cells", NULL);
<<<<<<< HEAD
	cells = prop ? *(u32 *)prop : of_n_size_cells(dn);
=======
	cells = prop ? of_read_number(prop, 1) : of_n_size_cells(dn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	*size = of_read_number(dma_window, cells);
}
