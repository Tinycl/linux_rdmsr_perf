ifneq ($(KERNELRELEASE),)
obj-m := zxperf.o
else
KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif

clean:
	rm -rf *.o *~ *.ko *.mod.c .tmp_versions *.order *.symvers *.ur-safe
depend .depend dep:
	$(CC) $(CFLAGS) -M *.c > .depend

ifeq (.depend, $(wildcard .depend))
include .depend
endif

