#ifndef GLOBAL_CLOCK_H_
#define GLOBAL_CLOCK_H_

#include "xr5/utils/types.h"

/** DISCUSS: we could have used a singleton wrapper for this but these functions
 * are very high frequency functions so we need highest performance eliminating
 * any indirection
 */
namespace xr5 {
namespace global_clock {

xr5::types::Tick get_curr_tick() noexcept;

class SimulatorBase; // forward declaration

class internal {
  internal() = delete;
  internal(const internal &) = delete;
  internal &operator=(const internal &) = delete;

private:
  friend SimulatorBase;

  static void increment() noexcept;
  static void inc() noexcept;

  static void set_epoch(const xr5::types::Tick epoch) noexcept;
  static void set_step(const xr5::types::Tick step) noexcept;
};
} // namespace global_clock
} // namespace xr5
#endif // !GLOBAL_TICK_H_