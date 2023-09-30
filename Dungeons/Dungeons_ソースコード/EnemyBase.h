#pragma once
// ===敵ベースクラス Enemy.h
// --Author kimura
#include "main.h"
#include "Object3DBase.h"
#include "EnemyStrategy.h"
#include <vector>

// ===構造体定義===
enum ENEMY_STATE		// 敵状態
{
	STAY_ENEMY = 0,
	MOVE_ENEMY,
	ATTACK_ENEMY,
	MAX_ENEMY
};

class EnemyStrategy;
// ---クラス宣言
class EnemyBase : public Object3DBase
{
	// ---メンバ関数---
public:
	EnemyBase();				// コンストラクタ
	~EnemyBase();				// デストラクタ

	virtual void Init() = 0;	// 初期化処理
	virtual void Update();		// 更新処理
	virtual void Draw();		// 描画処理

	void SetNear(bool flg) { bNearFlg = flg; }				// 近距離状態をセット
	void SetInterval(const float& time) { if (m_AttackInterval <= 0.0f) m_AttackInterval = time * 60.0f; }			// 攻撃間隔をセット

	bool IsUpdate();						// 更新可否取得
	void SetStrategy(ENEMY_STATE enemy);	// 状態遷移
	void DelLife(const int& num = 1) { m_nLife -= num; }		// ライフ減少
	float& GetView() { return m_view; }							// 敵視認距離取得
	float& GetInterval() { return m_AttackInterval; }			// 攻撃間隔時間取得

	// ---メンバ変数---
protected:
	EnemyStrategy* m_pStrategy[MAX_ENEMY];		// 敵
	ENEMY_STATE m_State;						// 敵状態
	int m_nLife;								// 敵ライフ
	float m_view;								// 視界サイズ(境界球半径)
	bool bNearFlg;								// 近距離フラグ
	float m_AttackInterval;						// 攻撃タイマー
};
