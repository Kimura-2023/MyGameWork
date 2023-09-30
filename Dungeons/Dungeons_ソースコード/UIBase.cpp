// ===UIベースクラス実装===
// UIベースクラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "UIBase.h"
#include "TextureManager.h"
#include "Polygon.h"
#include "ObjectManager.h"

// ===コンストラクタ===
UIBase::UIBase()
{
	// ---初期化処理---
	m_transform.Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);			// 座標
	m_transform.Rot = XMFLOAT3(0.0f, 0.0f, 0.0f);			// 回転
	m_transform.Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);			// 拡縮
	m_bGravity = false;								// 重力
	m_bEnable = false;								// 更新・描画可否
	m_Blend = BS_ALPHABLEND;						// ブレンドステート
	m_Texture = nullptr;							// テクスチャ

	m_uv = XMFLOAT2(0.0f,0.0f);						// 左上uv座標
	m_TexCut = XMFLOAT2(1.0f,1.0f);					// テクスチャ分割数
	m_color = XMFLOAT4(1.0f,1.0f,1.0f,1.0);			// 色

	OBJECT->AddObjectManager(this);					// オブジェクト管理下追加
}

// ===デストラクタ===
UIBase::~UIBase()
{
	OBJECT->Release(this);		// オブジェクト解放
}

// ===更新処理===
void UIBase::Update()
{

}

// ===描画処理===
void UIBase::Draw()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();		// デバイスコンテキスト取得

	// ---描画判定---
	if (!m_bEnable)
		return;
	if (m_Texture == nullptr)
		return;

	// ---描画準備---
	SetZWrite(false);

	SetBlendState(m_Blend);
	CPolygon::SetColor(m_color);
	CPolygon::SetTexture(m_Texture);
	CPolygon::SetSize(m_transform.Scale.x,m_transform.Scale.y);
	CPolygon::SetUV(m_uv.x, m_uv.y);
	CPolygon::SetFrameSize(m_TexCut.x, m_TexCut.y);
	CPolygon::SetPos(m_transform.Pos.x, m_transform.Pos.y);
	CPolygon::SetRotate(m_transform.Rot);
	CPolygon::SetDraw(true);

	// ---描画---
	CPolygon::Draw(pDC);

	// ---描画状態を元に戻す---
	SetZWrite(true);
	SetZBuffer(true);
}

// ===素材パス設定===
void UIBase::SetResource(std::string pass)
{
	m_ResourcePass = pass;
	m_Texture = TEX->Load(pass);
}

// ===更新判定===
bool UIBase::IsUpdate()
{
	return true;
}

void UIBase::UpdateMatrix()
{

}