// ===ゲームクリアシーンクラス実装===
// ゲームクリア時シーンのクラスを実装
// Author:	Kimura
// ==========================

// ===インクルード===
#include "GameClearScene.h"
#include "SceneManager.h"
#include "DebugProc.h"
#include "Input.h"

// ===コンストラクタ===
GameClearScene::GameClearScene()
{
	m_Name = "GameClearScene";						// シーン名
	CreateGameObject("GameClear", BG_GAMECLEAR);	// クリア画面オブジェクトを生成
}

// ===デストラクタ===
GameClearScene::~GameClearScene()
{
}

// ===更新処理===
void GameClearScene::Update()
{
	// ---キー判定---
	if (CInput::GetKeyPress(VK_RETURN) || CInput::GetJoyTrigger(JOYSTICKID1, VB_A))
	{
		SCENE->SetScene(SCENE_TITLE);		// タイトルシーンへ遷移
	}
}

// ===描画処理===
void GameClearScene::Draw()
{
}