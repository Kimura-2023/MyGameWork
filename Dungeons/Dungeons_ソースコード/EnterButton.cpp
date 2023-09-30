// ===UI�N���X����===
// UI�N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "EnterButton.h"
#include "TextureManager.h"
#include "DebugProc.h"

// ===�R���X�g���N�^===
EnterButton::EnterButton()
{
	m_transform.Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);			// ���W
	m_Scale = XMFLOAT2(800, 200);							// �T�C�Y
	m_transform.Rot = XMFLOAT3(0.0f, 0.0f, 0.0f);			// ��]
	m_uv = XMFLOAT2(0.0f, 0.0f);							// uv������W
	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);				// �F
	m_TexCut = XMFLOAT2(1, 1);								// uv�E�����W

	m_id = BG_ENTER;									// �^�O
	m_bEnable = true;									// �X�V�E�`���
	m_Blend = BS_ALPHABLEND;							// �u�����h�X�e�[�g
	m_ResourcePass = "data/texture/EnterButton.png";	// �e�N�X�`���f�ނ̃p�X

	m_Texture = TEX->Load("data/texture/EnterButton.png");	// �e�N�X�`���Ǎ�
}

// ===�f�X�g���N�^===
EnterButton::~EnterButton()
{
}

// ===�X�V����===
void EnterButton::Update()
{
	Object2DBase::Update();			// 2D�I�u�W�F�N�g�X�V����
}

// ===�`�揈��===
void EnterButton::Draw()
{
	Object2DBase::Draw();			// �`��
}

// ===�X�V�������ǂ���===
bool EnterButton::IsUpdate()
{
	return true;
}