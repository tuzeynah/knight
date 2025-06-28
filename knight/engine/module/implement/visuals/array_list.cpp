#include "array_list.hpp"

namespace implement::visuals {
	std::vector<std::pair<const char*, bool*>> module_array;

	void array_list::update() {

	}

    void array_list::handle_render() {
        std::call_once(thread_once, []() {
            module_array = {
                { "aim assist",     &implement::combat::aim_assist::enabled },
                { "auto clicker",   &implement::combat::auto_clicker::enabled },
                { "hit boxes",      &implement::combat::hit_boxes::enabled },
                { "reach",          &implement::combat::reach::enabled },
                { "velocity",       &implement::combat::velocity::enabled },
                { "fast mine",      &implement::utility::fast_mine::enabled },
                { "fast place",     &implement::utility::fast_place::enabled },
                { "no hit delay",   &implement::utility::no_hit_delay::enabled },
                { "no jump delay",  &implement::utility::no_jump_delay::enabled }
            };

        std::ranges::sort(module_array, [](auto& a, auto& b) {
            return ImGui::CalcTextSize(a.first).x > ImGui::CalcTextSize(b.first).x;
            });
        });

        if (!array_list::enabled)
            return;

        auto rainbow_color = [](int offset, float saturation)
        {
            const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

            const auto state = ceil((millis.count() + offset) / 20);
            const auto rainBow = util::extra::from_hsb((fmod(state, 360) / 360), saturation, 0.8f);
            return ImColor((int)rainBow.x, (int)rainBow.y, (int)rainBow.z);
        };

        int idx = 0;

        for (auto& [name, enabled_ptr] : module_array) {
            if (!*enabled_ptr) continue;

            ImVec2 text_size = ImGui::CalcTextSize(name);
            ImVec4 mod_color = rainbow ? ImVec4(rainbow_color(idx * 200, array_list::rainbow_saturation)) : ImVec4(array_list::color[0], array_list::color[1], array_list::color[2], 1.0f);

            float padding = array_list::color_bar ? 25.f : 20.f;
            float watermark = 0;
            ImVec2 pos;

            if (Menu::Watermark) {
                watermark = 25;
            }

            if (position == 0) {
                pos = ImVec2(0.f, idx * text_size.y + watermark);
            }
            else {
                pos = ImVec2(ImGui::GetIO().DisplaySize.x - text_size.x - padding, idx * text_size.y);
            }

            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(ImColor(19, 19, 19, array_list::background ? 150 : 0)));
            ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
            ImGui::BeginChild(std::to_string(idx).c_str(), ImVec2(text_size.x + padding, text_size.y), false);
            {
                ImGui::SetCursorPosX(8.f);
                ImGui::PushStyleColor(ImGuiCol_Text, mod_color);
                ImGui::TextUnformatted(name);
                ImGui::PopStyleColor();
            }
            ImGui::EndChild();
            ImGui::PopStyleColor();

            if (array_list::color_bar) {
                ImGui::PushStyleColor(ImGuiCol_ChildBg, mod_color);
                ImVec2 bar_pos = (position == 0)
                    ? ImVec2(0.f, idx * text_size.y + watermark)
                    : ImVec2(ImGui::GetIO().DisplaySize.x - 3.f, idx * text_size.y);
                ImGui::SetNextWindowPos(bar_pos, ImGuiCond_Always);
                ImGui::BeginChild((std::to_string(idx) + "r").c_str(), ImVec2(3.f, text_size.y), false);
                ImGui::EndChild();
                ImGui::PopStyleColor();
            }

            ++idx;
        }
    }


	void array_list::handle_menu() {
		ImGui::Separator();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
		Menu::ToggleWithKeybind(94385343, "array list", &array_list::enabled, &array_list::key_bind, &array_list::show_opt);
		ImGui::Spacing();

		if (array_list::show_opt) {
            ImGui::Separator();
            ImGui::Spacing();

			Menu::Toggle(7653454, "background", &array_list::background);
			Menu::Toggle(3456321, "color bar", &array_list::color_bar);
            
            ImGui::Separator();
            ImGui::Spacing();
            Menu::Toggle(32425784, "rainbow", &array_list::rainbow);
            Menu::ColorEdit("color", array_list::color);
            Menu::Dropdown("position", array_list::position_list, &position, IM_ARRAYSIZE(array_list::position_list));
		}
	}
}