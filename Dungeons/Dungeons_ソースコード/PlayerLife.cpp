// ===�̗�UI�N���X����===
// �̗�UI�N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "PlayerLife.h"
#include "Player.h"
#include "SceneManager.h"
#include "Polygon.h"
#include "TextureManager.h"

// ===�萔�E�}�N����`===
#define MAX_PLAYER_LIFE (10)		// �ő�̗�

// ===�R���X�g���N===
PlayerLife::PlayerLife()
{
	// ---����������---
	m_transform.Pos = XMFLOAT3(-SCREEN_WIDTH / 2 + 32, SCREEN_HEIGHT / 2 - 64, 0.0f);		// �������W
	m_transform.Scale = XMFLOAT3(64.0f, 32.0f, 1.0f);										// �T�C�Y
	m_uv = XMFLOAT2(0.0f, 0.0f);						// uv������W
	m_TexCut = XMFLOAT2(1, 1);							// uv�E����W
	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);			// �F
	m_id = UI_LIFE;										// �I�u�W�F�N�g�^�O
	m_Texture = TEX->Load("data/texture/life.png");		// �e�N�X�`��
	m_nLife = MAX_PLAYER_LIFE;							// �̗�
}

// ===�f�X�g���N�^===
PlayerLife::~PlayerLife()
{
}

// ===�X�V����===
void PlayerLife::Update()
{
	// ---�̗͍X�V---
	Object* player = SCENE->GetCurrentScene()->FindObjectWithTag(OT_PLAYER);		// �v���C���[�擾
		// ---��O����
	if (!player)
		return;
	m_nLife = ((Player*)player)->GetLife();				// �v���C���[�̗̑͂��擾����
}

// ===�`�揈��===
void PlayerLife::Draw()
{
	// ---�`�攻��
	if (!m_bEnable)
		return;
	if (m_Texture == nullptr)
		return;

	// ---�`��---
	SetZWrite(false);													// �[�x�o�b�t�@�X�V�ۃZ�b�g
	ID3D11DeviceContext* pDC = GetDeviceContext();						// �f�o�C�X�R���e�L�X�g�擾
	SetBlendState(m_Blend);												// �u�����h�����Z�b�g
	CPolygon::SetColor(m_color);										// �F���Z�b�g
	CPolygon::SetTexture(m_Texture);									// �e�N�X�`�����Z�b�g
	CPolygon::SetSize(m_transform.Scale.x, m_transform.Scale.y);		// �T�C�Y�Z�b�g
	CPolygon::SetUV(m_uv.x, m_uv.y);									// ����uv���W�Z�b�g
	CPolygon::SetFrameSize(m_TexCut.x, m_TexCut.y);						// �E��uv���W�Z�b�g
	CPolygon::SetRotate(m_transform.Rot);								// ��]�Z�b�g										

	for (int i = 0; i < m_nLife; ++i)				// �̗͂̐������`��
	{
		CPolygon::SetPos(m_transform.Pos.x + m_transform.Scale.x / 2 * i, m_transform.Pos.y);		// ���W���Z�b�g
		CPolygon::Draw(pDC);		// �`��
	}

	SetZWrite(true);			// �[�x�o�b�t�@�X�V�ۃZ�b�g
	SetZBuffer(true);			// �[�x�o�b�t�@�L����
}