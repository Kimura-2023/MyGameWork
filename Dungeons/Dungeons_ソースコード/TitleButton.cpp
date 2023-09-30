// ===タイトルボタンクラス実装===
// タイトル遷移ボタンクラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "SceneManager.h"
#include "TitleButton.h"
#include "TextureManager.h"

// ===コンストラクタ===
TitleButton::TitleButton()
{
	// ---初期化---
	m_transform.Scale = XMFLOAT3(96.0f, 32.0f, 1.0f);			// サイズ
	m_uv = XMFLOAT2(0.0f, 0.0f);								// uv左上座標
	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);					// 色
	m_TexCut = XMFLOAT2(1, 1);									// uv右下座標
	m_id = UI_TITLE_BUTTON;										// タグ
	m_Texture = TEX->Load("data/texture/EnterButton.png");		// テクスチャ読込
}

// ===デストラクタ===
TitleButton::~TitleButton()
{
}

// ===ボタン押下処理===
void TitleButton::OnPushed()
{
	SCENE->SetScene(SCENE_TITLE);								// タイトルシーンに遷移
}