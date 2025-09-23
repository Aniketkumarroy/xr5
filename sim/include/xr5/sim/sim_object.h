#ifndef SIM_OBJECT_H_
#define SIM_OBJECT_H_
#include "xr5/sim/event_queue.h"
#include "xr5/sim/port.h"

namespace xr5 {
namespace sim {

class SimObject {
public:
  SimObject() = delete;
  SimObject(EventQueue::Ptr ptr_evt_queue) : ptr_event_queue_(ptr_evt_queue) {}
  ~SimObject() = default;

  inline void schedule(Event::Ptr event) {
    ptr_event_queue_->insert(std::move(event));
  }

  template <typename T>
  inline void
  schedule(T *obj, void (T::*method)(), const xr5::types::Tick when,
           const Event::Priority priority = Event::kDefaultPriority) {
    Event::Ptr _event = xr5::utils::make_ptr<Event, MemberFuncEvent<T>>(
        obj, method, when, priority);
    ptr_event_queue_->insert(std::move(_event));
  }

  virtual Port *getPort(const std::string &name,
                        Port::Id idx = Port::kInvalidId);

private:
  EventQueue::Ptr ptr_event_queue_;
};

} // namespace sim

} // namespace xr5

#endif