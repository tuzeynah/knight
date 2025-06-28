#include "auto_clicker.hpp"

namespace implement::combat {
	void auto_clicker::update() {
		std::call_once(thread_once, [] {
			std::thread(random_behavior).detach();
			});

		if (Menu::Open)
			return;
		if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
			return;

		float f_cps = auto_clicker::average_cps;
		f_cps += random;

		delay = (1000.0f / f_cps) / 2.0f;
		delay += util::number::noise_float(-5.0f, 5.0f);
		 
		POINT cursor;
		GetCursorPos(&cursor);

		precise_sleep(delay);
		PostMessage(Menu::HandleWindow, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(cursor.x, cursor.y));
		precise_sleep(delay);
		PostMessage(Menu::HandleWindow, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(cursor.x, cursor.y));
	}

	void auto_clicker::random_behavior() noexcept {
		while (true) {
			auto rate = util::number::noise_float(0, 2000);

			if (should_update) {
				if (auto_clicker::persistence) {
					random = util::number::noise_float(-auto_clicker::persistence_value, auto_clicker::persistence_value);
				}

				if (auto_clicker::drops && auto_clicker::drop_chance > 0 && (std::rand() % (100 / auto_clicker::drop_chance) == 0)) {
					random -= auto_clicker::drop_remove;
				}

				if (spikes && auto_clicker::spike_chance > 0 && (std::rand() % (100 / auto_clicker::spike_chance) == 0)) {
					random += auto_clicker::spike_add;
				}

				should_update = false;
			}

			precise_sleep(rate);

			random = 0.f;
			should_update = true;
		}
	}

	void auto_clicker::handle_menu()
	{
		ImGui::Separator();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);

		Menu::ToggleWithKeybind(349853672, "auto clicker", &auto_clicker::enabled, &auto_clicker::key_bind, &auto_clicker::show_opt);
		ImGui::Spacing();

		if (auto_clicker::show_opt) {
			ImGui::Separator();

			Menu::Slider(7343456, "average cps", &auto_clicker::average_cps, 1.f, 20.f, "%.2f cps");

			ImGui::Separator();


			Menu::Toggle(7523245, "persistance", &auto_clicker::persistence);
			if (auto_clicker::persistence) {
				Menu::Slider(34567532, "value", &auto_clicker::persistence_value, 1.f, 5.f, "%.1f cps");
				ImGui::Spacing();
			}

			ImGui::Separator();
			Menu::Toggle(75683453, "spike chance", &auto_clicker::spikes);

			if (auto_clicker::spikes) {
				Menu::Slider(7455325, "chance", &auto_clicker::spike_chance, 1, 100, "chance %d%%");
				Menu::Slider(8654564, "add", &auto_clicker::spike_add, 1.f, 5.f, "add %.1f cps");
				ImGui::Spacing();
			}

			ImGui::Separator();

			Menu::Toggle(75423423, "drop chance", &auto_clicker::drops);

			if (auto_clicker::drops) {
				Menu::Slider(2342578, "remove", &auto_clicker::drop_remove, 1.f, 5.f, "remove %.1f cps");
				Menu::Slider(2433251, "chance", &auto_clicker::drop_chance, 1, 100, "chance %d%%");
				ImGui::Spacing();
			}

			ImGui::Spacing();
		}
	}
}
