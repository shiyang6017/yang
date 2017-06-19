#ifndef _CURRENT_THREAD_H
#define _CURRENT_THREAD_H

#include <type_traits>
#include <cassert>

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>

namespace yang {

namespace CurrentThread
{
extern __thread int currentTid;
extern __thread char tidString[32];
extern __thread int tidStringLength;
extern __thread const char* ptrThreadName;

inline pid_t gettid() {
    return static_cast<pid_t>(syscall(SYS_gettid));
}

inline void saveTidInfos() {
    if (currentTid == 0) {
        currentTid = gettid();
        tidStringLength = 
            snprintf(tidString, sizeof tidString, "%5d ", currentTid);
    }
}

inline int getCurrentTid() {
    if (__builtin_expect(currentTid == 0, 0)) { // unlikely(currentTid == 0)
        saveTidInfos();
    }
    
    return currentTid;
}

inline const char* getTidString() {
    return tidString;
}

inline int getTidStringLength() {
    return tidStringLength;
}

inline const char* getPtrThreadName() {
    return ptrThreadName;
}

}
}
#endif