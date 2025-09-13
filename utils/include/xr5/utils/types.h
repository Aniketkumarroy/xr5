#ifndef TYPES_H_
#define TYPES_H_

#include "xr5/config.h"
#include <cstdint>
#include <cstring>
#include <limits>

namespace xr5 {
namespace types {

using Tick = uint64_t;
#define MAX_TICK std::numeric_limits<xr5::types::Tick>::max()

using Address64 = uint64_t;
using Address32 = uint32_t;
using Byte = uint8_t;
using TimeStamp = double;

#if XR5_ADDR_WIDTH == 64
using Address = Address64;
#elif XR5_ADDR_WIDTH == 32
using Address = Address32;
#else
#error "Unsupported address width"
#endif

using Word = Address;
uint64_t WordSize = sizeof(Address);

class Size {
public:
  enum class Unit : uint64_t {
    B = 1ULL,
    KB = 1024ULL * B,
    MB = 1024ULL * KB,
    GB = 1024ULL * MB
  };

  Size() : bytes_(0) {}
  Size(uint64_t value, Unit unit)
      : bytes_(value * static_cast<uint64_t>(unit)) {}

  ~Size() = default;

  static Size Byte(uint64_t b) { return Size(b, Unit::B); }
  static Size KiloByte(uint64_t k) { return Size(k, Unit::KB); }
  static Size MegaByte(uint64_t m) { return Size(m, Unit::MB); }
  static Size GigaByte(uint64_t g) { return Size(g, Unit::GB); }

  uint64_t bytes() const { return bytes_; }

  double as(Unit unit) const {
    return static_cast<double>(bytes_) / static_cast<uint64_t>(unit);
  }

  Size operator+(const Size &other) const {
    return Size::Byte(bytes_ + other.bytes_);
  }

  Size &operator+=(const Size &other) {
    bytes_ += other.bytes_;
    return *this;
  }

  bool operator<(const Size &other) const { return bytes_ < other.bytes_; }
  bool operator==(const Size &other) const { return bytes_ == other.bytes_; }

private:
  uint64_t bytes_;
};

class Time {
public:
  enum class Unit : Tick {
    PS = 1ULL,
    NS = 1000ULL * PS,
    US = 1000ULL * NS,
    MS = 1000ULL * US,
    S = 1000ULL * MS
  };

  Time() : picoseconds_(0) {}
  Time(Tick value, Unit unit)
      : picoseconds_(value * static_cast<uint64_t>(unit)) {}

  static Time Sec(uint64_t s) { return Time(s, Unit::S); }
  static Time MiliSec(uint64_t ms) { return Time(ms, Unit::MS); }
  static Time MicroDec(uint64_t us) { return Time(us, Unit::US); }
  static Time NanoSec(uint64_t ns) { return Time(ns, Unit::NS); }
  static Time PicoSec(uint64_t ps) { return Time(ps, Unit::NS); }

  Tick picosec() const { return picoseconds_; }

  double as(Unit unit) const {
    return static_cast<double>(picoseconds_) / static_cast<Tick>(unit);
  }

  Time operator+(const Time &other) const {
    return Time::PicoSec(picoseconds_ + other.picoseconds_);
  }

  Time &operator+=(const Time &other) {
    picoseconds_ += other.picoseconds_;
    return *this;
  }

  Time &operator++() {
    ++picoseconds_;
    return *this;
  }

  bool operator<(const Time &other) const {
    return picoseconds_ < other.picoseconds_;
  }

  bool operator==(const Time &other) const {
    return picoseconds_ == other.picoseconds_;
  }

private:
  Tick picoseconds_;
};
} // namespace types
} // namespace xr5

#endif