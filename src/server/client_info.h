#ifndef MEDIS_SERVER_CLIENT_INFO_H_
#define MEDIS_SERVER_CLIENT_INFO_H_

#include <cstddef>  // for size_t
#include <string>

#include "lib/tcp_socket.h"

struct ClientInfo {
  ClientInfo(TcpSocket&& client_sock);
  ClientInfo(const ClientInfo& rhs) = delete;
  ClientInfo(ClientInfo&& rhs) noexcept;
  ClientInfo& operator=(const ClientInfo& rhs) = delete;
  ClientInfo& operator=(ClientInfo&& rhs) noexcept;
  ~ClientInfo() = default;

  TcpSocket _client_sock;
  std::size_t _current_db_num;
  std::string _in;
  std::string _out;
};

#endif  // MEDIS_SERVER_CLIENT_INFO_H_
