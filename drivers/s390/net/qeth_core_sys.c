<<<<<<< HEAD
/*
 *  drivers/s390/net/qeth_core_sys.c
 *
=======
// SPDX-License-Identifier: GPL-2.0
/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *    Copyright IBM Corp. 2007
 *    Author(s): Utz Bacher <utz.bacher@de.ibm.com>,
 *		 Frank Pavlic <fpavlic@de.ibm.com>,
 *		 Thomas Spatzier <tspat@de.ibm.com>,
 *		 Frank Blaschka <frank.blaschka@de.ibm.com>
 */

#define KMSG_COMPONENT "qeth"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt

#include <linux/list.h>
#include <linux/rwsem.h>
#include <asm/ebcdic.h>

#include "qeth_core.h"

static ssize_t qeth_dev_state_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct qeth_card *card = dev_get_drvdata(dev);
<<<<<<< HEAD
	if (!card)
		return -EINVAL;

	switch (card->state) {
	case CARD_STATE_DOWN:
		return sprintf(buf, "DOWN\n");
	case CARD_STATE_HARDSETUP:
		return sprintf(buf, "HARDSETUP\n");
	case CARD_STATE_SOFTSETUP:
		return sprintf(buf, "SOFTSETUP\n");
	case CARD_STATE_UP:
		if (card->lan_online)
		return sprintf(buf, "UP (LAN ONLINE)\n");
		else
			return sprintf(buf, "UP (LAN OFFLINE)\n");
	case CARD_STATE_RECOVER:
		return sprintf(buf, "RECOVER\n");
	default:
		return sprintf(buf, "UNKNOWN\n");
=======

	switch (card->state) {
	case CARD_STATE_DOWN:
		return sysfs_emit(buf, "DOWN\n");
	case CARD_STATE_SOFTSETUP:
		if (card->dev->flags & IFF_UP)
			return sysfs_emit(buf, "UP (LAN %s)\n",
					  netif_carrier_ok(card->dev) ?
					  "ONLINE" : "OFFLINE");
		return sysfs_emit(buf, "SOFTSETUP\n");
	default:
		return sysfs_emit(buf, "UNKNOWN\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static DEVICE_ATTR(state, 0444, qeth_dev_state_show, NULL);

static ssize_t qeth_dev_chpid_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct qeth_card *card = dev_get_drvdata(dev);
<<<<<<< HEAD
	if (!card)
		return -EINVAL;

	return sprintf(buf, "%02X\n", card->info.chpid);
=======

	return sysfs_emit(buf, "%02X\n", card->info.chpid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static DEVICE_ATTR(chpid, 0444, qeth_dev_chpid_show, NULL);

static ssize_t qeth_dev_if_name_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct qeth_card *card = dev_get_drvdata(dev);
<<<<<<< HEAD
	if (!card)
		return -EINVAL;
	return sprintf(buf, "%s\n", QETH_CARD_IFNAME(card));
=======

	return sysfs_emit(buf, "%s\n", netdev_name(card->dev));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static DEVICE_ATTR(if_name, 0444, qeth_dev_if_name_show, NULL);

static ssize_t qeth_dev_card_type_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct qeth_card *card = dev_get_drvdata(dev);
<<<<<<< HEAD
	if (!card)
		return -EINVAL;

	return sprintf(buf, "%s\n", qeth_get_cardname_short(card));
=======

	return sysfs_emit(buf, "%s\n", qeth_get_cardname_short(card));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static DEVICE_ATTR(card_type, 0444, qeth_dev_card_type_show, NULL);

<<<<<<< HEAD
static inline const char *qeth_get_bufsize_str(struct qeth_card *card)
=======
static const char *qeth_get_bufsize_str(struct qeth_card *card)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (card->qdio.in_buf_size == 16384)
		return "16k";
	else if (card->qdio.in_buf_size == 24576)
		return "24k";
	else if (card->qdio.in_buf_size == 32768)
		return "32k";
	else if (card->qdio.in_buf_size == 40960)
		return "40k";
	else
		return "64k";
}

static ssize_t qeth_dev_inbuf_size_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct qeth_card *card = dev_get_drvdata(dev);
<<<<<<< HEAD
	if (!card)
		return -EINVAL;

	return sprintf(buf, "%s\n", qeth_get_bufsize_str(card));
=======

	return sysfs_emit(buf, "%s\n", qeth_get_bufsize_str(card));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static DEVICE_ATTR(inbuf_size, 0444, qeth_dev_inbuf_size_show, NULL);

static ssize_t qeth_dev_portno_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct qeth_card *card = dev_get_drvdata(dev);
<<<<<<< HEAD
	if (!card)
		return -EINVAL;

	return sprintf(buf, "%i\n", card->info.portno);
=======

	return sysfs_emit(buf, "%i\n", card->dev->dev_port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t qeth_dev_portno_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct qeth_card *card = dev_get_drvdata(dev);
<<<<<<< HEAD
	char *tmp;
	unsigned int portno, limit;
	int rc = 0;

	if (!card)
		return -EINVAL;

	mutex_lock(&card->conf_mutex);
	if ((card->state != CARD_STATE_DOWN) &&
	    (card->state != CARD_STATE_RECOVER)) {
=======
	unsigned int portno, limit;
	int rc = 0;

	rc = kstrtouint(buf, 16, &portno);
	if (rc)
		return rc;
	if (portno > QETH_MAX_PORTNO)
		return -EINVAL;

	mutex_lock(&card->conf_mutex);
	if (card->state != CARD_STATE_DOWN) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rc = -EPERM;
		goto out;
	}

<<<<<<< HEAD
	portno = simple_strtoul(buf, &tmp, 16);
	if (portno > QETH_MAX_PORTNO) {
		rc = -EINVAL;
		goto out;
	}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	limit = (card->ssqd.pcnt ? card->ssqd.pcnt - 1 : card->ssqd.pcnt);
	if (portno > limit) {
		rc = -EINVAL;
		goto out;
	}
<<<<<<< HEAD
	card->info.portno = portno;
=======
	card->dev->dev_port = portno;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	mutex_unlock(&card->conf_mutex);
	return rc ? rc : count;
}

static DEVICE_ATTR(portno, 0644, qeth_dev_portno_show, qeth_dev_portno_store);

static ssize_t qeth_dev_portname_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
<<<<<<< HEAD
	struct qeth_card *card = dev_get_drvdata(dev);
	char portname[9] = {0, };

	if (!card)
		return -EINVAL;

	if (card->info.portname_required) {
		memcpy(portname, card->info.portname + 1, 8);
		EBCASC(portname, 8);
		return sprintf(buf, "%s\n", portname);
	} else
		return sprintf(buf, "no portname required\n");
=======
	return sysfs_emit(buf, "no portname required\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t qeth_dev_portname_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct qeth_card *card = dev_get_drvdata(dev);
<<<<<<< HEAD
	char *tmp;
	int i, rc = 0;

	if (!card)
		return -EINVAL;

	mutex_lock(&card->conf_mutex);
	if ((card->state != CARD_STATE_DOWN) &&
	    (card->state != CARD_STATE_RECOVER)) {
		rc = -EPERM;
		goto out;
	}

	tmp = strsep((char **) &buf, "\n");
	if ((strlen(tmp) > 8) || (strlen(tmp) == 0)) {
		rc = -EINVAL;
		goto out;
	}

	card->info.portname[0] = strlen(tmp);
	/* for beauty reasons */
	for (i = 1; i < 9; i++)
		card->info.portname[i] = ' ';
	strcpy(card->info.portname + 1, tmp);
	ASCEBC(card->info.portname + 1, 8);
out:
	mutex_unlock(&card->conf_mutex);
	return rc ? rc : count;
=======

	dev_warn_once(&card->gdev->dev,
		      "portname is deprecated and is ignored\n");
	return count;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static DEVICE_ATTR(portname, 0644, qeth_dev_portname_show,
		qeth_dev_portname_store);

static ssize_t qeth_dev_prioqing_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct qeth_card *card = dev_get_drvdata(dev);

<<<<<<< HEAD
	if (!card)
		return -EINVAL;

	switch (card->qdio.do_prio_queueing) {
	case QETH_PRIO_Q_ING_PREC:
		return sprintf(buf, "%s\n", "by precedence");
	case QETH_PRIO_Q_ING_TOS:
		return sprintf(buf, "%s\n", "by type of service");
	default:
		return sprintf(buf, "always queue %i\n",
			       card->qdio.default_out_queue);
=======
	switch (card->qdio.do_prio_queueing) {
	case QETH_PRIO_Q_ING_PREC:
		return sysfs_emit(buf, "%s\n", "by precedence");
	case QETH_PRIO_Q_ING_TOS:
		return sysfs_emit(buf, "%s\n", "by type of service");
	case QETH_PRIO_Q_ING_SKB:
		return sysfs_emit(buf, "%s\n", "by skb-priority");
	case QETH_PRIO_Q_ING_VLAN:
		return sysfs_emit(buf, "%s\n", "by VLAN headers");
	case QETH_PRIO_Q_ING_FIXED:
		return sysfs_emit(buf, "always queue %i\n",
			       card->qdio.default_out_queue);
	default:
		return sysfs_emit(buf, "disabled\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static ssize_t qeth_dev_prioqing_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct qeth_card *card = dev_get_drvdata(dev);
<<<<<<< HEAD
	char *tmp;
	int rc = 0;

	if (!card)
		return -EINVAL;

	mutex_lock(&card->conf_mutex);
	if ((card->state != CARD_STATE_DOWN) &&
	    (card->state != CARD_STATE_RECOVER)) {
=======
	int rc = 0;

	if (IS_IQD(card) || IS_VM_NIC(card))
		return -EOPNOTSUPP;

	mutex_lock(&card->conf_mutex);
	if (card->state != CARD_STATE_DOWN) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rc = -EPERM;
		goto out;
	}

	/* check if 1920 devices are supported ,
	 * if though we have to permit priority queueing
	 */
	if (card->qdio.no_out_queues == 1) {
		card->qdio.do_prio_queueing = QETH_PRIOQ_DEFAULT;
		rc = -EPERM;
		goto out;
	}

<<<<<<< HEAD
	tmp = strsep((char **) &buf, "\n");
	if (!strcmp(tmp, "prio_queueing_prec"))
		card->qdio.do_prio_queueing = QETH_PRIO_Q_ING_PREC;
	else if (!strcmp(tmp, "prio_queueing_tos"))
		card->qdio.do_prio_queueing = QETH_PRIO_Q_ING_TOS;
	else if (!strcmp(tmp, "no_prio_queueing:0")) {
		card->qdio.do_prio_queueing = QETH_NO_PRIO_QUEUEING;
		card->qdio.default_out_queue = 0;
	} else if (!strcmp(tmp, "no_prio_queueing:1")) {
		card->qdio.do_prio_queueing = QETH_NO_PRIO_QUEUEING;
		card->qdio.default_out_queue = 1;
	} else if (!strcmp(tmp, "no_prio_queueing:2")) {
		card->qdio.do_prio_queueing = QETH_NO_PRIO_QUEUEING;
		card->qdio.default_out_queue = 2;
	} else if (!strcmp(tmp, "no_prio_queueing:3")) {
		card->qdio.do_prio_queueing = QETH_NO_PRIO_QUEUEING;
		card->qdio.default_out_queue = 3;
	} else if (!strcmp(tmp, "no_prio_queueing")) {
=======
	if (sysfs_streq(buf, "prio_queueing_prec")) {
		card->qdio.do_prio_queueing = QETH_PRIO_Q_ING_PREC;
		card->qdio.default_out_queue = QETH_DEFAULT_QUEUE;
	} else if (sysfs_streq(buf, "prio_queueing_skb")) {
		card->qdio.do_prio_queueing = QETH_PRIO_Q_ING_SKB;
		card->qdio.default_out_queue = QETH_DEFAULT_QUEUE;
	} else if (sysfs_streq(buf, "prio_queueing_tos")) {
		card->qdio.do_prio_queueing = QETH_PRIO_Q_ING_TOS;
		card->qdio.default_out_queue = QETH_DEFAULT_QUEUE;
	} else if (sysfs_streq(buf, "prio_queueing_vlan")) {
		if (IS_LAYER3(card)) {
			rc = -EOPNOTSUPP;
			goto out;
		}
		card->qdio.do_prio_queueing = QETH_PRIO_Q_ING_VLAN;
		card->qdio.default_out_queue = QETH_DEFAULT_QUEUE;
	} else if (sysfs_streq(buf, "no_prio_queueing:0")) {
		card->qdio.do_prio_queueing = QETH_PRIO_Q_ING_FIXED;
		card->qdio.default_out_queue = 0;
	} else if (sysfs_streq(buf, "no_prio_queueing:1")) {
		card->qdio.do_prio_queueing = QETH_PRIO_Q_ING_FIXED;
		card->qdio.default_out_queue = 1;
	} else if (sysfs_streq(buf, "no_prio_queueing:2")) {
		card->qdio.do_prio_queueing = QETH_PRIO_Q_ING_FIXED;
		card->qdio.default_out_queue = 2;
	} else if (sysfs_streq(buf, "no_prio_queueing:3")) {
		card->qdio.do_prio_queueing = QETH_PRIO_Q_ING_FIXED;
		card->qdio.default_out_queue = 3;
	} else if (sysfs_streq(buf, "no_prio_queueing")) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		card->qdio.do_prio_queueing = QETH_NO_PRIO_QUEUEING;
		card->qdio.default_out_queue = QETH_DEFAULT_QUEUE;
	} else
		rc = -EINVAL;
out:
	mutex_unlock(&card->conf_mutex);
	return rc ? rc : count;
}

static DEVICE_ATTR(priority_queueing, 0644, qeth_dev_prioqing_show,
		qeth_dev_prioqing_store);

static ssize_t qeth_dev_bufcnt_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct qeth_card *card = dev_get_drvdata(dev);

<<<<<<< HEAD
	if (!card)
		return -EINVAL;

	return sprintf(buf, "%i\n", card->qdio.in_buf_pool.buf_count);
=======
	return sysfs_emit(buf, "%i\n", card->qdio.in_buf_pool.buf_count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t qeth_dev_bufcnt_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct qeth_card *card = dev_get_drvdata(dev);
<<<<<<< HEAD
	char *tmp;
	int cnt, old_cnt;
	int rc = 0;

	if (!card)
		return -EINVAL;

	mutex_lock(&card->conf_mutex);
	if ((card->state != CARD_STATE_DOWN) &&
	    (card->state != CARD_STATE_RECOVER)) {
=======
	unsigned int cnt;
	int rc = 0;

	rc = kstrtouint(buf, 10, &cnt);
	if (rc)
		return rc;

	mutex_lock(&card->conf_mutex);
	if (card->state != CARD_STATE_DOWN) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rc = -EPERM;
		goto out;
	}

<<<<<<< HEAD
	old_cnt = card->qdio.in_buf_pool.buf_count;
	cnt = simple_strtoul(buf, &tmp, 10);
	cnt = (cnt < QETH_IN_BUF_COUNT_MIN) ? QETH_IN_BUF_COUNT_MIN :
		((cnt > QETH_IN_BUF_COUNT_MAX) ? QETH_IN_BUF_COUNT_MAX : cnt);
	if (old_cnt != cnt) {
		rc = qeth_realloc_buffer_pool(card, cnt);
	}
=======
	cnt = clamp(cnt, QETH_IN_BUF_COUNT_MIN, QETH_IN_BUF_COUNT_MAX);
	rc = qeth_resize_buffer_pool(card, cnt);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	mutex_unlock(&card->conf_mutex);
	return rc ? rc : count;
}

static DEVICE_ATTR(buffer_count, 0644, qeth_dev_bufcnt_show,
		qeth_dev_bufcnt_store);

static ssize_t qeth_dev_recover_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct qeth_card *card = dev_get_drvdata(dev);
<<<<<<< HEAD
	char *tmp;
	int i;

	if (!card)
		return -EINVAL;

	if (card->state != CARD_STATE_UP)
		return -EPERM;

	i = simple_strtoul(buf, &tmp, 16);
	if (i == 1)
		qeth_schedule_recovery(card);

	return count;
=======
	bool reset;
	int rc;

	rc = kstrtobool(buf, &reset);
	if (rc)
		return rc;

	if (!qeth_card_hw_is_reachable(card))
		return -EPERM;

	if (reset)
		rc = qeth_schedule_recovery(card);

	return rc ? rc : count;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static DEVICE_ATTR(recover, 0200, NULL, qeth_dev_recover_store);

static ssize_t qeth_dev_performance_stats_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
<<<<<<< HEAD
	struct qeth_card *card = dev_get_drvdata(dev);

	if (!card)
		return -EINVAL;

	return sprintf(buf, "%i\n", card->options.performance_stats ? 1:0);
=======
	return sysfs_emit(buf, "1\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t qeth_dev_performance_stats_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct qeth_card *card = dev_get_drvdata(dev);
<<<<<<< HEAD
	char *tmp;
	int i, rc = 0;

	if (!card)
		return -EINVAL;

	mutex_lock(&card->conf_mutex);
	i = simple_strtoul(buf, &tmp, 16);
	if ((i == 0) || (i == 1)) {
		if (i == card->options.performance_stats)
			goto out;
		card->options.performance_stats = i;
		if (i == 0)
			memset(&card->perf_stats, 0,
				sizeof(struct qeth_perf_stats));
		card->perf_stats.initial_rx_packets = card->stats.rx_packets;
		card->perf_stats.initial_tx_packets = card->stats.tx_packets;
	} else
		rc = -EINVAL;
out:
	mutex_unlock(&card->conf_mutex);
	return rc ? rc : count;
=======
	struct qeth_qdio_out_q *queue;
	unsigned int i;
	bool reset;
	int rc;

	rc = kstrtobool(buf, &reset);
	if (rc)
		return rc;

	if (reset) {
		memset(&card->stats, 0, sizeof(card->stats));
		for (i = 0; i < card->qdio.no_out_queues; i++) {
			queue = card->qdio.out_qs[i];
			if (!queue)
				break;
			memset(&queue->stats, 0, sizeof(queue->stats));
		}
	}

	return count;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static DEVICE_ATTR(performance_stats, 0644, qeth_dev_performance_stats_show,
		   qeth_dev_performance_stats_store);

static ssize_t qeth_dev_layer2_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct qeth_card *card = dev_get_drvdata(dev);

<<<<<<< HEAD
	if (!card)
		return -EINVAL;

	return sprintf(buf, "%i\n", card->options.layer2);
=======
	return sysfs_emit(buf, "%i\n", card->options.layer);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t qeth_dev_layer2_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct qeth_card *card = dev_get_drvdata(dev);
<<<<<<< HEAD
	char *tmp;
	int i, rc = 0;
	enum qeth_discipline_id newdis;

	if (!card)
		return -EINVAL;

	mutex_lock(&card->discipline_mutex);
	if (card->state != CARD_STATE_DOWN) {
		rc = -EPERM;
		goto out;
	}

	i = simple_strtoul(buf, &tmp, 16);
	switch (i) {
=======
	struct net_device *ndev;
	enum qeth_discipline_id newdis;
	unsigned int input;
	int rc;

	rc = kstrtouint(buf, 16, &input);
	if (rc)
		return rc;

	switch (input) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case 0:
		newdis = QETH_DISCIPLINE_LAYER3;
		break;
	case 1:
		newdis = QETH_DISCIPLINE_LAYER2;
		break;
	default:
<<<<<<< HEAD
		rc = -EINVAL;
		goto out;
	}

	if (card->options.layer2 == newdis)
		goto out;
	else {
		card->info.mac_bits  = 0;
		if (card->discipline.ccwgdriver) {
			card->discipline.ccwgdriver->remove(card->gdev);
			qeth_core_free_discipline(card);
		}
	}

	rc = qeth_core_load_discipline(card, newdis);
	if (rc)
		goto out;

	rc = card->discipline.ccwgdriver->probe(card->gdev);
=======
		return -EINVAL;
	}

	mutex_lock(&card->discipline_mutex);
	if (card->state != CARD_STATE_DOWN) {
		rc = -EPERM;
		goto out;
	}

	if (card->options.layer == newdis)
		goto out;
	if (card->info.layer_enforced) {
		/* fixed layer, can't switch */
		rc = -EOPNOTSUPP;
		goto out;
	}

	if (card->discipline) {
		/* start with a new, pristine netdevice: */
		ndev = qeth_clone_netdev(card->dev);
		if (!ndev) {
			rc = -ENOMEM;
			goto out;
		}

		qeth_remove_discipline(card);
		free_netdev(card->dev);
		card->dev = ndev;
	}

	rc = qeth_setup_discipline(card, newdis);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	mutex_unlock(&card->discipline_mutex);
	return rc ? rc : count;
}

static DEVICE_ATTR(layer2, 0644, qeth_dev_layer2_show,
		   qeth_dev_layer2_store);

#define ATTR_QETH_ISOLATION_NONE	("none")
#define ATTR_QETH_ISOLATION_FWD		("forward")
#define ATTR_QETH_ISOLATION_DROP	("drop")

static ssize_t qeth_dev_isolation_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct qeth_card *card = dev_get_drvdata(dev);

<<<<<<< HEAD
	if (!card)
		return -EINVAL;

	switch (card->options.isolation) {
	case ISOLATION_MODE_NONE:
		return snprintf(buf, 6, "%s\n", ATTR_QETH_ISOLATION_NONE);
	case ISOLATION_MODE_FWD:
		return snprintf(buf, 9, "%s\n", ATTR_QETH_ISOLATION_FWD);
	case ISOLATION_MODE_DROP:
		return snprintf(buf, 6, "%s\n", ATTR_QETH_ISOLATION_DROP);
	default:
		return snprintf(buf, 5, "%s\n", "N/A");
=======
	switch (card->options.isolation) {
	case ISOLATION_MODE_NONE:
		return sysfs_emit(buf, "%s\n", ATTR_QETH_ISOLATION_NONE);
	case ISOLATION_MODE_FWD:
		return sysfs_emit(buf, "%s\n", ATTR_QETH_ISOLATION_FWD);
	case ISOLATION_MODE_DROP:
		return sysfs_emit(buf, "%s\n", ATTR_QETH_ISOLATION_DROP);
	default:
		return sysfs_emit(buf, "%s\n", "N/A");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static ssize_t qeth_dev_isolation_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct qeth_card *card = dev_get_drvdata(dev);
	enum qeth_ipa_isolation_modes isolation;
	int rc = 0;
<<<<<<< HEAD
	char *tmp, *curtoken;
	curtoken = (char *) buf;

	if (!card)
		return -EINVAL;

	mutex_lock(&card->conf_mutex);
	/* check for unknown, too, in case we do not yet know who we are */
	if (card->info.type != QETH_CARD_TYPE_OSD &&
	    card->info.type != QETH_CARD_TYPE_OSX &&
	    card->info.type != QETH_CARD_TYPE_UNKNOWN) {
=======

	mutex_lock(&card->conf_mutex);
	if (!IS_OSD(card) && !IS_OSX(card)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rc = -EOPNOTSUPP;
		dev_err(&card->gdev->dev, "Adapter does not "
			"support QDIO data connection isolation\n");
		goto out;
	}

	/* parse input into isolation mode */
<<<<<<< HEAD
	tmp = strsep(&curtoken, "\n");
	if (!strcmp(tmp, ATTR_QETH_ISOLATION_NONE)) {
		isolation = ISOLATION_MODE_NONE;
	} else if (!strcmp(tmp, ATTR_QETH_ISOLATION_FWD)) {
		isolation = ISOLATION_MODE_FWD;
	} else if (!strcmp(tmp, ATTR_QETH_ISOLATION_DROP)) {
=======
	if (sysfs_streq(buf, ATTR_QETH_ISOLATION_NONE)) {
		isolation = ISOLATION_MODE_NONE;
	} else if (sysfs_streq(buf, ATTR_QETH_ISOLATION_FWD)) {
		isolation = ISOLATION_MODE_FWD;
	} else if (sysfs_streq(buf, ATTR_QETH_ISOLATION_DROP)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		isolation = ISOLATION_MODE_DROP;
	} else {
		rc = -EINVAL;
		goto out;
	}
<<<<<<< HEAD
	rc = count;

	/* defer IP assist if device is offline (until discipline->set_online)*/
	card->options.isolation = isolation;
	if (card->state == CARD_STATE_SOFTSETUP ||
	    card->state == CARD_STATE_UP) {
		int ipa_rc = qeth_set_access_ctrl_online(card);
		if (ipa_rc != 0)
			rc = ipa_rc;
	}
out:
	mutex_unlock(&card->conf_mutex);
	return rc;
}

static DEVICE_ATTR(isolation, 0644, qeth_dev_isolation_show,
		   qeth_dev_isolation_store);
=======

	if (qeth_card_hw_is_reachable(card))
		rc = qeth_setadpparms_set_access_ctrl(card, isolation);

	if (!rc)
		WRITE_ONCE(card->options.isolation, isolation);

out:
	mutex_unlock(&card->conf_mutex);

	return rc ? rc : count;
}

static DEVICE_ATTR(isolation, 0644, qeth_dev_isolation_show,
			qeth_dev_isolation_store);

static ssize_t qeth_dev_switch_attrs_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct qeth_card *card = dev_get_drvdata(dev);
	struct qeth_switch_info sw_info;
	int	rc = 0;

	if (!qeth_card_hw_is_reachable(card))
		return sysfs_emit(buf, "n/a\n");

	rc = qeth_query_switch_attributes(card, &sw_info);
	if (rc)
		return rc;

	if (!sw_info.capabilities)
		rc = sysfs_emit(buf, "unknown");

	if (sw_info.capabilities & QETH_SWITCH_FORW_802_1)
		rc = sysfs_emit(buf,
				(sw_info.settings & QETH_SWITCH_FORW_802_1 ?
				"[802.1]" : "802.1"));
	if (sw_info.capabilities & QETH_SWITCH_FORW_REFL_RELAY)
		rc += sysfs_emit_at(buf, rc,
				    (sw_info.settings &
				    QETH_SWITCH_FORW_REFL_RELAY ?
				    " [rr]" : " rr"));
	rc += sysfs_emit_at(buf, rc, "\n");

	return rc;
}

static DEVICE_ATTR(switch_attrs, 0444,
		   qeth_dev_switch_attrs_show, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static ssize_t qeth_hw_trap_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct qeth_card *card = dev_get_drvdata(dev);

<<<<<<< HEAD
	if (!card)
		return -EINVAL;
	if (card->info.hwtrap)
		return snprintf(buf, 5, "arm\n");
	else
		return snprintf(buf, 8, "disarm\n");
=======
	if (card->info.hwtrap)
		return sysfs_emit(buf, "arm\n");
	else
		return sysfs_emit(buf, "disarm\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t qeth_hw_trap_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct qeth_card *card = dev_get_drvdata(dev);
	int rc = 0;
<<<<<<< HEAD
	char *tmp, *curtoken;
	int state = 0;
	curtoken = (char *)buf;

	if (!card)
		return -EINVAL;

	mutex_lock(&card->conf_mutex);
	if (card->state == CARD_STATE_SOFTSETUP || card->state == CARD_STATE_UP)
		state = 1;
	tmp = strsep(&curtoken, "\n");

	if (!strcmp(tmp, "arm") && !card->info.hwtrap) {
=======
	int state = 0;

	mutex_lock(&card->conf_mutex);
	if (qeth_card_hw_is_reachable(card))
		state = 1;

	if (sysfs_streq(buf, "arm") && !card->info.hwtrap) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (state) {
			if (qeth_is_diagass_supported(card,
			    QETH_DIAGS_CMD_TRAP)) {
				rc = qeth_hw_trap(card, QETH_DIAGS_TRAP_ARM);
				if (!rc)
					card->info.hwtrap = 1;
			} else
				rc = -EINVAL;
		} else
			card->info.hwtrap = 1;
<<<<<<< HEAD
	} else if (!strcmp(tmp, "disarm") && card->info.hwtrap) {
=======
	} else if (sysfs_streq(buf, "disarm") && card->info.hwtrap) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (state) {
			rc = qeth_hw_trap(card, QETH_DIAGS_TRAP_DISARM);
			if (!rc)
				card->info.hwtrap = 0;
		} else
			card->info.hwtrap = 0;
<<<<<<< HEAD
	} else if (!strcmp(tmp, "trap") && state && card->info.hwtrap)
=======
	} else if (sysfs_streq(buf, "trap") && state && card->info.hwtrap)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rc = qeth_hw_trap(card, QETH_DIAGS_TRAP_CAPTURE);
	else
		rc = -EINVAL;

	mutex_unlock(&card->conf_mutex);
	return rc ? rc : count;
}

static DEVICE_ATTR(hw_trap, 0644, qeth_hw_trap_show,
		   qeth_hw_trap_store);

<<<<<<< HEAD
static ssize_t qeth_dev_blkt_show(char *buf, struct qeth_card *card, int value)
{

	if (!card)
		return -EINVAL;

	return sprintf(buf, "%i\n", value);
}

static ssize_t qeth_dev_blkt_store(struct qeth_card *card,
		const char *buf, size_t count, int *value, int max_value)
{
	char *tmp;
	int i, rc = 0;

	if (!card)
		return -EINVAL;

	mutex_lock(&card->conf_mutex);
	if ((card->state != CARD_STATE_DOWN) &&
	    (card->state != CARD_STATE_RECOVER)) {
		rc = -EPERM;
		goto out;
	}
	i = simple_strtoul(buf, &tmp, 10);
	if (i <= max_value)
		*value = i;
	else
		rc = -EINVAL;
out:
=======
static ssize_t qeth_dev_blkt_store(struct qeth_card *card,
		const char *buf, size_t count, int *value, int max_value)
{
	unsigned int input;
	int rc;

	rc = kstrtouint(buf, 10, &input);
	if (rc)
		return rc;

	if (input > max_value)
		return -EINVAL;

	mutex_lock(&card->conf_mutex);
	if (card->state != CARD_STATE_DOWN)
		rc = -EPERM;
	else
		*value = input;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_unlock(&card->conf_mutex);
	return rc ? rc : count;
}

static ssize_t qeth_dev_blkt_total_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct qeth_card *card = dev_get_drvdata(dev);

<<<<<<< HEAD
	return qeth_dev_blkt_show(buf, card, card->info.blkt.time_total);
=======
	return sysfs_emit(buf, "%i\n", card->info.blkt.time_total);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t qeth_dev_blkt_total_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct qeth_card *card = dev_get_drvdata(dev);

	return qeth_dev_blkt_store(card, buf, count,
				   &card->info.blkt.time_total, 5000);
}

<<<<<<< HEAD


=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static DEVICE_ATTR(total, 0644, qeth_dev_blkt_total_show,
		   qeth_dev_blkt_total_store);

static ssize_t qeth_dev_blkt_inter_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct qeth_card *card = dev_get_drvdata(dev);

<<<<<<< HEAD
	return qeth_dev_blkt_show(buf, card, card->info.blkt.inter_packet);
=======
	return sysfs_emit(buf, "%i\n", card->info.blkt.inter_packet);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t qeth_dev_blkt_inter_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct qeth_card *card = dev_get_drvdata(dev);

	return qeth_dev_blkt_store(card, buf, count,
				   &card->info.blkt.inter_packet, 1000);
}

static DEVICE_ATTR(inter, 0644, qeth_dev_blkt_inter_show,
		   qeth_dev_blkt_inter_store);

static ssize_t qeth_dev_blkt_inter_jumbo_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct qeth_card *card = dev_get_drvdata(dev);

<<<<<<< HEAD
	return qeth_dev_blkt_show(buf, card,
				  card->info.blkt.inter_packet_jumbo);
=======
	return sysfs_emit(buf, "%i\n", card->info.blkt.inter_packet_jumbo);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t qeth_dev_blkt_inter_jumbo_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct qeth_card *card = dev_get_drvdata(dev);

	return qeth_dev_blkt_store(card, buf, count,
				   &card->info.blkt.inter_packet_jumbo, 1000);
}

static DEVICE_ATTR(inter_jumbo, 0644, qeth_dev_blkt_inter_jumbo_show,
		   qeth_dev_blkt_inter_jumbo_store);

static struct attribute *qeth_blkt_device_attrs[] = {
	&dev_attr_total.attr,
	&dev_attr_inter.attr,
	&dev_attr_inter_jumbo.attr,
	NULL,
};

<<<<<<< HEAD
static struct attribute_group qeth_device_blkt_group = {
=======
static const struct attribute_group qeth_dev_blkt_group = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.name = "blkt",
	.attrs = qeth_blkt_device_attrs,
};

<<<<<<< HEAD
static struct attribute *qeth_device_attrs[] = {
	&dev_attr_state.attr,
	&dev_attr_chpid.attr,
	&dev_attr_if_name.attr,
	&dev_attr_card_type.attr,
=======
static struct attribute *qeth_dev_extended_attrs[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	&dev_attr_inbuf_size.attr,
	&dev_attr_portno.attr,
	&dev_attr_portname.attr,
	&dev_attr_priority_queueing.attr,
<<<<<<< HEAD
	&dev_attr_buffer_count.attr,
	&dev_attr_recover.attr,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	&dev_attr_performance_stats.attr,
	&dev_attr_layer2.attr,
	&dev_attr_isolation.attr,
	&dev_attr_hw_trap.attr,
<<<<<<< HEAD
	NULL,
};

static struct attribute_group qeth_device_attr_group = {
	.attrs = qeth_device_attrs,
};

static struct attribute *qeth_osn_device_attrs[] = {
=======
	&dev_attr_switch_attrs.attr,
	NULL,
};

static const struct attribute_group qeth_dev_extended_group = {
	.attrs = qeth_dev_extended_attrs,
};

static struct attribute *qeth_dev_attrs[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	&dev_attr_state.attr,
	&dev_attr_chpid.attr,
	&dev_attr_if_name.attr,
	&dev_attr_card_type.attr,
	&dev_attr_buffer_count.attr,
	&dev_attr_recover.attr,
	NULL,
};

<<<<<<< HEAD
static struct attribute_group qeth_osn_device_attr_group = {
	.attrs = qeth_osn_device_attrs,
};

int qeth_core_create_device_attributes(struct device *dev)
{
	int ret;
	ret = sysfs_create_group(&dev->kobj, &qeth_device_attr_group);
	if (ret)
		return ret;
	ret = sysfs_create_group(&dev->kobj, &qeth_device_blkt_group);
	if (ret)
		sysfs_remove_group(&dev->kobj, &qeth_device_attr_group);

	return 0;
}

void qeth_core_remove_device_attributes(struct device *dev)
{
	sysfs_remove_group(&dev->kobj, &qeth_device_attr_group);
	sysfs_remove_group(&dev->kobj, &qeth_device_blkt_group);
}

int qeth_core_create_osn_attributes(struct device *dev)
{
	return sysfs_create_group(&dev->kobj, &qeth_osn_device_attr_group);
}

void qeth_core_remove_osn_attributes(struct device *dev)
{
	sysfs_remove_group(&dev->kobj, &qeth_osn_device_attr_group);
	return;
}
=======
static const struct attribute_group qeth_dev_group = {
	.attrs = qeth_dev_attrs,
};

const struct attribute_group *qeth_dev_groups[] = {
	&qeth_dev_group,
	&qeth_dev_extended_group,
	&qeth_dev_blkt_group,
	NULL,
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
