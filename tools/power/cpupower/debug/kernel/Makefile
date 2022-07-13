<<<<<<< HEAD
obj-m	:=

KDIR	:= /lib/modules/$(shell uname -r)/build
PWD		:= $(shell pwd)
=======
# SPDX-License-Identifier: GPL-2.0
obj-m	:=

KDIR	:= /lib/modules/$(shell uname -r)/build
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
KMISC   := /lib/modules/$(shell uname -r)/cpufrequtils/

ifeq ("$(CONFIG_X86_TSC)", "y")
  obj-m	 += cpufreq-test_tsc.o
endif

default:
<<<<<<< HEAD
	$(MAKE) -C $(KDIR) M=$(PWD)

clean:
	- rm -rf *.o *.ko .tmp-versions .*.cmd .*.mod.* *.mod.c
	- rm -rf .tmp_versions* Module.symvers modules.order
=======
	$(MAKE) -C $(KDIR) M=$(CURDIR)

clean:
	- rm -rf *.o *.ko .*.cmd .*.mod.* *.mod.c
	- rm -rf Module.symvers modules.order
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

install: default
	install -d $(KMISC)
	install -m 644 -c *.ko $(KMISC)
	/sbin/depmod -a

all:	default
