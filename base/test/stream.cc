#include <iostream>
using namespace std;

struct MyStream {
    char buf[1024];
    int idx = 0;
    template<typename T>
    MyStream& operator<<(T t) {
        
    }
};