#include "condition.h"

#include <assert.h>
#include <errno.h>

using namespace yang;

void Condition::waitForSecond(time_t second) {
    struct timespec realTime = {0};
    clock_gettime(CLOCK_REALTIME, &realTime);
    realTime.tv_sec += second;
    
    int err = pthread_cond_timedwait(&cond_, mutex_.getMutexPtr(), &realTime);
    if (err != ETIMEDOUT && err != 0) { 
        PCHECK(err); 
    }
}

void Condition::waitForUsecond(long uSecond) {
    assert(0);
}