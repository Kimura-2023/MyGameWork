#pragma once
// ===敵移動クラス定義===
// 敵移動クラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "EnemyStrategy.h"

// ===クラス定義===
class EnemyMove : public EnemyStrategy
{
	// ---メンバ関数---
public:
	virtual void Init(EnemyBase* enemy);		// 初期化処理
	virtual void Update(EnemyBase* enemy);		// 更新処理
};