#include "no_jump_delay.hpp"

/*
this produces the same result as spamming the jump key

tuzeynah @ knight
*/

namespace implement::utility {
	void no_jump_delay::update() {
		if (Menu::Open)
			return;
		if (!game_data::check())
			return;
		if (SDK::Minecraft->IsInGuiState())
			return;

		CEntityPlayerSP* local_player = SDK::Minecraft->thePlayer;

		if (GetAsyncKeyState(VK_SPACE) & 0x8000 && !local_player->isOnGround()) {
			local_player->forceGround();
			local_player->jump();
		}
	}

	void no_jump_delay::handle_menu() {
		ImGui::Separator();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
		Menu::ToggleWithKeybind(23352411, "no jump delay", &no_jump_delay::enabled, &no_jump_delay::key_bind);
		ImGui::Spacing();
	}
}



