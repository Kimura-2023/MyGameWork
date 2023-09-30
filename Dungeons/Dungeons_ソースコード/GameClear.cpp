// ===クリア画面クラス実装===
// ゲームクリア画面UIクラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "GameClear.h"
#include "TextureManager.h"
#include "DebugProc.h"

// ===コンストラクタ===
GameClear::GameClear()
{
	m_transform.Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);		// 初期座標
	m_Scale = XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT);	// サイズ
	m_transform.Rot = XMFLOAT3(0.0f, 0.0f, 0.0f);		// 回転
	m_uv = XMFLOAT2(0.0f, 0.0f);						// uv左上座標
	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);			// 色
	m_TexCut = XMFLOAT2(1, 1);							// uv右下座標
	m_id = BG_GAMECLEAR;			// タグ
	m_bEnable = true;				// 更新・描画可否
	m_Texture = TEX->Load("data/texture/clear.png");	// テクスチャ読込
}

// ===デストラクタ===
GameClear::~GameClear()
{
}

// ===更新処理===
void GameClear::Update()
{
}

// ===描画処理===
void GameClear::Draw()
{
	Object2DBase::Draw();		// 描画
}

// ===更新有無===
bool GameClear::IsUpdate()
{
	return true;
}