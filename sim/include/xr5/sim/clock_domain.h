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

  void setPeriodAndFrequency(const xr5::types::Time &period) noexcept;

  void setPeriodAndFrequency(const xr5::types::Freq &freq) noexcept;

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

class DerivedClockDomain; // forward declaration
class SrcClockDomain : public ClockDomain {
public:
  SrcClockDomain() = delete;
  SrcClockDomain(const xr5::types::Time &period) noexcept {
    setPeriodAndFrequency(period);
  }
  SrcClockDomain(const xr5::types::Freq &freq) noexcept {
    setPeriodAndFrequency(freq);
  }
  ~SrcClockDomain();

  void updateDerivedClockDomains();

  void registerDerivedClockDomain(DerivedClockDomain *derived_clk_domain);

  void deRegisterDerivedClockDomain(DerivedClockDomain *derived_clk_domain);

private:
  std::vector<DerivedClockDomain *> derived_clock_domains_;
};

class DerivedClockDomain : public ClockDomain {
public:
  DerivedClockDomain() = delete;
  DerivedClockDomain(SrcClockDomain *src_clk_domain,
                     const xr5::types::Scalar freq_multiplier);
  ~DerivedClockDomain();

  void setFreqMultiplier(const double multiplier) noexcept {
    freq_scalar_ = multiplier;
  }

private:
  /** NOTE: this functions are made private so that no one except the src clock
   * can change the clock params or deregisters itself.
   */
  inline void registerSrcClockDomain(SrcClockDomain *src_clk_domain) noexcept {
    src_clock_ = src_clk_domain;
  }
  inline void deRegisterSrcClockDomain() noexcept { src_clock_ = nullptr; }
  void updatePeriodAndFrequencyFromSource();

  xr5::types::Scalar freq_scalar_;
  SrcClockDomain *src_clock_;

  /** NOTE: src clock should be the only one to edit this params */
  friend SrcClockDomain;
};
} // namespace sim

} // namespace xr5

#endif // !CLOCK_DOMAIN_H_
