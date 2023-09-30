// ===�^�C�g���{�^���N���X����===
// �^�C�g���J�ڃ{�^���N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "SceneManager.h"
#include "TitleButton.h"
#include "TextureManager.h"

// ===�R���X�g���N�^===
TitleButton::TitleButton()
{
	// ---������---
	m_transform.Scale = XMFLOAT3(96.0f, 32.0f, 1.0f);			// �T�C�Y
	m_uv = XMFLOAT2(0.0f, 0.0f);								// uv������W
	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);					// �F
	m_TexCut = XMFLOAT2(1, 1);									// uv�E�����W
	m_id = UI_TITLE_BUTTON;										// �^�O
	m_Texture = TEX->Load("data/texture/EnterButton.png");		// �e�N�X�`���Ǎ�
}

// ===�f�X�g���N�^===
TitleButton::~TitleButton()
{
}

// ===�{�^����������===
void TitleButton::OnPushed()
{
	SCENE->SetScene(SCENE_TITLE);								// �^�C�g���V�[���ɑJ��
}