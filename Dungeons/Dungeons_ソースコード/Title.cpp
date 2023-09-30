// ===�^�C�g�����S�N���X����===
// �^�C�g�����SUI�N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "Title.h"
#include "TextureManager.h"

// ===�R���X�g���N�^===
Title::Title()
{
	// ---����������---
	m_transform.Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);			// �������W
	m_Scale = XMFLOAT2(1200, 300);							// �g�k
	m_transform.Rot = XMFLOAT3(0.0f, 0.0f, 0.0f);			// ��]
	m_uv = XMFLOAT2(0.0f, 0.0f);							// uv������W
	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);				// �F
	m_TexCut = XMFLOAT2(1.0f, 1.0f);						// uv�E�����W
	m_id = BG_TITLE;										// �^�O
	m_bEnable = true;										// �X�V�E�`���
	m_Blend = BS_ALPHABLEND;								// �u�����h�X�e�[�g
	m_ResourcePass = "data/texture/TitleLogo.png";			// �e�N�X�`���̃p�X
	m_Texture = TEX->Load("data/texture/TitleLogo.png");	// �e�N�X�`���ǂݍ���
}

// ===�f�X�g���N�^===
Title::~Title()
{
}

// ===�X�V����===
void Title::Update()
{
	m_transform.Scale.x += 0.005f;		// ���X�Ɋg��(x����)
	m_transform.Scale.y += 0.005f;		// ���X�Ɋg��(y����)
	if (m_transform.Scale.x > 0.9f)			// ���T�C�Y�ȏ�ŏ���������
	{
		m_transform.Scale.x = 0.7f;		// �g�k��0.7�{��
		m_transform.Scale.y = 0.7f;		// �g�k��0.7�{��
	}

	Object2DBase::Update();		// �X�V
}

// ===�`��===
void Title::Draw()
{
	Object2DBase::Draw();		// �`��
}

// ===�X�V�L��===
bool Title::IsUpdate()
{
	return true;
}