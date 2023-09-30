#pragma once
// ===プレイヤー状態ベースクラス定義===
// プレイヤー状態ベースクラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "Player.h"

// クラス定義があることを宣言
class Player;

// ===クラス定義===
class PlayerStrategy
{
	// ---メンバ関数---
public:
	virtual void Init(Player* player) = 0;			// 初期化処理
	virtual void Update(Player* player) = 0;		// 更新処理
	virtual bool IsUpdate(Player* player) = 0;		// 更新状態
};