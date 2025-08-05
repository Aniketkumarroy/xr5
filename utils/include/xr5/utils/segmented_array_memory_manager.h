#ifndef SEGMENTED_ARRAY_MEMORY_MANAGER_
#define SEGMENTED_ARRAY_MEMORY_MANAGER_

#include "data_array.h"
#include "logger.h"
#include "types.h"
#include <unordered_map>

#define XR5_MEMORY_MANAGER_SEGMENT_SIZE 4096

namespace xr5 {
namespace utils {

using ByteArray =
    xr5::utils::DataArray<xr5::types::Byte, XR5_MEMORY_MANAGER_SEGMENT_SIZE>;

// @todo print addresses in hexadecimal
class SegmentedArrayMemoryManager {
private:
  std::unordered_map<xr5::types::Address, ByteArray> memory_;
  xr5::types::Address base_addr_;
  size_t size_;

public:
  bool allocateMemory(const xr5::types::Address &addr);

  void removeMemory(const xr5::types::Address &addr);

  SegmentedArrayMemoryManager(const xr5::types::Address &base_addr, size_t size)
      : base_addr_(base_addr), size_(size) {}

  ~SegmentedArrayMemoryManager() = default;

  xr5::types::Byte &at(const xr5::types::Address &addr);

  bool isValid(const xr5::types::Address &addr) {
    return (base_addr_ <= addr) && (addr <= (base_addr_ + size_));
  }
};
} // namespace utils

} // namespace xr5

#endif