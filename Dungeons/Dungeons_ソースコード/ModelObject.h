#pragma once
// ===背景モデル定義===
// 背景モデルクラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "main.h"
#include "Object3DBase.h"
#include "ModelManager.h"

// ===クラス定義===
class ModelObject : public Object3DBase
{
	// ---メンバ関数---
public:
	ModelObject();			// コンストラクタ
	~ModelObject();			// デストラクタ

	void Update();			// 更新処理
	void Draw();			// 描画処理

	bool IsUpdate();		// 更新有無

	void SetResource(std::string);		// 素材のパス設定

	void ImGuiWindow();		// エディタ表示

	// ---メンバ変数---
private:
};