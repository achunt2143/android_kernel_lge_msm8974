<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright 2008-2011 Freescale Semiconductor, Inc.
 */

#ifndef CAAM_COMPAT_H
#define CAAM_COMPAT_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mod_devicetable.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/crypto.h>
<<<<<<< HEAD
=======
#include <linux/hash.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/hw_random.h>
#include <linux/of_platform.h>
#include <linux/dma-mapping.h>
#include <linux/io.h>
<<<<<<< HEAD
=======
#include <linux/iommu.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/spinlock.h>
#include <linux/rtnetlink.h>
#include <linux/in.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/debugfs.h>
#include <linux/circ_buf.h>
<<<<<<< HEAD
#include <net/xfrm.h>

#include <crypto/algapi.h>
#include <crypto/aes.h>
#include <crypto/des.h>
#include <crypto/sha.h>
#include <crypto/md5.h>
#include <crypto/aead.h>
#include <crypto/authenc.h>
#include <crypto/scatterwalk.h>
#include <crypto/internal/skcipher.h>
=======
#include <linux/clk.h>
#include <net/xfrm.h>

#include <crypto/algapi.h>
#include <crypto/null.h>
#include <crypto/aes.h>
#include <crypto/ctr.h>
#include <crypto/internal/des.h>
#include <crypto/gcm.h>
#include <crypto/sha1.h>
#include <crypto/sha2.h>
#include <crypto/md5.h>
#include <crypto/chacha.h>
#include <crypto/poly1305.h>
#include <crypto/internal/aead.h>
#include <crypto/authenc.h>
#include <crypto/akcipher.h>
#include <crypto/scatterwalk.h>
#include <crypto/skcipher.h>
#include <crypto/internal/skcipher.h>
#include <crypto/internal/hash.h>
#include <crypto/internal/rsa.h>
#include <crypto/internal/akcipher.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* !defined(CAAM_COMPAT_H) */
