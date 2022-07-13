#!/bin/sh
<<<<<<< HEAD

echo "int foo(void) { char X[200]; return 3; }" | $* -S -x c -c -O0 -fstack-protector - -o - 2> /dev/null | grep -q "%gs"
if [ "$?" -eq "0" ] ; then
	echo y
else
	echo n
fi
=======
# SPDX-License-Identifier: GPL-2.0

# This requires GCC 8.1 or better.  Specifically, we require
# -mstack-protector-guard-reg, added by
# https://gcc.gnu.org/bugzilla/show_bug.cgi?id=81708

echo "int foo(void) { char X[200]; return 3; }" | $* -S -x c -c -m32 -O0 -fstack-protector -mstack-protector-guard-reg=fs -mstack-protector-guard-symbol=__stack_chk_guard - -o - 2> /dev/null | grep -q "%fs"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
