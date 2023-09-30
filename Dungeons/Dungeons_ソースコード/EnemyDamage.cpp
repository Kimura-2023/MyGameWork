// ===プレイヤー移動クラス EnemyDamage.h
// --Author kimura

#include "EnemyDamage.h"

// プレイヤーダメージ更新
void EnemyDamage::Update(Enemy* enemy)
{
	// ダメージ処理
	if (enemy->GetCol())
	{
		enemy->SetCol(false);
		enemy->SetMuteki(30);		// 20フレーム無敵
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