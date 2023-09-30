#pragma once
// ===ゲームオーバーUIクラス定義===
// ゲームオーバーUIクラスを定義
// Author: Kimura
// ===============================

// ===インクルード===
#include "Object2DBase.h"

// ===コンストラクタ===
class GameOver : public Object2DBase
{
public:
	GameOver();			// コンストラクタ
	~GameOver();		// デストラクタ

	void Update();		// 更新処理
	void Draw();		// 描画処理
	bool IsUpdate();	// 更新有無
private:
};
