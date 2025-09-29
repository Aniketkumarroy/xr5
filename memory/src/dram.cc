#include "xr5/memory/dram.h"
#include "xr5/utils/global_clock.h"

namespace xr5 {
namespace memory {

void Dram::setLatency(const Dram::Latency &latency) {
  t_RCD_ = latency.t_RCD.picosec();
  t_RAS_ = latency.t_RAS.picosec();
  t_RP_ = latency.t_RP.picosec();
  t_RC_ = latency.t_RC.picosec();
  t_CL_ = latency.t_CL.picosec();
  t_CWL_ = latency.t_CWL.picosec();
  t_CCD_ = latency.t_CCD.picosec();
  t_RRD_ = latency.t_RRD.picosec();
  t_FAW_ = latency.t_FAW.picosec();
  t_WTR_ = latency.t_WTR.picosec();
  t_WR_ = latency.t_WR.picosec();
  t_RFC_ = latency.t_RFC.picosec();
  t_REFI_ = latency.t_REFI.picosec();
}

void Dram::handleCmdPacket() {}
} // namespace memory

} // namespace xr5
