#include <iostream>
using namespace std;



template<typename T>
void specialFunc(const T& t) {
    cout << t << endl;
}
template <typename... Args>
void templateFunc(Args... args) {
}
template<typename T, typename ... Args>
void templateFunc(const T t, Args ... args) {
    specialFunc(t);
    templateFunc(args...);
}

int main() {
    templateFunc(1, 2, 3, 'a', 'b', 'c');
    return 0;
}