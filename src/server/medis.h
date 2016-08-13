#ifndef MEDIS_SERVER_MEDIS_H_
#define MEDIS_SERVER_MEDIS_H_

#include <functional>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

#include "parser.h"

#include "base/base.h"

class Medis {
 public:
  Medis() = default;
  Medis(const Medis& rhs) = delete;
  Medis(Medis&& rhs) = delete;
  Medis& operator=(const Medis& rhs) = delete;
  Medis& operator=(Medis&& rhs) = delete;
  ~Medis() = default;

  void handle(const std::string& in, std::string& out);

 private:
  Parser _parser;
};

#endif  // MEDIS_SERVER_MEDIS_H_
