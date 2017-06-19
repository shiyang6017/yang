#ifndef _THREAD_H
#define _THREAD_H

#include <pthread.h>
#include <unistd.h>

#include <string>
#include <functional>
#include <atomic>
#include <memory>
#include <cassert>

#include <boost/noncopyable.hpp>


namespace yang {

class Thread : public boost::noncopyable {

public:
    using ThreadFunc = std::function<void()>; // c++11

    Thread(const ThreadFunc&, const std::string&);
    Thread(ThreadFunc&&, const std::string&);
    ~Thread();
    
    void run();
  //  bool stop();
    void join();

    bool started() { return started_; }

    const std::string& name() const { return stringName_; }

    pid_t getTid() const { return *tid_; }

    static int threadsCreated() { return threadsCreated_; }

private:
    std::shared_ptr<pid_t> tid_;
    pthread_t pthreadId_;
    std::string stringName_;    
    bool started_;
    bool joined_;

    ThreadFunc func_;

    static std::atomic<unsigned int> threadsCreated_; 
private:
    void init();
};

}
#endif