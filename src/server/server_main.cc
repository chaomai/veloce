#include <cstdint>
#include <string>

#include "blocking_server.h"

using std::string;

int main(int argc, const char *argv[]) {
  string addr("127.0.0.1");
  uint16_t port = 6789;

  BlockingServer blocking_server(addr, port);
  blocking_server.serve();

  return 0;
}
