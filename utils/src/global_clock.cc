#include "xr5/utils/global_clock.h"

namespace xr5 {
namespace global_clock {

xr5::types::TimePS clock_ = 0;
xr5::types::Tick epoch_ = 0;
#if ENABLE_STEP_TIME
xr5::types::Tick step_ = 1;
#endif

} // namespace global_clock
} // namespace xr5
