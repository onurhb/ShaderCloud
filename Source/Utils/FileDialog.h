
#ifndef PROJECT_FILEDIALOG_H
#define PROJECT_FILEDIALOG_H

// -------------- INCLUDES
#include <nfd.h>
#include <string>

enum RESULT {
    DIALOG_ERROR, DIALOG_OK, DIALOG_CANCEL
};

class FileDialog {
    std::string outPath;

public:
    std::string getPath() const;
    RESULT askPath(std::string wildCard);

};


#endif //PROJECT_FILEDIALOG_H
