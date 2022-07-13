#include <linux/fs.h>

extern const struct file_operations xen_privcmd_fops;
<<<<<<< HEAD
=======
extern const struct file_operations xen_privcmdbuf_fops;

extern struct miscdevice xen_privcmdbuf_dev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
