// ===スタミナUIクラス実装===
// スタミナUIクラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "PlayerStamina.h"
#include "Player.h"
#include "SceneManager.h"
#include "Polygon.h"
#include "TextureManager.h"

// ===コンストラクタ===
PlayerStamina::PlayerStamina()
{
	// ---初期化---
	m_transform.Pos = XMFLOAT3(-SCREEN_WIDTH / 2 + 160 + 16, SCREEN_HEIGHT / 2 - 96, 0.0f);			// 初期座標
	m_transform.Scale = XMFLOAT3(320.0f, 16.0f, 1.0f);												// 拡縮
	m_uv = XMFLOAT2(0.0f, 0.0f);							// uv左上座標
	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);				// 色
	m_TexCut = XMFLOAT2(1, 1);								// uv右下座標
	m_id = UI_STAMINA;										// タグ
	m_Texture = TEX->Load("data/texture/stamina.png");		// テクスチャ読込
}

// ===デストラクタ===
PlayerStamina::~PlayerStamina()
{
}

// ===更新処理===
void PlayerStamina::Update()
{
	Object* player = SCENE->GetCurrentScene()->FindObjectWithTag(OT_PLAYER);	// プレイヤー取得

	// ---例外処理---
	if (!player)
		return;

	// ---スタミナ更新---
	float stamina = (float)((Player*)player)->GetStamina();
	m_transform.Scale.x = 320.0f * stamina / 50.0f;								// 最大スタミナ数で除算
	m_transform.Pos.x = -SCREEN_WIDTH / 2 + m_transform.Scale.x / 2 + 16;		// 拡縮が動いた分移動(ゲージのような見た目にする)
}

// ===描画処理===
void PlayerStamina::Draw()
{
	// ---描画可否---
	if (!m_bEnable)
		return;
	if (m_Texture == nullptr)
		return;

	// ---描画処理---
	SetZWrite(false);													// 深度バッファ更新可否セット
	ID3D11DeviceContext* pDC = GetDeviceContext();						// デバイスコンテキスト取得
	SetBlendState(m_Blend);												// ブレンド情報をセット
	CPolygon::SetColor(m_color);										// 色をセット
	CPolygon::SetTexture(m_Texture);									// テクスチャをセット
	CPolygon::SetSize(m_transform.Scale.x, m_transform.Scale.y);		// サイズセット
	CPolygon::SetUV(m_uv.x, m_uv.y);									// 左上uv座標セット
	CPolygon::SetFrameSize(m_TexCut.x, m_TexCut.y);						// 右下uv座標セット
	CPolygon::SetRotate(m_transform.Rot);								// 回転セット	
	CPolygon::SetPos(m_transform.Pos.x, m_transform.Pos.y);
	CPolygon::Draw(pDC);

	SetZWrite(true);
}