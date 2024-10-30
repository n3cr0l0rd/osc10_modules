#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

ssize_t proc_entry_read(struct file *f, char __user *user_buff, size_t count,
										loff_t *pos);

static const char PROC_ENTRY_FILE_NAME[] = "s1mpl3_pr0c_f1l3";

static struct proc_ops procfs_funcs = {
	.proc_read = proc_entry_read,
};

int second_init(void)
{
	proc_create(PROC_ENTRY_FILE_NAME, 0666, NULL, &procfs_funcs);
	pr_info("%s file is now available in /proc pseudo-fs!\n",                    \
					PROC_ENTRY_FILE_NAME);
	return 0;
}

void second_exit(void)
{
	remove_proc_entry(PROC_ENTRY_FILE_NAME, NULL);
	pr_info("%s file is deleted from /proc pseudo-fs\n",
					PROC_ENTRY_FILE_NAME);
}

ssize_t proc_entry_read(struct file *f, char __user *user_buff, size_t count,
										loff_t *pos)
{
  ssize_t read_bytes;
  const unsigned short KERNEL_BUFF_SIZE = 256;
  char kernel_buff[KERNEL_BUFF_SIZE];
  static int done_writing = 0;
  static const char PROC_ENTRY_WHEN_READ_MESSAGE[128] = "Hello!\n              \
  If you see this message, module was compiled and then\n                      \
  insmod'ed successfully!\n";

  if(done_writing) {
    done_writing = 0;
    return 0;
  }

  done_writing = 1;

  read_bytes = snprintf(kernel_buff, KERNEL_BUFF_SIZE,                         \
												PROC_ENTRY_WHEN_READ_MESSAGE);
  copy_to_user(user_buff, kernel_buff, sizeof(kernel_buff));
  return read_bytes;
}

MODULE_LICENSE("BSD");
module_init(second_init);
module_exit(second_exit);
