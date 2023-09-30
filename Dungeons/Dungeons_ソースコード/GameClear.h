#pragma once
// ===ゲームクリアUIクラス定義===
// ゲームクリアUIクラスを定義
// Author: Kimura
// ===============================

// ===インクルード===
#include "Object2DBase.h"

// ===クラス定義===
class GameClear : public Object2DBase
{
	// ---メンバ関数---
public:
	GameClear();		// コンストラクタ
	~GameClear();		// デストラクタ

	void Update();		// 更新処理
	void Draw();		// 描画処理
	bool IsUpdate();	// 更新有無
private:
};
