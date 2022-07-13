/*
 * XZ decoder module information
 *
 * Author: Lasse Collin <lasse.collin@tukaani.org>
 *
 * This file has been put into the public domain.
 * You can do whatever you want with this file.
 */

#include <linux/module.h>
#include <linux/xz.h>

EXPORT_SYMBOL(xz_dec_init);
EXPORT_SYMBOL(xz_dec_reset);
EXPORT_SYMBOL(xz_dec_run);
EXPORT_SYMBOL(xz_dec_end);

<<<<<<< HEAD
MODULE_DESCRIPTION("XZ decompressor");
MODULE_VERSION("1.0");
=======
#ifdef CONFIG_XZ_DEC_MICROLZMA
EXPORT_SYMBOL(xz_dec_microlzma_alloc);
EXPORT_SYMBOL(xz_dec_microlzma_reset);
EXPORT_SYMBOL(xz_dec_microlzma_run);
EXPORT_SYMBOL(xz_dec_microlzma_end);
#endif

MODULE_DESCRIPTION("XZ decompressor");
MODULE_VERSION("1.1");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_AUTHOR("Lasse Collin <lasse.collin@tukaani.org> and Igor Pavlov");

/*
 * This code is in the public domain, but in Linux it's simplest to just
 * say it's GPL and consider the authors as the copyright holders.
 */
MODULE_LICENSE("GPL");
