// ===ボタンUIベースクラス実装===
// ボタンUIベースクラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "UIButton.h"
#include "Input.h"
#include "SelectButtonManager.h"

// ===コンストラクタ===
UIButton::UIButton()
{
	// ---初期化処理---
	m_bSelected = false;						// 非選択状態
	m_nIndex = BUTTON->AddButtonManager(this);	// ボタン管理下に追加
}

// ===デストラクタ===
UIButton::~UIButton()
{
	BUTTON->Release(this);						// ボタン管理下から解放
}

// ===更新処理===
void UIButton::Update()
{
	// ---更新判定---
	if (!m_bEnable)
		return;

	// ---スクリーン上でのマウスカーソルとの衝突判定(AABB)---
	XMFLOAT2 min, max;
	min = XMFLOAT2(m_transform.Pos.x - m_transform.Scale.x / 2, m_transform.Pos.y - m_transform.Scale.y / 2);			// 左上座標
	max = XMFLOAT2(m_transform.Pos.x + m_transform.Scale.x / 2, m_transform.Pos.y + m_transform.Scale.y / 2);			// 右下座標

	POINT mouse = *CInput::GetMousePosition();																			// マウス座標取得
	XMFLOAT2 MousePos = XMFLOAT2((float)mouse.x, (float)mouse.y);														// 計算をするために変換
	MousePos = XMFLOAT2(MousePos.x - SCREEN_WIDTH / 2, -MousePos.y + SCREEN_HEIGHT / 2);								// スクリーンに収まるように補正
	if (min.x <= MousePos.x && max.x >= MousePos.x && min.y <= MousePos.y && max.y >= MousePos.y)						// スクリーン上でカーソルが重なっている場合
	{
		BUTTON->SetSelect(m_nIndex);		// 選択状態にする
	}

	// ---選択状態ごとの処理---
	if (m_bSelected)
		m_color.w = 1.0f;				// 標準
	else
		m_color.w = 0.6f;				// 選択されているボタンはα値を薄くする
}