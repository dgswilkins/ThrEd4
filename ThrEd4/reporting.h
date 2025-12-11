#ifndef THRED_REPORTING_H
#define THRED_REPORTING_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// Local Headers

// Standard Libraries

namespace rpt {

void reportError([[maybe_unused]] const wchar_t* prompt, DWORD& errorCode);
} // namespace rpt

#endif // THRED_REPORTING_H
