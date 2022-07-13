<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only

#include "sas_internal.h"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/export.h>
#include <scsi/sas.h>
#include <scsi/libsas.h>

/* fill task_status_struct based on SSP response frame */
void sas_ssp_task_response(struct device *dev, struct sas_task *task,
			   struct ssp_response_iu *iu)
{
	struct task_status_struct *tstat = &task->task_status;

	tstat->resp = SAS_TASK_COMPLETE;

<<<<<<< HEAD
	if (iu->datapres == 0)
		tstat->stat = iu->status;
	else if (iu->datapres == 1)
		tstat->stat = iu->resp_data[3];
	else if (iu->datapres == 2) {
		tstat->stat = SAM_STAT_CHECK_CONDITION;
=======
	switch (iu->datapres) {
	case SAS_DATAPRES_NO_DATA:
		tstat->stat = iu->status;
		break;
	case SAS_DATAPRES_RESPONSE_DATA:
		tstat->stat = iu->resp_data[3];
		break;
	case SAS_DATAPRES_SENSE_DATA:
		tstat->stat = SAS_SAM_STAT_CHECK_CONDITION;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		tstat->buf_valid_size =
			min_t(int, SAS_STATUS_BUF_SIZE,
			      be32_to_cpu(iu->sense_data_len));
		memcpy(tstat->buf, iu->sense_data, tstat->buf_valid_size);

		if (iu->status != SAM_STAT_CHECK_CONDITION)
<<<<<<< HEAD
			dev_printk(KERN_WARNING, dev,
				   "dev %llx sent sense data, but "
				   "stat(%x) is not CHECK CONDITION\n",
				   SAS_ADDR(task->dev->sas_addr),
				   iu->status);
	}
	else
		/* when datapres contains corrupt/unknown value... */
		tstat->stat = SAM_STAT_CHECK_CONDITION;
=======
			dev_warn(dev, "dev %016llx sent sense data, but stat(0x%x) is not CHECK CONDITION\n",
				 SAS_ADDR(task->dev->sas_addr), iu->status);
		break;
	default:
		/* when datapres contains corrupt/unknown value... */
		tstat->stat = SAS_SAM_STAT_CHECK_CONDITION;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(sas_ssp_task_response);

