<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * drivers/pcmcia/sa1100_jornada720.c
 *
 * Jornada720 PCMCIA specific routines
 *
 */
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/init.h>

#include <mach/hardware.h>
#include <asm/hardware/sa1111.h>
=======
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/gpio/consumer.h>
#include <linux/init.h>
#include <linux/io.h>

#include <mach/hardware.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/mach-types.h>

#include "sa1111_generic.h"

<<<<<<< HEAD
/* Does SOCKET1_3V actually do anything? */
#define SOCKET0_POWER	GPIO_GPIO0
#define SOCKET0_3V	GPIO_GPIO2
#define SOCKET1_POWER	(GPIO_GPIO1 | GPIO_GPIO3)
#define SOCKET1_3V	GPIO_GPIO3
=======
/*
 * Socket 0 power: GPIO A0
 * Socket 0 3V: GPIO A2
 * Socket 1 power: GPIO A1 & GPIO A3
 * Socket 1 3V: GPIO A3
 * Does Socket 1 3V actually do anything?
 */
enum {
	J720_GPIO_PWR,
	J720_GPIO_3V,
	J720_GPIO_MAX,
};
struct jornada720_data {
	struct gpio_desc *gpio[J720_GPIO_MAX];
};

static int jornada720_pcmcia_hw_init(struct soc_pcmcia_socket *skt)
{
	struct device *dev = skt->socket.dev.parent;
	struct jornada720_data *j;

	j = devm_kzalloc(dev, sizeof(*j), GFP_KERNEL);
	if (!j)
		return -ENOMEM;

	j->gpio[J720_GPIO_PWR] = devm_gpiod_get(dev, skt->nr ? "s1-power" :
						"s0-power", GPIOD_OUT_LOW);
	if (IS_ERR(j->gpio[J720_GPIO_PWR]))
		return PTR_ERR(j->gpio[J720_GPIO_PWR]);

	j->gpio[J720_GPIO_3V] = devm_gpiod_get(dev, skt->nr ? "s1-3v" :
					       "s0-3v", GPIOD_OUT_LOW);
	if (IS_ERR(j->gpio[J720_GPIO_3V]))
		return PTR_ERR(j->gpio[J720_GPIO_3V]);

	skt->driver_data = j;

	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int
jornada720_pcmcia_configure_socket(struct soc_pcmcia_socket *skt, const socket_state_t *state)
{
<<<<<<< HEAD
	struct sa1111_pcmcia_socket *s = to_skt(skt);
	unsigned int pa_dwr_mask, pa_dwr_set;
=======
	struct jornada720_data *j = skt->driver_data;
	DECLARE_BITMAP(values, J720_GPIO_MAX) = { 0, };
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret;

	printk(KERN_INFO "%s(): config socket %d vcc %d vpp %d\n", __func__,
		skt->nr, state->Vcc, state->Vpp);

	switch (skt->nr) {
	case 0:
<<<<<<< HEAD
		pa_dwr_mask = SOCKET0_POWER | SOCKET0_3V;

		switch (state->Vcc) {
		default:
		case  0:
			pa_dwr_set = 0;
			break;
		case 33:
			pa_dwr_set = SOCKET0_POWER | SOCKET0_3V;
			break;
		case 50:
			pa_dwr_set = SOCKET0_POWER;
=======
		switch (state->Vcc) {
		default:
		case  0:
			__assign_bit(J720_GPIO_PWR, values, 0);
			__assign_bit(J720_GPIO_3V, values, 0);
			break;
		case 33:
			__assign_bit(J720_GPIO_PWR, values, 1);
			__assign_bit(J720_GPIO_3V, values, 1);
			break;
		case 50:
			__assign_bit(J720_GPIO_PWR, values, 1);
			__assign_bit(J720_GPIO_3V, values, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}
		break;

	case 1:
<<<<<<< HEAD
		pa_dwr_mask = SOCKET1_POWER;

		switch (state->Vcc) {
		default:
		case 0:
			pa_dwr_set = 0;
			break;
		case 33:
			pa_dwr_set = SOCKET1_POWER;
			break;
		case 50:
			pa_dwr_set = SOCKET1_POWER;
=======
		switch (state->Vcc) {
		default:
		case 0:
			__assign_bit(J720_GPIO_PWR, values, 0);
			__assign_bit(J720_GPIO_3V, values, 0);
			break;
		case 33:
		case 50:
			__assign_bit(J720_GPIO_PWR, values, 1);
			__assign_bit(J720_GPIO_3V, values, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}
		break;

	default:
		return -1;
	}

	if (state->Vpp != state->Vcc && state->Vpp != 0) {
		printk(KERN_ERR "%s(): slot cannot support VPP %u\n",
			__func__, state->Vpp);
		return -EPERM;
	}

	ret = sa1111_pcmcia_configure_socket(skt, state);
	if (ret == 0)
<<<<<<< HEAD
		sa1111_set_io(s->dev, pa_dwr_mask, pa_dwr_set);
=======
		ret = gpiod_set_array_value_cansleep(J720_GPIO_MAX, j->gpio,
						     NULL, values);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

static struct pcmcia_low_level jornada720_pcmcia_ops = {
	.owner			= THIS_MODULE,
<<<<<<< HEAD
=======
	.hw_init		= jornada720_pcmcia_hw_init,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.configure_socket	= jornada720_pcmcia_configure_socket,
	.first			= 0,
	.nr			= 2,
};

<<<<<<< HEAD
int __devinit pcmcia_jornada720_init(struct device *dev)
{
	int ret = -ENODEV;

	if (machine_is_jornada720()) {
		unsigned int pin = GPIO_A0 | GPIO_A1 | GPIO_A2 | GPIO_A3;

		GRER |= 0x00000002;

		/* Set GPIO_A<3:1> to be outputs for PCMCIA/CF power controller: */
		sa1111_set_io_dir(dev, pin, 0, 0);
		sa1111_set_io(dev, pin, 0);
		sa1111_set_sleep_io(dev, pin, 0);

		sa11xx_drv_pcmcia_ops(&jornada720_pcmcia_ops);
		ret = sa1111_pcmcia_add(dev, &jornada720_pcmcia_ops,
				sa11xx_drv_pcmcia_add_one);
	}

	return ret;
=======
int pcmcia_jornada720_init(struct sa1111_dev *sadev)
{
	/* Fixme: why messing around with SA11x0's GPIO1? */
	GRER |= 0x00000002;

	sa11xx_drv_pcmcia_ops(&jornada720_pcmcia_ops);
	return sa1111_pcmcia_add(sadev, &jornada720_pcmcia_ops,
				 sa11xx_drv_pcmcia_add_one);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
