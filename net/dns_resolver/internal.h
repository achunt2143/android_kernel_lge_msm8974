/*
 *   Copyright (c) 2010 Wang Lei
 *   Author(s): Wang Lei (wang840925@gmail.com). All Rights Reserved.
 *
 *   Internal DNS Rsolver stuff
 *
 *   This library is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as published
 *   by the Free Software Foundation; either version 2.1 of the License, or
 *   (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
<<<<<<< HEAD
 *   along with this library; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
=======
 *   along with this library; if not, see <http://www.gnu.org/licenses/>.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/compiler.h>
#include <linux/kernel.h>
#include <linux/sched.h>

/*
<<<<<<< HEAD
=======
 * Layout of key payload words.
 */
enum {
	dns_key_data,
	dns_key_error,
};

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * dns_key.c
 */
extern const struct cred *dns_resolver_cache;

/*
 * debug tracing
 */
<<<<<<< HEAD
extern unsigned dns_resolver_debug;
=======
extern unsigned int dns_resolver_debug;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define	kdebug(FMT, ...)				\
do {							\
	if (unlikely(dns_resolver_debug))		\
		printk(KERN_DEBUG "[%-6.6s] "FMT"\n",	\
		       current->comm, ##__VA_ARGS__);	\
} while (0)

#define kenter(FMT, ...) kdebug("==> %s("FMT")", __func__, ##__VA_ARGS__)
#define kleave(FMT, ...) kdebug("<== %s()"FMT"", __func__, ##__VA_ARGS__)
