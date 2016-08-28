#ifndef MEDIS_LIB_THREAD_H_
#define MEDIS_LIB_THREAD_H_

#include <pthread.h>

#include <memory>

#include "base/action.h"
#include "base/base.h"
#include "thread_exception.h"

using base::Action;
using base::make_action;

/**
 * @brief thread library.
 */
class Thread {
  class Id {
   public:
    Id(pthread_t thread_id = 0) : _value(thread_id) {}

   private:
    pthread_t _value;
  };

 public:
  using id = Id;
  using native_handle_type = pthread_t;

  Thread();

  template <class Function, class... Args>
  explicit Thread(Function&& f, Args&&... args);

  Thread(const Thread& rhs) = delete;
  Thread(Thread&& rhs) noexcept;
  Thread& operator=(const Thread& rhs) = delete;
  Thread& operator=(Thread&& rhs) noexcept;
  ~Thread() = default;

  bool joinable() const { return _joinable; }
  id get_id() const { return id(_thread_id); }
  native_handle_type native_handle() { return _thread_id; }
  static unsigned hardware_concurrency();

  void join();
  void detach();

 private:
  pthread_t _thread_id;
  std::shared_ptr<pthread_attr_t> _attr;
  id _id;
  bool _joinable;
};

template <typename Function, typename... Args>
void* start_routine(void* arg) {
  Action<Args...>* action = reinterpret_cast<Action<Args...>*>(arg);
  action->act();
  pthread_exit(nullptr);
  return nullptr;
}

template <typename Function, typename... Args>
Thread::Thread(Function&& f, Args&&... args) : _joinable(false) {
  _attr = std::make_shared<pthread_attr_t>();
  pthread_attr_init(_attr.get());

  auto action =
      make_action(std::forward<Function>(f), std::forward<Args>(args)...);
  int ret = pthread_create(&_thread_id, _attr.get(),
                           &start_routine<Function, Args...>,
                           reinterpret_cast<void*>(&action));

  if (ret) {
    throw ThreadException(get_errno_str(ret).c_str());
  }

  _joinable = true;
}

#endif  // MEDIS_LIB_THREAD_H_
