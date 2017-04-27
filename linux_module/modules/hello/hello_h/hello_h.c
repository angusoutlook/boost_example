#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
static unsigned int hello_data=100;
EXPORT_SYMBOL(hello_data);

static int hello_h_init(void)
{
	hello_data+=5;
	printk(KERN_ERR "hello_data:%d\nhello kernel,this is hello_h module\n",hello_data);

	return 0;
}

static void hello_h_exit(void)
{
	hello_data-=5;
	printk(KERN_ERR "hello_data:%d\nleave hello_h module\n",hello_data);
}

module_init(hello_h_init);
module_exit(hello_h_exit);
MODULE_AUTHOR("Mike Feng");

