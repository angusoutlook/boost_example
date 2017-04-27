obj-m+=hello_h.o
CURRENT:=$(shell pwd)
KERNEL_PATH:=/usr/src/kernels/$(shell uname -r)

all:
	make -C $(KERNEL_PATH) M=$(CURRENT) modules
clean:
	make -C $(KERNEL_PATH) M=$(CURRENT) clean

