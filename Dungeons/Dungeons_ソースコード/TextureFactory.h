#ifdef _MSC_VER
#pragma once
#endif

// ===�e�N�X�`�������N���X��`===
// �e�N�X�`�������N���X���`
// Author kimura
// ======================

// ===�C���N���[�h===
#include <d3d11.h>
#include <DirectXTex.h>

#pragma warning(push)
#pragma warning(disable : 4005)
#include <stdint.h>
#pragma warning(pop)
#include "Texture.h"

// ===�N���X��`===
class TextureFactory
{
	// ---�����o�֐�---
public:
	static Texture* CreateFromFile(const char* fileName);											// �t�@�C�������琶��
	static Texture* CreateFromData(DXGI_FORMAT format, UINT width, UINT height, const void* pData);	// �f�[�^���琶��
	static Texture* CreateRenderTarget(DXGI_FORMAT format, UINT width, UINT height);				// RenderTarget����
	static Texture* CreateRenderTargetFromScreen();													// �X�N���[�����琶��
	static Texture* CreateDepthStencil(UINT width, UINT height, bool useStencil);					// DepthStencil����
};