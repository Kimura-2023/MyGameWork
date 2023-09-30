// ===通常敵攻撃クラス実装===
// 通常敵攻撃クラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "EnemyAttack.h"
#include "Player.h"
#include "SceneManager.h"
#include "Calc.h"
#include "EffectManager.h"
#include "Sound.h"

// ===初期化処理===
void EnemyAttack::Init(EnemyBase* enemy)
{
	m_vStart = enemy->GetPos();					// 攻撃開始地点をセット
	XMFLOAT3 vec;								// 進行方向
	Object* player = SCENE->GetCurrentScene()->FindGameObject("Player");		// プレイヤー取得

	// ---例外処理---
	if (!player)
	{
		enemy->SetVel(XMFLOAT3(0.0f, 0.0f, 0.0f));
		return;
	}

	// ---速度セット---
	vec.x = player->GetPos().x - enemy->GetPos().x;
	vec.z = player->GetPos().z - enemy->GetPos().z;
	vec.y = 0.0f;

	Normalize(&vec);
	vec.x *= 0.1f;
	vec.z *= 0.1f;

	enemy->SetVel(vec);			// 速度セット
	enemy->SetFront(vec);		// 正面方向をセット
	enemy->SetNear(true);		// 近距離フラグセット
	CSound::Play(SE_ENEMY);		// 攻撃音再生
}

// ===更新処理===
void EnemyAttack::Update(EnemyBase* enemy)
{
	// ---距離を求める
	float length;
	Length(&length, enemy->GetPos(),m_vStart);


	EFFECT->Play(ENEMY_EFFECT, enemy->GetPos());		// エフェクト発生

	// ---徐々に回転
	XMFLOAT3 rot = enemy->GetRotate();
	enemy->SetRot(XMFLOAT3(rot.x, rot.y, rot.z + 2.0f));

	// ---一定距離以上進行したら状態遷移
	if (length >= 10.0f)
	{
		enemy->SetStrategy(MOVE_ENEMY);			// 移動状態に遷移
		enemy->SetInterval(2.0f);				// インターバルをセット
	}

}