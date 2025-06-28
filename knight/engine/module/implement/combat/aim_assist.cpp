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

		CEntityPlayerSP* local_player = SDK::Minecraft->thePlayer;

		double closest_yaw = 500.0;

		const auto local_pos = local_player->GetPos();

		float local_yaw = local_player->GetRotationYaw();
		float local_previous_yaw = local_player->GetPrevRotationYaw();

		float my_pitch = local_player->GetRotationPitch();

		game_data::player_data target;

		for (game_data::player_data players : player_list)
		{
			if (!Java::Env->IsSameObject(local_player->GetInstance(), players.obj.GetInstance()))
			{
				if (!local_player->CanEntityBeSeen(players.obj.GetInstance()))
					continue;
				if (players.obj.IsInvisibleToPlayer(local_player->GetInstance()))
					continue;

				Vector3 target_pos = players.pos;

				double distance = sqrt(pow(target_pos.x - local_pos.x, 2) + pow(target_pos.y - local_pos.y, 2) + pow(target_pos.z - local_pos.z, 2));

				if (distance >= aim_assist::distance)
					continue;

				double distance_x = target_pos.x - local_pos.x;
				double distance_z = target_pos.z - local_pos.z;

				float dif = (float)atan2((float)distance_z, (float)distance_x) * 180.00f / PI - 90.00f;

				dif = fmodf(dif - local_yaw, 360.f);
				if (dif >= 180.0f) 
					dif -= 360.0f;
				if (dif < -180.0f)
					dif += 360.0f;

				if (abs(dif) <= aim_assist::max_angle && abs(dif) < abs(closest_yaw))
					target = players;
					closest_yaw = dif;
			}
		}

		if (closest_yaw != 500.0) {
			Vector3 cp_pos = target.pos, crnr_1, crnr_2, crnr_3, crnr_4;

			crnr_1 = { cp_pos.x - 0.30f, cp_pos.y, cp_pos.z + 0.30f };
			crnr_2 = { cp_pos.x - 0.30f, cp_pos.y, cp_pos.z - 0.30f };
			crnr_3 = { cp_pos.x + 0.30f, cp_pos.y, cp_pos.z - 0.30f };
			crnr_4 = { cp_pos.x + 0.30f, cp_pos.y, cp_pos.z + 0.30f };

			double distance = sqrt(pow(cp_pos.x - local_pos.x, 2) + pow(cp_pos.y - local_pos.y, 2) + pow(cp_pos.z - local_pos.z, 2));

			double distance_c_1 = sqrt(pow(crnr_1.x - local_pos.x, 2) + pow(crnr_1.y - local_pos.y, 2) + pow(crnr_1.z - local_pos.z, 2));
			double distance_c_2 = sqrt(pow(crnr_2.x - local_pos.x, 2) + pow(crnr_2.y - local_pos.y, 2) + pow(crnr_2.z - local_pos.z, 2));
			double distance_c_3 = sqrt(pow(crnr_3.x - local_pos.x, 2) + pow(crnr_3.y - local_pos.y, 2) + pow(crnr_3.z - local_pos.z, 2));
			double distance_c_4 = sqrt(pow(crnr_4.x - local_pos.x, 2) + pow(crnr_4.y - local_pos.y, 2) + pow(crnr_4.z - local_pos.z, 2));

			std::vector<double> closest_crn;
			closest_crn.push_back(distance_c_1);
			closest_crn.push_back(distance_c_2);
			closest_crn.push_back(distance_c_3);
			closest_crn.push_back(distance_c_4), util::extra::sort_vec(closest_crn);

			Vector3 closest_crnr_1, closest_crnr_2;

			for (size_t x = 0; x < 2; ++x) {

				if (closest_crn[x] == distance_c_1) if (x == 0) closest_crnr_1 = crnr_1; else if (x == 1) closest_crnr_2 = crnr_1;
				if (closest_crn[x] == distance_c_2) if (x == 0) closest_crnr_1 = crnr_2; else if (x == 1) closest_crnr_2 = crnr_2;
				if (closest_crn[x] == distance_c_3) if (x == 0) closest_crnr_1 = crnr_3; else if (x == 1) closest_crnr_2 = crnr_3;
				if (closest_crn[x] == distance_c_4) if (x == 0) closest_crnr_1 = crnr_4; else if (x == 1) closest_crnr_2 = crnr_4;

			}

			Vector3 closest_point = closest_crnr_1;

			if (closest_crnr_1.x == closest_crnr_2.x) {
				closest_point.z = local_pos.z;

				if (!util::extra::in_range(closest_point.z, closest_crnr_1.z, closest_crnr_2.z)) closest_point = closest_crnr_1;

			}
			else if (closest_crnr_2.z == closest_crnr_2.z) {
				closest_point.x = local_pos.x;

				if (!util::extra::in_range(closest_point.x, closest_crnr_1.x, closest_crnr_2.x)) closest_point = closest_crnr_1;

			}

			double distance_x = closest_point.x - local_pos.x, distance_z = closest_point.z - local_pos.z;

			float dif = (float)atan2((float)distance_z, (float)distance_x) * 180.f / PI - 90.f;
			dif = Math::wrapAngleTo180(fmodf(dif - local_yaw, 360.f));

			if (static_cast<int>(abs(dif)) <= aim_assist::max_angle && abs(dif) > 2.f)
			{
				float tmp = 0.5f * 0.600f + 0.200f, actual_mc_sense = pow(tmp, 3) * 8.00f;

				if (abs(dif) < 2.0f)
					dif = util::number::get_random_float(1.f, 2.f) * (abs(dif) / dif);

				dif *= actual_mc_sense * (aim_assist::speed + (int)util::number::get_random_float(1.0f, 4.0f));

				float new_yaw_a = local_yaw + (int)dif * 0.0015f;
				float prv_yaw_a = local_previous_yaw + (int)dif * 0.0015f;

				local_player->SetRotationYaw(new_yaw_a);
				local_player->SetPrevRotationYaw(prv_yaw_a);
			}

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

			Menu::Slider(5643475, "speed", &aim_assist::speed, 1.0f, 90.0f, "%.2f");
			Menu::Slider(5647665, "fov", &aim_assist::max_angle, 5.0f, 180.f);
			Menu::Slider(5646675, "distance", &aim_assist::distance, 1.0f, 10.0f, "%.2f");

			ImGui::Spacing();
		}
	}
}


