// ===体力UIクラス実装===
// 体力UIクラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "PlayerLife.h"
#include "Player.h"
#include "SceneManager.h"
#include "Polygon.h"
#include "TextureManager.h"

// ===定数・マクロ定義===
#define MAX_PLAYER_LIFE (10)		// 最大体力

// ===コンストラク===
PlayerLife::PlayerLife()
{
	// ---初期化処理---
	m_transform.Pos = XMFLOAT3(-SCREEN_WIDTH / 2 + 32, SCREEN_HEIGHT / 2 - 64, 0.0f);		// 初期座標
	m_transform.Scale = XMFLOAT3(64.0f, 32.0f, 1.0f);										// サイズ
	m_uv = XMFLOAT2(0.0f, 0.0f);						// uv左上座標
	m_TexCut = XMFLOAT2(1, 1);							// uv右上座標
	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);			// 色
	m_id = UI_LIFE;										// オブジェクトタグ
	m_Texture = TEX->Load("data/texture/life.png");		// テクスチャ
	m_nLife = MAX_PLAYER_LIFE;							// 体力
}

// ===デストラクタ===
PlayerLife::~PlayerLife()
{
}

// ===更新処理===
void PlayerLife::Update()
{
	// ---体力更新---
	Object* player = SCENE->GetCurrentScene()->FindObjectWithTag(OT_PLAYER);		// プレイヤー取得
		// ---例外処理
	if (!player)
		return;
	m_nLife = ((Player*)player)->GetLife();				// プレイヤーの体力を取得する
}

// ===描画処理===
void PlayerLife::Draw()
{
	// ---描画判定
	if (!m_bEnable)
		return;
	if (m_Texture == nullptr)
		return;

	// ---描画---
	SetZWrite(false);													// 深度バッファ更新可否セット
	ID3D11DeviceContext* pDC = GetDeviceContext();						// デバイスコンテキスト取得
	SetBlendState(m_Blend);												// ブレンド情報をセット
	CPolygon::SetColor(m_color);										// 色をセット
	CPolygon::SetTexture(m_Texture);									// テクスチャをセット
	CPolygon::SetSize(m_transform.Scale.x, m_transform.Scale.y);		// サイズセット
	CPolygon::SetUV(m_uv.x, m_uv.y);									// 左上uv座標セット
	CPolygon::SetFrameSize(m_TexCut.x, m_TexCut.y);						// 右下uv座標セット
	CPolygon::SetRotate(m_transform.Rot);								// 回転セット										

	for (int i = 0; i < m_nLife; ++i)				// 体力の数だけ描画
	{
		CPolygon::SetPos(m_transform.Pos.x + m_transform.Scale.x / 2 * i, m_transform.Pos.y);		// 座標をセット
		CPolygon::Draw(pDC);		// 描画
	}

	SetZWrite(true);			// 深度バッファ更新可否セット
	SetZBuffer(true);			// 深度バッファ有効化
}