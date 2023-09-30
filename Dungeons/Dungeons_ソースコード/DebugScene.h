// ===デバッグ用シーンクラス定義===
// デバッグ用シーンクラスを定義
// ---Author kimura
// =================================
#pragma once


// ===インクルード===
#include "main.h"
#include "SceneBase.h"

// ===クラス定義===
class DebugScene : public SceneBase
{
	// ---メンバ関数---
public:
	DebugScene();			// コンストラクタ
	~DebugScene();			// デストラクタ
	void Update();			// 更新処理
	void Draw();			// 描画処理
private:
	void CheckSelectObject();		// オブジェクト選択状態更新

	// ---メンバ変数---
private:
	float m_HitZ;			// カーソルが当たっている一番手前の奥行
};