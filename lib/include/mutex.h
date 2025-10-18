
typedef struct mutex_t {
    int lock_flag;
}mutex_t;

int mutex_init(mutex_t *mutex);

int mutex_lock(mutex_t *mutex);

int mutex_unlock(mutex_t *mutex);