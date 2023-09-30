#pragma once
// ===スコアUIクラス定義===
// スコアUIクラスを定義
// Author: Kimura
// ======================

// ===インクルード===
#include "main.h"
#include "UIBase.h"

// ===クラス定義===
class Score : public UIBase
{
	// ---メンバ関数---
public:
	Score();		// コンストラクタ
	~Score();		// デストラクタ

	void Update() override;		// 更新処理
	void Draw() override;		// 描画処理

	// ---メンバ変数---
private:
};