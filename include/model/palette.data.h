#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t _binary_model_palette_data_start __asm("_binary_model_palette_data_start");
extern uint32_t _binary_model_palette_data_end __asm("_binary_model_palette_data_end");

#define model_palette_data_start ((const char *)&_binary_model_palette_data_start)
#define model_palette_data_end ((const char *)&_binary_model_palette_data_end)
#define model_palette_data_size ((uintptr_t)model_palette_data_end - (uintptr_t)model_palette_data_start)

#ifdef __cplusplus
}
#endif
