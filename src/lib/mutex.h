#ifndef MEDIS_BASE_MUTEX_H_
#define MEDIS_BASE_MUTEX_H_

#include <pthread.h>

class Mutex {
 public:
  using native_handle_type = pthread_mutex_t;

  Mutex();
  Mutex(const Mutex& rhs) = delete;
  Mutex(Mutex&& rhs) = delete;
  Mutex& operator=(const Mutex& rhs) = delete;
  Mutex& operator=(Mutex&& rhs) = delete;
  ~Mutex();

  void lock();
  bool try_lock();
  void unlock();

  native_handle_type native_handle() { return _mutex; }

 private:
  pthread_mutex_t _mutex;
  pthread_mutexattr_t _mutexattr;
};

#endif  // MEDIS_BASE_MUTEX_H_
