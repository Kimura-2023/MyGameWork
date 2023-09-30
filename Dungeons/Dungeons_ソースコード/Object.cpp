#include "Object.h"

// ===オブジェクト型名定義===
namespace ObjectTag
{
	LPCSTR tag[MAX_GAMEOBJTYPE] = {					// エディタ上で表示するオブジェクト種類名
		"GameObject",		// 型なし
		"Player",			// プレイヤー
		"Enemy",			// エネミー
		"Floor",			// 地面
		"Wall",				// 壁
		"Pillar",			// 柱
		"3DModel",			// 背景オブジェクト
		"2DGameObject",		// 2Dオブジェクト
		"TitleBG",			// タイトルロゴ
		"EnterButton",		// ボタン
		"Fence",			// フェンス
		"None",				// プレイヤー武器
		"WallPillar",		// 壁柱
		"Bullet",			// 弾
		"Door",				// ドア
		"BulletEnemy",		// 遠距離エネミー
		"StartButton",		// スタートボタン
		"TitleButton",		// タイトルボタン
		"FinButton",		// 終了ボタン
		"Life",				// ライフ
		"Stamina",			// スタミナ
		"GameOver",			// ゲームオーバー画面
		"GameClear",		// ゲームクリア画面
		"Goal",				// ゴールオブジェクト
		"Score",			// スコアUI
		"ScoreEff",			// スコアエフェクト
	};
}

// ===コンストラクタ===
Object::Object() : m_name("オブジェクト"),m_ResourcePass("")
{
	// ---初期化処理---
	m_id = OT_OBJ;			// オブジェクトid

	m_transform = Transform();								// 座標等初期化
	m_qRot = Quaternion(XMFLOAT3(0.0f, 1.0f, 0.0f), 0.0f);	// 回転
	m_vVel = XMFLOAT3(0.0f, 0.0f, 0.0f);					// 速度
	m_vAcc = XMFLOAT3(0.0f, 0.0f, 0.0f);					// 加速度

	m_bGravity = false;										// 重力フラグ
	m_bEnable = false;										// 更新・描画有無(falseで更新、描画停止)
	m_bVisible = false;										// 見えるかどうか(trueで可視)
	m_col = false;											// 当たり判定を取るかどうか
	m_bStatic = false;										// 静的オブジェクトかどうか
	m_bSelect = false;										// 選択されているかどうか

	m_fRadius = 1.0f;										// 境界球サイズ

	XMStoreFloat4x4(&m_mWorld, XMMatrixIdentity());			// ワールドマトリックス初期化
	m_nInvincibleTime = 0;									// 無敵時間
}

// ===デストラクタ===
Object::~Object()
{
}

// ===更新・描画有無設定===
void Object::SetEnable(bool flg)
{
	m_bEnable = flg;
}

// ===重力可否設定===
void Object::SetGravity(bool flg)
{
	m_bGravity = flg;
}

// ===当たり判定可否設定===
void Object::SetCol(bool flg)
{
	m_col = flg;
}

// ===オブジェクト座標設定===
void Object::SetPos(XMFLOAT3 pos)
{
	m_transform.Pos = pos;
}

// ===オブジェクト回転設定===
void Object::SetRot(XMFLOAT3 rot)
{
	m_transform.Rot = rot;
}

// ===オブジェクト拡縮設定===
void Object::SetScl(XMFLOAT3 scl)
{
	m_transform.Scale = scl;
}

// ===オブジェクト速度設定===
void Object::SetVel(XMFLOAT3 vel)
{
	m_vVel = vel;
}

// ===オブジェクト加速度設定===
void Object::SetAcc(XMFLOAT3 acc)
{
	m_vAcc = acc;
}

// ===オブジェクト境界球サイズ設定===
void Object::SetRad(float r)
{
	m_fRadius = r;
}

// ===オブジェクト名設定===
void Object::SetName(std::string name)
{
	m_name = name;
}

// ===オブジェクト選択状態設定===
void Object::SetSelect(bool flg)
{
	m_bSelect = flg;
}

// ===静的オブジェクト設定===
void Object::SetStatic(bool flg)
{
	m_bStatic = flg;
}

// ===座標・回転・拡縮設定===
void Object::SetTransform(Transform transform)
{
	m_transform = transform;
}

// ===ワールド変換行列設定===
void Object::SetWorld(const XMFLOAT4X4& mtx)
{
	m_mWorld = mtx;
}

// ===オブジェクト型名取得===
LPCSTR GetObjectTagName(ObjType type)
{
	return ObjectTag::tag[type];
}

// ===オブジェクト無敵時間設定===
void Object::SetInvincible(const int& time)
{
	m_nInvincibleTime = time;
}