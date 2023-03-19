#include "hooks.h"
#include <stdexcept>
#include <intrin.h>
#include <thread>

#include "../minhook/MinHook.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../Core/colored_cout.h"
#include <iostream>



void hooks::Setup()
{
	if (MH_Initialize()) 
		throw std::runtime_error("failed to init minhook!");

	if (MH_CreateHook(VF(Menu::device, 42), &EndScene,
		reinterpret_cast<void**>(&EndSceneOrg))) throw std::runtime_error("unable to hook end_scene");
	else
		std::cout << clr::green << "Hooked end_scene\n";

	if (MH_CreateHook(VF(Menu::device, 16), &Reset,
		reinterpret_cast<void**>(&ResetOrg))) throw std::runtime_error("unable to hook reset");

	if (MH_EnableHook(MH_ALL_HOOKS))
		throw std::runtime_error("hookfunction error");
	else
		std::cout << clr::magenta << "Enabled Hooks!\n";

	Menu::DestroyDX();
}

void hooks::Destroy() noexcept
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

long __stdcall hooks::EndScene(IDirect3DDevice9* device) noexcept
{
	const auto result = EndSceneOrg(device, device);

	if (!Menu::setup)
		Menu::SetupMenu(device);

	if (Menu::show_overlay)
		Menu::Render();

	return result;
}

HRESULT __stdcall hooks::Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const auto result = ResetOrg(device, device, params);
	ImGui_ImplDX9_CreateDeviceObjects();

	return result;
}