<<<<<<< HEAD
#ifndef _NET_INET_DATALINK_H_
#define _NET_INET_DATALINK_H_

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _NET_INET_DATALINK_H_
#define _NET_INET_DATALINK_H_

#include <linux/list.h>

struct llc_sap;
struct net_device;
struct packet_type;
struct sk_buff;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct datalink_proto {
        unsigned char   type[8];

	struct llc_sap   *sap;

        unsigned short  header_length;

        int     (*rcvfunc)(struct sk_buff *, struct net_device *,
                                struct packet_type *, struct net_device *);
	int     (*request)(struct datalink_proto *, struct sk_buff *,
<<<<<<< HEAD
                                        unsigned char *);
=======
			   const unsigned char *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct list_head node;
};

#endif
