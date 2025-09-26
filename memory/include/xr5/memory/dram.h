#ifndef DRAM_H_
#define DRAM_H_

#include "xr5/memory/memory_base.h"
#include "xr5/utils/types.h"

namespace xr5 {
namespace memory {

struct DramParams {
  xr5::types::Size capacity;
  uint16_t io_width_of_chip;
  uint8_t no_of_chips_per_rank;
  uint16_t rank;
};

class Dram : public MemoryBase<xr5::types::Address> {

public:
  Dram() = delete;
  Dram(const DramParams &params, const MemoryObject::Params mem_param)
      : MemoryBase<xr5::types::Address>(
            0, params.capacity.bytes() / xr5::types::WordSize, mem_param),
        params_(params) {
    assert(params_.io_width_of_chip * params_.no_of_chips_per_rank ==
           xr5::types::WordSize * 8);
  }
  ~Dram() = default;

  const xr5::types::Size &getCapacity() { return params_.capacity; }
  const DramParams &getDramParams() { return params_; }

private:
  const DramParams params_;
  xr5::types::Address addr_;
  xr5::types::Word data_;
};

} // namespace memory

} // namespace xr5

#endif
