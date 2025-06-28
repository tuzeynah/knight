#include "hit_boxes.hpp"

/*
blatant asf
just use this for fun

tuzeynah @ knight
*/

namespace implement::combat {
	void hit_boxes::update() {
		if (!game_data::check())
			return;
		if (Menu::Open)
			return;
		if (SDK::Minecraft->IsInGuiState())
			return;

		const auto& playerList = game_data::native_player_list;

		if (playerList.empty()) {
			return;
		}

		CEntityPlayerSP* local_player = SDK::Minecraft->thePlayer;

		for (game_data::player_data target : playerList)
		{
			if (!Java::Env->IsSameObject(local_player->GetInstance(), target.obj.GetInstance()))
			{
				if (!local_player->CanEntityBeSeen(target.obj.GetInstance()))
					continue;
				if (target.obj.IsInvisibleToPlayer(local_player->GetInstance()))
					continue;

				BoundingBox target_box = target.bounding_box;

				if
					(
						(target_box.maxX - target_box.minX) > 0.61f &&
						(target_box.maxY - target_box.minY) > 1.81f
						)
					continue;

				target_box.minX = target_box.minX - hit_boxes::x_expand;
				target_box.maxX = target_box.maxX + hit_boxes::x_expand;
				target_box.minZ = target_box.minZ - hit_boxes::x_expand;
				target_box.maxZ = target_box.maxZ + hit_boxes::x_expand;

				target_box.minY = target_box.minY - hit_boxes::y_expand;
				target_box.maxY = target_box.maxY + hit_boxes::y_expand;

				target.obj.GetBB().SetBoundingBox(target_box);
			}
		}
	}

	void hit_boxes::handle_menu()
	{
		ImGui::Separator();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
		Menu::ToggleWithKeybind(23876321, "hit boxes", &hit_boxes::enabled, &hit_boxes::key_bind, &hit_boxes::show_opt);
		ImGui::Spacing();

		if (hit_boxes::show_opt) {
			Menu::Text("pedro tip: blatant asf, beware");

			Menu::Slider(560117, "x expand", &hit_boxes::x_expand, 0.01f, 2.0f, "%.2f");
			Menu::Slider(564325, "y expand", &hit_boxes::y_expand, 0.01f, 2.0f, "%.2f");

			ImGui::Spacing();
		}
	}
}
