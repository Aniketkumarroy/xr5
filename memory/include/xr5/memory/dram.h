#ifndef DRAM_H_
#define DRAM_H_

#include "xr5/memory/memory_base.h"
#include "xr5/utils/logger.h"
#include "xr5/utils/types.h"

namespace xr5 {
namespace memory {

class Dram : public MemoryBase<xr5::types::Address> {
public:
  struct Params {
    xr5::types::Size capacity;
    uint8_t io_width_of_chip;
    uint8_t no_of_chips_per_rank;
    uint8_t rank;
    uint8_t burst_length;
  };

  struct Latency {
    xr5::types::TimePS t_RCD;
    xr5::types::TimePS t_RAS;
    xr5::types::TimePS t_RP;
    xr5::types::TimePS t_RC;
    xr5::types::TimePS t_CL;
    xr5::types::TimePS t_CWL;
    xr5::types::TimePS t_CCD;
    xr5::types::TimePS t_RRD;
    xr5::types::TimePS t_FAW;
    xr5::types::TimePS t_WTR;
    xr5::types::TimePS t_WR;
    xr5::types::TimePS t_RFC;
    xr5::types::TimePS t_REFI;
  };

  Dram() = delete;
  Dram(const Params &dram_params, const Latency &latency,
       const MemoryObject::Params &mem_params)
      : MemoryBase<xr5::types::Address>(
            0, dram_params.capacity.bytes() / xr5::types::WordSize, mem_params),
        capacity_(dram_params.capacity),
        io_width_of_chip_(dram_params.io_width_of_chip),
        no_of_chips_per_rank_(dram_params.no_of_chips_per_rank),
        rank_(dram_params.rank), burst_length_(dram_params.burst_length) {
    if (io_width_of_chip_ * no_of_chips_per_rank_ != xr5::types::WordSize * 8) {
      auto logger = xr5::utils::Logger::getInstance();
      logger->error("[Dram::Dram] CONFIG ERROR!! the data bus width of the "
                    "Dram is calculated as 'io width of each chip x no of "
                    "chips per rank'. however the provided params equals "
                    "to {} width of data bus, not compatible for {}-bit system",
                    io_width_of_chip_ * no_of_chips_per_rank_,
                    xr5::types::WordSize * 8);

      setLatency(latency);
    }
  }
  ~Dram() = default;

  void setLatency(const Latency &latency);

  const xr5::types::Size &getCapacity() { return capacity_; }
  uint8_t getIOWidthOfEachChip() { return io_width_of_chip_; }
  uint8_t getNoOfChipsPerRank() { return no_of_chips_per_rank_; }
  uint8_t getRank() { return rank_; }
  uint8_t getBurstLength() { return burst_length_; }

  void handleDataPacket() override {};
  void handleAddrPacket() override {};
  void handleCmdPacket() override;

private:
  const xr5::types::Size capacity_;
  const uint8_t io_width_of_chip_;
  const uint8_t no_of_chips_per_rank_;
  const uint8_t rank_;
  const uint8_t burst_length_;

  xr5::types::Tick t_RCD_;
  xr5::types::Tick t_RAS_;
  xr5::types::Tick t_RP_;
  xr5::types::Tick t_RC_;
  xr5::types::Tick t_CL_;
  xr5::types::Tick t_CWL_;
  xr5::types::Tick t_CCD_;
  xr5::types::Tick t_RRD_;
  xr5::types::Tick t_FAW_;
  xr5::types::Tick t_WTR_;
  xr5::types::Tick t_WR_;
  xr5::types::Tick t_RFC_;
  xr5::types::Tick t_REFI_;

  xr5::types::DramAddr active_row_;
  xr5::types::Tick t_active_row_ = 0;
};

} // namespace memory

} // namespace xr5

#endif
