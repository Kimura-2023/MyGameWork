#ifdef _MSC_VER
#pragma once
#endif

// ===テクスチャ生成クラス定義===
// テクスチャ生成クラスを定義
// Author kimura
// ======================

// ===インクルード===
#include <d3d11.h>
#include <DirectXTex.h>

#pragma warning(push)
#pragma warning(disable : 4005)
#include <stdint.h>
#pragma warning(pop)
#include "Texture.h"

// ===クラス定義===
class TextureFactory
{
	// ---メンバ関数---
public:
	static Texture* CreateFromFile(const char* fileName);											// ファイル名から生成
	static Texture* CreateFromData(DXGI_FORMAT format, UINT width, UINT height, const void* pData);	// データから生成
	static Texture* CreateRenderTarget(DXGI_FORMAT format, UINT width, UINT height);				// RenderTarget生成
	static Texture* CreateRenderTargetFromScreen();													// スクリーンから生成
	static Texture* CreateDepthStencil(UINT width, UINT height, bool useStencil);					// DepthStencil生成
};