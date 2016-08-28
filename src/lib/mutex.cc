#include "mutex.h"

#include <pthread.h>

Mutex::Mutex() {
  pthread_mutexattr_init(&_mutexattr);
  pthread_mutex_init(&_mutex, &_mutexattr);
}

void Mutex::lock() { pthread_mutex_lock(&_mutex); }

bool Mutex::try_lock() {
  pthread_mutex_trylock(&_mutex);
  return false;
}

void Mutex::unlock() { pthread_mutex_unlock(&_mutex); }

Mutex::~Mutex() {
  pthread_mutexattr_destroy(&_mutexattr);
  pthread_mutex_destroy(&_mutex);
}
