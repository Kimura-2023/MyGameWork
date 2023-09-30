#pragma once
// ===モデル管理クラス定義===
// モデル管理クラスを定義
// Author kimura
// ======================

// ===インクルード===
#include "main.h"
#include <vector>
#include <memory>
#include <map>
#include "Model.h"

// ===定数・マクロ定義===
#define MODEL (ModelManager::Get())		// インスタンス取得

// ===背景モデル種類定義===
enum MODEL_TYPE
{
	TABLE_MODEL = 0,
	CHAIR_MODEL,
	HOUSE_MODEL,
	SWORD_MODEL,
	PLAYER_MODEL,
	WALL_MODEL,
	SPIKE_MODEL,

	MAX_MODEL
};

// ===クラス定義===
class ModelManager
{
	// ---メンバ関数---
public:
	ModelManager();						// コンストラクタ
	static ModelManager* Get();			// インスタンス取得
	static void Destroy();				// インスタンス破棄
	static void Create();				// インスタンス生成

	void Release();						// モデル全解放

	Model* Load(std::string);			// モデル読込
	void Draw();						// 描画処理

	// ---メンバ変数
private:
	static ModelManager* m_pInstance;									// インスタンス
	std::map<std::string, std::shared_ptr<Model>> m_ModelResource;		// 読み込んだモデル
};

// ===関数定義===
LPCSTR GetModelPass(MODEL_TYPE);	// imgui上で使用するモデルデータのファイルパスを取得