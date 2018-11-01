#ifndef XDB_TYPES_HPP
#define XDB_TYPES_HPP

#include <cstdint>

namespace xdb {

class NullType {} ;

// Wraps pointer to buffer and its size. Memory management is external
class Blob {
public:

    Blob() = default ;
    Blob(const char *data, uint32_t sz): size_(sz), data_(data) {}

    const char *data() const { return data_ ; }
    uint32_t size() const { return size_ ; }

private:
    const char *data_ = nullptr;
    uint32_t size_ = 0 ;
};


} // namespace xdb


#endif
