#include "thread.h"

#include <unistd.h>  // for sysconf

#include <memory>

#include "thread_exception.h"

Thread::Thread() : _thread_id(0), _joinable(false) {}

Thread::Thread(Thread&& rhs) noexcept : _thread_id(rhs._thread_id),
                                        _attr(rhs._attr),
                                        _joinable(rhs._joinable) {
  rhs._thread_id = 0;
  rhs._joinable = false;
}

Thread& Thread::operator=(Thread&& rhs) noexcept {
  if (this != &rhs) {
    _thread_id = rhs._thread_id;
    _attr = rhs._attr;
    _joinable = rhs._joinable;

    rhs._thread_id = 0;
    rhs._joinable = false;
  }

  return *this;
}

unsigned Thread::hardware_concurrency() {
  return static_cast<unsigned>(sysconf(_SC_NPROCESSORS_ONLN));
}

void Thread::join() {
  if (!joinable()) {
    throw ThreadException("thread is not joinable.");
  }

  pthread_join(_thread_id, nullptr);
  _joinable = false;
}

void Thread::detach() {
  if (!joinable()) {
    throw ThreadException("thread is not joinable.");
  }

  pthread_detach(_thread_id);
  _joinable = false;
}
