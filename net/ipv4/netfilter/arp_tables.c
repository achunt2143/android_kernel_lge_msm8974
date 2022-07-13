<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Packet matching code for ARP packets.
 *
 * Based heavily, if not almost entirely, upon ip_tables.c framework.
 *
 * Some ARP specific bits are:
 *
 * Copyright (C) 2002 David S. Miller (davem@redhat.com)
<<<<<<< HEAD
=======
 * Copyright (C) 2006-2009 Patrick McHardy <kaber@trash.net>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/capability.h>
#include <linux/if_arp.h>
#include <linux/kmod.h>
#include <linux/vmalloc.h>
#include <linux/proc_fs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mutex.h>
#include <linux/err.h>
#include <net/compat.h>
#include <net/sock.h>
<<<<<<< HEAD
#include <asm/uaccess.h>
=======
#include <linux/uaccess.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_arp/arp_tables.h>
#include "../../netfilter/xt_repldata.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("David S. Miller <davem@redhat.com>");
MODULE_DESCRIPTION("arptables core");

<<<<<<< HEAD
/*#define DEBUG_ARP_TABLES*/
/*#define DEBUG_ARP_TABLES_USER*/

#ifdef DEBUG_ARP_TABLES
#define dprintf(format, args...)  printk(format , ## args)
#else
#define dprintf(format, args...)
#endif

#ifdef DEBUG_ARP_TABLES_USER
#define duprintf(format, args...) printk(format , ## args)
#else
#define duprintf(format, args...)
#endif

#ifdef CONFIG_NETFILTER_DEBUG
#define ARP_NF_ASSERT(x)	WARN_ON(!(x))
#else
#define ARP_NF_ASSERT(x)
#endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void *arpt_alloc_initial_table(const struct xt_table *info)
{
	return xt_alloc_initial_table(arpt, ARPT);
}
EXPORT_SYMBOL_GPL(arpt_alloc_initial_table);

static inline int arp_devaddr_compare(const struct arpt_devaddr_info *ap,
				      const char *hdr_addr, int len)
{
	int i, ret;

	if (len > ARPT_DEV_ADDR_LEN_MAX)
		len = ARPT_DEV_ADDR_LEN_MAX;

	ret = 0;
	for (i = 0; i < len; i++)
		ret |= (hdr_addr[i] ^ ap->addr[i]) & ap->mask[i];

	return ret != 0;
}

/*
 * Unfortunately, _b and _mask are not aligned to an int (or long int)
 * Some arches dont care, unrolling the loop is a win on them.
 * For other arches, we only have a 16bit alignement.
 */
static unsigned long ifname_compare(const char *_a, const char *_b, const char *_mask)
{
#ifdef CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS
	unsigned long ret = ifname_compare_aligned(_a, _b, _mask);
#else
	unsigned long ret = 0;
	const u16 *a = (const u16 *)_a;
	const u16 *b = (const u16 *)_b;
	const u16 *mask = (const u16 *)_mask;
	int i;

	for (i = 0; i < IFNAMSIZ/sizeof(u16); i++)
		ret |= (a[i] ^ b[i]) & mask[i];
#endif
	return ret;
}

/* Returns whether packet matches rule or not. */
static inline int arp_packet_match(const struct arphdr *arphdr,
				   struct net_device *dev,
				   const char *indev,
				   const char *outdev,
				   const struct arpt_arp *arpinfo)
{
	const char *arpptr = (char *)(arphdr + 1);
	const char *src_devaddr, *tgt_devaddr;
	__be32 src_ipaddr, tgt_ipaddr;
	long ret;

<<<<<<< HEAD
#define FWINV(bool, invflg) ((bool) ^ !!(arpinfo->invflags & (invflg)))

	if (FWINV((arphdr->ar_op & arpinfo->arpop_mask) != arpinfo->arpop,
		  ARPT_INV_ARPOP)) {
		dprintf("ARP operation field mismatch.\n");
		dprintf("ar_op: %04x info->arpop: %04x info->arpop_mask: %04x\n",
			arphdr->ar_op, arpinfo->arpop, arpinfo->arpop_mask);
		return 0;
	}

	if (FWINV((arphdr->ar_hrd & arpinfo->arhrd_mask) != arpinfo->arhrd,
		  ARPT_INV_ARPHRD)) {
		dprintf("ARP hardware address format mismatch.\n");
		dprintf("ar_hrd: %04x info->arhrd: %04x info->arhrd_mask: %04x\n",
			arphdr->ar_hrd, arpinfo->arhrd, arpinfo->arhrd_mask);
		return 0;
	}

	if (FWINV((arphdr->ar_pro & arpinfo->arpro_mask) != arpinfo->arpro,
		  ARPT_INV_ARPPRO)) {
		dprintf("ARP protocol address format mismatch.\n");
		dprintf("ar_pro: %04x info->arpro: %04x info->arpro_mask: %04x\n",
			arphdr->ar_pro, arpinfo->arpro, arpinfo->arpro_mask);
		return 0;
	}

	if (FWINV((arphdr->ar_hln & arpinfo->arhln_mask) != arpinfo->arhln,
		  ARPT_INV_ARPHLN)) {
		dprintf("ARP hardware address length mismatch.\n");
		dprintf("ar_hln: %02x info->arhln: %02x info->arhln_mask: %02x\n",
			arphdr->ar_hln, arpinfo->arhln, arpinfo->arhln_mask);
		return 0;
	}
=======
	if (NF_INVF(arpinfo, ARPT_INV_ARPOP,
		    (arphdr->ar_op & arpinfo->arpop_mask) != arpinfo->arpop))
		return 0;

	if (NF_INVF(arpinfo, ARPT_INV_ARPHRD,
		    (arphdr->ar_hrd & arpinfo->arhrd_mask) != arpinfo->arhrd))
		return 0;

	if (NF_INVF(arpinfo, ARPT_INV_ARPPRO,
		    (arphdr->ar_pro & arpinfo->arpro_mask) != arpinfo->arpro))
		return 0;

	if (NF_INVF(arpinfo, ARPT_INV_ARPHLN,
		    (arphdr->ar_hln & arpinfo->arhln_mask) != arpinfo->arhln))
		return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	src_devaddr = arpptr;
	arpptr += dev->addr_len;
	memcpy(&src_ipaddr, arpptr, sizeof(u32));
	arpptr += sizeof(u32);
	tgt_devaddr = arpptr;
	arpptr += dev->addr_len;
	memcpy(&tgt_ipaddr, arpptr, sizeof(u32));

<<<<<<< HEAD
	if (FWINV(arp_devaddr_compare(&arpinfo->src_devaddr, src_devaddr, dev->addr_len),
		  ARPT_INV_SRCDEVADDR) ||
	    FWINV(arp_devaddr_compare(&arpinfo->tgt_devaddr, tgt_devaddr, dev->addr_len),
		  ARPT_INV_TGTDEVADDR)) {
		dprintf("Source or target device address mismatch.\n");

		return 0;
	}

	if (FWINV((src_ipaddr & arpinfo->smsk.s_addr) != arpinfo->src.s_addr,
		  ARPT_INV_SRCIP) ||
	    FWINV(((tgt_ipaddr & arpinfo->tmsk.s_addr) != arpinfo->tgt.s_addr),
		  ARPT_INV_TGTIP)) {
		dprintf("Source or target IP address mismatch.\n");

		dprintf("SRC: %pI4. Mask: %pI4. Target: %pI4.%s\n",
			&src_ipaddr,
			&arpinfo->smsk.s_addr,
			&arpinfo->src.s_addr,
			arpinfo->invflags & ARPT_INV_SRCIP ? " (INV)" : "");
		dprintf("TGT: %pI4 Mask: %pI4 Target: %pI4.%s\n",
			&tgt_ipaddr,
			&arpinfo->tmsk.s_addr,
			&arpinfo->tgt.s_addr,
			arpinfo->invflags & ARPT_INV_TGTIP ? " (INV)" : "");
		return 0;
	}
=======
	if (NF_INVF(arpinfo, ARPT_INV_SRCDEVADDR,
		    arp_devaddr_compare(&arpinfo->src_devaddr, src_devaddr,
					dev->addr_len)) ||
	    NF_INVF(arpinfo, ARPT_INV_TGTDEVADDR,
		    arp_devaddr_compare(&arpinfo->tgt_devaddr, tgt_devaddr,
					dev->addr_len)))
		return 0;

	if (NF_INVF(arpinfo, ARPT_INV_SRCIP,
		    (src_ipaddr & arpinfo->smsk.s_addr) != arpinfo->src.s_addr) ||
	    NF_INVF(arpinfo, ARPT_INV_TGTIP,
		    (tgt_ipaddr & arpinfo->tmsk.s_addr) != arpinfo->tgt.s_addr))
		return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Look for ifname matches.  */
	ret = ifname_compare(indev, arpinfo->iniface, arpinfo->iniface_mask);

<<<<<<< HEAD
	if (FWINV(ret != 0, ARPT_INV_VIA_IN)) {
		dprintf("VIA in mismatch (%s vs %s).%s\n",
			indev, arpinfo->iniface,
			arpinfo->invflags&ARPT_INV_VIA_IN ?" (INV)":"");
		return 0;
	}

	ret = ifname_compare(outdev, arpinfo->outiface, arpinfo->outiface_mask);

	if (FWINV(ret != 0, ARPT_INV_VIA_OUT)) {
		dprintf("VIA out mismatch (%s vs %s).%s\n",
			outdev, arpinfo->outiface,
			arpinfo->invflags&ARPT_INV_VIA_OUT ?" (INV)":"");
		return 0;
	}

	return 1;
#undef FWINV
=======
	if (NF_INVF(arpinfo, ARPT_INV_VIA_IN, ret != 0))
		return 0;

	ret = ifname_compare(outdev, arpinfo->outiface, arpinfo->outiface_mask);

	if (NF_INVF(arpinfo, ARPT_INV_VIA_OUT, ret != 0))
		return 0;

	return 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int arp_checkentry(const struct arpt_arp *arp)
{
<<<<<<< HEAD
	if (arp->flags & ~ARPT_F_MASK) {
		duprintf("Unknown flag bits set: %08X\n",
			 arp->flags & ~ARPT_F_MASK);
		return 0;
	}
	if (arp->invflags & ~ARPT_INV_MASK) {
		duprintf("Unknown invflag bits set: %08X\n",
			 arp->invflags & ~ARPT_INV_MASK);
		return 0;
	}
=======
	if (arp->flags & ~ARPT_F_MASK)
		return 0;
	if (arp->invflags & ~ARPT_INV_MASK)
		return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 1;
}

static unsigned int
arpt_error(struct sk_buff *skb, const struct xt_action_param *par)
{
<<<<<<< HEAD
	if (net_ratelimit())
		pr_err("arp_tables: error: '%s'\n",
		       (const char *)par->targinfo);
=======
	net_err_ratelimited("arp_tables: error: '%s'\n",
			    (const char *)par->targinfo);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return NF_DROP;
}

static inline const struct xt_entry_target *
arpt_get_target_c(const struct arpt_entry *e)
{
	return arpt_get_target((struct arpt_entry *)e);
}

static inline struct arpt_entry *
get_entry(const void *base, unsigned int offset)
{
	return (struct arpt_entry *)(base + offset);
}

<<<<<<< HEAD
static inline __pure
=======
static inline
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct arpt_entry *arpt_next_entry(const struct arpt_entry *entry)
{
	return (void *)entry + entry->next_offset;
}

<<<<<<< HEAD
unsigned int arpt_do_table(struct sk_buff *skb,
			   unsigned int hook,
			   const struct net_device *in,
			   const struct net_device *out,
			   struct xt_table *table)
{
	static const char nulldevname[IFNAMSIZ] __attribute__((aligned(sizeof(long))));
	unsigned int verdict = NF_DROP;
	const struct arphdr *arp;
	struct arpt_entry *e, *back;
	const char *indev, *outdev;
	void *table_base;
=======
unsigned int arpt_do_table(void *priv,
			   struct sk_buff *skb,
			   const struct nf_hook_state *state)
{
	const struct xt_table *table = priv;
	unsigned int hook = state->hook;
	static const char nulldevname[IFNAMSIZ] __attribute__((aligned(sizeof(long))));
	unsigned int verdict = NF_DROP;
	const struct arphdr *arp;
	struct arpt_entry *e, **jumpstack;
	const char *indev, *outdev;
	const void *table_base;
	unsigned int cpu, stackidx = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	const struct xt_table_info *private;
	struct xt_action_param acpar;
	unsigned int addend;

	if (!pskb_may_pull(skb, arp_hdr_len(skb->dev)))
		return NF_DROP;

<<<<<<< HEAD
	indev = in ? in->name : nulldevname;
	outdev = out ? out->name : nulldevname;

	local_bh_disable();
	addend = xt_write_recseq_begin();
	private = table->private;
	/*
	 * Ensure we load private-> members after we've fetched the base
	 * pointer.
	 */
	smp_read_barrier_depends();
	table_base = private->entries[smp_processor_id()];

	e = get_entry(table_base, private->hook_entry[hook]);
	back = get_entry(table_base, private->underflow[hook]);

	acpar.in      = in;
	acpar.out     = out;
	acpar.hooknum = hook;
	acpar.family  = NFPROTO_ARP;
=======
	indev = state->in ? state->in->name : nulldevname;
	outdev = state->out ? state->out->name : nulldevname;

	local_bh_disable();
	addend = xt_write_recseq_begin();
	private = READ_ONCE(table->private); /* Address dependency. */
	cpu     = smp_processor_id();
	table_base = private->entries;
	jumpstack  = (struct arpt_entry **)private->jumpstack[cpu];

	/* No TEE support for arptables, so no need to switch to alternate
	 * stack.  All targets that reenter must return absolute verdicts.
	 */
	e = get_entry(table_base, private->hook_entry[hook]);

	acpar.state   = state;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	acpar.hotdrop = false;

	arp = arp_hdr(skb);
	do {
		const struct xt_entry_target *t;
<<<<<<< HEAD
=======
		struct xt_counters *counter;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (!arp_packet_match(arp, skb->dev, indev, outdev, &e->arp)) {
			e = arpt_next_entry(e);
			continue;
		}

<<<<<<< HEAD
		ADD_COUNTER(e->counters, arp_hdr_len(skb->dev), 1);
=======
		counter = xt_get_this_cpu_counter(&e->counters);
		ADD_COUNTER(*counter, arp_hdr_len(skb->dev), 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		t = arpt_get_target_c(e);

		/* Standard target? */
		if (!t->u.kernel.target->target) {
			int v;

			v = ((struct xt_standard_target *)t)->verdict;
			if (v < 0) {
				/* Pop from stack? */
				if (v != XT_RETURN) {
<<<<<<< HEAD
					verdict = (unsigned)(-v) - 1;
					break;
				}
				e = back;
				back = get_entry(table_base, back->comefrom);
=======
					verdict = (unsigned int)(-v) - 1;
					break;
				}
				if (stackidx == 0) {
					e = get_entry(table_base,
						      private->underflow[hook]);
				} else {
					e = jumpstack[--stackidx];
					e = arpt_next_entry(e);
				}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				continue;
			}
			if (table_base + v
			    != arpt_next_entry(e)) {
<<<<<<< HEAD
				/* Save old back ptr in next entry */
				struct arpt_entry *next = arpt_next_entry(e);
				next->comefrom = (void *)back - table_base;

				/* set back pointer to next entry */
				back = next;
=======
				if (unlikely(stackidx >= private->stacksize)) {
					verdict = NF_DROP;
					break;
				}
				jumpstack[stackidx++] = e;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}

			e = get_entry(table_base, v);
			continue;
		}

<<<<<<< HEAD
		/* Targets which reenter must return
		 * abs. verdicts
		 */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		acpar.target   = t->u.kernel.target;
		acpar.targinfo = t->data;
		verdict = t->u.kernel.target->target(skb, &acpar);

<<<<<<< HEAD
		/* Target might have changed stuff. */
		arp = arp_hdr(skb);

		if (verdict == XT_CONTINUE)
			e = arpt_next_entry(e);
		else
			/* Verdict */
			break;
=======
		if (verdict == XT_CONTINUE) {
			/* Target might have changed stuff. */
			arp = arp_hdr(skb);
			e = arpt_next_entry(e);
		} else {
			/* Verdict */
			break;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} while (!acpar.hotdrop);
	xt_write_recseq_end(addend);
	local_bh_enable();

	if (acpar.hotdrop)
		return NF_DROP;
	else
		return verdict;
}

/* All zeroes == unconditional rule. */
static inline bool unconditional(const struct arpt_entry *e)
{
	static const struct arpt_arp uncond;

	return e->target_offset == sizeof(struct arpt_entry) &&
	       memcmp(&e->arp, &uncond, sizeof(uncond)) == 0;
}

/* Figures out from what hook each rule can be called: returns 0 if
 * there are loops.  Puts hook bitmask in comefrom.
 */
static int mark_source_chains(const struct xt_table_info *newinfo,
<<<<<<< HEAD
			      unsigned int valid_hooks, void *entry0)
=======
			      unsigned int valid_hooks, void *entry0,
			      unsigned int *offsets)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned int hook;

	/* No recursion; use packet counter to save back ptrs (reset
	 * to 0 as we leave), and comefrom to save source hook bitmask.
	 */
	for (hook = 0; hook < NF_ARP_NUMHOOKS; hook++) {
		unsigned int pos = newinfo->hook_entry[hook];
<<<<<<< HEAD
		struct arpt_entry *e
			= (struct arpt_entry *)(entry0 + pos);
=======
		struct arpt_entry *e = entry0 + pos;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (!(valid_hooks & (1 << hook)))
			continue;

		/* Set initial back pointer. */
		e->counters.pcnt = pos;

		for (;;) {
			const struct xt_standard_target *t
				= (void *)arpt_get_target_c(e);
			int visited = e->comefrom & (1 << hook);

<<<<<<< HEAD
			if (e->comefrom & (1 << NF_ARP_NUMHOOKS)) {
				pr_notice("arptables: loop hook %u pos %u %08X.\n",
				       hook, pos, e->comefrom);
				return 0;
			}
=======
			if (e->comefrom & (1 << NF_ARP_NUMHOOKS))
				return 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			e->comefrom
				|= ((1 << hook) | (1 << NF_ARP_NUMHOOKS));

			/* Unconditional return/END. */
			if ((unconditional(e) &&
			     (strcmp(t->target.u.user.name,
				     XT_STANDARD_TARGET) == 0) &&
			     t->verdict < 0) || visited) {
				unsigned int oldpos, size;

<<<<<<< HEAD
				if ((strcmp(t->target.u.user.name,
					    XT_STANDARD_TARGET) == 0) &&
				    t->verdict < -NF_MAX_VERDICT - 1) {
					duprintf("mark_source_chains: bad "
						"negative verdict (%i)\n",
								t->verdict);
					return 0;
				}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				/* Return: backtrack through the last
				 * big jump.
				 */
				do {
					e->comefrom ^= (1<<NF_ARP_NUMHOOKS);
					oldpos = pos;
					pos = e->counters.pcnt;
					e->counters.pcnt = 0;

					/* We're at the start. */
					if (pos == oldpos)
						goto next;

<<<<<<< HEAD
					e = (struct arpt_entry *)
						(entry0 + pos);
=======
					e = entry0 + pos;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				} while (oldpos == pos + e->next_offset);

				/* Move along one */
				size = e->next_offset;
<<<<<<< HEAD
				e = (struct arpt_entry *)
					(entry0 + pos + size);
=======
				e = entry0 + pos + size;
				if (pos + size >= newinfo->size)
					return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				e->counters.pcnt = pos;
				pos += size;
			} else {
				int newpos = t->verdict;

				if (strcmp(t->target.u.user.name,
					   XT_STANDARD_TARGET) == 0 &&
				    newpos >= 0) {
<<<<<<< HEAD
					if (newpos > newinfo->size -
						sizeof(struct arpt_entry)) {
						duprintf("mark_source_chains: "
							"bad verdict (%i)\n",
								newpos);
						return 0;
					}

					/* This a jump; chase it. */
					duprintf("Jump rule %u -> %u\n",
						 pos, newpos);
				} else {
					/* ... this is a fallthru */
					newpos = pos + e->next_offset;
				}
				e = (struct arpt_entry *)
					(entry0 + newpos);
=======
					/* This a jump; chase it. */
					if (!xt_find_jump_offset(offsets, newpos,
								 newinfo->number))
						return 0;
				} else {
					/* ... this is a fallthru */
					newpos = pos + e->next_offset;
					if (newpos >= newinfo->size)
						return 0;
				}
				e = entry0 + newpos;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				e->counters.pcnt = pos;
				pos = newpos;
			}
		}
<<<<<<< HEAD
		next:
		duprintf("Finished chain %u\n", hook);
=======
next:		;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return 1;
}

<<<<<<< HEAD
static inline int check_entry(const struct arpt_entry *e, const char *name)
{
	const struct xt_entry_target *t;

	if (!arp_checkentry(&e->arp)) {
		duprintf("arp_tables: arp check failed %p %s.\n", e, name);
		return -EINVAL;
	}

	if (e->target_offset + sizeof(struct xt_entry_target) > e->next_offset)
		return -EINVAL;

	t = arpt_get_target_c(e);
	if (e->target_offset + t->u.target_size > e->next_offset)
		return -EINVAL;

	return 0;
}

static inline int check_target(struct arpt_entry *e, const char *name)
{
	struct xt_entry_target *t = arpt_get_target(e);
	int ret;
	struct xt_tgchk_param par = {
=======
static int check_target(struct arpt_entry *e, struct net *net, const char *name)
{
	struct xt_entry_target *t = arpt_get_target(e);
	struct xt_tgchk_param par = {
		.net       = net,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.table     = name,
		.entryinfo = e,
		.target    = t->u.kernel.target,
		.targinfo  = t->data,
		.hook_mask = e->comefrom,
		.family    = NFPROTO_ARP,
	};

<<<<<<< HEAD
	ret = xt_check_target(&par, t->u.target_size - sizeof(*t), 0, false);
	if (ret < 0) {
		duprintf("arp_tables: check failed for `%s'.\n",
			 t->u.kernel.target->name);
		return ret;
	}
	return 0;
}

static inline int
find_check_entry(struct arpt_entry *e, const char *name, unsigned int size)
=======
	return xt_check_target(&par, t->u.target_size - sizeof(*t), 0, false);
}

static int
find_check_entry(struct arpt_entry *e, struct net *net, const char *name,
		 unsigned int size,
		 struct xt_percpu_counter_alloc_state *alloc_state)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct xt_entry_target *t;
	struct xt_target *target;
	int ret;

<<<<<<< HEAD
	ret = check_entry(e, name);
	if (ret)
		return ret;
=======
	if (!xt_percpu_counter_alloc(alloc_state, &e->counters))
		return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	t = arpt_get_target(e);
	target = xt_request_find_target(NFPROTO_ARP, t->u.user.name,
					t->u.user.revision);
	if (IS_ERR(target)) {
<<<<<<< HEAD
		duprintf("find_check_entry: `%s' not found\n", t->u.user.name);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = PTR_ERR(target);
		goto out;
	}
	t->u.kernel.target = target;

<<<<<<< HEAD
	ret = check_target(e, name);
=======
	ret = check_target(e, net, name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret)
		goto err;
	return 0;
err:
	module_put(t->u.kernel.target->me);
out:
<<<<<<< HEAD
=======
	xt_percpu_counter_free(&e->counters);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static bool check_underflow(const struct arpt_entry *e)
{
	const struct xt_entry_target *t;
	unsigned int verdict;

	if (!unconditional(e))
		return false;
	t = arpt_get_target_c(e);
	if (strcmp(t->u.user.name, XT_STANDARD_TARGET) != 0)
		return false;
	verdict = ((struct xt_standard_target *)t)->verdict;
	verdict = -verdict - 1;
	return verdict == NF_DROP || verdict == NF_ACCEPT;
}

static inline int check_entry_size_and_hooks(struct arpt_entry *e,
					     struct xt_table_info *newinfo,
					     const unsigned char *base,
					     const unsigned char *limit,
					     const unsigned int *hook_entries,
					     const unsigned int *underflows,
					     unsigned int valid_hooks)
{
	unsigned int h;
<<<<<<< HEAD

	if ((unsigned long)e % __alignof__(struct arpt_entry) != 0 ||
	    (unsigned char *)e + sizeof(struct arpt_entry) >= limit ||
	    (unsigned char *)e + e->next_offset > limit) {
		duprintf("Bad offset %p\n", e);
		return -EINVAL;
	}

	if (e->next_offset
	    < sizeof(struct arpt_entry) + sizeof(struct xt_entry_target)) {
		duprintf("checking: element %p size %u\n",
			 e, e->next_offset);
		return -EINVAL;
	}
=======
	int err;

	if ((unsigned long)e % __alignof__(struct arpt_entry) != 0 ||
	    (unsigned char *)e + sizeof(struct arpt_entry) >= limit ||
	    (unsigned char *)e + e->next_offset > limit)
		return -EINVAL;

	if (e->next_offset
	    < sizeof(struct arpt_entry) + sizeof(struct xt_entry_target))
		return -EINVAL;

	if (!arp_checkentry(&e->arp))
		return -EINVAL;

	err = xt_check_entry_offsets(e, e->elems, e->target_offset,
				     e->next_offset);
	if (err)
		return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Check hooks & underflows */
	for (h = 0; h < NF_ARP_NUMHOOKS; h++) {
		if (!(valid_hooks & (1 << h)))
			continue;
		if ((unsigned char *)e - base == hook_entries[h])
			newinfo->hook_entry[h] = hook_entries[h];
		if ((unsigned char *)e - base == underflows[h]) {
<<<<<<< HEAD
			if (!check_underflow(e)) {
				pr_debug("Underflows must be unconditional and "
					 "use the STANDARD target with "
					 "ACCEPT/DROP\n");
				return -EINVAL;
			}
=======
			if (!check_underflow(e))
				return -EINVAL;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			newinfo->underflow[h] = underflows[h];
		}
	}

	/* Clear counters and comefrom */
	e->counters = ((struct xt_counters) { 0, 0 });
	e->comefrom = 0;
	return 0;
}

<<<<<<< HEAD
static inline void cleanup_entry(struct arpt_entry *e)
=======
static void cleanup_entry(struct arpt_entry *e, struct net *net)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct xt_tgdtor_param par;
	struct xt_entry_target *t;

	t = arpt_get_target(e);
<<<<<<< HEAD
=======
	par.net      = net;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	par.target   = t->u.kernel.target;
	par.targinfo = t->data;
	par.family   = NFPROTO_ARP;
	if (par.target->destroy != NULL)
		par.target->destroy(&par);
	module_put(par.target->me);
<<<<<<< HEAD
=======
	xt_percpu_counter_free(&e->counters);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Checks and translates the user-supplied table segment (held in
 * newinfo).
 */
<<<<<<< HEAD
static int translate_table(struct xt_table_info *newinfo, void *entry0,
                           const struct arpt_replace *repl)
{
	struct arpt_entry *iter;
=======
static int translate_table(struct net *net,
			   struct xt_table_info *newinfo,
			   void *entry0,
			   const struct arpt_replace *repl)
{
	struct xt_percpu_counter_alloc_state alloc_state = { 0 };
	struct arpt_entry *iter;
	unsigned int *offsets;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int i;
	int ret = 0;

	newinfo->size = repl->size;
	newinfo->number = repl->num_entries;

	/* Init all hooks to impossible value. */
	for (i = 0; i < NF_ARP_NUMHOOKS; i++) {
		newinfo->hook_entry[i] = 0xFFFFFFFF;
		newinfo->underflow[i] = 0xFFFFFFFF;
	}

<<<<<<< HEAD
	duprintf("translate_table: size %u\n", newinfo->size);
=======
	offsets = xt_alloc_entry_offsets(newinfo->number);
	if (!offsets)
		return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	i = 0;

	/* Walk through entries, checking offsets. */
	xt_entry_foreach(iter, entry0, newinfo->size) {
		ret = check_entry_size_and_hooks(iter, newinfo, entry0,
						 entry0 + repl->size,
						 repl->hook_entry,
						 repl->underflow,
						 repl->valid_hooks);
		if (ret != 0)
<<<<<<< HEAD
			break;
=======
			goto out_free;
		if (i < repl->num_entries)
			offsets[i] = (void *)iter - entry0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		++i;
		if (strcmp(arpt_get_target(iter)->u.user.name,
		    XT_ERROR_TARGET) == 0)
			++newinfo->stacksize;
	}
<<<<<<< HEAD
	duprintf("translate_table: ARPT_ENTRY_ITERATE gives %d\n", ret);
	if (ret != 0)
		return ret;

	if (i != repl->num_entries) {
		duprintf("translate_table: %u not %u entries\n",
			 i, repl->num_entries);
		return -EINVAL;
	}

	/* Check hooks all assigned */
	for (i = 0; i < NF_ARP_NUMHOOKS; i++) {
		/* Only hooks which are valid */
		if (!(repl->valid_hooks & (1 << i)))
			continue;
		if (newinfo->hook_entry[i] == 0xFFFFFFFF) {
			duprintf("Invalid hook entry %u %u\n",
				 i, repl->hook_entry[i]);
			return -EINVAL;
		}
		if (newinfo->underflow[i] == 0xFFFFFFFF) {
			duprintf("Invalid underflow %u %u\n",
				 i, repl->underflow[i]);
			return -EINVAL;
		}
	}

	if (!mark_source_chains(newinfo, repl->valid_hooks, entry0)) {
		duprintf("Looping hook\n");
		return -ELOOP;
	}
=======

	ret = -EINVAL;
	if (i != repl->num_entries)
		goto out_free;

	ret = xt_check_table_hooks(newinfo, repl->valid_hooks);
	if (ret)
		goto out_free;

	if (!mark_source_chains(newinfo, repl->valid_hooks, entry0, offsets)) {
		ret = -ELOOP;
		goto out_free;
	}
	kvfree(offsets);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Finally, each sanity check must pass */
	i = 0;
	xt_entry_foreach(iter, entry0, newinfo->size) {
<<<<<<< HEAD
		ret = find_check_entry(iter, repl->name, repl->size);
=======
		ret = find_check_entry(iter, net, repl->name, repl->size,
				       &alloc_state);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret != 0)
			break;
		++i;
	}

	if (ret != 0) {
		xt_entry_foreach(iter, entry0, newinfo->size) {
			if (i-- == 0)
				break;
<<<<<<< HEAD
			cleanup_entry(iter);
=======
			cleanup_entry(iter, net);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		return ret;
	}

<<<<<<< HEAD
	/* And one copy for every other CPU */
	for_each_possible_cpu(i) {
		if (newinfo->entries[i] && newinfo->entries[i] != entry0)
			memcpy(newinfo->entries[i], entry0, newinfo->size);
	}

=======
	return ret;
 out_free:
	kvfree(offsets);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static void get_counters(const struct xt_table_info *t,
			 struct xt_counters counters[])
{
	struct arpt_entry *iter;
	unsigned int cpu;
	unsigned int i;

	for_each_possible_cpu(cpu) {
		seqcount_t *s = &per_cpu(xt_recseq, cpu);

		i = 0;
<<<<<<< HEAD
		xt_entry_foreach(iter, t->entries[cpu], t->size) {
			u64 bcnt, pcnt;
			unsigned int start;

			do {
				start = read_seqcount_begin(s);
				bcnt = iter->counters.bcnt;
				pcnt = iter->counters.pcnt;
=======
		xt_entry_foreach(iter, t->entries, t->size) {
			struct xt_counters *tmp;
			u64 bcnt, pcnt;
			unsigned int start;

			tmp = xt_get_per_cpu_counter(&iter->counters, cpu);
			do {
				start = read_seqcount_begin(s);
				bcnt = tmp->bcnt;
				pcnt = tmp->pcnt;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			} while (read_seqcount_retry(s, start));

			ADD_COUNTER(counters[i], bcnt, pcnt);
			++i;
<<<<<<< HEAD
=======
			cond_resched();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
}

<<<<<<< HEAD
=======
static void get_old_counters(const struct xt_table_info *t,
			     struct xt_counters counters[])
{
	struct arpt_entry *iter;
	unsigned int cpu, i;

	for_each_possible_cpu(cpu) {
		i = 0;
		xt_entry_foreach(iter, t->entries, t->size) {
			struct xt_counters *tmp;

			tmp = xt_get_per_cpu_counter(&iter->counters, cpu);
			ADD_COUNTER(counters[i], tmp->bcnt, tmp->pcnt);
			++i;
		}
		cond_resched();
	}
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct xt_counters *alloc_counters(const struct xt_table *table)
{
	unsigned int countersize;
	struct xt_counters *counters;
	const struct xt_table_info *private = table->private;

	/* We need atomic snapshot of counters: rest doesn't change
	 * (other than comefrom, which userspace doesn't care
	 * about).
	 */
	countersize = sizeof(struct xt_counters) * private->number;
	counters = vzalloc(countersize);

	if (counters == NULL)
		return ERR_PTR(-ENOMEM);

	get_counters(private, counters);

	return counters;
}

static int copy_entries_to_user(unsigned int total_size,
				const struct xt_table *table,
				void __user *userptr)
{
	unsigned int off, num;
	const struct arpt_entry *e;
	struct xt_counters *counters;
	struct xt_table_info *private = table->private;
	int ret = 0;
	void *loc_cpu_entry;

	counters = alloc_counters(table);
	if (IS_ERR(counters))
		return PTR_ERR(counters);

<<<<<<< HEAD
	loc_cpu_entry = private->entries[raw_smp_processor_id()];
	/* ... then copy entire thing ... */
	if (copy_to_user(userptr, loc_cpu_entry, total_size) != 0) {
		ret = -EFAULT;
		goto free_counters;
	}
=======
	loc_cpu_entry = private->entries;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* FIXME: use iterator macros --RR */
	/* ... then go back and fix counters and names */
	for (off = 0, num = 0; off < total_size; off += e->next_offset, num++){
		const struct xt_entry_target *t;

<<<<<<< HEAD
		e = (struct arpt_entry *)(loc_cpu_entry + off);
=======
		e = loc_cpu_entry + off;
		if (copy_to_user(userptr + off, e, sizeof(*e))) {
			ret = -EFAULT;
			goto free_counters;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (copy_to_user(userptr + off
				 + offsetof(struct arpt_entry, counters),
				 &counters[num],
				 sizeof(counters[num])) != 0) {
			ret = -EFAULT;
			goto free_counters;
		}

		t = arpt_get_target_c(e);
<<<<<<< HEAD
		if (copy_to_user(userptr + off + e->target_offset
				 + offsetof(struct xt_entry_target,
					    u.user.name),
				 t->u.kernel.target->name,
				 strlen(t->u.kernel.target->name)+1) != 0) {
=======
		if (xt_target_to_user(t, userptr + off + e->target_offset)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ret = -EFAULT;
			goto free_counters;
		}
	}

 free_counters:
	vfree(counters);
	return ret;
}

<<<<<<< HEAD
#ifdef CONFIG_COMPAT
=======
#ifdef CONFIG_NETFILTER_XTABLES_COMPAT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void compat_standard_from_user(void *dst, const void *src)
{
	int v = *(compat_int_t *)src;

	if (v > 0)
		v += xt_compat_calc_jump(NFPROTO_ARP, v);
	memcpy(dst, &v, sizeof(v));
}

static int compat_standard_to_user(void __user *dst, const void *src)
{
	compat_int_t cv = *(int *)src;

	if (cv > 0)
		cv -= xt_compat_calc_jump(NFPROTO_ARP, cv);
	return copy_to_user(dst, &cv, sizeof(cv)) ? -EFAULT : 0;
}

static int compat_calc_entry(const struct arpt_entry *e,
			     const struct xt_table_info *info,
			     const void *base, struct xt_table_info *newinfo)
{
	const struct xt_entry_target *t;
	unsigned int entry_offset;
	int off, i, ret;

	off = sizeof(struct arpt_entry) - sizeof(struct compat_arpt_entry);
	entry_offset = (void *)e - base;

	t = arpt_get_target_c(e);
	off += xt_compat_target_offset(t->u.kernel.target);
	newinfo->size -= off;
	ret = xt_compat_add_offset(NFPROTO_ARP, entry_offset, off);
	if (ret)
		return ret;

	for (i = 0; i < NF_ARP_NUMHOOKS; i++) {
		if (info->hook_entry[i] &&
		    (e < (struct arpt_entry *)(base + info->hook_entry[i])))
			newinfo->hook_entry[i] -= off;
		if (info->underflow[i] &&
		    (e < (struct arpt_entry *)(base + info->underflow[i])))
			newinfo->underflow[i] -= off;
	}
	return 0;
}

static int compat_table_info(const struct xt_table_info *info,
			     struct xt_table_info *newinfo)
{
	struct arpt_entry *iter;
<<<<<<< HEAD
	void *loc_cpu_entry;
=======
	const void *loc_cpu_entry;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret;

	if (!newinfo || !info)
		return -EINVAL;

<<<<<<< HEAD
	/* we dont care about newinfo->entries[] */
	memcpy(newinfo, info, offsetof(struct xt_table_info, entries));
	newinfo->initial_entries = 0;
	loc_cpu_entry = info->entries[raw_smp_processor_id()];
	xt_compat_init_offsets(NFPROTO_ARP, info->number);
=======
	/* we dont care about newinfo->entries */
	memcpy(newinfo, info, offsetof(struct xt_table_info, entries));
	newinfo->initial_entries = 0;
	loc_cpu_entry = info->entries;
	ret = xt_compat_init_offsets(NFPROTO_ARP, info->number);
	if (ret)
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	xt_entry_foreach(iter, loc_cpu_entry, info->size) {
		ret = compat_calc_entry(iter, info, loc_cpu_entry, newinfo);
		if (ret != 0)
			return ret;
	}
	return 0;
}
#endif

<<<<<<< HEAD
static int get_info(struct net *net, void __user *user,
                    const int *len, int compat)
=======
static int get_info(struct net *net, void __user *user, const int *len)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	char name[XT_TABLE_MAXNAMELEN];
	struct xt_table *t;
	int ret;

<<<<<<< HEAD
	if (*len != sizeof(struct arpt_getinfo)) {
		duprintf("length %u != %Zu\n", *len,
			 sizeof(struct arpt_getinfo));
		return -EINVAL;
	}
=======
	if (*len != sizeof(struct arpt_getinfo))
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (copy_from_user(name, user, sizeof(name)) != 0)
		return -EFAULT;

	name[XT_TABLE_MAXNAMELEN-1] = '\0';
<<<<<<< HEAD
#ifdef CONFIG_COMPAT
	if (compat)
		xt_compat_lock(NFPROTO_ARP);
#endif
	t = try_then_request_module(xt_find_table_lock(net, NFPROTO_ARP, name),
				    "arptable_%s", name);
	if (t && !IS_ERR(t)) {
		struct arpt_getinfo info;
		const struct xt_table_info *private = t->private;
#ifdef CONFIG_COMPAT
		struct xt_table_info tmp;

		if (compat) {
=======
#ifdef CONFIG_NETFILTER_XTABLES_COMPAT
	if (in_compat_syscall())
		xt_compat_lock(NFPROTO_ARP);
#endif
	t = xt_request_find_table_lock(net, NFPROTO_ARP, name);
	if (!IS_ERR(t)) {
		struct arpt_getinfo info;
		const struct xt_table_info *private = t->private;
#ifdef CONFIG_NETFILTER_XTABLES_COMPAT
		struct xt_table_info tmp;

		if (in_compat_syscall()) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ret = compat_table_info(private, &tmp);
			xt_compat_flush_offsets(NFPROTO_ARP);
			private = &tmp;
		}
#endif
		memset(&info, 0, sizeof(info));
		info.valid_hooks = t->valid_hooks;
		memcpy(info.hook_entry, private->hook_entry,
		       sizeof(info.hook_entry));
		memcpy(info.underflow, private->underflow,
		       sizeof(info.underflow));
		info.num_entries = private->number;
		info.size = private->size;
		strcpy(info.name, name);

		if (copy_to_user(user, &info, *len) != 0)
			ret = -EFAULT;
		else
			ret = 0;
		xt_table_unlock(t);
		module_put(t->me);
	} else
<<<<<<< HEAD
		ret = t ? PTR_ERR(t) : -ENOENT;
#ifdef CONFIG_COMPAT
	if (compat)
=======
		ret = PTR_ERR(t);
#ifdef CONFIG_NETFILTER_XTABLES_COMPAT
	if (in_compat_syscall())
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		xt_compat_unlock(NFPROTO_ARP);
#endif
	return ret;
}

static int get_entries(struct net *net, struct arpt_get_entries __user *uptr,
		       const int *len)
{
	int ret;
	struct arpt_get_entries get;
	struct xt_table *t;

<<<<<<< HEAD
	if (*len < sizeof(get)) {
		duprintf("get_entries: %u < %Zu\n", *len, sizeof(get));
		return -EINVAL;
	}
	if (copy_from_user(&get, uptr, sizeof(get)) != 0)
		return -EFAULT;
	if (*len != sizeof(struct arpt_get_entries) + get.size) {
		duprintf("get_entries: %u != %Zu\n", *len,
			 sizeof(struct arpt_get_entries) + get.size);
		return -EINVAL;
	}

	t = xt_find_table_lock(net, NFPROTO_ARP, get.name);
	if (t && !IS_ERR(t)) {
		const struct xt_table_info *private = t->private;

		duprintf("t->private->number = %u\n",
			 private->number);
		if (get.size == private->size)
			ret = copy_entries_to_user(private->size,
						   t, uptr->entrytable);
		else {
			duprintf("get_entries: I've got %u not %u!\n",
				 private->size, get.size);
			ret = -EAGAIN;
		}
		module_put(t->me);
		xt_table_unlock(t);
	} else
		ret = t ? PTR_ERR(t) : -ENOENT;
=======
	if (*len < sizeof(get))
		return -EINVAL;
	if (copy_from_user(&get, uptr, sizeof(get)) != 0)
		return -EFAULT;
	if (*len != sizeof(struct arpt_get_entries) + get.size)
		return -EINVAL;

	get.name[sizeof(get.name) - 1] = '\0';

	t = xt_find_table_lock(net, NFPROTO_ARP, get.name);
	if (!IS_ERR(t)) {
		const struct xt_table_info *private = t->private;

		if (get.size == private->size)
			ret = copy_entries_to_user(private->size,
						   t, uptr->entrytable);
		else
			ret = -EAGAIN;

		module_put(t->me);
		xt_table_unlock(t);
	} else
		ret = PTR_ERR(t);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

static int __do_replace(struct net *net, const char *name,
			unsigned int valid_hooks,
			struct xt_table_info *newinfo,
			unsigned int num_counters,
			void __user *counters_ptr)
{
	int ret;
	struct xt_table *t;
	struct xt_table_info *oldinfo;
	struct xt_counters *counters;
	void *loc_cpu_old_entry;
	struct arpt_entry *iter;

	ret = 0;
<<<<<<< HEAD
	counters = vzalloc(num_counters * sizeof(struct xt_counters));
=======
	counters = xt_counters_alloc(num_counters);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!counters) {
		ret = -ENOMEM;
		goto out;
	}

<<<<<<< HEAD
	t = try_then_request_module(xt_find_table_lock(net, NFPROTO_ARP, name),
				    "arptable_%s", name);
	if (!t || IS_ERR(t)) {
		ret = t ? PTR_ERR(t) : -ENOENT;
=======
	t = xt_request_find_table_lock(net, NFPROTO_ARP, name);
	if (IS_ERR(t)) {
		ret = PTR_ERR(t);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto free_newinfo_counters_untrans;
	}

	/* You lied! */
	if (valid_hooks != t->valid_hooks) {
<<<<<<< HEAD
		duprintf("Valid hook crap: %08X vs %08X\n",
			 valid_hooks, t->valid_hooks);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -EINVAL;
		goto put_module;
	}

	oldinfo = xt_replace_table(t, num_counters, newinfo, &ret);
	if (!oldinfo)
		goto put_module;

	/* Update module usage count based on number of rules */
<<<<<<< HEAD
	duprintf("do_replace: oldnum=%u, initnum=%u, newnum=%u\n",
		oldinfo->number, oldinfo->initial_entries, newinfo->number);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if ((oldinfo->number > oldinfo->initial_entries) ||
	    (newinfo->number <= oldinfo->initial_entries))
		module_put(t->me);
	if ((oldinfo->number > oldinfo->initial_entries) &&
	    (newinfo->number <= oldinfo->initial_entries))
		module_put(t->me);

<<<<<<< HEAD
	/* Get the old counters, and synchronize with replace */
	get_counters(oldinfo, counters);

	/* Decrease module usage counts and free resource */
	loc_cpu_old_entry = oldinfo->entries[raw_smp_processor_id()];
	xt_entry_foreach(iter, loc_cpu_old_entry, oldinfo->size)
		cleanup_entry(iter);
=======
	xt_table_unlock(t);

	get_old_counters(oldinfo, counters);

	/* Decrease module usage counts and free resource */
	loc_cpu_old_entry = oldinfo->entries;
	xt_entry_foreach(iter, loc_cpu_old_entry, oldinfo->size)
		cleanup_entry(iter, net);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	xt_free_table_info(oldinfo);
	if (copy_to_user(counters_ptr, counters,
			 sizeof(struct xt_counters) * num_counters) != 0) {
		/* Silent error, can't fail, new table is already in place */
		net_warn_ratelimited("arptables: counters copy to user failed while replacing table\n");
	}
	vfree(counters);
<<<<<<< HEAD
	xt_table_unlock(t);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;

 put_module:
	module_put(t->me);
	xt_table_unlock(t);
 free_newinfo_counters_untrans:
	vfree(counters);
 out:
	return ret;
}

<<<<<<< HEAD
static int do_replace(struct net *net, const void __user *user,
                      unsigned int len)
=======
static int do_replace(struct net *net, sockptr_t arg, unsigned int len)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int ret;
	struct arpt_replace tmp;
	struct xt_table_info *newinfo;
	void *loc_cpu_entry;
	struct arpt_entry *iter;

<<<<<<< HEAD
	if (copy_from_user(&tmp, user, sizeof(tmp)) != 0)
=======
	if (len < sizeof(tmp))
		return -EINVAL;
	if (copy_from_sockptr(&tmp, arg, sizeof(tmp)) != 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EFAULT;

	/* overflow check */
	if (tmp.num_counters >= INT_MAX / sizeof(struct xt_counters))
		return -ENOMEM;
<<<<<<< HEAD
=======
	if (tmp.num_counters == 0)
		return -EINVAL;
	if ((u64)len < (u64)tmp.size + sizeof(tmp))
		return -EINVAL;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	tmp.name[sizeof(tmp.name)-1] = 0;

	newinfo = xt_alloc_table_info(tmp.size);
	if (!newinfo)
		return -ENOMEM;

<<<<<<< HEAD
	/* choose the copy that is on our node/cpu */
	loc_cpu_entry = newinfo->entries[raw_smp_processor_id()];
	if (copy_from_user(loc_cpu_entry, user + sizeof(tmp),
			   tmp.size) != 0) {
=======
	loc_cpu_entry = newinfo->entries;
	if (copy_from_sockptr_offset(loc_cpu_entry, arg, sizeof(tmp),
			tmp.size) != 0) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -EFAULT;
		goto free_newinfo;
	}

<<<<<<< HEAD
	ret = translate_table(newinfo, loc_cpu_entry, &tmp);
	if (ret != 0)
		goto free_newinfo;

	duprintf("arp_tables: Translated table\n");

=======
	ret = translate_table(net, newinfo, loc_cpu_entry, &tmp);
	if (ret != 0)
		goto free_newinfo;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = __do_replace(net, tmp.name, tmp.valid_hooks, newinfo,
			   tmp.num_counters, tmp.counters);
	if (ret)
		goto free_newinfo_untrans;
	return 0;

 free_newinfo_untrans:
	xt_entry_foreach(iter, loc_cpu_entry, newinfo->size)
<<<<<<< HEAD
		cleanup_entry(iter);
=======
		cleanup_entry(iter, net);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 free_newinfo:
	xt_free_table_info(newinfo);
	return ret;
}

<<<<<<< HEAD
static int do_add_counters(struct net *net, const void __user *user,
			   unsigned int len, int compat)
{
	unsigned int i, curcpu;
	struct xt_counters_info tmp;
	struct xt_counters *paddc;
	unsigned int num_counters;
	const char *name;
	int size;
	void *ptmp;
	struct xt_table *t;
	const struct xt_table_info *private;
	int ret = 0;
	void *loc_cpu_entry;
	struct arpt_entry *iter;
	unsigned int addend;
#ifdef CONFIG_COMPAT
	struct compat_xt_counters_info compat_tmp;

	if (compat) {
		ptmp = &compat_tmp;
		size = sizeof(struct compat_xt_counters_info);
	} else
#endif
	{
		ptmp = &tmp;
		size = sizeof(struct xt_counters_info);
	}

	if (copy_from_user(ptmp, user, size) != 0)
		return -EFAULT;

#ifdef CONFIG_COMPAT
	if (compat) {
		num_counters = compat_tmp.num_counters;
		name = compat_tmp.name;
	} else
#endif
	{
		num_counters = tmp.num_counters;
		name = tmp.name;
	}

	if (len != size + num_counters * sizeof(struct xt_counters))
		return -EINVAL;

	paddc = vmalloc(len - size);
	if (!paddc)
		return -ENOMEM;

	if (copy_from_user(paddc, user + size, len - size) != 0) {
		ret = -EFAULT;
		goto free;
	}

	t = xt_find_table_lock(net, NFPROTO_ARP, name);
	if (!t || IS_ERR(t)) {
		ret = t ? PTR_ERR(t) : -ENOENT;
=======
static int do_add_counters(struct net *net, sockptr_t arg, unsigned int len)
{
	unsigned int i;
	struct xt_counters_info tmp;
	struct xt_counters *paddc;
	struct xt_table *t;
	const struct xt_table_info *private;
	int ret = 0;
	struct arpt_entry *iter;
	unsigned int addend;

	paddc = xt_copy_counters(arg, len, &tmp);
	if (IS_ERR(paddc))
		return PTR_ERR(paddc);

	t = xt_find_table_lock(net, NFPROTO_ARP, tmp.name);
	if (IS_ERR(t)) {
		ret = PTR_ERR(t);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto free;
	}

	local_bh_disable();
	private = t->private;
<<<<<<< HEAD
	if (private->number != num_counters) {
=======
	if (private->number != tmp.num_counters) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -EINVAL;
		goto unlock_up_free;
	}

	i = 0;
<<<<<<< HEAD
	/* Choose the copy that is on our node */
	curcpu = smp_processor_id();
	loc_cpu_entry = private->entries[curcpu];
	addend = xt_write_recseq_begin();
	xt_entry_foreach(iter, loc_cpu_entry, private->size) {
		ADD_COUNTER(iter->counters, paddc[i].bcnt, paddc[i].pcnt);
=======

	addend = xt_write_recseq_begin();
	xt_entry_foreach(iter,  private->entries, private->size) {
		struct xt_counters *tmp;

		tmp = xt_get_this_cpu_counter(&iter->counters);
		ADD_COUNTER(*tmp, paddc[i].bcnt, paddc[i].pcnt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		++i;
	}
	xt_write_recseq_end(addend);
 unlock_up_free:
	local_bh_enable();
	xt_table_unlock(t);
	module_put(t->me);
 free:
	vfree(paddc);

	return ret;
}

<<<<<<< HEAD
#ifdef CONFIG_COMPAT
=======
#ifdef CONFIG_NETFILTER_XTABLES_COMPAT
struct compat_arpt_replace {
	char				name[XT_TABLE_MAXNAMELEN];
	u32				valid_hooks;
	u32				num_entries;
	u32				size;
	u32				hook_entry[NF_ARP_NUMHOOKS];
	u32				underflow[NF_ARP_NUMHOOKS];
	u32				num_counters;
	compat_uptr_t			counters;
	struct compat_arpt_entry	entries[];
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void compat_release_entry(struct compat_arpt_entry *e)
{
	struct xt_entry_target *t;

	t = compat_arpt_get_target(e);
	module_put(t->u.kernel.target->me);
}

<<<<<<< HEAD
static inline int
=======
static int
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
check_compat_entry_size_and_hooks(struct compat_arpt_entry *e,
				  struct xt_table_info *newinfo,
				  unsigned int *size,
				  const unsigned char *base,
<<<<<<< HEAD
				  const unsigned char *limit,
				  const unsigned int *hook_entries,
				  const unsigned int *underflows,
				  const char *name)
=======
				  const unsigned char *limit)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct xt_entry_target *t;
	struct xt_target *target;
	unsigned int entry_offset;
<<<<<<< HEAD
	int ret, off, h;

	duprintf("check_compat_entry_size_and_hooks %p\n", e);
	if ((unsigned long)e % __alignof__(struct compat_arpt_entry) != 0 ||
	    (unsigned char *)e + sizeof(struct compat_arpt_entry) >= limit ||
	    (unsigned char *)e + e->next_offset > limit) {
		duprintf("Bad offset %p, limit = %p\n", e, limit);
		return -EINVAL;
	}

	if (e->next_offset < sizeof(struct compat_arpt_entry) +
			     sizeof(struct compat_xt_entry_target)) {
		duprintf("checking: element %p size %u\n",
			 e, e->next_offset);
		return -EINVAL;
	}

	/* For purposes of check_entry casting the compat entry is fine */
	ret = check_entry((struct arpt_entry *)e, name);
=======
	int ret, off;

	if ((unsigned long)e % __alignof__(struct compat_arpt_entry) != 0 ||
	    (unsigned char *)e + sizeof(struct compat_arpt_entry) >= limit ||
	    (unsigned char *)e + e->next_offset > limit)
		return -EINVAL;

	if (e->next_offset < sizeof(struct compat_arpt_entry) +
			     sizeof(struct compat_xt_entry_target))
		return -EINVAL;

	if (!arp_checkentry(&e->arp))
		return -EINVAL;

	ret = xt_compat_check_entry_offsets(e, e->elems, e->target_offset,
					    e->next_offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret)
		return ret;

	off = sizeof(struct arpt_entry) - sizeof(struct compat_arpt_entry);
	entry_offset = (void *)e - (void *)base;

	t = compat_arpt_get_target(e);
	target = xt_request_find_target(NFPROTO_ARP, t->u.user.name,
					t->u.user.revision);
	if (IS_ERR(target)) {
<<<<<<< HEAD
		duprintf("check_compat_entry_size_and_hooks: `%s' not found\n",
			 t->u.user.name);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = PTR_ERR(target);
		goto out;
	}
	t->u.kernel.target = target;

	off += xt_compat_target_offset(target);
	*size += off;
	ret = xt_compat_add_offset(NFPROTO_ARP, entry_offset, off);
	if (ret)
		goto release_target;

<<<<<<< HEAD
	/* Check hooks & underflows */
	for (h = 0; h < NF_ARP_NUMHOOKS; h++) {
		if ((unsigned char *)e - base == hook_entries[h])
			newinfo->hook_entry[h] = hook_entries[h];
		if ((unsigned char *)e - base == underflows[h])
			newinfo->underflow[h] = underflows[h];
	}

	/* Clear counters and comefrom */
	memset(&e->counters, 0, sizeof(e->counters));
	e->comefrom = 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;

release_target:
	module_put(t->u.kernel.target->me);
out:
	return ret;
}

<<<<<<< HEAD
static int
compat_copy_entry_from_user(struct compat_arpt_entry *e, void **dstptr,
			    unsigned int *size, const char *name,
			    struct xt_table_info *newinfo, unsigned char *base)
{
	struct xt_entry_target *t;
	struct xt_target *target;
	struct arpt_entry *de;
	unsigned int origsize;
	int ret, h;

	ret = 0;
	origsize = *size;
	de = (struct arpt_entry *)*dstptr;
=======
static void
compat_copy_entry_from_user(struct compat_arpt_entry *e, void **dstptr,
			    unsigned int *size,
			    struct xt_table_info *newinfo, unsigned char *base)
{
	struct xt_entry_target *t;
	struct arpt_entry *de;
	unsigned int origsize;
	int h;

	origsize = *size;
	de = *dstptr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	memcpy(de, e, sizeof(struct arpt_entry));
	memcpy(&de->counters, &e->counters, sizeof(e->counters));

	*dstptr += sizeof(struct arpt_entry);
	*size += sizeof(struct arpt_entry) - sizeof(struct compat_arpt_entry);

	de->target_offset = e->target_offset - (origsize - *size);
	t = compat_arpt_get_target(e);
<<<<<<< HEAD
	target = t->u.kernel.target;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	xt_compat_target_from_user(t, dstptr, size);

	de->next_offset = e->next_offset - (origsize - *size);
	for (h = 0; h < NF_ARP_NUMHOOKS; h++) {
		if ((unsigned char *)de - base < newinfo->hook_entry[h])
			newinfo->hook_entry[h] -= origsize - *size;
		if ((unsigned char *)de - base < newinfo->underflow[h])
			newinfo->underflow[h] -= origsize - *size;
	}
<<<<<<< HEAD
	return ret;
}

static int translate_compat_table(const char *name,
				  unsigned int valid_hooks,
				  struct xt_table_info **pinfo,
				  void **pentry0,
				  unsigned int total_size,
				  unsigned int number,
				  unsigned int *hook_entries,
				  unsigned int *underflows)
=======
}

static int translate_compat_table(struct net *net,
				  struct xt_table_info **pinfo,
				  void **pentry0,
				  const struct compat_arpt_replace *compatr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned int i, j;
	struct xt_table_info *newinfo, *info;
	void *pos, *entry0, *entry1;
	struct compat_arpt_entry *iter0;
<<<<<<< HEAD
	struct arpt_entry *iter1;
	unsigned int size;
	int ret = 0;

	info = *pinfo;
	entry0 = *pentry0;
	size = total_size;
	info->number = number;

	/* Init all hooks to impossible value. */
	for (i = 0; i < NF_ARP_NUMHOOKS; i++) {
		info->hook_entry[i] = 0xFFFFFFFF;
		info->underflow[i] = 0xFFFFFFFF;
	}

	duprintf("translate_compat_table: size %u\n", info->size);
	j = 0;
	xt_compat_lock(NFPROTO_ARP);
	xt_compat_init_offsets(NFPROTO_ARP, number);
	/* Walk through entries, checking offsets. */
	xt_entry_foreach(iter0, entry0, total_size) {
		ret = check_compat_entry_size_and_hooks(iter0, info, &size,
							entry0,
							entry0 + total_size,
							hook_entries,
							underflows,
							name);
=======
	struct arpt_replace repl;
	unsigned int size;
	int ret;

	info = *pinfo;
	entry0 = *pentry0;
	size = compatr->size;
	info->number = compatr->num_entries;

	j = 0;
	xt_compat_lock(NFPROTO_ARP);
	ret = xt_compat_init_offsets(NFPROTO_ARP, compatr->num_entries);
	if (ret)
		goto out_unlock;
	/* Walk through entries, checking offsets. */
	xt_entry_foreach(iter0, entry0, compatr->size) {
		ret = check_compat_entry_size_and_hooks(iter0, info, &size,
							entry0,
							entry0 + compatr->size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret != 0)
			goto out_unlock;
		++j;
	}

	ret = -EINVAL;
<<<<<<< HEAD
	if (j != number) {
		duprintf("translate_compat_table: %u not %u entries\n",
			 j, number);
		goto out_unlock;
	}

	/* Check hooks all assigned */
	for (i = 0; i < NF_ARP_NUMHOOKS; i++) {
		/* Only hooks which are valid */
		if (!(valid_hooks & (1 << i)))
			continue;
		if (info->hook_entry[i] == 0xFFFFFFFF) {
			duprintf("Invalid hook entry %u %u\n",
				 i, hook_entries[i]);
			goto out_unlock;
		}
		if (info->underflow[i] == 0xFFFFFFFF) {
			duprintf("Invalid underflow %u %u\n",
				 i, underflows[i]);
			goto out_unlock;
		}
	}
=======
	if (j != compatr->num_entries)
		goto out_unlock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = -ENOMEM;
	newinfo = xt_alloc_table_info(size);
	if (!newinfo)
		goto out_unlock;

<<<<<<< HEAD
	newinfo->number = number;
	for (i = 0; i < NF_ARP_NUMHOOKS; i++) {
		newinfo->hook_entry[i] = info->hook_entry[i];
		newinfo->underflow[i] = info->underflow[i];
	}
	entry1 = newinfo->entries[raw_smp_processor_id()];
	pos = entry1;
	size = total_size;
	xt_entry_foreach(iter0, entry0, total_size) {
		ret = compat_copy_entry_from_user(iter0, &pos, &size,
						  name, newinfo, entry1);
		if (ret != 0)
			break;
	}
	xt_compat_flush_offsets(NFPROTO_ARP);
	xt_compat_unlock(NFPROTO_ARP);
	if (ret)
		goto free_newinfo;

	ret = -ELOOP;
	if (!mark_source_chains(newinfo, valid_hooks, entry1))
		goto free_newinfo;

	i = 0;
	xt_entry_foreach(iter1, entry1, newinfo->size) {
		ret = check_target(iter1, name);
		if (ret != 0)
			break;
		++i;
		if (strcmp(arpt_get_target(iter1)->u.user.name,
		    XT_ERROR_TARGET) == 0)
			++newinfo->stacksize;
	}
	if (ret) {
		/*
		 * The first i matches need cleanup_entry (calls ->destroy)
		 * because they had called ->check already. The other j-i
		 * entries need only release.
		 */
		int skip = i;
		j -= i;
		xt_entry_foreach(iter0, entry0, newinfo->size) {
			if (skip-- > 0)
				continue;
			if (j-- == 0)
				break;
			compat_release_entry(iter0);
		}
		xt_entry_foreach(iter1, entry1, newinfo->size) {
			if (i-- == 0)
				break;
			cleanup_entry(iter1);
		}
		xt_free_table_info(newinfo);
		return ret;
	}

	/* And one copy for every other CPU */
	for_each_possible_cpu(i)
		if (newinfo->entries[i] && newinfo->entries[i] != entry1)
			memcpy(newinfo->entries[i], entry1, newinfo->size);

=======
	memset(newinfo->entries, 0, size);

	newinfo->number = compatr->num_entries;
	for (i = 0; i < NF_ARP_NUMHOOKS; i++) {
		newinfo->hook_entry[i] = compatr->hook_entry[i];
		newinfo->underflow[i] = compatr->underflow[i];
	}
	entry1 = newinfo->entries;
	pos = entry1;
	size = compatr->size;
	xt_entry_foreach(iter0, entry0, compatr->size)
		compat_copy_entry_from_user(iter0, &pos, &size,
					    newinfo, entry1);

	/* all module references in entry0 are now gone */

	xt_compat_flush_offsets(NFPROTO_ARP);
	xt_compat_unlock(NFPROTO_ARP);

	memcpy(&repl, compatr, sizeof(*compatr));

	for (i = 0; i < NF_ARP_NUMHOOKS; i++) {
		repl.hook_entry[i] = newinfo->hook_entry[i];
		repl.underflow[i] = newinfo->underflow[i];
	}

	repl.num_counters = 0;
	repl.counters = NULL;
	repl.size = newinfo->size;
	ret = translate_table(net, newinfo, entry1, &repl);
	if (ret)
		goto free_newinfo;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	*pinfo = newinfo;
	*pentry0 = entry1;
	xt_free_table_info(info);
	return 0;

free_newinfo:
	xt_free_table_info(newinfo);
<<<<<<< HEAD
out:
	xt_entry_foreach(iter0, entry0, total_size) {
=======
	return ret;
out_unlock:
	xt_compat_flush_offsets(NFPROTO_ARP);
	xt_compat_unlock(NFPROTO_ARP);
	xt_entry_foreach(iter0, entry0, compatr->size) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (j-- == 0)
			break;
		compat_release_entry(iter0);
	}
	return ret;
<<<<<<< HEAD
out_unlock:
	xt_compat_flush_offsets(NFPROTO_ARP);
	xt_compat_unlock(NFPROTO_ARP);
	goto out;
}

struct compat_arpt_replace {
	char				name[XT_TABLE_MAXNAMELEN];
	u32				valid_hooks;
	u32				num_entries;
	u32				size;
	u32				hook_entry[NF_ARP_NUMHOOKS];
	u32				underflow[NF_ARP_NUMHOOKS];
	u32				num_counters;
	compat_uptr_t			counters;
	struct compat_arpt_entry	entries[0];
};

static int compat_do_replace(struct net *net, void __user *user,
			     unsigned int len)
=======
}

static int compat_do_replace(struct net *net, sockptr_t arg, unsigned int len)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int ret;
	struct compat_arpt_replace tmp;
	struct xt_table_info *newinfo;
	void *loc_cpu_entry;
	struct arpt_entry *iter;

<<<<<<< HEAD
	if (copy_from_user(&tmp, user, sizeof(tmp)) != 0)
		return -EFAULT;

	/* overflow check */
	if (tmp.size >= INT_MAX / num_possible_cpus())
		return -ENOMEM;
	if (tmp.num_counters >= INT_MAX / sizeof(struct xt_counters))
		return -ENOMEM;
=======
	if (len < sizeof(tmp))
		return -EINVAL;
	if (copy_from_sockptr(&tmp, arg, sizeof(tmp)) != 0)
		return -EFAULT;

	/* overflow check */
	if (tmp.num_counters >= INT_MAX / sizeof(struct xt_counters))
		return -ENOMEM;
	if (tmp.num_counters == 0)
		return -EINVAL;
	if ((u64)len < (u64)tmp.size + sizeof(tmp))
		return -EINVAL;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	tmp.name[sizeof(tmp.name)-1] = 0;

	newinfo = xt_alloc_table_info(tmp.size);
	if (!newinfo)
		return -ENOMEM;

<<<<<<< HEAD
	/* choose the copy that is on our node/cpu */
	loc_cpu_entry = newinfo->entries[raw_smp_processor_id()];
	if (copy_from_user(loc_cpu_entry, user + sizeof(tmp), tmp.size) != 0) {
=======
	loc_cpu_entry = newinfo->entries;
	if (copy_from_sockptr_offset(loc_cpu_entry, arg, sizeof(tmp),
			tmp.size) != 0) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -EFAULT;
		goto free_newinfo;
	}

<<<<<<< HEAD
	ret = translate_compat_table(tmp.name, tmp.valid_hooks,
				     &newinfo, &loc_cpu_entry, tmp.size,
				     tmp.num_entries, tmp.hook_entry,
				     tmp.underflow);
	if (ret != 0)
		goto free_newinfo;

	duprintf("compat_do_replace: Translated table\n");

=======
	ret = translate_compat_table(net, &newinfo, &loc_cpu_entry, &tmp);
	if (ret != 0)
		goto free_newinfo;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = __do_replace(net, tmp.name, tmp.valid_hooks, newinfo,
			   tmp.num_counters, compat_ptr(tmp.counters));
	if (ret)
		goto free_newinfo_untrans;
	return 0;

 free_newinfo_untrans:
	xt_entry_foreach(iter, loc_cpu_entry, newinfo->size)
<<<<<<< HEAD
		cleanup_entry(iter);
=======
		cleanup_entry(iter, net);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 free_newinfo:
	xt_free_table_info(newinfo);
	return ret;
}

<<<<<<< HEAD
static int compat_do_arpt_set_ctl(struct sock *sk, int cmd, void __user *user,
				  unsigned int len)
{
	int ret;

	if (!ns_capable(sock_net(sk)->user_ns, CAP_NET_ADMIN))
		return -EPERM;

	switch (cmd) {
	case ARPT_SO_SET_REPLACE:
		ret = compat_do_replace(sock_net(sk), user, len);
		break;

	case ARPT_SO_SET_ADD_COUNTERS:
		ret = do_add_counters(sock_net(sk), user, len, 1);
		break;

	default:
		duprintf("do_arpt_set_ctl:  unknown request %i\n", cmd);
		ret = -EINVAL;
	}

	return ret;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int compat_copy_entry_to_user(struct arpt_entry *e, void __user **dstptr,
				     compat_uint_t *size,
				     struct xt_counters *counters,
				     unsigned int i)
{
	struct xt_entry_target *t;
	struct compat_arpt_entry __user *ce;
	u_int16_t target_offset, next_offset;
	compat_uint_t origsize;
	int ret;

	origsize = *size;
<<<<<<< HEAD
	ce = (struct compat_arpt_entry __user *)*dstptr;
=======
	ce = *dstptr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (copy_to_user(ce, e, sizeof(struct arpt_entry)) != 0 ||
	    copy_to_user(&ce->counters, &counters[i],
	    sizeof(counters[i])) != 0)
		return -EFAULT;

	*dstptr += sizeof(struct compat_arpt_entry);
	*size -= sizeof(struct arpt_entry) - sizeof(struct compat_arpt_entry);

	target_offset = e->target_offset - (origsize - *size);

	t = arpt_get_target(e);
	ret = xt_compat_target_to_user(t, dstptr, size);
	if (ret)
		return ret;
	next_offset = e->next_offset - (origsize - *size);
	if (put_user(target_offset, &ce->target_offset) != 0 ||
	    put_user(next_offset, &ce->next_offset) != 0)
		return -EFAULT;
	return 0;
}

static int compat_copy_entries_to_user(unsigned int total_size,
				       struct xt_table *table,
				       void __user *userptr)
{
	struct xt_counters *counters;
	const struct xt_table_info *private = table->private;
	void __user *pos;
	unsigned int size;
	int ret = 0;
<<<<<<< HEAD
	void *loc_cpu_entry;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int i = 0;
	struct arpt_entry *iter;

	counters = alloc_counters(table);
	if (IS_ERR(counters))
		return PTR_ERR(counters);

<<<<<<< HEAD
	/* choose the copy on our node/cpu */
	loc_cpu_entry = private->entries[raw_smp_processor_id()];
	pos = userptr;
	size = total_size;
	xt_entry_foreach(iter, loc_cpu_entry, total_size) {
=======
	pos = userptr;
	size = total_size;
	xt_entry_foreach(iter, private->entries, total_size) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = compat_copy_entry_to_user(iter, &pos,
						&size, counters, i++);
		if (ret != 0)
			break;
	}
	vfree(counters);
	return ret;
}

struct compat_arpt_get_entries {
	char name[XT_TABLE_MAXNAMELEN];
	compat_uint_t size;
<<<<<<< HEAD
	struct compat_arpt_entry entrytable[0];
=======
	struct compat_arpt_entry entrytable[];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int compat_get_entries(struct net *net,
			      struct compat_arpt_get_entries __user *uptr,
			      int *len)
{
	int ret;
	struct compat_arpt_get_entries get;
	struct xt_table *t;

<<<<<<< HEAD
	if (*len < sizeof(get)) {
		duprintf("compat_get_entries: %u < %zu\n", *len, sizeof(get));
		return -EINVAL;
	}
	if (copy_from_user(&get, uptr, sizeof(get)) != 0)
		return -EFAULT;
	if (*len != sizeof(struct compat_arpt_get_entries) + get.size) {
		duprintf("compat_get_entries: %u != %zu\n",
			 *len, sizeof(get) + get.size);
		return -EINVAL;
	}

	xt_compat_lock(NFPROTO_ARP);
	t = xt_find_table_lock(net, NFPROTO_ARP, get.name);
	if (t && !IS_ERR(t)) {
		const struct xt_table_info *private = t->private;
		struct xt_table_info info;

		duprintf("t->private->number = %u\n", private->number);
=======
	if (*len < sizeof(get))
		return -EINVAL;
	if (copy_from_user(&get, uptr, sizeof(get)) != 0)
		return -EFAULT;
	if (*len != sizeof(struct compat_arpt_get_entries) + get.size)
		return -EINVAL;

	get.name[sizeof(get.name) - 1] = '\0';

	xt_compat_lock(NFPROTO_ARP);
	t = xt_find_table_lock(net, NFPROTO_ARP, get.name);
	if (!IS_ERR(t)) {
		const struct xt_table_info *private = t->private;
		struct xt_table_info info;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = compat_table_info(private, &info);
		if (!ret && get.size == info.size) {
			ret = compat_copy_entries_to_user(private->size,
							  t, uptr->entrytable);
<<<<<<< HEAD
		} else if (!ret) {
			duprintf("compat_get_entries: I've got %u not %u!\n",
				 private->size, get.size);
			ret = -EAGAIN;
		}
=======
		} else if (!ret)
			ret = -EAGAIN;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		xt_compat_flush_offsets(NFPROTO_ARP);
		module_put(t->me);
		xt_table_unlock(t);
	} else
<<<<<<< HEAD
		ret = t ? PTR_ERR(t) : -ENOENT;
=======
		ret = PTR_ERR(t);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	xt_compat_unlock(NFPROTO_ARP);
	return ret;
}
<<<<<<< HEAD

static int do_arpt_get_ctl(struct sock *, int, void __user *, int *);

static int compat_do_arpt_get_ctl(struct sock *sk, int cmd, void __user *user,
				  int *len)
{
	int ret;

	if (!ns_capable(sock_net(sk)->user_ns, CAP_NET_ADMIN))
		return -EPERM;

	switch (cmd) {
	case ARPT_SO_GET_INFO:
		ret = get_info(sock_net(sk), user, len, 1);
		break;
	case ARPT_SO_GET_ENTRIES:
		ret = compat_get_entries(sock_net(sk), user, len);
		break;
	default:
		ret = do_arpt_get_ctl(sk, cmd, user, len);
	}
	return ret;
}
#endif

static int do_arpt_set_ctl(struct sock *sk, int cmd, void __user *user, unsigned int len)
=======
#endif

static int do_arpt_set_ctl(struct sock *sk, int cmd, sockptr_t arg,
		unsigned int len)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int ret;

	if (!ns_capable(sock_net(sk)->user_ns, CAP_NET_ADMIN))
		return -EPERM;

	switch (cmd) {
	case ARPT_SO_SET_REPLACE:
<<<<<<< HEAD
		ret = do_replace(sock_net(sk), user, len);
		break;

	case ARPT_SO_SET_ADD_COUNTERS:
		ret = do_add_counters(sock_net(sk), user, len, 0);
		break;

	default:
		duprintf("do_arpt_set_ctl:  unknown request %i\n", cmd);
=======
#ifdef CONFIG_NETFILTER_XTABLES_COMPAT
		if (in_compat_syscall())
			ret = compat_do_replace(sock_net(sk), arg, len);
		else
#endif
			ret = do_replace(sock_net(sk), arg, len);
		break;

	case ARPT_SO_SET_ADD_COUNTERS:
		ret = do_add_counters(sock_net(sk), arg, len);
		break;

	default:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -EINVAL;
	}

	return ret;
}

static int do_arpt_get_ctl(struct sock *sk, int cmd, void __user *user, int *len)
{
	int ret;

	if (!ns_capable(sock_net(sk)->user_ns, CAP_NET_ADMIN))
		return -EPERM;

	switch (cmd) {
	case ARPT_SO_GET_INFO:
<<<<<<< HEAD
		ret = get_info(sock_net(sk), user, len, 0);
		break;

	case ARPT_SO_GET_ENTRIES:
		ret = get_entries(sock_net(sk), user, len);
=======
		ret = get_info(sock_net(sk), user, len);
		break;

	case ARPT_SO_GET_ENTRIES:
#ifdef CONFIG_NETFILTER_XTABLES_COMPAT
		if (in_compat_syscall())
			ret = compat_get_entries(sock_net(sk), user, len);
		else
#endif
			ret = get_entries(sock_net(sk), user, len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;

	case ARPT_SO_GET_REVISION_TARGET: {
		struct xt_get_revision rev;

		if (*len != sizeof(rev)) {
			ret = -EINVAL;
			break;
		}
		if (copy_from_user(&rev, user, sizeof(rev)) != 0) {
			ret = -EFAULT;
			break;
		}
		rev.name[sizeof(rev.name)-1] = 0;

		try_then_request_module(xt_find_revision(NFPROTO_ARP, rev.name,
							 rev.revision, 1, &ret),
					"arpt_%s", rev.name);
		break;
	}

	default:
<<<<<<< HEAD
		duprintf("do_arpt_get_ctl: unknown request %i\n", cmd);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -EINVAL;
	}

	return ret;
}

<<<<<<< HEAD
struct xt_table *arpt_register_table(struct net *net,
				     const struct xt_table *table,
				     const struct arpt_replace *repl)
{
	int ret;
	struct xt_table_info *newinfo;
	struct xt_table_info bootstrap = {0};
	void *loc_cpu_entry;
	struct xt_table *new_table;

	newinfo = xt_alloc_table_info(repl->size);
	if (!newinfo) {
		ret = -ENOMEM;
		goto out;
	}

	/* choose the copy on our node/cpu */
	loc_cpu_entry = newinfo->entries[raw_smp_processor_id()];
	memcpy(loc_cpu_entry, repl->entries, repl->size);

	ret = translate_table(newinfo, loc_cpu_entry, repl);
	duprintf("arpt_register_table: translate table gives %d\n", ret);
	if (ret != 0)
		goto out_free;

	new_table = xt_register_table(net, table, &bootstrap, newinfo);
	if (IS_ERR(new_table)) {
		ret = PTR_ERR(new_table);
		goto out_free;
	}
	return new_table;

out_free:
	xt_free_table_info(newinfo);
out:
	return ERR_PTR(ret);
}

void arpt_unregister_table(struct xt_table *table)
=======
static void __arpt_unregister_table(struct net *net, struct xt_table *table)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct xt_table_info *private;
	void *loc_cpu_entry;
	struct module *table_owner = table->me;
	struct arpt_entry *iter;

	private = xt_unregister_table(table);

	/* Decrease module usage counts and free resources */
<<<<<<< HEAD
	loc_cpu_entry = private->entries[raw_smp_processor_id()];
	xt_entry_foreach(iter, loc_cpu_entry, private->size)
		cleanup_entry(iter);
=======
	loc_cpu_entry = private->entries;
	xt_entry_foreach(iter, loc_cpu_entry, private->size)
		cleanup_entry(iter, net);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (private->number > private->initial_entries)
		module_put(table_owner);
	xt_free_table_info(private);
}

<<<<<<< HEAD
=======
int arpt_register_table(struct net *net,
			const struct xt_table *table,
			const struct arpt_replace *repl,
			const struct nf_hook_ops *template_ops)
{
	struct nf_hook_ops *ops;
	unsigned int num_ops;
	int ret, i;
	struct xt_table_info *newinfo;
	struct xt_table_info bootstrap = {0};
	void *loc_cpu_entry;
	struct xt_table *new_table;

	newinfo = xt_alloc_table_info(repl->size);
	if (!newinfo)
		return -ENOMEM;

	loc_cpu_entry = newinfo->entries;
	memcpy(loc_cpu_entry, repl->entries, repl->size);

	ret = translate_table(net, newinfo, loc_cpu_entry, repl);
	if (ret != 0) {
		xt_free_table_info(newinfo);
		return ret;
	}

	new_table = xt_register_table(net, table, &bootstrap, newinfo);
	if (IS_ERR(new_table)) {
		struct arpt_entry *iter;

		xt_entry_foreach(iter, loc_cpu_entry, newinfo->size)
			cleanup_entry(iter, net);
		xt_free_table_info(newinfo);
		return PTR_ERR(new_table);
	}

	num_ops = hweight32(table->valid_hooks);
	if (num_ops == 0) {
		ret = -EINVAL;
		goto out_free;
	}

	ops = kmemdup(template_ops, sizeof(*ops) * num_ops, GFP_KERNEL);
	if (!ops) {
		ret = -ENOMEM;
		goto out_free;
	}

	for (i = 0; i < num_ops; i++)
		ops[i].priv = new_table;

	new_table->ops = ops;

	ret = nf_register_net_hooks(net, ops, num_ops);
	if (ret != 0)
		goto out_free;

	return ret;

out_free:
	__arpt_unregister_table(net, new_table);
	return ret;
}

void arpt_unregister_table_pre_exit(struct net *net, const char *name)
{
	struct xt_table *table = xt_find_table(net, NFPROTO_ARP, name);

	if (table)
		nf_unregister_net_hooks(net, table->ops, hweight32(table->valid_hooks));
}
EXPORT_SYMBOL(arpt_unregister_table_pre_exit);

void arpt_unregister_table(struct net *net, const char *name)
{
	struct xt_table *table = xt_find_table(net, NFPROTO_ARP, name);

	if (table)
		__arpt_unregister_table(net, table);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* The built-in targets: standard (NULL) and error. */
static struct xt_target arpt_builtin_tg[] __read_mostly = {
	{
		.name             = XT_STANDARD_TARGET,
		.targetsize       = sizeof(int),
		.family           = NFPROTO_ARP,
<<<<<<< HEAD
#ifdef CONFIG_COMPAT
=======
#ifdef CONFIG_NETFILTER_XTABLES_COMPAT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.compatsize       = sizeof(compat_int_t),
		.compat_from_user = compat_standard_from_user,
		.compat_to_user   = compat_standard_to_user,
#endif
	},
	{
		.name             = XT_ERROR_TARGET,
		.target           = arpt_error,
		.targetsize       = XT_FUNCTION_MAXNAMELEN,
		.family           = NFPROTO_ARP,
	},
};

static struct nf_sockopt_ops arpt_sockopts = {
	.pf		= PF_INET,
	.set_optmin	= ARPT_BASE_CTL,
	.set_optmax	= ARPT_SO_SET_MAX+1,
	.set		= do_arpt_set_ctl,
<<<<<<< HEAD
#ifdef CONFIG_COMPAT
	.compat_set	= compat_do_arpt_set_ctl,
#endif
	.get_optmin	= ARPT_BASE_CTL,
	.get_optmax	= ARPT_SO_GET_MAX+1,
	.get		= do_arpt_get_ctl,
#ifdef CONFIG_COMPAT
	.compat_get	= compat_do_arpt_get_ctl,
#endif
=======
	.get_optmin	= ARPT_BASE_CTL,
	.get_optmax	= ARPT_SO_GET_MAX+1,
	.get		= do_arpt_get_ctl,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.owner		= THIS_MODULE,
};

static int __net_init arp_tables_net_init(struct net *net)
{
	return xt_proto_init(net, NFPROTO_ARP);
}

static void __net_exit arp_tables_net_exit(struct net *net)
{
	xt_proto_fini(net, NFPROTO_ARP);
}

static struct pernet_operations arp_tables_net_ops = {
	.init = arp_tables_net_init,
	.exit = arp_tables_net_exit,
};

static int __init arp_tables_init(void)
{
	int ret;

	ret = register_pernet_subsys(&arp_tables_net_ops);
	if (ret < 0)
		goto err1;

	/* No one else will be downing sem now, so we won't sleep */
	ret = xt_register_targets(arpt_builtin_tg, ARRAY_SIZE(arpt_builtin_tg));
	if (ret < 0)
		goto err2;

	/* Register setsockopt */
	ret = nf_register_sockopt(&arpt_sockopts);
	if (ret < 0)
		goto err4;

<<<<<<< HEAD
	printk(KERN_INFO "arp_tables: (C) 2002 David S. Miller\n");
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;

err4:
	xt_unregister_targets(arpt_builtin_tg, ARRAY_SIZE(arpt_builtin_tg));
err2:
	unregister_pernet_subsys(&arp_tables_net_ops);
err1:
	return ret;
}

static void __exit arp_tables_fini(void)
{
	nf_unregister_sockopt(&arpt_sockopts);
	xt_unregister_targets(arpt_builtin_tg, ARRAY_SIZE(arpt_builtin_tg));
	unregister_pernet_subsys(&arp_tables_net_ops);
}

EXPORT_SYMBOL(arpt_register_table);
EXPORT_SYMBOL(arpt_unregister_table);
EXPORT_SYMBOL(arpt_do_table);

module_init(arp_tables_init);
module_exit(arp_tables_fini);
