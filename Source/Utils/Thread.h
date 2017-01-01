
#ifndef PROJECT_THREAD_H
#define PROJECT_THREAD_H

// -------------- INCLUDES
#include <thread>
#include "../config.h"

/**
 * Currently supporting plain threads
 * Will support future and async threads later.
 */
class Thread {

    bool daemon;
    std::thread t;
    bool threadsRunning = false;

public:
    Thread(bool daemon = true);
    template<class F, class... Arg>
    bool startThread(F &&f, Arg &&... args);
    void join();
    void setDone();

};


inline Thread::Thread(bool daemon) {
    this->daemon = daemon;
}

/**
 * Runs a thread on a function. Handles errors.
 * @param f : a pointer (memory adress) to a function typically Class::Member-function
 * @param args : a pointer to the class with the member function
 */
template<class F, class... Arg>
bool Thread::startThread(F &&f, Arg &&... args ) {

    if (this->threadsRunning) return false;

    this->threadsRunning = true;
    t = std::thread(std::forward<F>(f), args...);
    if(daemon) t.detach();
    return true;
}

/**
 * Should be called when done.
 * Else it won't be possible to start the thread again.
 * This is for preventing starting same thread over and over again.
 */
inline void Thread::setDone() {
    this->threadsRunning = false;
}

/**
 * @brief If thread is not daemon should be called to block main.
 */
inline void Thread::join() {
    this->threadsRunning = false;
    t.join();
}



#endif //PROJECT_THREAD_H
