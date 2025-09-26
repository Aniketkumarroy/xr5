#include "global_clock.h"

namespace xr5 {
namespace global_clock {

namespace {
xr5::types::TimeQuanta clock_ = 0;
xr5::types::Tick step_ = 1;
} // namespace

xr5::types::Tick get_curr_tick() noexcept { return clock_.getRawTick(); }

void internal::increment() noexcept { clock_ += step_; }
void internal::inc() noexcept { ++clock_; }

void internal::set_epoch(const xr5::types::Tick epoch) noexcept {
  clock_ = epoch;
};
void internal::set_step(const xr5::types::Tick step) noexcept { step_ = step; };
} // namespace global_clock
} // namespace xr5
