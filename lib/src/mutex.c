#include <unistd.h>
#include <mutex.h>
#include <string.h>
#include <sys/syscall.h>
#include <linux/futex.h>
#include <errno.h>

static int futex(int *uaddr, int futex_op, int val, const struct timespec *timeout) {
    return syscall(SYS_futex, uaddr, futex_op, val, timeout, NULL, 0);
}

int mutex_init(mutex_t *mutex) {
    if (!mutex) {
        return -1;
    }

    mutex->lock_flag = 0;
    
    return 0;
}

int mutex_lock(mutex_t *mutex) {
    if (!mutex) {
        printf("%s\n", "invalid mutex_lock_t");
        return -1;
    }

    if (!((mutex->lock_flag == 1) || (mutex->lock_flag == 0))) {
        printf("%s\n", "mutex_lock failed, bad lock_flag");
        return -1;
    }

    int err;

    int expected_value = 0;
    while (!atomic_compare_exchange_strong(&mutex->lock_flag,  &expected_value, 1)) {
        expected_value = 0;

        err = futex(&mutex->lock_flag, FUTEX_WAIT, 1, NULL);
        if (err == -1) {
            printf("%s\n", strerror(errno));
            return -1;
        }

    }

    return 0;
}

int mutex_unlock(mutex_t *mutex) {
    if (!mutex) {
        printf("%s\n", "invalid mutex_lock_t");
        return -1;
    }

    if (!((mutex->lock_flag == 1) || (mutex->lock_flag == 0))) {
        printf("%s\n", "mutex_unlock failed, bad lock_flag");
        return -1;
    }

    int expected_value = 1;
    if (!atomic_compare_exchange_strong(&mutex->lock_flag,  &expected_value, 0)) {
        printf("%s\n", "already locked");
        return -1;
    }

    int err;

    err = futex(&mutex->lock_flag, FUTEX_WAKE, 0, NULL);
    if (err == -1) {
        printf("%s\n", strerror(errno));
        return -1;
    }    

    return 0;
}