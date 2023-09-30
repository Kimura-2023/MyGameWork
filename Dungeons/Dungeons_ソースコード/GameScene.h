#pragma once
// ===ゲームシーンクラス定義===
// ゲームシーンクラスを定義
// Author kimura
// =================================

// ===インクルード===
#include "main.h"
#include "SceneBase.h"

// ===クラス定義===
class GameScene : public SceneBase
{
	// ---メンバ関数---
public:
	GameScene();		// コンストラクタ
	~GameScene();		// デストラクタ
	void Update();		// 更新処理
	void Draw();		// 描画処理
};