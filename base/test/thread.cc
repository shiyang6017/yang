#include "condition.h"
#include "thread.h"
#include "current_thread.h"
#include <iostream>
#include <atomic>
#include <functional>

#include <pthread.h>

using namespace yang;
using namespace yang::CurrentThread;


void fun() {
   std::cout << currentTid << std::endl;
   std::cout << tidString << std::endl; 
   std::cout << tidStringLength << std::endl;
   std::cout << ptrThreadName << std:: endl;
}

int main() {
    
    fun(); 

    Thread thread(std::bind(fun), "");

    std::function<void()> p = std::bind(fun);

    thread.run();

    sleep(1);
    return 0;
}