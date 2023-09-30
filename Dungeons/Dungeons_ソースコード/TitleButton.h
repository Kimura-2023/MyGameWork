#pragma once
// ===タイトルボタンクラス定義===
// タイトル画面遷移ボタンクラスを定義
// Author: Kimura

// ===インクルード===
#include "UIButton.h"
#include "main.h"

// ===クラス定義===
class TitleButton : public UIButton
{
	// ---メンバ関数---
public:
	TitleButton();			// コンストラクタ
	~TitleButton();			// デストラクタ

	void OnPushed();		// ボタン押下処理

	// ---メンバ変数---
private:
};