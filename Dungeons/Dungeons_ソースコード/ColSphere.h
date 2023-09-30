#pragma once

// ===�����蔻��(���E��)�N���X��`===
// �����蔻��(���E��)�N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "main.h"
#include "CollisionBase.h"
#include "Sphere.h"

// ===�N���X��`===
class CollSphere : public CollisionBase
{
	// ---�����o�֐�---
public:
	CollSphere(Collision3D*,float);				// �R���X�g���N�^(���a�w��
	CollSphere(Collision3D*, XMFLOAT3, float);	// �R���X�g���N�^(�I�t�Z�b�g�A���a�w��
	~CollSphere();								// �f�X�g���N�^

	void Draw();								// �f�o�b�O�\��
	bool CheckHit(CollisionBase*);				// �����蔻��
	float GetRadius();							// ���E�����a�擾
private:
	void Init();								// ������

	// ---�����o�ϐ�---
private:
	float m_Radius;					// ���E�����a
	static CSphere* m_pBSphere;		// �f�o�b�O�\���p���b�V��
	static int m_nCnt;				// ���E������
};