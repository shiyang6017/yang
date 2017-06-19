#ifndef _MUTEX_H
#define _MUTEX_H

#include "utility.h"

#include <boost/noncopyable.hpp>

#include <pthread.h>
#include <time.h>

namespace yang {

class Mutex : public boost::noncopyable {
    friend class MutexGuard;
    friend class Condition;
public:
    Mutex() {
        PCHECK(pthread_mutex_init(&mutex_, NULL));
    }
    //  Mutex(const Mutex&) = delete;
    //  const Mutex& operator=(const Mutex&) = delete;

    ~Mutex() {
        PCHECK(pthread_mutex_destroy(&mutex_));
    }

private:
    void lock() {
        PCHECK(pthread_mutex_lock(&mutex_));
    }

    void unlock() {
        PCHECK(pthread_mutex_unlock(&mutex_));
    }

    pthread_mutex_t* getMutexPtr() {
        return &mutex_;
    }

private:
    pthread_mutex_t mutex_;
};

class MutexGuard : public boost::noncopyable {
public:
    explicit MutexGuard(Mutex& mutexArg) : mutex_(mutexArg) {
        mutex_.lock();
    }

    ~MutexGuard() {
        mutex_.unlock();
    }

private:
    Mutex& mutex_;
};

}
#endif