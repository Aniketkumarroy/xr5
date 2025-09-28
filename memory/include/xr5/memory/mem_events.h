#ifndef MEM_EVENTS_H_
#define MEM_EVENTS_H_
#include "xr5/memory/memory_base.h"
#include "xr5/sim/event_queue.h"
#include "xr5/utils/utils.h"

namespace xr5 {
namespace memory {

class MemDataLatchEvent : public xr5::sim::Event {
public:
  MemDataLatchEvent() = delete;
  MemDataLatchEvent(MemoryObject *obj, const xr5::sim::Packet &pckt,
                    const xr5::types::Tick when,
                    const xr5::sim::Event::Priority priority =
                        xr5::sim::Event::kDefaultPriority)
      : xr5::sim::Event(when, priority), mem_obj_(obj), pckt_(pckt) {}
  ~MemDataLatchEvent() = default;

  void run() override;

  const std::string &getName() const noexcept override { return name_; }

private:
  MemoryObject *mem_obj_;
  xr5::sim::Packet pckt_;

  const std::string name_ = "memory_data_latch_event";
};

class MemAddrLatchEvent : public xr5::sim::Event {
public:
  MemAddrLatchEvent() = delete;
  MemAddrLatchEvent(MemoryObject *obj, const xr5::sim::Packet &pckt,
                    const xr5::types::Tick when,
                    const xr5::sim::Event::Priority priority =
                        xr5::sim::Event::kDefaultPriority)
      : xr5::sim::Event(when, priority), mem_obj_(obj), pckt_(pckt) {}
  ~MemAddrLatchEvent() = default;

  void run() override;

  const std::string &getName() const noexcept override { return name_; }

private:
  MemoryObject *mem_obj_;
  xr5::sim::Packet pckt_;

  const std::string name_ = "memory_addr_latch_event";
};

class MemCmdLatchEvent : public xr5::sim::Event {
public:
  MemCmdLatchEvent() = delete;
  MemCmdLatchEvent(MemoryObject *obj, const xr5::sim::Packet &pckt,
                   const xr5::types::Tick when,
                   const xr5::sim::Event::Priority priority =
                       xr5::sim::Event::kDefaultPriority)
      : xr5::sim::Event(when, priority), mem_obj_(obj), pckt_(pckt) {}
  ~MemCmdLatchEvent() = default;

  void run() override;

  const std::string &getName() const noexcept override { return name_; }

private:
  MemoryObject *mem_obj_;
  xr5::sim::Packet pckt_;

  const std::string name_ = "memory_cmd_latch_event";
};

} // namespace memory

} // namespace xr5

#endif