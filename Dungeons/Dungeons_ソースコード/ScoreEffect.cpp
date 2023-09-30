// ===�X�R�A�G�t�F�N�g�N���X����===
// �X�R�A�G�t�F�N�g�N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "ScoreEffect.h"
#include "Polygon.h"
#include "TextureManager.h"
#include "Calc.h"

// ===�萔�E�}�N����`===
#define TEX_WIDTH (56.0f)
#define TEX_HEIGHT (48.0f)

// ===�R���X�g���N�^===
ScoreEffect::ScoreEffect()
{
	// ---����������---
	m_transform.Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);		// �������W
	m_transform.Scale = XMFLOAT3(0.7f, 0.6f, 1.0f);		// �T�C�Y
	m_uv = XMFLOAT2(0.0f, 0.0f);						// uv������W
	m_TexCut = XMFLOAT2(1, 1);							// uv�E����W
	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);			// �F
	m_id = BILL_SCORE;									// �I�u�W�F�N�g�^�O
	m_bBillboard = true;								// �r���{�[�h�ݒ�
	m_Blend = BS_ALPHABLEND;							// �u�����h�ݒ�
	m_Texture = TEX->Load("data/texture/Number.png");	// �e�N�X�`��
	m_nScore = 0;
}

// ===�f�X�g���N�^===
ScoreEffect::~ScoreEffect()
{
}

// ===�X�V����===
void ScoreEffect::Update()
{
	// ---�X�V����---
	if (!m_bEnable)
		return;

	// ---��Ԃ��X�V---
	m_color.w -= 0.01f;					// ���l�����X�ɉ�����
	//m_transform.Pos.y += 0.01f;			// �ړ����x��ݒ�

	if (m_color.w <= 0.0f)				// �����ɂȂ�����
	{
		m_bEnable = false;		// �X�V�ۂ�ݒ�
		m_color.w = 1.0f;		// ���l�����ɖ߂�
	}

}

// ===�`�揈��===
void ScoreEffect::Draw()
{
	// ---�`�攻��
	if (!m_bEnable)
		return;
	if (m_Texture == nullptr)
		return;

	// ---�`�揀��---
	SetZWrite(false);													// �[�x�o�b�t�@�X�V�ۃZ�b�g
	ID3D11DeviceContext* pDC = GetDeviceContext();						// �f�o�C�X�R���e�L�X�g�擾
	SetBlendState(m_Blend);												// �u�����h�����Z�b�g
	CPolygon::SetColor(m_color);										// �F���Z�b�g
	CPolygon::SetDraw(!m_bBillboard);									// UI�����false
	CPolygon::SetBill(m_bBillboard);									// �r���{�[�h�ݒ�
	CPolygon::SetTexture(m_Texture);									// �e�N�X�`�����Z�b�g	
	CPolygon::SetSize(m_transform.Scale.x, m_transform.Scale.y);		// �T�C�Y�Z�b�g
	CPolygon::SetFrameSize(1.0f / 10.0f, 0.5f);							// �E��uv���W�Z�b�g

	// ---�X�R�A�\��---
	int score = m_nScore;
	int digit = 0;														// ����
	Digit(&digit, score);												// �������Z�o
	for (int i = 0; i < digit; ++i)
	{
		int num = (score / (int)pow(10, digit - (i + 1))) % 10;			// �X�R�A���ꌅ���`�悷�邽�߂Ɍ����Ƃ̒l���Z�o
		CPolygon::SetPos(m_transform.Pos.x + m_transform.Scale.x / 2 * i, m_transform.Pos.y, m_transform.Pos.z);		// ���W���Z�b�g
		CPolygon::SetUV((float)num / 10.0f, 0.0f);													// ����uv���W�Z�b�g
		CPolygon::Draw(pDC);																		// ������`��
	}

	CPolygon::SetBill(!m_bBillboard);		// �r���{�[�h�ݒ��������
	CPolygon::SetDraw(m_bBillboard);		// UI�`��ݒ�
	SetZWrite(true);			// �[�x�o�b�t�@�X�V�ۃZ�b�g
	SetZBuffer(true);			// �[�x�o�b�t�@�L����
}

// ===�X�R�A�ݒ�===
void ScoreEffect::SetScore(const int& num)
{
	m_nScore = num;
}