#pragma once
// ===UIベースクラス定義===
// UIベースクラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "Object.h"
#include "main.h"

// ===クラス定義===
class UIBase : public Object
{
	// ---メンバ関数---
public:
	UIBase();				// コンストラクタ
	virtual ~UIBase();		// デストラクタ

	void virtual Update();	// 更新処理
	void virtual Draw();	// 描画処理

	void SetResource(std::string);		// 素材パス設定

	bool IsUpdate();		// 更新判定
	void UpdateMatrix();	//

	// ---メンバ変数---
protected:
	EBlendState m_Blend;	// ブレンドステート
	ID3D11ShaderResourceView* m_Texture;		// テクスチャ

	XMFLOAT2 m_uv;			// 左上uv座標
	XMFLOAT2 m_TexCut;		// テクスチャ分割数
	XMFLOAT4 m_color;		// 色
};