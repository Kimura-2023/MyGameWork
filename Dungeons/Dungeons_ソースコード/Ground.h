#pragma once
// ===地面クラス定義===
// 地面ブロッククラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "main.h"
#include "Object3DBase.h"
#include "SceneManager.h"
#include "Collision3D.h"

// ===クラス定義===
class Ground : public Object3DBase, Collision3D
{
	// ---メンバ関数---
public:
	Ground();				// コンストラクタ
	~Ground();				// デストラクタ

	void Update();			// 更新処理
	void Draw();			// 描画処理

	bool IsUpdate();		// 更新有無

	void OnCollision(Object*);		// 衝突処理
	void SetResource(std::string);	// 素材パス設定

	void ImGuiWindow();		// エディタ表示設定

	// ---メンバ変数---
private:
	ID3D11ShaderResourceView* m_pTex;		// テクスチャ
};