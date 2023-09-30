#pragma once
// ===�ʏ�G�N���X��`===
// �ʏ�G�N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "main.h"
#include "Object3DBase.h"
#include "EnemyStrategy.h"
#include "Collision3D.h"
#include "EnemyBase.h"

// ===�N���X�錾===
class EnemyNormal : public EnemyBase,Collision3D
{
	// ---�����o�֐�
public:
	EnemyNormal();			// �R���X�g���N�^
	~EnemyNormal();			// �f�X�g���N�^

	virtual void Init();	// ����������
	virtual void Update();	// �X�V����
	virtual void Draw();	// �`�揈��
	void OnCollision(Object*);		// �Փˏ���
};