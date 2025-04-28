#include "aim_assist.hpp"
/*
blatant asf
use wirh safety

@tuzeynah
*/
namespace implement::combat {
	void aim_assist::update() {
		if (Menu::Open)
			return;
		if (!game_data::check())
			return;
		if (SDK::Minecraft->IsInGuiState())
			return;

		if (aim_assist::only_sprinting && !SDK::Minecraft->thePlayer->IsSprinting()) {
			return;
		}

		if (aim_assist::only_clicking && !(GetAsyncKeyState(VK_LBUTTON) & 0x8000)) {
			return;
		}

		const auto& player_list = game_data::native_player_list;

		if (player_list.empty())
			return;

		CEntityPlayerSP* thePlayer = SDK::Minecraft->thePlayer;

		Vector3 pos = thePlayer->GetPos();
		Vector3 headPos = thePlayer->GetEyePos();
		Vector2 currentLookAngles = thePlayer->GetAngles();

		game_data::player_data target;

		float finalDist = FLT_MAX;
		float finalDiff = 370;
		float finalHealth = FLT_MAX;

		float realAimDistance = aim_assist::distance;

		for (game_data::player_data player : player_list)
		{
			if (Java::Env->IsSameObject(thePlayer->GetInstance(), player.obj.GetInstance()))
				continue;
			if (!thePlayer->CanEntityBeSeen(player.obj.GetInstance()) && visibility_check)
				continue;
			if (player.obj.IsInvisibleToPlayer(thePlayer->GetInstance()) && invisible_check)
				continue;

			Vector3 playerPos = player.pos;
			float playerHeight = target.height - 0.1f;
			Vector3 playerHeadPos = playerPos + Vector3(0, playerHeight, 0);

			Vector2 anglesFoot = Math::getAngles(headPos, playerPos);
			Vector2 anglesHead = Math::getAngles(headPos, playerHeadPos);

			Vector2 difference = Math::vec_wrapAngleTo180(currentLookAngles.Invert() - anglesHead.Invert());
			if (difference.x < 0) difference.x = -difference.x;
			if (difference.y < 0) difference.y = -difference.y;
			Vector2 differenceFoot = Math::vec_wrapAngleTo180(currentLookAngles.Invert() - anglesFoot.Invert());
			if (differenceFoot.x < 0) differenceFoot.x = -differenceFoot.x;
			if (differenceFoot.y < 0) differenceFoot.y = -differenceFoot.y;

			float angleYaw = currentLookAngles.x - difference.x;

			Vector3 diff = pos - playerPos;
			float dist = sqrt(pow(diff.x, 2.f) + pow(diff.y, 2.f) + pow(diff.z, 2.f));

			if ((abs(difference.x) <= aim_assist::fov) && dist <= realAimDistance)
			{
				float health = player.health;
				switch (aim_assist::target_priority)
				{
				case 1:
					if (finalHealth > health)
					{
						target = player;
						finalHealth = health;
					}
					break;

				case 2:
					if (finalDiff > difference.x)
					{
						target = player;
						finalDiff = difference.x;
					}
					break;
				default:
					if (finalDist > dist)
					{
						target = player;
						finalDist = (float)dist;
					}
				}
			}
		}

		if (!target.obj.GetInstance()) {
			return;
		}


		Vector3 ePos = target.pos;
		Vector3 eLastPos = target.last_pos;

		float eHeight = target.height - 0.1f;
		Vector3 eHeadPos = ePos + Vector3(0, eHeight, 0);
		Vector3 eLastHeadPos = eLastPos + Vector3(0, eHeight, 0);

		Vector2 anglesFoot = Math::getAngles(headPos, ePos);
		Vector2 anglesHead = Math::getAngles(headPos, eHeadPos);

		Vector2 difference = Math::vec_wrapAngleTo180(currentLookAngles.Invert() - anglesHead.Invert());
		Vector2 differenceFoot = Math::vec_wrapAngleTo180(currentLookAngles.Invert() - anglesFoot.Invert());

		float offset = util::number::get_random_float(-0.075f, 0.075f);

		if (adaptive) {
			if ((GetAsyncKeyState('D') & 0x8000) && !(GetAsyncKeyState('A') & 0x8000)) {
				offset -= adaptive_offset;
			}

			if ((GetAsyncKeyState('A') & 0x8000) && !(GetAsyncKeyState('D') & 0x8000)) {
				offset += adaptive_offset;
			}
		}

		float targetYaw = currentLookAngles.x + ((difference.x + offset) / smooth);

		Vector3 renderPos = game_data::render_pos;
		float renderPartialTicks = game_data::render_partial_ticks;

		if (currentLookAngles.y > anglesFoot.y || currentLookAngles.y < anglesHead.y) {
			float targetPitchFoot = currentLookAngles.y + (differenceFoot.y / smooth);
			float targetPitchHead = currentLookAngles.y + (difference.y / smooth);

			float diffFoot = currentLookAngles.y - targetPitchFoot;
			float diffHead = currentLookAngles.y - targetPitchHead;
			diffFoot = diffFoot < 0 ? -diffFoot : diffFoot;
			diffHead = diffHead < 0 ? -diffHead : diffHead;

			float targetPitch;
			if (diffFoot > diffHead)
			{
				targetPitch = targetPitchHead;
			}
			else
			{
				targetPitch = targetPitchFoot;
			}
			targetPitch += util::number::get_random_float(-0.075f, 0.075f);
			thePlayer->SetAngles(Vector2(targetYaw, targetPitch));
		}
		else {
			thePlayer->SetAngles(Vector2(targetYaw, currentLookAngles.y + util::number::get_random_float(-0.075f, 0.075f)));
		}
	}

	void aim_assist::handle_menu() {
		ImGui::Separator();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);

		Menu::ToggleWithKeybind(234598, "aim assist", &aim_assist::enabled, &aim_assist::key_bind, &aim_assist::show_opt);
		ImGui::Spacing();

		if (aim_assist::show_opt) {
			ImGui::Separator();

			Menu::Toggle(453567, "visiblity check", &aim_assist::visibility_check);
			Menu::Toggle(456765, "ignore invisible", &aim_assist::invisible_check);
			Menu::Toggle(345634, "only sprinting", &aim_assist::only_sprinting);
			Menu::Toggle(456234, "only clicking", &aim_assist::only_clicking);


			Menu::Toggle(6543590, "adaptive", &aim_assist::adaptive);
			if (aim_assist::adaptive) {
				Menu::Slider(6542356, "offset", &aim_assist::adaptive_offset, 0.0f, 15.f, "%.2f");
			}

			Menu::Slider(5643475, "smooth", &aim_assist::smooth, 1.0f, 90.0f, "%.2f");
			Menu::Slider(5647665, "fov", &aim_assist::fov, 5.0f, 180.f);
			Menu::Slider(5646675, "distance", &aim_assist::distance, 1.0f, 10.0f, "%.2f");

			Menu::Dropdown("target priority", aim_assist::target_priority_list, &aim_assist::target_priority, IM_ARRAYSIZE(aim_assist::target_priority_list));

			ImGui::Spacing();
		}
	}
}


