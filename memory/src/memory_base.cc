#include "xr5/memory/memory_base.h"
#include "xr5/memory/mem_events.h"

namespace xr5 {
namespace memory {

/** TODO: handle for multiplexed ports, example: data port and addr port are
 * multiplexed into one */

/** TODO: replace nullptr with proper event queue pointer for SimObject */
MemoryObject::MemoryObject() : SimObject(nullptr) {
  data_port_ = xr5::utils::make_ptr<xr5::sim::Port, DataPort>(
      "data_port", xr5::sim::Port::getNewId(), this);
  addr_port_ = xr5::utils::make_ptr<xr5::sim::Port, AddrPort>(
      "address_port", xr5::sim::Port::getNewId(), this);
  cmd_port_ = xr5::utils::make_ptr<xr5::sim::Port, CmdPort>(
      "command_port", xr5::sim::Port::getNewId(), this);
}
/** TODO: replace nullptr with proper event queue pointer for SimObject */
MemoryObject::MemoryObject(xr5::sim::Port::Ptr data_port,
                           xr5::sim::Port::Ptr addr_port,
                           xr5::sim::Port::Ptr cmd_port)
    : SimObject(nullptr) {
  data_port_ = std::move(data_port);
  addr_port_ = std::move(addr_port);
  cmd_port_ = std::move(cmd_port);
}

xr5::sim::Port *MemoryObject::getPort(const std::string &name) {
  if (name == data_port_->getName())
    return getDataPort();
  else if (name == addr_port_->getName())
    return getAddrPort();
  else if (name == cmd_port_->getName())
    return getCmdPort();
  else
    return nullptr;
}

xr5::sim::Port *MemoryObject::getPort(const xr5::sim::Port::Id id) {
  if (id == data_port_->getId())
    return getDataPort();
  else if (id == addr_port_->getId())
    return getAddrPort();
  else if (id == cmd_port_->getId())
    return getCmdPort();
  else
    return nullptr;
}

void MemoryObject::DataPort::receive(const xr5::sim::Packet *packet) {
  /** TODO: replace 0 with proper schedule time */
  xr5::sim::Event::Ptr e =
      xr5::utils::make_ptr<xr5::sim::Event, DataLatchEvent>(mem_obj_,
                                                            packet->word, 0);
  mem_obj_->schedule(std::move(e));
}

void MemoryObject::AddrPort::receive(const xr5::sim::Packet *packet) {
  /** TODO: replace 0 with proper schedule time */
  xr5::sim::Event::Ptr e =
      xr5::utils::make_ptr<xr5::sim::Event, AddrLatchEvent>(
          mem_obj_, packet->addr, packet->data.dram_addr, 0);

  mem_obj_->schedule(std::move(e));
}

void MemoryObject::CmdPort::receive(const xr5::sim::Packet *packet) {
  /** TODO: replace 0 with proper schedule time */
  xr5::sim::Event::Ptr e = xr5::utils::make_ptr<xr5::sim::Event, CmdLatchEvent>(
      mem_obj_, packet->data.dram_cmd, 0);

  mem_obj_->schedule(std::move(e));
}

} // namespace memory

} // namespace xr5
