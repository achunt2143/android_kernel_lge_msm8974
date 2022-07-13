#!/bin/sh
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 1995 by Linus Torvalds
#
# Blatantly stolen from in arch/i386/boot/install.sh by Dave Hansen 
#
# "make install" script for ppc64 architecture
#
# Arguments:
#   $1 - kernel version
#   $2 - kernel image file
#   $3 - kernel map file
#   $4 - default install path (blank if root directory)
<<<<<<< HEAD
#   $5 and more - kernel boot files; zImage*, uImage, cuImage.*, etc.
#

# Bail with error code if anything goes wrong
set -e

# User may have a custom install script

if [ -x ~/bin/${INSTALLKERNEL} ]; then exec ~/bin/${INSTALLKERNEL} "$@"; fi
if [ -x /sbin/${INSTALLKERNEL} ]; then exec /sbin/${INSTALLKERNEL} "$@"; fi

# Default install

# this should work for both the pSeries zImage and the iSeries vmlinux.sm
image_name=`basename $2`

if [ -f $4/$image_name ]; then
	mv $4/$image_name $4/$image_name.old
fi

if [ -f $4/System.map ]; then
	mv $4/System.map $4/System.old
fi

cat $2 > $4/$image_name
cp $3 $4/System.map

# Copy all the bootable image files
path=$4
shift 4
while [ $# -ne 0 ]; do
	image_name=`basename $1`
	if [ -f $path/$image_name ]; then
		mv $path/$image_name $path/$image_name.old
	fi
	cat $1 > $path/$image_name
	shift
done;
=======

set -e

# this should work for both the pSeries zImage and the iSeries vmlinux.sm
image_name=`basename $2`


echo "Warning: '${INSTALLKERNEL}' command not available... Copying" \
     "directly to $4/$image_name-$1" >&2

if [ -f $4/$image_name-$1 ]; then
	mv $4/$image_name-$1 $4/$image_name-$1.old
fi

if [ -f $4/System.map-$1 ]; then
	mv $4/System.map-$1 $4/System-$1.old
fi

cat $2 > $4/$image_name-$1
cp $3 $4/System.map-$1
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
