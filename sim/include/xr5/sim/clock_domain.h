#ifndef CLOCK_DOMAIN_H_
#define CLOCK_DOMAIN_H_

#include "xr5/utils/types.h"
#include <algorithm>
#include <vector>

namespace xr5 {
namespace sim {

class ClockDomain {
public:
  ClockDomain() = default;
  ~ClockDomain() = default;

  inline const xr5::types::Time &getPeriod() const noexcept { return period_; }
  inline xr5::types::Tick getPeriodInPicoSec() const noexcept {
    return time_in_picosec_;
  }

  inline const xr5::types::Freq &getFrequency() const noexcept { return freq_; }
  inline xr5::types::Cycle getFrequencyInHertz() const noexcept {
    return freq_in_hertz_;
  }

  void updatePeriodAndFrequency(const xr5::types::Time &period) noexcept;

  void updatePeriodAndFrequency(const xr5::types::Freq &freq) noexcept;

protected:
  xr5::types::Freq freq_;
  xr5::types::Time period_;

  /** DISCUSS: since \c xr5::types::Time and \c xr5::types::Freq already have
   * inline function to get time and hertz so is it good to store this variables
   * explicitly
   */
  xr5::types::Tick time_in_picosec_;
  xr5::types::Cycle freq_in_hertz_;
};

class SrcClockDomain : public ClockDomain {
public:
  SrcClockDomain() = delete;
  SrcClockDomain(const xr5::types::Time &period) noexcept {
    updatePeriodAndFrequency(period);
  }
  SrcClockDomain(const xr5::types::Freq &freq) noexcept {
    updatePeriodAndFrequency(freq);
  }
  ~SrcClockDomain() = default;

  void updateDerivedClockDomains();

  void registerDerivedClockDomain(const DerivedClockDomain *derived_clk_domain);

  void
  deRegisterDerivedClockDomain(const DerivedClockDomain *derived_clk_domain);

private:
  std::vector<DerivedClockDomain *> derived_clock_domains_;
};

class DerivedClockDomain : public ClockDomain {
public:
  DerivedClockDomain() = delete;
  DerivedClockDomain(const SrcClockDomain *src_clk_domain,
                     const xr5::types::Scalar freq_multiplier) {
    registerSrcClockDomain(src_clk_domain);
    freq_scalar_ = freq_multiplier;
  }
  ~DerivedClockDomain() = default;

  inline void
  registerSrcClockDomain(const SrcClockDomain *src_clk_domain) noexcept {
    src_clock_ = src_clk_domain;
  }
  inline void deRegisterSrcClockDomain() noexcept { src_clock_ = nullptr; }

private:
  xr5::types::Scalar freq_scalar_;
  SrcClockDomain *src_clock_;
};
} // namespace sim

} // namespace xr5

#endif // !CLOCK_DOMAIN_H_
