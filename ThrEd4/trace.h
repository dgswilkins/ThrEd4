#pragma once

namespace trace {
void blak();
void dutrnum1();
void dutrnum2();
void initColorRef() noexcept;
void initTraceWindows();
void trace();
void tracedg();
void traceNumberInput(wchar_t NumericCode);
void traceNumberReset() noexcept(!(std::is_same_v<ptrdiff_t, int>));
void tracpar();
void trcsel();
void trdif();
void trinit();
void untrace();
void wasTrace();
void wasTrace1();
} // namespace trace
