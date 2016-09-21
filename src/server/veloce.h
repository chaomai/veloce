#ifndef VELOCE_SERVER_VELOCE_H_
#define VELOCE_SERVER_VELOCE_H_

#include <cstddef>  // for size_t
#include <functional>
#include <string>

#include "base/base.h"
#include "client_info.h"
#include "db.h"
#include "ds/coarse_grained_hash_table.h"
#include "parser.h"

class Veloce {
  using key_type = std::string;
  using hasher = std::hash<key_type>;
  using key_equal = std::equal_to<key_type>;
  using handler = std::function<void(const Args&, ClientInfo&)>;
  using hash_table =
      ds::CoarseGrainedHashTable<key_type, handler, hasher, key_equal>;

 public:
  Veloce();
  Veloce(const Veloce& rhs) = delete;
  Veloce(Veloce&& rhs) = delete;
  Veloce& operator=(const Veloce& rhs) = delete;
  Veloce& operator=(Veloce&& rhs) = delete;
  ~Veloce();

  void handle(ClientInfo& clinet_info);

 private:
  void init_handler();
  void append_error(std::string& out, const std::string& err,
                    const std::string& info = "");
  void append_item(std::string& out, const Item* item);
  void append_item(std::string& out, const std::string& str);

  Db* _dbs;
  hash_table _handlers;
};

#endif  // VELOCE_SERVER_VELOCE_H_
