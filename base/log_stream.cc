#include "log_stream.h"

#include <algorithm>

#include <assert.h>

namespace yang {
namespace tools {

template<typename T>
size_t convert(char buf[], T value)
{
    static const char digitsMap[] = "9876543210123456789";
    static const char* zero = digitsMap + 9;
    static_assert(sizeof digitsMap == 20, "digit size != 20");

    T i = value;
    char* p = buf;

    do
    {
        int lsd = static_cast<int>(i % 10);
        i /= 10;
        *p++ = zero[lsd];
    } while (i != 0);

    if (value < 0)
    {
        *p++ = '-';
    }
    *p = '\0';
    std::reverse(buf, p);

    return p - buf;
}

size_t convertHex(char buf[], uintptr_t value)
{
    static const char digitsHexMap[] = "0123456789ABCDEF";
    static_assert(sizeof digitsHexMap == 17, "digitsHex != 17");

    uintptr_t i = value;
    char* p = buf;

    do
    {
        int lsd = static_cast<int>(i % 16);
        i /= 16;
        *p++ = digitsHexMap[lsd];
    } while (i != 0);

    *p = '\0';
    std::reverse(buf, p);

    return p - buf;
}

// Explicit instantiations
template class FixedBuffer<kSmallBufferSize>;
template class FixedBuffer<kLargeBufferSize>;
}
}