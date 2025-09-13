#include "xr5/sim/port.h"

namespace xr5 {
namespace sim {

Port::Id Port::num_of_ports_minus_one_ = Port::kInvalidId;

void Port::bindSink(Port *sink) {
  if (std::find(sinks_.begin(), sinks_.end(), sink) == sinks_.end()) {
    // sink is already added
    return;
  }
  sinks_.emplace_back(sink);
  sink->bindSource(this);
}

void Port::unbindSink(Port *sink) {
  auto it = std::find(sinks_.begin(), sinks_.end(), sink);
  if (it == sinks_.end())
    return;
  sinks_.erase(it);
  sink->unbindSource(this);
}

void Port::bindSource(Port *source) {
  if (std::find(sources_.begin(), sources_.end(), source) == sources_.end()) {
    // source is already added
    return;
  }
  sources_.emplace_back(source);
  source->bindSink(this);
}

void Port::unbindSource(Port *source) {
  auto it = std::find(sources_.begin(), sources_.end(), source);
  if (it == sources_.end())
    return;
  sources_.erase(it);
  source->unbindSink(this);
}

void Port::unbindAll() {
  for (const auto sink : sinks_)
    sink->unbindSource(this);
  for (const auto source : sources_)
    source->unbindSink(this);
}

} // namespace sim

} // namespace xr5