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
template <typename DataArray = ByteArray> class SegmentedArrayMemoryManager {
private:
  std::unordered_map<xr5::types::Address, DataArray> memory_;
  xr5::types::Address base_addr_;
  size_t size_;

public:
  SegmentedArrayMemoryManager(const xr5::types::Address &base_addr, size_t size)
      : base_addr_(base_addr), size_(size) {}

  ~SegmentedArrayMemoryManager() = default;

  typename DataArray::DataType &at(const xr5::types::Address &addr) {
    if (!isValid(addr)) {
      auto logger = xr5::utils::Logger::GetInstance();
      logger->error(
          "[SegmentedArrayMemoryManager::at] {} is out of limits [{}, {}]",
          addr, base_addr_, base_addr_ + size_);
      throw std::domain_error("address out of range");
    }
    if constexpr (DataArray::size &&
                  !(DataArray::size & (DataArray::size - 1))) {
      // if segment size is a power of two we will use cheap bitwise operations
      constexpr auto _shift = __builtin_ctzll(DataArray::size);
      constexpr auto _mask = DataArray::size - 1;
      return memory_[addr >> _shift][addr & _mask];
    } else {
      return memory_[addr / DataArray::size][addr % DataArray::size];
    }
  }

  bool allocateMemory(const xr5::types::Address &addr) {
    if (!isValid(addr)) {
      auto logger = xr5::utils::Logger::GetInstance();
      logger->error(
          "[SegmentedArrayMemoryManager::allocateMemory] {} is out of "
          "limits [{}, {}]",
          addr, base_addr_, base_addr_ + size_);
      return false;
    }

    if constexpr (DataArray::size &&
                  !(DataArray::size & (DataArray::size - 1))) {
      // if segment size is a power of two we will use cheap bitwise operations
      constexpr auto _shift = __builtin_ctzll(DataArray::size);
      auto it = memory_.emplace(std::make_pair(addr >> _shift, DataArray()));
      return it.second;
    } else {
      auto it =
          memory_.emplace(std::make_pair(addr / DataArray::size, DataArray()));
      return it.second;
    }
  }

  void removeMemory(const xr5::types::Address &addr) {
    if (!isValid(addr)) {
      auto logger = xr5::utils::Logger::GetInstance();
      logger->error("[SegmentedArrayMemoryManager::removeMemory] {} is out of "
                    "limits [{}, {}]",
                    addr, base_addr_, base_addr_ + size_);
      return;
    }

    if constexpr (DataArray::size &&
                  !(DataArray::size & (DataArray::size - 1))) {
      // if segment size is a power of two we will use cheap bitwise operations
      constexpr auto _shift = __builtin_ctzll(DataArray::size);
      size_t _ret = memory_.erase(addr >> _shift);
      if (_ret == 0) {
        auto logger = xr5::utils::Logger::GetInstance();
        logger->warn(
            "[SegmentedArrayMemoryManager::removeMemory] {} doesn't exist",
            addr);
      }
    } else {
      size_t _ret = memory_.erase(addr / DataArray::size);
      if (_ret == 0) {
        auto logger = xr5::utils::Logger::GetInstance();
        logger->warn(
            "[SegmentedArrayMemoryManager::removeMemory] {} doesn't exist",
            addr);
      }
    }
  }

  bool isValid(const xr5::types::Address &addr) {
    return (base_addr_ <= addr) && (addr <= (base_addr_ + size_));
  }
};
} // namespace utils

} // namespace xr5

#endif