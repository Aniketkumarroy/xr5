#ifndef CLOCK_TICK_H_
#define CLOCK_TICK_H_

#include "utils/types.h"

namespace xr5 {
namespace clock {

/**
 * @brief minimalistic class to propagate time passage as tick
 *
 */
class ClockTick {

public:
  ClockTick(xr5::types::Tick c = 0) : c_(c) {}

  types::Tick now() { return c_; }

  void tick() { ++c_; }

private:
  types::Tick c_ = 0;
};

} // namespace clock
} // namespace xr5
#endif