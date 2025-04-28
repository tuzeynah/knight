#include "no_hit_delay.hpp"

namespace implement::utility {
	void no_hit_delay::update() {
		if (!game_data::check())
			return;
		if (Menu::Open)
			return;
		if (SDK::Minecraft->IsInGuiState())
			return;

		SDK::Minecraft->setLeftClickCounter(0);
	}

	void no_hit_delay::handle_menu() {
		ImGui::Separator();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
		Menu::ToggleWithKeybind(233256782, "no hit delay", &no_hit_delay::enabled, &no_hit_delay::key_bind);
		ImGui::Spacing();
	}
}


