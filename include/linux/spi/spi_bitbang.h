<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef	__SPI_BITBANG_H
#define	__SPI_BITBANG_H

#include <linux/workqueue.h>

struct spi_bitbang {
<<<<<<< HEAD
	struct workqueue_struct	*workqueue;
	struct work_struct	work;

	spinlock_t		lock;
	struct list_head	queue;
	u8			busy;
	u8			use_dma;
	u8			flags;		/* extra spi->mode support */

	struct spi_master	*master;
=======
	struct mutex		lock;
	u8			busy;
	u8			use_dma;
	u16			flags;		/* extra spi->mode support */

	struct spi_controller	*ctlr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* setup_transfer() changes clock and/or wordsize to match settings
	 * for this transfer; zeroes restore defaults from spi_device.
	 */
	int	(*setup_transfer)(struct spi_device *spi,
			struct spi_transfer *t);

	void	(*chipselect)(struct spi_device *spi, int is_on);
#define	BITBANG_CS_ACTIVE	1	/* normally nCS, active low */
#define	BITBANG_CS_INACTIVE	0

	/* txrx_bufs() may handle dma mapping for transfers that don't
	 * already have one (transfer.{tx,rx}_dma is zero), or use PIO
	 */
	int	(*txrx_bufs)(struct spi_device *spi, struct spi_transfer *t);

	/* txrx_word[SPI_MODE_*]() just looks like a shift register */
	u32	(*txrx_word[4])(struct spi_device *spi,
			unsigned nsecs,
<<<<<<< HEAD
			u32 word, u8 bits);
=======
			u32 word, u8 bits, unsigned flags);
	int	(*set_line_direction)(struct spi_device *spi, bool output);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* you can call these default bitbang->master methods from your custom
 * methods, if you like.
 */
extern int spi_bitbang_setup(struct spi_device *spi);
extern void spi_bitbang_cleanup(struct spi_device *spi);
<<<<<<< HEAD
extern int spi_bitbang_transfer(struct spi_device *spi, struct spi_message *m);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern int spi_bitbang_setup_transfer(struct spi_device *spi,
				      struct spi_transfer *t);

/* start or stop queue processing */
extern int spi_bitbang_start(struct spi_bitbang *spi);
<<<<<<< HEAD
extern int spi_bitbang_stop(struct spi_bitbang *spi);
=======
extern int spi_bitbang_init(struct spi_bitbang *spi);
extern void spi_bitbang_stop(struct spi_bitbang *spi);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif	/* __SPI_BITBANG_H */
