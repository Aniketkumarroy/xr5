#include "xr5/utils/segmented_array_memory_manager.h"

namespace xr5 {
namespace utils {

xr5::types::Byte &
SegmentedArrayMemoryManager::at(const xr5::types::Address &addr) {
  if (!isValid(addr)) {
    auto logger = xr5::utils::Logger::GetInstance();
    logger->error(
        "[SegmentedArrayMemoryManager::at] {} is out of limits [{}, {}]", addr,
        base_addr_, base_addr_ + size_);
    throw std::domain_error("address out of range");
  }
  if constexpr (XR5_MEMORY_MANAGER_SEGMENT_SIZE &&
                !(XR5_MEMORY_MANAGER_SEGMENT_SIZE &
                  (XR5_MEMORY_MANAGER_SEGMENT_SIZE - 1))) {
    // if segment size is a power of two we will use cheap bitwise operations
    constexpr auto _shift = __builtin_ctzll(XR5_MEMORY_MANAGER_SEGMENT_SIZE);
    constexpr auto _mask = XR5_MEMORY_MANAGER_SEGMENT_SIZE - 1;
    return memory_[addr >> _shift][addr & _mask];
  } else {
    return memory_[addr / XR5_MEMORY_MANAGER_SEGMENT_SIZE]
                  [addr % XR5_MEMORY_MANAGER_SEGMENT_SIZE];
  }
}

bool SegmentedArrayMemoryManager::allocateMemory(
    const xr5::types::Address &addr) {
  if (!isValid(addr)) {
    auto logger = xr5::utils::Logger::GetInstance();
    logger->error(
        "[SegmentedArrayMemoryManager::at] {} is out of limits [{}, {}]", addr,
        base_addr_, base_addr_ + size_);
    return false;
  }
  auto it = memory_.emplace(
      std::make_pair(addr / XR5_MEMORY_MANAGER_SEGMENT_SIZE, ByteArray()));
  return it.second;
}

} // namespace utils

} // namespace xr5
