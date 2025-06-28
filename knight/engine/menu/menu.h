#pragma once

#include <string>
#include <Windows.h>
#include <vector>
 
#include "../game_data.h"
#include "../util/fonts.h"
#include "../util/logger.hpp"
#include "../module/module_manager.hpp"
#include "../config/config_manager.hpp"

#include "../../ext/imgui/imgui.h"
#include "../../ext/imgui/imgui_internal.h"
#include "../../ext/imgui/imgui_impl_win32.h"
#include "../../ext/imgui/imgui_impl_opengl2.h"

struct Menu
{
	static void Init();
	static void Kill();

	static inline bool config_opt = false;

	static inline bool Open;
	static inline int Keybind;

	static inline bool Watermark = true;
	static inline float Accent[4]{ 0.4f, 0.7f, 0.7f, 1.0f };


	static inline ImFont* Font;
	static inline ImFont* IconFont;
	static inline bool Initialized;

	static void SetupImgui();
	static void RenderMenu();
	 
	static bool Dropdown(const char* label, const char* items[], int* item_current, int items_count, ImVec2 size = ImVec2(0, 0));

	static void Text(const char* text);

	static void Slider(int id, const char* text, float* bruh, float min, float max, const char* format = "%.2f");
	static void Slider(int id, const char* text, double* bruh, double min, double max, const char* format = "%.2f");
	static void Slider(int id, const char* text, int* bruh, int min, int max, const char* format = "%d");

	static void Toggle(int id, const char* text, bool* value);
	static void ToggleWithKeybind(int id, const char* text, bool* value, int* key);
	static void ShowOption(const char* text, bool* show);
	static void ToggleWithKeybind(int id, const char* text, bool* value, int* key, bool* shown);

	static bool ColorEdit(const char* text, float col[4]);

	static ImVec4 Darken(const ImVec4& color, float factor);

	static void ToggleButton(const char* format, bool* value);


	static inline HWND HandleWindow;
	static inline HDC HandleDeviceContext;

	static inline HGLRC OriginalGLContext;
	static inline HGLRC MenuGLContext;

	static inline ImGuiContext* CurrentImGuiContext;

	static void PlaceHooks();
	static void RemoveHooks();

	static void Hook_wglSwapBuffers();
	static void Hook_wndProc();

	static void Unhook_wglSwapBuffers();
	static void Unhook_wndProc();
};

