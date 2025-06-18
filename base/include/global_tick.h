#ifndef GLOBAL_CLOCK_H_
#define GLOBAL_CLOCK_H_

#include "utils/clock_tick.h"
#include <memory>

namespace xr5 {
namespace clock {

class GlobalTick {
public:
  GlobalTick() = delete;
  GlobalTick(const GlobalTick &) = delete;
  GlobalTick &operator=(const GlobalTick &) = delete;

  static std::shared_ptr<xr5::utils::ClockTick> getInstance();

  static void init(xr5::utils::types::Tick c = 0);

private:
  static std::shared_ptr<xr5::utils::ClockTick> clock_;
};

} // namespace clock
} // namespace xr5
#endif
