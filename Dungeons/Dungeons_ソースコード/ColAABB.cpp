// ===�����蔻��(AABB)�N���X����===
// �����蔻��(AABB)�N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "ColAABB.h"
#include "Light.h"
#include "ModelManager.h"
#include "DebugProc.h"
#include "ColSphere.h"

// ===�ÓI�����o===
CBox* CollAABB::m_pBox = nullptr;			// �f�o�b�O�\���p���b�V��
int CollAABB::m_nCnt = 0;					// AABB����

// ===�R���X�g���N�^(�I�t�Z�b�g�Ȃ�)===
CollAABB::CollAABB(Collision3D* col, XMFLOAT3 size)
{
	// ---����������---
	Init();									// ������
	m_Size = size;							// �T�C�Y���Z�b�g
	SetParent(col);							// �����蔻��̌����Z�b�g
	HRESULT hr = m_pBox->Init(m_Size);		// ���b�V��������

	// ---��O����---
	if (FAILED(hr))
	{
		m_pBox->Fin();			// �f�o�b�O�`��I��
		SAFE_DELETE(m_pBox);	// ���
		return;
	}
	m_pBox->SetMaterial(&m_Material);		// �}�e���A���Z�b�g
}

// ===�R���X�g���N�^(�I�t�Z�b�g����)===
CollAABB::CollAABB(Collision3D* col, XMFLOAT3 off, XMFLOAT3 size)
{
	// ---����������---
	Init();									// ������
	m_Offset = off;							// �I�t�Z�b�g���Z�b�g
	m_Size = { size.x / 2,size.y / 2,size.z / 2 };		// �T�C�Y���Z�b�g
	SetParent(col);							// �e���Z�b�g
	HRESULT hr = m_pBox->Init(m_Size);		// ���b�V��������

	// ---��O����---
	if (FAILED(hr))
	{
		m_pBox->Fin();			// �f�o�b�O�`��I��
		SAFE_DELETE(m_pBox);	// ���
		return;
	}
	m_pBox->SetMaterial(&m_Material);		// �}�e���A���Z�b�g
}

// ===�f�X�g���N�^===
CollAABB::~CollAABB()
{
	// ---�I������---
	--m_nCnt;					// �����蔻�葍������
	if (m_nCnt <= 0)
	{
		m_pBox->Fin();			// �f�o�b�O�\���I��
		SAFE_DELETE(m_pBox);	// ���
	}
}

// ===����������===
void CollAABB::Init()
{
	// ---������---
	m_Size = XMFLOAT3(1.0f, 1.0f, 1.0f);			// �����T�C�Y
	m_coltype = COL_AABB;							// �����蔻��^

	if (m_nCnt == 0)
	{
		m_pBox = new CBox();						// ���b�V������
	}
	m_nCnt++;										// AABB�������Z
}

// ===�q�b�g�m�F===
bool CollAABB::CheckHit(CollisionBase* col)
{
	// ---���O����---
	if (col->GetFollowCollision() == GetFollowCollision())
		return false;
	if (!col->GetEnable())
		return false;

	// ---�q�b�g�m�F---
	XMFLOAT3 min, max;															// �{�����[���v�Z
	min = { m_Pos.x - m_Size.x ,m_Pos.y - m_Size.y ,m_Pos.z - m_Size.z };
	max = { m_Pos.x + m_Size.x ,m_Pos.y + m_Size.y ,m_Pos.z + m_Size.z };

	XMFLOAT3 pos = col->GetCenter();											// ������W

	switch (col->GetColType())
	{
	case COL_AABB: {													// ---AABB���m�̔���
		XMFLOAT3 size = ((CollAABB*)col)->GetSize();
		XMFLOAT3 omin, omax;
		omin = { pos.x - size.x ,pos.y - size.y ,pos.z - size.z };		// ����ŏ�
		omax = { pos.x + size.x ,pos.y + size.y ,pos.z + size.z };		// ����ő�

		m_bHit = false;						// �q�b�g��Ԃ��X�V

		// ---��Փˎ�---
		if (min.x > omax.x) return false;
		if (max.x < omin.x) return false;
		if (min.y > omax.y) return false;
		if (max.y < omin.y) return false;
		if (min.z > omax.z) return false;
		if (max.z < omin.z) return false;

		// ---�Փˎ�---
		CDebugProc::Print("˯�\n");			// �f�o�b�O�\��
		m_bHit = true;						// �q�b�g��Ԃ��X�V
		return true;						
		break;
	}
	case COL_NONE:
		break;
	default:
		break;
	}
	return false;
}

// ===�f�o�b�O�\��===
void CollAABB::Draw()
{
	// ---���O����---
	if (!GetVisible())
		return;

	// ---�`�揈��---
	m_pBox->SetMaterial(&m_Material);		// �}�e���A�����Z�b�g
	SetBlendState(BS_ALPHABLEND);			// �u�����h�X�e�[�g�ݒ�

	m_pBox->SetWorld(&GetWorld());			// ���[���h�ϊ��s����Z�b�g
	m_pBox->Draw();							// �f�o�b�O�\��

	SetBlendState(BS_NONE);					// �u�����h�X�e�[�g�ݒ�
}

// ===AABB�T�C�Y�擾===
XMFLOAT3 CollAABB::GetSize()
{
	return m_Size;
}