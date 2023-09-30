// ===�X�^�~�iUI�N���X����===
// �X�^�~�iUI�N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "PlayerStamina.h"
#include "Player.h"
#include "SceneManager.h"
#include "Polygon.h"
#include "TextureManager.h"

// ===�R���X�g���N�^===
PlayerStamina::PlayerStamina()
{
	// ---������---
	m_transform.Pos = XMFLOAT3(-SCREEN_WIDTH / 2 + 160 + 16, SCREEN_HEIGHT / 2 - 96, 0.0f);			// �������W
	m_transform.Scale = XMFLOAT3(320.0f, 16.0f, 1.0f);												// �g�k
	m_uv = XMFLOAT2(0.0f, 0.0f);							// uv������W
	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);				// �F
	m_TexCut = XMFLOAT2(1, 1);								// uv�E�����W
	m_id = UI_STAMINA;										// �^�O
	m_Texture = TEX->Load("data/texture/stamina.png");		// �e�N�X�`���Ǎ�
}

// ===�f�X�g���N�^===
PlayerStamina::~PlayerStamina()
{
}

// ===�X�V����===
void PlayerStamina::Update()
{
	Object* player = SCENE->GetCurrentScene()->FindObjectWithTag(OT_PLAYER);	// �v���C���[�擾

	// ---��O����---
	if (!player)
		return;

	// ---�X�^�~�i�X�V---
	float stamina = (float)((Player*)player)->GetStamina();
	m_transform.Scale.x = 320.0f * stamina / 50.0f;								// �ő�X�^�~�i���ŏ��Z
	m_transform.Pos.x = -SCREEN_WIDTH / 2 + m_transform.Scale.x / 2 + 16;		// �g�k�����������ړ�(�Q�[�W�̂悤�Ȍ����ڂɂ���)
}

// ===�`�揈��===
void PlayerStamina::Draw()
{
	// ---�`���---
	if (!m_bEnable)
		return;
	if (m_Texture == nullptr)
		return;

	// ---�`�揈��---
	SetZWrite(false);													// �[�x�o�b�t�@�X�V�ۃZ�b�g
	ID3D11DeviceContext* pDC = GetDeviceContext();						// �f�o�C�X�R���e�L�X�g�擾
	SetBlendState(m_Blend);												// �u�����h�����Z�b�g
	CPolygon::SetColor(m_color);										// �F���Z�b�g
	CPolygon::SetTexture(m_Texture);									// �e�N�X�`�����Z�b�g
	CPolygon::SetSize(m_transform.Scale.x, m_transform.Scale.y);		// �T�C�Y�Z�b�g
	CPolygon::SetUV(m_uv.x, m_uv.y);									// ����uv���W�Z�b�g
	CPolygon::SetFrameSize(m_TexCut.x, m_TexCut.y);						// �E��uv���W�Z�b�g
	CPolygon::SetRotate(m_transform.Rot);								// ��]�Z�b�g	
	CPolygon::SetPos(m_transform.Pos.x, m_transform.Pos.y);
	CPolygon::Draw(pDC);

	SetZWrite(true);
}