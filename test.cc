#include <iostream>
#include <string.h>
#include <vector>
#include <memory>

#include <stdio.h>

#include <boost/ptr_container/ptr_vector.hpp>

using std::cout;
using std::endl;
using std::string;

class Stream {
    typedef Stream self; 
public:
    Stream() : cur(buf) {}
    friend std::ostream& operator << (std::ostream& os, Stream& stream) {
        cout << stream.buf; 
        return os;
    }
    template<typename T>
    self& operator<<(T t) {
        int len = sprintf(cur, "%d", t);
        cur += len;
        *cur = 0;
        return *this;
    }
    template<typename T>
    void fun(T t) {
        cout << "typename fun" << endl;
    }
private:

    char buf[1024];
    char* cur;
};

template<>
void Stream::fun<char>(char c) {
    cout << "char c" << endl;
}
int main() {
    Stream stream;
    stream << 124;
    stream << 'c';
    cout << stream << endl;
    stream.fun(1);
    stream.fun('c');
    return 0;
}
