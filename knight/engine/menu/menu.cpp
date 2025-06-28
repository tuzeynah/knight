#include "menu.h"

// TODO: rewrite everything here
void Menu::Init()
{
	Menu::Initialized = false;
	Menu::Open = false;
	Menu::Keybind = VK_INSERT;

	Menu::PlaceHooks();
}

static int menu_tab = 0;

void Menu::RenderMenu()
{
	ImGui::SetNextWindowSize(ImVec2(700, 420));
	ImGui::Begin("##menu", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::PushFont(Menu::Font);

	ImGui::Text("                                                              knight");
	ImGui::PopFont();
	ImGui::SameLine();

	ImGui::PushFont(Menu::IconFont);
	ImGui::Text("K");

	ImGui::Spacing();

	if (ImGui::Button("J", ImVec2(133, 40))) {
		menu_tab = 0;
	}

	ImGui::SameLine();

	if (ImGui::Button("A", ImVec2(133, 40))) {
		menu_tab = 1;
	}
	ImGui::SameLine();

	if (ImGui::Button("C", ImVec2(133, 40))) {
		menu_tab = 2;
	}
	ImGui::SameLine();

	ImGui::Button("G", ImVec2(133, 40));
	ImGui::SameLine();

	if (ImGui::Button("B", ImVec2(133, 40))) {
		menu_tab = 4;
	}

	ImGui::PopFont();

	ImGui::PushFont(Menu::Font);
	ImGui::Separator();

	ImVec4* colors = ImGui::GetStyle().Colors;

	ImGui::BeginChild("##main_child", ImVec2(700, 325), true, ImGuiWindowFlags_NoBackground);
	{
		switch (menu_tab) {
		case 0:
			implement::combat::auto_clicker::handle_menu();
			implement::combat::aim_assist::handle_menu();
			implement::combat::hit_boxes::handle_menu();
			implement::combat::velocity::handle_menu();
			implement::combat::reach::handle_menu();
			break;
		case 1:
			implement::visuals::array_list::handle_menu();
			implement::visuals::esp::handle_menu();
			break;
		case 2:
			implement::utility::no_hit_delay::handle_menu();
			implement::utility::no_jump_delay::handle_menu();
			implement::utility::fast_mine::handle_menu();
			implement::utility::fast_place::handle_menu();
			break;
		case 4:
			Menu::Toggle(94358345, "watermark", &Menu::Watermark);
			if (Menu::ColorEdit("accent", Menu::Accent)) {

				colors[ImGuiCol_SliderGrab] = ImVec4(Menu::Accent[0], Menu::Accent[1], Menu::Accent[2], Menu::Accent[3]);
				colors[ImGuiCol_SliderGrabActive] = Menu::Darken(colors[ImGuiCol_SliderGrab], 0.4f);
			}

			ImGui::Separator();


			constexpr auto& config_items = config_manager.get_configs();
			static auto current_config = -1;

			if (static_cast<size_t>(current_config) >= config_items.size())
				current_config = -1;

			static char buffer[32];

			Menu::ShowOption("configs", &config_opt);

			if (config_opt) {

				ImGui::SetCursorPos(ImVec2(20, ImGui::GetCursorPosY()));
				if (ImGui::ListBox("##config_list", &current_config, [](void* data, int idx, const char** out_text)
					{
						auto& vector = *static_cast<std::vector<std::string> *>(data);
				*out_text = vector[idx].c_str();
				return true;
					}, &config_items, static_cast<int>(config_items.size()), 5) && current_config != -1) 
					
					
					strcpy_s(buffer, config_items[current_config].c_str());
					ImGui::SetCursorPos(ImVec2(20, ImGui::GetCursorPosY()));

					if (ImGui::InputText("##config_name", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
					{
						if (current_config != -1)
							config_manager.rename(current_config, buffer);
					}

					ImGui::SetCursorPos(ImVec2(20, ImGui::GetCursorPosY()));
					if (ImGui::Button("open folder", ImVec2(120, 25)))
						config_manager.open_folder();
					ImGui::SameLine();

					if (ImGui::Button("create", ImVec2(60, 25)))
						config_manager.add(buffer);

					ImGui::SameLine();

					if (current_config > -1)
					{
						if (ImGui::Button("load", ImVec2(60, 25)))
							config_manager.load(current_config);

						ImGui::SameLine();

						if (ImGui::Button("save", ImVec2(60, 25)))
							config_manager.save(current_config);

						ImGui::SameLine();

						if (ImGui::Button("delete", ImVec2(60, 25)))
							config_manager.remove(current_config);
					}
			}


			break;
		}
	}
	ImGui::EndChild();

	ImGui::PopFont();
	ImGui::End();
}

void Menu::Kill()
{
	Menu::Open = false;
	Menu::RemoveHooks();
	wglMakeCurrent(Menu::HandleDeviceContext, Menu::OriginalGLContext);
	wglDeleteContext(Menu::MenuGLContext);
	ImGui::DestroyContext();
}

void Menu::PlaceHooks()
{
	Menu::Hook_wglSwapBuffers();
}

void Menu::RemoveHooks()
{
	Menu::Unhook_wndProc();
	Menu::Unhook_wglSwapBuffers();
}

void Menu::Text(const char* text) {
	ImGui::SetCursorPos(ImVec2(20.0f, ImGui::GetCursorPosY() + 5.0f));

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5.0f);
	ImGui::Text(text);
	ImGui::SameLine();
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5.0f);

	ImGui::Spacing();
}

bool Menu::Dropdown(const char* label, const char* items[], int* item_current, int items_count, ImVec2 size) {

	ImGui::SetCursorPos(ImVec2(20.0f, ImGui::GetCursorPosY() + 5.0f));
	ImGui::Text(label);
	ImGui::SameLine();

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 100.0f);
	return ImGui::Combo(("##" + std::string(label)).c_str(), item_current, items, items_count);
}

void Menu::Slider(int id, const char* text, float* value, float min, float max, const char* format) {
	ImVec2 textSize = Menu::Font->CalcTextSizeA(Menu::Font->FontSize, FLT_MAX, 0.0f, text);

	ImGui::PushID(id);
	ImGui::SetCursorPos(ImVec2(20.0f, ImGui::GetCursorPosY() + 5.0f));
	ImGui::Text(text);
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 230.0f - textSize.x - 20.0f);
	ImGui::SliderFloat("", value, min, max, format);
	ImGui::SameLine();
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
	ImGui::PopID();

	ImGui::PushID(id - 9);
	ImGui::InputFloat("", value, 0, 0, format);
	ImGui::PopStyleVar();
	ImGui::PopID();

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
	ImGui::Spacing();
}

void Menu::Slider(int id, const char* text, double* value, double min, double max, const char* format) {
	ImVec2 textSize = Menu::Font->CalcTextSizeA(Menu::Font->FontSize, FLT_MAX, 0.0f, text);

	ImGui::PushID(id);
	ImGui::SetCursorPos(ImVec2(20.0f, ImGui::GetCursorPosY() + 5.0f));
	ImGui::Text(text);
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 230.0f - textSize.x - 20.0f);
	ImGui::SliderDouble("", value, min, max, format);
	ImGui::SameLine();
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
	ImGui::PopID();

	ImGui::PushID(id - 9);
	ImGui::InputDouble("", value, 0, 0, format);
	ImGui::PopStyleVar();
	ImGui::PopID();

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5.0f);
	ImGui::Spacing();
}

void Menu::Slider(int id, const char* text, int* value, int min, int max, const char* format) {
	ImVec2 textSize = Menu::Font->CalcTextSizeA(Menu::Font->FontSize, FLT_MAX, 0.0f, text);

	ImGui::PushID(id);
	ImGui::SetCursorPos(ImVec2(20.0f, ImGui::GetCursorPosY() + 5.f));
	ImGui::Text(text);
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 230.0f - textSize.x - 20.0f);
	ImGui::SliderInt("", value, min, max);
	ImGui::SameLine();
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
	ImGui::PopID();

	ImGui::PushID(id - 9);
	ImGui::InputIntK("", value, 1, 100, format);
	ImGui::PopStyleVar();
	ImGui::PopID();

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5.0f);
	ImGui::Spacing();
}

void Menu::Toggle(int id, const char* text, bool* value) {
	ImVec2 textSize = Menu::Font->CalcTextSizeA(Menu::Font->FontSize, FLT_MAX, 0.0f, text);

	ImGui::SetCursorPos(ImVec2(20.f, ImGui::GetCursorPosY() + 5.f));

	ImGui::PushID(id);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5.f);
	ImGui::Text(text);
	ImGui::SameLine();

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 1.1f);
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 340 - textSize.x);
	Menu::ToggleButton(text, value);
	ImGui::PopID();

	ImGui::Spacing();
}

void Menu::ToggleWithKeybind(int id, const char* text, bool* value, int* key) {
	ImGui::SetCursorPos(ImVec2(20.f, ImGui::GetCursorPosY() + 5.f));

	float cursor_x = ImGui::GetCursorPosX();

	ImGui::PushID(id);

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5.f);
	ImGui::Text(text);
	ImGui::SameLine();

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.3f);
	ImGui::SetCursorPosX(cursor_x + 120.0f);

	ImGui::Hotkey("##bind", key, { 100.f, 25.f });
	ImGui::SameLine();

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 1.1f);
	ImGui::SetCursorPosX(cursor_x + 348.0f);

	Menu::ToggleButton(text, value);
	ImGui::PopID();
	ImGui::Spacing();
}

void Menu::ShowOption(const char* text, bool* show) {
	ImGui::SetCursorPos(ImVec2(20.f, ImGui::GetCursorPosY() + 5.f));

	float cursor_x = ImGui::GetCursorPosX();

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5.f);
	ImGui::Text(text);
	ImGui::SameLine();

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.5f);
	ImGui::SetCursorPosX(cursor_x + 525.0f);

	if (ImGui::AdaptedButton(*show ? "-" : "+", { 25.f, 25.f }))
	{
		*show = !*show;
	}

	ImGui::Spacing();
}
void Menu::ToggleWithKeybind(int id, const char* text, bool* value, int* key, bool* show) {
	ImGui::SetCursorPos(ImVec2(20.0f, ImGui::GetCursorPosY() + 5));

	float cursor_x = ImGui::GetCursorPosX();

	ImGui::PushID(id);

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5.f);
	ImGui::Text(text);
	ImGui::SameLine();

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.3f);
	ImGui::SetCursorPosX(cursor_x + 120.0f);

	ImGui::Hotkey("##bind", key, { 100.f, 25.f });
	ImGui::SameLine();

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 1.1f);
	ImGui::SetCursorPosX(cursor_x + 348.0f);

	Menu::ToggleButton(text, value);
	ImGui::SameLine();

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.5f);
	ImGui::SetCursorPosX(cursor_x + 525.0f);

	if (ImGui::AdaptedButton(*show ? "-" : "+", { 25.f, 25.f }))
	{
		*show = !*show; 
	}

	ImGui::PopID();
	ImGui::Spacing();
}

bool Menu::ColorEdit(const char* text, float col[4]) {
	ImGui::SetCursorPos(ImVec2(20.0f, ImGui::GetCursorPosY() + 5));

	float cursor_x = ImGui::GetCursorPosX();

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5.0f);
	ImGui::Text(text);
	ImGui::SameLine();

	ImGui::SetCursorPosX(cursor_x + 358.0f);
	bool tounched = ImGui::ColorEdit4(text, col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
	ImGui::Spacing();

	return tounched;
}

ImVec4 Menu::Darken(const ImVec4& color, float factor) {
	factor = std::clamp(factor, 0.0f, 1.0f);

	return ImVec4(
		color.x * (1.0f - factor),
		color.y * (1.0f - factor),
		color.z * (1.0f - factor),
		color.w
	);
}

/*
src: unknowncheats.me
old asf but still works pretty well
*/
void Menu::ToggleButton(const char* format, bool* value)
{
	ImVec2 p = ImGui::GetCursorScreenPos();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	float height = ImGui::GetFrameHeight() - 5;
	float width = height * 1.65f;
	float radius = height * 0.50f;

	if (ImGui::InvisibleButton(format, ImVec2(width, height)))
		*value = !*value;
	ImU32 col_circle;
	ImU32 col_bg;

	if (ImGui::IsItemHovered())
	{
		col_circle = *value ? IM_COL32(255, 255, 255, 255) : IM_COL32(230, 230, 230, 255);
		col_bg = *value ? ImGui::GetColorU32(ImGuiCol_SliderGrab, 0.9f) : ImGui::GetColorU32(ImGuiCol_SliderGrabActive);
	}
	else
	{
		col_circle = *value ? IM_COL32(230, 230, 230, 255) : IM_COL32(175, 175, 175, 255);
		col_bg = *value ? ImGui::GetColorU32(ImGuiCol_SliderGrab) : ImGui::GetColorU32(ImGuiCol_SliderGrabActive, 0.5);
	}
	 
	float offsetX = 8;
	float offsetY = 3;

	draw_list->AddRectFilled(ImVec2(p.x + offsetX, p.y + offsetY), ImVec2(p.x + width + offsetX, p.y + height + offsetY), col_bg, height * 0.5f);
	draw_list->AddCircleFilled(ImVec2(*value ? (p.x + width - radius + offsetX) : (p.x + radius + offsetX), p.y + radius + offsetY), radius - 1.5f, col_circle);
}

