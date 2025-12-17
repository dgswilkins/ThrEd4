#ifndef THRED_THRED_FILE_H
#define THRED_THRED_FILE_H

// Local Headers
#include "globals.h"

// Windows Header Files:
#include <windef.h>

// Standard Libraries
#include <filesystem>

namespace thredFile {
auto readTHRFile(std::filesystem::path const& newFileName,
                 THR_HEAD_EX&                 extendedHeader,
                 COLORREF&                    backgroundColor,
                 std::array<COLORREF, COLORCNT>& customThreadColor,
                 std::array<wchar_t, COLORCNT>&  threadSizeArray) -> bool;
} // namespace thredFile

#endif // THRED_THRED_FILE_H
