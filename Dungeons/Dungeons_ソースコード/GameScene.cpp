// ===ゲームシーンクラス実装===
// ゲームシーンのクラスを実装
// Author:	Kimura
// ==========================

// ===インクルード===
#include "GameScene.h"
#include "SceneManager.h"
#include "DebugProc.h"
#include "Input.h"
#include "MapManager.h"
#include "GameManager.h"
#include "GameCamera.h"
#include "CameraManager.h"
#include "Sound.h"
#include "Door.h"
#include "ScoreManager.h"

// ===コンストラクタ===
GameScene::GameScene()
{
	// ---初期化処理---
	CSound::SetVolume(0.5f);					// 音量設定
	CSound::Play(BGM_GAME);						// BGM再生

	m_Name = "GameScene";						// シーン名

	MapManager::Create();						// マップ生成
	Room* pRoom = MAP->GetCurrentRoom();		// 初期の部屋取得
	LoadStageData(pRoom->pass, pRoom->rot, 0);		// 部屋の形からデータを読み込む

	CameraManager::Get()->RegisterCamera("GameCamera", GAME_CAMERA);					// カメラ生成
	CameraManager::Get()->SetCamera(CameraManager::Get()->FindCamera("GameCamera"));	// カメラをセット

	// ---オブジェクト生成---
	Object* player = CreateGameObject("Player", OT_PLAYER);		// プレイヤー生成

	((GameCamera*)(CameraManager::Get()->GetCurrentCamera()))->SetTargetObj(player);	// カメラの追従対象をプレイヤーにセット
	XMFLOAT3 startPos = { 0.0f,1.0f,0.0f };		// 初期座標
	player->SetPos(startPos);					// 座標を設定
}

// ===デストラクタ===
GameScene::~GameScene()
{
	CSound::Stop(BGM_GAME);						// BGMストップ
	MAP->Destroy();								// マップ破棄
	SCORE->UpdateHiScore();						// ハイスコア更新
	SCORE->Reset();								// 現在スコアをリセット
}

// ===更新処理===
void GameScene::Update()
{
	// ---ゴール到達処理---
	if (MAP->GetCurrentRoom()->type == ROOM_GOAL)
	{
		// SCENE->SetScene(SCENE_GAMEOVER);
	}
	else
	{
		CDebugProc::Print("%d,%d\n", MAP->goal_x, MAP->goal_y);
	}

	// ---ステージクリア判定---
	std::vector<Object*>enemy = FindObjectListWithTag(OT_ENEMY);			// 通常敵リストを取得
	std::vector<Object*>enemy2 = FindObjectListWithTag(OT_ENEMY_BULLET);	// 遠距離敵リストを取得
	auto it = enemy.begin();			// 通常敵リストの先頭イテレータを取得
	bool clear = true;					// クリア判定
	while (it != enemy.end())
	{
		if ((*it)->GetEnable())			// 生存している敵キャラクターがいれば非クリア
			clear = false;			// クリアフラグをオフにする
		++it;							// イテレータをずらす
	}

	auto bit = enemy2.begin();			// 遠距離敵リストの先頭イテレータを取得
	while (bit != enemy2.end())
	{
		if ((*bit)->GetEnable())		// 生存している敵キャラクターがいれば非クリア
			clear = false;			// クリアフラグをオフにする
		++bit;							// イテレータをずらす
	}

	// ---クリア時処理---
	if (clear && enemy.size() > 0)			// 生存している敵キャラクターがいない場合
	{
		std::vector<Object*> door = FindObjectListWithTag(OT_DOOR);		// ドアのリストを取得
		auto dit = door.begin();			// ドアリストの先頭イテレータを取得
		while (dit != door.end())
		{
			((Door*)(*dit))->Open();		// ドア開ける
			++dit;							// イテレータをずらす
		}
		MAP->SetClear(true);				// クリア判定をセット
	}
}

// ===描画処理===
void GameScene::Draw()
{
	SceneBase::Draw();						// シーン描画
}