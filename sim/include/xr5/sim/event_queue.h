#ifndef EVENT_QUEUE_H_
#define EVENT_QUEUE_H_

#include "xr5/sim/event.h"
#include "xr5/utils/logger.h"
#include <memory>

namespace xr5 {
namespace sim {

/**
 * @brief
 *
 * DISCUSS: should we use a priority queue
 */
class EventQueue {
public:
  using Ptr = std::shared_ptr<EventQueue>;

  EventQueue() : logger_(xr5::utils::Logger::getInstance()) {}
  ~EventQueue() = default;

  bool isEmpty() { return !head_; }

  void insert(Event::Ptr event);
  bool remove(const Event::Ptr event);

private:
  Event::Ptr head_ = nullptr;
  std::shared_ptr<spdlog::logger> logger_ = nullptr;
};
} // namespace sim
} // namespace xr5

#endif