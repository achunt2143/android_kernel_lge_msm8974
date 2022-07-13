<<<<<<< HEAD
#include <variant/core.h>
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 *  linux/arch/xtensa/boot/boot-elf/boot.lds.S
 *
 *  Copyright (C) 2008 - 2013 by Tensilica Inc.
 *
 *  Chris Zankel <chris@zankel.net>
 *  Marc Gauthier <marc@tensilica.com
 *  Pete Delaney <piet@tensilica.com>
 */

#include <asm/vectors.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
OUTPUT_ARCH(xtensa)
ENTRY(_ResetVector)

SECTIONS
{
<<<<<<< HEAD
	.start 0xD0000000 : { *(.start) }

	.text 0xD0000000:
	{
		__reloc_start = . ;
		_text_start = . ;
		*(.literal .text.literal .text)
		_text_end = . ;
	}

	.rodata ALIGN(0x04):
	{
		*(.rodata)
		*(.rodata1)
	}

	.data ALIGN(0x04):
	{
		*(.data)
		*(.data1)
		*(.sdata)
		*(.sdata2)
		*(.got.plt)
		*(.got)
		*(.dynamic)
	}

	__reloc_end = . ;

	.initrd ALIGN(0x10) :
	{
		boot_initrd_start = . ;
		*(.initrd)
		boot_initrd_end = .;
	}

	. = ALIGN(0x10);
	__image_load = . ;
	.image 0xd0001000:
=======
	.ResetVector.text XCHAL_RESET_VECTOR_VADDR :
	{
		*(.ResetVector.text)
	}

	.image KERNELOFFSET: AT (CONFIG_KERNEL_LOAD_ADDRESS)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{
		_image_start = .;
		*(image)
		. = (. + 3) & ~ 3;
		_image_end = .	;
	}

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.bss ((LOADADDR(.image) + SIZEOF(.image) + 3) & ~ 3):
	{
		__bss_start = .;
		*(.sbss)
		*(.scommon)
		*(.dynbss)
		*(.bss)
		__bss_end = .;
	}
<<<<<<< HEAD
	_end = .;
	_param_start = .;

	.ResetVector.text XCHAL_RESET_VECTOR_VADDR :
	{
		*(.ResetVector.text)
	}


	PROVIDE (end = .);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
