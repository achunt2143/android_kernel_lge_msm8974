#!/bin/sh
#
# This is a wrapper for xz to compress the kernel image using appropriate
# compression options depending on the architecture.
#
# Author: Lasse Collin <lasse.collin@tukaani.org>
#
# This file has been put into the public domain.
# You can do whatever you want with this file.
#

BCJ=
LZMA2OPTS=

case $SRCARCH in
	x86)            BCJ=--x86 ;;
	powerpc)        BCJ=--powerpc ;;
<<<<<<< HEAD
	ia64)           BCJ=--ia64; LZMA2OPTS=pb=4 ;;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	arm)            BCJ=--arm ;;
	sparc)          BCJ=--sparc ;;
esac

<<<<<<< HEAD
exec xz --check=crc32 $BCJ --lzma2=$LZMA2OPTS,dict=32MiB
=======
exec $XZ --check=crc32 $BCJ --lzma2=$LZMA2OPTS,dict=32MiB
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
