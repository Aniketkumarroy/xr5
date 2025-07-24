#ifndef TIME_H_
#define TIME_H_

#include "xr5/utils/types.h"
#include <chrono>

namespace xr5 {
namespace time {

using tUnit = xr5::types::TimeStamp;
using sys_clock = std::chrono::high_resolution_clock;
/**
 * @brief class for measuring system time since epoch
 *
 */
class Time {
public:
  void setEpoch(tUnit epoch) { epoch_ = epoch; }
  /**
   * @brief Set the \c epoch_ with current time
   *
   */
  void setEpoch();
  /**
   * @brief Get the current system time
   *
   * @return tUnit
   */
  tUnit getTime();
  /**
   * @brief Get the Time Since \c epoch_
   *
   * @return tUnit
   */
  tUnit getTimeSinceEpoch();

private:
  tUnit epoch_ = 0.0;
};

} // namespace time
} // namespace xr5
#endif