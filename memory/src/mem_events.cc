#include "xr5/memory/mem_events.h"

namespace xr5 {
namespace memory {

void DataLatchEvent::run() {
  mem_obj_->data_packet_.word = word_;
  mem_obj_->handleDataPacket();
}

void AddrLatchEvent::run() {
  mem_obj_->addr_packet_.addr = addr_;
  mem_obj_->addr_packet_.data.dram_addr = dram_addr_;
  mem_obj_->handleAddrPacket();
}

void CmdLatchEvent::run() {
  mem_obj_->cmd_packet_.data.dram_cmd = dram_cmd_;
  mem_obj_->handleCmdPacket();
}

} // namespace memory

} // namespace xr5
