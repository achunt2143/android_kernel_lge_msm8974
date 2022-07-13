<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Portions:
 * Copyright (C) 2023 Intel Corporation
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* routines exported for debugfs handling */

#ifndef __IEEE80211_DEBUGFS_NETDEV_H
#define __IEEE80211_DEBUGFS_NETDEV_H

<<<<<<< HEAD
#ifdef CONFIG_MAC80211_DEBUGFS
void ieee80211_debugfs_add_netdev(struct ieee80211_sub_if_data *sdata);
void ieee80211_debugfs_remove_netdev(struct ieee80211_sub_if_data *sdata);
void ieee80211_debugfs_rename_netdev(struct ieee80211_sub_if_data *sdata);
#else
static inline void ieee80211_debugfs_add_netdev(
	struct ieee80211_sub_if_data *sdata)
{}
=======
#include "ieee80211_i.h"

#ifdef CONFIG_MAC80211_DEBUGFS
void ieee80211_debugfs_remove_netdev(struct ieee80211_sub_if_data *sdata);
void ieee80211_debugfs_rename_netdev(struct ieee80211_sub_if_data *sdata);
void ieee80211_debugfs_recreate_netdev(struct ieee80211_sub_if_data *sdata,
				       bool mld_vif);

void ieee80211_link_debugfs_add(struct ieee80211_link_data *link);
void ieee80211_link_debugfs_remove(struct ieee80211_link_data *link);

void ieee80211_link_debugfs_drv_add(struct ieee80211_link_data *link);
void ieee80211_link_debugfs_drv_remove(struct ieee80211_link_data *link);
#else
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void ieee80211_debugfs_remove_netdev(
	struct ieee80211_sub_if_data *sdata)
{}
static inline void ieee80211_debugfs_rename_netdev(
	struct ieee80211_sub_if_data *sdata)
{}
<<<<<<< HEAD
=======
static inline void ieee80211_debugfs_recreate_netdev(
	struct ieee80211_sub_if_data *sdata, bool mld_vif)
{}
static inline void ieee80211_link_debugfs_add(struct ieee80211_link_data *link)
{}
static inline void ieee80211_link_debugfs_remove(struct ieee80211_link_data *link)
{}

static inline void ieee80211_link_debugfs_drv_add(struct ieee80211_link_data *link)
{}
static inline void ieee80211_link_debugfs_drv_remove(struct ieee80211_link_data *link)
{}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#endif /* __IEEE80211_DEBUGFS_NETDEV_H */
