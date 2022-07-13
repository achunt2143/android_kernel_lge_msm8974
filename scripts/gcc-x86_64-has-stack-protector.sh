#!/bin/sh
<<<<<<< HEAD

echo "int foo(void) { char X[200]; return 3; }" | $* -S -x c -c -O0 -mcmodel=kernel -fstack-protector - -o - 2> /dev/null | grep -q "%gs"
if [ "$?" -eq "0" ] ; then
	echo y
else
	echo n
fi
=======
# SPDX-License-Identifier: GPL-2.0

echo "int foo(void) { char X[200]; return 3; }" | $* -S -x c -c -m64 -O0 -mcmodel=kernel -fno-PIE -fstack-protector - -o - 2> /dev/null | grep -q "%gs"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
