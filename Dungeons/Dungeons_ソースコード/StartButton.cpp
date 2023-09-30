// ===開始ボタンクラス実装===
// ゲーム開始ボタンクラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "SceneManager.h"
#include "StartButton.h"
#include "TextureManager.h"

// ===コンストラクタ===
StartButton::StartButton()
{
	// ---初期化---
	m_transform.Scale = XMFLOAT3(192.0f, 64.0f, 1.0f);			// 初期座標
	m_uv = XMFLOAT2(0.0f, 0.0f);								// uv左上座標
	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);					// 色
	m_TexCut = XMFLOAT2(1, 1);									// uv右下座標
	m_id = UI_START_BUTTON;										// タグ
	m_Texture = TEX->Load("data/texture/StartButton.png");		// テクスチャ読込
}

// ===デストラクタ===
StartButton::~StartButton()
{
}

// ===ボタン押下処理===
void StartButton::OnPushed()
{
	SCENE->SetScene(SCENE_GAME);				// ゲームシーンに切替え
}