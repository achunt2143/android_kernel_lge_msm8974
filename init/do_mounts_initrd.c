<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/unistd.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/minix_fs.h>
#include <linux/romfs_fs.h>
#include <linux/initrd.h>
#include <linux/sched.h>
#include <linux/freezer.h>
<<<<<<< HEAD
=======
#include <linux/kmod.h>
#include <uapi/linux/mount.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "do_mounts.h"

unsigned long initrd_start, initrd_end;
int initrd_below_start_ok;
<<<<<<< HEAD
unsigned int real_root_dev;	/* do_proc_dointvec cannot handle kdev_t */
static int __initdata old_fd, root_fd;
static int __initdata mount_initrd = 1;

=======
static unsigned int real_root_dev;	/* do_proc_dointvec cannot handle kdev_t */
static int __initdata mount_initrd = 1;

phys_addr_t phys_initrd_start __initdata;
unsigned long phys_initrd_size __initdata;

#ifdef CONFIG_SYSCTL
static struct ctl_table kern_do_mounts_initrd_table[] = {
	{
		.procname       = "real-root-dev",
		.data           = &real_root_dev,
		.maxlen         = sizeof(int),
		.mode           = 0644,
		.proc_handler   = proc_dointvec,
	},
	{ }
};

static __init int kernel_do_mounts_initrd_sysctls_init(void)
{
	register_sysctl_init("kernel", kern_do_mounts_initrd_table);
	return 0;
}
late_initcall(kernel_do_mounts_initrd_sysctls_init);
#endif /* CONFIG_SYSCTL */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int __init no_initrd(char *str)
{
	mount_initrd = 0;
	return 1;
}

__setup("noinitrd", no_initrd);

<<<<<<< HEAD
static int __init do_linuxrc(void *_shell)
{
	static const char *argv[] = { "linuxrc", NULL, };
	extern const char *envp_init[];
	const char *shell = _shell;

	sys_close(old_fd);sys_close(root_fd);
	sys_setsid();
	return kernel_execve(shell, argv, envp_init);
}

static void __init handle_initrd(void)
{
	int error;
	int pid;
=======
static int __init early_initrdmem(char *p)
{
	phys_addr_t start;
	unsigned long size;
	char *endp;

	start = memparse(p, &endp);
	if (*endp == ',') {
		size = memparse(endp + 1, NULL);

		phys_initrd_start = start;
		phys_initrd_size = size;
	}
	return 0;
}
early_param("initrdmem", early_initrdmem);

static int __init early_initrd(char *p)
{
	return early_initrdmem(p);
}
early_param("initrd", early_initrd);

static int __init init_linuxrc(struct subprocess_info *info, struct cred *new)
{
	ksys_unshare(CLONE_FS | CLONE_FILES);
	console_on_rootfs();
	/* move initrd over / and chdir/chroot in initrd root */
	init_chdir("/root");
	init_mount(".", "/", NULL, MS_MOVE, NULL);
	init_chroot(".");
	ksys_setsid();
	return 0;
}

static void __init handle_initrd(char *root_device_name)
{
	struct subprocess_info *info;
	static char *argv[] = { "linuxrc", NULL, };
	extern char *envp_init[];
	int error;

	pr_warn("using deprecated initrd support, will be removed in 2021.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	real_root_dev = new_encode_dev(ROOT_DEV);
	create_dev("/dev/root.old", Root_RAM0);
	/* mount initrd on rootfs' /root */
<<<<<<< HEAD
	mount_block_root("/dev/root.old", root_mountflags & ~MS_RDONLY);
	sys_mkdir("/old", 0700);
	root_fd = sys_open("/", 0, 0);
	old_fd = sys_open("/old", 0, 0);
	/* move initrd over / and chdir/chroot in initrd root */
	sys_chdir("/root");
	sys_mount(".", "/", NULL, MS_MOVE, NULL);
	sys_chroot(".");

	/*
	 * In case that a resume from disk is carried out by linuxrc or one of
	 * its children, we need to tell the freezer not to wait for us.
	 */
	current->flags |= PF_FREEZER_SKIP;

	pid = kernel_thread(do_linuxrc, "/linuxrc", SIGCHLD);
	if (pid > 0)
		while (pid != sys_wait4(-1, NULL, 0, NULL))
			yield();

	current->flags &= ~PF_FREEZER_SKIP;

	/* move initrd to rootfs' /old */
	sys_fchdir(old_fd);
	sys_mount("/", ".", NULL, MS_MOVE, NULL);
	/* switch root and cwd back to / of rootfs */
	sys_fchdir(root_fd);
	sys_chroot(".");
	sys_close(old_fd);
	sys_close(root_fd);

	if (new_decode_dev(real_root_dev) == Root_RAM0) {
		sys_chdir("/old");
		return;
	}

	ROOT_DEV = new_decode_dev(real_root_dev);
	mount_root();

	printk(KERN_NOTICE "Trying to move old root to /initrd ... ");
	error = sys_mount("/old", "/root/initrd", NULL, MS_MOVE, NULL);
	if (!error)
		printk("okay\n");
	else {
		int fd = sys_open("/dev/root.old", O_RDWR, 0);
=======
	mount_root_generic("/dev/root.old", root_device_name,
			   root_mountflags & ~MS_RDONLY);
	init_mkdir("/old", 0700);
	init_chdir("/old");

	info = call_usermodehelper_setup("/linuxrc", argv, envp_init,
					 GFP_KERNEL, init_linuxrc, NULL, NULL);
	if (!info)
		return;
	call_usermodehelper_exec(info, UMH_WAIT_PROC|UMH_FREEZABLE);

	/* move initrd to rootfs' /old */
	init_mount("..", ".", NULL, MS_MOVE, NULL);
	/* switch root and cwd back to / of rootfs */
	init_chroot("..");

	if (new_decode_dev(real_root_dev) == Root_RAM0) {
		init_chdir("/old");
		return;
	}

	init_chdir("/");
	ROOT_DEV = new_decode_dev(real_root_dev);
	mount_root(root_device_name);

	printk(KERN_NOTICE "Trying to move old root to /initrd ... ");
	error = init_mount("/old", "/root/initrd", NULL, MS_MOVE, NULL);
	if (!error)
		printk("okay\n");
	else {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (error == -ENOENT)
			printk("/initrd does not exist. Ignored.\n");
		else
			printk("failed\n");
		printk(KERN_NOTICE "Unmounting old root\n");
<<<<<<< HEAD
		sys_umount("/old", MNT_DETACH);
		printk(KERN_NOTICE "Trying to free ramdisk memory ... ");
		if (fd < 0) {
			error = fd;
		} else {
			error = sys_ioctl(fd, BLKFLSBUF, 0);
			sys_close(fd);
		}
		printk(!error ? "okay\n" : "failed\n");
	}
}

int __init initrd_load(void)
=======
		init_umount("/old", MNT_DETACH);
	}
}

bool __init initrd_load(char *root_device_name)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (mount_initrd) {
		create_dev("/dev/ram", Root_RAM0);
		/*
		 * Load the initrd data into /dev/ram0. Execute it as initrd
		 * unless /dev/ram0 is supposed to be our actual root device,
		 * in that case the ram disk is just set up here, and gets
		 * mounted in the normal path.
		 */
		if (rd_load_image("/initrd.image") && ROOT_DEV != Root_RAM0) {
<<<<<<< HEAD
			sys_unlink("/initrd.image");
			handle_initrd();
			return 1;
		}
	}
	sys_unlink("/initrd.image");
	return 0;
=======
			init_unlink("/initrd.image");
			handle_initrd(root_device_name);
			return true;
		}
	}
	init_unlink("/initrd.image");
	return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
