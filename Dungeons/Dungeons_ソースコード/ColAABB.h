#pragma once

// ===�����蔻��(AABB)�N���X��`===
// �����蔻��(AABB)�N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "main.h"
#include "CollisionBase.h"
#include "Box.h"

// ===�N���X��`===
class CollAABB : public CollisionBase
{
	// ---�����o�֐�---
public:
	CollAABB(Collision3D*, XMFLOAT3);				// �R���X�g���N�^(�T�C�Y�w��
	CollAABB(Collision3D*, XMFLOAT3, XMFLOAT3);		// �R���X�g���N�^(�I�t�Z�b�g�A�T�C�Y�w��
	~CollAABB();									// �f�X�g���N�^

	void Draw();									// �f�o�b�O�\��
	bool CheckHit(CollisionBase*);					// �����蔻��
	XMFLOAT3 GetSize();								// �T�C�Y�擾
private:
	void Init();									// ������

	// ---�����o�ϐ�---
private:
	XMFLOAT3 m_Size;			// AABB�T�C�Y
	static CBox* m_pBox;		// �f�o�b�O�\���p���b�V��
	static int m_nCnt;			// AABB����
};