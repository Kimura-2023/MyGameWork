// ===タイトルロゴクラス実装===
// タイトルロゴUIクラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "Title.h"
#include "TextureManager.h"

// ===コンストラクタ===
Title::Title()
{
	// ---初期化処理---
	m_transform.Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);			// 初期座標
	m_Scale = XMFLOAT2(1200, 300);							// 拡縮
	m_transform.Rot = XMFLOAT3(0.0f, 0.0f, 0.0f);			// 回転
	m_uv = XMFLOAT2(0.0f, 0.0f);							// uv左上座標
	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);				// 色
	m_TexCut = XMFLOAT2(1.0f, 1.0f);						// uv右下座標
	m_id = BG_TITLE;										// タグ
	m_bEnable = true;										// 更新・描画可否
	m_Blend = BS_ALPHABLEND;								// ブレンドステート
	m_ResourcePass = "data/texture/TitleLogo.png";			// テクスチャのパス
	m_Texture = TEX->Load("data/texture/TitleLogo.png");	// テクスチャ読み込み
}

// ===デストラクタ===
Title::~Title()
{
}

// ===更新処理===
void Title::Update()
{
	m_transform.Scale.x += 0.005f;		// 徐々に拡大(x方向)
	m_transform.Scale.y += 0.005f;		// 徐々に拡大(y方向)
	if (m_transform.Scale.x > 0.9f)			// 一定サイズ以上で小さくする
	{
		m_transform.Scale.x = 0.7f;		// 拡縮を0.7倍に
		m_transform.Scale.y = 0.7f;		// 拡縮を0.7倍に
	}

	Object2DBase::Update();		// 更新
}

// ===描画===
void Title::Draw()
{
	Object2DBase::Draw();		// 描画
}

// ===更新有無===
bool Title::IsUpdate()
{
	return true;
}