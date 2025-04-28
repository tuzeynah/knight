#pragma once

#include "implement/combat/aim_assist.hpp"
#include "implement/combat/hit_boxes.hpp"
#include "implement/combat/velocity.hpp"
#include "implement/combat/reach.hpp"
#include "implement/combat/auto_clicker.hpp"

#include "implement/utility/no_hit_delay.hpp"
#include "implement/utility/no_jump_delay.hpp"
#include "implement/utility/fast_place.hpp"
#include "implement/utility/fast_mine.hpp"

#include "implement/visuals/esp.hpp"
#include "implement/visuals/array_list.hpp"

#include "..\game_data.h"

#include <vector>
#include <memory>
#include <functional>

class c_module_manager {
private:
    struct module {
        std::function<void()> update_func;
        bool* enabled;
        int* key;

        void toggle() const noexcept {
            if (enabled)
                *enabled = !*enabled;
        }
    };

    std::vector<module> modules;
    bool initialized = false;
public:
    void initialize();
    void update_modules();
    void handle_keys(WPARAM wParam);
    void register_module(std::function<void()> update_func, bool* enabled_flag, int* key_bind);
};

inline auto module_manager = c_module_manager();