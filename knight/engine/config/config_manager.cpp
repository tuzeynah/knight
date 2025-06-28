#include "config_manager.hpp"
#include <fstream>

void c_config_manager::initialize() {
    if (PWSTR appdata_path; SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &appdata_path)))
    {
        path = appdata_path;
        path /= "cavaleiro";

        CoTaskMemFree(appdata_path);
    }

    if (std::filesystem::is_directory(path))
    {
        std::transform
        (
            std::filesystem::directory_iterator{ path },
            std::filesystem::directory_iterator{ },
            std::back_inserter(configs),
            [](const auto& entry) { return entry.path().filename().string(); }
        );
    }
}

void c_config_manager::add(std::string name)
{
    if (!(name.empty()) && std::find(std::cbegin(configs), std::cend(configs), name) == std::cend(configs))
        configs.emplace_back(name);
}

void c_config_manager::remove(size_t id)
{
    std::filesystem::remove(path / configs[id]);
    configs.erase(configs.cbegin() + id);
}

void c_config_manager::rename(size_t item, std::string new_name)
{
    std::filesystem::rename(path / configs[item], path / new_name);
    configs[item] = new_name;
}


void c_config_manager::open_folder() {
    if (PIDLIST_ABSOLUTE pidl; SUCCEEDED(SHParseDisplayName(path.wstring().data(), 0, &pidl, 0, 0)))
    {
        ITEMIDLIST id_null = { 0 };
        LPCITEMIDLIST pidl_null[1] = { &id_null };
        SHOpenFolderAndSelectItems(pidl, 1, pidl_null, 0);
        ILFree(pidl);
    }
}

/*
not the best way but works really good
*/
void c_config_manager::save(size_t id) const {
    if (!std::filesystem::is_directory(path))
    {
        std::filesystem::remove(path);
        std::filesystem::create_directory(path);
    }

    std::ofstream out{ path / configs[id] };

    if (!out.good())
        return;

    json j;

    j["menu"] = {
        {"watermark", Menu::Watermark},
        {"accent", {Menu::Accent[0], Menu::Accent[1], Menu::Accent[2], Menu::Accent[3]}}
    };

    j["aim_assist"] = {
        {"enabled", implement::combat::aim_assist::enabled},
        {"visibility_check", implement::combat::aim_assist::visibility_check},
        {"invisible_check", implement::combat::aim_assist::invisible_check},
        {"only_sprinting", implement::combat::aim_assist::only_sprinting},
        {"only_clicking", implement::combat::aim_assist::only_clicking},
        {"key_bind", implement::combat::aim_assist::key_bind},
        {"fov", implement::combat::aim_assist::max_angle},
        {"speed", implement::combat::aim_assist::speed},
        {"distance", implement::combat::aim_assist::distance},
    };

    j["auto_clicker"] = {
        {"enabled", implement::combat::auto_clicker::enabled},
        {"average_cps", implement::combat::auto_clicker::average_cps},
        {"persistence", implement::combat::auto_clicker::persistence},
        {"persistence_value", implement::combat::auto_clicker::persistence_value},
        {"spikes", implement::combat::auto_clicker::spikes},
        {"spike_chance", implement::combat::auto_clicker::spike_chance},
        {"spike_add", implement::combat::auto_clicker::spike_add},
        {"drops", implement::combat::auto_clicker::drops},
        {"drop_chance", implement::combat::auto_clicker::drop_chance},
        {"drop_remove", implement::combat::auto_clicker::drop_remove},
        {"key_bind", implement::combat::auto_clicker::key_bind}
    };

    j["hit_boxes"] = {
        {"enabled", implement::combat::hit_boxes::enabled},
        {"key_bind", implement::combat::hit_boxes::key_bind},
        {"x_expand", implement::combat::hit_boxes::x_expand},
        {"y_expand", implement::combat::hit_boxes::y_expand}
    };

    j["reach"] = {
        {"enabled", implement::combat::reach::enabled},
        {"key_bind", implement::combat::reach::key_bind},
        {"distance", implement::combat::reach::distance}
    };

    j["velocity"] = {
        {"enabled", implement::combat::velocity::enabled},
        {"key_bind", implement::combat::velocity::key_bind},
        {"horizontal", implement::combat::velocity::horizontal},
        {"vertical", implement::combat::velocity::vertical}
    };

    j["fast_mine"] = {
        {"enabled", implement::utility::fast_mine::enabled},
        {"key_bind", implement::utility::fast_mine::key_bind},
        {"no_break_delay", implement::utility::fast_mine::no_break_delay},
        {"speed_multiplier", implement::utility::fast_mine::speed_multiplier}
    };

    j["fast_place"] = {
        {"enabled", implement::utility::fast_place::enabled},
        {"key_bind", implement::utility::fast_place::key_bind}
    };

    j["no_hit_delay"] = {
        {"enabled", implement::utility::no_hit_delay::enabled},
        {"key_bind", implement::utility::no_hit_delay::key_bind}
    };

    j["no_jump_delay"] = {
        {"enabled", implement::utility::no_jump_delay::enabled},
        {"key_bind", implement::utility::no_jump_delay::key_bind}
    };

    j["array_list"] = {
        {"enabled", implement::visuals::array_list::enabled},
        {"key_bind", implement::visuals::array_list::key_bind},
        {"color_bar", implement::visuals::array_list::color_bar},
        {"rainbow", implement::visuals::array_list::rainbow},
        {"background", implement::visuals::array_list::background},
        {"color", implement::visuals::array_list::color},
        {"position", implement::visuals::array_list::position}
    };

    j["esp"] = {
        {"enabled", implement::visuals::esp::enabled},
        {"key_bind", implement::visuals::esp::key_bind},
        {"box", implement::visuals::esp::Box},
        {"box_color", {implement::visuals::esp::BoxColor[0],
                      implement::visuals::esp::BoxColor[1],
                      implement::visuals::esp::BoxColor[2],
                      implement::visuals::esp::BoxColor[3]}},
        {"filled_box", implement::visuals::esp::FilledBox},
        {"filled_box_color", {implement::visuals::esp::FilledBoxColor[0],
                             implement::visuals::esp::FilledBoxColor[1],
                             implement::visuals::esp::FilledBoxColor[2]}},
        {"outline", implement::visuals::esp::Outline},
        {"outline_color", {implement::visuals::esp::OutlineColor[0],
                          implement::visuals::esp::OutlineColor[1],
                          implement::visuals::esp::OutlineColor[2],
                          implement::visuals::esp::OutlineColor[3]}},
        {"text", implement::visuals::esp::Text},
        {"text_size", implement::visuals::esp::TextSize},
        {"text_color", {implement::visuals::esp::TextColor[0],
                       implement::visuals::esp::TextColor[1],
                       implement::visuals::esp::TextColor[2],
                       implement::visuals::esp::TextColor[3]}},
        {"fade_distance", implement::visuals::esp::FadeDistance},
        {"health_bar", implement::visuals::esp::HealthBar}
    };

    if (out.is_open()) {
        out << j.dump(4);
        out.close();
    }
}

void c_config_manager::load(size_t id) {
    if (!std::filesystem::is_directory(path))
    {
        std::filesystem::remove(path);
        std::filesystem::create_directory(path);
    }

    std::ifstream in{ path / configs[id] };

    if (!in.good())
        return;

    json j;
    in >> j;

    if (j.contains("menu")) {
        Menu::Watermark = j["menu"].value("watermark", true);


        auto accentArray = j["menu"]["accent"];
        if (accentArray.is_array() && accentArray.size() == 4) {
            Menu::Accent[0] = j["menu"]["accent"][0].get<float>();
            Menu::Accent[1] = j["menu"]["accent"][1].get<float>();
            Menu::Accent[2] = j["menu"]["accent"][2].get<float>();
            Menu::Accent[3] = j["menu"]["accent"][3].get<float>();

            ImVec4* colors = ImGui::GetStyle().Colors;
            colors[ImGuiCol_SliderGrab] = ImVec4(Menu::Accent[0], Menu::Accent[1], Menu::Accent[2], Menu::Accent[3]);
            colors[ImGuiCol_SliderGrabActive] = Menu::Darken(colors[ImGuiCol_SliderGrab], 0.4f);
        }
    }

    if (j.contains("aim_assist")) {
        // better way to do it
        const auto& aa = j["aim_assist"];

        implement::combat::aim_assist::enabled = aa.value("enabled", false);
        implement::combat::aim_assist::visibility_check = aa.value("visibility_check", true);
        implement::combat::aim_assist::invisible_check = aa.value("invisible_check", true);
        implement::combat::aim_assist::only_sprinting = aa.value("only_sprinting", false);
        implement::combat::aim_assist::only_clicking = aa.value("only_clicking", true);
        implement::combat::aim_assist::key_bind = aa.value("key_bind", 0x0);
        implement::combat::aim_assist::max_angle = aa.value("fov", 35.0f);
        implement::combat::aim_assist::speed = aa.value("speed", 15.0f);
        implement::combat::aim_assist::distance = aa.value("distance", 4.0f);
    }

    if (j.contains("auto_clicker")) {
        implement::combat::auto_clicker::enabled = j["auto_clicker"].value("enabled", false);
        implement::combat::auto_clicker::average_cps = j["auto_clicker"].value("average_cps", 15.0f);
        implement::combat::auto_clicker::persistence = j["auto_clicker"].value("persistence", false);
        implement::combat::auto_clicker::persistence_value = j["auto_clicker"].value("persistence_value", 2.5f);
        implement::combat::auto_clicker::spikes = j["auto_clicker"].value("spikes", false);
        implement::combat::auto_clicker::spike_chance = j["auto_clicker"].value("spike_chance", 5);
        implement::combat::auto_clicker::spike_add = j["auto_clicker"].value("spike_add", 2.5f);
        implement::combat::auto_clicker::drops = j["auto_clicker"].value("drops", false);
        implement::combat::auto_clicker::drop_chance = j["auto_clicker"].value("drop_chance", 15);
        implement::combat::auto_clicker::drop_remove = j["auto_clicker"].value("drop_remove", 3.5f);
        implement::combat::auto_clicker::key_bind = j["auto_clicker"].value("key_bind", 0x0);
    }

    if (j.contains("hit_boxes")) {
        implement::combat::hit_boxes::enabled = j["hit_boxes"].value("enabled", false);
        implement::combat::hit_boxes::key_bind = j["hit_boxes"].value("key", 0x0);
        implement::combat::hit_boxes::x_expand = j["hit_boxes"].value("x_expand", 0.15f);
        implement::combat::hit_boxes::y_expand = j["hit_boxes"].value("y_expand", 0.15f);
    }

    if (j.contains("reach")) {
        implement::combat::reach::enabled = j["reach"].value("enabled", false);
        implement::combat::reach::key_bind = j["reach"].value("key_bind", 0x0);
        implement::combat::reach::distance = j["reach"].value("distance", 3.6f);
    }

    if (j.contains("velocity")) {
        implement::combat::velocity::enabled = j["velocity"].value("enabled", false);
        implement::combat::velocity::key_bind = j["velocity"].value("key_bind", 0x0);
        implement::combat::velocity::horizontal = j["velocity"].value("horizontal", 100.0);
        implement::combat::velocity::vertical = j["velocity"].value("vertical", 100.0);
    }

    if (j.contains("fast_mine")) {
        implement::utility::fast_mine::enabled = j["fast_mine"].value("enabled", false);
        implement::utility::fast_mine::key_bind = j["fast_mine"].value("key_bind", 0x0);
        implement::utility::fast_mine::no_break_delay = j["fast_mine"].value("no_break_delay", false);
        implement::utility::fast_mine::speed_multiplier = j["fast_mine"].value("speed_multiplier", 0.15f);
    }

    if (j.contains("fast_place")) {
        implement::utility::fast_place::enabled = j["fast_place"].value("enabled", false);
        implement::utility::fast_place::key_bind = j["fast_place"].value("key_bind", 0x0);
    }

    if (j.contains("no_hit_delay")) {
        implement::utility::no_hit_delay::enabled = j["no_hit_delay"].value("enabled", false);
        implement::utility::no_hit_delay::key_bind = j["no_hit_delay"].value("key_bind", 0x0);
    }

    if (j.contains("no_jump_delay")) {
        implement::utility::no_jump_delay::enabled = j["no_jump_delay"].value("enabled", false);
        implement::utility::no_jump_delay::key_bind = j["no_jump_delay"].value("key", 0x0);
    }

    if (j.contains("array_list")) {
        implement::visuals::array_list::enabled = j["array_list"].value("enabled", false);
        implement::visuals::array_list::key_bind = j["array_list"].value("key_bind", 0x0);
        implement::visuals::array_list::color_bar = j["array_list"].value("color_bar", true);
        implement::visuals::array_list::rainbow = j["array_list"].value("rainbow", true);
        implement::visuals::array_list::background = j["array_list"].value("background", true);

        if (j["array_list"].contains("color")) {
            auto colorArray = j["array_list"]["color"];
            if (colorArray.is_array() && colorArray.size() == 4) {
                implement::visuals::array_list::color[0] = colorArray[0].get<float>();
                implement::visuals::array_list::color[1] = colorArray[1].get<float>();
                implement::visuals::array_list::color[2] = colorArray[2].get<float>();
                implement::visuals::array_list::color[3] = colorArray[3].get<float>();
            }
        }
        implement::visuals::array_list::position = j["array_list"].value("position", 0);
    }

    if (j.contains("esp")) {
        implement::visuals::esp::enabled = j["esp"].value("enabled", false);
        implement::visuals::esp::key_bind = j["esp"].value("key_bind", 0x0);
        implement::visuals::esp::Box = j["esp"].value("box", true);
        if (j["esp"].contains("box_color") && j["esp"]["box_color"].is_array() && j["esp"]["box_color"].size() == 4) {
            auto& color = j["esp"]["box_color"];
            implement::visuals::esp::BoxColor[0] = color[0].get<float>();
            implement::visuals::esp::BoxColor[1] = color[1].get<float>();
            implement::visuals::esp::BoxColor[2] = color[2].get<float>();
            implement::visuals::esp::BoxColor[3] = color[3].get<float>();
        }

        implement::visuals::esp::FilledBox = j["esp"].value("filled_box", true);
        if (j["esp"].contains("filled_box_color") && j["esp"]["filled_box_color"].is_array() && j["esp"]["filled_box_color"].size() == 3) {
            auto& color = j["esp"]["filled_box_color"];
            implement::visuals::esp::FilledBoxColor[0] = color[0].get<float>();
            implement::visuals::esp::FilledBoxColor[1] = color[1].get<float>();
            implement::visuals::esp::FilledBoxColor[2] = color[2].get<float>();
        }

        implement::visuals::esp::Outline = j["esp"].value("outline", true);
        if (j["esp"].contains("outline_color") && j["esp"]["outline_color"].is_array() && j["esp"]["outline_color"].size() == 4) {
            auto& color = j["esp"]["outline_color"];
            implement::visuals::esp::OutlineColor[0] = color[0].get<float>();
            implement::visuals::esp::OutlineColor[1] = color[1].get<float>();
            implement::visuals::esp::OutlineColor[2] = color[2].get<float>();
            implement::visuals::esp::OutlineColor[3] = color[3].get<float>();
        }

        implement::visuals::esp::Text = j["esp"].value("text", true);
        implement::visuals::esp::TextSize = j["esp"].value("text_size", 18.0f);
        if (j["esp"].contains("text_color") && j["esp"]["text_color"].is_array() && j["esp"]["text_color"].size() == 4) {
            auto& color = j["esp"]["text_color"];
            implement::visuals::esp::TextColor[0] = color[0].get<float>();
            implement::visuals::esp::TextColor[1] = color[1].get<float>();
            implement::visuals::esp::TextColor[2] = color[2].get<float>();
            implement::visuals::esp::TextColor[3] = color[3].get<float>();
        }

        implement::visuals::esp::FadeDistance = j["esp"].value("fade_distance", 3.0f);
        implement::visuals::esp::HealthBar = j["esp"].value("health_bar", true);
    }

    in.close();
}