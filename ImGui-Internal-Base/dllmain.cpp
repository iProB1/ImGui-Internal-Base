#include <stdexcept>
#include <windows.h>
#include "Core/globals.h"
#include "Core/utils.h"
#include "hooks/hooks.h"
#include <iostream>
#include "Core/colored_cout.h"

HMODULE hlmodule = nullptr;
bool loop = true;

void injection_thread()
{
	utils::Attach_Console();
	SetConsoleTitleA("[ImGui Internal Base by ProB1]");
	printf("base address: 0x%llx\n", Window::base);
	try
	{
		Menu::Core();
		hooks::Setup();
	}
	catch (const std::exception& err)
	{
		std::cout << clr::red << "Error: " << err.what() << std::endl;
		//MessageBoxA(0, err.what(), "error", MB_OK | MB_ICONEXCLAMATION);
	}
	while (loop)
	{
		if (GetAsyncKeyState(VK_F1) & 1 && GetAsyncKeyState(VK_F2) & 1)
			loop = false;
	}

	utils::Detach_Console();
	hooks::Destroy();
	Menu::Destroy();
	FreeLibraryAndExitThread(hlmodule, 1);
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved) {
	
	switch (reason)
	{
	case DLL_PROCESS_ATTACH: {
		hlmodule = module;
		Window::base = (uintptr_t)GetModuleHandleW(nullptr);
		DisableThreadLibraryCalls(module);
		CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(injection_thread), nullptr, 0, nullptr);
		} break;
	case DLL_PROCESS_DETACH: break;
	}
	return TRUE;
}