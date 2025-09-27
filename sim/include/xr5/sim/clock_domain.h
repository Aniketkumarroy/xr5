#ifndef CLOCK_DOMAIN_H_
#define CLOCK_DOMAIN_H_

#include "xr5/utils/global_clock.h"
#include "xr5/utils/types.h"
#include <algorithm>
#include <vector>

namespace xr5 {
namespace sim {

class ClockDomain {
public:
  ClockDomain() = default;
  ~ClockDomain() = default;

  inline const xr5::types::TimePS &getPeriod() const noexcept {
    return period_;
  }
  inline xr5::types::Tick getPeriodInPicoSec() const noexcept {
    return full_cycle_;
  }

  inline const xr5::types::FreqHz &getFrequency() const noexcept {
    return freq_;
  }
  inline xr5::types::Cycle getFrequencyInHertz() const noexcept {
    /** DISCUSS: since freq_ unit is already in Hertz we can use a cheap getter
     */
    return freq_.getRawCycles();
  }

  inline xr5::types::Tick getTickForNextCycle() {

    /**
     *   _______         _______         _______         _______         _______
     * _|       |_______|       |_______|       |_______|       |_______|
     *   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _
     * _| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_
     */
    return full_cycle_ -
           ((xr5::global_clock::get_curr_tick() - offset_) % full_cycle_);
  }

  inline xr5::types::Tick getTickForNextHalfCycle() {
    auto _offset = (xr5::global_clock::get_curr_tick() - offset_) % full_cycle_;
    return half_cycle_ - (_offset % half_cycle_);
  }

  void setPeriodAndFrequency(const xr5::types::TimePS &period) noexcept;

  void setPeriodAndFrequency(const xr5::types::FreqHz &freq) noexcept;

protected:
  xr5::types::FreqHz freq_;
  xr5::types::TimePS period_;

  xr5::types::Tick offset_;
  xr5::types::Tick full_cycle_;
  xr5::types::Tick half_cycle_;
};

class DerivedClockDomain; // forward declaration
class SrcClockDomain : public ClockDomain {
public:
  SrcClockDomain() = delete;
  SrcClockDomain(const xr5::types::TimePS &period) noexcept {
    setPeriodAndFrequency(period);
  }
  SrcClockDomain(const xr5::types::FreqHz &freq) noexcept {
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
