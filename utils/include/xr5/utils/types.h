#ifndef TYPES_H_
#define TYPES_H_

#include "xr5/config.h"
#include <cstdint>
#include <cstring>
#include <limits>

namespace xr5 {
namespace types {

using Tick = uint64_t;
using Cycle = uint64_t;
#define MAX_TICK std::numeric_limits<xr5::types::Tick>::max()

using Address64 = uint64_t;
using Address32 = uint32_t;
using Byte = uint8_t;
using TimeStamp = double;
using Scalar = float;

#if XR5_ADDR_WIDTH == 64
using Address = Address64;
#elif XR5_ADDR_WIDTH == 32
using Address = Address32;
#else
#error "Unsupported address width"
#endif

using Word = Address;
constexpr uint64_t WordSize = sizeof(Address);

struct DramAddr {
  uint64_t bank;
  uint64_t row;
  uint64_t col;
  bool isRead; // DRAM read/write command
};

enum class DramCmd : uint8_t { ACTIVATE, PRECHARGE };

class Size {
public:
  enum class Unit : uint64_t {
    B = 1ULL,
    KB = 1024ULL * B,
    MB = 1024ULL * KB,
    GB = 1024ULL * MB
  };

  constexpr Size() : bytes_(0) {}
  constexpr Size(uint64_t value, Unit unit) noexcept
      : bytes_(value * static_cast<uint64_t>(unit)) {}

  static constexpr Size Byte(uint64_t b) noexcept { return Size(b, Unit::B); }
  static constexpr Size KiloByte(uint64_t k) noexcept {
    return Size(k, Unit::KB);
  }
  static constexpr Size MegaByte(uint64_t m) noexcept {
    return Size(m, Unit::MB);
  }
  static constexpr Size GigaByte(uint64_t g) noexcept {
    return Size(g, Unit::GB);
  }

  inline constexpr uint64_t bytes() const noexcept { return bytes_; }

  inline constexpr Scalar as(Unit unit) const noexcept {
    return static_cast<Scalar>(bytes_) / static_cast<Scalar>(unit);
  }

  inline constexpr Size operator+(const Size &other) const noexcept {
    return Size::Byte(bytes_ + other.bytes_);
  }

  Size &operator+=(const Size &other) noexcept {
    bytes_ += other.bytes_;
    return *this;
  }

  constexpr bool operator<(const Size &other) const noexcept {
    return bytes_ < other.bytes_;
  }
  constexpr bool operator==(const Size &other) const noexcept {
    return bytes_ == other.bytes_;
  }

private:
  uint64_t bytes_;
};

class TimeBase {
public:
  enum class Unit : Tick {
    PS = 1ULL,
    NS = 1000ULL * PS,
    US = 1000ULL * NS,
    MS = 1000ULL * US,
    S = 1000ULL * MS
  };

  constexpr TimeBase() : tick_(0) {}
  constexpr TimeBase(Tick time) : tick_(time) {}

  static auto Sec(const Tick s) noexcept;
  static auto MiliSec(uint64_t ms) noexcept;
  static auto MicroSec(uint64_t us) noexcept;
  static auto NanoSec(uint64_t ns) noexcept;
  static auto PicoSec(uint64_t ps) noexcept;

  constexpr Tick getRawTick() const noexcept { return tick_; }

  TimeBase &operator++() noexcept {
    ++tick_;
    return *this;
  }

  TimeBase &operator*=(Scalar multiplier) noexcept {
    tick_ = static_cast<uint64_t>(tick_ * multiplier);
    return *this;
  }

protected:
  Tick tick_;
};

class FreqBase {
public:
  enum class Unit : uint64_t {
    Hz = 1ULL,
    KHz = 1000ULL * Hz,
    MHz = 1000ULL * KHz,
    GHz = 1000ULL * MHz,
    THz = 1000ULL * GHz
  };

  constexpr FreqBase() : cycles_(0) {}
  constexpr FreqBase(Cycle value) noexcept : cycles_(value) {}

  static auto Hertz(Cycle hz) noexcept;
  static auto KiloHertz(Cycle khz) noexcept;
  static auto MegaHertz(Cycle mhz) noexcept;
  static auto GigaHertz(Cycle ghz) noexcept;
  static auto TeraHertz(Cycle thz) noexcept;

  inline constexpr Cycle getRawCycles() const noexcept { return cycles_; }

  FreqBase &operator*=(Scalar multiplier) noexcept {
    cycles_ = static_cast<Cycle>(cycles_ * multiplier);
    return *this;
  }

protected:
  Cycle cycles_;
};

template <FreqBase::Unit U> class Freq; // forward declaration
template <TimeBase::Unit U> class Time : public TimeBase {
public:
  constexpr Time() : TimeBase(0) {}
  constexpr Time(Tick time) : TimeBase(time) {}

  inline constexpr Tick picosec() const noexcept {
    return tick_ * static_cast<Tick>(U);
  }

  inline constexpr Scalar as(TimeBase::Unit unit) const noexcept {
    return static_cast<Scalar>(tick_) *
           (static_cast<Scalar>(U) / static_cast<Scalar>(unit));
  }

  inline constexpr Scalar getFreqInHertz() const noexcept {
    if (tick_ != 0)
      return static_cast<Scalar>(TimeBase::Unit::S) /
             static_cast<Scalar>(picosec());
    return 0.0;
  }

  // inline constexpr Freq<FreqBase::Unit::Hz> getFrequency() const noexcept;
  constexpr Time<U> operator+(const Time<U> &other) const noexcept {
    return Time<U>(tick_ + other.getRawTick());
  }

  Time<U> &operator+=(const Time<U> &other) noexcept {
    tick_ += other.tick_;
    return *this;
  }

  Time<U> operator*(Scalar multiplier) const noexcept {
    return Time<U>(static_cast<uint64_t>(tick_ * multiplier));
  }

  constexpr bool operator<(const Time<U> &other) const noexcept {
    return tick_ < other.tick_;
  }

  constexpr bool operator==(const Time<U> &other) const noexcept {
    return tick_ == other.tick_;
  }
};

auto TimeBase::Sec(const Tick s) noexcept { return Time<TimeBase::Unit::S>(s); }
auto TimeBase::MiliSec(const Tick ms) noexcept {
  return Time<TimeBase::Unit::MS>(ms);
}
auto TimeBase::MicroSec(const Tick us) noexcept {
  return Time<TimeBase::Unit::US>(us);
}
auto TimeBase::NanoSec(const Tick ns) noexcept {
  return Time<TimeBase::Unit::NS>(ns);
}
auto TimeBase::PicoSec(const Tick ps) noexcept {
  return Time<TimeBase::Unit::PS>(ps);
}

template <FreqBase::Unit U> class Freq : public FreqBase {
public:
  constexpr Freq() : FreqBase(0) {}
  constexpr Freq(Cycle value) noexcept : FreqBase(value) {}

  inline constexpr Scalar as(Unit unit) const noexcept {
    return static_cast<Scalar>(cycles_) *
           (static_cast<Scalar>(U) / static_cast<Scalar>(unit));
  }

  inline constexpr Cycle hertz() const noexcept {
    return cycles_ * static_cast<Cycle>(U);
  }

  inline constexpr Scalar getPeriodInSec() const noexcept {
    if (cycles_ != 0)
      return 1.0 / static_cast<Scalar>(hertz());
    return 0.0;
  }

  // inline constexpr Time<TimeBase::Unit::S> getPeriod() const noexcept;

  constexpr Freq<U> operator+(const Freq<U> &other) const noexcept {
    return FreqBase::Hertz(cycles_ + other.cycles_);
  }

  Freq<U> &operator+=(const Freq<U> &other) noexcept {
    cycles_ += other.cycles_;
    return *this;
  }

  Freq operator*(Scalar multiplier) const noexcept {
    return FreqBase::Hertz(static_cast<Cycle>(cycles_ * multiplier));
  }

  Freq<U> &operator*=(Scalar multiplier) noexcept {
    cycles_ = static_cast<Cycle>(cycles_ * multiplier);
    return *this;
  }

  constexpr bool operator<(const Freq<U> &other) const noexcept {
    return cycles_ < other.cycles_;
  }

  constexpr bool operator==(const Freq<U> &other) const noexcept {
    return cycles_ == other.cycles_;
  }
};

auto FreqBase::Hertz(Cycle hz) noexcept { return Freq<FreqBase::Unit::Hz>(hz); }
auto FreqBase::KiloHertz(Cycle khz) noexcept {
  return Freq<FreqBase::Unit::KHz>(khz);
}
auto FreqBase::MegaHertz(Cycle mhz) noexcept {
  return Freq<FreqBase::Unit::MHz>(mhz);
}
auto FreqBase::GigaHertz(Cycle ghz) noexcept {
  return Freq<FreqBase::Unit::GHz>(ghz);
}
auto FreqBase::TeraHertz(Cycle thz) noexcept {
  return Freq<FreqBase::Unit::THz>(thz);
}

static_assert(TimeBase::Unit::XR5_TIME_QUANTA == TimeBase::Unit::PS ||
                  TimeBase::Unit::XR5_TIME_QUANTA == TimeBase::Unit::NS ||
                  TimeBase::Unit::XR5_TIME_QUANTA == TimeBase::Unit::US ||
                  TimeBase::Unit::XR5_TIME_QUANTA == TimeBase::Unit::MS ||
                  TimeBase::Unit::XR5_TIME_QUANTA == TimeBase::Unit::S,
              "Invalid XR5_TIME_QUANTA: must be PS, NS, US, MS, or S");
using TimeQuanta = Time<TimeBase::Unit::XR5_TIME_QUANTA>;
} // namespace types
} // namespace xr5

#endif // !TYPES_H_