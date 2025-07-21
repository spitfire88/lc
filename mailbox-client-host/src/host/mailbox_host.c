#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mailbox.h>
#include "mailbox_host.h"
#include "mailbox_common.h"

static struct mbox_client mbox_client;
static struct mbox_chan *mbox_chan;

static void handle_message(struct mbox_msg *msg)
{
    // Process the incoming message
    printk(KERN_INFO "Received message: %s\n", msg->data);
}

int init_host(void)
{
    mbox_client.dev = NULL; // Set to the appropriate device
    mbox_client.rx_callback = handle_message;

    mbox_chan = mbox_request_channel(&mbox_client, 0);
    if (!mbox_chan) {
        printk(KERN_ERR "Failed to request mailbox channel\n");
        return -ENOMEM;
    }

    printk(KERN_INFO "Mailbox host initialized\n");
    return 0;
}

void start_host(void)
{
    // Start the host to listen for incoming messages
    printk(KERN_INFO "Host is now listening for messages\n");
}

void cleanup_host(void)
{
    mbox_free_channel(mbox_chan);
    printk(KERN_INFO "Mailbox host cleaned up\n");
}

module_init(init_host);
module_exit(cleanup_host);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Mailbox Host Implementation");
MODULE_VERSION("1.0");