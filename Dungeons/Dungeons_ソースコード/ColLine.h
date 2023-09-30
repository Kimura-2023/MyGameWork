#pragma once

// ===�����蔻��(����)�N���X��`===
// �����蔻��(����)�N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "main.h"
#include "CollisionBase.h"
#include "Line.h"

// ===�N���X��`===
class CollLine : public CollisionBase
{
	// ---�����o�֐�---
public:
	CollLine(Collision3D*, XMFLOAT3);				// �R���X�g���N�^(�x�N�g���w��
	CollLine(Collision3D*, XMFLOAT3, XMFLOAT3);		// �R���X�g���N�^(�I�t�Z�b�g�A�x�N�g���w��
	~CollLine();									// �f�X�g���N�^

	void Draw();							// �f�o�b�O�\��
	bool CheckHit(CollisionBase*);			// �q�b�g�m�F

	XMFLOAT3 GetEnd();		// �I�_���W�擾
	XMFLOAT3 GetVec();		// �n�_����I�_�ւ̃x�N�g���擾
private:
	void Init();			// ����������

	// ---�����o�ϐ�---
private:
	XMFLOAT3 m_End;			// �I�_���W
	XMFLOAT3 m_vec;			// �n�_����I�_�ւ̌���
	static Line* m_Line;	// ����
	static int m_nCnt;		// ����
};