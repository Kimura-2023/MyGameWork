#pragma once
// ===�G�ҋ@�N���X��`===
// �G�ҋ@��ԃN���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "EnemyStrategy.h"

// ===�N���X��`===
class EnemyStay : public EnemyStrategy
{
	// ---�����o�֐�
public:
	virtual void Init(EnemyBase* enemy);		// ����������
	virtual void Update(EnemyBase* enemy);		// �X�V����

	// ---�����o�ϐ�
private:
};