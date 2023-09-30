#pragma once
// ===プレイヤークラス定義===
// プレイヤークラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "main.h"
#include "PlayerStrategy.h"
#include "Collision3D.h"
#include "Weapon.h"
#include "Bullet.h"

// ===列挙定義===
enum PLAYER_STATE		// ---プレイヤー状態
{
	STAY_PLAYER = 0,	// 待機
	MOVE_PLAYER,		// 移動
	ATTACK_PLAYER,		// 攻撃
	ROLL_PLAYER,		// 回避
	ATTACK2_PLAYER,		// 攻撃(二段目)
	REPEL_PLAYER,		// はじき

	MAX_PLAYER
};
namespace {				// ---アニメーション
	enum EAnimNo {
		ANIM_IDLE = 0,	// 待機アニメーション
		ANIM_RUN,		// 移動アニメーション
		ANIM_ROLL,		// 回避アニメーション
		ANIM_ATTACK,	// 攻撃アニメーション
		ANIM_ATTACK2,	// 2段目攻撃アニメーション
		ANIM_REPEL,		// 弾きアニメーション
		MAX_ANIMNO		// 最大
	};
}

// ===クラス定義===
class PlayerStrategy;
class Player : public Object3DBase, Collision3D
{
	// ---メンバ関数---
public:
	Player();			// コンストラクタ
	~Player();			// デストラクタ

	void Init();		// 初期化処理
	void Update();		// 更新処理
	void Draw();		// 描画処理

	bool IsUpdate();	// 更新有無
	void SetStrategy(PLAYER_STATE player);		// 状態設定
	void OnCollision(Object*);					// 衝突処理
	void ReflectMovement();						// めり込み量を戻す

	bool GetAtk() { return m_bAtk; }			// 攻撃状態取得
	void SetAtk(bool flg) { m_bAtk = flg; }		// 攻撃状態設定

	float& GetSpeed() { return m_fSpeed; }		// 速度取得	
	void SetSpeed(const float& speed) { m_fSpeed = speed; }		// 速度設定

	void PlayerSetAnime(int num) { m_animNo = num; }			// アニメーション番号設定
	void PlayerSetBlendAnime(int num) { m_blendNo = num; }		// アニメーションブレンド設定

	int& GetLife() { return m_nLife; }			// 体力取得
	float& GetStamina() { return m_nStamina; }	// スタミナ取得

	int& GetAnimeNo() { return m_animNo; }		// アニメーション番号取得
	float& GetAnimTime() { return m_animTime; }	// アニメーション再生時間取得
	float& GetRate() { return m_rate; }			// アニメーション遷移率取得
	bool GetEndAnime();							// アニメーション終了取得
	void SetAnimeRate(float rate) { m_rate = rate; }		// アニメーション遷移率設定
	void SetAnimeTime(float time) { m_animTime = time; }	// アニメーション再生時間設定
	void SetAnimeLoop(bool flg) { m_bLoop = flg; }			// アニメーションループ設定

	Weapon* GetWeapon() { return m_pWeapon; }	// 武器取得

	void DelLife();								// 体力減少
	void DelStamina(const float& num = 1);		// スタミナ減少

	// ---メンバ変数---
private:
	PlayerStrategy* m_pStrategy[MAX_PLAYER];	// プレイヤー
	PLAYER_STATE m_PlayerState;					// プレイヤー状態

	float m_fSpeed;								// 移動速度

	int m_nLife;								// ライフ
	float m_nStamina;							// スタミナ
	bool m_bAtk;								// 攻撃フラグ
	bool m_bLoop;								// アニメーションループフラグ

	int m_animNo;								// アニメーションNo.
	int m_blendNo;								// ブレンドアニメーション
	float m_animTime;							// 再生時間
	float m_rate;								// ブレンドするためのレート

	Weapon* m_pWeapon;							// 武器
};