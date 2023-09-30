
// ===�G�ړ��N���X EnemyMoving.h
// --Author kimura

#include "EnemyMoving.h"

// �G�ړ��X�V
void EnemyMoving::Update(Enemy* enemy)
{
	XMFLOAT3 pos = enemy->GetTransform().Pos;

	if (enemy->GetTransform().Pos.x <= -48.0f)
	{
		enemy->SetVel(XMFLOAT3(((rand() % 10) - 5) / 20.0f, 0, ((rand() % 10) - 5) / 20.0f));
	}
	if (enemy->GetTransform().Pos.x >= 48.0f)
	{
		enemy->SetVel(XMFLOAT3(((rand() % 10) - 5) / 20.0f, 0, ((rand() % 10) - 5) / 20.0f));
	}
	if (enemy->GetTransform().Pos.z <= -48.0f)
	{
		enemy->SetVel(XMFLOAT3(((rand() % 10) - 5) / 20.0f, 0, ((rand() % 10) - 5) / 20.0f));
	}
	if (enemy->GetTransform().Pos.z >= 48.0f)
	{
		enemy->SetVel(XMFLOAT3(((rand() % 10) - 5) / 20.0f, 0, ((rand() % 10) - 5) / 20.0f));
	}

}

bool EnemyMoving::IsUpdate(Enemy* kakasi)
{
	return true;
}