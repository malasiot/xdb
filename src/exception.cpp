#include <xdb/exception.hpp>

using namespace std ;

namespace xdb {

Exception::Exception(const std::string &msg): std::runtime_error(msg) {}

} // namespace xdb

