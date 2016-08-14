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
  tcp_socket.connect();

  int count = 0;
  while (true) {
    ++count;

    tcp_socket.send("*3\r\n$5\r\nlpush\r\n$7\r\nmy_list\r\n$2\r\n12\r\n");
    string out;
    tcp_socket.recv(out);
    // cout << out << endl;

    if (count % 10000 == 0) {
      cout << count << endl;
    }
  }

  return 0;
}
