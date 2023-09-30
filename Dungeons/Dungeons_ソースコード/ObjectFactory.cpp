// ===オブジェクト生成クラス実装====
// オブジェクト生成を行うクラスを実装
// Author Kimura
// ==============================

// ===インクルード===
#include "ObjectFactory.h"
#include "Title.h"
#include "Player.h"
#include "Ground.h"
#include "Wall.h"
#include "ModelObject.h"
#include "EnterButton.h"
#include "Pillar.h"
#include "Fence.h"
#include "WallPillar.h"
#include "Door.h"
#include "EnemyNormal.h"
#include "EnemyBullet.h"
#include "StartButton.h"
#include "TitleButton.h"
#include "FinButton.h"
#include "PlayerLife.h"
#include "PlayerStamina.h"
#include "GameOver.h"
#include "GameClear.h"
#include "Goal.h"
#include "Score.h"
#include "ScoreEffect.h"

// ===オブジェクト生成===
Object* ObjectFactrory::CreateObject(ObjType objtype)
{
	switch (objtype)		// 型によって切替
	{
	case OT_PLAYER:				// プレイヤー
		return new Player;
		break;
	case OT_GROUND:				// 地面
		return new Ground;
		break;
	case OT_WALL:				// 壁
		return new Wall;
		break;
	case OT_PILLAR:				// 柱
		return new Pillar;
		break;
	case BG_3DOBJ:				// 背景オブジェクト
		return new ModelObject;
		break;
	case BG_TITLE:				// タイトルロゴ
		return new Title;
		break;
	case BG_ENTER:				// Enterボタン
		return new EnterButton;
		break;
	case OT_ENEMY:				// 通常エネミー
		return new EnemyNormal;
		break;
	case OT_FENCE:				// 柵
		return new Fence;
		break;
	case OT_WALLPILLAR:			// 壁柱
		return new WallPillar;
		break;
	case OT_DOOR:				// 扉
		return new Door;
		break;
	case OT_ENEMY_BULLET:		// 遠距離エネミー
		return new EnemyBullet;
		break;

	case UI_START_BUTTON:		// スタートボタン
		return new StartButton;
		break;
	case UI_TITLE_BUTTON:		// タイトルボタン
		return new TitleButton;
		break;
	case UI_FIN_BUTTON:			// 終了ボタン
		return new FinButton;
		break;
	case UI_LIFE:				// 体力UI
		return new PlayerLife;
		break;
	case UI_STAMINA:			// スタミナUI
		return new PlayerStamina;
		break;
	case BG_GAMEOVER:			// ゲームオーバー背景
		return new GameOver;
		break;

	case BG_GAMECLEAR:			// ゲームクリア背景
		return new GameClear;
		break;

	case OT_GOAL:
		return new Goal;		// ゴールオブジェクト
		break;

	case UI_SCORE:
		return new Score;		// スコアUI

	case BILL_SCORE:
		return new ScoreEffect;	// スコアエフェクト
	}

	return nullptr;
}