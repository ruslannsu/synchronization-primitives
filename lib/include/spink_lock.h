
#include <stdatomic.h>

typedef struct spin_lock_t {
    _Atomic lock_flag;
}spin_lock_t;

int spin_lock_init(spin_lock_t *spin_lock);

int spin_lock_lock(spin_lock_t *spin_lock);

int spin_lock_unlock(spin_lock_t *spin_lock);