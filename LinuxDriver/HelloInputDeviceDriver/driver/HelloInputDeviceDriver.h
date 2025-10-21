#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>          // file_operations
#include <linux/uaccess.h>     // copy_to_user/copy_from_user
#include <linux/cdev.h>        // cdev
#include <linux/device.h>      // class_create, device_create


#define DEVICE_NAME "my_char_dev"
#define CLASS_NAME "my_char_class"

static int Major;
static struct cdev HelloCdev;
static struct class *DeviceClass;
static char msg[100] = {0};

static int HelloCharDeviceDriverOpen(struct inode *inode, struct file *file)
{
    return 0;
}

static ssize_t HelloCharDeviceDriverRead(struct file *file, char __user *buf, size_t len, loff_t *offset) 
{
    if (len > 100)
        len = 100;
    int ret = copy_to_user(buf, msg, len);
    if (ret) {
        printk(KERN_ALERT "Failed to send data to user\n");
        return -EFAULT;
    }
    printk(KERN_INFO "Sent %zu bytes to user\n", len);
    return len;
}

static ssize_t HelloCharDeviceDriverWrite(struct file *file, const char __user *buf, size_t len, loff_t *offset)
{
    if (len > sizeof(msg)) {
        printk(KERN_ALERT "Data too long\n");
        return -EINVAL;
    }
    memset(msg, 0, sizeof(msg));
    int ret = copy_from_user(msg, buf, len);
    if (ret) {
        printk(KERN_ALERT "Failed to read data from user\n");
        return -EFAULT;
    }
    printk(KERN_INFO "Received %zu bytes: %s\n", len, msg);
    return len;
}

static int HelloCharDeviceDriverRelease(struct inode *inode, struct file *file) 
{
    printk(KERN_INFO "Device closed\n");
    return 0;
}

// 定义文件操作结构体
static struct file_operations fops = {
    .owner      = THIS_MODULE,
    .open       = HelloCharDeviceDriverOpen,
    .read       = HelloCharDeviceDriverRead,
    .write      = HelloCharDeviceDriverWrite,
    .release    = HelloCharDeviceDriverRelease,
};

static int __init HelloCharDeviceDriver_init(void)
{
    printk("HelloCharDeviceDriver init\n");
    Major = register_chrdev(0, DEVICE_NAME, &fops);
    if (Major < 0)
    {
        printk(KERN_ALERT "Failed to register device\n");
        return Major;
    }

     // 2. 初始化 cdev 结构并添加到内核
    cdev_init(&HelloCdev, &fops);
    if (cdev_add(&HelloCdev, MKDEV(Major, 0), 1) < 0) 
    {
        printk(KERN_ALERT "Failed to add cdev\n");
        unregister_chrdev(Major, DEVICE_NAME);
        return -1;
    }

    // 3. 创建设备类和设备文件（自动出现在 /dev/）
    DeviceClass = class_create(CLASS_NAME);
    if (IS_ERR(DeviceClass)) {
        printk(KERN_ALERT "Failed to create class\n");
        cdev_del(&HelloCdev);
        unregister_chrdev(Major, DEVICE_NAME);
        return PTR_ERR(DeviceClass);
    }

    if (IS_ERR(device_create(DeviceClass, NULL, MKDEV(Major, 0), NULL, DEVICE_NAME))) 
    {
        printk(KERN_ALERT "Failed to create device\n");
        class_destroy(DeviceClass);
        cdev_del(&HelloCdev);
        unregister_chrdev(Major, DEVICE_NAME);
        return -1;
    }


    return 0;
}

static void __exit HelloCharDeviceDriver_exit(void) 
{
    printk("HelloCharDeviceDriver exit\n");

       // 1. 删除设备文件
    device_destroy(DeviceClass, MKDEV(Major, 0));
    // 2. 删除设备类
    class_destroy(DeviceClass);
    // 3. 删除 cdev
    cdev_del(&HelloCdev);

    // 4. 注销设备号
    unregister_chrdev(Major, DEVICE_NAME);
}

module_init(HelloCharDeviceDriver_init);
module_exit(HelloCharDeviceDriver_exit);

MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Hello Driver");
MODULE_LICENSE("GPL");
