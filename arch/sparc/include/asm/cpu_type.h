<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __ASM_CPU_TYPE_H
#define __ASM_CPU_TYPE_H

/*
 * Sparc (general) CPU types
 */
enum sparc_cpu {
<<<<<<< HEAD
  sun4        = 0x00,
  sun4c       = 0x01,
  sun4m       = 0x02,
  sun4d       = 0x03,
  sun4e       = 0x04,
  sun4u       = 0x05, /* V8 ploos ploos */
  sun_unknown = 0x06,
  ap1000      = 0x07, /* almost a sun4m */
  sparc_leon  = 0x08, /* Leon SoC */
=======
  sun4m       = 0x00,
  sun4d       = 0x01,
  sun4e       = 0x02,
  sun4u       = 0x03, /* V8 ploos ploos */
  sun_unknown = 0x04,
  ap1000      = 0x05, /* almost a sun4m */
  sparc_leon  = 0x06, /* Leon SoC */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#ifdef CONFIG_SPARC32
extern enum sparc_cpu sparc_cpu_model;

<<<<<<< HEAD
#define ARCH_SUN4C (sparc_cpu_model==sun4c)

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SUN4M_NCPUS            4              /* Architectural limit of sun4m. */

#else

#define sparc_cpu_model sun4u

<<<<<<< HEAD
/* This cannot ever be a sun4c :) That's just history. */
#define ARCH_SUN4C 0
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#endif /* __ASM_CPU_TYPE_H */
