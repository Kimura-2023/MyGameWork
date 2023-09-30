// ===モデル管理クラス定義===
// モデル管理クラスを定義
// Author kimura
// ======================

// ===インクルード===
#include "ModelManager.h"

// ===静的メンバ===
ModelManager* ModelManager::m_pInstance = nullptr;

// ===モデルのパス===
namespace ModelKind
{
	LPCSTR pass[MAX_MODEL] = {
		"data/model/Table.fbx",
		"data/model/Chair.fbx",
		"data/model/House.fbx",
		"data/model/Sword.fbx",
		"data/model/Player.fbx",
		"data/model/Wall.fbx",
		"data/model/Spike.fbx",
	};
}

// ===コンストラクタ===
ModelManager::ModelManager()
{
}

// ===生成===
void ModelManager::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new ModelManager;
	}
}

// ===破棄===
void ModelManager::Destroy()
{
	MODEL->Release();							// モデル解放
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

// ===取得===
ModelManager* ModelManager::Get()
{
	return m_pInstance;
}

// ===モデル全解放===
void ModelManager::Release()
{
	auto ait = m_ModelResource.begin();				// モデルリストの先頭イテレータ取得
	while (ait != m_ModelResource.end())			// 終端まで繰り返し
	{
		ait->second.reset();					// 解放
		++ait;									// イテレータをずらす
	}
}

// ===モデル読込===
Model* ModelManager::Load(std::string file)
{
	auto it = m_ModelResource.find(file);		// 引数のパスで探索
	if (it == m_ModelResource.end())			// リソースが見つからなかった場合
	{
		std::shared_ptr<Model> pmodel = std::make_shared<Model>();
		if (pmodel->Load(file.c_str()))									// モデルが読み込めた場合
		{
			m_ModelResource.insert(std::make_pair(file, pmodel));		// マップに登録
			return m_ModelResource.at(file).get();
		}
		else															// モデルが読み込めなかった場合
		{
			return nullptr;												// nullptrを返す
		}
	}
	else																// リソースが見つかった場合
	{
		return it->second.get();
	}
}

// ===インスタンス描画===
void ModelManager::Draw()
{
	// 未実装
}

// ===エディタ上で表示するパス名取得===
LPCSTR GetModelPass(MODEL_TYPE type)
{
	return ModelKind::pass[type];
}