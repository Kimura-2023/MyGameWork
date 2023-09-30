// ===当たり判定(OBB)クラス実装===
// 当たり判定(OBB)クラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "ColSphere.h"
#include "Light.h"
#include "ModelManager.h"
#include "DebugProc.h"
#include "ColOBB.h"

// ===静的メンバ===
CSphere* CollSphere::m_pBSphere = nullptr;					// デバッグ表示用メッシュ
int CollSphere::m_nCnt = 0;									// 境界球総数

// ===境界球当たり判定コンストラクタ(オフセットなし)===
CollSphere::CollSphere(Collision3D* col, float rad)
{
	// ---初期化処理---
	Init();													// 初期化
	m_Radius = rad;											// 半径をセット
	SetParent(col);											// 親をセット
	HRESULT hr = m_pBSphere->Init(32, 16, m_Radius);		// デバッグ表示初期化

	// ---例外処理---
	if (FAILED(hr))
	{
		m_pBSphere->Fin();							// デバッグ表示終了処理
		SAFE_DELETE(m_pBSphere);					// 解放
		return;
	}

	m_pBSphere->SetMaterial(&m_Material);			// メッシュ色設定
}

// ===境界球当たり判定コンストラクタ(オフセットあり)===
CollSphere::CollSphere(Collision3D* col, XMFLOAT3 off, float rad)
{
	// ---初期化処理---
	Init();													// 初期化
	m_Offset = off;											// 中心からのオフセットをセット
	m_Radius = rad;											// 半径をセット
	SetParent(col);											// 親をセット
	HRESULT hr = m_pBSphere->Init(32, 16, m_Radius);		// デバッグ表示初期化

	// ---例外処理---
	if (FAILED(hr))
	{
		m_pBSphere->Fin();							// デバッグ表示終了処理
		SAFE_DELETE(m_pBSphere);					// 解放
		return;
	}

	m_pBSphere->SetMaterial(&m_Material);			// メッシュ色設定
}

// ===デストラクタ===
CollSphere::~CollSphere()
{
	// ---終了処理---
	m_nCnt--;										// 境界球当たり判定総数減少
	if (m_nCnt <= 0)
	{
		m_pBSphere->Fin();							// デバッグ表示終了
		SAFE_DELETE(m_pBSphere);					// 解放
	}
}

// ===初期化処理===
void CollSphere::Init()
{
	// ---初期化---
	m_Radius = 1.0f;								// 初期半径
	m_coltype = COL_SPHERE;							// 当たり判定の型設定
	if (m_nCnt == 0)
	{
		m_pBSphere = new CSphere();					// デバッグ表示生成
	}
	m_nCnt++;										// 境界球当たり判定総数増加
}

// ===ヒット確認===
bool CollSphere::CheckHit(CollisionBase* col)
{
	// ---除外判定---
	if (col->GetFollowCollision() == GetFollowCollision())
		return false;
	if (!col->GetEnable())
		return false;

	// ---値の取得---
	XMFLOAT3 pos = col->GetCenter();											// 相手座標
	XMFLOAT3 vec = { pos.x - m_Pos.x, pos.y - m_Pos.y, pos.z - m_Pos.z };		// 相手座標へのベクトル

	// ---当たり判定---
	switch (col->GetColType())
	{
	case COL_SPHERE: {														// ---境界球同士の当たり判定
		// ---値の取得---
		float rad = ((CollSphere*)col)->GetRadius();			// 相手半径取得
		float length;											// 相手との距離
		Length(&length, m_Pos, pos);							// 距離を算出

		// ---衝突時---
		if (length <= (m_Radius + rad))
		{
			// ---戻し量の計算---
			if (!col->GetFollowCollision()->GetCollider())
			{
				XMFLOAT3 normal(m_Pos.x - pos.x, m_Pos.y - pos.y, m_Pos.z - pos.z);		// 戻す方向は中心座標間のベクトルとする
				Normalize(&normal);														// 正規化
				length = m_Radius + rad - length;										// お互いの半径を足した距離から中心座標間の距離を引いてめり込み量を算出

				// ---長さを反映---
				normal.x *= length;
				normal.y *= length;
				normal.z *= length;

				// ---戻し量の設定---
				m_pParent->CalcMovement(&normal);	// 反映

				m_bHit = true;						// ヒット
			}
			return true;
		}
		// ---非衝突時---
		else
		{
			m_bHit = false;
		}
		break; 
	}
	case COL_OBB: {															// ---OBBとの判定
		// ---値の取得---
		float length;
		((CollOBB*)col)->CalcLenOBBtoPoint(&length, m_Pos);					// 中心座標との最短距離を求める

		// ---衝突時---
		if (length <= m_Radius)
		{
			m_bHit = true;

			//---戻し量の計算---
			if (!col->GetFollowCollision()->GetCollider())
			{
				XMFLOAT3 work;
				CollOBB::CalcOBBtoPoint(&work, m_Pos, (CollOBB*)col);						// 衝突した点を求める
				XMFLOAT3 normal(m_Pos.x - work.x, m_Pos.y - work.y, m_Pos.z - work.z);		// 衝突した点の座標からめり込み量を戻す方向を求める
				Normalize(&normal);						// 正規化

				Length(&length, work, m_Pos);			// 衝突した点から境界球の中心座標の距離を求める
				length = m_Radius - length;				// 求めた距離からめり込み量を求める

				// ---長さを反映---
				normal.x *= length;
				normal.y *= length;
				normal.z *= length;

				// ---戻し量の設定---
				m_pParent->CalcMovement(&normal);		// めり込み量を反映
				m_pParent->SetHitPos(&work);			// 最近接点をセット
			}

			return true;
		}

		// ---非衝突時---
		else
			m_bHit = false;
		break;
	}
	case COL_NONE:
		break;
	default:
		break;
	}
	return false;
}

// ===デバッグ表示===
void CollSphere::Draw()
{
	// ---除外処理---
	if (!GetVisible())
		return;

	// ---描画処理---
	m_pBSphere->SetMaterial(&m_Material);				// 色を設定
	m_pBSphere->SetWorld(&GetWorld());					// ワールド変換行列セット
	m_pBSphere->Draw();									// 描画
}

// ===境界球半径取得===
float CollSphere::GetRadius()
{
	return m_Radius;
}