#ifndef MEMORY_CONFIG_H_
#define MEMORY_CONFIG_H_
#include "xr5/utils/segmented_array_memory_manager.h"

namespace xr5 {
namespace memory {

/**
 * @brief Memory manager type used internally by the DRAM simulator.
 *
 * This class is responsible for handling all internal memory allocations,
 * deallocations, and address-to-data lookups required by the DRAM simulator.
 *
 * You can replace this with your own implementation if you need a custom
 * memory manager. **However, your implementation must provide the following
 * methods with the same signatures**:
 *
 * @note **Required Interface** (example):
 * @code
 * void* allocate(std::size_t bytes);
 * void  free(void* ptr);
 * void* getAddress(std::size_t index) const;
 * std::size_t capacity() const;
 * @endcode
 *
 * @attention
 * - Allocation and deallocation **must be thread-safe** if used in
 *   multi-threaded simulations.
 * - `getAddress` should return a **stable pointer** for the lifetime of the
 *   allocation.
 * - Violating these requirements will lead to undefined behavior in the
 *   DRAM simulator.
 *
 * @tparam MemoryManager The concrete memory manager type.
 */
using MemoryManager = xr5::utils::SegmentedArrayMemoryManager;
} // namespace memory

} // namespace xr5

#endif