#ifndef MEDIS_LIB_TCP_SOCKET_H_
#define MEDIS_LIB_TCP_SOCKET_H_

#include <netinet/in.h>  // for sockaddr_in
#include <sys/socket.h>  // for socket

#include <string>

class TcpSocket {
 public:
  TcpSocket(int sockfd, sockaddr_in addr) : _sockfd(sockfd), _addr(addr) {}
  TcpSocket(const std::string& addr, uint16_t port);
  TcpSocket(const TcpSocket& rhs) = delete;
  TcpSocket(TcpSocket&& rhs) noexcept;
  TcpSocket& operator=(const TcpSocket& rhs) = delete;
  TcpSocket& operator=(TcpSocket&& rhs) noexcept;
  ~TcpSocket();

  void bind();
  void listen(int backlog = 5);
  TcpSocket accept();
  void connect();
  ssize_t send(const std::string& in, int flags = 0);
  ssize_t recv(std::string& out, int flags = 0);

 private:
  int _sockfd = -1;
  sockaddr_in _addr;

  bool is_sockfd_valid() const { return _sockfd != -1; }
  inline int get_sockfd();
};

#endif  // MEDIS_LIB_TCP_SOCKET_H_
