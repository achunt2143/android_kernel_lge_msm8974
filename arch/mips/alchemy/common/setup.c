/*
 * Copyright 2000, 2007-2008 MontaVista Software Inc.
 * Author: MontaVista Software, Inc. <source@mvista.com
 *
 * Updates to 2.6, Pete Popov, Embedded Alley Solutions, Inc.
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 *  THIS  SOFTWARE  IS PROVIDED   ``AS  IS'' AND   ANY  EXPRESS OR IMPLIED
 *  WARRANTIES,   INCLUDING, BUT NOT  LIMITED  TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 *  NO  EVENT  SHALL   THE AUTHOR  BE    LIABLE FOR ANY   DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED   TO, PROCUREMENT OF  SUBSTITUTE GOODS  OR SERVICES; LOSS OF
 *  USE, DATA,  OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  You should have received a copy of the  GNU General Public License along
 *  with this program; if not, write  to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/init.h>
#include <linux/ioport.h>
<<<<<<< HEAD
#include <linux/jiffies.h>
#include <linux/module.h>

#include <asm/mipsregs.h>
#include <asm/time.h>

#include <au1000.h>

extern void __init board_setup(void);
extern void set_cpuspec(void);

void __init plat_mem_setup(void)
{
	unsigned long est_freq;

	/* determine core clock */
	est_freq = au1xxx_calc_clock();
	est_freq += 5000;    /* round */
	est_freq -= est_freq % 10000;
	printk(KERN_INFO "(PRId %08x) @ %lu.%02lu MHz\n", read_c0_prid(),
	       est_freq / 1000000, ((est_freq % 1000000) * 100) / 1000000);

	/* this is faster than wasting cycles trying to approximate it */
	preset_lpj = (est_freq >> 1) / HZ;
=======
#include <linux/mm.h>
#include <linux/dma-map-ops.h> /* for dma_default_coherent */

#include <asm/bootinfo.h>
#include <asm/mipsregs.h>

#include <au1000.h>

static bool alchemy_dma_coherent(void)
{
	switch (alchemy_get_cputype()) {
	case ALCHEMY_CPU_AU1000:
	case ALCHEMY_CPU_AU1500:
	case ALCHEMY_CPU_AU1100:
		return false;
	case ALCHEMY_CPU_AU1200:
		/* Au1200 AB USB does not support coherent memory */
		if ((read_c0_prid() & PRID_REV_MASK) == 0)
			return false;
		return true;
	default:
		return true;
	}
}

void __init plat_mem_setup(void)
{
	alchemy_set_lpj();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (au1xxx_cpu_needs_config_od())
		/* Various early Au1xx0 errata corrected by this */
		set_c0_config(1 << 19); /* Set Config[OD] */
	else
		/* Clear to obtain best system bus performance */
		clear_c0_config(1 << 19); /* Clear Config[OD] */

<<<<<<< HEAD
	board_setup();  /* board specific setup */
=======
	dma_default_coherent = alchemy_dma_coherent();

	board_setup();	/* board specific setup */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* IO/MEM resources. */
	set_io_port_base(0);
	ioport_resource.start = IOPORT_RESOURCE_START;
	ioport_resource.end = IOPORT_RESOURCE_END;
	iomem_resource.start = IOMEM_RESOURCE_START;
	iomem_resource.end = IOMEM_RESOURCE_END;
}

<<<<<<< HEAD
#if defined(CONFIG_64BIT_PHYS_ADDR) && defined(CONFIG_PCI)
/* This routine should be valid for all Au1x based boards */
phys_t __fixup_bigphys_addr(phys_t phys_addr, phys_t size)
=======
#ifdef CONFIG_MIPS_FIXUP_BIGPHYS_ADDR
/* This routine should be valid for all Au1x based boards */
phys_addr_t fixup_bigphys_addr(phys_addr_t phys_addr, phys_addr_t size)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long start = ALCHEMY_PCI_MEMWIN_START;
	unsigned long end = ALCHEMY_PCI_MEMWIN_END;

	/* Don't fixup 36-bit addresses */
	if ((phys_addr >> 32) != 0)
		return phys_addr;

	/* Check for PCI memory window */
	if (phys_addr >= start && (phys_addr + size - 1) <= end)
<<<<<<< HEAD
		return (phys_t)(AU1500_PCI_MEM_PHYS_ADDR + phys_addr);
=======
		return (phys_addr_t)(AU1500_PCI_MEM_PHYS_ADDR + phys_addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* default nop */
	return phys_addr;
}
<<<<<<< HEAD
EXPORT_SYMBOL(__fixup_bigphys_addr);
#endif
=======

int io_remap_pfn_range(struct vm_area_struct *vma, unsigned long vaddr,
		unsigned long pfn, unsigned long size, pgprot_t prot)
{
	phys_addr_t phys_addr = fixup_bigphys_addr(pfn << PAGE_SHIFT, size);

	return remap_pfn_range(vma, vaddr, phys_addr >> PAGE_SHIFT, size, prot);
}
EXPORT_SYMBOL(io_remap_pfn_range);
#endif /* CONFIG_MIPS_FIXUP_BIGPHYS_ADDR */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
