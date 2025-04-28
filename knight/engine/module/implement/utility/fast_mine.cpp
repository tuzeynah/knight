#include "fast_mine.hpp"

/*
really good on prison/rankup server
otherwise i wouldn't use it on a prac server

tuzeynah @ knight
*/

namespace implement::utility {
	void fast_mine::update() {
		if (!game_data::check())
			return;
		if (Menu::Open)
			return;
		if (SDK::Minecraft->IsInGuiState())
			return;
		if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
			return;
		
		CPlayerControllerMP* controller = SDK::Minecraft->playerController;
		if (!controller)
			return;

		if (no_break_delay) {
			controller->setBlockHitDelay(0);
		}

		if (controller->isHittingBlock()) {
			controller->setBlockDamage(speed_multiplier);
		}
	}


	void fast_mine::handle_menu() {
		ImGui::Separator();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);

		Menu::ToggleWithKeybind(230353637, "fast mine", &fast_mine::enabled, &fast_mine::key_bind, &fast_mine::show_opt);

		ImGui::Spacing();
		if (fast_mine::show_opt) {
			Menu::Text("pedro tip: useful in prison/rankup servers");

			Menu::Slider(5645375, "mining speed", &fast_mine::speed_multiplier, 0, 5);
			Menu::Toggle(1343462, "no break delay", &fast_mine::no_break_delay);
		}
	}
}
