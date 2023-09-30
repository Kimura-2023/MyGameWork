#pragma once

// ===ゲームオーバーシーンクラス定義===
// ゲームオーバーシーンクラスを定義
// Author kimura
// =================================

// ===インクルード===
#include "main.h"
#include "SceneBase.h"

// ===クラス定義===
class GameOverScene : public SceneBase
{
	// ---メンバ関数---
public:
	GameOverScene();		// コンストラクタ
	~GameOverScene();		// デストラクタ
	void Update();			// 更新処理
	void Draw();			// 描画処理
};