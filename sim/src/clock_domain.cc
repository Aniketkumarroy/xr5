#include "xr5/sim/clock_domain.h"
#include <cmath>

namespace xr5 {
namespace sim {

void ClockDomain::setPeriodAndFrequency(
    const xr5::types::TimePS &period) noexcept {
  period_ = period;
  freq_ = static_cast<xr5::types::Cycle>(period_.getFreqInHertz());
  offset_ = xr5::global_clock::get_curr_tick();
  /** DISCUSS: since period_ unit is already in picosec we can use a cheap
   * getter */
  full_cycle_ = period_.getRawTick();
  half_cycle_ = std::ceil(static_cast<xr5::types::Scalar>(full_cycle_) / 2.0);
}

void ClockDomain::setPeriodAndFrequency(
    const xr5::types::FreqHz &freq) noexcept {
  freq_ = freq;
  period_ = static_cast<xr5::types::Tick>(
      freq_.getPeriod(xr5::types::TimeBase::Unit::PS));
  offset_ = xr5::global_clock::get_curr_tick();
  /** DISCUSS: since period_ unit is already in picosec we can use a cheap
   * getter */
  full_cycle_ = period_.getRawTick();
  half_cycle_ = std::ceil(static_cast<xr5::types::Scalar>(full_cycle_) / 2.0);
}

SrcClockDomain::~SrcClockDomain() {
  for (auto clk : derived_clock_domains_)
    clk->deRegisterSrcClockDomain();
}

void SrcClockDomain::updateDerivedClockDomains() {
  for (const auto clk : derived_clock_domains_)
    clk->updatePeriodAndFrequencyFromSource();
}

void SrcClockDomain::registerDerivedClockDomain(
    DerivedClockDomain *derived_clk_domain) {
  if (std::find(derived_clock_domains_.begin(), derived_clock_domains_.end(),
                derived_clk_domain) == derived_clock_domains_.end())
    derived_clock_domains_.emplace_back(derived_clk_domain);
  derived_clk_domain->registerSrcClockDomain(this);
}

void SrcClockDomain::deRegisterDerivedClockDomain(
    DerivedClockDomain *derived_clk_domain) {
  auto it = std::find(derived_clock_domains_.begin(),
                      derived_clock_domains_.end(), derived_clk_domain);
  if (it == derived_clock_domains_.end())
    return;

  derived_clock_domains_.erase(it);
  derived_clk_domain->deRegisterSrcClockDomain();
}

DerivedClockDomain::DerivedClockDomain(
    SrcClockDomain *src_clk_domain, const xr5::types::Scalar freq_multiplier) {
  src_clk_domain->registerDerivedClockDomain(this);
  registerSrcClockDomain(src_clk_domain);
  freq_scalar_ = freq_multiplier;
  updatePeriodAndFrequencyFromSource();
}

DerivedClockDomain::~DerivedClockDomain() {
  src_clock_->deRegisterDerivedClockDomain(this);
  deRegisterSrcClockDomain();
}

void DerivedClockDomain::updatePeriodAndFrequencyFromSource() {
  /** NOTE: we don't check for src_clock_ != nullptr because we don't want to
   * add any runtime branch. also its a private function so only src_clock_ can
   * call this function or deregister itself
   */
  setPeriodAndFrequency(src_clock_->getFrequency() * freq_scalar_);
}
} // namespace sim

} // namespace xr5
