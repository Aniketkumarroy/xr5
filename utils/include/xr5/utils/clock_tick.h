#ifndef CLOCK_TICK_H_
#define CLOCK_TICK_H_

#include "xr5/utils/types.h"

namespace xr5 {
namespace utils {

/**
 * @brief minimalistic class to propagate time passage as tick
 *
 * NOTE: currently here one tick corresponds to 1 picosecond
 * TODO: we can templatize the class to have one tick to any of
 * \c xr5::types::Time::Unit
 *
 */
class ClockTick {

public:
  ClockTick(xr5::types::Tick c = 0) : t_(xr5::types::Time::PicoSec(c)) {}

  xr5::types::Tick now() { return t_.picosec(); }

  const xr5::types::Time &getTime() { return t_; }

  void tick() { ++t_; }

private:
  xr5::types::Time t_;
};

} // namespace utils
} // namespace xr5
#endif