#include "module_manager.hpp"

void c_module_manager::initialize() {
    if (initialized)
        return;

    register_module(&implement::combat::auto_clicker::update, &implement::combat::auto_clicker::enabled, &implement::combat::auto_clicker::key_bind);
    register_module(&implement::combat::reach::update, &implement::combat::reach::enabled, &implement::combat::reach::key_bind);
    register_module(&implement::combat::aim_assist::update, &implement::combat::aim_assist::enabled, &implement::combat::aim_assist::key_bind);
    register_module(&implement::combat::hit_boxes::update, &implement::combat::hit_boxes::enabled, &implement::combat::hit_boxes::key_bind);
    register_module(&implement::combat::velocity::update, &implement::combat::velocity::enabled, &implement::combat::velocity::key_bind);

    register_module(&implement::utility::fast_place::update, &implement::utility::fast_place::enabled, &implement::utility::fast_place::key_bind);
    register_module(&implement::utility::fast_mine::update, &implement::utility::fast_mine::enabled, &implement::utility::fast_mine::key_bind);
    register_module(&implement::utility::no_hit_delay::update, &implement::utility::no_hit_delay::enabled, &implement::utility::no_hit_delay::key_bind);
    register_module(&implement::utility::no_jump_delay::update, &implement::utility::no_jump_delay::enabled, &implement::utility::no_jump_delay::key_bind);

    register_module(&implement::visuals::esp::update, &implement::visuals::esp::enabled, &implement::visuals::esp::key_bind);
    register_module(&implement::visuals::array_list::update, &implement::visuals::array_list::enabled, &implement::visuals::array_list::key_bind);

    log_ok("modules initialized");

    initialized = true;
}

void c_module_manager::update_modules() {
    if (!game_data::check())
        return;

    game_data::update();

    for (auto& module : modules) {
        if (*module.enabled) {
            module.update_func();
        }
    }
} 

void c_module_manager::handle_keys(WPARAM wparam) {
    for (auto& module : modules) {
        if (*module.key == wparam) {
            module.toggle();
        }
    }
}

void c_module_manager::register_module(std::function<void()> update_func, bool* enabled_flag, int* key_flag) {
    modules.push_back({ update_func, enabled_flag, key_flag });
}