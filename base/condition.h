#ifndef _CONDITION_H
#define _CONDITION_H


#include <pthread.h>
#include <time.h>
#include <assert.h>

#include <boost/noncopyable.hpp>

#include "mutex.h"



namespace yang {

class Condition : public boost::noncopyable {
public:
    explicit Condition(Mutex& mutexArg) : mutex_(mutexArg) {
        PCHECK( pthread_cond_init(&cond_, NULL) );
    }

    ~Condition() {
        PCHECK( pthread_cond_destroy(&cond_) );
    }

    void notify() {
        PCHECK( pthread_cond_signal(&cond_) );
    }

    void notifyAll() {
        PCHECK( pthread_cond_broadcast(&cond_) );
    }

    void wait() {
        PCHECK( pthread_cond_wait(&cond_, mutex_.getMutexPtr()) );
    }

    void waitForSecond(time_t second);
    void waitForUsecond(long uSecond);

private:
    Mutex& mutex_;
    pthread_cond_t cond_;
};
}
#endif