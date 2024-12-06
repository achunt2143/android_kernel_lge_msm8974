<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _RDMA_TRANSPORT_H
#define _RDMA_TRANSPORT_H

#include <rdma/ib_verbs.h>
#include <rdma/rdma_cm.h>
#include "rds.h"

<<<<<<< HEAD
#define RDS_RDMA_RESOLVE_TIMEOUT_MS     5000

int rds_rdma_conn_connect(struct rds_connection *conn);
int rds_rdma_cm_event_handler(struct rdma_cm_id *cm_id,
			      struct rdma_cm_event *event);
=======
/* RDMA_CM also uses 16385 as the listener port. */
#define RDS_CM_PORT	16385

#define RDS_RDMA_RESOLVE_TIMEOUT_MS     5000

/* Below reject reason is for legacy interoperability issue with non-linux
 * RDS endpoints where older version incompatibility is conveyed via value 1.
 * For future version(s), proper encoded reject reason should be used.
 */
#define RDS_RDMA_REJ_INCOMPAT		1

int rds_rdma_cm_event_handler(struct rdma_cm_id *cm_id,
			      struct rdma_cm_event *event);
int rds6_rdma_cm_event_handler(struct rdma_cm_id *cm_id,
			       struct rdma_cm_event *event);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* from ib.c */
extern struct rds_transport rds_ib_transport;
int rds_ib_init(void);
void rds_ib_exit(void);

<<<<<<< HEAD
/* from iw.c */
extern struct rds_transport rds_iw_transport;
int rds_iw_init(void);
void rds_iw_exit(void);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
