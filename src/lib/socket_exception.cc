#include "socket_exception.h"

const char* SocketException::what() const noexcept { return _s.c_str(); }
