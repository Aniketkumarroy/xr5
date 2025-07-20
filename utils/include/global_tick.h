#ifndef GLOBAL_CLOCK_H_
#define GLOBAL_CLOCK_H_

#include "utils/clock_tick.h"
#include <memory>

namespace xr5 {
namespace clock {

  /**
   * @brief singleton class to propagate global \c time as tick
   *
   */
class GlobalTick {
public:
  GlobalTick() = delete;
  GlobalTick(const GlobalTick &) = delete;
  GlobalTick &operator=(const GlobalTick &) = delete;

  static std::shared_ptr<xr5::clock::ClockTick> getInstance();

  static void init(xr5::types::Tick c = 0);

private:
  static std::shared_ptr<xr5::clock::ClockTick> clock_;
};

} // namespace clock
} // namespace xr5
#endif
