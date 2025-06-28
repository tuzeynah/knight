#include "esp.hpp"

namespace implement::visuals {
	void esp::update()
	{
		if (!game_data::check())
			return;

		CEntityPlayerSP* player = SDK::Minecraft->thePlayer;
		CWorld* world = SDK::Minecraft->theWorld;

		std::vector<game_data::player_data> playerList = game_data::native_player_list;

		if (playerList.empty())
			return;

		Vector3 renderPos = game_data::render_pos;
		Vector3 pos = player->GetPos();

		// This is to fix the third person issue, there is still one issue with it.
		// When the camera collides in the wall or the ground, or something like that, the calculation fails entirely because the position is obviously not the same.
		if (game_data::third_person_view != 0) {
			Vector2 angles = player->GetAngles();
			float distance = 8;
			if (game_data::third_person_view == 2) {
				distance = -distance;
			}

			// This whole calculation came from gaspers source from their reach module, which you can find in this cheat as well
			float cos = std::cos(Math::degToRadiants(angles.x + 90.0f));
			float sin = std::sin(Math::degToRadiants(angles.x + 90.0f));
			float cosPitch = std::cos(Math::degToRadiants(angles.y));
			float sinPitch = std::sin(Math::degToRadiants(angles.y));

			float x = renderPos.x - (cos * distance * cosPitch);
			float y = renderPos.y + (distance * sinPitch);
			float z = renderPos.z - (sin * distance * cosPitch);

			// The raycast that is commented out below does not work that well for some reason, acts weirdly when colliding with chests, and other things.
			// Also might be poor in performance.
			//Vector3 ray = world->rayTraceBlocks(renderPos, Vector3{ x, y, z }, false, false, false);
			//renderPos = ray.x == 0 ? Vector3{ x,y,z } : ray;

			renderPos = Vector3{ x,y,z };
		}

		std::vector<Data> newData;

		float renderPartialTicks = game_data::render_partial_ticks;

		for (game_data::player_data entity : playerList)
		{
			Vector3 entityPos = entity.pos;
			Vector3 entityLastPos = entity.last_pos;

			float entityWidth = 0.7f;
			float entityHeight = (float)(entity.height / 2) + 0.2f;

			Vector3 diff = pos - entityPos;
			float dist = sqrt(pow(diff.x, 2.f) + pow(diff.y, 2.f) + pow(diff.z, 2.f)); // Sqrt((x2 - x1)^2 + (y2 - y1)^2 + (z2 - z1)^2)
			// Regular distance check.
			if (dist > 300) {
				continue;
			}

			// You can't put one vector3 for fixing the jittering and use that to subtract all of them with it
			// it will mess screw up for some weird reason. Try it for yourself if you wish!
			//Vector3 fixedEntityPos{ entityLastPos + (entityLastPos - entityPos) * renderPartialTicks };

			// This stops the jittering, the calculations must be inverted for some reason from the original
			Vector3 origin{ renderPos - entityLastPos + (entityLastPos - entityPos) * renderPartialTicks }; // At the feet

			// Same with the offset of the point, it must be offset from the render position, not the entity position for some weird reason.
			Vector3 top{ (renderPos - Vector3{0, entityHeight * 2, 0}) - entityLastPos + (entityLastPos - entityPos) * renderPartialTicks }; // Over the head
			Vector3 left{ (renderPos - Vector3{entityWidth, entityHeight, 0}) - entityLastPos + (entityLastPos - entityPos) * renderPartialTicks }; // In the middle to the left
			Vector3 right{ (renderPos - Vector3{-entityWidth, entityHeight, 0}) - entityLastPos + (entityLastPos - entityPos) * renderPartialTicks }; // In the middle to the right
			Vector3 back{ (renderPos - Vector3{0, entityHeight, entityWidth}) - entityLastPos + (entityLastPos - entityPos) * renderPartialTicks }; // In the middle to the back
			Vector3 front{ (renderPos - Vector3{0, entityHeight, -entityWidth}) - entityLastPos + (entityLastPos - entityPos) * renderPartialTicks }; // And in the middle to the front

			entityWidth /= 1.388888f;
			Vector3 left2{ (renderPos - Vector3{entityWidth, entityHeight, entityWidth}) - entityLastPos + (entityLastPos - entityPos) * renderPartialTicks }; // In the middle to the left
			Vector3 right2{ (renderPos - Vector3{-entityWidth, entityHeight, -entityWidth}) - entityLastPos + (entityLastPos - entityPos) * renderPartialTicks }; // In the middle to the right
			Vector3 back2{ (renderPos - Vector3{-entityWidth, entityHeight, entityWidth}) - entityLastPos + (entityLastPos - entityPos) * renderPartialTicks }; // In the middle to the back
			Vector3 front2{ (renderPos - Vector3{entityWidth, entityHeight, -entityWidth}) - entityLastPos + (entityLastPos - entityPos) * renderPartialTicks }; // And in the middle to the front

			// ^ This shapes a diamond like formation which results in 6 world to screens instead of 8.
			// However if a 3d ESP is desired, 8 world to screens would be required.

			// Another note for this data, is we cannot use the bounding box values because it can be changed by the reach module, so its best we make our own values with the cost
			// of consuming a little bit of resources for a bit of math.
			std::vector<Vector3> boxVerticies{
				origin, top, left, right, back, front, left2, right2,back2, front2
			};

			// For when the player gets close to an entity, a fade factor; a value between 0 and 1, with basic math, can get a cool looking fade effect if the player is too close
			// or inside the FadeDistance radius.
			float fadeFactor = 1.0f;
			if ((dist - 1) <= FadeDistance)
				fadeFactor = ((dist - 1) / FadeDistance);

			// To render the distance value under the ESP box.
			char distC[32];
			std::snprintf(distC, sizeof(distC), "%.1f", dist);
			std::string distS(distC);

			// Then finally taking all the data we acquired for this loop and pushing it to the data list.
			newData.push_back(Data{
				boxVerticies, // Box data
				entity.name, // Entity name
				distS + "m", // Distance
				dist, // Real distance value (for fade)
				fadeFactor, // Fade factor
				entity.health, // Entity health
				entity.max_health, // And max health (for health bar)
				});
		}
		renderData = newData;
	}

	void esp::handle_render()
	{
		if (!enabled || !game_data::updated)
			return;

		for (Data data : renderData)
		{
			ImVec2 screenSize = ImGui::GetWindowSize();

			std::vector<Vector3> bv = data.boxVerticies;

			float left = FLT_MAX;
			float top = FLT_MAX;
			float right = FLT_MIN;
			float bottom = FLT_MIN;

			bool skip = false;
			for (Vector3 position : bv)
			{
				Vector2 p;

				if (!CWorldToScreen::WorldToScreen(position, game_data::model_view, game_data::projection, (int)screenSize.x, (int)screenSize.y, p))
				{
					skip = true;
					break;
				}

				//ImGui::GetWindowDrawList()->AddCircle(ImVec2(p.x, p.y), 3, ImColor(255, 0, 0), 100, 2);

				// This is a neat trick to get the top left and bottom right corners of all the box verticies quickly.
				left = fmin(p.x, left);
				top = fmin(p.y, top);
				right = fmax(p.x, right);
				bottom = fmax(p.y, bottom);
			}

			// This is for when the world to screen fails, we just want to ignore rendering the entire box completely.
			// The fade stuff will compensate for it randomly disappearing when you're too close.
			if (skip)
				continue;

			// The rest is just rendering the ESP with the customizable options, self explanitory.

			if (FilledBox)
			{
				ImColor bottomColor = ImColor(SecondFilledBoxColor[0], SecondFilledBoxColor[1], SecondFilledBoxColor[2], FilledBoxOpacity * data.opacityFadeFactor);
				ImColor topColor = ImColor(FilledBoxColor[0], FilledBoxColor[1], FilledBoxColor[2], FilledBoxOpacity * data.opacityFadeFactor);

				ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(left, top), ImVec2(right, bottom), topColor, topColor, bottomColor, bottomColor);
			}

			if (Box)
			{
				ImGui::GetWindowDrawList()->AddRect(ImVec2(left, top), ImVec2(right, bottom), ImColor(BoxColor[0], BoxColor[1], BoxColor[2], BoxColor[3] * data.opacityFadeFactor));
			}

			if (Outline)
			{
				ImGui::GetWindowDrawList()->AddRect(ImVec2(left - 1, top - 1), ImVec2(right + 1, bottom + 1), ImColor(OutlineColor[0], OutlineColor[1], OutlineColor[2], OutlineColor[3] * data.opacityFadeFactor));
				ImGui::GetWindowDrawList()->AddRect(ImVec2(left + 1, top + 1), ImVec2(right - 1, bottom - 1), ImColor(OutlineColor[0], OutlineColor[1], OutlineColor[2], OutlineColor[3] * data.opacityFadeFactor));
			}

			if (HealthBar)
			{
				ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(left - 3, top), ImVec2(left - 1, bottom), ImColor(FilledBoxColor[0], FilledBoxColor[1], FilledBoxColor[2], FilledBoxOpacity * data.opacityFadeFactor));

				if (data.health <= 0)
					data.health = 0.00001f;

				float scaleFactor = data.health / data.maxHealth;
				float diff = bottom - top;

				ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(left - 3, bottom - (diff * scaleFactor)), ImVec2(left - 1, bottom), ImColor((int)(255 * (1.0 - scaleFactor)), (int)(255 * scaleFactor), 0, (int)(255 * data.opacityFadeFactor)));
			}

			if (Text && Menu::Font->IsLoaded())
			{
				const char* name = data.name.c_str();
				ImVec2 textSize = Menu::Font->CalcTextSizeA(TextSize, FLT_MAX, 0.0f, name);
				float posX = left + ((right - left) / 2) - (textSize.x / 2);
				float posY = top - textSize.y - 1;

				if (data.dist > TextUnrenderDistance) {

					ImGui::GetWindowDrawList()->AddText(Menu::Font, TextSize, ImVec2(posX, posY), ImColor(TextColor[0], TextColor[1], TextColor[2], TextColor[3] * data.opacityFadeFactor), name);
				}

				const char* dist = data.distText.c_str();
				float distTextSize = TextSize / 1.5;
				textSize = Menu::Font->CalcTextSizeA(distTextSize, FLT_MAX, 0.0f, dist);
				posX = left + ((right - left) / 2) - (textSize.x / 2);
				posY = bottom;

				ImGui::GetWindowDrawList()->AddText(Menu::Font, distTextSize, ImVec2(posX, posY), ImColor(TextColor[0], TextColor[1], TextColor[2], TextColor[3] * data.opacityFadeFactor), dist);
				
			}
		}
	}

	void esp::handle_menu()
	{
		ImGui::Separator();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
		Menu::ToggleWithKeybind(28374, "esp", &esp::enabled, &esp::key_bind, &esp::show_opt);
		ImGui::Spacing();

		if (esp::show_opt) {
			ImGui::Separator();
			Menu::Slider(34875, "fade distance", &FadeDistance, 0.0f, 10.0f);
			Menu::Toggle(23445, "health bar", &HealthBar);
			ImGui::Separator();
			ImGui::Spacing();

			Menu::Toggle(34576, "text", &Text);
			Menu::Slider(128763, "text size", &TextSize, 12, 24);
			Menu::ColorEdit("text color", TextColor);

			ImGui::Separator();
			ImGui::Spacing(); 
			Menu::Toggle(654354, "box", &Box);
			Menu::ColorEdit("box color", BoxColor);
			Menu::Toggle(654899, "filled box", &FilledBox);
			Menu::ColorEdit("filled color", FilledBoxColor);
			Menu::Toggle(675453, "outline box", &Outline);
			Menu::ColorEdit("outline color", OutlineColor);

		}
	}
}