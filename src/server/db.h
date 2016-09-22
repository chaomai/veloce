#ifndef VELOCE_SERVER_DB_H_
#define VELOCE_SERVER_DB_H_

#include <utility>

#include "base/base.h"
#include "ds/dict.h"

class Db {
 public:
  enum class State { OK, TYPE_ERROR };

  Db() = default;
  Db(const Db& rhs) = delete;
  Db(Db&& rhs) = delete;
  Db& operator=(const Db& rhs) = delete;
  Db& operator=(Db&& rhs) = delete;
  ~Db() = default;

  /*
   * keys
   */
  std::pair<int, State> del(const Args& args);
  std::pair<bool, State> exists(const Args& args);

  /*
   * strings
   */
  std::pair<int, State> append(const Args& args);
  std::pair<Item*, State> get(const Args& args);
  std::pair<Item*, State> getset(const Args& args);
  std::pair<int, State> set(const Args& args);
  std::pair<int, State> setnx(const Args& args);
  std::pair<int, State> strlen(const Args& args);

  /*
   * list
   */
  std::pair<int, State> llen(const Args& args);

 private:
  ds::Dict _dict;
};

#endif  // VELOCE_SERVER_DB_H_
