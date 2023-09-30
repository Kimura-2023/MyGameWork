// ===プレイヤー攻撃クラス実装===
// プレイヤー攻撃クラスを実装
// Author:	Kimura
// ==========================

// ===インクルード===
#include "PlayerAttack.h"
#include "Input.h"
#include "GameManager.h"
#include "DefineData.h"
#include "CameraManager.h"

// ===初期化処理===
void PlayerAttack::Init(Player* player)
{
	if (player->GetStamina() <= 5)	// スタミナが一定以下の場合は何もしない
		return;

	player->SetAnimeTime(0.0f);		// アニメーション時間を設定
	player->SetAnimeRate(0.0f);		// アニメーション遷移率を設定
	player->DelStamina(5);			// スタミナを更新

	// ---向き算出---
	XMFLOAT3 vec = XMFLOAT3(0.0f,0.0f,0.0f);		// 攻撃方向
	if (CInput::GetMouseButton(MOUSEBUTTON_L))		// マウス入力
	{
		vec = GAME->MousePos();						// マウス座標を取得
		XMFLOAT3 pos = player->GetTransform().Pos;	// プレイヤーの座標を取得
		vec.x -= pos.x;				// 向き(x方向)を算出
		vec.z -= pos.z;				// 向き(z方向)を算出
		vec.y = 0.0f;				// 高さは無視する
		Normalize(&vec);			// ベクトルを正規化
		player->SetFront(vec);		// プレイヤーの正面方向を算出したベクトルに設定
	}
	else											// コントローラー入力
	{
		XMFLOAT2 stick = XMFLOAT2(0.0f, 0.0f);		// スティック入力方向
		stick.x = CInput::GetStick(CInput::GetJoyX(JOYSTICKID1));			// スティックX方向の入力値を取得
		stick.y = -CInput::GetStick(CInput::GetJoyY(JOYSTICKID1));			// スティックY方向の入力値を取得

		// ---スティック値の補正---
		if (fabs(stick.x) >= DEAD_STICK_X)			// デッドゾーンを超えていれば値を反映
		{
			Normalize(&stick);		// 入力方向を正規化(スティックの倒し具合で速度を変えたい場合は修正)
			vec.x = stick.x;		// 進行方向にスティックの値(X)を反映
		}
		if (fabs(stick.y) >= DEAD_STICK_Y)			// デッドゾーンを超えていれば値を反映
		{
			Normalize(&stick);		// 入力方向を正規化(スティックの倒し具合で速度を変えたい場合は修正)
			vec.z = stick.y;		// 進行方向にスティックの値(Y)を反映
		}
		
		// ---向きの計算---
		XMFLOAT3 view = CAMERA->GetCurrentCamera()->GetLookVector();	// カメラの視点ベクトルを取得
		Quaternion q;													// 回転クォータニオン
		view.y = 0;														// プレイヤーはY軸回転させたいので視点ベクトルのYの値は無視する
		Quaternion::LookRotation(&q, view);								// 視点方向を向くクォータニオンを算出

		Quaternion p(vec);												// 入力方向
		p = -q * p * q;													// 回転クォータニオンで挟み込み、進行方向の補正をする
		vec = XMFLOAT3(p.x, p.y, p.z);									// 求めた向きを新しい進行方向とする
		XMFLOAT3 input = vec;											// Y方向の速度を無視した値を格納
		vec.y = player->GetVel().y;										// 進行速度(Y)を取得

		float speed = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);		// 速度を求める
		if (speed != 0)				// 進行速度が正しく求められている場合
			player->SetFront(input);				// プレイヤーの正面向きを進行方向にセット
		else
			vec = player->GetFront();
	}

	// ---速度算出---
	player->SetSpeed(PLAYER_SPEED * 0.4f);			// 速度設定
	vec.x *= player->GetSpeed();					// 進行速度を算出(x方向)
	vec.z *= player->GetSpeed();					// 進行速度を算出(y方向)
	player->SetVel(vec);							// 速度を設定

	// ---アニメーション設定---
	player->PlayerSetAnime(ANIM_ATTACK);			// アニメーション設定
	player->PlayerSetBlendAnime(-1);				// 遷移設定
	player->SetAnimeLoop(false);					// ループ状態設定

	// ---武器設定---
	Weapon* weapon = player->GetWeapon();			// 武器取得
	weapon->SetAttack(true);						// 武器の攻撃状態設定
}

// ===更新処理===
void PlayerAttack::Update(Player* player)
{
	// ---取得---
	int nowAnime = player->GetAnimeNo();		// 現在アニメーション番号取得
	float nowTime = player->GetAnimTime();		// 現在アニメーション時間取得
	Model* model = player->GetModel();			// プレイヤーモデル取得
	Weapon* weapon = player->GetWeapon();		// 武器取得

	// ---待機モーションに切替---
	if (player->GetEndAnime())					// アニメーション終了判定
	{
		player->SetStrategy(STAY_PLAYER);		// 待機状態に切替
		weapon->SetCol(false);					// 武器判定無効化
		return;
	}

	// ---武器判定切替---
	if (nowTime * model->GetAnimTick(nowAnime) / 60.0f >= model->GetAnimDuration(nowAnime) * 0.3f)				// 現在アニメーション時間で判定
		weapon->SetCol(true);					// 当たり判定有効化
	else
		weapon->SetCol(false);					// 当たり判定無効化

	if (nowTime * model->GetAnimTick(nowAnime) / 60.0f >= model->GetAnimDuration(nowAnime) * 0.7f)				// 現在アニメーション時間で判定
	{
		if (CInput::GetMouseButton(MOUSEBUTTON_L) || CInput::GetJoyButton(JOYSTICKID1, VB_R))					// ボタン押下
			player->SetStrategy(ATTACK2_PLAYER);		// 攻撃2段目に遷移
		if (CInput::GetMouseButton(MOUSEBUTTON_R) || CInput::GetJoyTrigger(JOYSTICKID1, VB_L))
			player->SetStrategy(REPEL_PLAYER);			// 弾きに遷移
	}
}

// ===更新有無===
bool PlayerAttack::IsUpdate(Player* player)
{
	return player->GetEndAnime();				// アニメーション終了判定
}