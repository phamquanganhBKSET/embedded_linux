#include <linux/module.h>

#define DRIVER_AUTHOR "Pham Quang Anh 20182359"
#define DRIVER_DESC   "Kernel Module"


static int __init init_kernel_module(void)
{
    printk("Bai tap He dieu hanh: Kernel Module\n");
    return 0;
}

static void __exit exit_kernel_module(void)
{
    printk("Thoat Kernel Module\n");
}

module_init(init_kernel_module);
module_exit(exit_kernel_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_SUPPORTED_DEVICE("OSdevice");
