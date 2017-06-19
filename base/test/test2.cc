#include <assert.h>
#include <iostream>
#include <time.h>
using namespace std;


int main() {
    struct timespec timeSpec;
    
    clock_gettime(CLOCK_REALTIME, &timeSpec);
    cout << timeSpec.tv_sec << " " << timeSpec.tv_nsec << endl;

    clock_gettime(CLOCK_MONOTONIC, &timeSpec);
    cout << timeSpec.tv_sec << " " << timeSpec.tv_nsec << endl;

    return 0;
}