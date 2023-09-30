// ===�����蔻��(OBB)�N���X����===
// �����蔻��(OBB)�N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "ColOBB.h"
#include "Light.h"
#include "ModelManager.h"
#include "DebugProc.h"
#include "ColSphere.h"

// ===�ÓI�����o===
CBox* CollOBB::m_pBox = nullptr;			// �f�o�b�O�\���p���b�V��
int CollOBB::m_nCnt = 0;					// ����

// ===���E�������蔻��R���X�g���N�^(�I�t�Z�b�g�Ȃ�)===
CollOBB::CollOBB(Collision3D* col, XMFLOAT3 size)
{
	// ---������---
	Init();														// ������
	m_Scale = size;												// �T�C�Y�Z�b�g
	SetParent(col);												// �e���Z�b�g
	HRESULT hr = m_pBox->Init(XMFLOAT3(1.0f,1.0f,1.0f));		// �f�o�b�O�\��������

	// ---��O����---
	if (FAILED(hr))
	{
		m_pBox->Fin();						// �f�o�b�O�\���I������
		SAFE_DELETE(m_pBox);				// �f�o�b�O�\�����
		return;
	}

	m_pBox->SetMaterial(&m_Material);		// �f�o�b�O�\���F�ݒ�
}

// ===���E�������蔻��R���X�g���N�^(�I�t�Z�b�g����)===
CollOBB::CollOBB(Collision3D* col, XMFLOAT3 off, XMFLOAT3 size)
{
	Init();														// ������
	m_Offset = off;												// �I�t�Z�b�g�ݒ�
	m_Scale = { size.x,size.y,size.z};							// �T�C�Y�Z�b�g
	SetParent(col);												// �e���Z�b�g
	HRESULT hr = m_pBox->Init(XMFLOAT3(1.0f,1.0f,1.0f));		// �f�o�b�O�\��������

	// ---��O����---
	if (FAILED(hr))
	{
		m_pBox->Fin();			// �f�o�b�O�\���I������
		SAFE_DELETE(m_pBox);	// �f�o�b�O�\�����
		return;
	}

	m_pBox->SetMaterial(&m_Material);		// �f�o�b�O�\���F�ݒ�
}

// ===�f�X�g���N�^===
CollOBB::~CollOBB()
{
	m_nCnt--;						// OBB�����蔻�葍������
	if (m_nCnt <= 0)
	{
		m_pBox->Fin();				// �f�o�b�O�\���I��
		SAFE_DELETE(m_pBox);		// ���
	}
}

// ===�������֐�===
void CollOBB::Init()
{
	// ---������---
	m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);		// �g�k������
	m_coltype = COL_OBB;						// �����蔻��̌^���Z�b�g

	if (m_nCnt == 0)
	{
		m_pBox = new CBox();						// �f�o�b�O�\������
	}
	m_nCnt++;									// OBB�����蔻�葍������
}

// ===�q�b�g�m�F===
bool CollOBB::CheckHit(CollisionBase* col)
{
	// ---���O����---
	if (col->GetFollowCollision() == GetFollowCollision())		// ���������蔻����Q�Ƃ��Ă���ꍇ
		return false;			// �������Ă��Ȃ��Ɣ���
	if (!col->GetEnable())										// �X�V�`��Ȃ��̏ꍇ
		return false;			// �������Ă��Ȃ��Ɣ���

	m_bHit = false;									// ���菉����
	XMFLOAT3 pos = col->GetCenter();				// ���蓖���蔻�蒆�S���W

	// ---�����蔻��---
	switch (col->GetColType())						// �����蔻��̌^�ŕ���
	{
	case COL_AABB:
	case COL_OBB: {									// OBB���m
		XMFLOAT3 size = ((CollOBB*)col)->GetSize();					// ����T�C�Y

		// ---���[���h�s��擾---
		XMFLOAT4X4 mW1 = GetWorld();			// ���[���h�s��擾
		XMFLOAT4X4 mW2 = col->GetWorld();		// ���葊��̃��[���h�s��擾

		// ---���S���W�Ԃ̃x�N�g�������߂�---
		XMVECTOR vD = XMVectorZero();			// ������
		vD = XMVectorSet(pos.x - m_Pos.x, pos.y - m_Pos.y, pos.z - m_Pos.z, 0.0f);		// ���S���W�ԃx�N�g��

		// ---���f�����W�������߂�---
		XMVECTOR vN[6];
		vN[0] = XMVectorSet(mW1._11, mW1._12, mW1._13, 0.0f);
		vN[1] = XMVectorSet(mW1._21, mW1._22, mW1._23, 0.0f);
		vN[2] = XMVectorSet(mW1._31, mW1._32, mW1._33, 0.0f);
		vN[3] = XMVectorSet(mW2._11, mW2._12, mW2._13, 0.0f);
		vN[4] = XMVectorSet(mW2._21, mW2._22, mW2._23, 0.0f);
		vN[5] = XMVectorSet(mW2._31, mW2._32, mW2._33, 0.0f);

		// ---OBB�̑傫��(����)���|�����x�N�g�������߂�---
		XMFLOAT3 vBB1 = m_Scale;
		XMFLOAT3 vBB2 = size;
		XMVECTOR vL[6];
		vL[0] = XMVectorSet(mW1._11 * vBB1.x, mW1._12 * vBB1.x, mW1._13 * vBB1.x, 0.0f);
		vL[1] = XMVectorSet(mW1._21 * vBB1.y, mW1._22 * vBB1.y, mW1._23 * vBB1.y, 0.0f);
		vL[2] = XMVectorSet(mW1._31 * vBB1.z, mW1._32 * vBB1.z, mW1._33 * vBB1.z, 0.0f);
		vL[3] = XMVectorSet(mW2._11 * vBB2.x, mW2._12 * vBB2.x, mW2._13 * vBB2.x, 0.0f);
		vL[4] = XMVectorSet(mW2._21 * vBB2.y, mW2._22 * vBB2.y, mW2._23 * vBB2.y, 0.0f);
		vL[5] = XMVectorSet(mW2._31 * vBB2.z, mW2._32 * vBB2.z, mW2._33 * vBB2.z, 0.0f);

		// ---��������₪�ʂ̖@���x�N�g��(���f�����W��)---
		float fL, f, fD;
		for (int i = 0; i < 6; ++i) {
			XMVECTOR& vS = vN[i];			//���������

			// ---OBB�̉e(�̔���)�̍��v---
			fL = 0.0f;
			for (int j = 0; j < 6; ++j) {
				XMStoreFloat(&f, XMVector3Dot(vS, vL[j]));			// �ˉe
				fL += fabsf(f);										// ���v����
			}

			// ---���S���W�Ԃ̋����Ɣ�r---
			XMStoreFloat(&fD, XMVector3Dot(vS, vD));
			fD = fabsf(fD);
			if (fL < fD) return false;			// �������Ă��Ȃ�
		}

		// ---��������₪���ꂼ��̕ӂɐ����ȃx�N�g��---
		XMVECTOR vS = XMVectorZero();
		for (int i = 0; i < 3; ++i) {
			for (int j = 3; j < 6; ++j) {

				// ---�������������߂�---
				vS = XMVector3Normalize(XMVector3Cross(vN[i], vN[j]));

				// ---OBB�̉e(�̔���)�̍��v---
				fL = 0.0f;
				for (int k = 0; k < 6; ++k) {
					XMStoreFloat(&f, XMVector3Dot(vS, vL[k]));		// �ˉe
					fL += fabsf(f);									// ���v����
				}

				// ---���S���W�Ԃ̋����Ɣ�r---
				XMStoreFloat(&fD, XMVector3Dot(vS, vD));
				fD = fabsf(fD);
				if (fL < fD) return false;		// �������Ă��Ȃ�
			}
		}
		m_bHit = true;

		// --- �������Ă����ꍇ�̖߂��ʌv�Z---
		if (!col->GetFollowCollision()->GetCollider())
		{
			XMFLOAT3 move = { 0.0f,0.0f,0.0f };
			XMFLOAT3 normal = { 0.0f,0.0f,0.0f };	// �߂荞�݂�߂�����
			XMVECTOR work = XMVectorZero();			// ��Ɨp
			float length = 0.0f;					// ����
			float r = 0.0f;
			float s = 0.0f;

			// ---�ŋߐړ_�����߂�---
			CalcOBBtoPoint(&move, m_Pos, ((CollOBB*)col));

			// ---�߂荞��OBB��̓_����߂����������߂�(�ŋߓ_���璆�S���W�ւ̃x�N�g���𕽖ʂ̖@���Ƃ���---
			normal = XMFLOAT3(m_Pos.x - move.x, m_Pos.y - move.y, m_Pos.z - move.z);		// �ŋߐړ_���璆�S���W�ւ̃x�N�g���𕽖ʂ̖@���Ƃ���
			Normalize(&normal);							// ���K��
			work = XMLoadFloat3(&normal);

			for (int i = 0; i < 3; i++)
			{
				XMStoreFloat(&length, XMVector3Dot(vL[i], work));
				r += fabs(length);
			}

			// ---�ŋߐړ_���q�b�g�������W�Ƃ��Ēl���Z�b�g---
			m_pParent->SetHitPos(&move);

			// ---����OBB�̍ŋߓ_�Ɠ�������OBB�Ԃ̋��������߂�---
			move = XMFLOAT3(m_Pos.x - move.x, m_Pos.y - move.y, m_Pos.z - move.z);
			Dot(&s, move, normal);

			// ---�߂��������Z�o---
			if (s > 0)
				length = r - fabs(s);
			else
				length = r + fabs(s);

			// ---�@�������ɖ߂������̕�����Z---
			move.x = normal.x * length;
			move.y = normal.y * length;
			move.z = normal.z * length;
			m_pParent->CalcMovement(&move);			// �ړ��ʂ�ݒ�
		}
		return true;			// �������Ă���
		break;
	}
	case COL_SPHERE:

		float len;
		CalcLenOBBtoPoint(&len,pos);
		if (len <= (((CollSphere*)col)->GetRadius()))
		{
			// ---�߂��ʂ̌v�Z---
			if (!col->GetFollowCollision()->GetCollider())
			{
				XMFLOAT3 work;
				CalcOBBtoPoint(&work, pos, this);									// �ŋߐړ_�����߂�
				XMFLOAT3 normal(work.x - pos.x, work.y - pos.y, work.z - pos.z);	// �ŋߐړ_����I�u�W�F�N�g�ւ̃x�N�g�������߂�
				Normalize(&normal);			// ���K��
				float length;				// �����Z�o�p
				Length(&length, work, pos);	// �ŋߐړ_����̋��������߂�
				length = ((CollSphere*)col)->GetRadius() - length;		// ���E���̔��a���狗�������Z���邱�Ƃł߂荞�񂾗ʂ��Z�o

				normal.x *= length;			// --�x�N�g�������ɒ�����������
				normal.y *= length;
				normal.z *= length;
				// ---���W�␳---
				m_pParent->CalcMovement(&normal);

				// ---�ŋߐړ_���q�b�g�������W�Ƃ��Ēl���Z�b�g
				m_pParent->SetHitPos(&work);
			}

			m_bHit = true;
			return true;
		}
	case COL_NONE:
		break;
	default:
		break;
	}
	return false;
}

// ===�f�o�b�O�\��===
void CollOBB::Draw()
{
	// ---�`�攻��---
	if (!GetVisible())
		return;

	// ---�`��ݒ�---
	m_pBox->SetMaterial(&m_Material);

	// ---�ϊ��s��v�Z(�v���P)---
	XMMATRIX mtxworld, mtx;
	XMFLOAT4X4 mW;
	mtxworld = XMMatrixIdentity();								// ������
	mtx = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);		// �X�P�[���s��
	mtxworld = XMMatrixMultiply(mtxworld, mtx);					// ���f������

	mtx = XMLoadFloat4x4(&GetWorld());							// 
	mtxworld = XMMatrixMultiply(mtxworld, mtx);					// �X�P�[���𔽉f

	XMStoreFloat4x4(&mW, mtxworld);								// XMFLOAT4X4�^�ɕϊ�
	m_pBox->SetWorld(&mW);										// ���b�V���ɔ��f

	// ---�`��---
	m_pBox->Draw();
}

// ===OBB�T�C�Y�擾===
XMFLOAT3 CollOBB::GetSize()
{
	return m_Scale;
}

// ===�ŒZ���������߂�===
// ����2:��_
void CollOBB::CalcLenOBBtoPoint(float* ans,const XMFLOAT3& point)
{
	XMVECTOR vD;
	vD = XMVectorSet(point.x - m_Pos.x, point.y - m_Pos.y, point.z - m_Pos.z, 0.0f);	// ���S���W�ԃx�N�g��
	XMFLOAT4X4 mW1 = GetWorld();														// ���[���h�s��擾
	XMVECTOR mRot[3];

	// ---�T�C�Y�擾---
	XMFLOAT3 Scale = m_pParent->GetSize();
	mRot[0] = XMVectorSet(mW1._11, mW1._12, mW1._13, 0.0f);
	mRot[1] = XMVectorSet(mW1._21, mW1._22, mW1._23, 0.0f);
	mRot[2] = XMVectorSet(mW1._31, mW1._32, mW1._33, 0.0f);

	// ---�l�␳(�v�C��)---
	mRot[0] /= Scale.x;
	mRot[1] /= Scale.y;
	mRot[2] /= Scale.z;

	// ---�����Z�o---
	float distance = 0.0f;																// ����
	float size[3] = { m_Scale.x * Scale.x,m_Scale.y * Scale.y,m_Scale.z * Scale.z };
	XMVECTOR calc = XMVectorZero();

	for (int i = 0; i < 3; i++)
	{
		if (size[i] <= 0)																// -�̒l�͉��Z���Ȃ�
			continue;
		XMStoreFloat(&distance, XMVector3Dot(vD, mRot[i]) / size[i]);					// �ˉe�v�Z
		distance = fabs(distance);														// ������m�肽���̂Ő�Βl�Ƃ���
		if(distance > 1)
			calc += (1 - distance) * size[i] * mRot[i];
	}
	XMStoreFloat(ans, XMVector3Length(calc));											// �v�Z����
}

// ===OBB��̍ŋߐړ_���W�����߂�===
// ����3:�v�Z����������OBB�N���X�̃|�C���^
void CollOBB::CalcOBBtoPoint(XMFLOAT3* ans, const XMFLOAT3& point,CollOBB* obb)
{
	XMVECTOR vD;
	XMFLOAT3 pos = obb->GetCenter();
	XMFLOAT3 vsize = obb->GetSize();
	XMFLOAT3 Scale = obb->GetFollowCollision()->GetSize();
	vD = XMVectorSet(point.x - pos.x, point.y - pos.y, point.z - pos.z, 0.0f);		// ���S���W�ԃx�N�g��
	XMFLOAT4X4 mW1 = obb->GetWorld();												// ���[���h�s��擾
	XMVECTOR mRot[3];

	// ---�T�C�Y�擾---
	mRot[0] = XMVectorSet(mW1._11, mW1._12, mW1._13, 0.0f);
	mRot[1] = XMVectorSet(mW1._21, mW1._22, mW1._23, 0.0f);
	mRot[2] = XMVectorSet(mW1._31, mW1._32, mW1._33, 0.0f);

	// ---�l�␳(�v�C��)---
	mRot[0] /= Scale.x;
	mRot[1] /= Scale.y;
	mRot[2] /= Scale.z;

	// ---�����Z�o---
	float distance;
	float size[3] = { vsize.x * Scale.x, vsize.y * Scale.y, vsize.z * Scale.z};
	XMVECTOR calc = XMVectorZero();

	for (int i = 0; i < 3; i++)
	{
		XMStoreFloat(&distance, XMVector3Dot(vD, mRot[i]));			// �ˉe�v�Z

		// ---���̎��̒����ɂ���Ĉʒu�����߂�---
		if (distance > size[i])
		{
			distance = size[i];
		}
		if (distance < -size[i])
		{
			distance = -size[i];
		}
		calc += distance * mRot[i];
	}
	XMStoreFloat3(ans, calc);

	// ---���߂���ŋߓ_�̓��[�J���Ȃ̂ŁA���[���h���W�ɂȂ�悤�ɒ��S���W����I�t�Z�b�g����---
	ans->x = ans->x + pos.x;
	ans->y = ans->y + pos.y;
	ans->z = ans->z + pos.z;

}