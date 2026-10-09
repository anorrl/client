#pragma once

#include <cstdint>
#include <cstddef>

uintptr_t elfDynamicBaseAddress();
size_t elfTextSize();
uintptr_t staticElfTextVAddr();
intptr_t imageSlideElf();