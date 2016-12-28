
#ifndef PROJECT_THREAD_H
#define PROJECT_THREAD_H

// -------------- INCLUDES
#include <thread>
#include "../config.h"

/**
 * TODO
 * Currently supporting plain threads
 * Will support future and async threads later.
 */
class Threading {

    bool daemon;
    std::thread t;

    // - Name of this thread
    const char *name;
    // - If it is already running
    bool threadsRunning = false;

public:
    Threading(const char *name = NULL, bool daemon = true);
    template<class F, class... Arg>
    bool startThread(F &&f, Arg &&... args);
    void join();
    void setDone();

};


/**
 * Runs a thread on a function. Handles errors.
 * @param f : a pointer (memory adress) to a function typically Class::Member-function
 * @param args : a pointer to the class with the member function
 */
template<class F, class... Arg>
bool Threading::startThread(F &&f, Arg &&... args ) {

    if (this->threadsRunning) {
        return false;
    };

    this->threadsRunning = true;
    t = std::thread(std::forward<F>(f), args...);
    if(daemon) t.detach();
    return true;
}


#endif //PROJECT_THREAD_H
