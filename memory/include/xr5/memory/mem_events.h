#ifndef MEM_EVENTS_H_
#define MEM_EVENTS_H_
#include "xr5/sim/event_queue.h"
#include "xr5/sim/sim_object.h"
#include "xr5/utils/utils.h"

namespace xr5 {
namespace sim {
class MemCpyEvent : Event {
public:
  MemCpyEvent() = delete;
  MemCpyEvent(SimObject *obj, const xr5::types::Tick when,
              const xr5::sim::Event::Priority priority =
                  xr5::sim::Event::DefaultPriority)
      : Event(when, priority), mem_obj_(obj) {}
  ~MemCpyEvent() = default;

  void run() override;

private:
  SimObject *mem_obj_;
};

inline void scheduleMemCpyEvent(SimObject *sim_obj, const xr5::types::Tick when,
                                const xr5::sim::Event::Priority priority =
                                    xr5::sim::Event::DefaultPriority) {
  Event::Ptr e =
      xr5::utils::make_ptr<Event, MemCpyEvent>(sim_obj, when, priority);
  sim_obj->schedule(std::move(e));
}

} // namespace sim

} // namespace xr5

#endif