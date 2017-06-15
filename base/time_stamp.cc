#include "time_stamp.h"

#include <sys/time.h>
#include <stdio.h>

using namespace yang;

TimeStamp TimeStamp::current() {
    struct timeval tval;
    gettimeofday(&tval, NULL);
    int64_t arg = tval.tv_sec * TimeStamp::UsecsPerSecond + tval.tv_usec;
    return TimeStamp(arg);
}

std::string TimeStamp::toFormattedString(bool showUsec) const {
    char buf[32] = {0};
    struct tm tmTime;


    time_t second = static_cast<time_t> (usecSinceEpoch_ / UsecsPerSecond);
    
    /* thread safe function */

    if (gmtime_r(&second, &tmTime) == NULL) {
        abort();
    }
    if (showUsec) {
        int useconds = static_cast<int>(usecSinceEpoch_ % UsecsPerSecond);
        snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
             tmTime.tm_year + 1900, tmTime.tm_mon + 1, tmTime.tm_mday,
             tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec,
             useconds);
    } else {
          snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
             tmTime.tm_year + 1900, tmTime.tm_mon + 1, tmTime.tm_mday,
             tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec);
    }
    return buf;
}
