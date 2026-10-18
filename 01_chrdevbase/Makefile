#KERNELDIR := /home/boot/linux/IMX6ULL/linux/linux-imx-rel_imx_4.1.15_2.1.0_ga_alientek
KERNELDIR := /home/boot/linux/Release_source

ARCH := arm
CROSS_COMPILE := /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-

CURRENT_PATH := $(shell pwd)

obj-m := chrdevbase.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) \
		M=$(CURRENT_PATH) \
		ARCH=$(ARCH) \
		CROSS_COMPILE=$(CROSS_COMPILE) \
		modules

clean:
	$(MAKE) -C $(KERNELDIR) \
		M=$(CURRENT_PATH) \
		ARCH=$(ARCH) \
		CROSS_COMPILE=$(CROSS_COMPILE) \
		clean
