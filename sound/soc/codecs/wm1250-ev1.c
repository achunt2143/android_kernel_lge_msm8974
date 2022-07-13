<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Driver for the 1250-EV1 audio I/O module
 *
 * Copyright 2011 Wolfson Microelectronics plc
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/i2c.h>
<<<<<<< HEAD
#include <linux/gpio.h>

#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <sound/wm1250-ev1.h>

static const char *wm1250_gpio_names[WM1250_EV1_NUM_GPIOS] = {
	"WM1250 CLK_ENA",
	"WM1250 CLK_SEL0",
	"WM1250 CLK_SEL1",
	"WM1250 OSR",
	"WM1250 MASTER",
};

struct wm1250_priv {
	struct gpio gpios[WM1250_EV1_NUM_GPIOS];
};

static int wm1250_ev1_set_bias_level(struct snd_soc_codec *codec,
				     enum snd_soc_bias_level level)
{
	struct wm1250_priv *wm1250 = dev_get_drvdata(codec->dev);
	int ena;

	if (wm1250)
		ena = wm1250->gpios[WM1250_EV1_GPIO_CLK_ENA].gpio;
	else
		ena = -1;
=======
#include <linux/gpio/consumer.h>

#include <sound/soc.h>
#include <sound/soc-dapm.h>

struct wm1250_priv {
	struct gpio_desc *clk_ena;
	struct gpio_desc *clk_sel0;
	struct gpio_desc *clk_sel1;
	struct gpio_desc *osr;
	struct gpio_desc *master;
};

static int wm1250_ev1_set_bias_level(struct snd_soc_component *component,
				     enum snd_soc_bias_level level)
{
	struct wm1250_priv *wm1250 = dev_get_drvdata(component->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (level) {
	case SND_SOC_BIAS_ON:
		break;

	case SND_SOC_BIAS_PREPARE:
		break;

	case SND_SOC_BIAS_STANDBY:
<<<<<<< HEAD
		if (ena >= 0)
			gpio_set_value_cansleep(ena, 1);
		break;

	case SND_SOC_BIAS_OFF:
		if (ena >= 0)
			gpio_set_value_cansleep(ena, 0);
		break;
	}

	codec->dapm.bias_level = level;

=======
		gpiod_set_value_cansleep(wm1250->clk_ena, 1);
		break;

	case SND_SOC_BIAS_OFF:
		gpiod_set_value_cansleep(wm1250->clk_ena, 0);
		break;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static const struct snd_soc_dapm_widget wm1250_ev1_dapm_widgets[] = {
SND_SOC_DAPM_ADC("ADC", "wm1250-ev1 Capture", SND_SOC_NOPM, 0, 0),
SND_SOC_DAPM_DAC("DAC", "wm1250-ev1 Playback", SND_SOC_NOPM, 0, 0),

SND_SOC_DAPM_INPUT("WM1250 Input"),
SND_SOC_DAPM_OUTPUT("WM1250 Output"),
};

static const struct snd_soc_dapm_route wm1250_ev1_dapm_routes[] = {
	{ "ADC", NULL, "WM1250 Input" },
	{ "WM1250 Output", NULL, "DAC" },
};

<<<<<<< HEAD
=======
static int wm1250_ev1_hw_params(struct snd_pcm_substream *substream,
				struct snd_pcm_hw_params *params,
				struct snd_soc_dai *dai)
{
	struct wm1250_priv *wm1250 = snd_soc_component_get_drvdata(dai->component);

	switch (params_rate(params)) {
	case 8000:
		gpiod_set_value(wm1250->clk_sel0, 1);
		gpiod_set_value(wm1250->clk_sel1, 1);
		break;
	case 16000:
		gpiod_set_value(wm1250->clk_sel0, 0);
		gpiod_set_value(wm1250->clk_sel1, 1);
		break;
	case 32000:
		gpiod_set_value(wm1250->clk_sel0, 1);
		gpiod_set_value(wm1250->clk_sel1, 0);
		break;
	case 64000:
		gpiod_set_value(wm1250->clk_sel0, 0);
		gpiod_set_value(wm1250->clk_sel1, 0);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static const struct snd_soc_dai_ops wm1250_ev1_ops = {
	.hw_params = wm1250_ev1_hw_params,
};

#define WM1250_EV1_RATES (SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_16000 |\
			  SNDRV_PCM_RATE_32000 | SNDRV_PCM_RATE_64000)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct snd_soc_dai_driver wm1250_ev1_dai = {
	.name = "wm1250-ev1",
	.playback = {
		.stream_name = "Playback",
		.channels_min = 1,
<<<<<<< HEAD
		.channels_max = 1,
		.rates = SNDRV_PCM_RATE_8000,
=======
		.channels_max = 2,
		.rates = WM1250_EV1_RATES,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.formats = SNDRV_PCM_FMTBIT_S16_LE,
	},
	.capture = {
		.stream_name = "Capture",
		.channels_min = 1,
<<<<<<< HEAD
		.channels_max = 1,
		.rates = SNDRV_PCM_RATE_8000,
		.formats = SNDRV_PCM_FMTBIT_S16_LE,
	},
};

static struct snd_soc_codec_driver soc_codec_dev_wm1250_ev1 = {
	.dapm_widgets = wm1250_ev1_dapm_widgets,
	.num_dapm_widgets = ARRAY_SIZE(wm1250_ev1_dapm_widgets),
	.dapm_routes = wm1250_ev1_dapm_routes,
	.num_dapm_routes = ARRAY_SIZE(wm1250_ev1_dapm_routes),

	.set_bias_level = wm1250_ev1_set_bias_level,
	.idle_bias_off = true,
};

static int __devinit wm1250_ev1_pdata(struct i2c_client *i2c)
{
	struct wm1250_ev1_pdata *pdata = dev_get_platdata(&i2c->dev);
	struct wm1250_priv *wm1250;
	int i, ret;
=======
		.channels_max = 2,
		.rates = WM1250_EV1_RATES,
		.formats = SNDRV_PCM_FMTBIT_S16_LE,
	},
	.ops = &wm1250_ev1_ops,
};

static const struct snd_soc_component_driver soc_component_dev_wm1250_ev1 = {
	.dapm_widgets		= wm1250_ev1_dapm_widgets,
	.num_dapm_widgets	= ARRAY_SIZE(wm1250_ev1_dapm_widgets),
	.dapm_routes		= wm1250_ev1_dapm_routes,
	.num_dapm_routes	= ARRAY_SIZE(wm1250_ev1_dapm_routes),
	.set_bias_level		= wm1250_ev1_set_bias_level,
	.use_pmdown_time	= 1,
	.endianness		= 1,
};

static int wm1250_ev1_pdata(struct i2c_client *i2c)
{
	struct wm1250_ev1_pdata *pdata = dev_get_platdata(&i2c->dev);
	struct wm1250_priv *wm1250;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!pdata)
		return 0;

	wm1250 = devm_kzalloc(&i2c->dev, sizeof(*wm1250), GFP_KERNEL);
<<<<<<< HEAD
	if (!wm1250) {
		dev_err(&i2c->dev, "Unable to allocate private data\n");
		ret = -ENOMEM;
		goto err;
	}

	for (i = 0; i < ARRAY_SIZE(wm1250->gpios); i++) {
		wm1250->gpios[i].gpio = pdata->gpios[i];
		wm1250->gpios[i].label = wm1250_gpio_names[i];
		wm1250->gpios[i].flags = GPIOF_OUT_INIT_LOW;
	}
	wm1250->gpios[WM1250_EV1_GPIO_CLK_SEL0].flags = GPIOF_OUT_INIT_HIGH;
	wm1250->gpios[WM1250_EV1_GPIO_CLK_SEL1].flags = GPIOF_OUT_INIT_HIGH;

	ret = gpio_request_array(wm1250->gpios, ARRAY_SIZE(wm1250->gpios));
	if (ret != 0) {
		dev_err(&i2c->dev, "Failed to get GPIOs: %d\n", ret);
		goto err;
	}

	dev_set_drvdata(&i2c->dev, wm1250);

	return ret;

err:
	return ret;
}

static void wm1250_ev1_free(struct i2c_client *i2c)
{
	struct wm1250_priv *wm1250 = dev_get_drvdata(&i2c->dev);

	if (wm1250)
		gpio_free_array(wm1250->gpios, ARRAY_SIZE(wm1250->gpios));
}

static int __devinit wm1250_ev1_probe(struct i2c_client *i2c,
				      const struct i2c_device_id *i2c_id)
=======
	if (!wm1250)
		return -ENOMEM;

	wm1250->clk_ena = devm_gpiod_get(&i2c->dev, "clk-ena", GPIOD_OUT_LOW);
	if (IS_ERR(wm1250->clk_ena))
		return dev_err_probe(&i2c->dev, PTR_ERR(wm1250->clk_ena),
				     "failed to get clock enable GPIO\n");

	wm1250->clk_sel0 = devm_gpiod_get(&i2c->dev, "clk-sel0", GPIOD_OUT_HIGH);
	if (IS_ERR(wm1250->clk_sel0))
		return dev_err_probe(&i2c->dev, PTR_ERR(wm1250->clk_sel0),
				     "failed to get clock sel0 GPIO\n");

	wm1250->clk_sel1 = devm_gpiod_get(&i2c->dev, "clk-sel1", GPIOD_OUT_HIGH);
	if (IS_ERR(wm1250->clk_sel1))
		return dev_err_probe(&i2c->dev, PTR_ERR(wm1250->clk_sel1),
				     "failed to get clock sel1 GPIO\n");

	wm1250->osr = devm_gpiod_get(&i2c->dev, "osr", GPIOD_OUT_LOW);
	if (IS_ERR(wm1250->osr))
		return dev_err_probe(&i2c->dev, PTR_ERR(wm1250->osr),
				     "failed to get OSR GPIO\n");

	wm1250->master = devm_gpiod_get(&i2c->dev, "master", GPIOD_OUT_LOW);
	if (IS_ERR(wm1250->master))
		return dev_err_probe(&i2c->dev, PTR_ERR(wm1250->master),
				     "failed to get MASTER GPIO\n");

	dev_set_drvdata(&i2c->dev, wm1250);

	return 0;
}

static int wm1250_ev1_probe(struct i2c_client *i2c)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int id, board, rev, ret;

	dev_set_drvdata(&i2c->dev, NULL);

	board = i2c_smbus_read_byte_data(i2c, 0);
	if (board < 0) {
		dev_err(&i2c->dev, "Failed to read ID: %d\n", board);
		return board;
	}

	id = (board & 0xfe) >> 2;
	rev = board & 0x3;

	if (id != 1) {
		dev_err(&i2c->dev, "Unknown board ID %d\n", id);
		return -ENODEV;
	}

	dev_info(&i2c->dev, "revision %d\n", rev + 1);

	ret = wm1250_ev1_pdata(i2c);
	if (ret != 0)
		return ret;

<<<<<<< HEAD
	ret = snd_soc_register_codec(&i2c->dev, &soc_codec_dev_wm1250_ev1,
				     &wm1250_ev1_dai, 1);
	if (ret != 0) {
		dev_err(&i2c->dev, "Failed to register CODEC: %d\n", ret);
		wm1250_ev1_free(i2c);
=======
	ret = devm_snd_soc_register_component(&i2c->dev, &soc_component_dev_wm1250_ev1,
				     &wm1250_ev1_dai, 1);
	if (ret != 0) {
		dev_err(&i2c->dev, "Failed to register CODEC: %d\n", ret);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return ret;
	}

	return 0;
}

<<<<<<< HEAD
static int __devexit wm1250_ev1_remove(struct i2c_client *i2c)
{
	snd_soc_unregister_codec(&i2c->dev);
	wm1250_ev1_free(i2c);

	return 0;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct i2c_device_id wm1250_ev1_i2c_id[] = {
	{ "wm1250-ev1", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, wm1250_ev1_i2c_id);

static struct i2c_driver wm1250_ev1_i2c_driver = {
	.driver = {
		.name = "wm1250-ev1",
<<<<<<< HEAD
		.owner = THIS_MODULE,
	},
	.probe =    wm1250_ev1_probe,
	.remove =   __devexit_p(wm1250_ev1_remove),
	.id_table = wm1250_ev1_i2c_id,
};

static int __init wm1250_ev1_modinit(void)
{
	int ret = 0;

	ret = i2c_add_driver(&wm1250_ev1_i2c_driver);
	if (ret != 0)
		pr_err("Failed to register WM1250-EV1 I2C driver: %d\n", ret);

	return ret;
}
module_init(wm1250_ev1_modinit);

static void __exit wm1250_ev1_exit(void)
{
	i2c_del_driver(&wm1250_ev1_i2c_driver);
}
module_exit(wm1250_ev1_exit);
=======
	},
	.probe =    wm1250_ev1_probe,
	.id_table = wm1250_ev1_i2c_id,
};

module_i2c_driver(wm1250_ev1_i2c_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_AUTHOR("Mark Brown <broonie@opensource.wolfsonmicro.com>");
MODULE_DESCRIPTION("WM1250-EV1 audio I/O module driver");
MODULE_LICENSE("GPL");
