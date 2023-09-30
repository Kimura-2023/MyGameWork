#pragma once
// ===体力UIクラス定義===
// 体力UIクラスを定義
// Author: Kimura
// ======================

// ===インクルード===
#include "main.h"
#include "UIBase.h"

// ===クラス定義===
class PlayerLife : public UIBase
{
	// ---メンバ関数---
public:
	PlayerLife();		// コンストラクタ
	~PlayerLife();		// デストラクタ

	void Update() override;		// 更新処理
	void Draw() override;		// 描画処理

	// ---メンバ変数---
private:
	int m_nLife;				// 体力
};