#include "blocking_server.h"

#include <iostream>
#include <string>
#include <thread>
#include <utility>

#include "client_info.h"

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
    ClientInfo client_info(_tcp_socket.accept());

    thread t(
        [this](ClientInfo info) {
          while (true) {
            auto recv_sz = info._client_sock.recv(info._in);
            if (recv_sz == 0) {
              break;
            }

            _medis.handle(info);
            info._client_sock.send(info._out);
          }
        },
        std::move(client_info));
    t.detach();
  }
}
