<<<<<<< HEAD
/*
 * Copyright (c) 2006 Intel Corporation.  All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#if !defined(RDMA_CM_IB_H)
=======
/* SPDX-License-Identifier: GPL-2.0 OR Linux-OpenIB */
/*
 * Copyright (c) 2006 Intel Corporation.  All rights reserved.
 */

#ifndef RDMA_CM_IB_H
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define RDMA_CM_IB_H

#include <rdma/rdma_cm.h>

/**
<<<<<<< HEAD
 * rdma_set_ib_paths - Manually sets the path records used to establish a
=======
 * rdma_set_ib_path - Manually sets the path record used to establish a
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *   connection.
 * @id: Connection identifier associated with the request.
 * @path_rec: Reference to the path record
 *
 * This call permits a user to specify routing information for rdma_cm_id's
<<<<<<< HEAD
 * bound to Infiniband devices.  It is called on the client side of a
 * connection and replaces the call to rdma_resolve_route.
 */
int rdma_set_ib_paths(struct rdma_cm_id *id,
		      struct ib_sa_path_rec *path_rec, int num_paths);
=======
 * bound to InfiniBand devices. It is called on the client side of a
 * connection and replaces the call to rdma_resolve_route.
 */
int rdma_set_ib_path(struct rdma_cm_id *id,
		     struct sa_path_rec *path_rec);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Global qkey for UDP QPs and multicast groups. */
#define RDMA_UDP_QKEY 0x01234567

#endif /* RDMA_CM_IB_H */
