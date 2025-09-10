#include "xr5/sim/port.h"

namespace xr5 {
namespace sim {

void Port::bindSink(Port *sink) {
  sinks_.emplace_back(sink);
  sink->bindSource(this);
}

void Port::unbindSink(Port *sink) {
  sinks_.erase(std::remove(sinks_.begin(), sinks_.end(), sink), sinks_.end());
  sink->unbindSource(this);
}

void Port::send(const Packet *packet) {
  /** DISCUSS: for runtime speed, avoiding branch misprediction we are directly
   * calling \c receive without checking sink(sink != nullptr). its the
   * responsibility of the user to bind the port and check sink before using it
   */
  peer_->receive(packet);
}

} // namespace sim

} // namespace xr5
