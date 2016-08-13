#include <cstdint>
#include <iostream>
#include <string>

#include "lib/tcp_socket.h"

using std::cout;
using std::endl;
using std::string;

int main(int argc, const char *argv[]) {
  string addr("127.0.0.1");
  uint16_t port = 6789;

  TcpSocket tcp_socket(addr, port);
  tcp_socket.bind();
  tcp_socket.listen();
  auto ret = tcp_socket.accept();

  string buf;

  cout << ret.recv(buf) << endl;
  ret.send("done");

  return 0;
}
