#pragma once

#include "../../../../sdk/sdk.h"
#include "../../../game_data.h"
#include "../../../java/java.h"
#include "../../../util/utils.hpp"
#include "../../../../ext/imgui/imgui.h"
#include "../../../menu/menu.h"

#include <vector>
#include <string>
#include <random>

#include <MMSystem.h>
#include <timeapi.h>
#pragma comment(lib, "winmm.lib")

#define precise_sleep(ms) { timeBeginPeriod(1); util::number::precise_timer_sleep( static_cast<double>( ms / 1000 ) ); timeEndPeriod(1); }

namespace implement {
    namespace combat {

        struct auto_clicker {
            static void update();
            static void random_behavior() noexcept;
            static void handle_menu();

            static inline bool show_opt = false;

            static inline bool enabled = false;
            static inline float average_cps = 15;

            static inline bool persistence = false;
            static inline float persistence_value = 2.5f;

            static inline bool spikes = false;
            static inline int spike_chance = 5;
            static inline float spike_add = 2.5f;

            static inline bool drops = false;
            static inline int drop_chance = 15;
            static inline float drop_remove = 3.5f;

            static inline int key_bind = 0x0;

            static inline float delay = 0.f;
            static inline float random = 0.f;

            static inline bool should_update = false;
        private:
            static inline std::once_flag thread_once;
        };
    }
}