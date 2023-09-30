#pragma once
// ===�G��ԃx�[�X�N���X��`===
// �G��ԃx�[�X�N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "EnemyBase.h"

// �N���X��`�����邱�Ƃ�錾
class EnemyBase;

// ===�N���X��`===
class EnemyStrategy
{
	// ---�����o�֐�---
public:
	virtual void Init(EnemyBase* enemy) = 0;		// ����������
	virtual void Update(EnemyBase* enemy) = 0;		// �X�V����
};