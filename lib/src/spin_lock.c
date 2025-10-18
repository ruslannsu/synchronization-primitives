
#include <spink_lock.h>
#include <stdatomic.h>

#define EXPECTED_VALUE 0
#define DESIRED_VALUE 1


int spin_lock_init(spin_lock_t *spin_lock) {
    spin_lock->lock_flag = 0;
}

int spin_lock_lock(spin_lock_t *spin_lock) {
    int expected_value = 0;
    int val = atomic_compare_exchange_strong(&spin_lock->lock_flag,  &expected_value, 1);
    printf("%s, %d\n", "compared? - ", val);
    return 0;
    
}

int spin_lock_unlock(spin_lock_t *spin_lock);

