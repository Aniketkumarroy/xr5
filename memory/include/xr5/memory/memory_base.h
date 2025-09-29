#ifndef MEMORY_BASE_H_
#define MEMORY_BASE_H_
#include "xr5/memory_config.h"
#include "xr5/sim/clock_domain.h"
#include "xr5/sim/port.h"
#include "xr5/sim/sim_object.h"
#include "xr5/utils/utils.h"

namespace xr5 {
namespace memory {

constexpr xr5::types::Tick
getTickForNoClock([[maybe_unused]] xr5::sim::ClockDomain *clk) {
  return 0;
}

/** DISCUSS: if anyone have any better names instead of MemoryObject and
 * MemoryBase please help */

class MemoryObject : public xr5::sim::SimObject {
public:
  struct Params {
  public:
    /** DISCUSS: this addhoc solution is done to also make the MemoryObject
     * class usefull for non-clocked purely combinational memories */
    xr5::sim::ClockDomain *clock = nullptr;
    xr5::types::Tick (*getTickForDataReceive)(xr5::sim::ClockDomain *) =
        getTickForNoClock;
    xr5::types::Tick (*getTickForAddrReceive)(xr5::sim::ClockDomain *) =
        getTickForNoClock;
    xr5::types::Tick (*getTickForCmdReceive)(xr5::sim::ClockDomain *) =
        getTickForNoClock;

    xr5::types::TimePS data_receive_delay = 0;
    xr5::types::TimePS addr_receive_delay = 0;
    xr5::types::TimePS cmd_receive_delay = 0;
  };

  MemoryObject(const MemoryObject::Params &params,
               xr5::sim::Port::Ptr data_port = nullptr,
               xr5::sim::Port::Ptr addr_port = nullptr,
               xr5::sim::Port::Ptr cmd_port = nullptr);

  virtual ~MemoryObject() = default;

  inline void sendWord(xr5::types::Word word) {
    /** TODO: make it multithreading compatible. currently if multiple threads
     * use sendWord, packet_to_send_ may get corrupted */
    packet_to_send_.word = word;
    packet_to_send_.type = xr5::sim::Packet::Type::WORD;
    data_port_->send(&packet_to_send_);
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
  inline xr5::sim::Port *getDataPort() const {
    return xr5::utils::get_raw_ptr<xr5::sim::Port>(data_port_);
  }

  inline xr5::sim::Port *getCmdPort() const {
    return xr5::utils::get_raw_ptr<xr5::sim::Port>(cmd_port_);
  }

  inline xr5::sim::Port *getAddrPort() const {
    return xr5::utils::get_raw_ptr<xr5::sim::Port>(addr_port_);
  }

  xr5::sim::Port *getPort(const std::string &name) const;
  xr5::sim::Port *getPort(const xr5::sim::Port::Id id) const;

  xr5::sim::Port *getPort(std::string &name,
                          xr5::sim::Port::Id &idx) const override;

  class DataPort : public xr5::sim::Port {
  public:
    DataPort(const std::string &name, const xr5::sim::Port::Id id,
             MemoryObject *mem_obj)
        : xr5::sim::Port(name, id), mem_obj_(mem_obj) {}

    void receive(const xr5::sim::Packet *packet) override;

  private:
    MemoryObject *mem_obj_;
  };

  class AddrPort : public xr5::sim::Port {
  public:
    AddrPort(const std::string &name, const xr5::sim::Port::Id id,
             MemoryObject *mem_obj)
        : xr5::sim::Port(name, id), mem_obj_(mem_obj) {}

    void receive(const xr5::sim::Packet *packet) override;

  private:
    MemoryObject *mem_obj_;
  };

  class CmdPort : public xr5::sim::Port {
  public:
    CmdPort(const std::string &name, const xr5::sim::Port::Id id,
            MemoryObject *mem_obj)
        : xr5::sim::Port(name, id), mem_obj_(mem_obj) {}

    void receive(const xr5::sim::Packet *packet) override;

  private:
    MemoryObject *mem_obj_;
  };

  virtual void handleDataPacket() {};
  virtual void handleAddrPacket() {};
  virtual void handleCmdPacket() {};

  xr5::sim::Packet data_packet_;
  xr5::sim::Packet addr_packet_;
  xr5::sim::Packet cmd_packet_;

private:
  xr5::sim::ClockDomain *clock_ = nullptr;
  xr5::types::Tick (*getTickForDataReceive_)(xr5::sim::ClockDomain *) = nullptr;
  xr5::types::Tick (*getTickForAddrReceive_)(xr5::sim::ClockDomain *) = nullptr;
  xr5::types::Tick (*getTickForCmdReceive_)(xr5::sim::ClockDomain *) = nullptr;

  xr5::sim::Packet packet_to_send_;

  xr5::sim::Port::Ptr addr_port_ = nullptr;
  xr5::sim::Port::Ptr cmd_port_ = nullptr;
  xr5::sim::Port::Ptr data_port_ = nullptr;

  xr5::types::Tick data_receive_delay_;
  xr5::types::Tick addr_receive_delay_;
  xr5::types::Tick cmd_receive_delay_;
};

template <typename Data,
          template <typename> class MemoryManager = DefaultMemoryManager>
class MemoryBase : public MemoryObject {
public:
  MemoryBase() = delete;

  MemoryBase(const xr5::types::Address base_addr, const size_t size,
             const MemoryObject::Params &params)
      : MemoryObject(params), memory_(base_addr, size) {}

  virtual ~MemoryBase() = default;

  inline Data getData(const xr5::types::Address addr) {
    return memory_.at(addr);
  }

  inline void setData(const xr5::types::Address addr, const Data data) {
    memory_.at(addr) = data;
  }

private:
  MemoryManager<Data> memory_;
};
} // namespace memory

} // namespace xr5

#endif