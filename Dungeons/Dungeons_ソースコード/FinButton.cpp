// ===終了ボタンクラス実装===
// 終了ボタンクラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "SceneManager.h"
#include "FinButton.h"
#include "TextureManager.h"

// ===コンストラクタ===
FinButton::FinButton()
{
	// ---初期化処理---
	m_transform.Scale = XMFLOAT3(192.0f, 64.0f, 1.0f);			// サイズ
	m_uv = XMFLOAT2(0.0f, 0.0f);								// uv左上
	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);					// 色
	m_TexCut = XMFLOAT2(1, 1);									// uv右下
	m_id = UI_FIN_BUTTON;										// オブジェクトタグ
	m_Texture = TEX->Load("data/texture/EndButton.png");		// テクスチャ
}

// ===デストラクタ===
FinButton::~FinButton()
{
}

// ===ボタン押下時の処理===
void FinButton::OnPushed()
{
	Close();					// メインループ終了処理
}