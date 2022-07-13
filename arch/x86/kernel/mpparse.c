<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	Intel Multiprocessor Specification 1.1 and 1.4
 *	compliant MP-table parsing routines.
 *
 *	(c) 1995 Alan Cox, Building #3 <alan@lxorguk.ukuu.org.uk>
 *	(c) 1998, 1999, 2000, 2009 Ingo Molnar <mingo@redhat.com>
 *      (c) 2008 Alexey Starikovskiy <astarikovskiy@suse.de>
 */

#include <linux/mm.h>
#include <linux/init.h>
#include <linux/delay.h>
<<<<<<< HEAD
#include <linux/bootmem.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/memblock.h>
#include <linux/kernel_stat.h>
#include <linux/mc146818rtc.h>
#include <linux/bitops.h>
#include <linux/acpi.h>
<<<<<<< HEAD
#include <linux/module.h>
#include <linux/smp.h>
#include <linux/pci.h>

#include <asm/mtrr.h>
#include <asm/mpspec.h>
#include <asm/pgalloc.h>
#include <asm/io_apic.h>
#include <asm/proto.h>
#include <asm/bios_ebda.h>
#include <asm/e820.h>
#include <asm/trampoline.h>
=======
#include <linux/smp.h>
#include <linux/pci.h>

#include <asm/i8259.h>
#include <asm/io_apic.h>
#include <asm/acpi.h>
#include <asm/irqdomain.h>
#include <asm/mtrr.h>
#include <asm/mpspec.h>
#include <asm/proto.h>
#include <asm/bios_ebda.h>
#include <asm/e820/api.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/setup.h>
#include <asm/smp.h>

#include <asm/apic.h>
/*
 * Checksum an MP configuration block.
 */

<<<<<<< HEAD
=======
static unsigned int num_procs __initdata;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int __init mpf_checksum(unsigned char *mp, int len)
{
	int sum = 0;

	while (len--)
		sum += *mp++;

	return sum & 0xFF;
}

<<<<<<< HEAD
int __init default_mpc_apic_id(struct mpc_cpu *m)
{
	return m->apicid;
}

static void __init MP_processor_info(struct mpc_cpu *m)
{
	int apicid;
	char *bootup_cpu = "";

	if (!(m->cpuflag & CPU_ENABLED)) {
		disabled_cpus++;
		return;
	}

	apicid = x86_init.mpparse.mpc_apic_id(m);

	if (m->cpuflag & CPU_BOOTPROCESSOR) {
		bootup_cpu = " (Bootup-CPU)";
		boot_cpu_physical_apicid = m->apicid;
	}

	printk(KERN_INFO "Processor #%d%s\n", m->apicid, bootup_cpu);
	generic_processor_info(apicid, m->apicver);
}

#ifdef CONFIG_X86_IO_APIC
void __init default_mpc_oem_bus_info(struct mpc_bus *m, char *str)
=======
static void __init MP_processor_info(struct mpc_cpu *m)
{
	char *bootup_cpu = "";

	topology_register_apic(m->apicid, CPU_ACPIID_INVALID, m->cpuflag & CPU_ENABLED);
	if (!(m->cpuflag & CPU_ENABLED))
		return;

	if (m->cpuflag & CPU_BOOTPROCESSOR)
		bootup_cpu = " (Bootup-CPU)";

	pr_info("Processor #%d%s\n", m->apicid, bootup_cpu);
	num_procs++;
}

#ifdef CONFIG_X86_IO_APIC
static void __init mpc_oem_bus_info(struct mpc_bus *m, char *str)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	memcpy(str, m->bustype, 6);
	str[6] = 0;
	apic_printk(APIC_VERBOSE, "Bus #%d is %s\n", m->busid, str);
}

static void __init MP_bus_info(struct mpc_bus *m)
{
	char str[7];

<<<<<<< HEAD
	x86_init.mpparse.mpc_oem_bus_info(m, str);

#if MAX_MP_BUSSES < 256
	if (m->busid >= MAX_MP_BUSSES) {
		printk(KERN_WARNING "MP table busid value (%d) for bustype %s "
		       " is too large, max. supported is %d\n",
		       m->busid, str, MAX_MP_BUSSES - 1);
=======
	mpc_oem_bus_info(m, str);

#if MAX_MP_BUSSES < 256
	if (m->busid >= MAX_MP_BUSSES) {
		pr_warn("MP table busid value (%d) for bustype %s is too large, max. supported is %d\n",
			m->busid, str, MAX_MP_BUSSES - 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	}
#endif

	set_bit(m->busid, mp_bus_not_pci);
	if (strncmp(str, BUSTYPE_ISA, sizeof(BUSTYPE_ISA) - 1) == 0) {
<<<<<<< HEAD
#if defined(CONFIG_EISA) || defined(CONFIG_MCA)
		mp_bus_id_to_type[m->busid] = MP_BUS_ISA;
#endif
	} else if (strncmp(str, BUSTYPE_PCI, sizeof(BUSTYPE_PCI) - 1) == 0) {
		if (x86_init.mpparse.mpc_oem_pci_bus)
			x86_init.mpparse.mpc_oem_pci_bus(m);

		clear_bit(m->busid, mp_bus_not_pci);
#if defined(CONFIG_EISA) || defined(CONFIG_MCA)
		mp_bus_id_to_type[m->busid] = MP_BUS_PCI;
	} else if (strncmp(str, BUSTYPE_EISA, sizeof(BUSTYPE_EISA) - 1) == 0) {
		mp_bus_id_to_type[m->busid] = MP_BUS_EISA;
	} else if (strncmp(str, BUSTYPE_MCA, sizeof(BUSTYPE_MCA) - 1) == 0) {
		mp_bus_id_to_type[m->busid] = MP_BUS_MCA;
#endif
	} else
		printk(KERN_WARNING "Unknown bustype %s - ignoring\n", str);
=======
#ifdef CONFIG_EISA
		mp_bus_id_to_type[m->busid] = MP_BUS_ISA;
#endif
	} else if (strncmp(str, BUSTYPE_PCI, sizeof(BUSTYPE_PCI) - 1) == 0) {
		clear_bit(m->busid, mp_bus_not_pci);
#ifdef CONFIG_EISA
		mp_bus_id_to_type[m->busid] = MP_BUS_PCI;
	} else if (strncmp(str, BUSTYPE_EISA, sizeof(BUSTYPE_EISA) - 1) == 0) {
		mp_bus_id_to_type[m->busid] = MP_BUS_EISA;
#endif
	} else
		pr_warn("Unknown bustype %s - ignoring\n", str);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __init MP_ioapic_info(struct mpc_ioapic *m)
{
<<<<<<< HEAD
	if (m->flags & MPC_APIC_USABLE)
		mp_register_ioapic(m->apicid, m->apicaddr, gsi_top);
=======
	struct ioapic_domain_cfg cfg = {
		.type = IOAPIC_DOMAIN_LEGACY,
		.ops = &mp_ioapic_irqdomain_ops,
	};

	if (m->flags & MPC_APIC_USABLE)
		mp_register_ioapic(m->apicid, m->apicaddr, gsi_top, &cfg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __init print_mp_irq_info(struct mpc_intsrc *mp_irq)
{
<<<<<<< HEAD
	apic_printk(APIC_VERBOSE, "Int: type %d, pol %d, trig %d, bus %02x,"
		" IRQ %02x, APIC ID %x, APIC INT %02x\n",
=======
	apic_printk(APIC_VERBOSE,
		"Int: type %d, pol %d, trig %d, bus %02x, IRQ %02x, APIC ID %x, APIC INT %02x\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mp_irq->irqtype, mp_irq->irqflag & 3,
		(mp_irq->irqflag >> 2) & 3, mp_irq->srcbus,
		mp_irq->srcbusirq, mp_irq->dstapic, mp_irq->dstirq);
}

#else /* CONFIG_X86_IO_APIC */
static inline void __init MP_bus_info(struct mpc_bus *m) {}
static inline void __init MP_ioapic_info(struct mpc_ioapic *m) {}
#endif /* CONFIG_X86_IO_APIC */

static void __init MP_lintsrc_info(struct mpc_lintsrc *m)
{
<<<<<<< HEAD
	apic_printk(APIC_VERBOSE, "Lint: type %d, pol %d, trig %d, bus %02x,"
		" IRQ %02x, APIC ID %x, APIC LINT %02x\n",
=======
	apic_printk(APIC_VERBOSE,
		"Lint: type %d, pol %d, trig %d, bus %02x, IRQ %02x, APIC ID %x, APIC LINT %02x\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		m->irqtype, m->irqflag & 3, (m->irqflag >> 2) & 3, m->srcbusid,
		m->srcbusirq, m->destapic, m->destapiclint);
}

/*
 * Read/parse the MPC
 */
static int __init smp_check_mpc(struct mpc_table *mpc, char *oem, char *str)
{

	if (memcmp(mpc->signature, MPC_SIGNATURE, 4)) {
<<<<<<< HEAD
		printk(KERN_ERR "MPTABLE: bad signature [%c%c%c%c]!\n",
=======
		pr_err("MPTABLE: bad signature [%c%c%c%c]!\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       mpc->signature[0], mpc->signature[1],
		       mpc->signature[2], mpc->signature[3]);
		return 0;
	}
	if (mpf_checksum((unsigned char *)mpc, mpc->length)) {
<<<<<<< HEAD
		printk(KERN_ERR "MPTABLE: checksum error!\n");
		return 0;
	}
	if (mpc->spec != 0x01 && mpc->spec != 0x04) {
		printk(KERN_ERR "MPTABLE: bad table version (%d)!!\n",
		       mpc->spec);
		return 0;
	}
	if (!mpc->lapic) {
		printk(KERN_ERR "MPTABLE: null local APIC address!\n");
=======
		pr_err("MPTABLE: checksum error!\n");
		return 0;
	}
	if (mpc->spec != 0x01 && mpc->spec != 0x04) {
		pr_err("MPTABLE: bad table version (%d)!!\n", mpc->spec);
		return 0;
	}
	if (!mpc->lapic) {
		pr_err("MPTABLE: null local APIC address!\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	}
	memcpy(oem, mpc->oem, 8);
	oem[8] = 0;
<<<<<<< HEAD
	printk(KERN_INFO "MPTABLE: OEM ID: %s\n", oem);
=======
	pr_info("MPTABLE: OEM ID: %s\n", oem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	memcpy(str, mpc->productid, 12);
	str[12] = 0;

<<<<<<< HEAD
	printk(KERN_INFO "MPTABLE: Product ID: %s\n", str);

	printk(KERN_INFO "MPTABLE: APIC at: 0x%X\n", mpc->lapic);
=======
	pr_info("MPTABLE: Product ID: %s\n", str);

	pr_info("MPTABLE: APIC at: 0x%X\n", mpc->lapic);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 1;
}

static void skip_entry(unsigned char **ptr, int *count, int size)
{
	*ptr += size;
	*count += size;
}

static void __init smp_dump_mptable(struct mpc_table *mpc, unsigned char *mpt)
{
<<<<<<< HEAD
	printk(KERN_ERR "Your mptable is wrong, contact your HW vendor!\n"
		"type %x\n", *mpt);
=======
	pr_err("Your mptable is wrong, contact your HW vendor!\n");
	pr_cont("type %x\n", *mpt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	print_hex_dump(KERN_ERR, "  ", DUMP_PREFIX_ADDRESS, 16,
			1, mpc, mpc->length, 1);
}

<<<<<<< HEAD
void __init default_smp_read_mpc_oem(struct mpc_table *mpc) { }

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int __init smp_read_mpc(struct mpc_table *mpc, unsigned early)
{
	char str[16];
	char oem[10];

	int count = sizeof(*mpc);
	unsigned char *mpt = ((unsigned char *)mpc) + count;

	if (!smp_check_mpc(mpc, oem, str))
		return 0;

<<<<<<< HEAD
#ifdef CONFIG_X86_32
	generic_mps_oem_check(mpc, oem, str);
#endif
	/* Initialize the lapic mapping */
	if (!acpi_lapic)
		register_lapic_address(mpc->lapic);

	if (early)
		return 1;

	if (mpc->oemptr)
		x86_init.mpparse.smp_read_mpc_oem(mpc);

	/*
	 *      Now process the configuration blocks.
	 */
	x86_init.mpparse.mpc_record(0);

=======
	if (early) {
		/* Initialize the lapic mapping */
		if (!acpi_lapic)
			register_lapic_address(mpc->lapic);
		return 1;
	}

	/* Now process the configuration blocks. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	while (count < mpc->length) {
		switch (*mpt) {
		case MP_PROCESSOR:
			/* ACPI may have already provided this data */
			if (!acpi_lapic)
				MP_processor_info((struct mpc_cpu *)mpt);
			skip_entry(&mpt, &count, sizeof(struct mpc_cpu));
			break;
		case MP_BUS:
			MP_bus_info((struct mpc_bus *)mpt);
			skip_entry(&mpt, &count, sizeof(struct mpc_bus));
			break;
		case MP_IOAPIC:
			MP_ioapic_info((struct mpc_ioapic *)mpt);
			skip_entry(&mpt, &count, sizeof(struct mpc_ioapic));
			break;
		case MP_INTSRC:
			mp_save_irq((struct mpc_intsrc *)mpt);
			skip_entry(&mpt, &count, sizeof(struct mpc_intsrc));
			break;
		case MP_LINTSRC:
			MP_lintsrc_info((struct mpc_lintsrc *)mpt);
			skip_entry(&mpt, &count, sizeof(struct mpc_lintsrc));
			break;
		default:
			/* wrong mptable */
			smp_dump_mptable(mpc, mpt);
			count = mpc->length;
			break;
		}
<<<<<<< HEAD
		x86_init.mpparse.mpc_record(1);
	}

	if (!num_processors)
		printk(KERN_ERR "MPTABLE: no processors registered!\n");
	return num_processors;
=======
	}

	if (!num_procs && !acpi_lapic)
		pr_err("MPTABLE: no processors registered!\n");
	return num_procs || acpi_lapic;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifdef CONFIG_X86_IO_APIC

static int __init ELCR_trigger(unsigned int irq)
{
	unsigned int port;

<<<<<<< HEAD
	port = 0x4d0 + (irq >> 3);
=======
	port = PIC_ELCR1 + (irq >> 3);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return (inb(port) >> (irq & 7)) & 1;
}

static void __init construct_default_ioirq_mptable(int mpc_default_type)
{
	struct mpc_intsrc intsrc;
	int i;
	int ELCR_fallback = 0;

	intsrc.type = MP_INTSRC;
<<<<<<< HEAD
	intsrc.irqflag = 0;	/* conforming */
=======
	intsrc.irqflag = MP_IRQTRIG_DEFAULT | MP_IRQPOL_DEFAULT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	intsrc.srcbus = 0;
	intsrc.dstapic = mpc_ioapic_id(0);

	intsrc.irqtype = mp_INT;

	/*
	 *  If true, we have an ISA/PCI system with no IRQ entries
	 *  in the MP table. To prevent the PCI interrupts from being set up
	 *  incorrectly, we try to use the ELCR. The sanity check to see if
	 *  there is good ELCR data is very simple - IRQ0, 1, 2 and 13 can
	 *  never be level sensitive, so we simply see if the ELCR agrees.
	 *  If it does, we assume it's valid.
	 */
	if (mpc_default_type == 5) {
<<<<<<< HEAD
		printk(KERN_INFO "ISA/PCI bus type with no IRQ information... "
		       "falling back to ELCR\n");

		if (ELCR_trigger(0) || ELCR_trigger(1) || ELCR_trigger(2) ||
		    ELCR_trigger(13))
			printk(KERN_ERR "ELCR contains invalid data... "
			       "not using ELCR\n");
		else {
			printk(KERN_INFO
			       "Using ELCR to identify PCI interrupts\n");
=======
		pr_info("ISA/PCI bus type with no IRQ information... falling back to ELCR\n");

		if (ELCR_trigger(0) || ELCR_trigger(1) || ELCR_trigger(2) ||
		    ELCR_trigger(13))
			pr_err("ELCR contains invalid data... not using ELCR\n");
		else {
			pr_info("Using ELCR to identify PCI interrupts\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ELCR_fallback = 1;
		}
	}

	for (i = 0; i < 16; i++) {
		switch (mpc_default_type) {
		case 2:
			if (i == 0 || i == 13)
				continue;	/* IRQ0 & IRQ13 not connected */
<<<<<<< HEAD
			/* fall through */
=======
			fallthrough;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		default:
			if (i == 2)
				continue;	/* IRQ2 is never connected */
		}

		if (ELCR_fallback) {
			/*
			 *  If the ELCR indicates a level-sensitive interrupt, we
			 *  copy that information over to the MP table in the
			 *  irqflag field (level sensitive, active high polarity).
			 */
<<<<<<< HEAD
			if (ELCR_trigger(i))
				intsrc.irqflag = 13;
			else
				intsrc.irqflag = 0;
=======
			if (ELCR_trigger(i)) {
				intsrc.irqflag = MP_IRQTRIG_LEVEL |
						 MP_IRQPOL_ACTIVE_HIGH;
			} else {
				intsrc.irqflag = MP_IRQTRIG_DEFAULT |
						 MP_IRQPOL_DEFAULT;
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		intsrc.srcbusirq = i;
		intsrc.dstirq = i ? i : 2;	/* IRQ0 to INTIN2 */
		mp_save_irq(&intsrc);
	}

	intsrc.irqtype = mp_ExtINT;
	intsrc.srcbusirq = 0;
	intsrc.dstirq = 0;	/* 8259A to INTIN0 */
	mp_save_irq(&intsrc);
}


static void __init construct_ioapic_table(int mpc_default_type)
{
	struct mpc_ioapic ioapic;
	struct mpc_bus bus;

	bus.type = MP_BUS;
	bus.busid = 0;
	switch (mpc_default_type) {
	default:
<<<<<<< HEAD
		printk(KERN_ERR "???\nUnknown standard configuration %d\n",
		       mpc_default_type);
		/* fall through */
=======
		pr_err("???\nUnknown standard configuration %d\n",
		       mpc_default_type);
		fallthrough;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case 1:
	case 5:
		memcpy(bus.bustype, "ISA   ", 6);
		break;
	case 2:
	case 6:
	case 3:
		memcpy(bus.bustype, "EISA  ", 6);
		break;
<<<<<<< HEAD
	case 4:
	case 7:
		memcpy(bus.bustype, "MCA   ", 6);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	MP_bus_info(&bus);
	if (mpc_default_type > 4) {
		bus.busid = 1;
		memcpy(bus.bustype, "PCI   ", 6);
		MP_bus_info(&bus);
	}

	ioapic.type	= MP_IOAPIC;
	ioapic.apicid	= 2;
	ioapic.apicver	= mpc_default_type > 4 ? 0x10 : 0x01;
	ioapic.flags	= MPC_APIC_USABLE;
	ioapic.apicaddr	= IO_APIC_DEFAULT_PHYS_BASE;
	MP_ioapic_info(&ioapic);

	/*
	 * We set up most of the low 16 IO-APIC pins according to MPS rules.
	 */
	construct_default_ioirq_mptable(mpc_default_type);
}
#else
static inline void __init construct_ioapic_table(int mpc_default_type) { }
#endif

static inline void __init construct_default_ISA_mptable(int mpc_default_type)
{
	struct mpc_cpu processor;
	struct mpc_lintsrc lintsrc;
	int linttypes[2] = { mp_ExtINT, mp_NMI };
	int i;

	/*
<<<<<<< HEAD
	 * local APIC has default address
	 */
	mp_lapic_addr = APIC_DEFAULT_PHYS_BASE;

	/*
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * 2 CPUs, numbered 0 & 1.
	 */
	processor.type = MP_PROCESSOR;
	/* Either an integrated APIC or a discrete 82489DX. */
	processor.apicver = mpc_default_type > 4 ? 0x10 : 0x01;
	processor.cpuflag = CPU_ENABLED;
	processor.cpufeature = (boot_cpu_data.x86 << 8) |
<<<<<<< HEAD
	    (boot_cpu_data.x86_model << 4) | boot_cpu_data.x86_mask;
	processor.featureflag = boot_cpu_data.x86_capability[0];
=======
	    (boot_cpu_data.x86_model << 4) | boot_cpu_data.x86_stepping;
	processor.featureflag = boot_cpu_data.x86_capability[CPUID_1_EDX];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	processor.reserved[0] = 0;
	processor.reserved[1] = 0;
	for (i = 0; i < 2; i++) {
		processor.apicid = i;
		MP_processor_info(&processor);
	}

	construct_ioapic_table(mpc_default_type);

	lintsrc.type = MP_LINTSRC;
<<<<<<< HEAD
	lintsrc.irqflag = 0;		/* conforming */
=======
	lintsrc.irqflag = MP_IRQTRIG_DEFAULT | MP_IRQPOL_DEFAULT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	lintsrc.srcbusid = 0;
	lintsrc.srcbusirq = 0;
	lintsrc.destapic = MP_APIC_ALL;
	for (i = 0; i < 2; i++) {
		lintsrc.irqtype = linttypes[i];
		lintsrc.destapiclint = i;
		MP_lintsrc_info(&lintsrc);
	}
}

<<<<<<< HEAD
static struct mpf_intel *mpf_found;
=======
static unsigned long mpf_base;
static bool mpf_found;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static unsigned long __init get_mpc_size(unsigned long physptr)
{
	struct mpc_table *mpc;
	unsigned long size;

<<<<<<< HEAD
	mpc = early_ioremap(physptr, PAGE_SIZE);
	size = mpc->length;
	early_iounmap(mpc, PAGE_SIZE);
=======
	mpc = early_memremap(physptr, PAGE_SIZE);
	size = mpc->length;
	early_memunmap(mpc, PAGE_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	apic_printk(APIC_VERBOSE, "  mpc: %lx-%lx\n", physptr, physptr + size);

	return size;
}

static int __init check_physptr(struct mpf_intel *mpf, unsigned int early)
{
	struct mpc_table *mpc;
	unsigned long size;

	size = get_mpc_size(mpf->physptr);
<<<<<<< HEAD
	mpc = early_ioremap(mpf->physptr, size);
=======
	mpc = early_memremap(mpf->physptr, size);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Read the physical hardware table.  Anything here will
	 * override the defaults.
	 */
	if (!smp_read_mpc(mpc, early)) {
#ifdef CONFIG_X86_LOCAL_APIC
		smp_found_config = 0;
#endif
<<<<<<< HEAD
		printk(KERN_ERR "BIOS bug, MP table errors detected!...\n"
			"... disabling SMP support. (tell your hw vendor)\n");
		early_iounmap(mpc, size);
		return -1;
	}
	early_iounmap(mpc, size);
=======
		pr_err("BIOS bug, MP table errors detected!...\n");
		pr_cont("... disabling SMP support. (tell your hw vendor)\n");
		early_memunmap(mpc, size);
		return -1;
	}
	early_memunmap(mpc, size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (early)
		return -1;

#ifdef CONFIG_X86_IO_APIC
	/*
	 * If there are no explicit MP IRQ entries, then we are
	 * broken.  We set up most of the low 16 IO-APIC pins to
	 * ISA defaults and hope it will work.
	 */
	if (!mp_irq_entries) {
		struct mpc_bus bus;

<<<<<<< HEAD
		printk(KERN_ERR "BIOS bug, no explicit IRQ entries, "
		       "using default mptable. (tell your hw vendor)\n");
=======
		pr_err("BIOS bug, no explicit IRQ entries, using default mptable. (tell your hw vendor)\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		bus.type = MP_BUS;
		bus.busid = 0;
		memcpy(bus.bustype, "ISA   ", 6);
		MP_bus_info(&bus);

		construct_default_ioirq_mptable(0);
	}
#endif

	return 0;
}

/*
 * Scan the memory blocks for an SMP configuration block.
 */
<<<<<<< HEAD
void __init default_get_smp_config(unsigned int early)
{
	struct mpf_intel *mpf = mpf_found;

	if (!mpf)
=======
static __init void mpparse_get_smp_config(unsigned int early)
{
	struct mpf_intel *mpf;

	if (!smp_found_config)
		return;

	if (!mpf_found)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;

	if (acpi_lapic && early)
		return;

	/*
	 * MPS doesn't support hyperthreading, aka only have
	 * thread 0 apic id in MPS table
	 */
	if (acpi_lapic && acpi_ioapic)
		return;

<<<<<<< HEAD
	printk(KERN_INFO "Intel MultiProcessor Specification v1.%d\n",
	       mpf->specification);
#if defined(CONFIG_X86_LOCAL_APIC) && defined(CONFIG_X86_32)
	if (mpf->feature2 & (1 << 7)) {
		printk(KERN_INFO "    IMCR and PIC compatibility mode.\n");
		pic_mode = 1;
	} else {
		printk(KERN_INFO "    Virtual Wire compatibility mode.\n");
=======
	mpf = early_memremap(mpf_base, sizeof(*mpf));
	if (!mpf) {
		pr_err("MPTABLE: error mapping MP table\n");
		return;
	}

	pr_info("Intel MultiProcessor Specification v1.%d\n",
		mpf->specification);
#if defined(CONFIG_X86_LOCAL_APIC) && defined(CONFIG_X86_32)
	if (mpf->feature2 & (1 << 7)) {
		pr_info("    IMCR and PIC compatibility mode.\n");
		pic_mode = 1;
	} else {
		pr_info("    Virtual Wire compatibility mode.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		pic_mode = 0;
	}
#endif
	/*
	 * Now see if we need to read further.
	 */
<<<<<<< HEAD
	if (mpf->feature1 != 0) {
		if (early) {
			/*
			 * local APIC has default address
			 */
			mp_lapic_addr = APIC_DEFAULT_PHYS_BASE;
			return;
		}

		printk(KERN_INFO "Default MP configuration #%d\n",
		       mpf->feature1);
=======
	if (mpf->feature1) {
		if (early) {
			/* Local APIC has default address */
			register_lapic_address(APIC_DEFAULT_PHYS_BASE);
			goto out;
		}

		pr_info("Default MP configuration #%d\n", mpf->feature1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		construct_default_ISA_mptable(mpf->feature1);

	} else if (mpf->physptr) {
		if (check_physptr(mpf, early))
<<<<<<< HEAD
			return;
	} else
		BUG();

	if (!early)
		printk(KERN_INFO "Processors: %d\n", num_processors);
	/*
	 * Only use the first configuration found.
	 */
=======
			goto out;
	} else
		BUG();

	if (!early && !acpi_lapic)
		pr_info("Processors: %d\n", num_procs);
	/*
	 * Only use the first configuration found.
	 */
out:
	early_memunmap(mpf, sizeof(*mpf));
}

void __init mpparse_parse_early_smp_config(void)
{
	mpparse_get_smp_config(true);
}

void __init mpparse_parse_smp_config(void)
{
	mpparse_get_smp_config(false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __init smp_reserve_memory(struct mpf_intel *mpf)
{
	memblock_reserve(mpf->physptr, get_mpc_size(mpf->physptr));
}

static int __init smp_scan_config(unsigned long base, unsigned long length)
{
<<<<<<< HEAD
	unsigned int *bp = phys_to_virt(base);
	struct mpf_intel *mpf;
	unsigned long mem;

	apic_printk(APIC_VERBOSE, "Scan SMP from %p for %ld bytes.\n",
			bp, length);
	BUILD_BUG_ON(sizeof(*mpf) != 16);

	while (length > 0) {
=======
	unsigned int *bp;
	struct mpf_intel *mpf;
	int ret = 0;

	apic_printk(APIC_VERBOSE, "Scan for SMP in [mem %#010lx-%#010lx]\n",
		    base, base + length - 1);
	BUILD_BUG_ON(sizeof(*mpf) != 16);

	while (length > 0) {
		bp = early_memremap(base, length);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mpf = (struct mpf_intel *)bp;
		if ((*bp == SMP_MAGIC_IDENT) &&
		    (mpf->length == 1) &&
		    !mpf_checksum((unsigned char *)bp, 16) &&
		    ((mpf->specification == 1)
		     || (mpf->specification == 4))) {
#ifdef CONFIG_X86_LOCAL_APIC
			smp_found_config = 1;
#endif
<<<<<<< HEAD
			mpf_found = mpf;

			printk(KERN_INFO "found SMP MP-table at [%p] %llx\n",
			       mpf, (u64)virt_to_phys(mpf));

			mem = virt_to_phys(mpf);
			memblock_reserve(mem, sizeof(*mpf));
			if (mpf->physptr)
				smp_reserve_memory(mpf);

			return 1;
		}
		bp += 4;
		length -= 16;
	}
	return 0;
}

void __init default_find_smp_config(void)
=======
			mpf_base = base;
			mpf_found = true;

			pr_info("found SMP MP-table at [mem %#010lx-%#010lx]\n",
				base, base + sizeof(*mpf) - 1);

			memblock_reserve(base, sizeof(*mpf));
			if (mpf->physptr)
				smp_reserve_memory(mpf);

			ret = 1;
		}
		early_memunmap(bp, length);

		if (ret)
			break;

		base += 16;
		length -= 16;
	}
	return ret;
}

void __init mpparse_find_mptable(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned int address;

	/*
	 * FIXME: Linux assumes you have 640K of base ram..
	 * this continues the error...
	 *
	 * 1) Scan the bottom 1K for a signature
	 * 2) Scan the top 1K of base RAM
	 * 3) Scan the 64K of bios
	 */
	if (smp_scan_config(0x0, 0x400) ||
	    smp_scan_config(639 * 0x400, 0x400) ||
	    smp_scan_config(0xF0000, 0x10000))
		return;
	/*
	 * If it is an SMP machine we should know now, unless the
<<<<<<< HEAD
	 * configuration is in an EISA/MCA bus machine with an
=======
	 * configuration is in an EISA bus machine with an
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * extended bios data area.
	 *
	 * there is a real-mode segmented pointer pointing to the
	 * 4K EBDA area at 0x40E, calculate and scan it here.
	 *
	 * NOTE! There are Linux loaders that will corrupt the EBDA
	 * area, and as such this kind of SMP config may be less
	 * trustworthy, simply because the SMP table may have been
	 * stomped on during early boot. These loaders are buggy and
	 * should be fixed.
	 *
	 * MP1.4 SPEC states to only scan first 1K of 4K EBDA.
	 */

	address = get_bios_ebda();
	if (address)
		smp_scan_config(address, 0x400);
}

#ifdef CONFIG_X86_IO_APIC
static u8 __initdata irq_used[MAX_IRQ_SOURCES];

static int  __init get_MP_intsrc_index(struct mpc_intsrc *m)
{
	int i;

	if (m->irqtype != mp_INT)
		return 0;

<<<<<<< HEAD
	if (m->irqflag != 0x0f)
=======
	if (m->irqflag != (MP_IRQTRIG_LEVEL | MP_IRQPOL_ACTIVE_LOW))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	/* not legacy */

	for (i = 0; i < mp_irq_entries; i++) {
		if (mp_irqs[i].irqtype != mp_INT)
			continue;

<<<<<<< HEAD
		if (mp_irqs[i].irqflag != 0x0f)
=======
		if (mp_irqs[i].irqflag != (MP_IRQTRIG_LEVEL |
					   MP_IRQPOL_ACTIVE_LOW))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			continue;

		if (mp_irqs[i].srcbus != m->srcbus)
			continue;
		if (mp_irqs[i].srcbusirq != m->srcbusirq)
			continue;
		if (irq_used[i]) {
			/* already claimed */
			return -2;
		}
		irq_used[i] = 1;
		return i;
	}

	/* not found */
	return -1;
}

#define SPARE_SLOT_NUM 20

static struct mpc_intsrc __initdata *m_spare[SPARE_SLOT_NUM];

static void __init check_irq_src(struct mpc_intsrc *m, int *nr_m_spare)
{
	int i;

	apic_printk(APIC_VERBOSE, "OLD ");
	print_mp_irq_info(m);

	i = get_MP_intsrc_index(m);
	if (i > 0) {
		memcpy(m, &mp_irqs[i], sizeof(*m));
		apic_printk(APIC_VERBOSE, "NEW ");
		print_mp_irq_info(&mp_irqs[i]);
		return;
	}
	if (!i) {
		/* legacy, do nothing */
		return;
	}
	if (*nr_m_spare < SPARE_SLOT_NUM) {
		/*
		 * not found (-1), or duplicated (-2) are invalid entries,
		 * we need to use the slot later
		 */
		m_spare[*nr_m_spare] = m;
		*nr_m_spare += 1;
	}
}

static int __init
check_slot(unsigned long mpc_new_phys, unsigned long mpc_new_length, int count)
{
	if (!mpc_new_phys || count <= mpc_new_length) {
		WARN(1, "update_mptable: No spare slots (length: %x)\n", count);
		return -1;
	}

	return 0;
}
#else /* CONFIG_X86_IO_APIC */
static
inline void __init check_irq_src(struct mpc_intsrc *m, int *nr_m_spare) {}
#endif /* CONFIG_X86_IO_APIC */

static int  __init replace_intsrc_all(struct mpc_table *mpc,
					unsigned long mpc_new_phys,
					unsigned long mpc_new_length)
{
#ifdef CONFIG_X86_IO_APIC
	int i;
#endif
	int count = sizeof(*mpc);
	int nr_m_spare = 0;
	unsigned char *mpt = ((unsigned char *)mpc) + count;

<<<<<<< HEAD
	printk(KERN_INFO "mpc_length %x\n", mpc->length);
=======
	pr_info("mpc_length %x\n", mpc->length);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	while (count < mpc->length) {
		switch (*mpt) {
		case MP_PROCESSOR:
			skip_entry(&mpt, &count, sizeof(struct mpc_cpu));
			break;
		case MP_BUS:
			skip_entry(&mpt, &count, sizeof(struct mpc_bus));
			break;
		case MP_IOAPIC:
			skip_entry(&mpt, &count, sizeof(struct mpc_ioapic));
			break;
		case MP_INTSRC:
			check_irq_src((struct mpc_intsrc *)mpt, &nr_m_spare);
			skip_entry(&mpt, &count, sizeof(struct mpc_intsrc));
			break;
		case MP_LINTSRC:
			skip_entry(&mpt, &count, sizeof(struct mpc_lintsrc));
			break;
		default:
			/* wrong mptable */
			smp_dump_mptable(mpc, mpt);
			goto out;
		}
	}

#ifdef CONFIG_X86_IO_APIC
	for (i = 0; i < mp_irq_entries; i++) {
		if (irq_used[i])
			continue;

		if (mp_irqs[i].irqtype != mp_INT)
			continue;

<<<<<<< HEAD
		if (mp_irqs[i].irqflag != 0x0f)
=======
		if (mp_irqs[i].irqflag != (MP_IRQTRIG_LEVEL |
					   MP_IRQPOL_ACTIVE_LOW))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			continue;

		if (nr_m_spare > 0) {
			apic_printk(APIC_VERBOSE, "*NEW* found\n");
			nr_m_spare--;
			memcpy(m_spare[nr_m_spare], &mp_irqs[i], sizeof(mp_irqs[i]));
			m_spare[nr_m_spare] = NULL;
		} else {
			struct mpc_intsrc *m = (struct mpc_intsrc *)mpt;
			count += sizeof(struct mpc_intsrc);
			if (check_slot(mpc_new_phys, mpc_new_length, count) < 0)
				goto out;
			memcpy(m, &mp_irqs[i], sizeof(*m));
			mpc->length = count;
			mpt += sizeof(struct mpc_intsrc);
		}
		print_mp_irq_info(&mp_irqs[i]);
	}
#endif
out:
	/* update checksum */
	mpc->checksum = 0;
	mpc->checksum -= mpf_checksum((unsigned char *)mpc, mpc->length);

	return 0;
}

int enable_update_mptable;

static int __init update_mptable_setup(char *str)
{
	enable_update_mptable = 1;
#ifdef CONFIG_PCI
	pci_routeirq = 1;
#endif
	return 0;
}
early_param("update_mptable", update_mptable_setup);

static unsigned long __initdata mpc_new_phys;
static unsigned long mpc_new_length __initdata = 4096;

/* alloc_mptable or alloc_mptable=4k */
static int __initdata alloc_mptable;
static int __init parse_alloc_mptable_opt(char *p)
{
	enable_update_mptable = 1;
#ifdef CONFIG_PCI
	pci_routeirq = 1;
#endif
	alloc_mptable = 1;
	if (!p)
		return 0;
	mpc_new_length = memparse(p, &p);
	return 0;
}
early_param("alloc_mptable", parse_alloc_mptable_opt);

<<<<<<< HEAD
void __init early_reserve_e820_mpc_new(void)
{
	if (enable_update_mptable && alloc_mptable)
		mpc_new_phys = early_reserve_e820(mpc_new_length, 4);
=======
void __init e820__memblock_alloc_reserved_mpc_new(void)
{
	if (enable_update_mptable && alloc_mptable)
		mpc_new_phys = e820__memblock_alloc_reserved(mpc_new_length, 4);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int __init update_mp_table(void)
{
	char str[16];
	char oem[10];
	struct mpf_intel *mpf;
	struct mpc_table *mpc, *mpc_new;
<<<<<<< HEAD
=======
	unsigned long size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!enable_update_mptable)
		return 0;

<<<<<<< HEAD
	mpf = mpf_found;
	if (!mpf)
		return 0;

	/*
	 * Now see if we need to go further.
	 */
	if (mpf->feature1 != 0)
		return 0;

	if (!mpf->physptr)
		return 0;

	mpc = phys_to_virt(mpf->physptr);

	if (!smp_check_mpc(mpc, oem, str))
		return 0;

	printk(KERN_INFO "mpf: %llx\n", (u64)virt_to_phys(mpf));
	printk(KERN_INFO "physptr: %x\n", mpf->physptr);

	if (mpc_new_phys && mpc->length > mpc_new_length) {
		mpc_new_phys = 0;
		printk(KERN_INFO "mpc_new_length is %ld, please use alloc_mptable=8k\n",
			 mpc_new_length);
=======
	if (!mpf_found)
		return 0;

	mpf = early_memremap(mpf_base, sizeof(*mpf));
	if (!mpf) {
		pr_err("MPTABLE: mpf early_memremap() failed\n");
		return 0;
	}

	/*
	 * Now see if we need to go further.
	 */
	if (mpf->feature1)
		goto do_unmap_mpf;

	if (!mpf->physptr)
		goto do_unmap_mpf;

	size = get_mpc_size(mpf->physptr);
	mpc = early_memremap(mpf->physptr, size);
	if (!mpc) {
		pr_err("MPTABLE: mpc early_memremap() failed\n");
		goto do_unmap_mpf;
	}

	if (!smp_check_mpc(mpc, oem, str))
		goto do_unmap_mpc;

	pr_info("mpf: %llx\n", (u64)mpf_base);
	pr_info("physptr: %x\n", mpf->physptr);

	if (mpc_new_phys && mpc->length > mpc_new_length) {
		mpc_new_phys = 0;
		pr_info("mpc_new_length is %ld, please use alloc_mptable=8k\n",
			mpc_new_length);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (!mpc_new_phys) {
		unsigned char old, new;
		/* check if we can change the position */
		mpc->checksum = 0;
		old = mpf_checksum((unsigned char *)mpc, mpc->length);
		mpc->checksum = 0xff;
		new = mpf_checksum((unsigned char *)mpc, mpc->length);
		if (old == new) {
<<<<<<< HEAD
			printk(KERN_INFO "mpc is readonly, please try alloc_mptable instead\n");
			return 0;
		}
		printk(KERN_INFO "use in-position replacing\n");
	} else {
		mpf->physptr = mpc_new_phys;
		mpc_new = phys_to_virt(mpc_new_phys);
		memcpy(mpc_new, mpc, mpc->length);
		mpc = mpc_new;
=======
			pr_info("mpc is readonly, please try alloc_mptable instead\n");
			goto do_unmap_mpc;
		}
		pr_info("use in-position replacing\n");
	} else {
		mpc_new = early_memremap(mpc_new_phys, mpc_new_length);
		if (!mpc_new) {
			pr_err("MPTABLE: new mpc early_memremap() failed\n");
			goto do_unmap_mpc;
		}
		mpf->physptr = mpc_new_phys;
		memcpy(mpc_new, mpc, mpc->length);
		early_memunmap(mpc, size);
		mpc = mpc_new;
		size = mpc_new_length;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* check if we can modify that */
		if (mpc_new_phys - mpf->physptr) {
			struct mpf_intel *mpf_new;
			/* steal 16 bytes from [0, 1k) */
<<<<<<< HEAD
			printk(KERN_INFO "mpf new: %x\n", 0x400 - 16);
			mpf_new = phys_to_virt(0x400 - 16);
			memcpy(mpf_new, mpf, 16);
=======
			mpf_new = early_memremap(0x400 - 16, sizeof(*mpf_new));
			if (!mpf_new) {
				pr_err("MPTABLE: new mpf early_memremap() failed\n");
				goto do_unmap_mpc;
			}
			pr_info("mpf new: %x\n", 0x400 - 16);
			memcpy(mpf_new, mpf, 16);
			early_memunmap(mpf, sizeof(*mpf));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			mpf = mpf_new;
			mpf->physptr = mpc_new_phys;
		}
		mpf->checksum = 0;
		mpf->checksum -= mpf_checksum((unsigned char *)mpf, 16);
<<<<<<< HEAD
		printk(KERN_INFO "physptr new: %x\n", mpf->physptr);
=======
		pr_info("physptr new: %x\n", mpf->physptr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * only replace the one with mp_INT and
	 *	 MP_IRQ_TRIGGER_LEVEL|MP_IRQ_POLARITY_LOW,
	 * already in mp_irqs , stored by ... and mp_config_acpi_gsi,
	 * may need pci=routeirq for all coverage
	 */
	replace_intsrc_all(mpc, mpc_new_phys, mpc_new_length);

<<<<<<< HEAD
=======
do_unmap_mpc:
	early_memunmap(mpc, size);

do_unmap_mpf:
	early_memunmap(mpf, sizeof(*mpf));

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

late_initcall(update_mp_table);
