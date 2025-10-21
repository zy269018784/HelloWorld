#include <linux/module.h>
#include <linux/input.h>

static struct input_dev *HelloInputDeviceDriver_dev;

static irqreturn_t HelloInputDeviceDriver_interrupt(int irq, void *dummy)
{
    input_report_key(HelloInputDeviceDriver_dev, BTN_0, 1);  // 按下
    input_sync(HelloInputDeviceDriver_dev);
    input_report_key(HelloInputDeviceDriver_dev, BTN_0, 0);  // 释放
    input_sync(HelloInputDeviceDriver_dev);
    return IRQ_HANDLED;
}

static int __init HelloInputDeviceDriver_init(void)
{
    int error;
    
    // 分配输入设备
    HelloInputDeviceDriver_dev = input_allocate_device();
    if (!HelloInputDeviceDriver_dev) {
        printk(KERN_ERR "HelloInputDeviceDriver: not enough memory\n");
        return -ENOMEM;
    }
    
    // 设置设备信息
    HelloInputDeviceDriver_dev->name = "Example HelloInputDeviceDriver";
    HelloInputDeviceDriver_dev->phys = "HelloInputDeviceDriver/input0";
    
    // 设置支持的事件类型
    set_bit(EV_KEY, HelloInputDeviceDriver_dev->evbit);
    set_bit(BTN_0, HelloInputDeviceDriver_dev->keybit);
    
    // 注册设备
    error = input_register_device(HelloInputDeviceDriver_dev);
    if (error) {
        printk(KERN_ERR "HelloInputDeviceDriver: failed to register\n");
        input_free_device(HelloInputDeviceDriver_dev);
        return error;
    }
    
    // 注册中断处理程序(假设使用GPIO 5)
    if (request_irq(gpio_to_irq(5), HelloInputDeviceDriver_interrupt, IRQF_TRIGGER_RISING,
                    "HelloInputDeviceDriver", NULL)) {
        printk(KERN_ERR "HelloInputDeviceDriver: cannot allocate irq\n");
        input_unregister_device(HelloInputDeviceDriver_dev);
        return -EBUSY;
    }
    
    return 0;
}

static void __exit HelloInputDeviceDriver_exit(void)
{
    free_irq(gpio_to_irq(5), NULL);
    input_unregister_device(HelloInputDeviceDriver_dev);
    input_free_device(HelloInputDeviceDriver_dev);
}

module_init(HelloInputDeviceDriver_init);
module_exit(HelloInputDeviceDriver_exit);