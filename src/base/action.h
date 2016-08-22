#ifndef MEDIS_BASE_ACTION_H_
#define MEDIS_BASE_ACTION_H_

/**
 * http://stackoverflow.com/questions/16868129/how-to-store-variadic-template-arguments
 * https://www.murrayc.com/permalink/2015/12/05/modern-c-variadic-template-parameters-and-tuples/
 */

#include <cstddef>  // for size_t
#include <functional>
#include <tuple>
#include <utility>  // for forward

namespace base {

namespace action_helper {

template <std::size_t... Ts>
struct index {};

template <std::size_t N, std::size_t... Ts>
struct gen_seq : gen_seq<N - 1, N - 1, Ts...> {};

template <std::size_t... Ts>
struct gen_seq<0, Ts...> : index<Ts...> {};
}

template <typename... Ts>
class Action {
 public:
  template <typename Function>
  Action(Function&& func, Ts&&... args)
      : _f(std::forward<Function>(func)),
        _args(std::make_tuple(std::forward<Ts>(args)...)) {}

  void act() { func(_args); }

 private:
  std::function<void(Ts...)> _f;
  std::tuple<Ts...> _args;

  template <typename... Args>
  void func(std::tuple<Args...>& args) {
    func(args, action_helper::gen_seq<sizeof...(Args)>{});
  }

  template <typename... Args, std::size_t... Is>
  void func(std::tuple<Args...>& args, action_helper::index<Is...>) {
    _f(std::forward<Args>(std::get<Is>(args))...);
  }
};

template <typename Function, typename... Args>
Action<Args...> make_action(Function&& f, Args&&... args) {
  return Action<Args...>(std::forward<Function>(f),
                         std::forward<Args>(args)...);
}
}

#endif  // MEDIS_BASE_ACTION_H_
