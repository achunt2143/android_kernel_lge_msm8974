<<<<<<< HEAD
/*
 * include/linux/random.h
 *
 * Include file for the random number generator.
 */
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifndef _LINUX_RANDOM_H
#define _LINUX_RANDOM_H

<<<<<<< HEAD
#include <linux/types.h>
#include <linux/ioctl.h>
#include <linux/irqnr.h>

/* ioctl()'s for the random number generator */

/* Get the entropy count. */
#define RNDGETENTCNT	_IOR( 'R', 0x00, int )

/* Add to (or subtract from) the entropy count.  (Superuser only.) */
#define RNDADDTOENTCNT	_IOW( 'R', 0x01, int )

/* Get the contents of the entropy pool.  (Superuser only.) */
#define RNDGETPOOL	_IOR( 'R', 0x02, int [2] )

/* 
 * Write bytes into the entropy pool and add to the entropy count.
 * (Superuser only.)
 */
#define RNDADDENTROPY	_IOW( 'R', 0x03, int [2] )

/* Clear entropy count to 0.  (Superuser only.) */
#define RNDZAPENTCNT	_IO( 'R', 0x04 )

/* Clear the entropy pool and associated counters.  (Superuser only.) */
#define RNDCLEARPOOL	_IO( 'R', 0x06 )

struct rand_pool_info {
	int	entropy_count;
	int	buf_size;
	__u32	buf[0];
};

struct rnd_state {
	__u32 s1, s2, s3;
};

/* Exported functions */

/*
 * Flags for getrandom(2)
 *
 * GRND_NONBLOCK	Don't block and return EAGAIN instead
 * GRND_RANDOM		Use the /dev/random pool instead of /dev/urandom
 */
#define GRND_NONBLOCK	0x0001
#define GRND_RANDOM	0x0002

#ifdef __KERNEL__

extern void add_device_randomness(const void *, unsigned int);
extern void add_input_randomness(unsigned int type, unsigned int code,
				 unsigned int value);
extern void add_interrupt_randomness(int irq, int irq_flags);

extern void get_random_bytes(void *buf, int nbytes);
extern void get_random_bytes_arch(void *buf, int nbytes);
void generate_random_uuid(unsigned char uuid_out[16]);
extern int random_int_secret_init(void);
=======
#include <linux/bug.h>
#include <linux/kernel.h>
#include <linux/list.h>

#include <uapi/linux/random.h>

struct notifier_block;

void add_device_randomness(const void *buf, size_t len);
void __init add_bootloader_randomness(const void *buf, size_t len);
void add_input_randomness(unsigned int type, unsigned int code,
			  unsigned int value) __latent_entropy;
void add_interrupt_randomness(int irq) __latent_entropy;
void add_hwgenerator_randomness(const void *buf, size_t len, size_t entropy, bool sleep_after);

static inline void add_latent_entropy(void)
{
#if defined(LATENT_ENTROPY_PLUGIN) && !defined(__CHECKER__)
	add_device_randomness((const void *)&latent_entropy, sizeof(latent_entropy));
#else
	add_device_randomness(NULL, 0);
#endif
}

#if IS_ENABLED(CONFIG_VMGENID)
void add_vmfork_randomness(const void *unique_vm_id, size_t len);
int register_random_vmfork_notifier(struct notifier_block *nb);
int unregister_random_vmfork_notifier(struct notifier_block *nb);
#else
static inline int register_random_vmfork_notifier(struct notifier_block *nb) { return 0; }
static inline int unregister_random_vmfork_notifier(struct notifier_block *nb) { return 0; }
#endif

void get_random_bytes(void *buf, size_t len);
u8 get_random_u8(void);
u16 get_random_u16(void);
u32 get_random_u32(void);
u64 get_random_u64(void);
static inline unsigned long get_random_long(void)
{
#if BITS_PER_LONG == 64
	return get_random_u64();
#else
	return get_random_u32();
#endif
}

u32 __get_random_u32_below(u32 ceil);

/*
 * Returns a random integer in the interval [0, ceil), with uniform
 * distribution, suitable for all uses. Fastest when ceil is a constant, but
 * still fast for variable ceil as well.
 */
static inline u32 get_random_u32_below(u32 ceil)
{
	if (!__builtin_constant_p(ceil))
		return __get_random_u32_below(ceil);

	/*
	 * For the fast path, below, all operations on ceil are precomputed by
	 * the compiler, so this incurs no overhead for checking pow2, doing
	 * divisions, or branching based on integer size. The resultant
	 * algorithm does traditional reciprocal multiplication (typically
	 * optimized by the compiler into shifts and adds), rejecting samples
	 * whose lower half would indicate a range indivisible by ceil.
	 */
	BUILD_BUG_ON_MSG(!ceil, "get_random_u32_below() must take ceil > 0");
	if (ceil <= 1)
		return 0;
	for (;;) {
		if (ceil <= 1U << 8) {
			u32 mult = ceil * get_random_u8();
			if (likely(is_power_of_2(ceil) || (u8)mult >= (1U << 8) % ceil))
				return mult >> 8;
		} else if (ceil <= 1U << 16) {
			u32 mult = ceil * get_random_u16();
			if (likely(is_power_of_2(ceil) || (u16)mult >= (1U << 16) % ceil))
				return mult >> 16;
		} else {
			u64 mult = (u64)ceil * get_random_u32();
			if (likely(is_power_of_2(ceil) || (u32)mult >= -ceil % ceil))
				return mult >> 32;
		}
	}
}

/*
 * Returns a random integer in the interval (floor, U32_MAX], with uniform
 * distribution, suitable for all uses. Fastest when floor is a constant, but
 * still fast for variable floor as well.
 */
static inline u32 get_random_u32_above(u32 floor)
{
	BUILD_BUG_ON_MSG(__builtin_constant_p(floor) && floor == U32_MAX,
			 "get_random_u32_above() must take floor < U32_MAX");
	return floor + 1 + get_random_u32_below(U32_MAX - floor);
}

/*
 * Returns a random integer in the interval [floor, ceil], with uniform
 * distribution, suitable for all uses. Fastest when floor and ceil are
 * constant, but still fast for variable floor and ceil as well.
 */
static inline u32 get_random_u32_inclusive(u32 floor, u32 ceil)
{
	BUILD_BUG_ON_MSG(__builtin_constant_p(floor) && __builtin_constant_p(ceil) &&
			 (floor > ceil || ceil - floor == U32_MAX),
			 "get_random_u32_inclusive() must take floor <= ceil");
	return floor + get_random_u32_below(ceil - floor + 1);
}

void __init random_init_early(const char *command_line);
void __init random_init(void);
bool rng_is_initialized(void);
int wait_for_random_bytes(void);
int execute_with_initialized_rng(struct notifier_block *nb);

/* Calls wait_for_random_bytes() and then calls get_random_bytes(buf, nbytes).
 * Returns the result of the call to wait_for_random_bytes. */
static inline int get_random_bytes_wait(void *buf, size_t nbytes)
{
	int ret = wait_for_random_bytes();
	get_random_bytes(buf, nbytes);
	return ret;
}

#define declare_get_random_var_wait(name, ret_type) \
	static inline int get_random_ ## name ## _wait(ret_type *out) { \
		int ret = wait_for_random_bytes(); \
		if (unlikely(ret)) \
			return ret; \
		*out = get_random_ ## name(); \
		return 0; \
	}
declare_get_random_var_wait(u8, u8)
declare_get_random_var_wait(u16, u16)
declare_get_random_var_wait(u32, u32)
declare_get_random_var_wait(u64, u32)
declare_get_random_var_wait(long, unsigned long)
#undef declare_get_random_var

/*
 * This is designed to be standalone for just prandom
 * users, but for now we include it from <linux/random.h>
 * for legacy reasons.
 */
#include <linux/prandom.h>

#ifdef CONFIG_SMP
int random_prepare_cpu(unsigned int cpu);
int random_online_cpu(unsigned int cpu);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifndef MODULE
extern const struct file_operations random_fops, urandom_fops;
#endif

<<<<<<< HEAD
unsigned int get_random_int(void);
unsigned long get_random_long(void);
unsigned long randomize_range(unsigned long start, unsigned long end, unsigned long len);

u32 prandom_u32(void);
void prandom_bytes(void *buf, int nbytes);
void prandom_seed(u32 seed);
void prandom_reseed_late(void);

/*
 * These macros are preserved for backward compatibility and should be
 * removed as soon as a transition is finished.
 */
#define random32() prandom_u32()
#define srandom32(seed) prandom_seed(seed)

u32 prandom_u32_state(struct rnd_state *);
void prandom_bytes_state(struct rnd_state *state, void *buf, int nbytes);

/*
 * Handle minimum values for seeds
 */
static inline u32 __seed(u32 x, u32 m)
{
	return (x < m) ? x + m : x;
}

/**
 * prandom_seed_state - set seed for prandom_u32_state().
 * @state: pointer to state structure to receive the seed.
 * @seed: arbitrary 64-bit value to use as a seed.
 */
static inline void prandom_seed_state(struct rnd_state *state, u64 seed)
{
	u32 i = (seed >> 32) ^ (seed << 10) ^ seed;

	state->s1 = __seed(i, 2);
	state->s2 = __seed(i, 8);
	state->s3 = __seed(i, 16);
}

#ifdef CONFIG_ARCH_RANDOM
# include <asm/archrandom.h>
#else
static inline int arch_get_random_long(unsigned long *v)
{
	return 0;
}
static inline int arch_get_random_int(unsigned int *v)
{
	return 0;
}
#endif

#endif /* __KERNEL___ */

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _LINUX_RANDOM_H */
