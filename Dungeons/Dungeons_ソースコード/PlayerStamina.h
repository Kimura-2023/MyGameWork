#pragma once

// ===スタミナUIクラス定義===
// スタミナUIクラスを定義
// Author: Kimura

// ===インクルード===
#include "main.h"
#include "UIBase.h"

// ===クラス定義===
class PlayerStamina : public UIBase
{
	// ---メンバ関数---
public:
	PlayerStamina();		// コンストラクタ
	~PlayerStamina();		// デストラクタ

	void Update();			// 更新処理
	void Draw();			// 描画処理

	// ---メンバ変数---
private:
};