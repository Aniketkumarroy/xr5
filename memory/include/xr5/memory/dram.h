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
  Dram(const DramParams &params)
      : MemoryBase<xr5::types::Address>(0, params.capacity.bytes() /
                                               xr5::types::WordSize),
        params_(params) {
    assert(params_.io_width_of_chip * params_.no_of_chips_per_rank ==
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
    DataPort(const std::string &name, const xr5::sim::Port::Id id,
             Dram *mem_obj)
        : xr5::sim::Port(name, id), mem_obj_(mem_obj) {}

    void receive(const xr5::sim::Packet *packet) override {}

  private:
    Dram *mem_obj_;
  };

  class AddrPort : public xr5::sim::Port {
  public:
    AddrPort(const std::string &name, const xr5::sim::Port::Id id,
             Dram *mem_obj)
        : xr5::sim::Port(name, id), mem_obj_(mem_obj) {}

    void receive(const xr5::sim::Packet *packet) override {}

  private:
    Dram *mem_obj_;
  };

  class CmdPort : public xr5::sim::Port {
  public:
    CmdPort(const std::string &name, const xr5::sim::Port::Id id, Dram *mem_obj)
        : xr5::sim::Port(name, id), mem_obj_(mem_obj) {}

    void receive(const xr5::sim::Packet *packet) override {}

  private:
    Dram *mem_obj_;
  };

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
