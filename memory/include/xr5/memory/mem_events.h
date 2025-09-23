#ifndef MEM_EVENTS_H_
#define MEM_EVENTS_H_
#include "xr5/memory/memory_base.h"
#include "xr5/sim/event_queue.h"
#include "xr5/utils/utils.h"

namespace xr5 {
namespace memory {

class DataLatchEvent : public xr5::sim::Event {
public:
  DataLatchEvent() = delete;
  DataLatchEvent(MemoryObject *obj, xr5::types::Word word,
                 const xr5::types::Tick when,
                 const xr5::sim::Event::Priority priority =
                     xr5::sim::Event::kDefaultPriority)
      : xr5::sim::Event(when, priority), mem_obj_(obj), word_(word) {}
  ~DataLatchEvent() = default;

  void run() override;

  const std::string &getName() const noexcept override { return name_; }

private:
  MemoryObject *mem_obj_;
  xr5::types::Word word_;

  const std::string name_ = "memory_data_latch_event";
};

class AddrLatchEvent : public xr5::sim::Event {
public:
  AddrLatchEvent() = delete;
  AddrLatchEvent(MemoryObject *obj, xr5::types::Address addr,
                 const xr5::types::DramAddr &dram_addr,
                 const xr5::types::Tick when,
                 const xr5::sim::Event::Priority priority =
                     xr5::sim::Event::kDefaultPriority)
      : xr5::sim::Event(when, priority), mem_obj_(obj), addr_(addr),
        dram_addr_(dram_addr) {}
  ~AddrLatchEvent() = default;

  void run() override;

  const std::string &getName() const noexcept override { return name_; }

private:
  MemoryObject *mem_obj_;
  xr5::types::Address addr_;
  xr5::types::DramAddr dram_addr_;

  const std::string name_ = "memory_addr_latch_event";
};

class CmdLatchEvent : public xr5::sim::Event {
public:
  CmdLatchEvent() = delete;
  CmdLatchEvent(MemoryObject *obj, const xr5::types::DramCmd &dram_cmd,
                const xr5::types::Tick when,
                const xr5::sim::Event::Priority priority =
                    xr5::sim::Event::kDefaultPriority)
      : xr5::sim::Event(when, priority), mem_obj_(obj), dram_cmd_(dram_cmd) {}
  ~CmdLatchEvent() = default;

  void run() override;

  const std::string &getName() const noexcept override { return name_; }

private:
  MemoryObject *mem_obj_;
  xr5::types::DramCmd dram_cmd_;

  const std::string name_ = "memory_cmd_latch_event";
};

} // namespace memory

} // namespace xr5

#endif