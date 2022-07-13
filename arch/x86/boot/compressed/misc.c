<<<<<<< HEAD
/*
 * misc.c
 *
 * This is a collection of several routines from gzip-1.0.3
 * adapted for Linux.
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * misc.c
 *
 * This is a collection of several routines used to extract the kernel
 * which includes KASLR relocation, decompression, ELF parsing, and
 * relocation processing. Additionally included are the screen and serial
 * output functions and related debugging support functions.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * malloc by Hannu Savolainen 1993 and Matthias Urlichs 1994
 * puts by Nick Holloway 1993, better puts by Martin Mares 1995
 * High loaded stuff by Hans Lermen & Werner Almesberger, Feb. 1996
 */

#include "misc.h"
<<<<<<< HEAD

/* WARNING!!
 * This code is compiled with -fPIC and it is relocated dynamically
 * at run time, but no relocation processing is performed.
 * This means that it is not safe to place pointers in static structures.
 */

/*
 * Getting to provable safe in place decompression is hard.
 * Worst case behaviours need to be analyzed.
 * Background information:
 *
 * The file layout is:
 *    magic[2]
 *    method[1]
 *    flags[1]
 *    timestamp[4]
 *    extraflags[1]
 *    os[1]
 *    compressed data blocks[N]
 *    crc[4] orig_len[4]
 *
 * resulting in 18 bytes of non compressed data overhead.
 *
 * Files divided into blocks
 * 1 bit (last block flag)
 * 2 bits (block type)
 *
 * 1 block occurs every 32K -1 bytes or when there 50% compression
 * has been achieved. The smallest block type encoding is always used.
 *
 * stored:
 *    32 bits length in bytes.
 *
 * fixed:
 *    magic fixed tree.
 *    symbols.
 *
 * dynamic:
 *    dynamic tree encoding.
 *    symbols.
 *
 *
 * The buffer for decompression in place is the length of the
 * uncompressed data, plus a small amount extra to keep the algorithm safe.
 * The compressed data is placed at the end of the buffer.  The output
 * pointer is placed at the start of the buffer and the input pointer
 * is placed where the compressed data starts.  Problems will occur
 * when the output pointer overruns the input pointer.
 *
 * The output pointer can only overrun the input pointer if the input
 * pointer is moving faster than the output pointer.  A condition only
 * triggered by data whose compressed form is larger than the uncompressed
 * form.
 *
 * The worst case at the block level is a growth of the compressed data
 * of 5 bytes per 32767 bytes.
 *
 * The worst case internal to a compressed block is very hard to figure.
 * The worst case can at least be boundined by having one bit that represents
 * 32764 bytes and then all of the rest of the bytes representing the very
 * very last byte.
 *
 * All of which is enough to compute an amount of extra data that is required
 * to be safe.  To avoid problems at the block level allocating 5 extra bytes
 * per 32767 bytes of data is sufficient.  To avoind problems internal to a
 * block adding an extra 32767 bytes (the worst case uncompressed block size)
 * is sufficient, to ensure that in the worst case the decompressed data for
 * block will stop the byte before the compressed data for a block begins.
 * To avoid problems with the compressed data's meta information an extra 18
 * bytes are needed.  Leading to the formula:
 *
 * extra_bytes = (uncompressed_size >> 12) + 32768 + 18 + decompressor_size.
 *
 * Adding 8 bytes per 32K is a bit excessive but much easier to calculate.
 * Adding 32768 instead of 32767 just makes for round numbers.
 * Adding the decompressor_size is necessary as it musht live after all
 * of the data as well.  Last I measured the decompressor is about 14K.
 * 10K of actual data and 4K of bss.
 *
 */

/*
 * gzip declarations
 */
#define STATIC		static

#undef memset
#undef memcpy
#define memzero(s, n)	memset((s), 0, (n))


static void error(char *m);
=======
#include "error.h"
#include "pgtable.h"
#include "../string.h"
#include "../voffset.h"
#include <asm/bootparam_utils.h>

/*
 * WARNING!!
 * This code is compiled with -fPIC and it is relocated dynamically at
 * run time, but no relocation processing is performed. This means that
 * it is not safe to place pointers in static structures.
 */

/* Macros used by the included decompressor code below. */
#define STATIC		static
/* Define an externally visible malloc()/free(). */
#define MALLOC_VISIBLE
#include <linux/decompress/mm.h>

/*
 * Provide definitions of memzero and memmove as some of the decompressors will
 * try to define their own functions if these are not defined as macros.
 */
#define memzero(s, n)	memset((s), 0, (n))
#ifndef memmove
#define memmove		memmove
/* Functions used by the included decompressor code below. */
void *memmove(void *dest, const void *src, size_t n);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * This is set up by the setup-routine at boot-time
 */
<<<<<<< HEAD
struct boot_params *real_mode;		/* Pointer to real-mode data */
static int quiet;
static int debug;

void *memset(void *s, int c, size_t n);
void *memcpy(void *dest, const void *src, size_t n);

#ifdef CONFIG_X86_64
#define memptr long
#else
#define memptr unsigned
#endif

static memptr free_mem_ptr;
static memptr free_mem_end_ptr;

static char *vidmem;
static int vidport;
static int lines, cols;
=======
struct boot_params *boot_params_ptr;

struct port_io_ops pio_ops;

memptr free_mem_ptr;
memptr free_mem_end_ptr;
int spurious_nmi_count;

static char *vidmem;
static int vidport;

/* These might be accessed before .bss is cleared, so use .data instead. */
static int lines __section(".data");
static int cols __section(".data");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_KERNEL_GZIP
#include "../../../../lib/decompress_inflate.c"
#endif

#ifdef CONFIG_KERNEL_BZIP2
#include "../../../../lib/decompress_bunzip2.c"
#endif

#ifdef CONFIG_KERNEL_LZMA
#include "../../../../lib/decompress_unlzma.c"
#endif

#ifdef CONFIG_KERNEL_XZ
#include "../../../../lib/decompress_unxz.c"
#endif

#ifdef CONFIG_KERNEL_LZO
#include "../../../../lib/decompress_unlzo.c"
#endif

<<<<<<< HEAD
=======
#ifdef CONFIG_KERNEL_LZ4
#include "../../../../lib/decompress_unlz4.c"
#endif

#ifdef CONFIG_KERNEL_ZSTD
#include "../../../../lib/decompress_unzstd.c"
#endif
/*
 * NOTE: When adding a new decompressor, please update the analysis in
 * ../header.S.
 */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void scroll(void)
{
	int i;

<<<<<<< HEAD
	memcpy(vidmem, vidmem + cols * 2, (lines - 1) * cols * 2);
=======
	memmove(vidmem, vidmem + cols * 2, (lines - 1) * cols * 2);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (i = (lines - 1) * cols * 2; i < lines * cols * 2; i += 2)
		vidmem[i] = ' ';
}

#define XMTRDY          0x20

#define TXR             0       /*  Transmit register (WRITE) */
#define LSR             5       /*  Line Status               */
static void serial_putchar(int ch)
{
	unsigned timeout = 0xffff;

	while ((inb(early_serial_base + LSR) & XMTRDY) == 0 && --timeout)
		cpu_relax();

	outb(ch, early_serial_base + TXR);
}

<<<<<<< HEAD
void __putstr(int error, const char *s)
=======
void __putstr(const char *s)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int x, y, pos;
	char c;

<<<<<<< HEAD
#ifndef CONFIG_X86_VERBOSE_BOOTUP
	if (!error)
		return;
#endif
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (early_serial_base) {
		const char *str = s;
		while (*str) {
			if (*str == '\n')
				serial_putchar('\r');
			serial_putchar(*str++);
		}
	}

<<<<<<< HEAD
	if (real_mode->screen_info.orig_video_mode == 0 &&
	    lines == 0 && cols == 0)
		return;

	x = real_mode->screen_info.orig_x;
	y = real_mode->screen_info.orig_y;
=======
	if (lines == 0 || cols == 0)
		return;

	x = boot_params_ptr->screen_info.orig_x;
	y = boot_params_ptr->screen_info.orig_y;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	while ((c = *s++) != '\0') {
		if (c == '\n') {
			x = 0;
			if (++y >= lines) {
				scroll();
				y--;
			}
		} else {
			vidmem[(x + cols * y) * 2] = c;
			if (++x >= cols) {
				x = 0;
				if (++y >= lines) {
					scroll();
					y--;
				}
			}
		}
	}

<<<<<<< HEAD
	real_mode->screen_info.orig_x = x;
	real_mode->screen_info.orig_y = y;
=======
	boot_params_ptr->screen_info.orig_x = x;
	boot_params_ptr->screen_info.orig_y = y;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	pos = (x + cols * y) * 2;	/* Update cursor position */
	outb(14, vidport);
	outb(0xff & (pos >> 9), vidport+1);
	outb(15, vidport);
	outb(0xff & (pos >> 1), vidport+1);
}

<<<<<<< HEAD
void *memset(void *s, int c, size_t n)
{
	int i;
	char *ss = s;

	for (i = 0; i < n; i++)
		ss[i] = c;
	return s;
}
#ifdef CONFIG_X86_32
void *memcpy(void *dest, const void *src, size_t n)
{
	int d0, d1, d2;
	asm volatile(
		"rep ; movsl\n\t"
		"movl %4,%%ecx\n\t"
		"rep ; movsb\n\t"
		: "=&c" (d0), "=&D" (d1), "=&S" (d2)
		: "0" (n >> 2), "g" (n & 3), "1" (dest), "2" (src)
		: "memory");

	return dest;
}
#else
void *memcpy(void *dest, const void *src, size_t n)
{
	long d0, d1, d2;
	asm volatile(
		"rep ; movsq\n\t"
		"movq %4,%%rcx\n\t"
		"rep ; movsb\n\t"
		: "=&c" (d0), "=&D" (d1), "=&S" (d2)
		: "0" (n >> 3), "g" (n & 7), "1" (dest), "2" (src)
		: "memory");

	return dest;
}
#endif

static void error(char *x)
{
	__putstr(1, "\n\n");
	__putstr(1, x);
	__putstr(1, "\n\n -- System halted");

	while (1)
		asm("hlt");
}

static void parse_elf(void *output)
=======
static noinline void __putnum(unsigned long value, unsigned int base,
			      int mindig)
{
	char buf[8*sizeof(value)+1];
	char *p;

	p = buf + sizeof(buf);
	*--p = '\0';

	while (mindig-- > 0 || value) {
		unsigned char digit = value % base;
		digit += (digit >= 10) ? ('a'-10) : '0';
		*--p = digit;

		value /= base;
	}

	__putstr(p);
}

void __puthex(unsigned long value)
{
	__putnum(value, 16, sizeof(value)*2);
}

void __putdec(unsigned long value)
{
	__putnum(value, 10, 1);
}

#ifdef CONFIG_X86_NEED_RELOCS
static void handle_relocations(void *output, unsigned long output_len,
			       unsigned long virt_addr)
{
	int *reloc;
	unsigned long delta, map, ptr;
	unsigned long min_addr = (unsigned long)output;
	unsigned long max_addr = min_addr + (VO___bss_start - VO__text);

	/*
	 * Calculate the delta between where vmlinux was linked to load
	 * and where it was actually loaded.
	 */
	delta = min_addr - LOAD_PHYSICAL_ADDR;

	/*
	 * The kernel contains a table of relocation addresses. Those
	 * addresses have the final load address of the kernel in virtual
	 * memory. We are currently working in the self map. So we need to
	 * create an adjustment for kernel memory addresses to the self map.
	 * This will involve subtracting out the base address of the kernel.
	 */
	map = delta - __START_KERNEL_map;

	/*
	 * 32-bit always performs relocations. 64-bit relocations are only
	 * needed if KASLR has chosen a different starting address offset
	 * from __START_KERNEL_map.
	 */
	if (IS_ENABLED(CONFIG_X86_64))
		delta = virt_addr - LOAD_PHYSICAL_ADDR;

	if (!delta) {
		debug_putstr("No relocation needed... ");
		return;
	}
	debug_putstr("Performing relocations... ");

	/*
	 * Process relocations: 32 bit relocations first then 64 bit after.
	 * Three sets of binary relocations are added to the end of the kernel
	 * before compression. Each relocation table entry is the kernel
	 * address of the location which needs to be updated stored as a
	 * 32-bit value which is sign extended to 64 bits.
	 *
	 * Format is:
	 *
	 * kernel bits...
	 * 0 - zero terminator for 64 bit relocations
	 * 64 bit relocation repeated
	 * 0 - zero terminator for inverse 32 bit relocations
	 * 32 bit inverse relocation repeated
	 * 0 - zero terminator for 32 bit relocations
	 * 32 bit relocation repeated
	 *
	 * So we work backwards from the end of the decompressed image.
	 */
	for (reloc = output + output_len - sizeof(*reloc); *reloc; reloc--) {
		long extended = *reloc;
		extended += map;

		ptr = (unsigned long)extended;
		if (ptr < min_addr || ptr > max_addr)
			error("32-bit relocation outside of kernel!\n");

		*(uint32_t *)ptr += delta;
	}
#ifdef CONFIG_X86_64
	while (*--reloc) {
		long extended = *reloc;
		extended += map;

		ptr = (unsigned long)extended;
		if (ptr < min_addr || ptr > max_addr)
			error("inverse 32-bit relocation outside of kernel!\n");

		*(int32_t *)ptr -= delta;
	}
	for (reloc--; *reloc; reloc--) {
		long extended = *reloc;
		extended += map;

		ptr = (unsigned long)extended;
		if (ptr < min_addr || ptr > max_addr)
			error("64-bit relocation outside of kernel!\n");

		*(uint64_t *)ptr += delta;
	}
#endif
}
#else
static inline void handle_relocations(void *output, unsigned long output_len,
				      unsigned long virt_addr)
{ }
#endif

static size_t parse_elf(void *output)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
#ifdef CONFIG_X86_64
	Elf64_Ehdr ehdr;
	Elf64_Phdr *phdrs, *phdr;
#else
	Elf32_Ehdr ehdr;
	Elf32_Phdr *phdrs, *phdr;
#endif
	void *dest;
	int i;

	memcpy(&ehdr, output, sizeof(ehdr));
	if (ehdr.e_ident[EI_MAG0] != ELFMAG0 ||
	   ehdr.e_ident[EI_MAG1] != ELFMAG1 ||
	   ehdr.e_ident[EI_MAG2] != ELFMAG2 ||
<<<<<<< HEAD
	   ehdr.e_ident[EI_MAG3] != ELFMAG3) {
		error("Kernel is not a valid ELF file");
		return;
	}

	if (!quiet)
		putstr("Parsing ELF... ");
=======
	   ehdr.e_ident[EI_MAG3] != ELFMAG3)
		error("Kernel is not a valid ELF file");

	debug_putstr("Parsing ELF... ");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	phdrs = malloc(sizeof(*phdrs) * ehdr.e_phnum);
	if (!phdrs)
		error("Failed to allocate space for phdrs");

	memcpy(phdrs, output + ehdr.e_phoff, sizeof(*phdrs) * ehdr.e_phnum);

	for (i = 0; i < ehdr.e_phnum; i++) {
		phdr = &phdrs[i];

		switch (phdr->p_type) {
		case PT_LOAD:
<<<<<<< HEAD
=======
#ifdef CONFIG_X86_64
			if ((phdr->p_align % 0x200000) != 0)
				error("Alignment of LOAD segment isn't multiple of 2MB");
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_RELOCATABLE
			dest = output;
			dest += (phdr->p_paddr - LOAD_PHYSICAL_ADDR);
#else
			dest = (void *)(phdr->p_paddr);
#endif
<<<<<<< HEAD
			memcpy(dest,
			       output + phdr->p_offset,
			       phdr->p_filesz);
=======
			memmove(dest, output + phdr->p_offset, phdr->p_filesz);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		default: /* Ignore other PT_* */ break;
		}
	}

	free(phdrs);
<<<<<<< HEAD
}

asmlinkage void decompress_kernel(void *rmode, memptr heap,
				  unsigned char *input_data,
				  unsigned long input_len,
				  unsigned char *output)
{
	real_mode = rmode;

	if (cmdline_find_option_bool("quiet"))
		quiet = 1;
	if (cmdline_find_option_bool("debug"))
		debug = 1;

	if (real_mode->screen_info.orig_video_mode == 7) {
=======

	return ehdr.e_entry - LOAD_PHYSICAL_ADDR;
}

const unsigned long kernel_text_size = VO___start_rodata - VO__text;
const unsigned long kernel_total_size = VO__end - VO__text;

static u8 boot_heap[BOOT_HEAP_SIZE] __aligned(4);

extern unsigned char input_data[];
extern unsigned int input_len, output_len;

unsigned long decompress_kernel(unsigned char *outbuf, unsigned long virt_addr,
				void (*error)(char *x))
{
	unsigned long entry;

	if (!free_mem_ptr) {
		free_mem_ptr     = (unsigned long)boot_heap;
		free_mem_end_ptr = (unsigned long)boot_heap + sizeof(boot_heap);
	}

	if (__decompress(input_data, input_len, NULL, NULL, outbuf, output_len,
			 NULL, error) < 0)
		return ULONG_MAX;

	entry = parse_elf(outbuf);
	handle_relocations(outbuf, output_len, virt_addr);

	return entry;
}

/*
 * Set the memory encryption xloadflag based on the mem_encrypt= command line
 * parameter, if provided.
 */
static void parse_mem_encrypt(struct setup_header *hdr)
{
	int on = cmdline_find_option_bool("mem_encrypt=on");
	int off = cmdline_find_option_bool("mem_encrypt=off");

	if (on > off)
		hdr->xloadflags |= XLF_MEM_ENCRYPTION;
}

/*
 * The compressed kernel image (ZO), has been moved so that its position
 * is against the end of the buffer used to hold the uncompressed kernel
 * image (VO) and the execution environment (.bss, .brk), which makes sure
 * there is room to do the in-place decompression. (See header.S for the
 * calculations.)
 *
 *                             |-----compressed kernel image------|
 *                             V                                  V
 * 0                       extract_offset                      +INIT_SIZE
 * |-----------|---------------|-------------------------|--------|
 *             |               |                         |        |
 *           VO__text      startup_32 of ZO          VO__end    ZO__end
 *             ^                                         ^
 *             |-------uncompressed kernel image---------|
 *
 */
asmlinkage __visible void *extract_kernel(void *rmode, unsigned char *output)
{
	unsigned long virt_addr = LOAD_PHYSICAL_ADDR;
	memptr heap = (memptr)boot_heap;
	unsigned long needed_size;
	size_t entry_offset;

	/* Retain x86 boot parameters pointer passed from startup_32/64. */
	boot_params_ptr = rmode;

	/* Clear flags intended for solely in-kernel use. */
	boot_params_ptr->hdr.loadflags &= ~KASLR_FLAG;

	parse_mem_encrypt(&boot_params_ptr->hdr);

	sanitize_boot_params(boot_params_ptr);

	if (boot_params_ptr->screen_info.orig_video_mode == 7) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		vidmem = (char *) 0xb0000;
		vidport = 0x3b4;
	} else {
		vidmem = (char *) 0xb8000;
		vidport = 0x3d4;
	}

<<<<<<< HEAD
	lines = real_mode->screen_info.orig_video_lines;
	cols = real_mode->screen_info.orig_video_cols;

	console_init();
	if (debug)
		putstr("early console in decompress_kernel\n");
=======
	lines = boot_params_ptr->screen_info.orig_video_lines;
	cols = boot_params_ptr->screen_info.orig_video_cols;

	init_default_io_ops();

	/*
	 * Detect TDX guest environment.
	 *
	 * It has to be done before console_init() in order to use
	 * paravirtualized port I/O operations if needed.
	 */
	early_tdx_detect();

	console_init();

	/*
	 * Save RSDP address for later use. Have this after console_init()
	 * so that early debugging output from the RSDP parsing code can be
	 * collected.
	 */
	boot_params_ptr->acpi_rsdp_addr = get_rsdp_addr();

	debug_putstr("early console in extract_kernel\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	free_mem_ptr     = heap;	/* Heap */
	free_mem_end_ptr = heap + BOOT_HEAP_SIZE;

<<<<<<< HEAD
	if ((unsigned long)output & (MIN_KERNEL_ALIGN - 1))
		error("Destination address inappropriately aligned");
#ifdef CONFIG_X86_64
	if (heap > 0x3fffffffffffUL)
		error("Destination address too large");
=======
	/*
	 * The memory hole needed for the kernel is the larger of either
	 * the entire decompressed kernel plus relocation table, or the
	 * entire decompressed kernel plus .bss and .brk sections.
	 *
	 * On X86_64, the memory is mapped with PMD pages. Round the
	 * size up so that the full extent of PMD pages mapped is
	 * included in the check against the valid memory table
	 * entries. This ensures the full mapped area is usable RAM
	 * and doesn't include any reserved areas.
	 */
	needed_size = max_t(unsigned long, output_len, kernel_total_size);
#ifdef CONFIG_X86_64
	needed_size = ALIGN(needed_size, MIN_KERNEL_ALIGN);
#endif

	/* Report initial kernel position details. */
	debug_putaddr(input_data);
	debug_putaddr(input_len);
	debug_putaddr(output);
	debug_putaddr(output_len);
	debug_putaddr(kernel_total_size);
	debug_putaddr(needed_size);

#ifdef CONFIG_X86_64
	/* Report address of 32-bit trampoline */
	debug_putaddr(trampoline_32bit);
#endif

	choose_random_location((unsigned long)input_data, input_len,
				(unsigned long *)&output,
				needed_size,
				&virt_addr);

	/* Validate memory location choices. */
	if ((unsigned long)output & (MIN_KERNEL_ALIGN - 1))
		error("Destination physical address inappropriately aligned");
	if (virt_addr & (MIN_KERNEL_ALIGN - 1))
		error("Destination virtual address inappropriately aligned");
#ifdef CONFIG_X86_64
	if (heap > 0x3fffffffffffUL)
		error("Destination address too large");
	if (virt_addr + needed_size > KERNEL_IMAGE_SIZE)
		error("Destination virtual address is beyond the kernel mapping area");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else
	if (heap > ((-__PAGE_OFFSET-(128<<20)-1) & 0x7fffffff))
		error("Destination address too large");
#endif
#ifndef CONFIG_RELOCATABLE
<<<<<<< HEAD
	if ((unsigned long)output != LOAD_PHYSICAL_ADDR)
		error("Wrong destination address");
#endif

	if (!quiet)
		putstr("\nDecompressing Linux... ");
	decompress(input_data, input_len, NULL, NULL, output, NULL, error);
	parse_elf(output);
	if (!quiet)
		putstr("done.\nBooting the kernel.\n");
	return;
=======
	if (virt_addr != LOAD_PHYSICAL_ADDR)
		error("Destination virtual address changed when not relocatable");
#endif

	debug_putstr("\nDecompressing Linux... ");

	if (init_unaccepted_memory()) {
		debug_putstr("Accepting memory... ");
		accept_memory(__pa(output), __pa(output) + needed_size);
	}

	entry_offset = decompress_kernel(output, virt_addr, error);

	debug_putstr("done.\nBooting the kernel (entry_offset: 0x");
	debug_puthex(entry_offset);
	debug_putstr(").\n");

	/* Disable exception handling before booting the kernel */
	cleanup_exception_handling();

	if (spurious_nmi_count) {
		error_putstr("Spurious early NMIs ignored: ");
		error_putdec(spurious_nmi_count);
		error_putstr("\n");
	}

	return output + entry_offset;
}

void __fortify_panic(const u8 reason, size_t avail, size_t size)
{
	error("detected buffer overflow");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
