// ===遠距離敵クラス実装===
// 遠距離敵クラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "EnemyMove.h"
#include "Player.h"
#include "SceneManager.h"
#include "Calc.h"

// ===初期化処理===
void EnemyMove::Init(EnemyBase* enemy)
{
}

// ===更新処理===
void EnemyMove::Update(EnemyBase* enemy)
{
	Object* player = SCENE->GetCurrentScene()->FindGameObject("Player");		// プレイヤー取得

	// ---例外処理---
	if (!player)
	{
		enemy->SetVel(XMFLOAT3(0.0f, 0.0f, 0.0f));
		return;
	}

	// ---距離によって処理---
	float rad = enemy->GetView();		// 視認距離取得
	float length;						// 距離
	Length(&length, player->GetPos(), enemy->GetPos());		// プレイヤーとの距離を算出

	if (length <= rad && length >= 2.0f)		// 視認距離内の場合
	{
		XMFLOAT3 vec;		// 向き
		vec.x = player->GetPos().x - enemy->GetPos().x;		// プレイヤーへの向きを求める
		vec.z = player->GetPos().z - enemy->GetPos().z;
		vec.y = 0.0f;

		Normalize(&vec);	// 正規化
		vec.x *= 0.05f;		// x方向補正
		vec.z *= 0.05f;		// z方向補正

		enemy->SetVel(vec);		// 速度をセット
		enemy->SetFront(vec);	// 正面方向セット
		enemy->SetNear(true);	// 近距離判定セット
	}
	else					// 視認距離外の場合
	{
		enemy->SetVel(XMFLOAT3(0.0f, 0.0f, 0.0f));
		enemy->SetNear(false);
	}

	if(length <= rad && enemy->GetInterval() <= 60.0f && enemy->GetInterval() > 0.0f)
		enemy->SetNear(true);					// 近距離判定セット
	else if (length <= 3.0f && enemy->GetInterval() <= 0.0f)		// 一定距離内かつ攻撃間隔が0の場合
		enemy->SetStrategy(ATTACK_ENEMY);		// 攻撃状態へ遷移
}