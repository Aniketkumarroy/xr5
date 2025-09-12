#include "xr5/sim/event_queue.h"

namespace xr5 {
namespace sim {

void EventQueue::insert(Event::Ptr event) {
  // if head is not assigned or is greater than event
  if (head_ == nullptr || *event < *head_) {
    event->next_ = std::move(head_);
    head_ = std::move(event);
    return;
  }

  Event *curr = xr5::utils::get_raw_ptr(head_);
  while (curr->next_ != nullptr && *curr->next_ <= *event)
    curr = xr5::utils::get_raw_ptr(curr->next_);

  if (*curr == *event) {
    event->same_ = std::move(curr->same_);
    curr->same_ = std::move(event);
  } else {
    event->next_ = std::move(curr->next_);
    curr->next_ = std::move(event);
  }
}

bool EventQueue::remove(const Event::Ptr event) {
  if (!head_) {
    logger_->error("[EventQueue::remove] head is empty!!");
    return false;
  }

  if (*event < *head_) {
    logger_->warn("[EventQueue::remove] event '{}' is not in queue",
                  event->getName());
    return false;
  }

  if (*head_ == *event) {
    if (head_ == event) {
      if (head_->same_ != nullptr) {
        head_->same_->next_ = std::move(head_->next_);
        head_ = std::move(head_->same_);
      } else {
        head_ = std::move(head_->next_);
      }
      return true;
    } else {
      Event *prev = xr5::utils::get_raw_ptr(head_);
      while (prev->same_ != nullptr && prev->same_ != event) {
        prev = xr5::utils::get_raw_ptr(prev->same_);
      }
      if (prev->same_ == event) {
        prev->same_ = std::move(prev->same_->same_);
        return true;
      }
    }
  }

  Event *prev = xr5::utils::get_raw_ptr(head_);
  Event *curr = xr5::utils::get_raw_ptr(prev->next_);
  while (curr != nullptr && *curr < *event) {
    prev = curr;
    curr = xr5::utils::get_raw_ptr(prev->next_);
  }

  if (curr != nullptr && *curr == *event) {
    if (curr == xr5::utils::get_raw_ptr(event)) {
      if (curr->same_ != nullptr) {
        curr->same_->next_ = std::move(curr->next_);
        prev->next_ = std::move(curr->same_);
      } else {
        prev->next_ = std::move(curr->next_);
      }
      return true;
    }

    prev = curr;
    curr = xr5::utils::get_raw_ptr(prev->same_);

    while (curr != nullptr && curr != xr5::utils::get_raw_ptr(event)) {
      prev = curr;
      curr = xr5::utils::get_raw_ptr(prev->same_);
    }

    if (curr == xr5::utils::get_raw_ptr(event)) {
      prev->same_ = std::move(curr->same_);
      return true;
    }
  }
  logger_->warn("[EventQueue::remove] event '{}' is not in queue",
                event->getName());
  return false;
}
} // namespace sim

} // namespace xr5
