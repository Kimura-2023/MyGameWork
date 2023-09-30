// ===通常敵クラス実装===
// 通常敵クラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "EnemyNormal.h"
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
#include "EnemyStay.h"
#include "ScoreManager.h"
#include "ScoreEffect.h"
#include "CameraManager.h"
#include "GameCamera.h"

// ===コンストラクタ===
EnemyNormal::EnemyNormal() : Collision3D(this)
{
	// ---初期化処理---
	m_id = OT_ENEMY;											// オブジェクトの型を設定
	m_transform.Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);				// 初期座標
	m_transform.Rot = XMFLOAT3(0.0f, 0.0f, 0.0f);				// 初期回転
	m_transform.Scale = XMFLOAT3(0.5f, 0.5f, 0.5f);				// 初期拡縮
	m_fRadius = 2.5f;											// 境界球サイズ
	m_bEnable = true;											// 更新・描画可否
	m_col = true;												// 当たり判定可否
	m_bGravity = true;											// 重力可否
	m_view = 5.0f;												// プレイヤー視認距離
	m_ResourcePass = ("data/model/Enemy1.fbx");					// モデル素材へのパス
	m_Model = MODEL->Load("data/model/Enemy1.fbx");				// モデル読込
	m_nLife = 3;												// 体力
	AddCollisionSphere(XMFLOAT3(0.0f, 0.0f, 0.0f), 1.0f);		// 当たり判定追加
	Init();														// 初期化処理

	m_pPS = GetPS(PS_TOON);										// ピクセルシェーダーセット
	m_pVS = GetVS(VS_OBJECT);									// 頂点シェーダーセット
	m_State = MOVE_ENEMY;										// 待機状態に設定
	bNearFlg = false;											// 近距離判定セット
}

// ===初期化処理===
void EnemyNormal::Init()
{
	m_pStrategy[STAY_ENEMY] = new EnemyStay;			// 待機状態生成
	m_pStrategy[MOVE_ENEMY] = new EnemyMove;			// 移動状態生成
	m_pStrategy[ATTACK_ENEMY] = new EnemyAttack;		// 攻撃状態生成
}

// ===デストラクタ===
EnemyNormal::~EnemyNormal()
{
}

// ===更新処理===
void EnemyNormal::Update()
{
	// ---座標補正---
	if (m_transform.Pos.y <= -5.0f)
		m_transform.Pos.y = 1.0f;

	// ---更新判定---
	if (!m_bEnable)
		return;

	// ---死亡判定---
	if (m_nLife <= 0)
	{
		m_bEnable = false;												// 更新・描画可否状態をセット
		CSound::Play(SE_EXPLOSION);										// 効果音再生
		EFFECT->Play(ENEMY_DEAD_EFFECT,m_transform.Pos);				// エフェクト再生
		m_AttackInterval = 0.0f;										// 攻撃間隔リセット
		((GameCamera*)CAMERA->GetCurrentCamera())->Shake(0.3f);			// カメラ振動

		Object* score = SCENE->GetCurrentScene()->CreateGameObject("ScoreEff", BILL_SCORE);			// スコアエフェクト生成
		score->SetPos(XMFLOAT3(m_transform.Pos.x, m_transform.Pos.y + 1.0f, m_transform.Pos.z));						// 座標をセット
		score->UpdateMatrix();											// 行列更新
		((ScoreEffect*)score)->SetScore(10);							// スコアを設定
		SCORE->Add(10);													// スコア更新
		m_nLife = 3;													// ライフ初期化
		return;
	}

	// ---攻撃間隔判定---
	if (m_AttackInterval > 0.0f)
		--m_AttackInterval;					// 攻撃間隔時間減少
	else
		m_AttackInterval = 0.0f;			// 最小値

	// ---各更新処理---
	m_pStrategy[m_State]->Update(this);		// 状態ごとの更新
	m_transform.Rot.z += 0.1f;				// 徐々に回転
	Object3DBase::Update();					// オブジェクト更新
	m_bGravity = true;						// 重力有効
}

// ===描画処理===
void EnemyNormal::Draw()
{
	EnemyBase::Draw();						// 描画
}

// ===衝突処理===
void EnemyNormal::OnCollision(Object* obj)
{
	// ---プレイヤーと衝突---
	if (obj->GetObjID() == OT_PLAYER)
	{
		if (!obj->GetInvincible() && m_State == ATTACK_ENEMY)			// 攻撃状態の場合
		{
			((Player*)obj)->DelLife();			// ライフ減少
		}
	}

	// ---地面と衝突---
	if (obj->GetObjID() == OT_GROUND)
	{
		XMFLOAT3 movement = GetMovement();		// 移動量取得
		m_transform.Pos.y += movement.y;		// y座標補正
		m_vAcc.y = 0.0f;
		m_vVel.y = 0.0f;		
		m_bGravity = false;						// 重力無効化
	}

	// ---敵と衝突---
	if (obj->GetObjID() == OT_ENEMY || obj->GetObjID() == OT_PILLAR)
	{
		XMFLOAT3 movement = GetMovement();
		m_transform.Pos.x += movement.x;
		m_transform.Pos.y += movement.y;
		m_transform.Pos.z += movement.z;
	}

	// ---壁またはドアと衝突----
	if (obj->GetObjID() == OT_WALL || obj->GetObjID() == OT_DOOR)
	{
		XMFLOAT3 movement = GetMovement();
		m_transform.Pos.x += movement.x;
		m_transform.Pos.y += movement.y;
		m_transform.Pos.z += movement.z;
		SetStrategy(MOVE_ENEMY);
	}
}