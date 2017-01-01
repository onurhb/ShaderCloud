
#ifndef PROJECT_NOTIFICATIONHANDLER_H
#define PROJECT_NOTIFICATIONHANDLER_H
// -------------- INCLUDES
#include <string>
#include <chrono>

class NotificationHandler {

    std::string notification;
    double time;
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;

public:
    void setNotification(std::string notification, double time = -1);
    std::string getNotification();
};


#endif //PROJECT_NOTIFICATIONHANDLER_H
