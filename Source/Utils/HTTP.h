#ifndef PROJECT_HTTP_H
#define PROJECT_HTTP_H
// -------------- MACROS

// -------------- INCLUDES
#include <curl/curl.h>
#include "string"
#include "../config.h"

enum HTTP_STATUS {HTTP_OK, HTTP_REJECTED, HTTP_REQUESTING};

class HTTP {

    CURL *curl;
    std::string response;
    HTTP_STATUS status;

private:
    friend size_t callBack(void *contents, size_t size, size_t nmemb, std::string *s);
public:
    HTTP();
    ~HTTP();
    HTTP_STATUS get(std::string URL, bool follow = false);
    std::string getResponse() const;

    // ----------------------------- OTHER
    std::string getSoundCloudStream(std::string URL);
};


#endif //PROJECT_HTTP_H
