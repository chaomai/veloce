#include "tcp_socket.h"

#include <arpa/inet.h>   // for htons, inet_addr
#include <sys/socket.h>  // for socket
#include <unistd.h>      // for cloese

#include <cstdint>
#include <string>
#include <utility>

#include "base/base.h"
#include "socket_exception.h"

using std::string;

TcpSocket::TcpSocket(const string& addr, uint16_t port) {
  _sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (!is_sockfd_valid()) {
    throw SocketException("create socket error");
  }

  _addr.sin_family = AF_INET;
  _addr.sin_addr.s_addr = inet_addr(addr.c_str());
  _addr.sin_port = htons(port);
}

TcpSocket::TcpSocket(TcpSocket&& rhs) noexcept : _sockfd(rhs._sockfd),
                                                 _addr(rhs._addr) {
  rhs._sockfd = -1;
  rhs._addr = {};
}

TcpSocket& TcpSocket::operator=(TcpSocket&& rhs) noexcept {
  if (this != &rhs) {
    _sockfd = rhs._sockfd;
    _addr = rhs._addr;

    rhs._sockfd = -1;
    rhs._addr = {};
  }
  return *this;
}

TcpSocket::~TcpSocket() {
  if (is_sockfd_valid()) {
    ::close(_sockfd);
  }
}

void TcpSocket::bind() {
  // bind has same name with bind in socket.h
  if (::bind(get_sockfd(), reinterpret_cast<sockaddr*>(&_addr),
             sizeof(sockaddr_in)) == -1) {
    throw SocketException("bind socket error");
  }
}

void TcpSocket::listen(int backlog) {
  if (::listen(get_sockfd(), backlog) == -1) {
    throw SocketException("listen socket error");
  }
}

TcpSocket TcpSocket::accept() {
  sockaddr_in acc_addr;
  socklen_t acc_addrlen;
  int acc_sockfd = ::accept(
      get_sockfd(), reinterpret_cast<sockaddr*>(&acc_addr), &acc_addrlen);

  if (acc_sockfd == -1) {
    throw SocketException("accept socket error");
  }

  return TcpSocket(acc_sockfd, acc_addr);
}

void TcpSocket::connect() {
  if (::connect(get_sockfd(), reinterpret_cast<sockaddr*>(&_addr),
                sizeof(sockaddr_in)) == -1) {
    throw SocketException("connect socket error");
  }
}

ssize_t TcpSocket::send(const string& in, int flags) {
  ssize_t send_sz =
      ::send(get_sockfd(), reinterpret_cast<const void*>(in.c_str()), in.size(),
             flags);

  if (send_sz == -1) {
    throw SocketException("send through socket error");
  }

  return send_sz;
}

ssize_t TcpSocket::recv(string& out, int flags) {
  char tmp[BUF_SIZE] = {};
  ssize_t recv_sz =
      ::recv(get_sockfd(), reinterpret_cast<void*>(tmp), BUF_SIZE, flags);

  if (recv_sz == -1) {
    throw SocketException(
        (string("receive through socket error: ") + get_errno_str()).c_str());
  }

  string tmp_str(tmp);
  out = std::move(tmp_str);

  return recv_sz;
}

int TcpSocket::get_sockfd() {
  if (is_sockfd_valid()) {
    return _sockfd;
  } else {
    throw SocketException("socket is invalid");
  }
}
