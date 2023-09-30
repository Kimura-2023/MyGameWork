#pragma once
// ===敵状態ベースクラス定義===
// 敵状態ベースクラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "EnemyBase.h"

// クラス定義があることを宣言
class EnemyBase;

// ===クラス定義===
class EnemyStrategy
{
	// ---メンバ関数---
public:
	virtual void Init(EnemyBase* enemy) = 0;		// 初期化処理
	virtual void Update(EnemyBase* enemy) = 0;		// 更新処理
};