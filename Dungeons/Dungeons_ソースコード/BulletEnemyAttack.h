#pragma once
// ===�������G�U���N���X��`===
// �������G�U���N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "EnemyAttack.h"

// ===�N���X��`===
class BulletEnemyAttack : public EnemyAttack
{
	// ---�����o�֐�---
public:
	void Init(EnemyBase* enemy);		// ����������
	void Update(EnemyBase* enemy);		// �X�V����

	// ---�����o�ϐ�---
private:
	int m_nBulletCnt;		// �e�̐�
};