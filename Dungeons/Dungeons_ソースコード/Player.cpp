// ===プレイヤークラス実装===
// プレイヤークラスを実装
// Author:	Kimura
// ==========================

// ===インクルード===
#include "Player.h"
#include "DebugProc.h"
#include "ModelManager.h"
#include "PlayerStay.h"
#include "PlayerMove.h"
#include "PlayerAttack.h"
#include "PlayerAttack2.h"
#include "PlayerRoll.h"
#include "PlayerRepel.h"
#include "Input.h"
#include "GameManager.h"
#include "DefineData.h"
#include "CollisionManager.h"
#include "Calc.h"
#include "Sound.h"
#include "ShaderList.h"
#include "TextureManager.h"
#include "TimeController.h"
#include "Door.h"
#include "MapManager.h"
#include "EffectManager.h"
#include "Fade.h"
#include "ScoreManager.h"
#include "CameraManager.h"

// ===コンストラクタ===
Player::Player() : Collision3D(this)
{
	// ---初期化処理---
	m_PlayerState = STAY_PLAYER;						// 状態設定

	m_id = OT_PLAYER;									// オブジェクト型設定
	m_transform.Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);		// 初期座標
	m_transform.Rot = XMFLOAT3(0.0f, 0.0f, 0.0f);		// 初期回転
	m_transform.Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);		// 初期拡縮
	m_fRadius = 2.5f;									// 境界球半径
	m_bEnable = true;				// 更新・描画可否
	m_col = true;					// 当たり判定可否
	m_bAtk = false;					// 攻撃状態
	m_bGravity = true;				// 重力
	m_fSpeed = 0.0f;				// 移動速度
	m_bLoop = false;				// アニメーションループ

	// ===モデル関連初期化===
#ifdef _DEBUG
	m_ResourcePass = ("data/model/Character.fbx");
	m_Model = MODEL->Load("data/model/Character.fbx");
#else
	m_ResourcePass = ("data/model/Character.fbx");
	m_Model = MODEL->Load("data/model/Character.fbx");
#endif

	// ===アニメーション追加===
	m_Model->AddAnime("data/model/Idle.fbx");				// 待機アニメーション
	m_Model->AddAnime("data/model/Run.fbx");				// 走りアニメーション
	m_Model->AddAnime("data/model/Roll.fbx");				// 回避アニメーション
	m_Model->AddAnime("data/model/Attack1.fbx");			// 攻撃1アニメーション
	m_Model->AddAnime("data/model/Attack2.fbx");			// 攻撃2アニメーション
	m_Model->AddAnime("data/model/guard.fbx");				// 弾きアニメーション

	// ---アニメーション関連パラメータ初期化---
	m_animNo = ANIM_IDLE;			// アニメーション初期状態
	m_animTime = 0;					// アニメーション再生時間
	m_blendNo = 0;					// ブレンド再生するアニメーション
	m_rate = 0.0f;					// 遷移比率

	// ---ステータス初期化
	m_nLife = 10;					// 体力
	m_nStamina = 50.0f;				// スタミナ
	AddCollisionOBB(XMFLOAT3(0, 1.0f, 0), XMFLOAT3(0.25f,1.0f,0.25f));		// 当たり判定追加
	Init();							// プレイヤー状態初期化

	m_pPS = GetPS(PS_TOON);			// ピクセルシェーダーセット
	m_pVS = GetVS(VS_SKINMESH);		// 頂点シェーダ―セット
	m_pWeapon = new Weapon(this);	// 武器生成

	// COLLISION->SwitchVisibleCollision(this);
}

// ===プレイヤー状態初期化===
void Player::Init()
{
	m_pStrategy[STAY_PLAYER] = new PlayerStay;			// 待機状態生成
	m_pStrategy[MOVE_PLAYER] = new PlayerMove;			// 移動状態生成
	m_pStrategy[ATTACK_PLAYER] = new PlayerAttack;		// 攻撃①状態生成
	m_pStrategy[ROLL_PLAYER] = new PlayerRoll;			// 回避状態生成
	m_pStrategy[ATTACK2_PLAYER] = new PlayerAttack2;	// 攻撃②状態生成
	m_pStrategy[REPEL_PLAYER] = new PlayerRepel;		// 弾き状態生成
}

// ===デストラクタ===
Player::~Player()
{
	// ---終了処理---
	delete m_pWeapon;			// 武器破棄
	// ---状態破棄---
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		delete m_pStrategy[i];	// 状態破棄
	}
}

// ===更新===
void Player::Update()
{
	if (!m_bEnable)				// 更新可否
		return;

	// ---高さ補正---
	if (m_transform.Pos.y <= -3.0f)
	{
		m_transform.Pos.y = 3.0f;
		m_vVel.y = 0.0f;
	}

	// ---体力0でゲームオーバー---
	if (m_nLife <= 0)
	{
		SCENE->SetScene(SCENE_GAMEOVER);	// シーン切替
		return;
	}

	// ---ゲームパッドスティック値取得---
	XMFLOAT2 joyinput = XMFLOAT2(0.0f,0.0f);						// 初期化
	if (CInput::GetJoyCount() > 0)
	{
		joyinput.x = CInput::GetStick(CInput::GetJoyX(JOYSTICKID1));	// X軸入力
		joyinput.y = CInput::GetStick(CInput::GetJoyY(JOYSTICKID1));	// Y軸入力
	}

	// ---状態更新---
	if (m_pStrategy[m_PlayerState]->IsUpdate(this) && SCENE->GetCurrentScene()->GetSceneName() != "TitleScene")
	{
		// ---キー入力取得(操作)
		if (CInput::GetKeyRelease(VK_S) || CInput::GetKeyRelease(VK_D) || CInput::GetKeyRelease(VK_A) || CInput::GetKeyRelease(VK_W))
			SetStrategy(STAY_PLAYER);		// 状態を待機にセット

		// ---移動処理
		if (m_PlayerState == STAY_PLAYER)
		{
			// ---キー入力
			if (CInput::GetKeyPress(VK_A) || CInput::GetKeyPress(VK_W) || CInput::GetKeyPress(VK_S) || CInput::GetKeyPress(VK_D)) 
				SetStrategy(MOVE_PLAYER);	// 状態を移動にセット

			// ---ゲームパッド入力
			if (fabs(joyinput.x) >= DEAD_STICK_X || fabs(joyinput.y) >= DEAD_STICK_Y)
				SetStrategy(MOVE_PLAYER);
		}

		// ---移動以外のアクション
		if (m_PlayerState == STAY_PLAYER || m_PlayerState == MOVE_PLAYER)
		{
			if (CInput::GetKeyTrigger(VK_SPACE) || CInput::GetJoyTrigger(JOYSTICKID1,VB_A))
				SetStrategy(ROLL_PLAYER);	// 回避状態をセット

			else if (CInput::GetMouseTrigger(MOUSEBUTTON_L) || CInput::GetJoyButton(JOYSTICKID1,VB_R))
				SetStrategy(ATTACK_PLAYER);	// 攻撃状態をセット

			else if (CInput::GetMouseTrigger(MOUSEBUTTON_R) || CInput::GetJoyTrigger(JOYSTICKID1,VB_L))
				SetStrategy(REPEL_PLAYER);	// 弾き状態をセット
		}
	}

	// ---更新処理---
	m_pStrategy[m_PlayerState]->Update(this);		// 状態ごとの更新処理
	Object3DBase::Update();							// 座標等更新

	m_bGravity = true;								// 重力セット

	// ---アニメーションの更新---
	if (m_rate > 1.0f)				// 遷移比率が1.0を超えている場合は遷移終了
	{
		m_blendNo = -1;				// ブレンドアニメーションなし
		m_rate = 1.0f;				// 比率を1.0に納める
	}
	else
		m_rate += 0.2f;				// 遷移比率更新

	if (m_animTime * m_Model->GetAnimTick(m_animNo) / 60.0f >= m_Model->GetAnimDuration(m_animNo))		// アニメーション再生時間が最大時間を超えている場合の処理
	{
		if (m_bLoop && m_blendNo == -1)		// ループ再生の場合はリセット
			m_animTime = 0.0f;				// 再生時間を0にセット
	}
	else
	{
		if (m_blendNo == -1)				// アニメーションブレンドなしの場合
			m_animTime += 1.0f;				// 再生時間を更新
	}

	if (m_Model)
	{
		// ---姿勢更新---
		if (m_blendNo != -1)				// ブレンド有の場合
			m_Model->PlayBlendAnime(m_animTime, &m_animNo, m_blendNo, m_rate);		// ブレンド再生
		else
			m_Model->PlayAnime(m_animTime, &m_animNo);		// 通常再生
		if (m_animNo == m_blendNo)
			m_blendNo = -1;							// ブレンドアニメーションと現在再生中のアニメーションが一致していたらブレンドなしにセット
	}
}

// ===描画処理===
void Player::Draw()
{
	// ---モデル描画---
	SetCullMode(CULLMODE_CCW);	// 背面カリング
	ID3D11ShaderResourceView* pTex = TEX->Load("data/texture/ramp.png");	// 諧調テクスチャ取得
	SetTexturePS(pTex, 1);		// 諧調テクスチャセット
	Object3DBase::Draw();		// 描画

	// ---輪郭線描画---
	SetCullMode(CULLMODE_CW);	// 前面カリング
	m_pPS = GetPS(PS_OUTLINE);	// 輪郭線描画シェーダーに切替
	m_pVS = GetVS(VS_OUTLINE);	// 輪郭線描画シェーダーに切替
	Object3DBase::Draw();		// 描画

	// ---描画後処理---
	m_pPS = GetPS(PS_TOON);		// ピクセルシェーダーセット
	m_pVS = GetVS(VS_SKINMESH);	// 頂点シェーダ―セット
	SetCullMode(CULLMODE_CCW);	// 背面カリング
}

// ===更新有無の取得===
bool Player::IsUpdate()
{
	return true;
}

// ===状態をセット===
void Player::SetStrategy(PLAYER_STATE player)
{
	m_PlayerState = player;
	m_pStrategy[m_PlayerState]->Init(this);
}

// ===衝突処理===
void Player::OnCollision(Object* obj)
{
	// ---地面と接触した場合の処理
	if (obj->GetObjID() == OT_GROUND)
	{
		m_bGravity = false;		// 重力なし
		m_vAcc.y = 0.0f;		// y軸加速度リセット
		m_vVel.y = 0.0f;		// y軸速度リセット
	}

	// ---ゴールオブジェクトと接触した場合の処理
	if (obj->GetObjID() == OT_GOAL)
	{
		SCENE->SetScene(SCENE_LOAD);
	}

	// ---ドアと接触した場合の処理
	if (obj->GetObjID() == OT_DOOR)
	{
		// ---例外処理---
		if (!MAP->GetClear())	// クリアしていない状態の場合は何も起こらない
			return;

		// ---ステージの切替---
		int dir = ((Door*)obj)->GetDirection();			// 当たったドアの方向を取得
		MAP->ChangeRoom(dir);							// その方向の部屋に切替
		dir = dir ^ 0x2;								// 2ビット目を反転(奥のドアに触れた場合は次の部屋の手前のドアに移動するため)
		m_bEnable = true;								// 再度表示

		// ---ステージ切替後にプレイヤーの座標をセット---
		std::vector<Object*> doors = SCENE->GetCurrentScene()->FindObjectListWithTag(OT_DOOR);		// シーンに配置されたドアを取得
		auto it = doors.begin();						// ドアの先頭イテレータ

		while (it != doors.end())		// 終端イテレーターまで
		{
			if (((Door*)*it)->GetDirection() == dir)	// 接触したドアの方向と一致する場合は座標をそのドアの目の前にセット
			{
				m_transform.Pos = ((Door*)*it)->GetEntrance();	// 座標をドアの前にセット
				break;
			}
			++it;		// イテレーターをずらす
		}
	}
}

// ===めり込み量を戻す===
void Player::ReflectMovement()
{
	XMFLOAT3 movement = GetMovement();		// 移動量を取得

	m_transform.Pos.x += movement.x;		// x方向移動
	m_transform.Pos.y += movement.y;		// y方向移動
	m_transform.Pos.z += movement.z;		// z方向移動
}

// ===アニメーション終了状態取得===
bool Player::GetEndAnime()
{
	if (m_animTime * m_Model->GetAnimTick(m_animNo) / 60.0f >= m_Model->GetAnimDuration(m_animNo))		// 再生時間が最大時間を超えていたら終了
		return true;
	return false;
}

// ===ライフ更新===
void Player::DelLife()
{
	if (m_PlayerState != ROLL_PLAYER)	// ダメージ
	{
		m_nLife--;														// ライフ減少
		m_nInvincibleTime = 100;										// 無敵時間セット
		TIME->SetStop(0.3f);											// ヒットストップ
		CSound::Play(SE_DEFEND);										// ダメージSE再生
		EFFECT->Play(EXPLOSION_EFFECT, GetHitPos());					// エフェクトを衝突した位置に再生
		((GameCamera*)CAMERA->GetCurrentCamera())->Shake(0.5f);			// カメラ振動
	}
	else								// 回避
	{
		m_nInvincibleTime = 10;						// 無敵時間セット
		TIME->SetSlow(0.3f);						// スロー演出
		CSound::Play(SE_AVOID);						// 回避SE再生
		EFFECT->Play(AVOID_EFFECT, GetHitPos());	// エフェクトを衝突した位置に再生
	}
}

// ===スタミナ更新===
void Player::DelStamina(const float& num)
{
	m_nStamina -= num;			// スタミナ更新
	if (m_nStamina >= 50)		// 値を範囲内に納める
		m_nStamina = 50;		// 最大スタミナの値にする
	if (m_nStamina <= 0)		// 値を範囲内に納める
		m_nStamina = 0;			// 0以下の場合は0
}