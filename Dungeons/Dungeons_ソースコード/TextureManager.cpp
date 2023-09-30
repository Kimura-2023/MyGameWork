#include "TextureManager.h"
#include "Polygon.h"
#include "Texture.h"
#include "DebugProc.h"
#include "TextureFactory.h"

// ===�O���[�o���ϐ�===
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

// ===�R���X�g���N�^===
TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	TEX->Release();
}

// ===����===
void TextureManager::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new TextureManager;
	}
}

// ===�j��===
void TextureManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

// ===�擾===
TextureManager* TextureManager::Get()
{
	return m_pInstance;
}

void TextureManager::Release()
{
	// �S�e�N�X�`�����
	auto it = m_TextureResource.begin();
	while (it != m_TextureResource.end())
	{
		delete it->second;
		++it;
	}
}

// ===�`�揈��===
void TextureManager::Draw()
{
}

// ===�ǂݍ���===
ID3D11ShaderResourceView* TextureManager::Load(std::string pass)
{
	auto it = m_TextureResource.find(pass);
	if (it == m_TextureResource.end())			// ���\�[�X��������Ȃ������ꍇ
	{
		ID3D11Device* pDevice = GetDevice();
		Texture* tex = TextureFactory::CreateFromFile(pass.c_str());
		if (tex)									// �e�N�X�`�����ǂݍ��߂��ꍇ
		{
			m_TextureResource.insert(std::make_pair(pass, tex));		// �}�b�v�ɓo�^
			return tex->GetResource();
		}
		else															// �e�N�X�`�����ǂݍ��߂Ȃ������ꍇ
		{
			return nullptr;												// nullptr��Ԃ�
		}
	}
	else																// ���\�[�X�����������ꍇ
	{
		return it->second->GetResource();								// ���\�[�X��Ԃ�
	}
}

// ===�G�f�B�^��ŕ\������p�X���擾===
LPCSTR GetTexturePass(TEX_TYPE type)
{
	return TexKind::pass[type];					// �p�X���擾
}