<<<<<<< HEAD
#pragma once
#include <d3d11.h>			 
#include <windowsx.h>		 
#include <vector>			 
#include <string>			 
#include <chrono>			 
							 
namespace menu {			 
	inline int width = 0; //da cambiare
	inline int height = 0; 

	inline int index = 0;

	inline DXGI_SWAP_CHAIN_DESC sd = {};
	inline constexpr D3D_FEATURE_LEVEL levels[2]{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0
	};

	inline bool exit = false;
	inline bool show = false;

	inline float alpha = 0.0f;
	inline bool fadingIn = false;
	inline bool fadingOut = false;
	inline auto lastTime = std::chrono::steady_clock::now();

	void UpdateFade();
	void ShowMenu();
	void HideMenu();
	void UpdateClickability();

	inline HWND window = nullptr;

	inline WNDCLASSEXW windowClass = {};
	
	inline ID3D11Device* device{ nullptr };
	inline ID3D11DeviceContext* device_context{ nullptr };
	inline IDXGISwapChain* swap_chain{ nullptr };
	inline ID3D11RenderTargetView* render_target_view{ nullptr };
	inline D3D_FEATURE_LEVEL level{};

	void CreateHWindow(LPCWSTR windowName, LPCWSTR className) noexcept;
	void DestroyHWindow() noexcept;

	bool CreateDevice() noexcept;
	void DestroyDevice() noexcept;

	void CreateImGui() noexcept;
	void DestroyImGui() noexcept;

	void BeginRender() noexcept;
	void EndRender() noexcept;
	void Render() noexcept;
}
=======
#pragma once
#include <d3d11.h>			 
#include <windowsx.h>		 
#include <vector>			 
#include <string>			 
#include <chrono>			 
							 
namespace menu {			 
	inline int width = 0;
	inline int height = 0; 

	inline int index = 0;

	inline DXGI_SWAP_CHAIN_DESC sd = {};
	inline constexpr D3D_FEATURE_LEVEL levels[2]{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0
	};

	inline bool exit = false;
	inline bool show = false;

	inline float alpha = 0.0f;
	inline bool fadingIn = false;
	inline bool fadingOut = false;
	inline auto lastTime = std::chrono::steady_clock::now();

	void UpdateFade();
	void ShowMenu();
	void HideMenu();
	void UpdateClickability();

	inline HWND window = nullptr;

	inline WNDCLASSEXW windowClass = {};
	
	inline ID3D11Device* device{ nullptr };
	inline ID3D11DeviceContext* device_context{ nullptr };
	inline IDXGISwapChain* swap_chain{ nullptr };
	inline ID3D11RenderTargetView* render_target_view{ nullptr };
	inline D3D_FEATURE_LEVEL level{};

	void CreateHWindow(LPCWSTR windowName, LPCWSTR className) noexcept;
	void DestroyHWindow() noexcept;

	bool CreateDevice() noexcept;
	void DestroyDevice() noexcept;

	void CreateImGui() noexcept;
	void DestroyImGui() noexcept;

	void BeginRender() noexcept;
	void EndRender() noexcept;
	void Render() noexcept;

}
>>>>>>> 72f0e4004c77e605edd9bfe0f871a88bae23b5fb
