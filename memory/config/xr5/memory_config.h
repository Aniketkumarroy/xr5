#ifndef MEMORY_CONFIG_H_
#define MEMORY_CONFIG_H_
#include "xr5/utils/segmented_array_memory_manager.h"

template <typename Data>
using DefaultMemoryManager = xr5::utils::SegmentedArrayMemoryManager<
    xr5::utils::DataArray<Data, XR5_MEMORY_MANAGER_SEGMENT_SIZE>>;

#endif