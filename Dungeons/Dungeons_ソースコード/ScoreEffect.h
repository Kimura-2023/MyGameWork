#pragma once
// ===スコアエフェクトクラス定義===
// スコアエフェクトクラスを定義
// Author: Kimura
// ======================

// ===インクルード===
#include "main.h"
#include "Object2DBase.h"

// ===クラス定義===
class ScoreEffect : public Object2DBase
{
	// ---メンバ関数---
public:
	ScoreEffect();		// コンストラクタ
	~ScoreEffect();		// デストラクタ

	void Update() override;		// 更新処理
	void Draw() override;		// 描画処理

	void SetScore(const int& num);		// スコアをセット

	bool IsUpdate() override { return true; }

	// ---メンバ変数---
private:
	int m_nScore;				// スコア
};