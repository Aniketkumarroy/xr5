#ifndef TIME_H_
#define TIME_H_

#include "utils/types.h"
#include <chrono>

namespace xr5 {
namespace utils {

using tUnit = xr5::utils::types::TimeStamp;
class Time {
public:
  void setEpoch(tUnit epoch) { epoch_ = epoch; }
  void setEpoch();
  tUnit getTime();
  tUnit getTimeSinceEpoch();

private:
  tUnit epoch_ = 0.0;
};

} // namespace utils
} // namespace xr5
#endif