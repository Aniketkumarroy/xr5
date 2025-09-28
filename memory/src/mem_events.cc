#include "xr5/memory/mem_events.h"

namespace xr5 {
namespace memory {

void MemDataLatchEvent::run() {
  mem_obj_->data_packet_ = pckt_;
  mem_obj_->handleDataPacket();
}

void MemAddrLatchEvent::run() {
  mem_obj_->addr_packet_ = pckt_;
  mem_obj_->handleAddrPacket();
}

void MemCmdLatchEvent::run() {
  mem_obj_->cmd_packet_ = pckt_;
  mem_obj_->handleCmdPacket();
}

} // namespace memory

} // namespace xr5
