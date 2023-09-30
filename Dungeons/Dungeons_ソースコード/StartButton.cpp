// ===�J�n�{�^���N���X����===
// �Q�[���J�n�{�^���N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "SceneManager.h"
#include "StartButton.h"
#include "TextureManager.h"

// ===�R���X�g���N�^===
StartButton::StartButton()
{
	// ---������---
	m_transform.Scale = XMFLOAT3(192.0f, 64.0f, 1.0f);			// �������W
	m_uv = XMFLOAT2(0.0f, 0.0f);								// uv������W
	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);					// �F
	m_TexCut = XMFLOAT2(1, 1);									// uv�E�����W
	m_id = UI_START_BUTTON;										// �^�O
	m_Texture = TEX->Load("data/texture/StartButton.png");		// �e�N�X�`���Ǎ�
}

// ===�f�X�g���N�^===
StartButton::~StartButton()
{
}

// ===�{�^����������===
void StartButton::OnPushed()
{
	SCENE->SetScene(SCENE_GAME);				// �Q�[���V�[���ɐؑւ�
}