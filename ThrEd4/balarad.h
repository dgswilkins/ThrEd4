#ifndef THRED_BALARAD_H
#define THRED_BALARAD_H

// Standard Libraries
#include <filesystem>

namespace fs = std::filesystem;

namespace bal {
void balInit() noexcept;
void redbal();
void ritbal();
auto getBN0() noexcept -> fs::path&;
auto getBN1() noexcept -> fs::path&;
auto getBN2() noexcept -> fs::path&;
} // namespace bal

#endif // THRED_BALARAD_H
