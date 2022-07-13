<<<<<<< HEAD
# Taken from perf makefile
uname_M := $(shell uname -m 2>/dev/null || echo not)
ARCH ?= $(shell echo $(uname_M) | sed -e s/i.86/i386/)
ifeq ($(ARCH),i386)
        ARCH := x86
endif
ifeq ($(ARCH),x86_64)
	ARCH := x86
endif


all:
ifeq ($(ARCH),x86)
	gcc breakpoint_test.c -o breakpoint_test
else
	echo "Not an x86 target, can't build breakpoints selftests"
endif

run_tests:
	./breakpoint_test

clean:
	rm -fr breakpoint_test
=======
# SPDX-License-Identifier: GPL-2.0
# Taken from perf makefile
uname_M := $(shell uname -m 2>/dev/null || echo not)
ARCH ?= $(shell echo $(uname_M) | sed -e s/i.86/x86/ -e s/x86_64/x86/)

TEST_GEN_PROGS := step_after_suspend_test

ifeq ($(ARCH),x86)
TEST_GEN_PROGS += breakpoint_test
endif
ifneq (,$(filter $(ARCH),aarch64 arm64))
TEST_GEN_PROGS += breakpoint_test_arm64
endif

include ../lib.mk

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
