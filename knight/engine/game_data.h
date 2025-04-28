#pragma once

#include "util/math/geometry.h"
#include "../sdk/sdk.h"
#include "util/logger.hpp"
#include <vector>
#include <string>
#include <utility>

struct game_data {
	inline static bool updated = false;

	inline static Matrix model_view;
	inline static Matrix projection;
	inline static Vector3 render_pos;
	inline static float render_partial_ticks;
	inline static int third_person_view;

	struct player_data {
		CEntityPlayer obj = nullptr;
		Vector3 pos = Vector3();
		Vector3 last_pos = Vector3();
		BoundingBox bounding_box = BoundingBox();
		std::string name = "";
		float height = FLT_MAX;
		float health = FLT_MAX;
		float max_health = FLT_MAX;
	};

	inline static std::vector<player_data> native_player_list;

	static bool check() {
		if (!SDK::Minecraft->thePlayer->GetInstance() || !SDK::Minecraft->theWorld->GetInstance())
		{
			game_data::updated = false;
			return false;
		}
		return true;
	}

	static void update() {
		if (!check()) {
			return;
		}


		model_view = SDK::Minecraft->activeRenderInfo->ModelViewMatrix();
		projection = SDK::Minecraft->activeRenderInfo->ProjectionMatrix();
		third_person_view = SDK::Minecraft->gameSettings->GetThirdPersonView();
		 
		float y_subtract = 0.f;
		if (Java::Version == minecraft_version::MAJOR_1_8_9)
		{
			y_subtract = 3.4f;
			if (SDK::Minecraft->thePlayer->IsSneaking())
				y_subtract -= .175f;
		}
		else
		{
			y_subtract = .25f;
		}

		render_pos = SDK::Minecraft->renderManager->RenderPos() + Vector3{ 0, y_subtract, 0 };
		render_partial_ticks = SDK::Minecraft->timer->GetRenderPartialTicks();

		std::vector<player_data> new_data;

		std::vector<CEntityPlayer> playerList = SDK::Minecraft->theWorld->GetPlayerList();
		for (CEntityPlayer player : playerList) {
			new_data.push_back(player_data{
					player,
					player.GetPos(),
					player.GetLastTickPos(),
					player.GetBB().GetNativeBoundingBox(),
					player.GetName(),
					player.GetHeight(),
					player.GetHealth(),
					player.GetMaxHealth()
				});
		}

		native_player_list = std::move(new_data);
		game_data::updated = true;
	}
};