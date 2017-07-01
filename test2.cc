#include <pthread.h>
#include <iostream>
using namespace std;

class Example {
public:
    Example() {
        pthread_mutex_init(&mutex_, NULL);
    }
    void fun() const {
        pthread_mutex_lock(&mutex_);
        cout << "hello" << endl;
        pthread_mutex_unlock(&mutex_);
    }
private:
     pthread_mutex_t mutex_;
};

int main() {
    const Example e;
    e.fun();
    return 0; 
}