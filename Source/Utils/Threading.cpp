
// -------------- MACROS

// -------------- INCLUDES
#include "Threading.h"


Threading::Threading(const char* name, bool daemon) {
    this->name = name;
    this->daemon = daemon;
}

/**
 * Should be called when done.
 * Else it won't be possible to start the thread again.
 * This is for preventing starting same thread over and over again.
 */
void Threading::setDone() {
    this->threadsRunning = false;
#if DEBUG_INFO
    printf("Thread: Shutting down %s.\n", this->name);
#endif
}

void Threading::join() {
    this->threadsRunning = false;
    t.join();
}
