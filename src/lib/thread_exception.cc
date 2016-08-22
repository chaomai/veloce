#include "thread_exception.h"

const char* ThreadException::what() const noexcept { return _s.c_str(); }
