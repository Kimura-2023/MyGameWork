// ===敵待機クラス実装===
// 敵待機状態クラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "EnemyStay.h"
#include "Player.h"
#include "SceneManager.h"
#include "Calc.h"
#include "EffectManager.h"

// ===初期化処理===
void EnemyStay::Init(EnemyBase* enemy)
{
}

// ===更新処理===
void EnemyStay::Update(EnemyBase* enemy)
{
	enemy->SetVel(XMFLOAT3(0.0f, 0.0f, 0.0f));		// 速度をセット
	enemy->SetNear(false);							// 近距離判定セット
}