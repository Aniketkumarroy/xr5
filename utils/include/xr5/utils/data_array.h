#ifndef DATA_ARRAY_
#define DATA_ARRAY_
#include "types.h"
#include <array>

namespace xr5 {
namespace utils {

template <typename Type, size_t Size> class DataArray {
  using DataType = Type;
  static constexpr size_t size = Size;

public:
  DataArray() = default;
  ~DataArray() = default;

  DataType &operator[](size_t addr) { return buffer_[addr]; }
  void setAll(const DataType &value) { buffer_.fill(value); }

private:
  std::array<DataType, Size> buffer_;
};

} // namespace utils
} // namespace xr5

#endif