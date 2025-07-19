#include "utils/time.h"

namespace xr5 {
namespace time {

void Time::setEpoch() {
  tUnit t = getTime();
  setEpoch(t);
}

tUnit Time::getTime() {
  std::chrono::time_point<sys_clock, std::chrono::duration<tUnit>> t_epoch =
      sys_clock::now();
  return t_epoch.time_since_epoch().count();
}

tUnit Time::getTimeSinceEpoch() {
  tUnit t = getTime();
  return t - epoch_;
}
} // namespace time
} // namespace xr5
