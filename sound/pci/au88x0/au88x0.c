<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * ALSA driver for the Aureal Vortex family of soundprocessors.
 * Author: Manuel Jander (mjander@embedded.cl)
 *
 *   This driver is the result of the OpenVortex Project from Savannah
 * (savannah.nongnu.org/projects/openvortex). I would like to thank
 * the developers of OpenVortex, Jeff Muizelaar and Kester Maddock, from
 * whom i got plenty of help, and their codebase was invaluable.
 *   Thanks to the ALSA developers, they helped a lot working out
 * the ALSA part.
 *   Thanks also to Sourceforge for maintaining the old binary drivers,
<<<<<<< HEAD
 * and the forum, where developers could comunicate.
=======
 * and the forum, where developers could communicate.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Now at least i can play Legacy DOOM with MIDI music :-)
 */

#include "au88x0.h"
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/dma-mapping.h>
#include <sound/initval.h>

// module parameters (see "Module Parameters")
static int index[SNDRV_CARDS] = SNDRV_DEFAULT_IDX;
static char *id[SNDRV_CARDS] = SNDRV_DEFAULT_STR;
static bool enable[SNDRV_CARDS] = SNDRV_DEFAULT_ENABLE_PNP;
static int pcifix[SNDRV_CARDS] = {[0 ... (SNDRV_CARDS - 1)] = 255 };

module_param_array(index, int, NULL, 0444);
MODULE_PARM_DESC(index, "Index value for " CARD_NAME " soundcard.");
module_param_array(id, charp, NULL, 0444);
MODULE_PARM_DESC(id, "ID string for " CARD_NAME " soundcard.");
module_param_array(enable, bool, NULL, 0444);
MODULE_PARM_DESC(enable, "Enable " CARD_NAME " soundcard.");
module_param_array(pcifix, int, NULL, 0444);
MODULE_PARM_DESC(pcifix, "Enable VIA-workaround for " CARD_NAME " soundcard.");

MODULE_DESCRIPTION("Aureal vortex");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
MODULE_SUPPORTED_DEVICE("{{Aureal Semiconductor Inc., Aureal Vortex Sound Processor}}");

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_DEVICE_TABLE(pci, snd_vortex_ids);

static void vortex_fix_latency(struct pci_dev *vortex)
{
	int rc;
<<<<<<< HEAD
	if (!(rc = pci_write_config_byte(vortex, 0x40, 0xff))) {
			printk(KERN_INFO CARD_NAME
			       ": vortex latency is 0xff\n");
	} else {
		printk(KERN_WARNING CARD_NAME
				": could not set vortex latency: pci error 0x%x\n", rc);
=======
	rc = pci_write_config_byte(vortex, 0x40, 0xff);
	if (!rc) {
		dev_info(&vortex->dev, "vortex latency is 0xff\n");
	} else {
		dev_warn(&vortex->dev,
			 "could not set vortex latency: pci error 0x%x\n", rc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static void vortex_fix_agp_bridge(struct pci_dev *via)
{
	int rc;
	u8 value;

	/*
	 * only set the bit (Extend PCI#2 Internal Master for
	 * Efficient Handling of Dummy Requests) if the can
	 * read the config and it is not already set
	 */

<<<<<<< HEAD
	if (!(rc = pci_read_config_byte(via, 0x42, &value))
			&& ((value & 0x10)
				|| !(rc = pci_write_config_byte(via, 0x42, value | 0x10)))) {
		printk(KERN_INFO CARD_NAME
				": bridge config is 0x%x\n", value | 0x10);
	} else {
		printk(KERN_WARNING CARD_NAME
				": could not set vortex latency: pci error 0x%x\n", rc);
	}
}

static void __devinit snd_vortex_workaround(struct pci_dev *vortex, int fix)
=======
	rc = pci_read_config_byte(via, 0x42, &value);
	if (!rc) {
		if (!(value & 0x10))
			rc = pci_write_config_byte(via, 0x42, value | 0x10);
	}
	if (!rc) {
		dev_info(&via->dev, "bridge config is 0x%x\n", value | 0x10);
	} else {
		dev_warn(&via->dev,
			 "could not set vortex latency: pci error 0x%x\n", rc);
	}
}

static void snd_vortex_workaround(struct pci_dev *vortex, int fix)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct pci_dev *via = NULL;

	/* autodetect if workarounds are required */
	if (fix == 255) {
		/* VIA KT133 */
		via = pci_get_device(PCI_VENDOR_ID_VIA,
			PCI_DEVICE_ID_VIA_8365_1, NULL);
		/* VIA Apollo */
		if (via == NULL) {
			via = pci_get_device(PCI_VENDOR_ID_VIA,
				PCI_DEVICE_ID_VIA_82C598_1, NULL);
			/* AMD Irongate */
			if (via == NULL)
				via = pci_get_device(PCI_VENDOR_ID_AMD,
					PCI_DEVICE_ID_AMD_FE_GATE_7007, NULL);
		}
		if (via) {
<<<<<<< HEAD
			printk(KERN_INFO CARD_NAME ": Activating latency workaround...\n");
=======
			dev_info(&vortex->dev,
				 "Activating latency workaround...\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			vortex_fix_latency(vortex);
			vortex_fix_agp_bridge(via);
		}
	} else {
		if (fix & 0x1)
			vortex_fix_latency(vortex);
<<<<<<< HEAD
		if ((fix & 0x2) && (via = pci_get_device(PCI_VENDOR_ID_VIA,
				PCI_DEVICE_ID_VIA_8365_1, NULL)))
			vortex_fix_agp_bridge(via);
		if ((fix & 0x4) && (via = pci_get_device(PCI_VENDOR_ID_VIA,
				PCI_DEVICE_ID_VIA_82C598_1, NULL)))
			vortex_fix_agp_bridge(via);
		if ((fix & 0x8) && (via = pci_get_device(PCI_VENDOR_ID_AMD,
				PCI_DEVICE_ID_AMD_FE_GATE_7007, NULL)))
=======
		if (fix & 0x2)
			via = pci_get_device(PCI_VENDOR_ID_VIA,
					     PCI_DEVICE_ID_VIA_8365_1, NULL);
		else if (fix & 0x4)
			via = pci_get_device(PCI_VENDOR_ID_VIA,
					     PCI_DEVICE_ID_VIA_82C598_1, NULL);
		else if (fix & 0x8)
			via = pci_get_device(PCI_VENDOR_ID_AMD,
					     PCI_DEVICE_ID_AMD_FE_GATE_7007, NULL);
		if (via)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			vortex_fix_agp_bridge(via);
	}
	pci_dev_put(via);
}

// component-destructor
// (see "Management of Cards and Components")
<<<<<<< HEAD
static int snd_vortex_dev_free(struct snd_device *device)
{
	vortex_t *vortex = device->device_data;

	vortex_gameport_unregister(vortex);
	vortex_core_shutdown(vortex);
	// Take down PCI interface.
	free_irq(vortex->irq, vortex);
	iounmap(vortex->mmio);
	pci_release_regions(vortex->pci_dev);
	pci_disable_device(vortex->pci_dev);
	kfree(vortex);

	return 0;
=======
static void snd_vortex_free(struct snd_card *card)
{
	vortex_t *vortex = card->private_data;

	vortex_gameport_unregister(vortex);
	vortex_core_shutdown(vortex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

// chip-specific constructor
// (see "Management of Cards and Components")
<<<<<<< HEAD
static int __devinit
snd_vortex_create(struct snd_card *card, struct pci_dev *pci, vortex_t ** rchip)
{
	vortex_t *chip;
	int err;
	static struct snd_device_ops ops = {
		.dev_free = snd_vortex_dev_free,
	};

	*rchip = NULL;

	// check PCI availability (DMA).
	if ((err = pci_enable_device(pci)) < 0)
		return err;
	if (pci_set_dma_mask(pci, DMA_BIT_MASK(32)) < 0 ||
	    pci_set_consistent_dma_mask(pci, DMA_BIT_MASK(32)) < 0) {
		printk(KERN_ERR "error to set DMA mask\n");
		pci_disable_device(pci);
		return -ENXIO;
	}

	chip = kzalloc(sizeof(*chip), GFP_KERNEL);
	if (chip == NULL) {
		pci_disable_device(pci);
		return -ENOMEM;
	}

=======
static int
snd_vortex_create(struct snd_card *card, struct pci_dev *pci)
{
	vortex_t *chip = card->private_data;
	int err;

	// check PCI availability (DMA).
	err = pcim_enable_device(pci);
	if (err < 0)
		return err;
	if (dma_set_mask_and_coherent(&pci->dev, DMA_BIT_MASK(32))) {
		dev_err(card->dev, "error to set DMA mask\n");
		return -ENXIO;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	chip->card = card;

	// initialize the stuff
	chip->pci_dev = pci;
<<<<<<< HEAD
	chip->io = pci_resource_start(pci, 0);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	chip->vendor = pci->vendor;
	chip->device = pci->device;
	chip->card = card;
	chip->irq = -1;

	// (1) PCI resource allocation
	// Get MMIO area
	//
<<<<<<< HEAD
	if ((err = pci_request_regions(pci, CARD_NAME_SHORT)) != 0)
		goto regions_out;

	chip->mmio = pci_ioremap_bar(pci, 0);
	if (!chip->mmio) {
		printk(KERN_ERR "MMIO area remap failed.\n");
		err = -ENOMEM;
		goto ioremap_out;
	}
=======
	err = pcim_iomap_regions(pci, 1 << 0, CARD_NAME_SHORT);
	if (err)
		return err;

	chip->io = pci_resource_start(pci, 0);
	chip->mmio = pcim_iomap_table(pci)[0];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Init audio core.
	 * This must be done before we do request_irq otherwise we can get spurious
	 * interrupts that we do not handle properly and make a mess of things */
<<<<<<< HEAD
	if ((err = vortex_core_init(chip)) != 0) {
		printk(KERN_ERR "hw core init failed\n");
		goto core_out;
	}

	if ((err = request_irq(pci->irq, vortex_interrupt,
			       IRQF_SHARED, KBUILD_MODNAME,
	                       chip)) != 0) {
		printk(KERN_ERR "cannot grab irq\n");
		goto irq_out;
	}
	chip->irq = pci->irq;

	pci_set_master(pci);
	// End of PCI setup.

	// Register alsa root device.
	if ((err = snd_device_new(card, SNDRV_DEV_LOWLEVEL, chip, &ops)) < 0) {
		goto alloc_out;
	}

	snd_card_set_dev(card, &pci->dev);

	*rchip = chip;

	return 0;

      alloc_out:
	free_irq(chip->irq, chip);
      irq_out:
	vortex_core_shutdown(chip);
      core_out:
	iounmap(chip->mmio);
      ioremap_out:
	pci_release_regions(chip->pci_dev);
      regions_out:
	pci_disable_device(chip->pci_dev);
	//FIXME: this not the right place to unregister the gameport
	vortex_gameport_unregister(chip);
	kfree(chip);
	return err;
}

// constructor -- see "Constructor" sub-section
static int __devinit
snd_vortex_probe(struct pci_dev *pci, const struct pci_device_id *pci_id)
=======
	err = vortex_core_init(chip);
	if (err) {
		dev_err(card->dev, "hw core init failed\n");
		return err;
	}

	err = devm_request_irq(&pci->dev, pci->irq, vortex_interrupt,
			       IRQF_SHARED, KBUILD_MODNAME, chip);
	if (err) {
		dev_err(card->dev, "cannot grab irq\n");
		return err;
	}
	chip->irq = pci->irq;
	card->sync_irq = chip->irq;
	card->private_free = snd_vortex_free;

	pci_set_master(pci);
	// End of PCI setup.
	return 0;
}

// constructor -- see "Constructor" sub-section
static int
__snd_vortex_probe(struct pci_dev *pci, const struct pci_device_id *pci_id)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	static int dev;
	struct snd_card *card;
	vortex_t *chip;
	int err;

	// (1)
	if (dev >= SNDRV_CARDS)
		return -ENODEV;
	if (!enable[dev]) {
		dev++;
		return -ENOENT;
	}
	// (2)
<<<<<<< HEAD
	err = snd_card_create(index[dev], id[dev], THIS_MODULE, 0, &card);
	if (err < 0)
		return err;

	// (3)
	if ((err = snd_vortex_create(card, pci, &chip)) < 0) {
		snd_card_free(card);
		return err;
	}
=======
	err = snd_devm_card_new(&pci->dev, index[dev], id[dev], THIS_MODULE,
				sizeof(*chip), &card);
	if (err < 0)
		return err;
	chip = card->private_data;

	// (3)
	err = snd_vortex_create(card, pci);
	if (err < 0)
		return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	snd_vortex_workaround(pci, pcifix[dev]);

	// Card details needed in snd_vortex_midi
	strcpy(card->driver, CARD_NAME_SHORT);
	sprintf(card->shortname, "Aureal Vortex %s", CARD_NAME_SHORT);
	sprintf(card->longname, "%s at 0x%lx irq %i",
		card->shortname, chip->io, chip->irq);

	// (4) Alloc components.
	err = snd_vortex_mixer(chip);
<<<<<<< HEAD
	if (err < 0) {
		snd_card_free(card);
		return err;
	}
	// ADB pcm.
	err = snd_vortex_new_pcm(chip, VORTEX_PCM_ADB, NR_PCM);
	if (err < 0) {
		snd_card_free(card);
		return err;
	}
#ifndef CHIP_AU8820
	// ADB SPDIF
	if ((err = snd_vortex_new_pcm(chip, VORTEX_PCM_SPDIF, 1)) < 0) {
		snd_card_free(card);
		return err;
	}
	// A3D
	if ((err = snd_vortex_new_pcm(chip, VORTEX_PCM_A3D, NR_A3D)) < 0) {
		snd_card_free(card);
		return err;
	}
=======
	if (err < 0)
		return err;
	// ADB pcm.
	err = snd_vortex_new_pcm(chip, VORTEX_PCM_ADB, NR_PCM);
	if (err < 0)
		return err;
#ifndef CHIP_AU8820
	// ADB SPDIF
	err = snd_vortex_new_pcm(chip, VORTEX_PCM_SPDIF, 1);
	if (err < 0)
		return err;
	// A3D
	err = snd_vortex_new_pcm(chip, VORTEX_PCM_A3D, NR_A3D);
	if (err < 0)
		return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
	/*
	   // ADB I2S
	   if ((err = snd_vortex_new_pcm(chip, VORTEX_PCM_I2S, 1)) < 0) {
<<<<<<< HEAD
	   snd_card_free(card);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	   return err;
	   }
	 */
#ifndef CHIP_AU8810
	// WT pcm.
<<<<<<< HEAD
	if ((err = snd_vortex_new_pcm(chip, VORTEX_PCM_WT, NR_WT)) < 0) {
		snd_card_free(card);
		return err;
	}
#endif
	if ((err = snd_vortex_midi(chip)) < 0) {
		snd_card_free(card);
		return err;
	}
=======
	err = snd_vortex_new_pcm(chip, VORTEX_PCM_WT, NR_WT);
	if (err < 0)
		return err;
#endif
	err = snd_vortex_midi(chip);
	if (err < 0)
		return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	vortex_gameport_register(chip);

#if 0
	if (snd_seq_device_new(card, 1, SNDRV_SEQ_DEV_ID_VORTEX_SYNTH,
			       sizeof(snd_vortex_synth_arg_t), &wave) < 0
	    || wave == NULL) {
<<<<<<< HEAD
		snd_printk(KERN_ERR "Can't initialize Aureal wavetable synth\n");
=======
		dev_err(card->dev, "Can't initialize Aureal wavetable synth\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		snd_vortex_synth_arg_t *arg;

		arg = SNDRV_SEQ_DEVICE_ARGPTR(wave);
		strcpy(wave->name, "Aureal Synth");
		arg->hwptr = vortex;
		arg->index = 1;
		arg->seq_ports = seq_ports[dev];
		arg->max_voices = max_synth_voices[dev];
	}
#endif

	// (5)
<<<<<<< HEAD
	if ((err = pci_read_config_word(pci, PCI_DEVICE_ID,
				  &(chip->device))) < 0) {
		snd_card_free(card);
		return err;
	}	
	if ((err = pci_read_config_word(pci, PCI_VENDOR_ID,
				  &(chip->vendor))) < 0) {
		snd_card_free(card);
		return err;
	}
	chip->rev = pci->revision;
#ifdef CHIP_AU8830
	if ((chip->rev) != 0xfe && (chip->rev) != 0xfa) {
		printk(KERN_ALERT
		       "vortex: The revision (%x) of your card has not been seen before.\n",
		       chip->rev);
		printk(KERN_ALERT
		       "vortex: Please email the results of 'lspci -vv' to openvortex-dev@nongnu.org.\n");
		snd_card_free(card);
		err = -ENODEV;
		return err;
=======
	err = pci_read_config_word(pci, PCI_DEVICE_ID, &chip->device);
	if (err < 0)
		return err;
	err = pci_read_config_word(pci, PCI_VENDOR_ID, &chip->vendor);
	if (err < 0)
		return err;
	chip->rev = pci->revision;
#ifdef CHIP_AU8830
	if ((chip->rev) != 0xfe && (chip->rev) != 0xfa) {
		dev_alert(card->dev,
			  "The revision (%x) of your card has not been seen before.\n",
		       chip->rev);
		dev_alert(card->dev,
			  "Please email the results of 'lspci -vv' to openvortex-dev@nongnu.org.\n");
		return -ENODEV;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
#endif

	// (6)
<<<<<<< HEAD
	if ((err = snd_card_register(card)) < 0) {
		snd_card_free(card);
		return err;
	}
=======
	err = snd_card_register(card);
	if (err < 0)
		return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	// (7)
	pci_set_drvdata(pci, card);
	dev++;
	vortex_connect_default(chip, 1);
	vortex_enable_int(chip);
	return 0;
}

<<<<<<< HEAD
// destructor -- see "Destructor" sub-section
static void __devexit snd_vortex_remove(struct pci_dev *pci)
{
	snd_card_free(pci_get_drvdata(pci));
	pci_set_drvdata(pci, NULL);
}

// pci_driver definition
static struct pci_driver driver = {
	.name = KBUILD_MODNAME,
	.id_table = snd_vortex_ids,
	.probe = snd_vortex_probe,
	.remove = __devexit_p(snd_vortex_remove),
};

// initialization of the module
static int __init alsa_card_vortex_init(void)
{
	return pci_register_driver(&driver);
}

// clean up the module
static void __exit alsa_card_vortex_exit(void)
{
	pci_unregister_driver(&driver);
}

module_init(alsa_card_vortex_init)
module_exit(alsa_card_vortex_exit)
=======
static int
snd_vortex_probe(struct pci_dev *pci, const struct pci_device_id *pci_id)
{
	return snd_card_free_on_error(&pci->dev, __snd_vortex_probe(pci, pci_id));
}

// pci_driver definition
static struct pci_driver vortex_driver = {
	.name = KBUILD_MODNAME,
	.id_table = snd_vortex_ids,
	.probe = snd_vortex_probe,
};

module_pci_driver(vortex_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
