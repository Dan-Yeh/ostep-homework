#ifndef __common_threads_h__
#define __common_threads_h__

#include <assert.h>
#include <pthread.h>
#include <sched.h>

#define Pthread_create(thread, attr, start_routine, arg)                       \
  assert(pthread_create(thread, attr, start_routine, arg) == 0);
#define Pthread_join(thread, value_ptr)                                        \
  assert(pthread_join(thread, value_ptr) == 0);

#define Pthread_mutex_init(m, v) assert(pthread_mutex_init(m, v) == 0);
#define Pthread_mutex_lock(m) assert(pthread_mutex_lock(m) == 0);
#define Pthread_mutex_unlock(m) assert(pthread_mutex_unlock(m) == 0);

#define Pthread_cond_init(cond, v) assert(pthread_cond_init(cond, v) == 0);
#define Pthread_cond_signal(cond) assert(pthread_cond_signal(cond) == 0);
#define Pthread_cond_wait(cond, mutex)                                         \
  assert(pthread_cond_wait(cond, mutex) == 0);

#define Mutex_init(m) assert(pthread_mutex_init(m, NULL) == 0);
#define Mutex_lock(m) assert(pthread_mutex_lock(m) == 0);
#define Mutex_unlock(m) assert(pthread_mutex_unlock(m) == 0);

#define Cond_init(cond) assert(pthread_cond_init(cond, NULL) == 0);
#define Cond_signal(cond) assert(pthread_cond_signal(cond) == 0);
#define Cond_wait(cond, mutex) assert(pthread_cond_wait(cond, mutex) == 0);

#ifdef __linux__
#include <semaphore.h>
#define Sem_init(sem, value) assert(sem_init(sem, 0, value) == 0);
#define Sem_wait(sem) assert(sem_wait(sem) == 0);
#define Sem_post(sem) assert(sem_post(sem) == 0);
#elif defined __APPLE__
#include <dispatch/dispatch.h>
#define Sem_init(sem, value)                                                   \
  *sem = dispatch_semaphore_create(value);                                     \
  assert(*sem != NULL);
#define Sem_wait(sem)                                                          \
  assert(dispatch_semaphore_wait(*sem, DISPATCH_TIME_FOREVER) == 0);
#define Sem_post(sem) assert(dispatch_semaphore_signal(*sem) >= 0);
#endif

#endif // __common_threads_h__
