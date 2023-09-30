// ===UI�x�[�X�N���X����===
// UI�x�[�X�N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "UIBase.h"
#include "TextureManager.h"
#include "Polygon.h"
#include "ObjectManager.h"

// ===�R���X�g���N�^===
UIBase::UIBase()
{
	// ---����������---
	m_transform.Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);			// ���W
	m_transform.Rot = XMFLOAT3(0.0f, 0.0f, 0.0f);			// ��]
	m_transform.Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);			// �g�k
	m_bGravity = false;								// �d��
	m_bEnable = false;								// �X�V�E�`���
	m_Blend = BS_ALPHABLEND;						// �u�����h�X�e�[�g
	m_Texture = nullptr;							// �e�N�X�`��

	m_uv = XMFLOAT2(0.0f,0.0f);						// ����uv���W
	m_TexCut = XMFLOAT2(1.0f,1.0f);					// �e�N�X�`��������
	m_color = XMFLOAT4(1.0f,1.0f,1.0f,1.0);			// �F

	OBJECT->AddObjectManager(this);					// �I�u�W�F�N�g�Ǘ����ǉ�
}

// ===�f�X�g���N�^===
UIBase::~UIBase()
{
	OBJECT->Release(this);		// �I�u�W�F�N�g���
}

// ===�X�V����===
void UIBase::Update()
{

}

// ===�`�揈��===
void UIBase::Draw()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();		// �f�o�C�X�R���e�L�X�g�擾

	// ---�`�攻��---
	if (!m_bEnable)
		return;
	if (m_Texture == nullptr)
		return;

	// ---�`�揀��---
	SetZWrite(false);

	SetBlendState(m_Blend);
	CPolygon::SetColor(m_color);
	CPolygon::SetTexture(m_Texture);
	CPolygon::SetSize(m_transform.Scale.x,m_transform.Scale.y);
	CPolygon::SetUV(m_uv.x, m_uv.y);
	CPolygon::SetFrameSize(m_TexCut.x, m_TexCut.y);
	CPolygon::SetPos(m_transform.Pos.x, m_transform.Pos.y);
	CPolygon::SetRotate(m_transform.Rot);
	CPolygon::SetDraw(true);

	// ---�`��---
	CPolygon::Draw(pDC);

	// ---�`���Ԃ����ɖ߂�---
	SetZWrite(true);
	SetZBuffer(true);
}

// ===�f�ރp�X�ݒ�===
void UIBase::SetResource(std::string pass)
{
	m_ResourcePass = pass;
	m_Texture = TEX->Load(pass);
}

// ===�X�V����===
bool UIBase::IsUpdate()
{
	return true;
}

void UIBase::UpdateMatrix()
{

}