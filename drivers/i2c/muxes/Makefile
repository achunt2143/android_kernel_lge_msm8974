<<<<<<< HEAD
#
# Makefile for multiplexer I2C chip drivers.

obj-$(CONFIG_I2C_MUX_GPIO)	+= gpio-i2cmux.o
obj-$(CONFIG_I2C_MUX_PCA9541)	+= pca9541.o
obj-$(CONFIG_I2C_MUX_PCA954x)	+= pca954x.o
=======
# SPDX-License-Identifier: GPL-2.0
#
# Makefile for multiplexer I2C chip drivers.

obj-$(CONFIG_I2C_ARB_GPIO_CHALLENGE)	+= i2c-arb-gpio-challenge.o

obj-$(CONFIG_I2C_DEMUX_PINCTRL)		+= i2c-demux-pinctrl.o

obj-$(CONFIG_I2C_MUX_GPIO)	+= i2c-mux-gpio.o
obj-$(CONFIG_I2C_MUX_GPMUX)	+= i2c-mux-gpmux.o
obj-$(CONFIG_I2C_MUX_LTC4306)	+= i2c-mux-ltc4306.o
obj-$(CONFIG_I2C_MUX_MLXCPLD)	+= i2c-mux-mlxcpld.o
obj-$(CONFIG_I2C_MUX_PCA9541)	+= i2c-mux-pca9541.o
obj-$(CONFIG_I2C_MUX_PCA954x)	+= i2c-mux-pca954x.o
obj-$(CONFIG_I2C_MUX_PINCTRL)	+= i2c-mux-pinctrl.o
obj-$(CONFIG_I2C_MUX_REG)	+= i2c-mux-reg.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

ccflags-$(CONFIG_I2C_DEBUG_BUS) := -DDEBUG
