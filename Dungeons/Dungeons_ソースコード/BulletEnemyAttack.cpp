// ===遠距離敵攻撃クラス実装===
// 遠距離敵攻撃クラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "BulletEnemyAttack.h"
#include "Player.h"
#include "SceneManager.h"
#include "Calc.h"
#include "EffectManager.h"
#include "EnemyBullet.h"

// ===初期化処理===
void BulletEnemyAttack::Init(EnemyBase* enemy)
{
	XMFLOAT3 vec;			// 向き
	Object* player = SCENE->GetCurrentScene()->FindGameObject("Player");		// プレイヤーを取得

	// ---例外処理---
	if (!player)
	{
		enemy->SetVel(XMFLOAT3(0.0f, 0.0f, 0.0f));
		enemy->SetStrategy(STAY_ENEMY);					// 待機状態に遷移
		return;
	}

	// ---弾状態初期化---
	vec.x = player->GetPos().x - enemy->GetPos().x;		// 進行方向を求める
	vec.z = player->GetPos().z - enemy->GetPos().z;
	vec.y = 0.0f;
	Normalize(&vec);									// 正規化
	enemy->SetVel(XMFLOAT3(0.0f, 0.0f, 0.0f));			// 速度をセット
	enemy->SetFront(vec);								// 正面方向をセット
	m_nBulletCnt = 3;									// 残弾数を初期化
}

// ===更新処理===
void BulletEnemyAttack::Update(EnemyBase* enemy)
{
	// ---攻撃間隔で判定---
	if (enemy->GetInterval() <= 60.0f)
		enemy->SetNear(true);					// 攻撃直前に輪郭線の色を変える
	else
		enemy->SetNear(false);					// 通常に戻す

	// ---残弾数で判定---
	if (m_nBulletCnt <= 0)
	{
		enemy->SetStrategy(STAY_ENEMY);			// 待機状態に遷移
		enemy->SetInterval(30.0f);				// インターバルをセット
	}
	else if(enemy->GetInterval() <= 0.0f)
	{
		enemy->SetInterval(2.0f);				// インターバルをセット
		((EnemyBullet*)enemy)->FireBullet();	// 弾発射
		m_nBulletCnt--;							// 残弾数を減らす
	}
}