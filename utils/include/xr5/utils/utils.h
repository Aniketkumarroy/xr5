#ifndef UTILS_H_
#define UTILS_H_
#include "types.h"
#include <memory>

namespace xr5 {
namespace utils {
template <typename Base, typename Derived, typename... Args>
typename Base::Ptr make_ptr(Args &&...args) {
  using PtrType = typename Base::Ptr;

  if constexpr (std::is_same_v<PtrType, std::shared_ptr<Base>>) {
    return std::make_shared<Derived>(std::forward<Args>(args)...);
  } else if constexpr (std::is_same_v<PtrType, std::unique_ptr<Base>>) {
    return std::make_unique<Derived>(std::forward<Args>(args)...);
  } else if constexpr (std::is_same_v<PtrType, Base *>) {
    return new Derived(std::forward<Args>(args)...);
  } else {
    static_assert(!sizeof(Base), "Unsupported Base::Ptr type, should be one of "
                                 "std::shared_ptr, std::unique_ptr or Base*");
  }
}

template <typename T> inline T *get_raw_ptr(const std::shared_ptr<T> &obj) {
  return obj.get();
}

template <typename T> inline T *get_raw_ptr(const std::unique_ptr<T> &obj) {
  return obj.get();
}

template <typename T> inline T *get_raw_ptr(const T *obj) { return obj; }

} // namespace utils

} // namespace xr5

#endif