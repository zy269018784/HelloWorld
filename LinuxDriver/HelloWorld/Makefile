obj-m += helloworld.o

KDIR:=/root/linux-6.7

PWD?=$(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules
