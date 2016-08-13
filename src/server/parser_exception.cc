#include "parser_exception.h"

const char* ParserException::what() const noexcept { return _s.c_str(); }
