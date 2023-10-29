#pragma once

// Standard Libraries
#include <filesystem>

namespace fs = std::filesystem;

namespace bal {
void redbal();
void ritbal();
auto getBN0() noexcept -> fs::path*;
auto getBN1() noexcept -> fs::path*;
auto getBN2() noexcept -> fs::path*;
void setBN0(fs::path* name) noexcept;
void setBN1(fs::path* name) noexcept;
void setBN2(fs::path* name) noexcept;
} // namespace bal
