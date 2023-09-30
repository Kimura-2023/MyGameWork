// ===テクスチャ生成クラス定義===
// テクスチャ生成クラスを定義
// Author kimura
// ======================

// ===定数定義===
#define _CRT_SECURE_NO_WARNINGS

// ===インクルード===
#include <memory.h>
#include <stdlib.h>
#include "TextureFactory.h"
#include "RenderTarget.h"
#include "DepthStencil.h"

#pragma comment(lib, "DirectXTex")

using namespace DirectX;

D3D11_TEXTURE2D_DESC MakeTexDesc(DXGI_FORMAT format, UINT width, UINT height)
{
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.Format = format;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;
	return desc;
}

// ===ファイルから生成===
Texture* TextureFactory::CreateFromFile(const char* fileName)
{
	HRESULT hr = S_OK;

	// 文字変換
	wchar_t wPath[MAX_PATH];
	size_t wLen = 0;
	MultiByteToWideChar(0, 0, fileName, -1, wPath, MAX_PATH);

	// ファイル別読み込み
	TexMetadata mdata;
	ScratchImage image;
	if (strstr(fileName, ".tga"))
	{
		hr = LoadFromTGAFile(wPath, &mdata, image);
	}
	else
	{
		hr = LoadFromWICFile(wPath, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &mdata, image);
	}
	if (FAILED(hr)) {
		return nullptr;
	}

	// シェーダリソース生成
	ID3D11ShaderResourceView* pSRV;
	hr = CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), mdata, &pSRV);
	if (FAILED(hr)) { return nullptr; }

	// インスタンス生成
	Texture* pTexture = new Texture;
	pTexture->m_pSRV = pSRV;
	pTexture->m_width = (UINT)mdata.width;
	pTexture->m_height = (UINT)mdata.height;
	return pTexture;
}

// ===データから生成===
Texture* TextureFactory::CreateFromData(DXGI_FORMAT format, UINT width, UINT height, const void* pData)
{
	// テクスチャ設定
	D3D11_TEXTURE2D_DESC desc = MakeTexDesc(format, width, height);

	// インスタンス生成
	Texture* pTexture = new Texture;
	if (SUCCEEDED(pTexture->CreateResource(desc, pData)))
	{
		pTexture->m_width = width;
		pTexture->m_height = height;
	}
	else
	{
		delete pTexture;
		pTexture = nullptr;
	}

	return pTexture;
}

// ===レンダーターゲット生成===
Texture* TextureFactory::CreateRenderTarget(DXGI_FORMAT format, UINT width, UINT height)
{
	// テクスチャ設定
	D3D11_TEXTURE2D_DESC desc = MakeTexDesc(format, width, height);
	desc.BindFlags |= D3D11_BIND_RENDER_TARGET;

	// インスタンス生成
	RenderTarget* pTexture = new RenderTarget;
	if (SUCCEEDED(pTexture->CreateResource(desc)))
	{
		pTexture->m_width = width;
		pTexture->m_height = height;
	}
	else
	{
		delete pTexture;
		pTexture = nullptr;
	}
	return pTexture;
}

// ===スクリーンから生成===
Texture* TextureFactory::CreateRenderTargetFromScreen()
{
	HRESULT hr;
	RenderTarget* pTexture = new RenderTarget;

	// バックバッファのポインタを取得
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pTexture->m_pTex);

	// バックバッファへのポインタを指定してレンダーターゲットビューを作成
	if (SUCCEEDED(hr))
	{
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.Texture2D.MipSlice = 0;
		hr = GetDevice()->CreateRenderTargetView(pTexture->m_pTex, &rtvDesc, &pTexture->m_pRTV);
		if (SUCCEEDED(hr))
		{
			D3D11_TEXTURE2D_DESC desc;
			pTexture->m_pTex->GetDesc(&desc);
			pTexture->m_width = desc.Width;
			pTexture->m_height = desc.Height;
		}
	}

	// 作成に失敗していたら削除
	if (FAILED(hr))
	{
		delete pTexture;
		pTexture = nullptr;
	}

	return pTexture;
}

// ===デプスステンシル生成===
Texture* TextureFactory::CreateDepthStencil(UINT width, UINT height, bool useStencil)
{
	// https://docs.microsoft.com/ja-jp/windows/win32/direct3d11/d3d10-graphics-programming-guide-depth-stencil#compositing
	// テクスチャ設定
	D3D11_TEXTURE2D_DESC desc = MakeTexDesc(
		useStencil ? DXGI_FORMAT_R24G8_TYPELESS : DXGI_FORMAT_R32_TYPELESS, width, height);
	desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;

	// インスタンス生成
	DepthStencil* pTexture = new DepthStencil;
	if (SUCCEEDED(pTexture->CreateResource(desc)))
	{
		pTexture->m_width = width;
		pTexture->m_height = height;
	}
	else
	{
		delete pTexture;
		pTexture = nullptr;
	}
	return pTexture;
}