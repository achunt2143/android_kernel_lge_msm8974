<<<<<<< HEAD
#ifndef __SPI_SH_MSIOF_H__
#define __SPI_SH_MSIOF_H__

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __SPI_SH_MSIOF_H__
#define __SPI_SH_MSIOF_H__

enum {
	MSIOF_SPI_HOST,
	MSIOF_SPI_TARGET,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct sh_msiof_spi_info {
	int tx_fifo_override;
	int rx_fifo_override;
	u16 num_chipselect;
<<<<<<< HEAD
=======
	int mode;
	unsigned int dma_tx_id;
	unsigned int dma_rx_id;
	u32 dtdl;
	u32 syncdl;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#endif /* __SPI_SH_MSIOF_H__ */
