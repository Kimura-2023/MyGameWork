// ===UIクラス実装===
// UIクラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "EnterButton.h"
#include "TextureManager.h"
#include "DebugProc.h"

// ===コンストラクタ===
EnterButton::EnterButton()
{
	m_transform.Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);			// 座標
	m_Scale = XMFLOAT2(800, 200);							// サイズ
	m_transform.Rot = XMFLOAT3(0.0f, 0.0f, 0.0f);			// 回転
	m_uv = XMFLOAT2(0.0f, 0.0f);							// uv左上座標
	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);				// 色
	m_TexCut = XMFLOAT2(1, 1);								// uv右下座標

	m_id = BG_ENTER;									// タグ
	m_bEnable = true;									// 更新・描画可否
	m_Blend = BS_ALPHABLEND;							// ブレンドステート
	m_ResourcePass = "data/texture/EnterButton.png";	// テクスチャ素材のパス

	m_Texture = TEX->Load("data/texture/EnterButton.png");	// テクスチャ読込
}

// ===デストラクタ===
EnterButton::~EnterButton()
{
}

// ===更新処理===
void EnterButton::Update()
{
	Object2DBase::Update();			// 2Dオブジェクト更新処理
}

// ===描画処理===
void EnterButton::Draw()
{
	Object2DBase::Draw();			// 描画
}

// ===更新したかどうか===
bool EnterButton::IsUpdate()
{
	return true;
}