#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>

/***************************************************************
Copyright © ALIENTEK Co., Ltd. 1998-2029. All rights reserved.
文件名		: dtsled.c
作者	  	: pyj
版本	   	: V1.0
描述	   	: LED驱动文件。
其他	   	: 无
论坛 	   	: www.openedv.com
日志	   	: 初版V1.0 2019/7/9 pyj创建
***************************************************************/
#define DTSLED_CNT   1        /* 设备号个数 */
#define DTSLED_NAME  "dtsled" /* 设备名字 */
#define LEDOFF       0        /* 关灯 */
#define LEDON        1        /* 开灯 */

/* 映射后的寄存器虚拟地址指针 */
static void __iomem *IMX6U_CCM_CCGR1;
static void __iomem *SW_MUX_GPIO1_IO03;
static void __iomem *SW_PAD_GPIO1_IO03;
static void __iomem *GPIO1_DR;
static void __iomem *GPIO1_GDIR;

/* dtsled 设别结构体 */
struct dtsled_dev {
    dev_t devid;              /* 设备号 */
    struct cdev cdev;         /* cdev */
    struct class *class;      /* class 类 */
    struct device *device;    /* 设备 */
    int major;                /* 主设备号 */
    int minor;                /* 次设备号 */
    struct device_node *nd;   /* 设备节点 */
};

struct dtsled_dev dtsled;  /* led 设备 */

/**
 * @description  : LED/打开或者关闭
 * @param - sta  : LEDON(0) 打开LED  LEDOFF(1) 关闭LED
 * @return       : 无
 */
void led_switch(u8 sta)
{
    u32 val = 0;
    if (sta == LEDON)
    {
        val = readl(GPIO1_DR);
        val &= ~(1 << 3);
        writel(val, GPIO1_DR);
    } else if (sta == LEDOFF)
    {
        val = readl(GPIO1_DR);
        val |= (1 << 3);
        writel(val, GPIO1_DR);
    }
}

/**
 * @description   : 打开设备
 * @param - inode : 传递给驱动的 inode
 * @param - filp  : 设备文件，file结构体有个叫做private_data的成员变量
 *                  一般在open的时候将private_data指向设备结构体
 * @return        : 0 成功；其他 失败
 */
static int led_open(struct inode *inode, struct file *filp)
{
    filp->private_data = &dtsled; /* 设置私有数据 */
    return 0;
}

/**
 * @description    : 从设备读取数据
 * @param - filp   : 要打开的设备文件（文件描述符）
 * @param - buf    : 返回给用户空间的数据缓冲区
 * @param - cnt    : 要读取的数据长度
 * @param - offt   : 相对于文件首地址的偏移
 * @return         : 读取的字节数，如果为负值，表示读取失败
 */
static ssize_t led_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
    return 0;
}

/**
 * @description   : 向设备写数据
 * @param - filp  : 设备文件，表示打开的文件描述符
 * @param - buf   : 要写给设备写入的数据
 * @param - cnt   : 要写入的数据长度
 * @param - offt  : 相对于文件首地址的偏移
 * @return        : 写入的字节数，如果为负值，表示写入失败。
 */
static ssize_t led_write(struct file *filp, const char __user *buf, size_t cnt, loff_t, *offt)
{
    int retvalue;
    unsigned char databuf[1];
    unsigned char ledstat;

    retvalue = copy_from_user(databuf, buf, cnt);
    if (retvalue < 0)
    {
        printk("kernel write failed!\r\n");
        return -EFAULT;
    }

    ledstat = databuf[0];  /* 获取状态值 */

    if (ledstat == LEDON) {
        led_switch(LEDON);   /* 打开 LED  灯*/
    } else if (ledstat == LEDOFF) {
        led_switch(LEDOFF);  /* 关闭 LED 灯 */
    }
    return 0;
}


/**
 * @description  : 关闭/释放设备
 * @param - filp : 要关闭的设备文件(文件描述符)
 * @return       : 0 成功; 其他 失败
 */
static int led_release(struct *inode *inode, struct file *filp)
{
    return 0;
}

/**
 * @description 
 */
