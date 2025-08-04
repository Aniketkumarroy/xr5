#ifndef TYPES_H_
#define TYPES_H_

#include "numbers.h"
#include "xr5/config.h"
#include <cstdint>
#include <cstring>

namespace xr5 {
namespace types {

using Tick = uint64_t;
using Address64 = uint64_t;
using Address32 = uint32_t;
using Byte = uint8_t;
using TimeStamp = double;

#if XR5_ADDR_WIDTH == 64
using Address = Address64;
#elif XR5_ADDR_WIDTH == 32
using Address = Address32;
#else
#error "Unsupported address width"
#endif

} // namespace types
} // namespace xr5

#endif