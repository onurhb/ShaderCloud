
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
    std::string askPath(std::string wildCard);

};


/**
 * Path to file
 * @return path : absolute (C:/ ...) path to last file succesfully choosen
 */
inline std::string FileDialog::getPath() const {
    return outPath;
}


/**
 * Asks for a path using native dialog
 * @return result of the dialog
 */
inline std::string FileDialog::askPath(std::string wildCard) {
    nfdchar_t *p;
    nfdresult_t nfdResult;
    nfdResult = NFD_OpenDialog(wildCard.c_str(), NULL, &p);

    if (nfdResult == NFD_OKAY){
        outPath = std::string(p);
        delete[] p;
        return outPath;
    }

    return "";
}


#endif //PROJECT_FILEDIALOG_H
