#pragma once
// ===プレイヤー移動クラス定義===
// プレイヤー移動クラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "PlayerStrategy.h"

// ===クラス宣言===
class PlayerMove : public PlayerStrategy
{
	// ---メンバ関数---
public:
	void Init(Player* player);					// 初期化処理
	void Update(Player* player);				// 更新処理
	virtual bool IsUpdate(Player* player);		// 更新状態

	// ---メンバ変数---
private:
	bool m_bController;			// コントローラー入力フラグ
};