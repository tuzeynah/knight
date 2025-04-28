#include "reach.hpp"

namespace implement::combat {
	void reach::update() {
		if (Menu::Open)
			return;
		if (!game_data::check())
			return;
		if (SDK::Minecraft->IsInGuiState())
			return;

		const auto& playerList = game_data::native_player_list;

		if (playerList.empty()) {
			return;
		}

		CEntityPlayerSP* local_player = SDK::Minecraft->thePlayer;
		Vector3 pos = local_player->GetPos();

		game_data::player_data target;
		float finalDist = (std::numeric_limits<float>::max)();

		for (game_data::player_data player : playerList)
		{
			if (!Java::Env->IsSameObject(local_player->GetInstance(), player.obj.GetInstance()))
			{
				if (!local_player->CanEntityBeSeen(player.obj.GetInstance()))
					continue;
				if (player.obj.IsInvisibleToPlayer(local_player->GetInstance()))
					continue;

				Vector3 targetPos = player.pos;
				Vector3 diff = pos - targetPos;
				float dist = sqrt(pow(diff.x, 2.f) + pow(diff.y, 2.f) + pow(diff.z, 2.f));

				if (dist <= finalDist && dist <= 6.0)
				{
					finalDist = (float)dist;
					target = player;
				}
			}
		}

		if (!target.obj.GetInstance()) {
			return;
		}

		const Vector3 playerPos = target.pos;
		const auto distBetween = pos.Distance(playerPos);

		if ((distBetween > reach::distance + 0.5 || distBetween <= 3.000))
			return;

		auto x = (float)playerPos.x;
		auto z = (float)playerPos.z;

		const float hypothenuse_distance = hypotf((float)(pos.x - playerPos.x), (float)(pos.z - playerPos.z));
		float dist = reach::distance - 3.f;

		while (distBetween > 3.0 && (distBetween < (dist + 3.0)) && dist > 0.05)
			dist -= 0.05f;

		if (hypothenuse_distance < dist)
			dist -= hypothenuse_distance;

		auto GetAngle = [](float ex, float ez, Vector3 mypos)
		{
			const auto x = ex - (float)mypos.x;
			const auto z = ez - (float)mypos.z;

			auto y = (float)(-atanf(x / z) * 180.f / (float)PI);
			if (z < 0.f && x < 0.f) {
				y = 90.f + (float)(atanf(z / x) * 180.f / (float)PI);
			}
			else if (z < 0.f && x > 0.f) {
				y = -90.f + (float)(atanf(z / x) * 180.f / (float)PI);
			}
			return y;
		};

		const float r = GetAngle(x, z, pos);
		const float ax = cosf((float)((r + 90.f) * PI / 180.f));
		const float b = sinf((float)((r + 90.f) * PI / 180.f));

		x = (float)playerPos.x;
		z = (float)playerPos.z;

		x -= ax * dist;
		z -= b * dist;

		const float newWidth = (0.6f) /*entity width*/ / 2.0f;
		const BoundingBox curHitbox = target.bounding_box;

		BoundingBox bb{};
		bb.minX = x - newWidth;
		bb.minY = curHitbox.minY;
		bb.minZ = z - newWidth;

		bb.maxX = x + newWidth;
		bb.maxY = curHitbox.maxY;
		bb.maxZ = z + newWidth;

		// hitbox misplacing
		target.obj.GetBB().SetBoundingBox(bb);
	}

	void reach::handle_menu() {
		ImGui::Separator();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);

		Menu::ToggleWithKeybind(23459548, "reach", &reach::enabled, &reach::key_bind, &reach::show_opt);
		ImGui::Spacing();

		if (reach::show_opt) {
			ImGui::Separator();
			Menu::Slider(54277665, "distance", &reach::distance, 3.0f, 6.0f);

			ImGui::Spacing();
		}
	}
}


