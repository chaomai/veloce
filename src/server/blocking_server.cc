#include "blocking_server.h"

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

BlockingServer::BlockingServer(const string& addr, uint16_t port)
    : _tcp_socket(addr, port) {}

void BlockingServer::serve() {
  _tcp_socket.bind();
  _tcp_socket.listen();

  while (true) {
    TcpSocket acc_sock = _tcp_socket.accept();

    while (true) {
      string buf;
      auto recv_sz = acc_sock.recv(buf);
      if (recv_sz == 0) {
        break;
      }

      string out;
      _medis.handle(buf, out);
      acc_sock.send(out);
    }
  }
}
