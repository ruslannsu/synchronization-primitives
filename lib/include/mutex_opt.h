#include <stdatomic.h>

#define UNLOCKED 0
#define LOCKED_NO_CONFLICT 1
#define LOCKED_CONFLICT 2

typedef struct mutex_opt_t {
    _Atomic lock_flag;
}mutex_opt_t;

int mutex_opt_init(mutex_opt_t *mutex);

int mutex_opt_lock(mutex_opt_t *mutex);

int mutex_opt_unlock(mutex_opt_t *mutex);


