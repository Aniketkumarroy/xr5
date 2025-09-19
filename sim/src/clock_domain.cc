#include "xr5/sim/clock_domain.h"

namespace xr5 {
namespace sim {

void ClockDomain::updatePeriodAndFrequency(
    const xr5::types::Time &period) noexcept {
  period_ = period;
  time_in_picosec_ = period_.picosec();
  freq_ = period_.getFrequency();
  freq_in_hertz_ = period_.getFreqInHertz();
}

void ClockDomain::updatePeriodAndFrequency(
    const xr5::types::Freq &freq) noexcept {
  freq_ = freq;
  freq_in_hertz_ = freq_.hertz();
  period_ = freq_.getPeriod();
  time_in_picosec_ = period_.picosec();
}

void SrcClockDomain::updateDerivedClockDomains() {
  for (const auto clk : derived_clock_domains_)
    clk->updatePeriodAndFrequency(freq_);
}

void SrcClockDomain::registerDerivedClockDomain(
    const DerivedClockDomain *derived_clk_domain) {
  if (std::find(derived_clock_domains_.begin(), derived_clock_domains_.end(),
                derived_clk_domain) == derived_clock_domains_.end())
    derived_clock_domains_.emplace_back(derived_clk_domain);
}

void SrcClockDomain::deRegisterDerivedClockDomain(
    const DerivedClockDomain *derived_clk_domain) {
  auto it = std::find(derived_clock_domains_.begin(),
                      derived_clock_domains_.end(), derived_clk_domain);
  if (it == derived_clock_domains_.end())
    return;

  derived_clock_domains_.erase(it);
  derived_clk_domain->deRegisterSrcClockDomain(this);
}
} // namespace sim

} // namespace xr5
