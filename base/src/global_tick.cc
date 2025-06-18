#include "global_tick.h"

namespace xr5 {
namespace clock {

std::shared_ptr<xr5::utils::ClockTick> GlobalTick::clock_;

std::shared_ptr<xr5::utils::ClockTick> GlobalTick::getInstance() {
  if (!clock_) {
    init(0);
  }
  return clock_;
}

void GlobalTick::init(xr5::utils::types::Tick c) {
  struct clock_shared : public xr5::utils::ClockTick {
    clock_shared(xr5::utils::types::Tick tick) : xr5::utils::ClockTick(tick) {}
  };
  clock_ = std::make_shared<clock_shared>(c);
}

} // namespace clock

} // namespace xr5