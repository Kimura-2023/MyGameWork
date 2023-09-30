#pragma once
// ===敵ベースクラス Enemy.h
// --Author kimura
#include "main.h"
#include "Animation2DBase.h"
#include "EnemyState.h"
#include "Collision3D.h"

// ===構造体定義===
enum ENEMY2D_STATE		// 敵状態
{
	MOVING_ENEMY = 0,	// 移動
	DAMAGE_ENEMY,		// ダメージ

	MAX_ENEMY
};

class EnemyState;
// ---クラス宣言
class Enemy : public Animation2DBase, Collision3D
{
public:
	Enemy();
	Enemy(XMFLOAT3);

	~Enemy();

	void Init();
	void Update();
	void Draw();

	bool IsUpdate();
	void SetState(ENEMY2D_STATE state);
	void OnCollision(Object*);
private:
	EnemyState* m_pState[MAX_ENEMY];		// プレイヤー
	ENEMY2D_STATE m_EnemyState;					// プレイヤー状態
	XMFLOAT3 oldPos;

	int m_nLife;								// ライフ
	bool m_bAtk;								// 攻撃フラグ
};