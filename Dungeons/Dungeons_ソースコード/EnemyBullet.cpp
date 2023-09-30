// ===遠距離敵クラス実装===
// 遠距離敵クラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "EnemyBullet.h"
#include "EnemyMove.h"
#include "DebugProc.h"
#include "ModelManager.h"
#include "ObjectManager.h"
#include "ShaderList.h"
#include "TextureManager.h"
#include "Player.h"
#include "TimeController.h"
#include "Sound.h"
#include "SceneManager.h"
#include "EnemyAttack.h"
#include "EffectManager.h"
#include "BulletEnemyAttack.h"
#include "EnemyStay.h"
#include "CollisionManager.h"
#include "Bullet.h"
#include "ScoreManager.h"
#include "ScoreEffect.h"
#include "GameCamera.h"
#include "CameraManager.h"

// ===コンストラクタ===
EnemyBullet::EnemyBullet() : Collision3D(this)
{
	// ---初期化処理---
	m_id = OT_ENEMY_BULLET;									// オブジェクトの型を設定
	m_transform.Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);			// 初期座標
	m_transform.Rot = XMFLOAT3(0.0f, 0.0f, 0.0f);			// 初期回転
	m_transform.Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);			// 初期拡縮
	m_fRadius = 2.5f;										// 境界球サイズ
	m_bEnable = true;										// 更新・描画可否
	m_col = true;											// 当たり判定可否
	m_bGravity = true;										// 重力可否
	m_view = 8.0f;											// プレイヤー視認距離
	m_ResourcePass = ("data/model/Enemy2.fbx");				// モデル素材へのパス
	m_Model = MODEL->Load("data/model/Enemy2.fbx");			// モデル読込
	m_nLife = 3;											// 体力
	AddCollisionOBB(XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(1.0f,1.0f,1.0f));	// 当たり判定追加
	Init();													// 状態初期化

	m_pPS = GetPS(PS_TOON);									// ピクセルシェーダーセット
	m_pVS = GetVS(VS_OBJECT);								// 頂点シェーダーセット
	m_State = STAY_ENEMY;									// 待機状態に設定
	bNearFlg = false;										// 近距離判定セット
	m_pBullets = new Bullet[MAX_BULLET];					// 弾生成
}

// ===初期化処理===
void EnemyBullet::Init()
{
	m_pStrategy[STAY_ENEMY] = new EnemyStay;				// 待機状態生成
	m_pStrategy[MOVE_ENEMY] = new EnemyMove;				// 移動状態生成
	m_pStrategy[ATTACK_ENEMY] = new BulletEnemyAttack;		// 攻撃状態生成
}

// ===デストラクタ===
EnemyBullet::~EnemyBullet()
{
	delete[] m_pBullets;		// 弾解放
}

// ===更新処理===
void EnemyBullet::Update()
{
	if (!m_bEnable)							// 更新判定
		return;

	// ---死亡判定---
	if (m_nLife <= 0)
	{
		m_bEnable = false;									// 更新しない
		CSound::Play(SE_EXPLOSION);							// 効果音再生
		EFFECT->Play(ENEMY_DEAD_EFFECT, m_transform.Pos);	// エフェクト再生
		m_AttackInterval = 0.0f;							// 攻撃タイマーをリセット
		m_nLife = 3;										// ライフ初期化
		Object* score = SCENE->GetCurrentScene()->CreateGameObject("ScoreEff", BILL_SCORE);							// スコアエフェクト生成
		score->SetPos(XMFLOAT3(m_transform.Pos.x,m_transform.Pos.y + 1.0f,m_transform.Pos.z));						// 座標をセット
		((ScoreEffect*)score)->SetScore(20);				// スコアを設定
		SCORE->Add(20);		// スコア更新
		((GameCamera*)CAMERA->GetCurrentCamera())->Shake(0.3f);			// カメラ振動
		return;
	}

	// ---攻撃間隔更新---
	if (m_AttackInterval > 0.0f)
		--m_AttackInterval;			// 減少
	else
		m_AttackInterval = 0.0f;	// 最小値

	// ---距離によって判定---
	float length = 0;				// 距離
	Object* player = SCENE->GetCurrentScene()->FindGameObject("Player");		// プレイヤー取得
	if (player)
	{
		Length(&length, player->GetPos(), m_transform.Pos);			// プレイヤーとの距離を求める
		if (m_view >= length)				// 視野の値より近かった場合は攻撃
		{
			SetStrategy(ATTACK_ENEMY);		// 攻撃状態に遷移
		}
		else
		{
			SetStrategy(STAY_ENEMY);		// 待機状態に遷移
		}
	}

	m_pStrategy[m_State]->Update(this);		// 状態ごとの更新処理
	Object3DBase::Update();					// オブジェクトの更新
	m_bGravity = true;						// 重力有効化
}

// ===描画処理===
void EnemyBullet::Draw()
{
	EnemyBase::Draw();						// 描画
}

// ===衝突処理===
void EnemyBullet::OnCollision(Object* obj)
{
	XMFLOAT3 movement = GetMovement();		// 移動量

	// ---地面と衝突時---
	if (obj->GetObjID() == OT_GROUND)
	{
		m_transform.Pos.y = 0.0f;
		m_vAcc.y = 0.0f;
		m_vVel.y = 0.0f;
		m_bGravity = false;
	}

	// ---柱と衝突時---
	if (obj->GetObjID() == OT_PILLAR)
	{
		m_transform.Pos.x += movement.x;
		m_transform.Pos.y += movement.y;
		m_transform.Pos.z += movement.z;
	}

	// ---遠距離敵と衝突時---
	if (obj->GetObjID() == OT_ENEMY_BULLET)
	{
		m_transform.Pos.x += movement.x;
		m_transform.Pos.y += movement.y;
		m_transform.Pos.z += movement.z;
	}
}

// ===弾発射===
void EnemyBullet::FireBullet()
{
	for (int i = 0; i < MAX_BULLET; ++i)
	{
		if (m_pBullets[i].Fire(XMFLOAT3(m_transform.Pos.x, m_transform.Pos.y + 1, m_transform.Pos.z), m_vFront, 0.3f))
			return;
	}
}