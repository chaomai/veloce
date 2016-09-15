#ifndef MEDIS_SERVER_DB_H_
#define MEDIS_SERVER_DB_H_

#include "ds/dict.h"

class Db {
 public:
  Db();
  ~Db();

  // string
  // append
  // bitcount
  // decr
  // decrby
  // get
  // getrange
  // getset
  // incr
  // incrby
  // incrbyfloat
  // mget
  // mset
  // msetnx
  // set
  // setnx
  // setrange
  // strlen

  // list
  // llen
  // lpop
  // lpush
  // lpushx
  // lrem
  // lset
  // rpop
  // rpush
  // rpushx

 private:
  ds::Dict _dict;
};

#endif  // MEDIS_SERVER_DB_H_
