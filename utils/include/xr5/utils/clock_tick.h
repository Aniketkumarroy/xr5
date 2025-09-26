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

  inline xr5::types::Tick now() { return t_.getRawTick(); }

  inline const xr5::types::Time<U> &getTime() { return t_; }

  inline void inc() { ++t_; }
  inline void increment(const xr5::types::Tick tick) { t_ += tick; }

private:
  xr5::types::Time<U> t_;
};

} // namespace utils
} // namespace xr5
#endif