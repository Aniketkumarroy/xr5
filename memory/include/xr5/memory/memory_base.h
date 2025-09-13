#ifndef MEMORY_BASE_H_
#define MEMORY_BASE_H_
#include "xr5/sim/port.h"
#include "xr5/utils/utils.h"

namespace xr5 {
namespace memory {
class MemoryBase {
public:
  MemoryBase() = default;
  virtual ~MemoryBase() = default;

  inline void sendWord(xr5::types::Word word) {
    packet_.data.word = word;
    packet_.type = xr5::sim::Packet::Type::WORD;
    data_port_->send(&packet_);
  }

  /**
   * @brief Get the Data Port pointer.
   *
   * NOTE: although \c Port::Ptr maybe not a raw pointer but still return a raw
   * pointer because we don't want the receiver to own or manage the memory of
   * Port. its memory will only be managed by the owner class itself
   *
   * @return xr5::sim::Port* to the data_port_
   */
  inline xr5::sim::Port *getDataPort() {
    return xr5::utils::get_raw_ptr<xr5::sim::Port>(data_port_);
  }

  inline xr5::sim::Port *getCmdPort() {
    return xr5::utils::get_raw_ptr<xr5::sim::Port>(command_port_);
  }

  inline xr5::sim::Port *getAddrPort() {
    return xr5::utils::get_raw_ptr<xr5::sim::Port>(address_port_);
  }

protected:
  xr5::sim::Port::Ptr address_port_ = nullptr;
  xr5::sim::Port::Ptr command_port_ = nullptr;
  xr5::sim::Port::Ptr data_port_ = nullptr;

private:
  xr5::sim::Packet packet_;
};
} // namespace memory

} // namespace xr5

#endif