#ifndef MEMORY_BASE_H_
#define MEMORY_BASE_H_

#include "xr5/utils/data_buffer.h"
#include "xr5/utils/types.h"
#include <unordered_map>

namespace xr5 {
namespace memory {

template <typename AddressType, typename DataBuff> class MemoryBlockMap {
public:
  MemoryBlockMap(const AddressType &start_address,
                 const AddressType &end_address)
      : start_addr_(start_address), end_addr_(end_address) {
    block_size_ = DataBuff::size;
  }
  ~MemoryBlockMap();

  void createMemory(const AddressType &addr,
                    const typename DataBuff::DataType &value) {
    mem_blocks_.emplace(addr, DataBuff());
  }

  typename DataBuff::DataType &at(AddressType addr) {
    // TODO: if block_size_ is a power of 2, we can use cheap bitwise operations
    return mem_blocks_.at(addr / block_size_)[addr % block_size_];
  }

private:
  std::unordered_map<AddressType, DataBuff> mem_blocks_;
  size_t block_size_;
  AddressType start_addr_, end_addr_;
};
} // namespace memory

} // namespace xr5

#endif