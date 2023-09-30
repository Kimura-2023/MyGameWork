#pragma once
// ===終了ボタンクラス定義===
// ゲーム終了ボタンクラスを定義
// Author: Kimura

// ===インクルード===
#include "UIButton.h"
#include "main.h"

// ===クラス定義===
class FinButton : public UIButton
{
	// ---メンバ関数
public:
	FinButton();					// コンストラクタ
	~FinButton();					// デストラクタ

	void OnPushed() override;		// ボタン押下

	// ---メンバ変数
private:
};