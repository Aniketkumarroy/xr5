#ifndef CLOCK_TICK_H_
#define CLOCK_TICK_H_

#include "xr5/utils/types.h"

namespace xr5 {
namespace utils {

/**
 * @brief minimalistic class to propagate time passage as tick
 *
 *
 */
template <xr5::types::TimeBase::Unit U>
class ClockTick {

public:
  ClockTick(xr5::types::Tick c = 0) : t_(c) {}

  xr5::types::Tick now() { return t_.getRawTick(); }

  const xr5::types::Time<U> &getTime() { return t_; }

  void tick() { ++t_; }

private:
  xr5::types::Time<U> t_;
};

} // namespace utils
} // namespace xr5
#endif