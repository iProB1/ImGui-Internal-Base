#pragma once
#include <string>
#include <d3d9.h>

namespace Menu {
   inline bool show_overlay = true;

   inline bool setup = false;
   inline HWND hwnd = nullptr;
   inline WNDCLASSEX wnd_class = { };
   inline WNDPROC org_wndproc = nullptr;
   inline LPDIRECT3DDEVICE9 device = nullptr;
   inline LPDIRECT3D9 d3d9 = nullptr;

   bool setup_wnd_class(const char* class_name) noexcept;
   void destroy_wnd_class() noexcept;

   bool setup_hwnd(const char* name) noexcept;
   void destroy_hwnd() noexcept;

   bool SetupDX() noexcept;
   void DestroyDX() noexcept;

   void Core();

   void SetupMenu(LPDIRECT3DDEVICE9 device) noexcept;
   void Destroy() noexcept;

   void Render() noexcept;
}

namespace Window {
	inline uintptr_t base = 0;

}