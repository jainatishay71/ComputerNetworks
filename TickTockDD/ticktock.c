#include <linux/module.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <asm/uaccess.h>

static int Tick_init(void);
static void Tock_exit(void);
static int opendev(struct inode *, struct file *);
static int closedev(struct inode *, struct file *);
static ssize_t readme(struct file *, char *, size_t, loff_t *);
static ssize_t writeme(struct file *, const char *, size_t, loff_t *);
static int Major;            
#define SUCCESS 0
#define DEVICE_NAME "TickTock"   

static struct file_operations fops = {
  .read = readme, 
  .write = writeme,
  .open = opendev,
  .release = closedev
};

static int __init Tick_init(void){
  	Major = register_chrdev(0, DEVICE_NAME, &fops);
 		if(Major>0){
 			printk("TickTock is Ready For Requests!: 		  %d\n",Major);
 			return 0;
 		}else{
 			printk("Something Bad happened : %d",Major);
 			return 0;
 		}
}
module_init(Tick_init);

static void __exit Tock_exit(void)
{
 	unregister_chrdev(Major, DEVICE_NAME);
 	printk("Goodbye Cruel World!\n");
}
module_exit(Tock_exit);

MODULE_LICENSE("GPL");


static int opendev(struct inode * a, struct file * b){
printk("Someone opened me!\n");
//nothing to do!!!
return SUCCESS;
}
static int closedev(struct inode * a, struct file * b){
//nothing to do!!
printk("Someone Closed me!\n");
return SUCCESS;
}

static int itoa(int val,char** ca){
	char *t,*h;
	int r,i=0;
	char temp[2];
	char tmp[2];
	t=kmalloc(sizeof(char)*40,GFP_KERNEL);
	h=kmalloc(sizeof(char)*40,GFP_KERNEL);
	while(val)
	{
		r=val%10;
		temp[0]=(char)(r+48);
		temp[1]='\0';
		strcat(t,tmp);
	}
	for(i=strlen(t)-1;i>=0;i--)
	{
		tmp[0]=t[i];
		tmp[1]='\0';
		strcat(h,tmp);
	}
	strcpy(*ca,h);
	return strlen(h);
}


int atoi(const char * buff){
	int p=0,i=0,r;
	for(i=0;i<strlen(buff);i++)
	{
		r=buff[i]-48;
		p*=10;
		p+=r;
	}
	return p;
}

static ssize_t readme(struct file *filp,char *buffer,size_t length,loff_t *offset)  {
   int len;
   char *b;
   struct timeval time;
   do_gettimeofday(&time);
   b=kmalloc(sizeof(char)*40,GFP_KERNEL);
   len=itoa(time.tv_sec,&b);
   copy_to_user(buffer,b,len);
   printk("Return val:%s\n",b);
   return length;
}

static ssize_t writeme(struct file *filp,const char *buff,size_t len, loff_t *off)
{
  int i=atoi(buff);
  struct timespec ab;
  ab.tv_sec=i;
  ab.tv_nsec=0;
  do_settimeofday(&ab);
  return len;
}

