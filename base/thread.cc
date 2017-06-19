#include "utility.h"
#include "thread.h"
#include "exception.h"
#include "current_thread.h"

#include <string>
#include <memory>

#include <assert.h>
#include <unistd.h>
#include <pthread.h>

namespace yang {

namespace CurrentThread
{
__thread int currentTid = 0;
__thread char tidString[32];
__thread int tidStringLength = 6;

// ptrThreadName will point to a place which has already allocated, 
// hence it will not cause memory leakage 
__thread const char* ptrThreadName = "unknown";

const bool sameType = std::is_same<int, pid_t>::value;
static_assert(sameType, "pid_t is not int");
}


class SetNameForMainThread {
public:
     SetNameForMainThread() {
         using namespace CurrentThread; 
         currentTid =  CurrentThread::getCurrentTid();
         ptrThreadName = "MainThread"; 
     }
};

SetNameForMainThread setNameForMainThread;

struct ThreadInfo { // use "struct"

    using ThreadFunc = yang::Thread::ThreadFunc;

    ThreadFunc func_;
    std::weak_ptr<pid_t> weakTid_;
    std::string stringName_;

    ThreadInfo(const std::string& name,
               const std::shared_ptr<pid_t>& tid,
               const ThreadFunc& func)
        : stringName_(name), weakTid_(tid), func_(func) {}


    // main content: 
    //      1. set TSD for a thread : currentTid,  tidString, etc
    //      2. run main function
    //      3. handle exception

    void runInThread() {
          
        // set currentTid,  tidString, tidStringLength in getCurrentTid()
        pid_t tid = CurrentThread::getCurrentTid();
        {
            auto sharedTid = weakTid_.lock();
            if (sharedTid) {
                *sharedTid = tid;
            }
        }
        CurrentThread::ptrThreadName =
            stringName_.empty() ? CurrentThread::tidString : stringName_.c_str();

        try {
            func_();
        }
        catch (Exception& ex) {
            //TODO, Handle Exception
            assert(false);
        }
        catch (std::exception& ex) {
            //TODO, Handle exception
            assert(false);
        }

        catch(...) {
            //TODO
            assert(false);
            throw;
        }
    }

};

// called by pthread_create
void* funcInPthreadCreate(void* arg) {
    
    ThreadInfo* threadInfoPtr = static_cast<ThreadInfo*>(arg);
    
    assert(threadInfoPtr);

    //std::unique_ptr<ThreadInfo> uptr(threadInfoPtr); this is better ??
    
    threadInfoPtr->runInThread();
    
    delete threadInfoPtr;

    return NULL;
}

}


using namespace yang;

std::atomic<unsigned int> Thread::threadsCreated_(0);

void Thread::init() {
    int num = ++threadsCreated_;
    if (stringName_.empty()) {
        char buf[32];
        snprintf(buf, sizeof buf, "Thread_%d", num);
        stringName_ = buf;
    }
}

Thread::Thread(const ThreadFunc& func, const std::string& name) :
    started_(false),
    joined_(false),
    func_(func),
    pthreadId_(0),
    tid_(new pid_t(0)),
    stringName_(name) {
    init();
}

Thread::Thread(ThreadFunc&& func,  const std::string& name) :
    started_(false),
    joined_(false),
    func_(std::move(func)),
    pthreadId_(0),
    tid_(new pid_t(0)),
    stringName_(name) {
    init();
}
/*

// stupid way to use smart pointer ....  
// fully understand why muduo uses raw pointer!!!

void Thread::run() {
    assert(!started_);
    started_ = true;
    std::unique_ptr<ThreadInfo> 
        threadInfo(new ThreadInfo(stringName_, tid_, func_));

    PCHECK( pthread_create(&pthreadId_, 
                            NULL, 
                            funcInPthreadCreate, 
                            threadInfo.get()) 
          );
}

*/

void Thread::run() {
    assert(!started_);
    
    started_ = true;
    ThreadInfo* info = new ThreadInfo(stringName_, tid_, func_);
    
    assert(info);
    int err = pthread_create(&pthreadId_, 
                            NULL, 
                            funcInPthreadCreate, 
                            info);
    if (err) {
        started_ = false;
        delete info;
        PCHECK(err);
    } 
}



// TODO, if a thread call this fun by itself, what will happen logically?

// race condition ?
// man page :
//       If multiple threads simultaneously try to join with  the  same  thread,
//       the  results  are  undefined.

void Thread::join() {

    assert(started_);
    assert(!joined_);
    joined_ = true;
    PCHECK( pthread_join(pthreadId_, NULL) );
}

Thread::~Thread() {
    if (started_ && !joined_)
    {
        //when does it happen ?
        pthread_detach(pthreadId_);
    } else {
        throw Exception("started_ == false || joined_ == true");
    }
}

/*
bool Thread::stop() {
    assert(0);
}
*/
