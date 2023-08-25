#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/atomic.h>
#include <linux/spinlock.h>

DEFINE_SPINLOCK(smod_spinlock);
atomic_t smod_lock_cnt;

static int smod_init(void) {
	atomic_set(&smod_lock_cnt, 0);
	pr_info("inited\n");
	return 0;
}

static void smod_exit(void) {
	pr_info("exited\n");
}

int smod_lock_get(char *buf, struct kernel_param const *kp)
{
	ssize_t ret;
	int cnt;

	spin_lock(&smod_spinlock);

	cnt = atomic_inc_return(&smod_lock_cnt);
	ret = snprintf(buf, PAGE_SIZE, "smod lock count: %d\n", cnt);

	spin_unlock(&smod_spinlock);

	return ret;
}

static const struct kernel_param_ops smod_lock_ops = {
	.set = NULL,
	.get = smod_lock_get,
};

MODULE_PARM_DESC(smod_lock, "smod_lock");
module_param_cb(smod_lock, &smod_lock_ops, NULL, S_IRUGO);

module_init(smod_init);
module_exit(smod_exit);

MODULE_LICENSE("GPL");
