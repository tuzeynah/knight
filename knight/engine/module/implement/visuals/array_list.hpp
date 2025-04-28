#pragma once

#include "../../../../sdk/sdk.h"
#include "../../../game_data.h"
#include "../../../java/java.h"
#include "../../../util/utils.hpp"
#include "../../../../ext/imgui/imgui.h"
#include "../../../menu/menu.h"

namespace implement {
    namespace visuals {
        struct array_list {
            static void update();

            static void handle_render();
            static void handle_menu();

			static inline bool show_opt = false;

            static inline bool enabled = false;
            static inline int key_bind = 0x0;

            static inline bool color_bar = true;
            static inline bool rainbow = true;
            static inline bool background = true;

            static inline float rainbow_saturation{ 0.5f };
            static inline float color[4]{ 89.f / 255.f, 161.f / 255.f, 236.f / 255.f, 255.f / 255.f };
			 
			static inline int position = 0;
			static inline const char* position_list[2]{ "top left", "top right" };
        private:
            static inline std::once_flag thread_once;
        };
    }
}

