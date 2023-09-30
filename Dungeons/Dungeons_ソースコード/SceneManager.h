#pragma once
// ===シーン管理クラス定義===
// シーン管理クラスを定義
// Author kimura
// ======================

// ===インクルード===
#include "main.h"
#include "SceneBase.h"
#include <memory>

// ===シーン種類定義===
enum Scene
{
	SCENE_NONE,
	SCENE_TITLE,
	SCENE_GAME,
	SCENE_DEBUG,
	SCENE_GAMEOVER,
	SCENE_GAMECLEAR,
	SCENE_LOAD,
	SCENE_END,
};

// ===定数・マクロ定義===
#define SCENE (SceneManager::Get())			// SceneManagerのインスタンス取得

// ===クラス定義===
class SceneManager
{
	// ---メンバ関数---
public:
	static void Create();					// 生成
	static void Destroy();					// 破棄
	static SceneManager* Get();				// 取得

	void Update();							// 更新
	void Draw();							// 描画

	void SetScene(Scene);										// シーンをセット
	SceneBase* GetCurrentScene() { return m_pScene.get(); }		// 現在シーンを取得
	void LoadScene(Scene);										// シーン読込

private:
	void ReleaseScene();						// シーン解放
	SceneManager();								// コンストラクタ
	~SceneManager();							// デストラクタ

	// ---メンバ変数
private:
	static SceneManager* m_pInstance;			// マネージャのインスタンス

	std::unique_ptr<SceneBase> m_pScene;		// 現在シーンのポインタ
	Scene m_eCurrentScene;						// 現在シーン型
	Scene m_eChangeScene;						// 遷移先シーン型
};