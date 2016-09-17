#include "client_info.h"

#include <utility>

#include "lib/tcp_socket.h"

ClientInfo::ClientInfo(TcpSocket&& client_sock)
    : _client_sock(std::move(client_sock)), _current_db_num(0) {}

ClientInfo::ClientInfo(ClientInfo&& rhs) noexcept
    : _client_sock(std::move(rhs._client_sock)),
      _current_db_num(rhs._current_db_num),
      _in(std::move(rhs._in)),
      _out(std::move(rhs._out)) {}

ClientInfo& ClientInfo::operator=(ClientInfo&& rhs) noexcept {
  _client_sock = std::move(rhs._client_sock);
  _current_db_num = rhs._current_db_num;
  _in = std::move(rhs._in);
  _out = std::move(rhs._out);
}
