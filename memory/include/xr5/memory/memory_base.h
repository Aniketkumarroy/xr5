#ifndef MEMORY_BASE_H_
#define MEMORY_BASE_H_
#include "xr5/memory_config.h"
#include "xr5/sim/port.h"
#include "xr5/utils/utils.h"

namespace xr5 {
namespace memory {

template <typename Data,
          template <typename> class MemoryManager = DefaultMemoryManager>
class MemoryBase {
public:
  MemoryBase() = delete;
  MemoryBase(const xr5::types::Address base_addr, const size_t size)
      : memory_(base_addr, size) {
    data_port_ = xr5::utils::make_ptr<xr5::sim::Port, DataPort>(
        "data_port", xr5::sim::Port::getNewId(), this);
    addr_port_ = xr5::utils::make_ptr<xr5::sim::Port, AddrPort>(
        "address_port", xr5::sim::Port::getNewId(), this);
    cmd_port_ = xr5::utils::make_ptr<xr5::sim::Port, CmdPort>(
        "command_port", xr5::sim::Port::getNewId(), this);
  }

  MemoryBase(const xr5::types::Address base_addr, const size_t size,
             xr5::sim::Port::Ptr data_port, xr5::sim::Port::Ptr addr_port,
             xr5::sim::Port::Ptr cmd_port)
      : memory_(base_addr, size) {
    data_port_ = std::move(data_port);
    addr_port_ = std::move(addr_port);
    cmd_port_ = std::move(cmd_port);
  }
  virtual ~MemoryBase() = default;

  inline void sendWord(xr5::types::Word word) {
    packet_.word = word;
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
    return xr5::utils::get_raw_ptr<xr5::sim::Port>(cmd_port_);
  }

  inline xr5::sim::Port *getAddrPort() {
    return xr5::utils::get_raw_ptr<xr5::sim::Port>(addr_port_);
  }

  inline xr5::sim::Port *getPort(const std::string &name) {
    if (name == data_port_->getName())
      return getDataPort();
    else if (name == addr_port_->getName())
      return getAddrPort();
    else if (name == cmd_port_->getName())
      return getCmdPort();
    else
      return nullptr;
  }

  inline xr5::sim::Port *getPort(const xr5::sim::Port::Id id) {
    if (id == data_port_->getId())
      return getDataPort();
    else if (id == addr_port_->getId())
      return getAddrPort();
    else if (id == cmd_port_->getId())
      return getCmdPort();
    else
      return nullptr;
  }

  inline Data getData(const xr5::types::Address addr) {
    return memory_.at(addr);
  }

  inline void setData(const xr5::types::Address addr, const Data data) {
    memory_.at(addr) = data;
  }

  class DataPort : public xr5::sim::Port {
  public:
    DataPort(const std::string &name, const xr5::sim::Port::Id id,
             MemoryBase *mem_obj)
        : xr5::sim::Port(name, id), mem_obj_(mem_obj) {}

    void receive(const xr5::sim::Packet *packet) override {
      mem_obj_->data_packet_.word = packet->word;
      mem_obj_->handleDataPacket();
    }

  private:
    MemoryBase *mem_obj_;
  };

  class AddrPort : public xr5::sim::Port {
  public:
    AddrPort(const std::string &name, const xr5::sim::Port::Id id,
             MemoryBase *mem_obj)
        : xr5::sim::Port(name, id), mem_obj_(mem_obj) {}

    void receive(const xr5::sim::Packet *packet) override {
      mem_obj_->addr_packet_.addr = packet->addr;
      mem_obj_->addr_packet_.data.dram_addr = packet->data.dram_addr;
      mem_obj_->handleAddrPacket();
    }

  private:
    MemoryBase *mem_obj_;
  };

  class CmdPort : public xr5::sim::Port {
  public:
    CmdPort(const std::string &name, const xr5::sim::Port::Id id,
            MemoryBase *mem_obj)
        : xr5::sim::Port(name, id), mem_obj_(mem_obj) {}

    void receive(const xr5::sim::Packet *packet) override {
      mem_obj_->cmd_packet_.data.dram_cmd = packet->data.dram_cmd;
      mem_obj_->handleCmdPacket();
    }

  private:
    MemoryBase *mem_obj_;
  };

  virtual void handleDataPacket() {};
  virtual void handleAddrPacket() {};
  virtual void handleCmdPacket() {};

protected:
  xr5::sim::Port::Ptr addr_port_ = nullptr;
  xr5::sim::Port::Ptr cmd_port_ = nullptr;
  xr5::sim::Port::Ptr data_port_ = nullptr;

  xr5::sim::Packet data_packet_;
  xr5::sim::Packet addr_packet_;
  xr5::sim::Packet cmd_packet_;

private:
  xr5::sim::Packet packet_;
  MemoryManager<Data> memory_;
};
} // namespace memory

} // namespace xr5

#endif