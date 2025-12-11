#ifndef THRED_TRACE_H
#define THRED_TRACE_H

namespace trace {
void blak();
void dutrnum1();
void dutrnum2();
void initColorRef() noexcept;
void initTraceWindows();
void setTracedMapSize(uint32_t bitmapSize);
void trace();
void tracedg();
void traceInit() noexcept;
void traceNumberInput(wchar_t NumericCode);
void traceNumberReset() noexcept(!std::is_same_v<ptrdiff_t, int>);
void tracpar();
void trcsel();
void trdif();
void trinit();
void untrace();
void wasTrace(DRAWITEMSTRUCT const& drawItem);
void wasTrace1();
} // namespace trace

#endif // THRED_TRACE_H
