#
# arch/sh/tools/Makefile
#
# Copyright (C) 2003  Paul Mundt
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Shamelessly cloned from ARM.
#

include/generated/machtypes.h: $(src)/gen-mach-types $(src)/mach-types
	@echo '  Generating $@'
	$(Q)mkdir -p $(dir $@)
<<<<<<< HEAD
	$(Q)LC_ALL=C $(AWK) -f $^ > $@ || { rm -f $@; /bin/false; }
=======
	$(Q)LC_ALL=C $(AWK) -f $^ > $@
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
