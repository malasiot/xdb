#include <wspp/database/exception.hpp>

using namespace std ;

namespace cvx { namespace db {

Exception::Exception(const std::string &msg): std::runtime_error(msg) {}

} // namespace db
} // namespace wspp
