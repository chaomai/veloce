#ifndef MEDIS_SERVER_MEDIS_H_
#define MEDIS_SERVER_MEDIS_H_

#include <cstddef>  // for size_t
#include <functional>
#include <string>

#include "base/base.h"
#include "client_info.h"
#include "db.h"
#include "ds/coarse_grained_hash_table.h"
#include "parser.h"

class Medis {
  using key_type = std::string;
  using hasher = std::hash<key_type>;
  using key_equal = std::equal_to<key_type>;
  using handler = std::function<void(const Args&, ClientInfo&)>;
  using hash_table =
      ds::CoarseGrainedHashTable<key_type, handler, hasher, key_equal>;

 public:
  Medis();
  Medis(const Medis& rhs) = delete;
  Medis(Medis&& rhs) = delete;
  Medis& operator=(const Medis& rhs) = delete;
  Medis& operator=(Medis&& rhs) = delete;
  ~Medis();

  void handle(ClientInfo& clinet_info);

 private:
  void init_handler();

  Db* _dbs;
  hash_table _handlers;
};

#endif  // MEDIS_SERVER_MEDIS_H_
