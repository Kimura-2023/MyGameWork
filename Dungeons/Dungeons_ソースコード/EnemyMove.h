#pragma once
// ===�G�ړ��N���X��`===
// �G�ړ��N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "EnemyStrategy.h"

// ===�N���X��`===
class EnemyMove : public EnemyStrategy
{
	// ---�����o�֐�---
public:
	virtual void Init(EnemyBase* enemy);		// ����������
	virtual void Update(EnemyBase* enemy);		// �X�V����
};