#pragma once
#include "../../../util/math/geometry.h"

#include "../../../game_data.h"
#include "../../../../sdk/sdk.h"
#include "../../../util/logger.hpp"
#include "../../../../ext/imgui/imgui.h"
#include "../../../util/math/math.h"
#include "../../../util/math/worldToScreen.h"
#include "../../../menu/menu.h"
#include <gl/GL.h>

#include <vector>
#include <string>

namespace implement {
	namespace visuals {
		struct esp {
			static void update();
			static void handle_render();
			static void handle_menu();

			static inline bool show_opt = false;

			static inline bool enabled = false;

			static inline int key_bind = 0x0;

			struct Data {
				std::vector<Vector3> boxVerticies;
				std::string name;
				std::string distText;
				float dist;
				float opacityFadeFactor;
				float health;
				float maxHealth;
			};

			inline static std::vector<Data> renderData;

			inline static bool Box = true;
			inline static float BoxColor[4]{ 0, 0, 0, 1 };

			inline static bool FilledBox = true;
			inline static float FilledBoxColor[3]{ 0, 0, 0 };
			inline static float SecondFilledBoxColor[3]{ 0, 0, 0 };
			inline static float FilledBoxOpacity = 0.15f;

			inline static bool Outline = true;
			inline static float OutlineColor[4]{ 0, 0, 0, 0.25 };

			inline static bool Text = true;
			inline static float TextSize = 18;
			inline static float TextColor[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
			inline static bool TextOutline = true;

			inline static float TextOutlineColor[4]{ 0, 0, 0, 1.0f };
			inline static float TextUnrenderDistance = 14.0f;

			inline static float FadeDistance = 3.0f;

			inline static bool HealthBar = true;
		};
	}
}