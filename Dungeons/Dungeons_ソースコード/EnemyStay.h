#pragma once
// ===敵待機クラス定義===
// 敵待機状態クラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "EnemyStrategy.h"

// ===クラス定義===
class EnemyStay : public EnemyStrategy
{
	// ---メンバ関数
public:
	virtual void Init(EnemyBase* enemy);		// 初期化処理
	virtual void Update(EnemyBase* enemy);		// 更新処理

	// ---メンバ変数
private:
};