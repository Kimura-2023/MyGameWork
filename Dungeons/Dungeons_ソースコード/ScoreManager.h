#pragma once
// ===スコア管理クラス定義===
// スコア管理クラスを定義
// Author Kimura
// ==============================

// ===インクルード===
#include "main.h"
#include "Input.h"
#include "SceneManager.h"
#include "Score.h"

// ===定数・マクロ定義===
#define SCORE (ScoreManager::Get())					// インスタンス取得

class Score;
// ===クラス定義===
class ScoreManager
{
	friend Score;
	// ---メンバ関数---
public:
	ScoreManager();									// コンストラクタ
	~ScoreManager();								// デストラクタ

	static ScoreManager* Get();						// インスタンス取得
	static void Destroy();							// インスタンス破棄
	static void Create();							// インスタンス生成

	void Add(const int& num);						// スコア加算
	void Reset();									// 現在スコアリセット
	void UpdateHiScore();							// ハイスコア更新

protected:
	const int& GetHiScore();						// ハイスコア取得
	const int& GetScore();							// スコア取得

	// ---メンバ変数---
private:
	static ScoreManager* m_pInstance;				// インスタンス
	int m_nScore;									// 現在スコア
	int m_nHiScore;									// ハイスコア
};
