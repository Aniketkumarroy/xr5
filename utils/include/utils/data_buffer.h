#ifndef BYTE_BUFFER_
#define BYTE_BUFFER_
#include "types.h"
#include <array>

namespace xr5 {
namespace utils {

// TODO: take Size that is a power of 2 for more efficient operations
// constexpr bool is_power_of_2(size_t n) { return n != 0 && (n & (n - 1)) == 0;
// }
// template <size_t Size,
//           typename Enable = std::enable_if_t<is_power_of_2(Size)>>
template <typename Type, size_t Size> class DataBuffer {
  using DataType = Type;
  static constexpr size_t size = Size;

public:
  DataBuffer() = default;
  ~DataBuffer() = default;

  DataType &operator[](size_t addr) { return buffer_[addr]; }
  void setAll(const DataType &value) { buffer_.fill(value); }

private:
  std::array<DataType, Size> buffer_;
};

} // namespace utils
} // namespace xr5

#endif