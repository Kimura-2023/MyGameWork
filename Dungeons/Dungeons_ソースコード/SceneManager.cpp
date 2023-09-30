// ===シーン管理クラス 実装===============
// シーン管理クラスを実装
// Author kimura
// =========================================

// ===インクルード===
#include "SceneManager.h"
#include "TitleScene.h"
#include "DebugScene.h"
#include "GameScene.h"
#include "ObjectManager.h"
#include "Fade.h"
#include "GameCamera.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "GameOverScene.h"
#include "GameClearScene.h"
#include "CameraManager.h"
#include "MapManager.h"
#include "ScoreManager.h"
#include "ScoreEffect.h"
#include "Sound.h"

// ===グローバル宣言===
SceneManager* SceneManager::m_pInstance = nullptr;		// インスタンス初期化

// ===コンストラクタ===
SceneManager::SceneManager()
{
	// ---初期化処理---
#ifdef _DEBUG						// デバッグビルド
	m_pScene = std::make_unique<DebugScene>();			// デバッグシーン生成
	m_eCurrentScene = SCENE_DEBUG;						// 現在シーンをデバッグシーンにセット
	m_eChangeScene = SCENE_NONE;						// 遷移先シーン無し
#else								// リリースビルド
	m_pScene = std::make_unique<TitleScene>();			// タイトルシーン生成
	m_eCurrentScene = SCENE_TITLE;						// 現在シーンをタイトルシーンにセット
	m_eChangeScene = SCENE_NONE;						// 遷移先シーン無し
#endif
}

// ===デストラクタ===
SceneManager::~SceneManager()
{
	m_pScene.reset();			// シーン破棄
}

// ===生成===
void SceneManager::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new SceneManager;	// インスタンスを生成
	}
}

// ===破棄===
void SceneManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;		// インスタンスを破棄
		m_pInstance = nullptr;
	}
}

// ===取得===
SceneManager* SceneManager::Get()
{
	return m_pInstance;			// インスタンスを返す
}

// ===更新===
void SceneManager::Update()
{
	// ---更新判定---
	if (m_pScene != nullptr && FADE_NONE == CFade::GetState())		// フェード中は更新しない
	{
		m_pScene->Update();					// シーンの更新
	}

	// ---シーン切り替え処理---
	if (m_eChangeScene != SCENE_NONE && m_eChangeScene != m_eCurrentScene)		// シーン切り替えが起こっているか判定
	{
		if (FADE_NONE == CFade::GetState())
		{
			if (m_eChangeScene != SCENE_LOAD)
				ReleaseScene();						// 現在シーンを解放

			LoadScene(m_eChangeScene);
			CFade::In(0.7f);					// フェード処理
		}
	}
}

// ===終了===
void SceneManager::ReleaseScene()
{
	if (!m_pScene)
		return;
	m_pScene.reset();			// シーン解放
	m_pScene = nullptr;
}

// ===描画===
void SceneManager::Draw()
{
	// ---例外処理---
	if (m_pScene == nullptr)
		return;

	// ---カメラ取得---
	CameraBase* pCamera = CAMERA->GetCurrentCamera();
	if (pCamera)
	{
		pCamera->Clear();		// クリア
	}

	m_pScene->Draw();			// シーン描画(シャドウマップ)
}

// ===シーン切替関数===
// 遷移するシーンを指定する
void SceneManager::SetScene(Scene scene)
{
	if (m_eChangeScene != SCENE_NONE && m_eChangeScene == scene)
		return;

	m_eChangeScene = scene;			// 次シーンを引数でもらった値に書き換え
	CFade::Out(0.7f);				// フェードアウト

	// ---ゴールオブジェクトと接触時---
	if (scene == SCENE_LOAD)
	{
		Object* score = m_pScene->CreateGameObject("ScoreEff", BILL_SCORE);			// スコアエフェクト表示
		Object* goal = m_pScene->FindObjectWithTag(OT_GOAL);						// ゴールオブジェクトを取得
		score->SetPos(XMFLOAT3(goal->GetPos().x,goal->GetPos().y + 1.0f,goal->GetPos().z));					// エフェクトの座標をセット
		((ScoreEffect*)score)->SetScore(500);										// スコアをセット
		CSound::Play(SE_WARP);														// 効果音再生
	}

}

// ===シーン読み込み===
void SceneManager::LoadScene(Scene scene)
{
	// ---分岐処理---
	switch (scene)					// 引数のシーン定数ごとに生成するシーンを分岐
	{
	case SCENE_TITLE:
		m_pScene = std::make_unique<TitleScene>();	// タイトルシーンを生成
		break;
	case SCENE_GAME:
		m_pScene = std::make_unique<GameScene>();	// ゲームシーンを生成
		break;
	case SCENE_GAMEOVER:
		m_pScene = std::make_unique<GameOverScene>();	// ゲームオーバーシーンを生成
		break;
	case SCENE_GAMECLEAR:
		m_pScene = std::make_unique<GameClearScene>();	// ゲームクリアシーンを生成
		break;
	case SCENE_DEBUG:
		m_pScene = std::make_unique<DebugScene>();	// デバッグシーンを生成
		break;
	case SCENE_END:
		m_pScene = std::make_unique<TitleScene>();
		break;
	case SCENE_LOAD:
		SCORE->Add(500);
		MAP->AddLevel();
		MAP->ChangeRoom(5);
		m_eChangeScene = SCENE_NONE;
		return;
	}

	m_eCurrentScene = scene;		// 現在シーンを更新
	m_eChangeScene = SCENE_NONE;	// 遷移先シーンをリセット
}