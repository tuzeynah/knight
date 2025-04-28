#pragma once

#include "../../../../sdk/sdk.h"
#include "../../../game_data.h"
#include "../../../java/java.h"
#include "../../../../ext/imgui/imgui.h"
#include "../../../menu/menu.h"

namespace implement {
    namespace utility {
        struct no_hit_delay {
            static void update();
            static void handle_menu();

            static inline bool enabled = false;

            static inline int key_bind = 0x0;
        };
    }
}