#include "time_stamp.h"
#include <iostream>
using namespace yang;

int main() {
    TimeStamp timeStamp(TimeStamp::current());
    std::cout << timeStamp.toFormattedString() << std::endl;
    bool showUsec = false;
    std::cout << timeStamp.toFormattedString(showUsec) << std::endl;
}