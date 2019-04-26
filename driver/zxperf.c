#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <asm/page.h>
//#include <asm/uaccess.h>
#include <linux/uaccess.h>
#include <asm-generic/ioctl.h>

#include <asm/msr.h>  //rdmsr
#include "../app/zxperf.h"

MODULE_AUTHOR("Tiny");
MODULE_LICENSE("Dual BSD/GPL");

static int zxperf_major = 0;
module_param(zxperf_major, int, 0);

spinlock_t spinlock;

static int zxperf_opend(struct inode *inode, struct file *filp)
{
	return 0;
}

static int zxperf_release(struct inode *inode, struct file *filp)
{
	return 0;
}

void zxperf_vma_open(struct vm_area_struct *vma)
{
	printk(KERN_NOTICE "zxperf VMA open, vir %lx, phy %lx\n",vma->vm_start, vma->vm_pgoff << PAGE_SHIFT);
}

void zxperf_vma_close(struct vm_area_struct *vma)
{

}

static struct vm_operations_struct zxperf_remap_vm_ops = 
{
	.open = zxperf_vma_open,
	.close = zxperf_vma_close,
};

static int zxperf_remap_mmap(struct file *filp, struct vm_area_struct *vma)
{
	if(remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff, vma->vm_end - vma->vm_start, vma->vm_page_prot))
		return -EAGAIN;
	vma->vm_ops = &zxperf_remap_vm_ops;
	zxperf_vma_open(vma);
	return 0;
}


long zxperf_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	long retval = 0;
	struct ZXPERF chx002perf;
	unsigned long long temp;
	spin_lock(&spinlock);
	if(copy_from_user(&chx002perf, (void __user *)arg, sizeof(struct ZXPERF)))
	{	
		spin_unlock(&spinlock);
		printk(KERN_NOTICE "fun: copy_from_user error! \n");
		retval = -1;
		return retval;
	}
		
	switch(cmd)
	{
	case ZXPERF_RDMSR:
	{
		rdmsr(chx002perf.msr, chx002perf.msr_data_low, chx002perf.msr_data_high);
		break;
	}
	case ZXPERF_WRMSR:
	{
		wrmsr(chx002perf.msr,chx002perf.msr_data_low,chx002perf.msr_data_high);
		rdmsr(chx002perf.msr,chx002perf.msr_data_low,chx002perf.msr_data_high);
		break;
	}
	case ZXPERF_INIT:
	{
		rdmsr(chx002perf.core_msr_perf_global_ctrl,chx002perf.core_msr_perf_global_ctrl_data_low,chx002perf.core_msr_perf_global_ctrl_data_high);
		temp = (unsigned long long )(((unsigned long long )chx002perf.core_msr_perf_global_ctrl_data_high << 32) | chx002perf.core_msr_perf_global_ctrl_data_low);
		printk("ioctl ZXPERF_INIT \n");
		printk("core_msr_perf_global_ctrl data is %llx \n", temp );
		break;
	}
	case ZXPERF_EVENT_SET:
	{
		wrmsr(chx002perf.core_msr_perfevtsl0,chx002perf.core_msr_perfevtsl0_data_low,chx002perf.core_msr_perfevtsl0_data_high);
		printk("ioctl ZXPERF_EVENT_SET \n");
		break;
	}
	case ZXPERF_COUNTER_READ:
	{
		rdmsr(chx002perf.core_msr_pmc0,chx002perf.core_msr_pmc0_data_low, chx002perf.core_msr_pmc0_data_high);
		temp = (unsigned long long )chx002perf.core_msr_pmc0_data_high<<32 | chx002perf.core_msr_pmc0_data_low;
		printk("ioctl ZXPERF_COUNTER_READ \n");
		printk("core_msr_pmc0 data is %llx \n", temp);
		break;
	}
	default:
	{
		spin_unlock(&spinlock);
		return -ENOTTY;	
	}
		
	}
	if(copy_to_user((void __user*)arg, &chx002perf,sizeof(struct ZXPERF)))
	{
		spin_unlock(&spinlock);
		printk("fun: copy_to_user is error! \n");
		retval = -1;
		return retval;
	}
	spin_unlock(&spinlock);
	return retval;
}

static struct file_operations zxperf_remap_ops = 
{
	.owner = THIS_MODULE,
	.open = zxperf_opend,
	.release = zxperf_release,
	.unlocked_ioctl = zxperf_ioctl,
	.compat_ioctl = zxperf_ioctl,
	.mmap = zxperf_remap_mmap,
};

static void zxperf_setup_cdev(struct cdev *dev, int minor, struct file_operations *fops)
{
	int err, devno = MKDEV(zxperf_major, minor);
	cdev_init(dev, fops);
	dev->owner = THIS_MODULE;
	dev->ops = fops;
	err = cdev_add(dev, devno, 1);
	if(err)
	{
		printk(KERN_NOTICE "add zxperf dev error: err is %d, devno is %d\n",err,devno);
	}
		
}

static struct cdev ZxPerfDevs[2];
struct class *my_class;
dev_t dev;
static int zxperf_init(void)
{
	int result;
	dev = MKDEV(zxperf_major,0);
	if(zxperf_major)
	{
		result = register_chrdev_region(dev, 2, "zxperf");
	}
	else
	{
		result = alloc_chrdev_region(&dev,0,2,"zxperf");
		zxperf_major = MAJOR(dev);
	}
	if(result < 0)
	{
		printk(KERN_WARNING "zxperf: unable to get major %d\n", zxperf_major);
		return result;
	}
	if(zxperf_major == 0)
	{
		zxperf_major = result;
	}
	my_class = class_create(THIS_MODULE,"zxperf_class");
	zxperf_setup_cdev(ZxPerfDevs, 0, &zxperf_remap_ops);
	device_create(my_class, NULL, dev,NULL, "zxperf");
	printk(KERN_NOTICE "zxperf init \n");
	spin_lock_init(&spinlock);
	return 0;
}

static void zxperf_exit(void)
{
	cdev_del(ZxPerfDevs);
	device_destroy(my_class, dev);
	class_destroy(my_class);
	unregister_chrdev_region(MKDEV(zxperf_major,0),2);
	printk(KERN_NOTICE "zxperf exit \n");
}

module_init(zxperf_init);
module_exit(zxperf_exit);
