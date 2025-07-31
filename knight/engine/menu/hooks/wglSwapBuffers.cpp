#include "../menu.h"

#include <gl/GL.h>
#include <mutex>
#include <Shlobj.h>

#include "../../../ext/minhook/minhook.h"
#include "../../../ext/imgui/imgui.h"
#include "../../../ext/imgui/imgui_impl_win32.h"
#include "../../../ext/imgui/imgui_impl_opengl2.h"
#include "../../util/logger.hpp"
#include "../../module/implement/visuals/esp.hpp"
#include "../../knight.hpp"

std::once_flag setupFlag;
std::atomic_flag clipCursor = ATOMIC_FLAG_INIT;
RECT originalClip;

typedef bool(__stdcall* template_wglSwapBuffers) (HDC hdc);
template_wglSwapBuffers original_wglSwapBuffers;
bool __stdcall hook_wglSwapBuffers(_In_ HDC hdc)
{
	// handling fullscreen context switching before we set the new hwnd, so we can compare them
	
	// if the cached hwnd isnt equal to the current one
	// info: window handles change when you enter/exit fullscreen
	if (Menu::HandleWindow != WindowFromDC(hdc) && Menu::Initialized)
	{
		// set handlewindow so that the wndproc can attach to new one before passing
		Menu::HandleWindow = WindowFromDC(hdc);

		// opengl context for font issue
		Menu::OriginalGLContext = wglGetCurrentContext();
		wglMakeCurrent(hdc, wglCreateContext(hdc));

		// uninitialize imgui opengl and win32 implementation
		ImGui_ImplOpenGL2_Shutdown();
		ImGui_ImplWin32_Shutdown();
		
		// reinitialize it
		ImGui_ImplWin32_Init(Menu::HandleWindow);
		ImGui_ImplOpenGL2_Init();

		// set wndproc
		Menu::Hook_wndProc();

		// revert to old ctx
		wglMakeCurrent(hdc, Menu::OriginalGLContext);

		// end detour
		return original_wglSwapBuffers(hdc);
	}

	Menu::HandleDeviceContext = hdc;
	Menu::HandleWindow = WindowFromDC(hdc);
	Menu::OriginalGLContext = wglGetCurrentContext();

	std::call_once(setupFlag, [&] {
		Menu::Hook_wndProc();
		Menu::SetupImgui();
	});

	wglMakeCurrent(Menu::HandleDeviceContext, Menu::MenuGLContext);

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	if (Menu::Open)
	{
		if (clipCursor.test_and_set()) GetClipCursor(&originalClip);

		ClipCursor(NULL);
		Menu::RenderMenu();
	}
	else
	{
		// checking if originalClip is valid
		if (originalClip.right > originalClip.left && originalClip.bottom > originalClip.top)
		{
			ClipCursor(&originalClip);
			clipCursor.clear();
		}
	}

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
	ImGui::Begin("Overlay", nullptr,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoInputs |
		ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoBackground);

	if (knight::context::is_running) {

		if (Menu::Watermark) {
			ImGui::PushFont(Menu::Font);
			ImGui::PushStyleColor(ImGuiCol_Text, ImColor(255, 255, 255).Value);

			ImGui::Text("KNIGHT");
			ImGui::PopFont();

			ImGui::SameLine();

			ImGui::PushFont(Menu::IconFont);
			ImGui::Text("K");
			ImGui::PopFont();
			ImGui::PopStyleColor();
		}

		implement::visuals::esp::handle_render();

		ImGui::PushFont(Menu::Font);
		implement::visuals::array_list::handle_render();
		ImGui::PopFont();
	}

	ImGui::End();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	wglMakeCurrent(Menu::HandleDeviceContext, Menu::OriginalGLContext);
	return original_wglSwapBuffers(hdc);
}

void Menu::Hook_wglSwapBuffers()
{
	LPVOID wglSwapBuffers = (LPVOID)GetProcAddress(GetModuleHandle("opengl32.dll"), "wglSwapBuffers");
	MH_CreateHook(wglSwapBuffers, (LPVOID)hook_wglSwapBuffers, (LPVOID*)&original_wglSwapBuffers);
	MH_EnableHook(wglSwapBuffers);
}

void Menu::Unhook_wglSwapBuffers()
{	
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
}

void Menu::SetupImgui()
{
	Menu::MenuGLContext = wglCreateContext(Menu::HandleDeviceContext);
	wglMakeCurrent(Menu::HandleDeviceContext, Menu::MenuGLContext);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	GLint m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);

	glOrtho(0, m_viewport[2], m_viewport[3], 0, 1, -1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0, 0, 0, 0);

	Menu::CurrentImGuiContext = ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();

	Menu::Font = io.Fonts->AddFontFromMemoryCompressedTTF(font_compressed_data, font_compressed_size, 22);
	Menu::IconFont = io.Fonts->AddFontFromMemoryCompressedTTF(iconfont_compressed_data, iconfont_compressed_size, 24);

	ImVec4* colors = ImGui::GetStyle().Colors;

	if (Menu::light_mode) {
		apply_light_theme(colors);
	}
	else {
		apply_dark_theme(colors);
	}

	colors[ImGuiCol_SliderGrab] = ImVec4(Menu::Accent[0], Menu::Accent[1], Menu::Accent[2], Menu::Accent[3]);
	colors[ImGuiCol_SliderGrabActive] = Menu::Darken(colors[ImGuiCol_SliderGrab], 0.4f);

	colors[ImGuiCol_Header] = ImVec4(0.33f, 0.35f, 0.36f, 0.53f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.f, 0.44f, 0.44f, 0.67f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.47f, 0.47f, 0.47f, 0.67f);
	colors[ImGuiCol_Separator] = ImVec4(0.32f, 0.32f, 0.32f, 0.3f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	colors[ImGuiCol_Tab] = ImVec4(0.07f, 0.07f, 0.07f, 0.51f);
	colors[ImGuiCol_TabHovered] = ImVec4(0, 0.23f, 0.23f, 0.67f);
	colors[ImGuiCol_TabActive] = ImVec4(0.19f, 0.19f, 0.19f, 0.57f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.05f, 0.05f, 0.05f, 0.90f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.13f, 0.13f, 0.74f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.07f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 10.0f;
	style.WindowBorderSize = 0;
	style.WindowPadding = ImVec2(0, 0);

	ImGui_ImplWin32_Init(Menu::HandleWindow);
	ImGui_ImplOpenGL2_Init();


	Menu::Initialized = true;
}

void Menu::apply_dark_theme(ImVec4* colors) {
	colors[ImGuiCol_Text] = ImColor(255, 255, 255);
	colors[ImGuiCol_TextDisabled] = ImColor(89, 89, 89);

	colors[ImGuiCol_WindowBg] = ImColor(25, 25, 25);
	colors[ImGuiCol_ChildBg] = ImColor(255, 255, 255);
	colors[ImGuiCol_PopupBg] = ImColor(20, 20, 20, 239);

	colors[ImGuiCol_Border] = ImColor(0, 0, 0, 127);
	colors[ImGuiCol_BorderShadow] = ImColor(255, 255, 255);

	colors[ImGuiCol_FrameBg] = ImColor(32, 32, 32);
	colors[ImGuiCol_FrameBgHovered] = ImColor(38, 38, 38);
	colors[ImGuiCol_FrameBgActive] = ImColor(11, 11, 11, 229);

	colors[ImGuiCol_TitleBg] = ImColor(16, 16, 16, 229);
	colors[ImGuiCol_TitleBgActive] = ImColor(16, 16, 16, 229);
	colors[ImGuiCol_TitleBgCollapsed] = ImColor(16, 16, 16, 229);

	colors[ImGuiCol_MenuBarBg] = ImColor(0.14f, 0.14f, 0.14f, 1.00f);

	colors[ImGuiCol_ScrollbarBg] = ImColor(5, 5, 5, 0);
	colors[ImGuiCol_ScrollbarGrab] = ImColor(79, 79, 79);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(104, 104, 104);
	colors[ImGuiCol_ScrollbarGrabActive] = ImColor(130, 130, 130);

	colors[ImGuiCol_CheckMark] = ImColor(142, 25, 25);

	colors[ImGuiCol_Button] = ImColor(36, 36, 36);
	colors[ImGuiCol_ButtonHovered] = ImColor(42, 42, 42);
	colors[ImGuiCol_ButtonActive] = ImColor(76, 76, 76);
}

void Menu::apply_light_theme(ImVec4* colors) {
	colors[ImGuiCol_Text] = ImColor(20, 20, 20);
	colors[ImGuiCol_TextDisabled] = ImColor(140, 140, 140);

	colors[ImGuiCol_WindowBg] = ImColor(245, 245, 245);
	colors[ImGuiCol_ChildBg] = ImColor(255, 255, 255);
	colors[ImGuiCol_PopupBg] = ImColor(250, 250, 250, 240);

	colors[ImGuiCol_Border] = ImColor(180, 180, 180);
	colors[ImGuiCol_BorderShadow] = ImColor(255, 255, 255);

	colors[ImGuiCol_FrameBg] = ImColor(230, 230, 230);
	colors[ImGuiCol_FrameBgHovered] = ImColor(220, 220, 220);
	colors[ImGuiCol_FrameBgActive] = ImColor(200, 200, 200);

	colors[ImGuiCol_TitleBg] = ImColor(235, 235, 235);
	colors[ImGuiCol_TitleBgActive] = ImColor(225, 225, 225);
	colors[ImGuiCol_TitleBgCollapsed] = ImColor(245, 245, 245);

	colors[ImGuiCol_MenuBarBg] = ImColor(240, 240, 240);

	colors[ImGuiCol_ScrollbarBg] = ImColor(250, 250, 250, 0);
	colors[ImGuiCol_ScrollbarGrab] = ImColor(180, 180, 180);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(160, 160, 160);
	colors[ImGuiCol_ScrollbarGrabActive] = ImColor(140, 140, 140);

	colors[ImGuiCol_CheckMark] = ImColor(160, 40, 40);

	colors[ImGuiCol_Button] = ImColor(230, 230, 230);
	colors[ImGuiCol_ButtonHovered] = ImColor(215, 215, 215);
	colors[ImGuiCol_ButtonActive] = ImColor(190, 190, 190);
}
