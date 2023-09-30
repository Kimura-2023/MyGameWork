// ===2D�I�u�W�F�N�g�x�[�X
// ---Author kimura

// ====�C���N���[�h===
#include "Object2DBase.h"
#include "ObjectManager.h"
#include "Camera.h"
#include "DefineData.h"
#include "DebugProc.h"
#include "GameManager.h"
#include "CameraManager.h"
#include "Polygon.h"
#include "TextureManager.h"

// ===�R���X�g���N�^===
Object2DBase::Object2DBase()
{
	OBJECT->AddObjectManager(this);	// �I�u�W�F�N�g�ǉ�
	Init();							// ������
}

// ===�f�X�g���N�^===
Object2DBase::~Object2DBase()
{
	OBJECT->Release(this);			// �I�u�W�F�N�g�j��
}

// ===�X�V����===
void Object2DBase::Update()
{
	m_mWorld._41 = m_transform.Pos.x;
	m_mWorld._42 = m_transform.Pos.y;
	m_mWorld._43 = m_transform.Pos.z;

	// ---�����䔻��---
	CameraBase* pCamera = CAMERA->GetCurrentCamera();										// �J�����擾
	if (pCamera->CollisionViewFrustum(&m_transform.Pos, CalcObjSize(&m_Scale)) != 0)
	{
		m_bVisible = true;
	}
	else if (pCamera->CollisionViewFrustum(&m_transform.Pos, CalcObjSize(&m_Scale)) == 0)
	{
		m_bVisible = false;
	}

	// ---�X�V����---
	if (!m_bEnable)
		return;

	// ---�d��---
	if (m_bGravity)
		m_vAcc.y = -GRAVITY;

	// ---�����x���f---
	m_vVel.x += m_vAcc.x;
	m_vVel.y += m_vAcc.y;
	m_vVel.z += m_vAcc.z;

	// ---���x���f---
	m_transform.Pos.x += m_vVel.x;
	m_transform.Pos.y += m_vVel.y;
	m_transform.Pos.z += m_vVel.z;
}

// ===�`��===
void Object2DBase::Draw()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();			// �f�o�C�X�R���e�L�X�g�擾

	// ---�`�攻��---
	if (!m_bEnable)
		return;
	if (m_Texture == nullptr)
		return;

	// ---�`�揈��(�p�����[�^�Z�b�g)---
	SetBlendState(m_Blend);
	CPolygon::SetColor(m_color);
	CPolygon::SetTexture(m_Texture);
	CPolygon::SetSize(m_Scale.x * m_transform.Scale.x, m_Scale.y * m_transform.Scale.y);
	CPolygon::SetUV(m_uv.x, m_uv.y);
	CPolygon::SetFrameSize(m_TexCut.x, m_TexCut.y);
	CPolygon::SetPos(m_transform.Pos.x, m_transform.Pos.y);
	CPolygon::SetRotate(m_transform.Rot);
	CPolygon::SetDraw(m_bui);
	CPolygon::Draw(pDC);					// �`��

	SetZBuffer(true);
}

// ===�f�ރp�X�ݒ�===
void Object2DBase::SetResource(std::string pass)
{
	m_Texture = TEX->Load(pass);			// �e�N�X�`���Ǎ�
	if(m_Texture)
		m_ResourcePass = pass;				// �p�X�X�V
}

// ===�u�����h�X�e�[�g�ݒ�===
void Object2DBase::SetUseBlendState(EBlendState blend)
{
	m_Blend = blend;
}

// ===�}�g���b�N�X�X�V===
void Object2DBase::UpdateMatrix()
{
	XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScl = XMMatrixScaling(m_Scale.x * m_transform.Scale.x, m_Scale.y * m_transform.Scale.y,1.0f);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_transform.Rot.x),
		XMConvertToRadians(m_transform.Rot.y), XMConvertToRadians(m_transform.Rot.z));
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(m_transform.Pos.x, m_transform.Pos.y, m_transform.Pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�ݒ�
	XMStoreFloat4x4(&m_mWorld, mtxWorld);
}

// ===�E��UV�擾===
XMFLOAT2 Object2DBase::GetTexSize()
{
	XMFLOAT2 texsize;
	texsize.x = 1.0 / m_TexCut.x;
	texsize.y = 1.0 / m_TexCut.y;
	return texsize;
}

// ===�J���������Z�o===
float Object2DBase::GetCamVecSize()
{
	XMFLOAT3 cpos = CAMERA->GetCurrentCamera()->GetPos();		// �J�������W�擾
	float size;			// ����
	Length(&size, m_transform.Pos, cpos);		// �����Z�o
	return size;
}

// ===�T�C�Y�Z�o===
float Object2DBase::CalcObjSize(XMFLOAT2* f)
{
	return sqrt((f->x * f->x) + (f->y * f->y));
}

void Object2DBase::Init()
{
	// ---����������---
	m_transform = Transform();
	m_vAcc = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vVel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Scale = XMFLOAT2(1.0f, 1.0f);

	m_uv = XMFLOAT2(0.0f, 0.0f);
	m_TexCut = XMFLOAT2(1.0f, 1.0f);

	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	XMStoreFloat4x4(&m_mWorld, XMMatrixIdentity());

	m_Blend = BS_ALPHABLEND;

	m_Texture = nullptr;

	m_bGravity = false;
	m_bEnable = false;
	m_bVisible = false;
	m_bBillboard = true;
	m_bui = true;
}