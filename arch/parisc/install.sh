#!/bin/sh
#
<<<<<<< HEAD
# arch/parisc/install.sh, derived from arch/i386/boot/install.sh
#
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 1995 by Linus Torvalds
#
# Adapted from code in arch/i386/boot/Makefile by H. Peter Anvin
#
# "make install" script for i386 architecture
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

# Default install

if [ -f $4/vmlinux ]; then
	mv $4/vmlinux $4/vmlinux.old
fi

if [ -f $4/System.map ]; then
	mv $4/System.map $4/System.old
fi

cat $2 > $4/vmlinux
cp $3 $4/System.map
=======

if [ "$(basename $2)" = "vmlinuz" ]; then
# Compressed install
  echo "Installing compressed kernel"
  base=vmlinuz
else
# Normal install
  echo "Installing normal kernel"
  base=vmlinux
fi

if [ -f $4/$base-$1 ]; then
  mv $4/$base-$1 $4/$base-$1.old
fi
cat $2 > $4/$base-$1

# Install system map file
if [ -f $4/System.map-$1 ]; then
  mv $4/System.map-$1 $4/System.map-$1.old
fi
cp $3 $4/System.map-$1

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
