#include "fast_place.hpp"

namespace implement::utility {
	void fast_place::update() {
		if (!game_data::check())
			return;
		if (Menu::Open)
			return;
		if (SDK::Minecraft->IsInGuiState())
			return;

		if (SDK::Minecraft->getRightClickDelayTimer() == 4) {
			SDK::Minecraft->setRightClickDelayTimer(0);
		}
	}

	void fast_place::handle_menu() {
		ImGui::Separator();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
		Menu::ToggleWithKeybind(233626865, "fast place", &fast_place::enabled, &fast_place::key_bind);
		ImGui::Spacing();
	}
}
