<<<<<<< HEAD
#include <linux/module.h>
#include <linux/slab.h>

#include "mce_amd.h"

static struct amd_decoder_ops *fam_ops;

static u8 xec_mask	 = 0xf;
static u8 nb_err_cpumask = 0xf;

static bool report_gart_errors;
static void (*nb_bus_decoder)(int node_id, struct mce *m);

void amd_report_gart_errors(bool v)
{
	report_gart_errors = v;
}
EXPORT_SYMBOL_GPL(amd_report_gart_errors);

void amd_register_ecc_decoder(void (*f)(int, struct mce *))
{
	nb_bus_decoder = f;
=======
// SPDX-License-Identifier: GPL-2.0-only
#include <linux/module.h>
#include <linux/slab.h>

#include <asm/cpu.h>

#include "mce_amd.h"

static struct amd_decoder_ops fam_ops;

static u8 xec_mask	 = 0xf;

static void (*decode_dram_ecc)(int node_id, struct mce *m);

void amd_register_ecc_decoder(void (*f)(int, struct mce *))
{
	decode_dram_ecc = f;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(amd_register_ecc_decoder);

void amd_unregister_ecc_decoder(void (*f)(int, struct mce *))
{
<<<<<<< HEAD
	if (nb_bus_decoder) {
		WARN_ON(nb_bus_decoder != f);

		nb_bus_decoder = NULL;
=======
	if (decode_dram_ecc) {
		WARN_ON(decode_dram_ecc != f);

		decode_dram_ecc = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}
EXPORT_SYMBOL_GPL(amd_unregister_ecc_decoder);

/*
 * string representation for the different MCA reported error types, see F3x48
 * or MSR0000_0411.
 */

/* transaction type */
<<<<<<< HEAD
const char * const tt_msgs[] = { "INSN", "DATA", "GEN", "RESV" };
EXPORT_SYMBOL_GPL(tt_msgs);

/* cache level */
const char * const ll_msgs[] = { "RESV", "L1", "L2", "L3/GEN" };
EXPORT_SYMBOL_GPL(ll_msgs);

/* memory transaction type */
const char * const rrrr_msgs[] = {
       "GEN", "RD", "WR", "DRD", "DWR", "IRD", "PRF", "EV", "SNP"
};
EXPORT_SYMBOL_GPL(rrrr_msgs);
=======
static const char * const tt_msgs[] = { "INSN", "DATA", "GEN", "RESV" };

/* cache level */
static const char * const ll_msgs[] = { "RESV", "L1", "L2", "L3/GEN" };

/* memory transaction type */
static const char * const rrrr_msgs[] = {
       "GEN", "RD", "WR", "DRD", "DWR", "IRD", "PRF", "EV", "SNP"
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* participating processor */
const char * const pp_msgs[] = { "SRC", "RES", "OBS", "GEN" };
EXPORT_SYMBOL_GPL(pp_msgs);

/* request timeout */
<<<<<<< HEAD
const char * const to_msgs[] = { "no timeout", "timed out" };
EXPORT_SYMBOL_GPL(to_msgs);

/* memory or i/o */
const char * const ii_msgs[] = { "MEM", "RESV", "IO", "GEN" };
EXPORT_SYMBOL_GPL(ii_msgs);

static const char * const f15h_ic_mce_desc[] = {
=======
static const char * const to_msgs[] = { "no timeout", "timed out" };

/* memory or i/o */
static const char * const ii_msgs[] = { "MEM", "RESV", "IO", "GEN" };

/* internal error type */
static const char * const uu_msgs[] = { "RESV", "RESV", "HWA", "RESV" };

static const char * const f15h_mc1_mce_desc[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	"UC during a demand linefill from L2",
	"Parity error during data load from IC",
	"Parity error for IC valid bit",
	"Main tag parity error",
	"Parity error in prediction queue",
	"PFB data/address parity error",
	"Parity error in the branch status reg",
	"PFB promotion address error",
	"Tag error during probe/victimization",
	"Parity error for IC probe tag valid bit",
	"PFB non-cacheable bit parity error",
	"PFB valid bit parity error",			/* xec = 0xd */
	"Microcode Patch Buffer",			/* xec = 010 */
	"uop queue",
	"insn buffer",
	"predecode buffer",
<<<<<<< HEAD
	"fetch address FIFO"
};

static const char * const f15h_cu_mce_desc[] = {
=======
	"fetch address FIFO",
	"dispatch uop queue"
};

static const char * const f15h_mc2_mce_desc[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	"Fill ECC error on data fills",			/* xec = 0x4 */
	"Fill parity error on insn fills",
	"Prefetcher request FIFO parity error",
	"PRQ address parity error",
	"PRQ data parity error",
	"WCC Tag ECC error",
	"WCC Data ECC error",
	"WCB Data parity error",
	"VB Data ECC or parity error",
	"L2 Tag ECC error",				/* xec = 0x10 */
	"Hard L2 Tag ECC error",
	"Multiple hits on L2 tag",
	"XAB parity error",
	"PRB address parity error"
};

<<<<<<< HEAD
static const char * const nb_mce_desc[] = {
=======
static const char * const mc4_mce_desc[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	"DRAM ECC error detected on the NB",
	"CRC error detected on HT link",
	"Link-defined sync error packets detected on HT link",
	"HT Master abort",
	"HT Target abort",
	"Invalid GART PTE entry during GART table walk",
	"Unsupported atomic RMW received from an IO link",
	"Watchdog timeout due to lack of progress",
	"DRAM ECC error detected on the NB",
	"SVM DMA Exclusion Vector error",
	"HT data error detected on link",
	"Protocol error (link, L3, probe filter)",
	"NB internal arrays parity error",
	"DRAM addr/ctl signals parity error",
	"IO link transmission error",
	"L3 data cache ECC error",			/* xec = 0x1c */
	"L3 cache tag error",
	"L3 LRU parity bits error",
	"ECC Error in the Probe Filter directory"
};

<<<<<<< HEAD
static const char * const fr_ex_mce_desc[] = {
=======
static const char * const mc5_mce_desc[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	"CPU Watchdog timer expire",
	"Wakeup array dest tag",
	"AG payload array",
	"EX payload array",
	"IDRF array",
	"Retire dispatch queue",
	"Mapper checkpoint array",
	"Physical register file EX0 port",
	"Physical register file EX1 port",
	"Physical register file AG0 port",
	"Physical register file AG1 port",
	"Flag register file",
<<<<<<< HEAD
	"DE error occurred"
};

static bool f12h_dc_mce(u16 ec, u8 xec)
=======
	"DE error occurred",
	"Retire status queue"
};

static const char * const mc6_mce_desc[] = {
	"Hardware Assertion",
	"Free List",
	"Physical Register File",
	"Retire Queue",
	"Scheduler table",
	"Status Register File",
};

static bool f12h_mc0_mce(u16 ec, u8 xec)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	bool ret = false;

	if (MEM_ERROR(ec)) {
		u8 ll = LL(ec);
		ret = true;

		if (ll == LL_L2)
			pr_cont("during L1 linefill from L2.\n");
		else if (ll == LL_L1)
			pr_cont("Data/Tag %s error.\n", R4_MSG(ec));
		else
			ret = false;
	}
	return ret;
}

<<<<<<< HEAD
static bool f10h_dc_mce(u16 ec, u8 xec)
=======
static bool f10h_mc0_mce(u16 ec, u8 xec)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (R4(ec) == R4_GEN && LL(ec) == LL_L1) {
		pr_cont("during data scrub.\n");
		return true;
	}
<<<<<<< HEAD
	return f12h_dc_mce(ec, xec);
}

static bool k8_dc_mce(u16 ec, u8 xec)
=======
	return f12h_mc0_mce(ec, xec);
}

static bool k8_mc0_mce(u16 ec, u8 xec)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (BUS_ERROR(ec)) {
		pr_cont("during system linefill.\n");
		return true;
	}

<<<<<<< HEAD
	return f10h_dc_mce(ec, xec);
}

static bool f14h_dc_mce(u16 ec, u8 xec)
=======
	return f10h_mc0_mce(ec, xec);
}

static bool cat_mc0_mce(u16 ec, u8 xec)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u8 r4	 = R4(ec);
	bool ret = true;

	if (MEM_ERROR(ec)) {

		if (TT(ec) != TT_DATA || LL(ec) != LL_L1)
			return false;

		switch (r4) {
		case R4_DRD:
		case R4_DWR:
			pr_cont("Data/Tag parity error due to %s.\n",
				(r4 == R4_DRD ? "load/hw prf" : "store"));
			break;
		case R4_EVICT:
			pr_cont("Copyback parity error on a tag miss.\n");
			break;
		case R4_SNOOP:
			pr_cont("Tag parity error during snoop.\n");
			break;
		default:
			ret = false;
		}
	} else if (BUS_ERROR(ec)) {

		if ((II(ec) != II_MEM && II(ec) != II_IO) || LL(ec) != LL_LG)
			return false;

		pr_cont("System read data error on a ");

		switch (r4) {
		case R4_RD:
			pr_cont("TLB reload.\n");
			break;
		case R4_DWR:
			pr_cont("store.\n");
			break;
		case R4_DRD:
			pr_cont("load.\n");
			break;
		default:
			ret = false;
		}
	} else {
		ret = false;
	}

	return ret;
}

<<<<<<< HEAD
static bool f15h_dc_mce(u16 ec, u8 xec)
=======
static bool f15h_mc0_mce(u16 ec, u8 xec)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	bool ret = true;

	if (MEM_ERROR(ec)) {

		switch (xec) {
		case 0x0:
			pr_cont("Data Array access error.\n");
			break;

		case 0x1:
			pr_cont("UC error during a linefill from L2/NB.\n");
			break;

		case 0x2:
		case 0x11:
			pr_cont("STQ access error.\n");
			break;

		case 0x3:
			pr_cont("SCB access error.\n");
			break;

		case 0x10:
			pr_cont("Tag error.\n");
			break;

		case 0x12:
			pr_cont("LDQ access error.\n");
			break;

		default:
			ret = false;
		}
	} else if (BUS_ERROR(ec)) {

		if (!xec)
			pr_cont("System Read Data Error.\n");
		else
			pr_cont(" Internal error condition type %d.\n", xec);
<<<<<<< HEAD
=======
	} else if (INT_ERROR(ec)) {
		if (xec <= 0x1f)
			pr_cont("Hardware Assert.\n");
		else
			ret = false;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else
		ret = false;

	return ret;
}

<<<<<<< HEAD
static void amd_decode_dc_mce(struct mce *m)
=======
static void decode_mc0_mce(struct mce *m)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u16 ec = EC(m->status);
	u8 xec = XEC(m->status, xec_mask);

<<<<<<< HEAD
	pr_emerg(HW_ERR "Data Cache Error: ");
=======
	pr_emerg(HW_ERR "MC0 Error: ");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* TLB error signatures are the same across families */
	if (TLB_ERROR(ec)) {
		if (TT(ec) == TT_DATA) {
			pr_cont("%s TLB %s.\n", LL_MSG(ec),
				((xec == 2) ? "locked miss"
					    : (xec ? "multimatch" : "parity")));
			return;
		}
<<<<<<< HEAD
	} else if (fam_ops->dc_mce(ec, xec))
		;
	else
		pr_emerg(HW_ERR "Corrupted DC MCE info?\n");
}

static bool k8_ic_mce(u16 ec, u8 xec)
=======
	} else if (fam_ops.mc0_mce(ec, xec))
		;
	else
		pr_emerg(HW_ERR "Corrupted MC0 MCE info?\n");
}

static bool k8_mc1_mce(u16 ec, u8 xec)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u8 ll	 = LL(ec);
	bool ret = true;

	if (!MEM_ERROR(ec))
		return false;

	if (ll == 0x2)
		pr_cont("during a linefill from L2.\n");
	else if (ll == 0x1) {
		switch (R4(ec)) {
		case R4_IRD:
			pr_cont("Parity error during data load.\n");
			break;

		case R4_EVICT:
			pr_cont("Copyback Parity/Victim error.\n");
			break;

		case R4_SNOOP:
			pr_cont("Tag Snoop error.\n");
			break;

		default:
			ret = false;
			break;
		}
	} else
		ret = false;

	return ret;
}

<<<<<<< HEAD
static bool f14h_ic_mce(u16 ec, u8 xec)
=======
static bool cat_mc1_mce(u16 ec, u8 xec)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u8 r4    = R4(ec);
	bool ret = true;

<<<<<<< HEAD
	if (MEM_ERROR(ec)) {
		if (TT(ec) != 0 || LL(ec) != 1)
			ret = false;

		if (r4 == R4_IRD)
			pr_cont("Data/tag array parity error for a tag hit.\n");
		else if (r4 == R4_SNOOP)
			pr_cont("Tag error during snoop/victimization.\n");
		else
			ret = false;
	}
	return ret;
}

static bool f15h_ic_mce(u16 ec, u8 xec)
=======
	if (!MEM_ERROR(ec))
		return false;

	if (TT(ec) != TT_INSTR)
		return false;

	if (r4 == R4_IRD)
		pr_cont("Data/tag array parity error for a tag hit.\n");
	else if (r4 == R4_SNOOP)
		pr_cont("Tag error during snoop/victimization.\n");
	else if (xec == 0x0)
		pr_cont("Tag parity error from victim castout.\n");
	else if (xec == 0x2)
		pr_cont("Microcode patch RAM parity error.\n");
	else
		ret = false;

	return ret;
}

static bool f15h_mc1_mce(u16 ec, u8 xec)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	bool ret = true;

	if (!MEM_ERROR(ec))
		return false;

	switch (xec) {
	case 0x0 ... 0xa:
<<<<<<< HEAD
		pr_cont("%s.\n", f15h_ic_mce_desc[xec]);
		break;

	case 0xd:
		pr_cont("%s.\n", f15h_ic_mce_desc[xec-2]);
		break;

	case 0x10:
		pr_cont("%s.\n", f15h_ic_mce_desc[xec-4]);
		break;

	case 0x11 ... 0x14:
		pr_cont("Decoder %s parity error.\n", f15h_ic_mce_desc[xec-4]);
=======
		pr_cont("%s.\n", f15h_mc1_mce_desc[xec]);
		break;

	case 0xd:
		pr_cont("%s.\n", f15h_mc1_mce_desc[xec-2]);
		break;

	case 0x10:
		pr_cont("%s.\n", f15h_mc1_mce_desc[xec-4]);
		break;

	case 0x11 ... 0x15:
		pr_cont("Decoder %s parity error.\n", f15h_mc1_mce_desc[xec-4]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;

	default:
		ret = false;
	}
	return ret;
}

<<<<<<< HEAD
static void amd_decode_ic_mce(struct mce *m)
=======
static void decode_mc1_mce(struct mce *m)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u16 ec = EC(m->status);
	u8 xec = XEC(m->status, xec_mask);

<<<<<<< HEAD
	pr_emerg(HW_ERR "Instruction Cache Error: ");
=======
	pr_emerg(HW_ERR "MC1 Error: ");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (TLB_ERROR(ec))
		pr_cont("%s TLB %s.\n", LL_MSG(ec),
			(xec ? "multimatch" : "parity error"));
	else if (BUS_ERROR(ec)) {
		bool k8 = (boot_cpu_data.x86 == 0xf && (m->status & BIT_64(58)));

		pr_cont("during %s.\n", (k8 ? "system linefill" : "NB data read"));
<<<<<<< HEAD
	} else if (fam_ops->ic_mce(ec, xec))
		;
	else
		pr_emerg(HW_ERR "Corrupted IC MCE info?\n");
}

static void amd_decode_bu_mce(struct mce *m)
{
	u16 ec = EC(m->status);
	u8 xec = XEC(m->status, xec_mask);

	pr_emerg(HW_ERR "Bus Unit Error");
=======
	} else if (INT_ERROR(ec)) {
		if (xec <= 0x3f)
			pr_cont("Hardware Assert.\n");
		else
			goto wrong_mc1_mce;
	} else if (fam_ops.mc1_mce(ec, xec))
		;
	else
		goto wrong_mc1_mce;

	return;

wrong_mc1_mce:
	pr_emerg(HW_ERR "Corrupted MC1 MCE info?\n");
}

static bool k8_mc2_mce(u16 ec, u8 xec)
{
	bool ret = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (xec == 0x1)
		pr_cont(" in the write data buffers.\n");
	else if (xec == 0x3)
		pr_cont(" in the victim data buffers.\n");
	else if (xec == 0x2 && MEM_ERROR(ec))
		pr_cont(": %s error in the L2 cache tags.\n", R4_MSG(ec));
	else if (xec == 0x0) {
		if (TLB_ERROR(ec))
<<<<<<< HEAD
			pr_cont(": %s error in a Page Descriptor Cache or "
				"Guest TLB.\n", TT_MSG(ec));
=======
			pr_cont("%s error in a Page Descriptor Cache or Guest TLB.\n",
				TT_MSG(ec));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		else if (BUS_ERROR(ec))
			pr_cont(": %s/ECC error in data read from NB: %s.\n",
				R4_MSG(ec), PP_MSG(ec));
		else if (MEM_ERROR(ec)) {
			u8 r4 = R4(ec);

			if (r4 >= 0x7)
				pr_cont(": %s error during data copyback.\n",
					R4_MSG(ec));
			else if (r4 <= 0x1)
				pr_cont(": %s parity/ECC error during data "
					"access from L2.\n", R4_MSG(ec));
			else
<<<<<<< HEAD
				goto wrong_bu_mce;
		} else
			goto wrong_bu_mce;
	} else
		goto wrong_bu_mce;

	return;

wrong_bu_mce:
	pr_emerg(HW_ERR "Corrupted BU MCE info?\n");
}

static void amd_decode_cu_mce(struct mce *m)
{
	u16 ec = EC(m->status);
	u8 xec = XEC(m->status, xec_mask);

	pr_emerg(HW_ERR "Combined Unit Error: ");
=======
				ret = false;
		} else
			ret = false;
	} else
		ret = false;

	return ret;
}

static bool f15h_mc2_mce(u16 ec, u8 xec)
{
	bool ret = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (TLB_ERROR(ec)) {
		if (xec == 0x0)
			pr_cont("Data parity TLB read error.\n");
		else if (xec == 0x1)
			pr_cont("Poison data provided for TLB fill.\n");
		else
<<<<<<< HEAD
			goto wrong_cu_mce;
	} else if (BUS_ERROR(ec)) {
		if (xec > 2)
			goto wrong_cu_mce;
=======
			ret = false;
	} else if (BUS_ERROR(ec)) {
		if (xec > 2)
			ret = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		pr_cont("Error during attempted NB data read.\n");
	} else if (MEM_ERROR(ec)) {
		switch (xec) {
		case 0x4 ... 0xc:
<<<<<<< HEAD
			pr_cont("%s.\n", f15h_cu_mce_desc[xec - 0x4]);
			break;

		case 0x10 ... 0x14:
			pr_cont("%s.\n", f15h_cu_mce_desc[xec - 0x7]);
			break;

		default:
			goto wrong_cu_mce;
		}
	}

	return;

wrong_cu_mce:
	pr_emerg(HW_ERR "Corrupted CU MCE info?\n");
}

static void amd_decode_ls_mce(struct mce *m)
=======
			pr_cont("%s.\n", f15h_mc2_mce_desc[xec - 0x4]);
			break;

		case 0x10 ... 0x14:
			pr_cont("%s.\n", f15h_mc2_mce_desc[xec - 0x7]);
			break;

		default:
			ret = false;
		}
	} else if (INT_ERROR(ec)) {
		if (xec <= 0x3f)
			pr_cont("Hardware Assert.\n");
		else
			ret = false;
	}

	return ret;
}

static bool f16h_mc2_mce(u16 ec, u8 xec)
{
	u8 r4 = R4(ec);

	if (!MEM_ERROR(ec))
		return false;

	switch (xec) {
	case 0x04 ... 0x05:
		pr_cont("%cBUFF parity error.\n", (r4 == R4_RD) ? 'I' : 'O');
		break;

	case 0x09 ... 0x0b:
	case 0x0d ... 0x0f:
		pr_cont("ECC error in L2 tag (%s).\n",
			((r4 == R4_GEN)   ? "BankReq" :
			((r4 == R4_SNOOP) ? "Prb"     : "Fill")));
		break;

	case 0x10 ... 0x19:
	case 0x1b:
		pr_cont("ECC error in L2 data array (%s).\n",
			(((r4 == R4_RD) && !(xec & 0x3)) ? "Hit"  :
			((r4 == R4_GEN)   ? "Attr" :
			((r4 == R4_EVICT) ? "Vict" : "Fill"))));
		break;

	case 0x1c ... 0x1d:
	case 0x1f:
		pr_cont("Parity error in L2 attribute bits (%s).\n",
			((r4 == R4_RD)  ? "Hit"  :
			((r4 == R4_GEN) ? "Attr" : "Fill")));
		break;

	default:
		return false;
	}

	return true;
}

static void decode_mc2_mce(struct mce *m)
{
	u16 ec = EC(m->status);
	u8 xec = XEC(m->status, xec_mask);

	pr_emerg(HW_ERR "MC2 Error: ");

	if (!fam_ops.mc2_mce(ec, xec))
		pr_cont(HW_ERR "Corrupted MC2 MCE info?\n");
}

static void decode_mc3_mce(struct mce *m)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u16 ec = EC(m->status);
	u8 xec = XEC(m->status, xec_mask);

	if (boot_cpu_data.x86 >= 0x14) {
<<<<<<< HEAD
		pr_emerg("You shouldn't be seeing an LS MCE on this cpu family,"
=======
		pr_emerg("You shouldn't be seeing MC3 MCE on this cpu family,"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			 " please report on LKML.\n");
		return;
	}

<<<<<<< HEAD
	pr_emerg(HW_ERR "Load Store Error");
=======
	pr_emerg(HW_ERR "MC3 Error");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (xec == 0x0) {
		u8 r4 = R4(ec);

		if (!BUS_ERROR(ec) || (r4 != R4_DRD && r4 != R4_DWR))
<<<<<<< HEAD
			goto wrong_ls_mce;

		pr_cont(" during %s.\n", R4_MSG(ec));
	} else
		goto wrong_ls_mce;

	return;

wrong_ls_mce:
	pr_emerg(HW_ERR "Corrupted LS MCE info?\n");
}

void amd_decode_nb_mce(struct mce *m)
{
	struct cpuinfo_x86 *c = &boot_cpu_data;
	int node_id = amd_get_nb_id(m->extcpu);
=======
			goto wrong_mc3_mce;

		pr_cont(" during %s.\n", R4_MSG(ec));
	} else
		goto wrong_mc3_mce;

	return;

 wrong_mc3_mce:
	pr_emerg(HW_ERR "Corrupted MC3 MCE info?\n");
}

static void decode_mc4_mce(struct mce *m)
{
	unsigned int fam = x86_family(m->cpuid);
	int node_id = topology_amd_node_id(m->extcpu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u16 ec = EC(m->status);
	u8 xec = XEC(m->status, 0x1f);
	u8 offset = 0;

<<<<<<< HEAD
	pr_emerg(HW_ERR "Northbridge Error (node %d): ", node_id);
=======
	pr_emerg(HW_ERR "MC4 Error (node %d): ", node_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (xec) {
	case 0x0 ... 0xe:

		/* special handling for DRAM ECCs */
		if (xec == 0x0 || xec == 0x8) {
			/* no ECCs on F11h */
<<<<<<< HEAD
			if (c->x86 == 0x11)
				goto wrong_nb_mce;

			pr_cont("%s.\n", nb_mce_desc[xec]);

			if (nb_bus_decoder)
				nb_bus_decoder(node_id, m);
=======
			if (fam == 0x11)
				goto wrong_mc4_mce;

			pr_cont("%s.\n", mc4_mce_desc[xec]);

			if (decode_dram_ecc)
				decode_dram_ecc(node_id, m);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return;
		}
		break;

	case 0xf:
		if (TLB_ERROR(ec))
			pr_cont("GART Table Walk data error.\n");
		else if (BUS_ERROR(ec))
			pr_cont("DMA Exclusion Vector Table Walk error.\n");
		else
<<<<<<< HEAD
			goto wrong_nb_mce;
		return;

	case 0x19:
		if (boot_cpu_data.x86 == 0x15)
			pr_cont("Compute Unit Data Error.\n");
		else
			goto wrong_nb_mce;
=======
			goto wrong_mc4_mce;
		return;

	case 0x19:
		if (fam == 0x15 || fam == 0x16)
			pr_cont("Compute Unit Data Error.\n");
		else
			goto wrong_mc4_mce;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;

	case 0x1c ... 0x1f:
		offset = 13;
		break;

	default:
<<<<<<< HEAD
		goto wrong_nb_mce;
	}

	pr_cont("%s.\n", nb_mce_desc[xec - offset]);
	return;

wrong_nb_mce:
	pr_emerg(HW_ERR "Corrupted NB MCE info?\n");
}
EXPORT_SYMBOL_GPL(amd_decode_nb_mce);

static void amd_decode_fr_mce(struct mce *m)
{
	struct cpuinfo_x86 *c = &boot_cpu_data;
	u8 xec = XEC(m->status, xec_mask);

	if (c->x86 == 0xf || c->x86 == 0x11)
		goto wrong_fr_mce;

	pr_emerg(HW_ERR "%s Error: ",
		 (c->x86 == 0x15 ? "Execution Unit" : "FIROB"));

	if (xec == 0x0 || xec == 0xc)
		pr_cont("%s.\n", fr_ex_mce_desc[xec]);
	else if (xec < 0xd)
		pr_cont("%s parity error.\n", fr_ex_mce_desc[xec]);
	else
		goto wrong_fr_mce;

	return;

wrong_fr_mce:
	pr_emerg(HW_ERR "Corrupted FR MCE info?\n");
}

static void amd_decode_fp_mce(struct mce *m)
{
	u8 xec = XEC(m->status, xec_mask);

	pr_emerg(HW_ERR "Floating Point Unit Error: ");

	switch (xec) {
	case 0x1:
		pr_cont("Free List");
		break;

	case 0x2:
		pr_cont("Physical Register File");
		break;

	case 0x3:
		pr_cont("Retire Queue");
		break;

	case 0x4:
		pr_cont("Scheduler table");
		break;

	case 0x5:
		pr_cont("Status Register File");
		break;

	default:
		goto wrong_fp_mce;
		break;
	}

	pr_cont(" parity error.\n");

	return;

wrong_fp_mce:
	pr_emerg(HW_ERR "Corrupted FP MCE info?\n");
=======
		goto wrong_mc4_mce;
	}

	pr_cont("%s.\n", mc4_mce_desc[xec - offset]);
	return;

 wrong_mc4_mce:
	pr_emerg(HW_ERR "Corrupted MC4 MCE info?\n");
}

static void decode_mc5_mce(struct mce *m)
{
	unsigned int fam = x86_family(m->cpuid);
	u16 ec = EC(m->status);
	u8 xec = XEC(m->status, xec_mask);

	if (fam == 0xf || fam == 0x11)
		goto wrong_mc5_mce;

	pr_emerg(HW_ERR "MC5 Error: ");

	if (INT_ERROR(ec)) {
		if (xec <= 0x1f) {
			pr_cont("Hardware Assert.\n");
			return;
		} else
			goto wrong_mc5_mce;
	}

	if (xec == 0x0 || xec == 0xc)
		pr_cont("%s.\n", mc5_mce_desc[xec]);
	else if (xec <= 0xd)
		pr_cont("%s parity error.\n", mc5_mce_desc[xec]);
	else
		goto wrong_mc5_mce;

	return;

 wrong_mc5_mce:
	pr_emerg(HW_ERR "Corrupted MC5 MCE info?\n");
}

static void decode_mc6_mce(struct mce *m)
{
	u8 xec = XEC(m->status, xec_mask);

	pr_emerg(HW_ERR "MC6 Error: ");

	if (xec > 0x5)
		goto wrong_mc6_mce;

	pr_cont("%s parity error.\n", mc6_mce_desc[xec]);
	return;

 wrong_mc6_mce:
	pr_emerg(HW_ERR "Corrupted MC6 MCE info?\n");
}

static const char * const smca_long_names[] = {
	[SMCA_LS ... SMCA_LS_V2]	= "Load Store Unit",
	[SMCA_IF]			= "Instruction Fetch Unit",
	[SMCA_L2_CACHE]			= "L2 Cache",
	[SMCA_DE]			= "Decode Unit",
	[SMCA_RESERVED]			= "Reserved",
	[SMCA_EX]			= "Execution Unit",
	[SMCA_FP]			= "Floating Point Unit",
	[SMCA_L3_CACHE]			= "L3 Cache",
	[SMCA_CS ... SMCA_CS_V2]	= "Coherent Slave",
	[SMCA_PIE]			= "Power, Interrupts, etc.",

	/* UMC v2 is separate because both of them can exist in a single system. */
	[SMCA_UMC]			= "Unified Memory Controller",
	[SMCA_UMC_V2]			= "Unified Memory Controller v2",
	[SMCA_PB]			= "Parameter Block",
	[SMCA_PSP ... SMCA_PSP_V2]	= "Platform Security Processor",
	[SMCA_SMU ... SMCA_SMU_V2]	= "System Management Unit",
	[SMCA_MP5]			= "Microprocessor 5 Unit",
	[SMCA_MPDMA]			= "MPDMA Unit",
	[SMCA_NBIO]			= "Northbridge IO Unit",
	[SMCA_PCIE ... SMCA_PCIE_V2]	= "PCI Express Unit",
	[SMCA_XGMI_PCS]			= "Ext Global Memory Interconnect PCS Unit",
	[SMCA_NBIF]			= "NBIF Unit",
	[SMCA_SHUB]			= "System Hub Unit",
	[SMCA_SATA]			= "SATA Unit",
	[SMCA_USB]			= "USB Unit",
	[SMCA_GMI_PCS]			= "Global Memory Interconnect PCS Unit",
	[SMCA_XGMI_PHY]			= "Ext Global Memory Interconnect PHY Unit",
	[SMCA_WAFL_PHY]			= "WAFL PHY Unit",
	[SMCA_GMI_PHY]			= "Global Memory Interconnect PHY Unit",
};

static const char *smca_get_long_name(enum smca_bank_types t)
{
	if (t >= N_SMCA_BANK_TYPES)
		return NULL;

	return smca_long_names[t];
}

/* Decode errors according to Scalable MCA specification */
static void decode_smca_error(struct mce *m)
{
	enum smca_bank_types bank_type = smca_get_bank_type(m->extcpu, m->bank);
	u8 xec = XEC(m->status, xec_mask);

	if (bank_type >= N_SMCA_BANK_TYPES)
		return;

	if (bank_type == SMCA_RESERVED) {
		pr_emerg(HW_ERR "Bank %d is reserved.\n", m->bank);
		return;
	}

	pr_emerg(HW_ERR "%s Ext. Error Code: %d", smca_get_long_name(bank_type), xec);

	if ((bank_type == SMCA_UMC || bank_type == SMCA_UMC_V2) &&
	    xec == 0 && decode_dram_ecc)
		decode_dram_ecc(topology_amd_node_id(m->extcpu), m);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void amd_decode_err_code(u16 ec)
{
<<<<<<< HEAD
=======
	if (INT_ERROR(ec)) {
		pr_emerg(HW_ERR "internal: %s\n", UU_MSG(ec));
		return;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	pr_emerg(HW_ERR "cache level: %s", LL_MSG(ec));

	if (BUS_ERROR(ec))
		pr_cont(", mem/io: %s", II_MSG(ec));
	else
		pr_cont(", tx: %s", TT_MSG(ec));

	if (MEM_ERROR(ec) || BUS_ERROR(ec)) {
		pr_cont(", mem-tx: %s", R4_MSG(ec));

		if (BUS_ERROR(ec))
			pr_cont(", part-proc: %s (%s)", PP_MSG(ec), TO_MSG(ec));
	}

	pr_cont("\n");
}

<<<<<<< HEAD
/*
 * Filter out unwanted MCE signatures here.
 */
static bool amd_filter_mce(struct mce *m)
{
	u8 xec = (m->status >> 16) & 0x1f;

	/*
	 * NB GART TLB error reporting is disabled by default.
	 */
	if (m->bank == 4 && xec == 0x5 && !report_gart_errors)
		return true;

	return false;
}

int amd_decode_mce(struct notifier_block *nb, unsigned long val, void *data)
{
	struct mce *m = (struct mce *)data;
	struct cpuinfo_x86 *c = &boot_cpu_data;
	int ecc;

	if (amd_filter_mce(m))
		return NOTIFY_STOP;

	pr_emerg(HW_ERR "CPU:%d\tMC%d_STATUS[%s|%s|%s|%s|%s",
		m->extcpu, m->bank,
		((m->status & MCI_STATUS_OVER)	? "Over"  : "-"),
		((m->status & MCI_STATUS_UC)	? "UE"	  : "CE"),
		((m->status & MCI_STATUS_MISCV)	? "MiscV" : "-"),
		((m->status & MCI_STATUS_PCC)	? "PCC"	  : "-"),
		((m->status & MCI_STATUS_ADDRV)	? "AddrV" : "-"));

	if (c->x86 == 0x15)
		pr_cont("|%s|%s",
			((m->status & BIT_64(44)) ? "Deferred" : "-"),
			((m->status & BIT_64(43)) ? "Poison"   : "-"));
=======
static const char *decode_error_status(struct mce *m)
{
	if (m->status & MCI_STATUS_UC) {
		if (m->status & MCI_STATUS_PCC)
			return "System Fatal error.";
		if (m->mcgstatus & MCG_STATUS_RIPV)
			return "Uncorrected, software restartable error.";
		return "Uncorrected, software containable error.";
	}

	if (m->status & MCI_STATUS_DEFERRED)
		return "Deferred error, no action required.";

	return "Corrected error, no action required.";
}

static int
amd_decode_mce(struct notifier_block *nb, unsigned long val, void *data)
{
	struct mce *m = (struct mce *)data;
	unsigned int fam = x86_family(m->cpuid);
	int ecc;

	if (m->kflags & MCE_HANDLED_CEC)
		return NOTIFY_DONE;

	pr_emerg(HW_ERR "%s\n", decode_error_status(m));

	pr_emerg(HW_ERR "CPU:%d (%x:%x:%x) MC%d_STATUS[%s|%s|%s|%s|%s",
		m->extcpu,
		fam, x86_model(m->cpuid), x86_stepping(m->cpuid),
		m->bank,
		((m->status & MCI_STATUS_OVER)	? "Over"  : "-"),
		((m->status & MCI_STATUS_UC)	? "UE"	  :
		 (m->status & MCI_STATUS_DEFERRED) ? "-"  : "CE"),
		((m->status & MCI_STATUS_MISCV)	? "MiscV" : "-"),
		((m->status & MCI_STATUS_ADDRV)	? "AddrV" : "-"),
		((m->status & MCI_STATUS_PCC)	? "PCC"	  : "-"));

	if (boot_cpu_has(X86_FEATURE_SMCA)) {
		u32 low, high;
		u32 addr = MSR_AMD64_SMCA_MCx_CONFIG(m->bank);

		if (!rdmsr_safe(addr, &low, &high) &&
		    (low & MCI_CONFIG_MCAX))
			pr_cont("|%s", ((m->status & MCI_STATUS_TCC) ? "TCC" : "-"));

		pr_cont("|%s", ((m->status & MCI_STATUS_SYNDV) ? "SyndV" : "-"));
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* do the two bits[14:13] together */
	ecc = (m->status >> 45) & 0x3;
	if (ecc)
		pr_cont("|%sECC", ((ecc == 2) ? "C" : "U"));

<<<<<<< HEAD
	pr_cont("]: 0x%016llx\n", m->status);

	if (m->status & MCI_STATUS_ADDRV)
		pr_emerg(HW_ERR "\tMC%d_ADDR: 0x%016llx\n", m->bank, m->addr);

	switch (m->bank) {
	case 0:
		amd_decode_dc_mce(m);
		break;

	case 1:
		amd_decode_ic_mce(m);
		break;

	case 2:
		if (c->x86 == 0x15)
			amd_decode_cu_mce(m);
		else
			amd_decode_bu_mce(m);
		break;

	case 3:
		amd_decode_ls_mce(m);
		break;

	case 4:
		amd_decode_nb_mce(m);
		break;

	case 5:
		amd_decode_fr_mce(m);
		break;

	case 6:
		amd_decode_fp_mce(m);
=======
	if (fam >= 0x15) {
		pr_cont("|%s", (m->status & MCI_STATUS_DEFERRED ? "Deferred" : "-"));

		/* F15h, bank4, bit 43 is part of McaStatSubCache. */
		if (fam != 0x15 || m->bank != 4)
			pr_cont("|%s", (m->status & MCI_STATUS_POISON ? "Poison" : "-"));
	}

	if (fam >= 0x17)
		pr_cont("|%s", (m->status & MCI_STATUS_SCRUB ? "Scrub" : "-"));

	pr_cont("]: 0x%016llx\n", m->status);

	if (m->status & MCI_STATUS_ADDRV)
		pr_emerg(HW_ERR "Error Addr: 0x%016llx\n", m->addr);

	if (m->ppin)
		pr_emerg(HW_ERR "PPIN: 0x%016llx\n", m->ppin);

	if (boot_cpu_has(X86_FEATURE_SMCA)) {
		pr_emerg(HW_ERR "IPID: 0x%016llx", m->ipid);

		if (m->status & MCI_STATUS_SYNDV)
			pr_cont(", Syndrome: 0x%016llx", m->synd);

		pr_cont("\n");

		decode_smca_error(m);
		goto err_code;
	}

	if (m->tsc)
		pr_emerg(HW_ERR "TSC: %llu\n", m->tsc);

	/* Doesn't matter which member to test. */
	if (!fam_ops.mc0_mce)
		goto err_code;

	switch (m->bank) {
	case 0:
		decode_mc0_mce(m);
		break;

	case 1:
		decode_mc1_mce(m);
		break;

	case 2:
		decode_mc2_mce(m);
		break;

	case 3:
		decode_mc3_mce(m);
		break;

	case 4:
		decode_mc4_mce(m);
		break;

	case 5:
		decode_mc5_mce(m);
		break;

	case 6:
		decode_mc6_mce(m);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;

	default:
		break;
	}

<<<<<<< HEAD
	amd_decode_err_code(m->status & 0xffff);

	return NOTIFY_STOP;
}
EXPORT_SYMBOL_GPL(amd_decode_mce);

static struct notifier_block amd_mce_dec_nb = {
	.notifier_call	= amd_decode_mce,
=======
 err_code:
	amd_decode_err_code(m->status & 0xffff);

	m->kflags |= MCE_HANDLED_EDAC;
	return NOTIFY_OK;
}

static struct notifier_block amd_mce_dec_nb = {
	.notifier_call	= amd_decode_mce,
	.priority	= MCE_PRIO_EDAC,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int __init mce_amd_init(void)
{
	struct cpuinfo_x86 *c = &boot_cpu_data;

<<<<<<< HEAD
	if (c->x86_vendor != X86_VENDOR_AMD)
		return 0;

	if (c->x86 < 0xf || c->x86 > 0x15)
		return 0;

	fam_ops = kzalloc(sizeof(struct amd_decoder_ops), GFP_KERNEL);
	if (!fam_ops)
		return -ENOMEM;

	switch (c->x86) {
	case 0xf:
		fam_ops->dc_mce = k8_dc_mce;
		fam_ops->ic_mce = k8_ic_mce;
		break;

	case 0x10:
		fam_ops->dc_mce = f10h_dc_mce;
		fam_ops->ic_mce = k8_ic_mce;
		break;

	case 0x11:
		fam_ops->dc_mce = k8_dc_mce;
		fam_ops->ic_mce = k8_ic_mce;
		break;

	case 0x12:
		fam_ops->dc_mce = f12h_dc_mce;
		fam_ops->ic_mce = k8_ic_mce;
		break;

	case 0x14:
		nb_err_cpumask  = 0x3;
		fam_ops->dc_mce = f14h_dc_mce;
		fam_ops->ic_mce = f14h_ic_mce;
		break;

	case 0x15:
		xec_mask = 0x1f;
		fam_ops->dc_mce = f15h_dc_mce;
		fam_ops->ic_mce = f15h_ic_mce;
		break;

	default:
		printk(KERN_WARNING "Huh? What family is it: 0x%x?!\n", c->x86);
		kfree(fam_ops);
		return -EINVAL;
	}

=======
	if (c->x86_vendor != X86_VENDOR_AMD &&
	    c->x86_vendor != X86_VENDOR_HYGON)
		return -ENODEV;

	if (cpu_feature_enabled(X86_FEATURE_HYPERVISOR))
		return -ENODEV;

	if (boot_cpu_has(X86_FEATURE_SMCA)) {
		xec_mask = 0x3f;
		goto out;
	}

	switch (c->x86) {
	case 0xf:
		fam_ops.mc0_mce = k8_mc0_mce;
		fam_ops.mc1_mce = k8_mc1_mce;
		fam_ops.mc2_mce = k8_mc2_mce;
		break;

	case 0x10:
		fam_ops.mc0_mce = f10h_mc0_mce;
		fam_ops.mc1_mce = k8_mc1_mce;
		fam_ops.mc2_mce = k8_mc2_mce;
		break;

	case 0x11:
		fam_ops.mc0_mce = k8_mc0_mce;
		fam_ops.mc1_mce = k8_mc1_mce;
		fam_ops.mc2_mce = k8_mc2_mce;
		break;

	case 0x12:
		fam_ops.mc0_mce = f12h_mc0_mce;
		fam_ops.mc1_mce = k8_mc1_mce;
		fam_ops.mc2_mce = k8_mc2_mce;
		break;

	case 0x14:
		fam_ops.mc0_mce = cat_mc0_mce;
		fam_ops.mc1_mce = cat_mc1_mce;
		fam_ops.mc2_mce = k8_mc2_mce;
		break;

	case 0x15:
		xec_mask = c->x86_model == 0x60 ? 0x3f : 0x1f;

		fam_ops.mc0_mce = f15h_mc0_mce;
		fam_ops.mc1_mce = f15h_mc1_mce;
		fam_ops.mc2_mce = f15h_mc2_mce;
		break;

	case 0x16:
		xec_mask = 0x1f;
		fam_ops.mc0_mce = cat_mc0_mce;
		fam_ops.mc1_mce = cat_mc1_mce;
		fam_ops.mc2_mce = f16h_mc2_mce;
		break;

	case 0x17:
	case 0x18:
		pr_warn_once("Decoding supported only on Scalable MCA processors.\n");
		return -EINVAL;

	default:
		printk(KERN_WARNING "Huh? What family is it: 0x%x?!\n", c->x86);
		return -EINVAL;
	}

out:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pr_info("MCE: In-kernel MCE decoding enabled.\n");

	mce_register_decode_chain(&amd_mce_dec_nb);

	return 0;
}
early_initcall(mce_amd_init);

#ifdef MODULE
static void __exit mce_amd_exit(void)
{
	mce_unregister_decode_chain(&amd_mce_dec_nb);
<<<<<<< HEAD
	kfree(fam_ops);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

MODULE_DESCRIPTION("AMD MCE decoder");
MODULE_ALIAS("edac-mce-amd");
MODULE_LICENSE("GPL");
module_exit(mce_amd_exit);
#endif
