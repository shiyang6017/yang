#ifndef LOG_STREAM_H
#define LOG_STREAM_H

#include <boost/noncopyable.hpp>
#include <boost/implicit_cast.hpp>
#include <string>

#include <string.h>
namespace yang {

namespace tools { 
const int kSmallBufferSize = 4096;
const int kLargeBufferSize = 4096 * 1024;

template<int SIZE>
class FixedBuffer : boost::noncopyable
{
public:
    FixedBuffer() : cur_(data_) {
        setCookie(cookieStart);
    }
    ~FixedBuffer() {
        setCookie(cookieEnd);
    }

    void append(const char*  buf, size_t len) {
        // why use implicit_cast
        if (boost::implicit_cast<size_t>(avail()) > len) {
            memcpy(cur_, buf, len);
            cur_ += len;
        }
    }
    int length() const {
        return static_cast<int>(cur_ - data_);
    }
    int avail() const {
        return static_cast<int>(end() - cur_);
    }
    void reset() {
        cur_ = data_;
    }
    void bzero() {
        ::bzero(data_, sizeof data_);
    }

    const char* data() const {
        return data_;
    }
    char* current() {
        return cur_;
    }
    void add(size_t len) {
        cur_ += len;
    }

    const char* debugString();

    void setCookie(void (*cookie)()) {
        cookie_ = cookie;
    }

    std::string toString() const {
        return std::string(data_, length());
    }
private:
    const char* end() const {
        return data_ + sizeof data_;
    }

    static void cookieStart();
    static void cookieEnd();

    void (*cookie_)();
    char data_[SIZE];
    char* cur_;
};

}

class LogStream : boost::noncopyable {
    typedef LogStream self;
public:
    typedef tools::FixedBuffer<tools::kSmallBufferSize> Buffer;
    

    template<typename T>
    self& operator <<(T t);
    

    self& operator << (short);
    self& operator << (unsigned short);
    self& operator << (int);
    self& operator << (unsigned int);
    self& operator << (long);
    self& operator << (unsigned long);
    self& operator << (long long);
    self& operator << (unsigned long long);

    self& operator << (const void*);

    self& operator << (float v) {
        *this << static_cast<double>(v);
        return *this;
    }
    self& operator << (double);

    self& operator << (char v) {
        buffer_.append(&v, 1);
        return *this;
    }

    self& operator << (const char* str) {
        if (str) {
            buffer_.append(str, strlen(str));
        } else {
            buffer_.append("(null)", 6);
        }

        return *this;
    }

    self& operator << (const unsigned char* str) {
        return operator << (reinterpret_cast<const char*>(str));
    }

    self& operator << (const std::string& v) {
        buffer_.append(v.c_str(), v.size());
        return *this;
    }

    self& operator << (const Buffer& v)
    {
        *this << v.toString();
        return *this;
    }

    void append(const char* data, int len) {
        buffer_.append(data, len);
    }
    const Buffer& buffer() const {
        return buffer_;
    }
    void resetBuffer() {
        buffer_.reset();
    }

private:
    void staticCheck();
    template<typename T>
    void formatInteger(T);
    Buffer buffer_;

    static const int kMaxNumericSize = 32;
};



class Fmt {
public:
    template<typename T>
    Fmt(const char* fmt, T val);

    const char* data() const {
        return buf_;
    }
    int length() const {
        return length_;
    }

private:
    char buf_[32];
    int length_;
};

inline LogStream& operator<<(LogStream& s, const Fmt& fmt) {
    s.append(fmt.data(), fmt.length());
    return s;
}

}

#endif