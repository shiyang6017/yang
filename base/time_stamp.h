#ifndef TIME_STAMP_H
#define TIME_STAMP_H

#include <stdint.h>
#include <boost/operators.hpp>
#include <string>

namespace yang {

class TimeStamp : public boost::less_than_comparable<TimeStamp> {
    const static unsigned int UsecsPerSecond = 1000 * 1000;

public:
    TimeStamp():usecSinceEpoch_(0) {}

    explicit TimeStamp(int64_t arg) : usecSinceEpoch_(arg){}
   
    std::string toFormattedString(bool showUsec = true) const;
    
    int64_t getUsecSinceEpoch() const {
        return usecSinceEpoch_;
    }

    static TimeStamp current();

    static TimeStamp FormTimeStamp(time_t t) {
        return FormTimeStamp(t, 0);
    }

    static TimeStamp FormTimeStamp(time_t t, unsigned int usec) {
        int64_t arg = t * UsecsPerSecond + usec;
        return TimeStamp(arg);
    }

private: 
    int64_t usecSinceEpoch_;

};

inline bool operator < (const TimeStamp& lhs, const TimeStamp& rhs) {
    return lhs.getUsecSinceEpoch() < rhs.getUsecSinceEpoch();
}
inline bool operator == (const TimeStamp& lhs, const TimeStamp& rhs) {
    return lhs.getUsecSinceEpoch() == rhs.getUsecSinceEpoch();
}

}
#endif