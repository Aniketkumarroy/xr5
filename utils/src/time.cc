#include "utils/time.h"

namespace xr5 {
namespace utils {

void Time::setEpoch() {
  tUnit t = getTime();
  setEpoch(t);
}

tUnit Time::getTime() {
  std::chrono::time_point<std::chrono::high_resolution_clock,
                          std::chrono::duration<tUnit>>
      t_epoch = std::chrono::high_resolution_clock::now();
  return t_epoch.time_since_epoch().count();
}

tUnit Time::getTimeSinceEpoch() {
  tUnit t = getTime();
  return t - epoch_;
}
} // namespace utils
} // namespace xr5
