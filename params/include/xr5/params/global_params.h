#ifndef GLOBAL_PARAMS_H_
#define GLOBAL_PARAMS_H_

#include "xr5/utils/logger.h"
#include "xr5/utils/types.h"
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
  template <typename T> static void init(const std::string &config_root) {
    static std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);
    if (params_ != nullptr) {
      auto logger = xr5::utils::Logger::getInstance();
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

  /**
   * TODO: instead of hardcoding the unit to picosec, make it flexible to be
   * auto adjust according to simulator's simulated time granularity
   */
  inline xr5::types::Tick getDramDataReceiveDelayInPicoSec() const {
    return dram_data_receive_delay_.picosec();
  }

  inline const xr5::types::Time &getDramDataReceiveDelay() const {
    return dram_data_receive_delay_;
  }

  inline xr5::types::Tick getDramAddrReceiveDelayInPicoSec() const {
    return dram_addr_receive_delay_.picosec();
  }

  inline const xr5::types::Time &getDramAddrReceiveDelay() const {
    return dram_addr_receive_delay_;
  }

  inline xr5::types::Tick getDramCmdReceiveDelayInPicoSec() const {
    return dram_cmd_receive_delay_.picosec();
  }

  inline const xr5::types::Time &getDramCmdReceiveDelay() const {
    return dram_cmd_receive_delay_;
  }

  inline const xr5::types::Size &getDramCapacity() const {
    return dram_capacity_;
  }

private:
  explicit GlobalParams(const std::string &config_root)
      : config_root_(config_root) {}
  static std::shared_ptr<GlobalParams> params_;
  std::string config_root_;

  xr5::types::Time dram_data_receive_delay_;
  xr5::types::Time dram_addr_receive_delay_;
  xr5::types::Time dram_cmd_receive_delay_;
  xr5::types::Size dram_capacity_;
};
} // namespace params
} // namespace xr5
#endif // GLOBAL_PARAMS_H_