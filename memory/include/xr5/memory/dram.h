#ifndef DRAM_H_
#define DRAM_H_

#include "xr5/utils/segmented_array_memory_manager.h"
#include "xr5/utils/types.h"

namespace xr5 {
namespace memory {

using DramMemorySegment =
    xr5::utils::DataArray<xr5::types::Address, XR5_MEMORY_MANAGER_SEGMENT_SIZE>;

struct DramParams {
  xr5::types::Size capacity_;
  uint16_t io_width_of_chip_;
  uint8_t no_of_chips_per_rank_;
  uint16_t rank_;
};

class Dram {

public:
  Dram() = delete;
  Dram(const DramParams &params)
      : params_(params),
        memory_(0, params_.capacity_.bytes() / xr5::types::WordSize) {
    assert(params_.io_width_of_chip_ * params_.no_of_chips_per_rank_ ==
           xr5::types::WordSize * 8);

    data_port_ = xr5::utils::make_ptr<xr5::sim::Port, DataPort>(
        "data_port", xr5::sim::Port::getNewId());
    address_port_ = xr5::utils::make_ptr<xr5::sim::Port, AddrPort>(
        "data_port", xr5::sim::Port::getNewId());
    command_port_ = xr5::utils::make_ptr<xr5::sim::Port, CmdPort>(
        "data_port", xr5::sim::Port::getNewId());
  }
  ~Dram() = default;

  class DataPort : public xr5::sim::Port {
  public:
    DataPort(const std::string &name, const xr5::sim::Port::Id id)
        : xr5::sim::Port(name, id) {}

    void receive(const xr5::sim::Packet *packet) override {}
  };

  class AddrPort : public xr5::sim::Port {
  public:
    AddrPort(const std::string &name, const xr5::sim::Port::Id id)
        : xr5::sim::Port(name, id) {}

    void receive(const xr5::sim::Packet *packet) override {}
  };

  class CmdPort : public xr5::sim::Port {
  public:
    CmdPort(const std::string &name, const xr5::sim::Port::Id id)
        : xr5::sim::Port(name, id) {}

    void receive(const xr5::sim::Packet *packet) override {}
  };

  const xr5::types::Size &getCapacity() { return params_.capacity; }
  const DramParams &getDramParams() { return params_; }

private:
  const DramParams params_;
  xr5::utils::SegmentedArrayMemoryManager<DramMemorySegment> memory_;
};

} // namespace memory

} // namespace xr5

#endif
