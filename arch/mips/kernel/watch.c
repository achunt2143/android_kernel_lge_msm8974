/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2008 David Daney
 */

#include <linux/sched.h>

#include <asm/processor.h>
#include <asm/watch.h>

/*
<<<<<<< HEAD
 * Install the watch registers for the current thread.  A maximum of
 * four registers are installed although the machine may have more.
 */
void mips_install_watch_registers(void)
{
	struct mips3264_watch_reg_state *watches =
		&current->thread.watch.mips3264;
=======
 * Install the watch registers for the current thread.	A maximum of
 * four registers are installed although the machine may have more.
 */
void mips_install_watch_registers(struct task_struct *t)
{
	struct mips3264_watch_reg_state *watches = &t->thread.watch.mips3264;
	unsigned int watchhi = MIPS_WATCHHI_G |		/* Trap all ASIDs */
			       MIPS_WATCHHI_IRW;	/* Clear result bits */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (current_cpu_data.watch_reg_use_cnt) {
	default:
		BUG();
	case 4:
		write_c0_watchlo3(watches->watchlo[3]);
<<<<<<< HEAD
		/* Write 1 to the I, R, and W bits to clear them, and
		   1 to G so all ASIDs are trapped. */
		write_c0_watchhi3(0x40000007 | watches->watchhi[3]);
	case 3:
		write_c0_watchlo2(watches->watchlo[2]);
		write_c0_watchhi2(0x40000007 | watches->watchhi[2]);
	case 2:
		write_c0_watchlo1(watches->watchlo[1]);
		write_c0_watchhi1(0x40000007 | watches->watchhi[1]);
	case 1:
		write_c0_watchlo0(watches->watchlo[0]);
		write_c0_watchhi0(0x40000007 | watches->watchhi[0]);
=======
		write_c0_watchhi3(watchhi | watches->watchhi[3]);
		fallthrough;
	case 3:
		write_c0_watchlo2(watches->watchlo[2]);
		write_c0_watchhi2(watchhi | watches->watchhi[2]);
		fallthrough;
	case 2:
		write_c0_watchlo1(watches->watchlo[1]);
		write_c0_watchhi1(watchhi | watches->watchhi[1]);
		fallthrough;
	case 1:
		write_c0_watchlo0(watches->watchlo[0]);
		write_c0_watchhi0(watchhi | watches->watchhi[0]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/*
 * Read back the watchhi registers so the user space debugger has
 * access to the I, R, and W bits.  A maximum of four registers are
 * read although the machine may have more.
 */
void mips_read_watch_registers(void)
{
	struct mips3264_watch_reg_state *watches =
		&current->thread.watch.mips3264;
<<<<<<< HEAD
=======
	unsigned int watchhi_mask = MIPS_WATCHHI_MASK | MIPS_WATCHHI_IRW;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (current_cpu_data.watch_reg_use_cnt) {
	default:
		BUG();
	case 4:
<<<<<<< HEAD
		watches->watchhi[3] = (read_c0_watchhi3() & 0x0fff);
	case 3:
		watches->watchhi[2] = (read_c0_watchhi2() & 0x0fff);
	case 2:
		watches->watchhi[1] = (read_c0_watchhi1() & 0x0fff);
	case 1:
		watches->watchhi[0] = (read_c0_watchhi0() & 0x0fff);
	}
	if (current_cpu_data.watch_reg_use_cnt == 1 &&
	    (watches->watchhi[0] & 7) == 0) {
=======
		watches->watchhi[3] = (read_c0_watchhi3() & watchhi_mask);
		fallthrough;
	case 3:
		watches->watchhi[2] = (read_c0_watchhi2() & watchhi_mask);
		fallthrough;
	case 2:
		watches->watchhi[1] = (read_c0_watchhi1() & watchhi_mask);
		fallthrough;
	case 1:
		watches->watchhi[0] = (read_c0_watchhi0() & watchhi_mask);
	}
	if (current_cpu_data.watch_reg_use_cnt == 1 &&
	    (watches->watchhi[0] & MIPS_WATCHHI_IRW) == 0) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* Pathological case of release 1 architecture that
		 * doesn't set the condition bits.  We assume that
		 * since we got here, the watch condition was met and
		 * signal that the conditions requested in watchlo
		 * were met.  */
<<<<<<< HEAD
		watches->watchhi[0] |= (watches->watchlo[0] & 7);
=======
		watches->watchhi[0] |= (watches->watchlo[0] & MIPS_WATCHHI_IRW);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
 }

/*
<<<<<<< HEAD
 * Disable all watch registers.  Although only four registers are
=======
 * Disable all watch registers.	 Although only four registers are
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * installed, all are cleared to eliminate the possibility of endless
 * looping in the watch handler.
 */
void mips_clear_watch_registers(void)
{
	switch (current_cpu_data.watch_reg_count) {
	default:
		BUG();
	case 8:
		write_c0_watchlo7(0);
<<<<<<< HEAD
	case 7:
		write_c0_watchlo6(0);
	case 6:
		write_c0_watchlo5(0);
	case 5:
		write_c0_watchlo4(0);
	case 4:
		write_c0_watchlo3(0);
	case 3:
		write_c0_watchlo2(0);
	case 2:
		write_c0_watchlo1(0);
=======
		fallthrough;
	case 7:
		write_c0_watchlo6(0);
		fallthrough;
	case 6:
		write_c0_watchlo5(0);
		fallthrough;
	case 5:
		write_c0_watchlo4(0);
		fallthrough;
	case 4:
		write_c0_watchlo3(0);
		fallthrough;
	case 3:
		write_c0_watchlo2(0);
		fallthrough;
	case 2:
		write_c0_watchlo1(0);
		fallthrough;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case 1:
		write_c0_watchlo0(0);
	}
}

<<<<<<< HEAD
__cpuinit void mips_probe_watch_registers(struct cpuinfo_mips *c)
=======
void mips_probe_watch_registers(struct cpuinfo_mips *c)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned int t;

	if ((c->options & MIPS_CPU_WATCH) == 0)
		return;
	/*
	 * Check which of the I,R and W bits are supported, then
	 * disable the register.
	 */
<<<<<<< HEAD
	write_c0_watchlo0(7);
	t = read_c0_watchlo0();
	write_c0_watchlo0(0);
	c->watch_reg_masks[0] = t & 7;
=======
	write_c0_watchlo0(MIPS_WATCHLO_IRW);
	back_to_back_c0_hazard();
	t = read_c0_watchlo0();
	write_c0_watchlo0(0);
	c->watch_reg_masks[0] = t & MIPS_WATCHLO_IRW;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Write the mask bits and read them back to determine which
	 * can be used. */
	c->watch_reg_count = 1;
	c->watch_reg_use_cnt = 1;
	t = read_c0_watchhi0();
<<<<<<< HEAD
	write_c0_watchhi0(t | 0xff8);
	t = read_c0_watchhi0();
	c->watch_reg_masks[0] |= (t & 0xff8);
	if ((t & 0x80000000) == 0)
		return;

	write_c0_watchlo1(7);
	t = read_c0_watchlo1();
	write_c0_watchlo1(0);
	c->watch_reg_masks[1] = t & 7;
=======
	write_c0_watchhi0(t | MIPS_WATCHHI_MASK);
	back_to_back_c0_hazard();
	t = read_c0_watchhi0();
	c->watch_reg_masks[0] |= (t & MIPS_WATCHHI_MASK);
	if ((t & MIPS_WATCHHI_M) == 0)
		return;

	write_c0_watchlo1(MIPS_WATCHLO_IRW);
	back_to_back_c0_hazard();
	t = read_c0_watchlo1();
	write_c0_watchlo1(0);
	c->watch_reg_masks[1] = t & MIPS_WATCHLO_IRW;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	c->watch_reg_count = 2;
	c->watch_reg_use_cnt = 2;
	t = read_c0_watchhi1();
<<<<<<< HEAD
	write_c0_watchhi1(t | 0xff8);
	t = read_c0_watchhi1();
	c->watch_reg_masks[1] |= (t & 0xff8);
	if ((t & 0x80000000) == 0)
		return;

	write_c0_watchlo2(7);
	t = read_c0_watchlo2();
	write_c0_watchlo2(0);
	c->watch_reg_masks[2] = t & 7;
=======
	write_c0_watchhi1(t | MIPS_WATCHHI_MASK);
	back_to_back_c0_hazard();
	t = read_c0_watchhi1();
	c->watch_reg_masks[1] |= (t & MIPS_WATCHHI_MASK);
	if ((t & MIPS_WATCHHI_M) == 0)
		return;

	write_c0_watchlo2(MIPS_WATCHLO_IRW);
	back_to_back_c0_hazard();
	t = read_c0_watchlo2();
	write_c0_watchlo2(0);
	c->watch_reg_masks[2] = t & MIPS_WATCHLO_IRW;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	c->watch_reg_count = 3;
	c->watch_reg_use_cnt = 3;
	t = read_c0_watchhi2();
<<<<<<< HEAD
	write_c0_watchhi2(t | 0xff8);
	t = read_c0_watchhi2();
	c->watch_reg_masks[2] |= (t & 0xff8);
	if ((t & 0x80000000) == 0)
		return;

	write_c0_watchlo3(7);
	t = read_c0_watchlo3();
	write_c0_watchlo3(0);
	c->watch_reg_masks[3] = t & 7;
=======
	write_c0_watchhi2(t | MIPS_WATCHHI_MASK);
	back_to_back_c0_hazard();
	t = read_c0_watchhi2();
	c->watch_reg_masks[2] |= (t & MIPS_WATCHHI_MASK);
	if ((t & MIPS_WATCHHI_M) == 0)
		return;

	write_c0_watchlo3(MIPS_WATCHLO_IRW);
	back_to_back_c0_hazard();
	t = read_c0_watchlo3();
	write_c0_watchlo3(0);
	c->watch_reg_masks[3] = t & MIPS_WATCHLO_IRW;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	c->watch_reg_count = 4;
	c->watch_reg_use_cnt = 4;
	t = read_c0_watchhi3();
<<<<<<< HEAD
	write_c0_watchhi3(t | 0xff8);
	t = read_c0_watchhi3();
	c->watch_reg_masks[3] |= (t & 0xff8);
	if ((t & 0x80000000) == 0)
=======
	write_c0_watchhi3(t | MIPS_WATCHHI_MASK);
	back_to_back_c0_hazard();
	t = read_c0_watchhi3();
	c->watch_reg_masks[3] |= (t & MIPS_WATCHHI_MASK);
	if ((t & MIPS_WATCHHI_M) == 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;

	/* We use at most 4, but probe and report up to 8. */
	c->watch_reg_count = 5;
	t = read_c0_watchhi4();
<<<<<<< HEAD
	if ((t & 0x80000000) == 0)
=======
	if ((t & MIPS_WATCHHI_M) == 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;

	c->watch_reg_count = 6;
	t = read_c0_watchhi5();
<<<<<<< HEAD
	if ((t & 0x80000000) == 0)
=======
	if ((t & MIPS_WATCHHI_M) == 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;

	c->watch_reg_count = 7;
	t = read_c0_watchhi6();
<<<<<<< HEAD
	if ((t & 0x80000000) == 0)
=======
	if ((t & MIPS_WATCHHI_M) == 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;

	c->watch_reg_count = 8;
}
