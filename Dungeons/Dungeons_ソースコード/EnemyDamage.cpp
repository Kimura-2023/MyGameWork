// ===�v���C���[�ړ��N���X EnemyDamage.h
// --Author kimura

#include "EnemyDamage.h"

// �v���C���[�_���[�W�X�V
void EnemyDamage::Update(Enemy* enemy)
{
	// �_���[�W����
	if (enemy->GetCol())
	{
		enemy->SetCol(false);
		enemy->SetMuteki(30);		// 20�t���[�����G
	}

	if (enemy->GetMuteki() > 0)
	{
		enemy->SetColor(XMFLOAT4(1.0f, 0.0f, 0.0f, 0.4f));
	}

	if (enemy->GetMuteki() <= 0)
	{
		enemy->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		enemy->SetCol(true);
		enemy->SetState(MOVING_ENEMY);
	}

}

bool EnemyDamage::IsUpdate(Enemy* enemy)
{
	return true;
}