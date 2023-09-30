// ===�I���{�^���N���X����===
// �I���{�^���N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "SceneManager.h"
#include "FinButton.h"
#include "TextureManager.h"

// ===�R���X�g���N�^===
FinButton::FinButton()
{
	// ---����������---
	m_transform.Scale = XMFLOAT3(192.0f, 64.0f, 1.0f);			// �T�C�Y
	m_uv = XMFLOAT2(0.0f, 0.0f);								// uv����
	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);					// �F
	m_TexCut = XMFLOAT2(1, 1);									// uv�E��
	m_id = UI_FIN_BUTTON;										// �I�u�W�F�N�g�^�O
	m_Texture = TEX->Load("data/texture/EndButton.png");		// �e�N�X�`��
}

// ===�f�X�g���N�^===
FinButton::~FinButton()
{
}

// ===�{�^���������̏���===
void FinButton::OnPushed()
{
	Close();					// ���C�����[�v�I������
}