#pragma once
// ===スタートボタンクラス定義===
// ゲーム開始ボタンクラスを定義
// Author: Kimura

// ===インクルード===
#include "UIButton.h"
#include "main.h"

// ===クラス定義===
class StartButton : public UIButton
{
	// ---メンバ関数---
public:
	StartButton();		// コンストラクタ
	~StartButton();		// デストラクタ

	void OnPushed() override;	// ボタン押下

	// ---メンバ変数---
private:
};