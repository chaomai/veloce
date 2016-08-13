#include <string>

#include <cstdint>

#include "lib/tcp_socket.h"

using std::string;

int main(int argc, const char *argv[]) {
  string addr("127.0.0.1");
  uint16_t port = 6789;

  TcpSocket tcp_socket(addr, port);
  tcp_socket.connect();
  tcp_socket.send("hello");

  return 0;
}
