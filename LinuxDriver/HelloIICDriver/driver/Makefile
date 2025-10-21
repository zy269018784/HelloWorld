obj-m += HelloGPIODriver.o
KDIR:=/root/linux-6.14.11
PWD?=$(shell pwd)
all:
        make -C $(KDIR) M=$(PWD) modules
clean:
        make -C $(KDIR) M=$(PWD) clean
