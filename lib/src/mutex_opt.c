#include <unistd.h>
#include <mutex_opt.h>
#include <string.h>
#include <sys/syscall.h>
#include <linux/futex.h>
#include <errno.h>

static int futex(int *uaddr, int futex_op, int val, const struct timespec *timeout) {
    return syscall(SYS_futex, uaddr, futex_op, val, timeout, NULL, 0);
}

int mutex_opt_init(mutex_opt_t *mutex) {
    if (!mutex) {
        return -1;
    }

    mutex->lock_flag = UNLOCKED;
    
    return 0;
}

int mutex_opt_lock(mutex_opt_t *mutex) {
    if (!mutex) {
        printf("%s\n", "invalid mutex_lock_t");
        return -1;
    }

    int err;

    int expected_value = UNLOCKED;
    if (!atomic_compare_exchange_strong(&mutex->lock_flag,  &expected_value, LOCKED_NO_CONFLICT)) {

        expected_value = LOCKED_NO_CONFLICT;
        
        while ((atomic_compare_exchange_strong(&mutex->lock_flag,  &expected_value, LOCKED_CONFLICT)) || (mutex->lock_flag == LOCKED_CONFLICT)) {
            err = futex(&mutex->lock_flag, FUTEX_WAIT, LOCKED_CONFLICT, NULL);
            if (err == -1) {
                printf("%s\n", strerror(errno));
                return -1;
            }
        }
    }

    return 0;
}

int mutex_opt_unlock(mutex_opt_t *mutex) {
    if (!mutex) {
        printf("%s\n", "invalid mutex_lock_t");
        return -1;
    }
    
    int err;

    if (atomic_fetch_sub(&mutex->lock_flag, 1) != 1) {
        err = futex(&mutex->lock_flag, FUTEX_WAKE, 1, NULL);
        if (err == -1) {
            printf("%s\n", strerror(errno));
            return -1;
        }

        mutex->lock_flag = UNLOCKED;
    }
    
    return 0;
}