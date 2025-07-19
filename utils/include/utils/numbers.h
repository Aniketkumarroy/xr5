#ifndef NUMBERS_H_
#define NUMBERS_H_
#include <cstdint>
#include <stdexcept>
#include <string>
namespace xr5 {
namespace utils {

/**
 * @brief converting 1 byte data into decimal
 *
 * @tparam T template param of either 1 byte unsigned or signed int
 * @param[in] hex hexadecimal number as a string, \c should only contain the
 * hexadecimal characters, any preciding 0x should be removed
 * @return T value
 */
template <typename T>
typename std::enable_if<
    (std::is_same_v<T, uint8_t> || std::is_same_v<T, int8_t>), T>::type
hexToDecimal(const std::string &hex) {
  if (hex.length() > 2) {
    throw std::invalid_argument("Too many hex characters for 1-byte type");
  }

  if constexpr (std::is_same_v<T, uint8_t>) {
    return static_cast<uint8_t>(std::stoul(hex, nullptr, 16));
  } else {
    return static_cast<int8_t>(std::stoul(hex, nullptr, 16));
  }
}

/**
 * @brief converting 8 byte data into decimal
 *
 * @tparam T template param of either 8 byte unsigned or signed int
 * @param[in] hex hexadecimal number as a string, should only contain the
 * hexadecimal characters, any preciding 0x should be removed
 * @return T value
 */
template <typename T>
typename std::enable_if<
    (std::is_same_v<T, uint64_t> || std::is_same_v<T, int64_t>), T>::type
hexToDecimal(const std::string &hex) {
  if (hex.length() > 16) {
    throw std::invalid_argument("Too many hex characters for 8-byte type");
  }

  if constexpr (std::is_same_v<T, uint64_t>) {
    return static_cast<uint64_t>(std::stoull(hex, nullptr, 16));
  } else {
    return static_cast<int64_t>(std::stoull(hex, nullptr, 16));
  }
}

/**
 * @brief converting hexadecimal data as a string into decimal
 *
 * @tparam T template param of either 1/2/4/8 byte unsigned/signed int
 * @param[in] hex hexadecimal number as a const c string, should only contain
 * the hexadecimal characters, any preciding 0x should be removed
 * @return T decimal value
 */
template <typename T> T hexToDecimal(const char *hex) {
  return hexToDecimal<T>(std::string(hex));
}

} // namespace utils
} // namespace xr5

#endif