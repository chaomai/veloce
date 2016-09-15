#ifndef MEDIS_SERVER_DB_H_
#define MEDIS_SERVER_DB_H_

#include "ds/dict.h"

class Db {
 public:
  Db();
  ~Db();

 private:
  ds::Dict _dict;
};

#endif  // MEDIS_SERVER_DB_H_
