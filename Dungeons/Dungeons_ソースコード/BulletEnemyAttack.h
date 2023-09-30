#pragma once
// ===遠距離敵攻撃クラス定義===
// 遠距離敵攻撃クラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "EnemyAttack.h"

// ===クラス定義===
class BulletEnemyAttack : public EnemyAttack
{
	// ---メンバ関数---
public:
	void Init(EnemyBase* enemy);		// 初期化処理
	void Update(EnemyBase* enemy);		// 更新処理

	// ---メンバ変数---
private:
	int m_nBulletCnt;		// 弾の数
};