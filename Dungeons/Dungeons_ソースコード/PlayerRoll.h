#pragma once
// ===プレイヤー回避クラス定義===
// プレイヤー回避クラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "PlayerStrategy.h"

// ===クラス定義===
class PlayerRoll : public PlayerStrategy
{
	// ---メンバ関数---
public:
	void Init(Player* player);					// 初期化処理
	void Update(Player* player);				// 更新処理
	virtual bool IsUpdate(Player* player);		// 更新有無
};