// ===�����蔻��x�[�X�N���X����===
// �����蔻��x�[�X�N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "CollisionBase.h"
#include "DebugProc.h"

// ===�R���X�g���N�^===
CollisionBase::CollisionBase()
{
	// ---����������---
	m_Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);							// ���[���h���W
	m_Rot = XMFLOAT3(0.0f, 0.0f, 0.0f);							// ���[�J����]
	m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);						// ���[�J���X�P�[��
	m_Offset = XMFLOAT3(0.0f, 0.0f, 0.0f);						// ���[�J�����W
	m_pParent = nullptr;										// �����蔻��̑匳
	XMStoreFloat4x4(&m_World,XMMatrixIdentity());				// ���[���h�}�g���b�N�X
	m_coltype = COL_NONE;										// �����蔻����
	m_Enable = true;											// �����蔻��L��
	m_Visible = false;											// �f�o�b�O�\���L��
	m_bHit = false;												// �Փ˂������ǂ���
	Init();														// �f�o�b�O�\���̃}�e���A����񏉊���
}

// ===�f�X�g���N�^===
CollisionBase::~CollisionBase()
{
}

// ===�}�e���A��������===
void CollisionBase::Init()
{
	m_Material.m_Diffuse = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.5f);
	m_Material.m_Ambient = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.5f);
	m_Material.m_Emissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_Material.m_Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_Material.m_Power = 1.0f;
}

// ===�X�V===
void CollisionBase::Update()
{
	// ---�f�o�b�O�\���ύX����---
	if(m_bHit)		// �Փˎ�
		m_Material.m_Diffuse = m_Material.m_Ambient = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f);		// �ԐF
	else			// ��Փˎ�
		m_Material.m_Diffuse = m_Material.m_Ambient = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.5f);		// �ΐF

	m_bHit = false;			// �����蔻���ԍX�V
	UpdateMatrix();			// �s��v�Z
	m_Pos.x = m_World._41;	// x���W�ݒ�
	m_Pos.y = m_World._42;	// y���W�ݒ�
	m_Pos.z = m_World._43;	// z���W�ݒ�
}

void CollisionBase::UpdateMatrix()
{
	// ---���[�J���ϐ�---
	XMMATRIX mtxWorld, mtxRot, mtxTranslate;
	mtxWorld = XMMatrixIdentity();	// ������

	XMMATRIX parent = XMLoadFloat4x4(&m_pParent->GetWorld());		// �e�s��擾

	// ---��]�𔽉f---
	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_Rot.x),XMConvertToRadians(m_Rot.y), XMConvertToRadians(m_Rot.z));		// ��]�s��v�Z
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);					// ���[���h�ϊ��s��ɔ��f

	// ---�ړ��𔽉f---
	mtxTranslate = XMMatrixTranslation(m_Offset.x, m_Offset.y, m_Offset.z);		// �ړ��s��v�Z
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);			// ���[���h�ϊ��s��ɔ��f

	// ---�e����̃��[���h�ϊ������f������---
	mtxWorld = mtxWorld * parent;

	// ---���[���h�}�g���b�N�X�ݒ�---
	XMStoreFloat4x4(&m_World, mtxWorld);
}

// ===���S���W�擾===
XMFLOAT3& CollisionBase::GetCenter()
{
	return m_Pos;
}

// ===���[���h�}�g���b�N�X�擾===
XMFLOAT4X4& CollisionBase::GetWorld()
{
	return m_World;
}

// ===�L�����ǂ����擾===
bool CollisionBase::GetEnable()
{
	return m_Enable;
}

// ===�L����===
void CollisionBase::SetEnable(bool flg)
{
	m_Enable = flg;
}

// ===�����蔻��̎�ގ擾===
COLTYPE CollisionBase::GetColType()
{
	return m_coltype;
}

// ===����Ԏ擾===
bool CollisionBase::GetVisible()
{
	return m_Visible;
}

// ===����ԃZ�b�g===
void CollisionBase::SetVisible(bool flg)
{
	m_Visible = flg;
}

// ===�e�Z�b�g===
void CollisionBase::SetParent(Collision3D* col)
{
	m_pParent = col;
}

// ===�e�擾===
Collision3D* CollisionBase::GetFollowCollision()
{
	return m_pParent;
}

// ===�Փˏ�Ԏ擾===
bool CollisionBase::GetHit()
{
	return m_bHit;
}