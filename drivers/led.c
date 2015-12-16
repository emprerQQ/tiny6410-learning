#include <linux/module.h>
#include <linux/init.h>
#include<linux/fs.h>

#include<linux/miscdevice.h>
#include<asm/io.h>

#include<mach/gpio-bank-k.h>
#include<mach/regs-gpio.h>

#include<mach/map.h>
#include<linux/cdev.h>
#include<asm/uaccess.h>


#define DEVICE_NAME     "ledsdev"

unsigned int DEVICE_MAJOR = 0; //通过判断是否为零，来确定是动态还是静态分配设备号
/***********************************************
   strut cdev{
                struct kobject kobj;                            //内嵌的内核对象
                struct module *owner;                           //指向内核模块对象的指针
                const struct file_operations *ops;      //应用程>序调用操作系统接口呼叫到设备驱动程序中相应操作
                struct list_head list;                          //链表
                dev_t dev;                                                      //字符设备的设备号
                unsigned int count;                                     //次设备号的个数
   };
*************************************************/
static struct cdev leds_cdev;         //定义设备对象名称
static dev_t leds_dev_t;        //dev_t类型用来保存设备编号，包括主设备号和次设备号

static struct class * s3c6410_leds_class = NULL;
static struct device * s3c6410_leds_device = NULL;              //师兄说是undev创建节点使用，但是undev并不知道是什么东西
//{{{ open
// int (*open)(struct inode *inode, struct file *filp)
// open方法提供给驱动程序以初始化的能力，从而为以后的操作完成初始化做准备。
// 初始化字符设备 - LED IO端口
// 全局变量, 为0表示未初始化; 1表示已初始化;
static int leds_status = 0;   //如果设备首次打开，对其进行初始化，也就是说只会初始化一次
static int s3c6410_leds_open(struct inode *nd,struct file *flip)
{
	unsigned long tmp;

	if(leds_status == 0)
	{
		tmp = ioread32(S3C6410_GPKCON);
		tmp &= ~(0xFFFF0000);
		tmp |=(); //看一下芯片手册
		iowrite32(tmp,S3C64XX_GPKDAT);
		leds_status = 1;
	}
	
	printk(DEVICE_NAME",OPEN CHAR DEVICE.\n");
	return 0;

}

static int s3c6410_leds_close(struct inode *nd,struct file *filp)
{
	printk(DEVICE_NAME",CLOSE CHAR DEVICE.\n");
	return 0;
}

// ioctl是设备驱动程序中对设备的I/O通道进行管理的函数
// 用户程序通过命令码(cmd)告诉驱动程序想做什么, 至于怎么解释这些>命令和怎么实现这些命令,这都是驱动程序要做的事情

static long s3c6410_leds_ioctl(struct file*filp,unsigned int cmd,unsigned long arg)
{
	switch(cmd){
		unsigned tmp;
		case 0;
		case 1;
		{
			if(arg>4)
			{
				return -EINVAL
			}
			else if(arg == 0)
			{
				tmp = ioread32(S3C64XX_GPKDAT);
				if(cmd == 1)
					tmp &= ~(0xF<<4);
				else if(cmd == 0)					tmp |= (0xF<<4);
				iowrite32(tmp,S3C64XX_GPKDAT);
			
			}
			else 
			{
			tmp = ioread32(S3C64XX_GPKDAT);
			tmp &= ~(1 << (3+arg));
			tmp |=((!cmd) << (3+arg))
			iowrite32(tmp, S3C64XX_GPKDAT);	
			}
			return 0;

		}
		default:
			return -EINVAL;

		}
		return 0;		
}


}



static long s3c6410_leds_ioctl(struct file* filp, unsigned int cmd, unsingned long arg)
{
	switch(cmd)
	{
		unsigned tmp;
		case 0;
		case 1:
		{
			if(arg>4)
			{
				return -EINVAL;
			}
			else if(arg == 0)
			{
				tmp = ioread32(S3C64XX_GPKDAT)
				if(cmd ==1)
					tmp &= ~(0xF<<4);
				else if(cmd == 0)
					tmp |=(0xF<<4);
				iowrite32(emp, S3C64XX_GPKDATA);
			}
			else 
			{
				tmp = ioread32(S3C64XX_GPKDAT);
				tmp &= ～（1 << (3+arg)）;
				tmp |= ((!cmd) << (3+arg));
				iowrite32(tmp, S3C64XX_GPKDAT);
			}
			return 0;

		}
		default:
			return -EINVAL;	
	}
		return 0;

}
/**************************************************
为了增强程序的可移植性，便有了size_t，它是为了方便系统之间的移植而定义的，不同的系统上，定义size_t可能不一样。

l          在32位系统上定义为unsigned int ，也就是说在32位系统上是32位无符号整形（typedef unsigned int size_t）。在64位系统上定义为 unsigned long 也就是说在64位系统上是64位无符号整形（typedef unsigned long size_t）。size_t一般用来表示一种计数，比如有多少东西被拷贝等。例如：sizeof操作符的结果类型是size_t，该类型保证能容纳实现所建立的最大对象的字节大小。 它的意义大致是“适于计量内存中可容纳的数据项目个数的无符号整数类型”。所以，它在数组下标和内存管理函数之类的地方广泛使用。

l          而ssize_t这个数据类型用来表示可以被执行读写操作的数据块的大小。它和size_t类似,但必需是signed.意即：它表示的是signed size_t类型的（typedef signed int ssize_t）。

 
ssize_t是signed size_t，
size_t是标准C库中定义的，应为unsigned int。

size_t  为 unsigned  long/int 类型

ssize_t 为 long/int 类型

总而言之  ssize_t  =  signed  size_t

loff_t 为 long long 类型

********************************************/
static ssize_t s3c6410_leds_read(struct file *filep,char __user*buf,size_t count, loff_t *off)
{
	volatile unsigned long tmp = 0;
	size_t failed_copy_count = 0;
	tmp = ioread32(S3C64XX_GPKDAT);
	tmp = (tmp&(0xF<<4)) >> 4;

	failed_copy_count = copy_to_user((char *)buf,(char *)&tmp, 4);
	if(failed_copy_count != 0)
		return -EFAULT;
	else 
		return 0;
}

static struct file_operations S3C6410_LEDS_FOPS =
{

	.owner = THIS_MODULE,
	.open = s3c6410_leds_open,
	.release = s3c6410_leds_close,
	.read = s3c6410_leds_read,
	.unlocked_ioctl = s3c6410_leds_ioctl,
}

static int __init s3c6410_leds_init(void)
{
	int ret;
	printk(DEVICE_NAME",char device init.\n");

	if(DEVICE_MAJOR)
	{
		leds_dev_t MKDEV(DEVICE_MAJOR, 0);
// int register_chrdev_region(dev_t from, unsigned count, const char *name) - register a range of device numbers
		ret = register_chrdev_region(leds_dev_t,1,DEVICE_NAME);
		if(ret < 0)
		{
			printk(KERN_WARNING"register device numbers error.\n");
			return ret;
		}
	}
	else
	{
	
		int leds_major = 0;
		ret = alloc_chrdev_region(&leds_dev_t,0,1,DEVICE_NAME );
		if(ret < 0)
		{
			printk(KERN_WARNING"alloc char device number error|n");
			return ret;
		leds_major = MAJOR(leds_dev_t);
		
		leds_major = MAJOR(leds_dev_t);
		
		printk(KERN_ALERT"LEDS device major = %d.\n",leds_major);

		}
        // 已静态创建cdev结构体变量leds_cdev
        //void cdev_init(struct cdev*cdev,struct file_operations *fops)

		cdev_init(&leds_cdev,&S3C6410_LEDS_FOPS);
		cdev_add(&leds_cdev,leds_dev_t,1);
		
		return 0;
	
	}

//module exit
// 执行rmmod命令时就会调用这个函数 
// static int __exit cleanup_module(void)       //默认>驱动清除函数
// __exit 宏, 表示将此函数代码放在".exit.data"段中; 静>态链接时没有使用,因为静态链接的驱动不能被卸载

static void __exit s3c6410_leds_exit(void)
{
	printk(DEVICE_NAME"module exit.\n");
	cdev_del(&leds_cdev);
	unregister_chrdev_region(leds_dev_t, 1);
}

module_init(s3c6410_leds_init);
module_exit(s3c6410_leds_exit);

MODULE_VERSION("1");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LED DRIVER");


