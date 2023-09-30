#pragma once

// ===当たり判定クラス定義===
// 当たり判定クラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "main.h"
#include "Object.h"
#include "Collision3D.h"
#include "Mesh.h"

// ===当たり判定種別===
enum COLTYPE
{
	COL_SPHERE,				// 境界球
	COL_AABB,				// AABB
	COL_OBB,				// OBB
	COL_LINE,				// 線分
	COL_NONE,
};

// ===クラス定義===
class Collision3D;
class CollisionBase
{
	// ---メンバ関数---
public:
	CollisionBase();				// コンストラクタ
	virtual ~CollisionBase();		// デストラクタ
	virtual void Init();			// 初期化

	Collision3D* GetFollowCollision(void);		// 親当たり判定取得
	XMFLOAT3& GetCenter();						// 中心座標取得
	XMFLOAT4X4& GetWorld();						// ワールド行列取得
	bool GetEnable();							// Enable
	bool GetVisible();							// デバッグ表示
	bool GetHit();								// 衝突状態取得
	COLTYPE GetColType();						// 当たり判定種類取得

	void Update();								// 更新処理
	virtual void Draw() = 0;					// 描画処理
	virtual bool CheckHit(CollisionBase *) = 0;	// 当たったかどうか

	void SetParent(Collision3D*);	// 親をセット
	void SetEnable(bool);			// Enableセット
	void SetVisible(bool);			// 可視セット
private:
	void UpdateMatrix();			// ワールドマトリックス更新

	// ---メンバ変数---
protected:
	COLTYPE m_coltype;				// 当たり判定種類
	XMFLOAT3 m_Pos;					// 当たり判定計算用座標
	XMFLOAT3 m_Scale;				// 当たり判定計算用サイズ
	XMFLOAT3 m_Rot;					// 当たり判定回転量
	XMFLOAT3 m_Offset;				// オブジェクト中心座標からのずれ
	bool m_Enable;					// 存在
	bool m_Visible;					// 可視状態
	bool m_bHit;					// 衝突状態
	CMeshMaterial m_Material;		// デバッグ表示のマテリアル情報
	Collision3D* m_pParent;			// 親当たり判定
private:

	XMFLOAT4X4 m_World;		// ワールド行列
};
