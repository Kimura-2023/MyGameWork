#pragma once
// ===エンターキーUIクラス定義===
// UIクラスを定義
// Author: Kimura
// ===============================

// ===インクルード===
#include "Object2DBase.h"

// ===クラス定義===
class EnterButton : public Object2DBase
{
	// ---メンバ関数---
public:
	EnterButton();		// コンストラクタ
	~EnterButton();		// デストラクタ

	void Update();		// 更新処理
	void Draw();		// 描画処理
	bool IsUpdate();	// 更新有無

	// ---メンバ変数---
private:
};
