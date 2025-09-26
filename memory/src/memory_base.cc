#include "xr5/memory/memory_base.h"
#include "xr5/memory/mem_events.h"
#include "xr5/utils/global_clock.h"

namespace xr5 {
namespace memory {

/** TODO: handle for multiplexed ports, example: data port and addr port are
 * multiplexed into one */

/** TODO: replace nullptr with proper event queue pointer for SimObject */
MemoryObject::MemoryObject(const MemoryObject::Params &params)
    : SimObject(nullptr), data_receive_delay_(params.data_receive_delay),
      addr_receive_delay_(params.addr_receive_delay),
      cmd_receive_delay_(params.cmd_receive_delay), clock_(params.clock) {
  if (params.data_port == nullptr) {
    data_port_ = xr5::utils::make_ptr<xr5::sim::Port, DataPort>(
        "data_port", xr5::sim::Port::getNewId(), this);
  } else {
    data_port_ = std::move(params.data_port);
  }
  if (params.addr_port == nullptr) {
    addr_port_ = xr5::utils::make_ptr<xr5::sim::Port, AddrPort>(
        "address_port", xr5::sim::Port::getNewId(), this);
  } else {
    addr_port_ = std::move(params.addr_port);
  }
  if (params.cmd_port == nullptr) {
    cmd_port_ = xr5::utils::make_ptr<xr5::sim::Port, CmdPort>(
        "command_port", xr5::sim::Port::getNewId(), this);
  } else {
    cmd_port_ = std::move(params.cmd_port);
  }
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

xr5::sim::Port *MemoryObject::getPort(std::string &name,
                                      xr5::sim::Port::Id &idx) const {
  xr5::sim::Port *_port = getPort(name);
  if (_port != nullptr) {
    idx = _port->getId();
    return _port;
  }

  _port = getPort(idx);
  if (_port != nullptr) {
    name = _port->getName();
    return _port;
  }
  return nullptr;
}

void MemoryObject::DataPort::receive(const xr5::sim::Packet *packet) {
  /** TODO: replace 0 with proper schedule time */
  xr5::sim::Event::Ptr e =
      xr5::utils::make_ptr<xr5::sim::Event, DataLatchEvent>(
          mem_obj_, packet->word, xr5::global_clock::get_curr_tick() + 0);
  mem_obj_->schedule(std::move(e));
}

void MemoryObject::AddrPort::receive(const xr5::sim::Packet *packet) {
  /** TODO: replace 0 with proper schedule time */
  xr5::sim::Event::Ptr e =
      xr5::utils::make_ptr<xr5::sim::Event, AddrLatchEvent>(
          mem_obj_, packet->addr, packet->data.dram_addr,
          xr5::global_clock::get_curr_tick() + 0);

  mem_obj_->schedule(std::move(e));
}

void MemoryObject::CmdPort::receive(const xr5::sim::Packet *packet) {
  /** TODO: replace 0 with proper schedule time */
  xr5::sim::Event::Ptr e = xr5::utils::make_ptr<xr5::sim::Event, CmdLatchEvent>(
      mem_obj_, packet->data.dram_cmd, xr5::global_clock::get_curr_tick() + 0);

  mem_obj_->schedule(std::move(e));
}

} // namespace memory

} // namespace xr5
