#pragma once
// ===タイトルロゴUIクラス定義===
// タイトルロゴUIクラスを定義
// Author: Kimura
// ===============================

// ===インクルード===
#include "Object2DBase.h"

// ===クラス定義===
class Title : public Object2DBase
{
public:
	Title();			// コンストラクタ
	~Title();			// デストラクタ

	void Update();		// 更新処理
	void Draw();		// 描画処理
	bool IsUpdate();	// 更新有無
private:
}; 
