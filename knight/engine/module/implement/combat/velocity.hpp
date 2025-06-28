#pragma once

#include "../../../../sdk/sdk.h"
#include "../../../game_data.h"
#include "../../../java/java.h"
#include "../../../../ext/imgui/imgui.h"
#include "../../../menu/menu.h"

namespace implement {
    namespace combat {
        struct velocity {
            static void update();
            static void handle_menu();

            static inline bool show_opt = false;

            static inline bool enabled = false;

            static inline int key_bind = 0x0;

            static inline double horizontal = 100;
            static inline double vertical = 100;
        };
    }
}
