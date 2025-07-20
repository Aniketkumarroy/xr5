#include "utils/global_tick.h"

namespace xr5 {
namespace clock {

std::shared_ptr<xr5::clock::ClockTick> GlobalTick::clock_;

std::shared_ptr<xr5::clock::ClockTick> GlobalTick::getInstance() {
  if (!clock_) {
    init(0);
  }
  return clock_;
}

void GlobalTick::init(xr5::types::Tick c) {
  struct clock_shared : public xr5::clock::ClockTick {
    clock_shared(xr5::types::Tick tick) : xr5::clock::ClockTick(tick) {}
  };
  clock_ = std::make_shared<clock_shared>(c);
}

} // namespace clock

} // namespace xr5