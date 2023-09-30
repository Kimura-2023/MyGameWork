// ===�X�R�AUI�N���X����===
// �X�R�AUI�N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "Score.h"
#include "Polygon.h"
#include "TextureManager.h"
#include "ScoreManager.h"
#include "Calc.h"

// ===�萔�E�}�N����`===
#define TEX_WIDTH (56.0f)
#define TEX_HEIGHT (48.0f)

// ===�R���X�g���N�^===
Score::Score()
{
	// ---����������---
	m_transform.Pos = XMFLOAT3(-SCREEN_WIDTH / 2 + TEX_WIDTH, -SCREEN_HEIGHT / 2 + TEX_HEIGHT, 0.0f);		// �������W
	m_transform.Scale = XMFLOAT3(TEX_WIDTH, TEX_HEIGHT, 1.0f);					// �T�C�Y
	m_uv = XMFLOAT2(0.0f, 0.0f);						// uv������W
	m_TexCut = XMFLOAT2(1, 1);							// uv�E����W
	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);			// �F
	m_id = UI_SCORE;									// �I�u�W�F�N�g�^�O
	m_Texture = TEX->Load("data/texture/Number.png");	// �e�N�X�`��
}

// ===�f�X�g���N�^===
Score::~Score()
{
}

// ===�X�V����===
void Score::Update()
{
}

// ===�`�揈��===
void Score::Draw()
{
	// ---�`�攻��
	if (!m_bEnable)
		return;
	if (m_Texture == nullptr)
		return;

	// ---�����`��---
	SetZWrite(false);													// �[�x�o�b�t�@�X�V�ۃZ�b�g
	ID3D11DeviceContext* pDC = GetDeviceContext();						// �f�o�C�X�R���e�L�X�g�擾
	SetBlendState(m_Blend);												// �u�����h�����Z�b�g
	CPolygon::SetColor(m_color);										// �F���Z�b�g
	if (SCENE->GetCurrentScene()->GetSceneName() == "GameScene")
		CPolygon::SetTexture(TEX->Load("data/texture/score.png"));			// �e�N�X�`�����Z�b�g
	else
		CPolygon::SetTexture(TEX->Load("data/texture/hiscore.png"));		// �e�N�X�`�����Z�b�g

	CPolygon::SetSize(240.0f, m_transform.Scale.y / 2);					// �T�C�Y�Z�b�g
	CPolygon::SetRotate(m_transform.Rot);								// ��]�Z�b�g				
	CPolygon::SetPos(m_transform.Pos.x + TEX_WIDTH, m_transform.Pos.y + TEX_HEIGHT);	// ���W���Z�b�g
	CPolygon::SetUV(0.0f, 0.0f);												// ����uv���W�Z�b�g
	CPolygon::SetFrameSize(1.0f, 1.0f);											// �E��uv���W�Z�b�g
	CPolygon::Draw(pDC);														// �`�揈��

	// ---�`�揀��---
	CPolygon::SetTexture(m_Texture);									// �e�N�X�`�����Z�b�g	
	CPolygon::SetSize(m_transform.Scale.x, m_transform.Scale.y);					// �T�C�Y�Z�b�g

	// ---�X�R�A�\��---
	int score;
	if (SCENE->GetCurrentScene()->GetSceneName() == "GameScene")		// �V�[���ɂ���ď�����ؑ�
		score = SCORE->GetScore();										// ���݃X�R�A�擾
	else
		score = SCORE->GetHiScore();									// �n�C�X�R�A���擾
	int digit = 0;														// ����
	Digit(&digit, score);												// �������Z�o
	for (int i = 0; i < digit; ++i)
	{
		int num = (score / (int)pow(10, digit - (i + 1))) % 10;
		CPolygon::SetPos(m_transform.Pos.x + m_transform.Scale.x / 2 * i, m_transform.Pos.y);		// ���W���Z�b�g
		CPolygon::SetUV((float)num / 10.0f, 0.0f);													// ����uv���W�Z�b�g
		CPolygon::SetFrameSize(1.0f / 10.0f, 0.5f);													// �E��uv���W�Z�b�g
		CPolygon::Draw(pDC);																		// ������`��
	}

	SetZWrite(true);			// �[�x�o�b�t�@�X�V�ۃZ�b�g
	SetZBuffer(true);			// �[�x�o�b�t�@�L����
}