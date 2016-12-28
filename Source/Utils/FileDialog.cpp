
// -------------- INCLUDES
#include "FileDialog.h"

/**
 * Path to file
 * @return path : absolute (C:/ ...) path to last file succesfully choosen
 */
std::string FileDialog::getPath() const {
    return outPath;
}


/**
 * Asks for a path using native dialog
 * @return result of the dialog
 */
RESULT FileDialog::askPath(std::string wildCard) {
    nfdchar_t *p;
    nfdresult_t nfdResult;
    nfdResult = NFD_OpenDialog(wildCard.c_str(), NULL, &p);
    if (nfdResult == NFD_OKAY){
        outPath = std::string(p);
        delete[] p;
        return DIALOG_OK;
    }
    else if (nfdResult == NFD_CANCEL) return DIALOG_CANCEL;
    return DIALOG_ERROR;
}