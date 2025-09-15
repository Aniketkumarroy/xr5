#ifndef GLOBAL_PARAMS_H_
#define GLOBAL_PARAMS_H_

#include "xr5/utils/logger.h"
#include <memory>
#include <mutex>
#include <string>

namespace xr5 {
namespace params {

/* Singleton params class */

class GlobalParams {
public:
  // Disable constructor
  GlobalParams() = delete;
  /**
   * @brief As this class should not be cloneable.
   */
  GlobalParams(const GlobalParams &other) = delete;

  /**
   * @brief As this class should not be assignable.
   */
  GlobalParams &operator=(const GlobalParams &) = delete;

  /**
   * @brief initialize the params
   *
   * @param config_root root folder of the config
   */
  template <typename T>
  static void init(const std::string &config_root) {
    static std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);
    if (params_ != nullptr) {
      auto logger = xr5::utils::Logger::GetInstance();
      logger->warn("[GlobalParams::init] params is already initialized");
      return;
    }
    // struct TShared : public T {};
    params_ = std::make_shared<T>(config_root);
  }

  /**
   * @brief Get the GlobalParam object
   *
   * @return pointer to the param object
   */
  static std::shared_ptr<GlobalParams> GetInstance() { return params_; }

private:
  explicit GlobalParams(const std::string &config_root)
      : config_root_(config_root) {}
  static std::shared_ptr<GlobalParams> params_;
  std::string config_root_;
};
} // namespace params
} // namespace xr5
#endif // GLOBAL_PARAMS_H_