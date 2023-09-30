#pragma once

// ===クリアシーンクラス定義===
// クリアシーンクラスを定義
// Author kimura
// =================================

// ===インクルード===
#include "main.h"
#include "SceneBase.h"

// ===クラス定義===
class GameClearScene : public SceneBase
{
	// ---メンバ関数---
public:
	GameClearScene();		// コンストラクタ
	~GameClearScene();		// デストラクタ
	void Update();			// 更新処理
	void Draw();			// 描画処理
};