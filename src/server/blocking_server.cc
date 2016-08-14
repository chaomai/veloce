#include "blocking_server.h"

#include <iostream>
#include <string>
#include <thread>
#include <utility>

using std::cout;
using std::endl;
using std::string;
using std::thread;

BlockingServer::BlockingServer(const string& addr, uint16_t port)
    : _tcp_socket(addr, port) {}

void BlockingServer::serve() {
  _tcp_socket.bind();
  _tcp_socket.listen();

  while (true) {
    TcpSocket acc_sock = _tcp_socket.accept();

    thread t(
        [this](TcpSocket sock) {
          while (true) {
            string buf;
            auto recv_sz = sock.recv(buf);
            if (recv_sz == 0) {
              break;
            }

            string out;
            _medis.handle(buf, out);
            sock.send(out);
          }
        },
        std::move(acc_sock));
    t.detach();
  }
}
