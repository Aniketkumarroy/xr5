#ifndef EVENT_H_
#define EVENT_H_

#include "xr5/utils/types.h"
#include "xr5/utils/utils.h"
#include <memory>
#include <string_view>

namespace xr5 {
namespace sim {

class Event {
public:
  /** NOTE: we need lifetime management of the events because after creating a
   * event and inserting it in event queue, the creator may get destroyed and
   * the Event ptr too, in that case we want that the event doesn't get invalid
   * until event queue execute the event and destroys its event ptr. this
   * lifecycle will be handled by smart pointers
   */
  using Ptr = std::unique_ptr<Event>;
  using Priority = int8_t;
  static constexpr Priority kDefaultPriority = Priority(0);
  static constexpr Priority kMinimumPriority =
      std::numeric_limits<Priority>::min();
  static constexpr Priority kMaximumPriority =
      std::numeric_limits<Priority>::max();

  /** TODO: make a factory method to return Ptr
   */

  Event(const xr5::types::Tick t, const Priority p) : when_(t), priority_(p) {}
  Event() = delete;
  virtual ~Event();

  inline void setScheduledTime(const xr5::types::Tick t) { when_ = t; }
  inline void setPriority(const Priority p) { priority_ = p; }
  inline xr5::types::Tick getScheduledTime() const { return when_; }
  inline Priority getPriority() const { return priority_; }

  inline bool operator<(const Event &other) const {
    if (getScheduledTime() != other.getScheduledTime())
      return getScheduledTime() < other.getScheduledTime();
    return getPriority() < other.getPriority();
  }

  inline bool operator==(const Event &other) const {
    return getScheduledTime() == other.getScheduledTime() &&
           getPriority() == other.getPriority();
  }

  inline bool operator<=(const Event &other) const {
    return *this == other || *this < other;
  }

  // inline void run() { func_event_(sim_obj_); }

  virtual void run() = 0;
  virtual const std::string &getName() const noexcept = 0;

  Ptr next_ = nullptr;
  Ptr same_ = nullptr;

private:
  xr5::types::Tick when_;
  Priority priority_;

  /** DISCUSS: is it possible to store the simulation object and its member
   * function we want to schedule as a pointer? it is a workaround to avoid 2
   * memory loads of virtual function compared to 1 memory load here.
   * virtual function
   * load reg [vptr]
   * load reg [vptr + offset]
   * call reg
   * .
   * function pointer
   * load reg [ptr + offset]
   * call reg
   */
  // SimObject *sim_obj_;
  // void (*func_event_)(SimObject *) = nullptr;
};

template <typename T> class MemberFuncEvent : public Event {
public:
  MemberFuncEvent(T *obj, void (T::*method)(), const xr5::types::Tick when,
                  const Event::Priority priority = Event::kDefaultPriority)
      : Event(when, priority), obj_(obj), method_(method) {}

  MemberFuncEvent(T &obj, void (T::*method)(), const xr5::types::Tick when,
                  const Event::Priority priority = Event::kDefaultPriority)
      : Event(when, priority), obj_(&obj), method_(method) {}

  ~MemberFuncEvent() = default;

  /** DISCUSS: is there any better way to do this? we are having 4 memory
   * loads(2 to get virtual function call of run + 2 to load obj and method)
   * before calling the execution function
   */
  void run() override { (obj_->*method_)(); }

private:
  T *obj_;
  void (T::*method_)();
};

class EventCallback {
public:
  template <typename T>
  EventCallback(T *obj, void (T::*method)(), xr5::types::Tick when,
                Event::Priority priority = Event::kDefaultPriority)
      : ptr_event_(xr5::utils::make_ptr<Event, MemberFuncEvent<T>>(
            obj, method, when, priority)) {}

  template <typename T>
  EventCallback(T &obj, void (T::*method)(), xr5::types::Tick when,
                Event::Priority priority = Event::kDefaultPriority)
      : ptr_event_(xr5::utils::make_ptr<Event, MemberFuncEvent<T>>(
            obj, method, when, priority)) {}

  ~EventCallback() = default;

  void operator()() { ptr_event_->run(); }

private:
  Event::Ptr ptr_event_;
};

} // namespace sim

} // namespace xr5

#endif