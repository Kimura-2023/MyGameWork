#pragma once

// ===�����蔻��(OBB)�N���X��`===
// �����蔻��(OBB)�N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "main.h"
#include "CollisionBase.h"
#include "Box.h"

// ===�N���X��`===
class CollOBB : public CollisionBase
{
	// ---�����o�֐�---
public:
	CollOBB(Collision3D*, XMFLOAT3);					// �R���X�g���N�^(�T�C�Y�w��
	CollOBB(Collision3D*, XMFLOAT3, XMFLOAT3);			// �R���X�g���N�^(�I�t�Z�b�g�A�T�C�Y�w��
	~CollOBB();											// �f�X�g���N�^

	void Draw();										// �f�o�b�O�\��
	bool CheckHit(CollisionBase*);						// �����蔻��
	XMFLOAT3 GetSize();									// �T�C�Y�擾

	void CalcLenOBBtoPoint(float* ans, const XMFLOAT3& point);								// OBB�Ƃ̍ŒZ���������߂�
	static void CalcOBBtoPoint(XMFLOAT3* ans, const XMFLOAT3& point, CollOBB* obb);			// OBB�̍ŋߐړ_�����߂�
private:
	void Init();										// ������

	// ---�����o�ϐ�---
private:
	static CBox* m_pBox;								// �f�o�b�O�\���p���b�V��
	static int m_nCnt;									// OBB����
};
