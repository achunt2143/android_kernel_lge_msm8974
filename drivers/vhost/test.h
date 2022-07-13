<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef LINUX_VHOST_TEST_H
#define LINUX_VHOST_TEST_H

/* Start a given test on the virtio null device. 0 stops all tests. */
#define VHOST_TEST_RUN _IOW(VHOST_VIRTIO, 0x31, int)
<<<<<<< HEAD
=======
#define VHOST_TEST_SET_BACKEND _IOW(VHOST_VIRTIO, 0x32, int)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
