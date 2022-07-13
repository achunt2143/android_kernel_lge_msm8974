<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __TDO24M_H__
#define __TDO24M_H__

enum tdo24m_model {
	TDO24M,
	TDO35S,
};

struct tdo24m_platform_data {
	enum tdo24m_model model;
};

#endif /* __TDO24M_H__ */
