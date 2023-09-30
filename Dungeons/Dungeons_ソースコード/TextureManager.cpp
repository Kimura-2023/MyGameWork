#include "TextureManager.h"
#include "Polygon.h"
#include "Texture.h"
#include "DebugProc.h"
#include "TextureFactory.h"

// ===グローバル変数===
TextureManager* TextureManager::m_pInstance = nullptr;

namespace TexKind
{
	LPCSTR pass[MAX_TEX] = {
		"data/texture/BlockTex002.png",
		"data/texture/BlockTex003.png",
		"data/texture/field002.jpg",
		"data/texture/effect000.jpg",
		"data/texture/Title.png",
		"data/texture/TitleLogo.png",
		"data/texture/EnterButton.png",
		"data/texture/gameover.png",
		"data/texture/clear.png",
		"data/texture/ramp.png",
		"data/texture/BlockTex001.png",
		"data/texture/BlockTex004.png"
	};
}

// ===コンストラクタ===
TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	TEX->Release();
}

// ===生成===
void TextureManager::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new TextureManager;
	}
}

// ===破棄===
void TextureManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

// ===取得===
TextureManager* TextureManager::Get()
{
	return m_pInstance;
}

void TextureManager::Release()
{
	// 全テクスチャ解放
	auto it = m_TextureResource.begin();
	while (it != m_TextureResource.end())
	{
		delete it->second;
		++it;
	}
}

// ===描画処理===
void TextureManager::Draw()
{
}

// ===読み込み===
ID3D11ShaderResourceView* TextureManager::Load(std::string pass)
{
	auto it = m_TextureResource.find(pass);
	if (it == m_TextureResource.end())			// リソースが見つからなかった場合
	{
		ID3D11Device* pDevice = GetDevice();
		Texture* tex = TextureFactory::CreateFromFile(pass.c_str());
		if (tex)									// テクスチャが読み込めた場合
		{
			m_TextureResource.insert(std::make_pair(pass, tex));		// マップに登録
			return tex->GetResource();
		}
		else															// テクスチャが読み込めなかった場合
		{
			return nullptr;												// nullptrを返す
		}
	}
	else																// リソースが見つかった場合
	{
		return it->second->GetResource();								// リソースを返す
	}
}

// ===エディタ上で表示するパスを取得===
LPCSTR GetTexturePass(TEX_TYPE type)
{
	return TexKind::pass[type];					// パスを取得
}