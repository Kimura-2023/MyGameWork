#pragma once
// ===ボタンUIベースクラス定義===
// ボタンUIベースクラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "UIBase.h"
#include "main.h"

// ===クラス定義===
class UIButton : public UIBase
{
	// ---メンバ関数---
public:
	UIButton();					// コンストラクタ
	virtual ~UIButton();		// デストラクタ

	void Update();					// 更新処理
	virtual void OnPushed() = 0;	// ボタン押下処理
	void SetSelected(bool flg) { m_bSelected = flg; }		// 選択されているかどうか

	int& GetIndex() { return m_nIndex; }		// ボタン番号取得

	// ---メンバ変数---
private:
	bool m_bSelected;									// 選択されているかどうか
	int m_nIndex;										// ボタン番号
};