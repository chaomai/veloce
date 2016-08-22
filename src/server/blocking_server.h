#ifndef MEDIS_SRC_BLOCKING_SERVER_H_
#define MEDIS_SRC_BLOCKING_SERVER_H_

#include <cstdint>
#include <string>

#include "lib/tcp_socket.h"
#include "medis.h"

class BlockingServer {
 public:
  BlockingServer(const std::string &addr, uint16_t port);
  BlockingServer(const BlockingServer &rhs) = delete;
  BlockingServer(BlockingServer &&rhs) = delete;
  BlockingServer &operator=(const BlockingServer &rhs) = delete;
  BlockingServer &operator=(BlockingServer &&rhs) = delete;
  ~BlockingServer() = default;

  void serve();

 private:
  TcpSocket _tcp_socket;

  // _medis is shared among multiple threads.
  Medis _medis;
};

#endif  // MEDIS_SRC_BLOCKING_SERVER_H_
