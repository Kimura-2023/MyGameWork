// ===ゲームオーバーシーンクラス実装===
// ゲームオーバー時シーンのクラスを実装
// Author:	Kimura
// ==========================

// ===インクルード===
#include "GameOverScene.h"
#include "SceneManager.h"
#include "DebugProc.h"
#include "Input.h"

// ===コンストラクタ===
GameOverScene::GameOverScene()
{
	m_Name = "GameOverScene";						// シーン名
	CreateGameObject("GameOver", BG_GAMEOVER);		// ゲームオーバー画面オブジェクト生成
}

// ===デストラクタ===
GameOverScene::~GameOverScene()
{
}

// ===更新処理===
void GameOverScene::Update()
{
	// ---キー判定---
	if (CInput::GetKeyPress(VK_RETURN) || CInput::GetJoyButton(JOYSTICKID1,VB_A))
	{
		SCENE->SetScene(SCENE_TITLE);		// タイトルシーンへ遷移
	}
}

// ===描画処理===
void GameOverScene::Draw()
{
}