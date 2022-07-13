#!/bin/sh
<<<<<<< HEAD
#
# arch/s390x/boot/install.sh
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
=======
# SPDX-License-Identifier: GPL-2.0
#
# arch/s390x/boot/install.sh
#
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
# Copyright (C) 1995 by Linus Torvalds
#
# Adapted from code in arch/i386/boot/Makefile by H. Peter Anvin
#
# "make install" script for s390 architecture
#
# Arguments:
#   $1 - kernel version
#   $2 - kernel image file
#   $3 - kernel map file
#   $4 - default install path (blank if root directory)
<<<<<<< HEAD
#

# User may have a custom install script

if [ -x ~/bin/${INSTALLKERNEL} ]; then exec ~/bin/${INSTALLKERNEL} "$@"; fi
if [ -x /sbin/${INSTALLKERNEL} ]; then exec /sbin/${INSTALLKERNEL} "$@"; fi

# Default install - same as make zlilo

if [ -f $4/vmlinuz ]; then
	mv $4/vmlinuz $4/vmlinuz.old
fi

if [ -f $4/System.map ]; then
	mv $4/System.map $4/System.old
fi

cat $2 > $4/vmlinuz
cp $3 $4/System.map
=======

echo "Warning: '${INSTALLKERNEL}' command not available - additional " \
     "bootloader config required" >&2
if [ -f "$4/vmlinuz-$1" ]; then mv -- "$4/vmlinuz-$1" "$4/vmlinuz-$1.old"; fi
if [ -f "$4/System.map-$1" ]; then mv -- "$4/System.map-$1" "$4/System.map-$1.old"; fi

cat -- "$2" > "$4/vmlinuz-$1"
cp -- "$3" "$4/System.map-$1"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
