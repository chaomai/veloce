#ifndef MEDIS_SERVER_DB_H_
#define MEDIS_SERVER_DB_H_

#include "base/base.h"
#include "ds/dict.h"

class Db {
 public:
  Db() = default;
  Db(const Db& rhs) = delete;
  Db(Db&& rhs) = delete;
  Db& operator=(const Db& rhs) = delete;
  Db& operator=(Db&& rhs) = delete;
  ~Db() = default;

  int set(const Args& args);
  int setnx(const Args& args);
  Item* get(const Args& args);

 private:
  ds::Dict _dict;
};

#endif  // MEDIS_SERVER_DB_H_
