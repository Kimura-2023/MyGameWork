#pragma once
// ===タイトルシーンクラス定義===
// タイトルシーンクラスを定義
// Author kimura
// =================================

// ===インクルード===
#include "main.h"
#include "SceneBase.h"

// ===クラス定義===
class TitleScene : public SceneBase
{
public:
	TitleScene();		// コンストラクタ
	~TitleScene();		// デストラクタ
	void Update();		// 更新処理
	void Draw();		// 描画処理
};