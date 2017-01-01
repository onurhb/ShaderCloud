// -------------- INCLUDES
#include <iostream>
#include "NotificationHandler.h"

void NotificationHandler::setNotification(std::string notification, double time) {

    this->notification = notification;
    this->time = time;

}

std::string NotificationHandler::getNotification() {
    end = start;
    start = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = start - end;

    if (this->time > 0) {
        this->time -= elapsed_seconds.count();
        if (this->time < 0) {
            this->time = 0;
            this->notification = "";
        }
    }

    return this->notification;
}
