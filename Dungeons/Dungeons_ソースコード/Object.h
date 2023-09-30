#pragma once
// ===オブジェクトベースクラス定義===
// オブジェクトベースクラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "main.h"
#include "Calc.h"
#include <string>
#include "Line.h"

// ===オブジェクトのタグ定義===
//	※ファイルにタグの情報も吐き出すため、オブジェクトを追加する際は下に足していく(間にタグが追加されると元々の定義からその分後ろにずれてしまう)
enum ObjType {			// オブジェクト型
	OT_OBJ = 0,			// 型無し
	OT_PLAYER,			// プレイヤー
	OT_ENEMY,			// 敵
	OT_GROUND,			// 地面
	OT_WALL,			// 壁
	OT_PILLAR,			// 柱
	BG_3DOBJ,			// 背景オブジェクト
	TEST_2DOBJ,			// テストオブジェクト
	BG_TITLE,			// タイトルロゴ
	BG_ENTER,			// UI
	OT_FENCE,			// フェンス
	PLAYER_ATACK,		// プレイヤーの攻撃当たり判定
	OT_WALLPILLAR,		// 凸凹壁
	ENEMY_ATTACK,		// 敵の攻撃判定
	OT_DOOR,			// ドア
	OT_ENEMY_BULLET,	// 遠距離敵
	UI_START_BUTTON,	// ボタンテスト
	UI_TITLE_BUTTON,	// タイトルボタン
	UI_FIN_BUTTON,		// 終了ボタン
	UI_LIFE,			// 体力UI
	UI_STAMINA,			// スタミナUI
	BG_GAMEOVER,		// ゲームオーバー背景
	BG_GAMECLEAR,		// ゲームクリア背景
	OT_GOAL,			// ゴールオブジェクト
	UI_SCORE,			// スコアUI
	BILL_SCORE,			// スコアエフェクト

	MAX_GAMEOBJTYPE
};

// ===クラス定義===
class Object
{
	// ---メンバ関数---
public:
	Object();				// コンストラクタ
	virtual ~Object();		// デストラクタ

	virtual void DrawShadow() { ; }		// 影描画

	// ---値のセット---
	void SetEnable(bool);		// 更新の有無をセット
	void SetGravity(bool);		// 重力の有無をセット
	void SetCol(bool);
	void SetPos(XMFLOAT3);
	void SetRot(XMFLOAT3);
	void SetScl(XMFLOAT3);
	void SetVel(XMFLOAT3);
	void SetAcc(XMFLOAT3);
	void SetRad(float);
	void SetName(std::string);
	void SetStatic(bool);
	void SetSelect(bool);
	void SetTransform(Transform);
	void SetWorld(const XMFLOAT4X4&);

	void SetInvincible(const int& time);

	virtual bool IsUpdate() = 0;	// 更新したかどうか
	virtual void SetResource(std::string) = 0;
	virtual void OnCollision(Object*) { ; }

	// ---値の取得---
	Transform& GetTransform() { return m_transform; }				// オブジェクト座標等情報取得
	XMFLOAT3& GetPos() { return m_transform.Pos; }					// 座標取得
	XMFLOAT3& GetScale() { return m_transform.Scale; }				// 拡縮取得
	XMFLOAT3& GetRotate() { return m_transform.Rot; }				// 回転取得
	Quaternion& GetQuaternion() { return m_qRot; }					// 回転(クォータニオン取得)
	bool& GetEnable() { return m_bEnable; }							// 取得
	bool& GetGravity() { return m_bGravity; }						// 重力取得
	bool& GetVisible() { return m_bVisible; }						// 可視状態取得
	bool& GetCol() { return m_col; }								// 当たり判定可否
	bool& GetStatic() { return m_bStatic; }							// オブジェクト状態取得
	bool& GetSelect() { return m_bSelect; }							// セレクト状態取得
	float& GetRad() { return m_fRadius; }							// 境界球半径取得
	XMFLOAT3& GetVel() { return m_vVel; }							// 速度取得
	XMFLOAT3& GetAcc() { return m_vAcc; }							// 加速度取得
	ObjType GetObjID() { return m_id; }								// オブジェクトタイプ取得
	XMFLOAT4X4& GetWorld() { return m_mWorld; }						// ワールドマトリックス取得
	std::string GetName() { return m_name; }						// 名前取得
	std::string GetResourcePass() { return m_ResourcePass; }		// リソースへのパスを取得

	bool GetInvincible() { return m_nInvincibleTime > 0; }			// 無敵時間中かどうか
	int& GetInvincibleTime() { return m_nInvincibleTime; }

	virtual void UpdateMatrix() = 0;								// ワールドマトリクス更新

	virtual void ImGuiWindow() { ; }

	// ---メンバ変数---
protected:
	ObjType m_id;					// オブジェクトid

	Transform m_transform;			// 座標、拡縮、回転
	Quaternion m_qRot;
	XMFLOAT3 m_vVel;				// 速度
	XMFLOAT3 m_vAcc;				// 加速度

	bool m_bGravity;				// 重力フラグ
	bool m_bEnable;					// 存在するか(falseで更新、描画停止)
	bool m_bVisible;				// 見えるかどうか(trueで可視)
	bool m_col;						// 当たり判定を取るかどうか
	bool m_bStatic;					// 動くオブジェクトかどうか
	bool m_bSelect;					// エディタ上で選択されているかどうか

	float m_fRadius;				// 境界球サイズ

	XMFLOAT4X4 m_mWorld;			// ワールド変換マトリックス

	std::string m_name;				// オブジェクト名
	std::string m_ResourcePass;		// モデルや画像などのリソースパス

	int m_nInvincibleTime;			// 無敵時間
};

// ===関数定義===
LPCSTR GetObjectTagName(ObjType);	// エディタ上で表示するオブジェクトの型名取得