// ===�����蔻��(OBB)�N���X����===
// �����蔻��(OBB)�N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "ColSphere.h"
#include "Light.h"
#include "ModelManager.h"
#include "DebugProc.h"
#include "ColOBB.h"

// ===�ÓI�����o===
CSphere* CollSphere::m_pBSphere = nullptr;					// �f�o�b�O�\���p���b�V��
int CollSphere::m_nCnt = 0;									// ���E������

// ===���E�������蔻��R���X�g���N�^(�I�t�Z�b�g�Ȃ�)===
CollSphere::CollSphere(Collision3D* col, float rad)
{
	// ---����������---
	Init();													// ������
	m_Radius = rad;											// ���a���Z�b�g
	SetParent(col);											// �e���Z�b�g
	HRESULT hr = m_pBSphere->Init(32, 16, m_Radius);		// �f�o�b�O�\��������

	// ---��O����---
	if (FAILED(hr))
	{
		m_pBSphere->Fin();							// �f�o�b�O�\���I������
		SAFE_DELETE(m_pBSphere);					// ���
		return;
	}

	m_pBSphere->SetMaterial(&m_Material);			// ���b�V���F�ݒ�
}

// ===���E�������蔻��R���X�g���N�^(�I�t�Z�b�g����)===
CollSphere::CollSphere(Collision3D* col, XMFLOAT3 off, float rad)
{
	// ---����������---
	Init();													// ������
	m_Offset = off;											// ���S����̃I�t�Z�b�g���Z�b�g
	m_Radius = rad;											// ���a���Z�b�g
	SetParent(col);											// �e���Z�b�g
	HRESULT hr = m_pBSphere->Init(32, 16, m_Radius);		// �f�o�b�O�\��������

	// ---��O����---
	if (FAILED(hr))
	{
		m_pBSphere->Fin();							// �f�o�b�O�\���I������
		SAFE_DELETE(m_pBSphere);					// ���
		return;
	}

	m_pBSphere->SetMaterial(&m_Material);			// ���b�V���F�ݒ�
}

// ===�f�X�g���N�^===
CollSphere::~CollSphere()
{
	// ---�I������---
	m_nCnt--;										// ���E�������蔻�葍������
	if (m_nCnt <= 0)
	{
		m_pBSphere->Fin();							// �f�o�b�O�\���I��
		SAFE_DELETE(m_pBSphere);					// ���
	}
}

// ===����������===
void CollSphere::Init()
{
	// ---������---
	m_Radius = 1.0f;								// �������a
	m_coltype = COL_SPHERE;							// �����蔻��̌^�ݒ�
	if (m_nCnt == 0)
	{
		m_pBSphere = new CSphere();					// �f�o�b�O�\������
	}
	m_nCnt++;										// ���E�������蔻�葍������
}

// ===�q�b�g�m�F===
bool CollSphere::CheckHit(CollisionBase* col)
{
	// ---���O����---
	if (col->GetFollowCollision() == GetFollowCollision())
		return false;
	if (!col->GetEnable())
		return false;

	// ---�l�̎擾---
	XMFLOAT3 pos = col->GetCenter();											// ������W
	XMFLOAT3 vec = { pos.x - m_Pos.x, pos.y - m_Pos.y, pos.z - m_Pos.z };		// ������W�ւ̃x�N�g��

	// ---�����蔻��---
	switch (col->GetColType())
	{
	case COL_SPHERE: {														// ---���E�����m�̓����蔻��
		// ---�l�̎擾---
		float rad = ((CollSphere*)col)->GetRadius();			// ���蔼�a�擾
		float length;											// ����Ƃ̋���
		Length(&length, m_Pos, pos);							// �������Z�o

		// ---�Փˎ�---
		if (length <= (m_Radius + rad))
		{
			// ---�߂��ʂ̌v�Z---
			if (!col->GetFollowCollision()->GetCollider())
			{
				XMFLOAT3 normal(m_Pos.x - pos.x, m_Pos.y - pos.y, m_Pos.z - pos.z);		// �߂������͒��S���W�Ԃ̃x�N�g���Ƃ���
				Normalize(&normal);														// ���K��
				length = m_Radius + rad - length;										// ���݂��̔��a�𑫂����������璆�S���W�Ԃ̋����������Ă߂荞�ݗʂ��Z�o

				// ---�����𔽉f---
				normal.x *= length;
				normal.y *= length;
				normal.z *= length;

				// ---�߂��ʂ̐ݒ�---
				m_pParent->CalcMovement(&normal);	// ���f

				m_bHit = true;						// �q�b�g
			}
			return true;
		}
		// ---��Փˎ�---
		else
		{
			m_bHit = false;
		}
		break; 
	}
	case COL_OBB: {															// ---OBB�Ƃ̔���
		// ---�l�̎擾---
		float length;
		((CollOBB*)col)->CalcLenOBBtoPoint(&length, m_Pos);					// ���S���W�Ƃ̍ŒZ���������߂�

		// ---�Փˎ�---
		if (length <= m_Radius)
		{
			m_bHit = true;

			//---�߂��ʂ̌v�Z---
			if (!col->GetFollowCollision()->GetCollider())
			{
				XMFLOAT3 work;
				CollOBB::CalcOBBtoPoint(&work, m_Pos, (CollOBB*)col);						// �Փ˂����_�����߂�
				XMFLOAT3 normal(m_Pos.x - work.x, m_Pos.y - work.y, m_Pos.z - work.z);		// �Փ˂����_�̍��W����߂荞�ݗʂ�߂����������߂�
				Normalize(&normal);						// ���K��

				Length(&length, work, m_Pos);			// �Փ˂����_���狫�E���̒��S���W�̋��������߂�
				length = m_Radius - length;				// ���߂���������߂荞�ݗʂ����߂�

				// ---�����𔽉f---
				normal.x *= length;
				normal.y *= length;
				normal.z *= length;

				// ---�߂��ʂ̐ݒ�---
				m_pParent->CalcMovement(&normal);		// �߂荞�ݗʂ𔽉f
				m_pParent->SetHitPos(&work);			// �ŋߐړ_���Z�b�g
			}

			return true;
		}

		// ---��Փˎ�---
		else
			m_bHit = false;
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
void CollSphere::Draw()
{
	// ---���O����---
	if (!GetVisible())
		return;

	// ---�`�揈��---
	m_pBSphere->SetMaterial(&m_Material);				// �F��ݒ�
	m_pBSphere->SetWorld(&GetWorld());					// ���[���h�ϊ��s��Z�b�g
	m_pBSphere->Draw();									// �`��
}

// ===���E�����a�擾===
float CollSphere::GetRadius()
{
	return m_Radius;
}