#pragma once
// ===プレイヤー攻撃クラス定義===
// プレイヤー攻撃クラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "PlayerStrategy.h"

// ===クラス定義===
class PlayerAttack : public PlayerStrategy
{
	// ---メンバ関数---
public:
	void Init(Player* player);					// 初期化処理
	void Update(Player* player);				// 更新処理
	virtual bool IsUpdate(Player* player);		// 遷移できるかどうか
};