#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

MODULE_AUTHOR("Dariy Chaplytskyi <DoveFoke@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

struct hello_entry {
    struct list_head list;  
    ktime_t time;           
};

static LIST_HEAD(hello_list); 

static unsigned int print_count = 1;
module_param(print_count, uint, S_IRUGO);
MODULE_PARM_DESC(print_count, "Кількість разів для виводу 'Hello, world!' (за замовчуванням=1)");


static int __init hello_init(void)
{
    if (print_count == 0 || (print_count > 5 && print_count < 10)) {
        pr_warn("Print count is 0 or between 5 and 10. Defaulting to 1.\n");
    }
    BUG_ON(print_count > 10);

    while (print_count > 0) {
        struct hello_entry *entry;

        if (print_count == 1) {
            entry = NULL;
        } else {
            entry = kmalloc(sizeof(*entry), GFP_KERNEL);
        }

        entry->time = ktime_get();
        list_add(&entry->list, &hello_list);

        pr_emerg("Hello, world!\n");

        print_count--;
    }

    return 0;
}

static void __exit hello_exit(void)
{
    struct hello_entry *entry, *temp;

    list_for_each_entry_safe(entry, temp, &hello_list, list) {
        pr_emerg("Time: %lld ns\n", ktime_to_ns(entry->time));
        list_del(&entry->list);
        kfree(entry);
    }
}

module_init(hello_init);
module_exit(hello_exit);

