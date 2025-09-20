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

class Freq; // forward declaration
class Time {
public:
  enum class Unit : Tick {
    PS = 1ULL,
    NS = 1000ULL * PS,
    US = 1000ULL * NS,
    MS = 1000ULL * US,
    S = 1000ULL * MS
  };

  constexpr Time() : picoseconds_(0) {}
  constexpr Time(Tick value, Unit unit)
      : picoseconds_(value * static_cast<uint64_t>(unit)) {}

  static constexpr Time Sec(uint64_t s) noexcept { return Time(s, Unit::S); }
  static constexpr Time MiliSec(uint64_t ms) noexcept {
    return Time(ms, Unit::MS);
  }
  static constexpr Time MicroSec(uint64_t us) noexcept {
    return Time(us, Unit::US);
  }
  static constexpr Time NanoSec(uint64_t ns) noexcept {
    return Time(ns, Unit::NS);
  }
  static constexpr Time PicoSec(uint64_t ps) noexcept {
    return Time(ps, Unit::PS);
  }

  inline constexpr Tick picosec() const noexcept { return picoseconds_; }

  inline constexpr Scalar as(Unit unit) const noexcept {
    return static_cast<Scalar>(picoseconds_) / static_cast<Scalar>(unit);
  }

  inline constexpr Scalar getFreqInHertz() const noexcept {
    if (picoseconds_ != 0)
      return static_cast<Scalar>(Unit::S) / static_cast<Scalar>(picoseconds_);
    return 0.0;
  }

  inline constexpr Freq getFrequency() const noexcept;

  constexpr Time operator+(const Time &other) const noexcept {
    return Time::PicoSec(picoseconds_ + other.picoseconds_);
  }

  Time &operator+=(const Time &other) noexcept {
    picoseconds_ += other.picoseconds_;
    return *this;
  }

  Time &operator++() noexcept {
    ++picoseconds_;
    return *this;
  }

  Time operator*(Scalar multiplier) const noexcept {
    return Time::PicoSec(static_cast<uint64_t>(picoseconds_ * multiplier));
  }

  Time &operator*=(Scalar multiplier) noexcept {
    picoseconds_ = static_cast<uint64_t>(picoseconds_ * multiplier);
    return *this;
  }

  constexpr bool operator<(const Time &other) const noexcept {
    return picoseconds_ < other.picoseconds_;
  }

  constexpr bool operator==(const Time &other) const noexcept {
    return picoseconds_ == other.picoseconds_;
  }

private:
  Tick picoseconds_;
};

class Freq {
public:
  enum class Unit : uint64_t {
    Hz = 1ULL,
    KHz = 1000ULL * Hz,
    MHz = 1000ULL * KHz,
    GHz = 1000ULL * MHz,
    THz = 1000ULL * GHz
  };

  constexpr Freq() : cycles_in_hz_(0) {}
  constexpr Freq(Cycle value, Unit unit) noexcept
      : cycles_in_hz_(value * static_cast<uint64_t>(unit)) {}

  static constexpr Freq Hertz(Cycle hz) noexcept { return Freq(hz, Unit::Hz); }
  static constexpr Freq KiloHertz(Cycle khz) noexcept {
    return Freq(khz, Unit::KHz);
  }
  static constexpr Freq MegaHertz(Cycle mhz) noexcept {
    return Freq(mhz, Unit::MHz);
  }
  static constexpr Freq GigaHertz(Cycle ghz) noexcept {
    return Freq(ghz, Unit::GHz);
  }
  static constexpr Freq TeraHertz(Cycle thz) noexcept {
    return Freq(thz, Unit::THz);
  }

  inline constexpr Cycle hertz() const noexcept { return cycles_in_hz_; }

  inline constexpr Scalar as(Unit unit) const noexcept {
    return static_cast<Scalar>(cycles_in_hz_) / static_cast<Scalar>(unit);
  }

  inline constexpr Scalar getPeriodInSec() const noexcept {
    if (cycles_in_hz_ != 0)
      return 1.0 / static_cast<Scalar>(cycles_in_hz_);
    return 0.0;
  }

  inline constexpr Time getPeriod() const noexcept;

  constexpr Freq operator+(const Freq &other) const noexcept {
    return Freq::Hertz(cycles_in_hz_ + other.cycles_in_hz_);
  }

  Freq &operator+=(const Freq &other) noexcept {
    cycles_in_hz_ += other.cycles_in_hz_;
    return *this;
  }

  Freq operator*(Scalar multiplier) const noexcept {
    return Freq::Hertz(static_cast<Cycle>(cycles_in_hz_ * multiplier));
  }

  Freq &operator*=(Scalar multiplier) noexcept {
    cycles_in_hz_ = static_cast<Cycle>(cycles_in_hz_ * multiplier);
    return *this;
  }

  constexpr bool operator<(const Freq &other) const noexcept {
    return cycles_in_hz_ < other.cycles_in_hz_;
  }

  constexpr bool operator==(const Freq &other) const noexcept {
    return cycles_in_hz_ == other.cycles_in_hz_;
  }

private:
  Cycle cycles_in_hz_;
};

inline constexpr Freq Time::getFrequency() const noexcept {
  return Freq::Hertz(static_cast<Cycle>(getFreqInHertz()));
}

inline constexpr Time Freq::getPeriod() const noexcept {
  return Time::PicoSec(static_cast<Tick>(static_cast<Scalar>(Unit::THz) /
                                         static_cast<Scalar>(cycles_in_hz_)));
}
} // namespace types
} // namespace xr5

#endif // !TYPES_H_