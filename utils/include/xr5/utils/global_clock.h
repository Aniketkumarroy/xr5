#ifndef GLOBAL_CLOCK_H_
#define GLOBAL_CLOCK_H_

#include "xr5/utils/types.h"

/** DISCUSS: we could have used a singleton wrapper for this but these functions
 * are very high frequency functions so we need highest performance eliminating
 * any indirection
 */
namespace xr5 {
namespace global_clock {

extern xr5::types::TimePS clock_;
extern xr5::types::Tick epoch_;
#if ENABLE_STEP_TIME
extern xr5::types::Tick step_;
#endif
inline constexpr xr5::types::Tick order_ =
    static_cast<xr5::types::Tick>(xr5::types::TimeQuanta::getUnit());

inline xr5::types::Tick get_curr_tick() noexcept { return clock_.getRawTick(); }

inline xr5::types::Tick get_curr_sim_tick() noexcept {
#if ENABLE_STEP_TIME
  return (get_curr_tick() - epoch_) / step_;
#else
  return (get_curr_tick() - epoch_) / order_;
#endif
}

inline xr5::types::Tick get_sim_step() noexcept {
#if ENABLE_STEP_TIME
  return step_;
#else
  return order_;
#endif
}

class SimulatorBase; // forward declaration

class internal {
  internal() = delete;
  internal(const internal &) = delete;
  internal &operator=(const internal &) = delete;

private:
  friend SimulatorBase;

#if ENABLE_STEP_TIME
  static inline void increment() noexcept { clock_ += step_; }
#endif
  static inline void inc() noexcept { clock_ += order_; }

  static inline void set_epoch(const xr5::types::Tick epoch) noexcept {
    clock_ = epoch;
    epoch_ = epoch;
  };
#if ENABLE_STEP_TIME
  static inline void set_step(const xr5::types::Tick step) noexcept {
    step_ = step * order_;
  };
#endif
};
} // namespace global_clock
} // namespace xr5
#endif // !GLOBAL_TICK_H_