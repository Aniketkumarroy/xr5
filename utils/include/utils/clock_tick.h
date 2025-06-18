#include "utils/types.h"

namespace xr5 {
namespace utils {

/**
 * @brief minimalistic class to propagate time passage as tick
 *
 */
class ClockTick {

public:
  ClockTick(types::Tick c = 0) : c_(c) {}

  types::Tick now() { return c_; }

  void tick() { ++c_; }

private:
  types::Tick c_ = 0;
};

} // namespace utils
} // namespace xr5