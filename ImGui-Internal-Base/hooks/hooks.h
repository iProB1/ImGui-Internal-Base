#pragma once
#include "../Core/globals.h"

namespace hooks
{
	void Setup();
	void Destroy() noexcept;

	constexpr void* VF(void* ptr, size_t index) noexcept
	{
		return (*static_cast<void***>(ptr))[index];
	}

	using EndSceneFn = long(__thiscall*)(void*, IDirect3DDevice9*) noexcept;
	inline EndSceneFn EndSceneOrg = nullptr;
	long __stdcall EndScene(IDirect3DDevice9* device) noexcept;

	using ResetFn = HRESULT(__thiscall*)(void*, IDirect3DDevice9*, D3DPRESENT_PARAMETERS*) noexcept;
	inline ResetFn ResetOrg = nullptr;
	HRESULT __stdcall Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept;
}