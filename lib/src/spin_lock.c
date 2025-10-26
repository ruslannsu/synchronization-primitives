#include <spink_lock.h>

int spin_lock_init(spin_lock_t *spin_lock) {
    if (!spin_lock) {
        return -1;
    }

    spin_lock->lock_flag = 0;

    return 0;
}

int spin_lock_lock(spin_lock_t *spin_lock) {
    if (!spin_lock) {
        printf("%s\n", "invalid spin_lock_t");
        return -1;
    }

    if (!((spin_lock->lock_flag == 1) || (spin_lock->lock_flag == 0))) {
        printf("%s\n", "spin_lock failed, bad lock_flag");
        return -1;
    }

    int expected_value = 0;

    while (!atomic_compare_exchange_strong(&spin_lock->lock_flag,  &expected_value, 1)) {
        expected_value = 0;
    }

    return 0;
}

int spin_lock_unlock(spin_lock_t *spin_lock) {
    if (!spin_lock) {
        printf("%s\n", "invalid spin_lock_t");
        return -1;
    }

    if (!((spin_lock->lock_flag == 1) || (spin_lock->lock_flag == 0))) {
        printf("%s\n", "spin_lock failed, bad lock_flag");
        return -1;
    }

    int expected_value = 1;
    if (!atomic_compare_exchange_strong(&spin_lock->lock_flag,  &expected_value, 0)) {
        printf("%s\n", "already unlocked");
        return -1;
    }

    return 0;
}

