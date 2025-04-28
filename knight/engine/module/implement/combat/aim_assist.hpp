#pragma once

#include "../../../util/math/geometry.h"
#include "../../../../sdk/sdk.h"
#include "../../../game_data.h"
#include "../../../util/logger.hpp"
#include "../../../java/java.h"
#include "../../../util/math/math.h"
#include "../../../../ext/imgui/imgui.h"
#include "../../../menu/menu.h"

#include <vector>
#include <string>
#include <chrono>

namespace implement {
    namespace combat {
        struct aim_assist {
            static void update();
            static void handle_menu();

            static inline bool show_opt = false;

            static inline bool enabled = false;
            static inline bool visibility_check = true;
            static inline bool invisible_check = true;

            static inline bool only_sprinting = false;
            static inline bool only_clicking = true;

            static inline int key_bind = 0x0;

            static inline float dynamicSmooth = 8.0f;

            static inline float fov = 35.0f;
            static inline float smooth = 15.f;
            static inline float distance = 4.f;

            static inline bool adaptive = true;
            static inline float adaptive_offset = 3.f;

            static inline int target_priority = 2;
            static inline const char* target_priority_list[3]{ "distance", "health", "closest to crosshair" };
        };
    }
}