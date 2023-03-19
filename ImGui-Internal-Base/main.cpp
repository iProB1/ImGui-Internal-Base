#include <thread>
#include <stdexcept>
#include "Core/globals.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

LRESULT CALLBACK WNDProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );

bool Menu::setup_wnd_class(const char* class_name) noexcept
{
	wnd_class.cbSize = sizeof(WNDCLASSEX);
	wnd_class.style = CS_HREDRAW | CS_VREDRAW;
	wnd_class.lpfnWndProc = DefWindowProc;
	wnd_class.cbClsExtra = 0;
	wnd_class.cbWndExtra = 0;
	wnd_class.hInstance = GetModuleHandle(NULL);
	wnd_class.hIcon = 0;
	wnd_class.hCursor = 0;
	wnd_class.hbrBackground = 0;
	wnd_class.lpszMenuName = 0;
	wnd_class.lpszClassName = class_name;
	wnd_class.hIconSm = 0;

	if (!RegisterClassEx(&wnd_class))
		return false;

	return true;
}
void Menu::destroy_wnd_class() noexcept
{
	UnregisterClass( wnd_class.lpszClassName, wnd_class.hInstance);
}

bool Menu::setup_hwnd(const char* name) noexcept
{
	hwnd = CreateWindow( wnd_class.lpszClassName, name, WS_OVERLAPPEDWINDOW, 0, 0, 10, 10, 0, 0, wnd_class.hInstance, 0);

	if (!hwnd)
		return false;

	return true;
}
void Menu::destroy_hwnd() noexcept
{
	if (hwnd)
		DestroyWindow(hwnd);
}

bool Menu::SetupDX() noexcept
{
	const auto handle = GetModuleHandle("d3d9.dll");

	if (!handle)
		return false;

	using CreateFn = LPDIRECT3D9(__stdcall*)(UINT);

	const auto create = reinterpret_cast<CreateFn>(GetProcAddress(handle, "Direct3DCreate9"));
	if (!create)
		return false;

	d3d9 = create(D3D_SDK_VERSION);

	if (!d3d9)
		return false;

	D3DPRESENT_PARAMETERS params = {  };
	params.BackBufferWidth = 0;
	params.BackBufferHeight = 0;
	params.BackBufferFormat = D3DFMT_UNKNOWN;
	params.BackBufferCount = 0;
	params.MultiSampleType = D3DMULTISAMPLE_NONE;
	params.MultiSampleQuality = 0;
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	params.hDeviceWindow = hwnd;
	params.Windowed = 1;
	params.EnableAutoDepthStencil = 0;
	params.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	params.Flags = 0;
	params.FullScreen_RefreshRateInHz = 0;
	params.PresentationInterval = 0;

	if (d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT, &params, &device) < 0) 
		return false;

	return true;
}
void Menu::DestroyDX() noexcept
{
	if (device)
	{
		device->Release();
		device = NULL;
	}

	if (d3d9)
	{
		d3d9->Release();
		d3d9 = NULL;
	}
}

void Menu::Core()
{
	if (!setup_wnd_class("class1"))
		throw std::runtime_error("Failed to create window class!");

	if (!setup_hwnd("ImGuiBaseProB1"))
		throw std::exception("failed to create window / SF");

	if (!SetupDX())
		throw std::runtime_error("failed to create device");

	destroy_hwnd();
	destroy_wnd_class();
}

void Menu::SetupMenu(LPDIRECT3DDEVICE9 device) noexcept
{
	auto params = D3DDEVICE_CREATION_PARAMETERS{ };
	device->GetCreationParameters(&params);

	hwnd = params.hFocusWindow;
	org_wndproc = reinterpret_cast<WNDPROC>( SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WNDProc)) );

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(device);

	setup = true;
}
void Menu::Destroy() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	SetWindowLongPtr( hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(org_wndproc) );

	DestroyDX();
}

void Menu::Render() noexcept
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("ImGuiBaseProB1", &show_overlay);

	ImGui::Text("Internal base by ProB1#0100");

	ImGui::End();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

LRESULT CALLBACK WNDProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

	if (GetAsyncKeyState(VK_INSERT) & 1)
		Menu::show_overlay = !Menu::show_overlay;

	if (Menu::show_overlay && ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) 
		return true;

	return CallWindowProc(Menu::org_wndproc, hwnd, msg, wparam, lparam);
}