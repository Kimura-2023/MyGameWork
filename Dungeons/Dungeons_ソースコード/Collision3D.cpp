// ===�����蔻��N���X����===
// �����蔻��N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "Collision3D.h"
#include "ColSphere.h"
#include "ColAABB.h"
#include "ColOBB.h"
#include "ColLine.h"
#include "CollisionManager.h"
#include "DebugProc.h"
#include "Calc.h"

// ===�R���X�g���N�^===
Collision3D::Collision3D(Object* obj)
{
	// ---����������---
	m_pObj = obj;										// �Q�ƃI�u�W�F�N�g�ݒ�
	XMStoreFloat4x4(&m_mWorld, XMMatrixIdentity());		// ���[���h�s�񏉊���
	m_vCenter = m_pObj->GetTransform().Pos;				// ���S���W�ݒ�
	m_vSize = m_pObj->GetTransform().Scale;				// �X�P�[���ݒ�
	m_bCollider = false;								// �߂荞�ݗʂ�߂����ǂ���
	COLLISION->AddManager(this);						// �����蔻��Ǘ����ɒǉ�
}

// ===�f�X�g���N�^===
Collision3D::~Collision3D()
{
	// ---�������---
	COLDIT colit = m_pColList.begin();					// �����蔻�胊�X�g�̐擪�C�e���[�^�擾
	while (colit != m_pColList.end())					// �I�[�܂ŌJ��Ԃ�
	{
		delete *colit;									// ���
		*colit = nullptr;								//
		colit = m_pColList.erase(colit);				// ���X�g����폜
	}
	COLLISION->Release(this);							// �Ǘ�����������
}

// ===AABB�����蔻��ǉ�===
void Collision3D::AddCollisionAABB()
{
	m_vCenter = m_pObj->GetTransform().Pos;									// ���S���W�ݒ�
	CollAABB* box = new CollAABB(this,m_pObj->GetTransform().Scale);		// AABB����
	m_pColList.push_back(box);												// �����蔻�胊�X�g�ɒǉ�
}

// ===AABB�����蔻��ǉ�(�I�t�Z�b�g����)
void Collision3D::AddCollisionAABB(const XMFLOAT3& offset , const XMFLOAT3& size)
{
	m_vCenter = m_pObj->GetTransform().Pos;									// ���S���W�ݒ�
	CollAABB* box = new CollAABB(this, offset,size);						// AABB����
	m_pColList.push_back(box);												// �����蔻�胊�X�g�ɒǉ�
}

// ===���������蔻��ǉ�===
void Collision3D::AddCollisionLine()
{
	m_vCenter = m_pObj->GetTransform().Pos;									// ���S���W�ݒ�
	CollLine* line = new CollLine(this,XMFLOAT3(0.0f,0.0f,0.0f));			// ��������
	m_pColList.push_back(line);												// �����蔻�胊�X�g�ɒǉ�
}

// ===���������蔻��ǉ�(�I�t�Z�b�g����)===
void Collision3D::AddCollisionLine(const XMFLOAT3& offset, const XMFLOAT3& end)
{
	m_vCenter = m_pObj->GetTransform().Pos;									// ���S���W�ݒ�
	CollLine* line = new CollLine(this, offset, end);						// ��������
	m_pColList.push_back(line);												// �����蔻�胊�X�g�ɒǉ�
}

// ===���E�������蔻��ǉ�===
void Collision3D::AddCollisionSphere()
{
	m_vCenter = m_pObj->GetTransform().Pos;									// ���S���W�ݒ�
	CollSphere* sphere = new CollSphere(this,m_pObj->GetRad());				// ���E������
	m_pColList.push_back(sphere);											// �����蔻�胊�X�g�ɒǉ�
}

// ===���E�������蔻��ǉ�(�I�t�Z�b�g����)===
void Collision3D::AddCollisionSphere(const XMFLOAT3& offset, const float& rad)
{
	m_vCenter = m_pObj->GetTransform().Pos;									// ���S���W�ݒ�
	CollSphere* sphere = new CollSphere(this,offset,rad);					// ���E������
	m_pColList.push_back(sphere);											// �����蔻�胊�X�g�ɒǉ�
}

// ===OBB�����蔻��ǉ�===
void Collision3D::AddCollisionOBB()
{
	m_vCenter = m_pObj->GetTransform().Pos;									// ���S���W�ݒ�
	CollOBB* obb = new CollOBB(this, m_pObj->GetTransform().Scale);			// OBB����
	m_pColList.push_back(obb);												// �����蔻�胊�X�g�ɒǉ�
}

// ===OBB�����蔻��ǉ�(�I�t�Z�b�g����)===
void Collision3D::AddCollisionOBB(const XMFLOAT3& offset, const XMFLOAT3& size)
{
	m_vCenter = m_pObj->GetTransform().Pos;									// ���S���W�ݒ�
	CollOBB* obb = new CollOBB(this, offset, size);							// OBB����
	m_pColList.push_back(obb);												// �����蔻�胊�X�g�ɒǉ�
}

// ===�X�V����===
void Collision3D::Update()
{
	m_vCenter = m_pObj->GetPos();				// �Q�Ƃ���I�u�W�F�N�g�̒��S���W�ɍ��킹��
	m_vSize = m_pObj->GetScale();
	m_vRot = m_pObj->GetRotate();				// ��]�����l�ɔ��f������

	m_mWorld = m_pObj->GetWorld();				// �s��X�V
	m_bEnableflg = m_pObj->GetCol();			// �����蔻��̉ۍX�V
	m_Movement = XMFLOAT3(0.0f, 0.0f, 0.0f);	// �߂荞�񂾗ʂ����Z�b�g

	if (!m_pObj->GetEnable())
		m_bEnableflg = false;					// �����蔻����Ƃ邩�ǂ���

	COLDIT it = m_pColList.begin();				// �����蔻�胊�X�g�̐擪�C�e���[�^�擾
	while (it != m_pColList.end())				// �����蔻�胊�X�g�̏I�[�܂ŌJ��Ԃ�
	{
		// ---��O����---
		if ((*it) == nullptr)
		{
			it = m_pColList.erase(it);			// �����蔻�胊�X�g����폜
			continue;
		}

		// ---���X�g���̓����蔻�肻�ꂼ��̍X�V---
		(*it)->Update();						// �X�V����
		++it;									// ���̓����蔻���T��
	}
}

// ===�s��X�V===
void Collision3D::UpdateMatrix()
{
	XMMATRIX mtxWorld,mtxTranslate,mtxRot,mtxScale;
	mtxWorld = XMMatrixIdentity();						// �s�񏉊���

	// ---�g�k���f---
	mtxScale = XMMatrixScaling(m_vSize.x, m_vSize.y, m_vSize.z);			// �X�P�[������ɍs��𐶐�
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScale);						// ���[���h�ϊ��s��ɔ��f������

	// ---��]�𔽉f---
	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_vRot.x), 
		XMConvertToRadians(m_vRot.y), XMConvertToRadians(m_vRot.z));		// ��]�s��𐶐�
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);							// ���[���h�ϊ��s��ɔ��f

	Quaternion q;										// �N�H�[�^�j�I����]�̔��f
	q = m_pObj->GetQuaternion();						// �Q�ƃI�u�W�F�N�g�̎p�����擾
	Quaternion::GetRotateMatrix(&mtxRot, q);			// �N�H�[�^�j�I�������]�s����쐬
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);		// ���[���h�ϊ��s��ɔ��f
	
	// ---�ړ��𔽉f---
	mtxTranslate = XMMatrixTranslation(m_vCenter.x, m_vCenter.y, m_vCenter.z);		// �ړ��s��𐶐�
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);							// ���[���h�ϊ��s��ɔ��f

	// ---���[���h�}�g���b�N�X�ݒ�---
	XMStoreFloat4x4(&m_mWorld, mtxWorld);				// �����o�ϐ��ɔ��f
}

// ---�����蔻��f�o�b�O�\��---
void Collision3D::Draw()
{
	// ---���O����---
	if (!m_pObj->GetVisible())
		return;
	if (!m_pObj->GetEnable())
		return;

	// ---�`��---
	SetZWrite(false);							// ZWrite���I�t
	SetBlendState(BS_ALPHABLEND);				// �u�����h�X�e�[�g�ݒ�
	COLDIT it = m_pColList.begin();				// �����蔻�胊�X�g�̐擪�C�e���[�^�擾
	while (it != m_pColList.end())				// �I�[�܂ŌJ��Ԃ�
	{
		// ---��O����---
		if ((*it) == nullptr)
		{
			m_pColList.erase(it);				// ���X�g���珜�O
			continue;
		}

		// ---�`�揈��---
		(*it)->Draw();							// �f�o�b�O�\��
		++it;									// ���̓����蔻���T��
	}

	// ---�`��ݒ�����ɖ߂�---
	SetBlendState(BS_NONE);						// �u�����h�ݒ�
	SetZWrite(true);							// 
}

// ===�q�b�g�m�F===
bool Collision3D::CheckCollision(Collision3D* col)
{
	std::vector<CollisionBase*> other = col->GetCollisionData();		// ���I�u�W�F�N�g�̓����蔻��

	COLDIT it, otherit;
	it = m_pColList.begin();							// �������蔻�胊�X�g�̐擪�C�e���[�^�擾
	otherit = other.begin();							// ����̓����蔻�胊�X�g�̐擪�C�e���[�^�擾
	bool bHit = false;									// �����������ǂ���

	// ---����---
	while (it != m_pColList.end())						// ---�������蔻�胊�X�g�̏I�[�܂ŌJ��Ԃ�
	{
		while (otherit != other.end())						// ---���蓖���蔻�胊�X�g�̏I�[�܂ŌJ��Ԃ�
		{
			bHit = (*it)->CheckHit(*otherit);				// �q�b�g�m�F
			++otherit;										// ���̑��蓖���蔻���T��

			// ---�Փˎ�---
			if (bHit == true)
				return bHit;									// ���̃I�u�W�F�N�g�̂ǂ����̓����蔻��ɂԂ����Ă�����true��Ԃ�
		}
		++it;											// ���̓����蔻��T��
	}

	return bHit;										// ���ʂ�Ԃ�
}

// ===�����蔻��ۂ�ݒ�===
void Collision3D::SetCollision(bool flg)
{
	m_bEnableflg = flg;
}

// ===�߂荞�ݗʂ�߂����ǂ�����ݒ�===
void Collision3D::SetCollider(bool flg)
{
	m_bCollider = flg;
}

// ===�f�o�b�O�\���L���̐ؑ�====
void Collision3D::SwitchVisible()
{
	COLDIT it = m_pColList.begin();					// �����蔻�胊�X�g�̐擪�C�e���[�^�擾
	while (it != m_pColList.end())					// �����蔻�胊�X�g�̏I�[�܂ŌJ��Ԃ�
	{
		// ---��O����---
		if ((*it) == nullptr)
		{
			m_pColList.erase(it);					// ���X�g���珜�O
			continue;
		}

		// ---�ؑ�---
		(*it)->SetVisible(!(*it)->GetVisible());	// ����Ԃ̐ؑ�
		++it;										// ���̓����蔻���T��
	}
}

// ===�߂��ʂ̍X�V===
void Collision3D::CalcMovement(XMFLOAT3* movement)
{
	// ---�߂荞�񂾋������Βl�Ŕ�r���A�傫�����Ōv�Z����---
	if (fabs(m_Movement.x) < fabs(movement->x))
		m_Movement.x += movement->x;				// x����
	if (fabs(m_Movement.y) < fabs(movement->y))
		m_Movement.y += movement->y;				// y����
	if (fabs(m_Movement.z) < fabs(movement->z))
		m_Movement.z += movement->z;				// z����
}

// ===�߂��ʂ��擾===
XMFLOAT3& Collision3D::GetMovement()
{
	// ---��O����---
	if (std::isnan(m_Movement.x) || std::isnan(m_Movement.y) || std::isnan(m_Movement.z))
		return m_Movement = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// ---�l��Ԃ�---
	return m_Movement;
}

// ===�Փ˓_��ݒ�===
void Collision3D::SetHitPos(XMFLOAT3* hitpos)
{
	m_HitPos = *hitpos;
}

// ===�Փ˓X���擾===
XMFLOAT3& Collision3D::GetHitPos()
{
	// ---��O����---
	if (std::isnan(m_HitPos.x) || std::isnan(m_HitPos.y) || std::isnan(m_HitPos.z))
		return m_HitPos = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// ---�l��Ԃ�---
	return m_HitPos;
}