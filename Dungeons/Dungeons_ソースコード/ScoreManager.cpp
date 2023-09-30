// ===スコア管理クラス実装===
// スコア管理クラスを実装
// Author Kimura
// ==============================

// ===インクルード===
#include "ScoreManager.h"

// ===定数・マクロ定義===
#define SCOREFILE ("data/score/hiscore.dat")			// スコアファイル名定義

// ===静的メンバ===
ScoreManager* ScoreManager::m_pInstance = nullptr;

// ===コンストラクタ===
ScoreManager::ScoreManager() : m_nScore(0),m_nHiScore(0)
{
	// ---ハイスコアデータ取得---
	// ---ファイル操作---
	FILE* fp;
	fopen_s(&fp, SCOREFILE, "rb");			// ファイルオープン(読込

	// ---例外処理---
	if (!fp)
		return;

	// ---読み込み---
	fread(&m_nHiScore, sizeof(int), 1, fp);	// ファイルから読み込み
	fclose(fp);								// ファイルクローズ
}

// ===デストラクタ===
ScoreManager::~ScoreManager()
{
	UpdateHiScore();
}

// ===生成===
void ScoreManager::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new ScoreManager;		// インスタンスを生成
	}
}

// ===破棄===
void ScoreManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;					// インスタンスを破棄
		m_pInstance = nullptr;				// nullを格納
	}
}

// ===取得===
ScoreManager* ScoreManager::Get()
{
	return m_pInstance;						// インスタンスを返す
}

// ===スコア加算===
void ScoreManager::Add(const int& num)
{
	m_nScore += num;						// 引数分スコアを加算
}

// ===スコアリセット===
void ScoreManager::Reset()
{
	m_nScore = 0;
}

// ===ハイスコア更新===
void ScoreManager::UpdateHiScore()
{
	// ---ファイル操作---
	FILE* fp;
	// ---ハイスコアデータ書き込み---
	if (m_nHiScore < m_nScore)				// 現在スコアがハイスコアよりも高かった場合
	{
		fopen_s(&fp, SCOREFILE, "wb");		// ファイルオープン(書き込み
		if (fp)
		{
			m_nHiScore = m_nScore;						// スコア更新
			fwrite(&m_nScore, sizeof(int), 1, fp);		// ファイルに書き込み
		}
		fclose(fp);
	}
}

// ===ハイスコア取得===
const int& ScoreManager::GetHiScore()
{
	return m_nHiScore;
}

// ===スコア取得===
const int& ScoreManager::GetScore()
{
	return m_nScore;
}