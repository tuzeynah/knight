#include "velocity.hpp"

/*
normal velocity 
the only thing that we need to do here is
add more conditionals

tuzeynah @ knight
*/
namespace implement::combat {
	void velocity::update() {
		if (!game_data::check())
			return;
		if (Menu::Open)
			return;
		if (SDK::Minecraft->IsInGuiState())
			return;

		CEntityPlayerSP* local_player = SDK::Minecraft->thePlayer;

		if (local_player->getMaxHurtTime() > 0 && local_player->getHurtTime() == local_player->getMaxHurtTime()) {
			if (horizontal != 100) {
				local_player->setMotionX(local_player->getMotionX() * velocity::horizontal / 100);
				local_player->setMotionY(local_player->getMotionY() * velocity::horizontal / 100);
			}

			if (vertical != 100) {
				local_player->setMotionZ(local_player->getMotionZ() * velocity::vertical / 100);
			}
		}
	}

	void velocity::handle_menu() {
		ImGui::Separator();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
		Menu::ToggleWithKeybind(2564367, "velocity", &velocity::enabled, &velocity::key_bind, &velocity::show_opt);
		ImGui::Spacing();

		if (velocity::show_opt) {
			ImGui::Separator();
			Menu::Slider(5647675, "horizontal", &velocity::horizontal, 0, 100, "%.2f");
			Menu::Slider(5464564, "vertical", &velocity::vertical, 0, 100, "%.2f");

			ImGui::Spacing();
		}
	}
}

