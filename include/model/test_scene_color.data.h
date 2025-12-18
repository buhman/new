#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t _binary_model_test_scene_color_data_start __asm("_binary_model_test_scene_color_data_start");
extern uint32_t _binary_model_test_scene_color_data_end __asm("_binary_model_test_scene_color_data_end");

#define model_test_scene_color_data_start ((const char *)&_binary_model_test_scene_color_data_start)
#define model_test_scene_color_data_end ((const char *)&_binary_model_test_scene_color_data_end)
#define model_test_scene_color_data_size (model_test_scene_color_data_end - model_test_scene_color_data_start)

#ifdef __cplusplus
}
#endif
