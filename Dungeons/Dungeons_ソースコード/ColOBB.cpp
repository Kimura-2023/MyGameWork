// ===当たり判定(OBB)クラス実装===
// 当たり判定(OBB)クラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "ColOBB.h"
#include "Light.h"
#include "ModelManager.h"
#include "DebugProc.h"
#include "ColSphere.h"

// ===静的メンバ===
CBox* CollOBB::m_pBox = nullptr;			// デバッグ表示用メッシュ
int CollOBB::m_nCnt = 0;					// 総数

// ===境界球当たり判定コンストラクタ(オフセットなし)===
CollOBB::CollOBB(Collision3D* col, XMFLOAT3 size)
{
	// ---初期化---
	Init();														// 初期化
	m_Scale = size;												// サイズセット
	SetParent(col);												// 親をセット
	HRESULT hr = m_pBox->Init(XMFLOAT3(1.0f,1.0f,1.0f));		// デバッグ表示初期化

	// ---例外処理---
	if (FAILED(hr))
	{
		m_pBox->Fin();						// デバッグ表示終了処理
		SAFE_DELETE(m_pBox);				// デバッグ表示解放
		return;
	}

	m_pBox->SetMaterial(&m_Material);		// デバッグ表示色設定
}

// ===境界球当たり判定コンストラクタ(オフセットあり)===
CollOBB::CollOBB(Collision3D* col, XMFLOAT3 off, XMFLOAT3 size)
{
	Init();														// 初期化
	m_Offset = off;												// オフセット設定
	m_Scale = { size.x,size.y,size.z};							// サイズセット
	SetParent(col);												// 親をセット
	HRESULT hr = m_pBox->Init(XMFLOAT3(1.0f,1.0f,1.0f));		// デバッグ表示初期化

	// ---例外処理---
	if (FAILED(hr))
	{
		m_pBox->Fin();			// デバッグ表示終了処理
		SAFE_DELETE(m_pBox);	// デバッグ表示解放
		return;
	}

	m_pBox->SetMaterial(&m_Material);		// デバッグ表示色設定
}

// ===デストラクタ===
CollOBB::~CollOBB()
{
	m_nCnt--;						// OBB当たり判定総数減少
	if (m_nCnt <= 0)
	{
		m_pBox->Fin();				// デバッグ表示終了
		SAFE_DELETE(m_pBox);		// 解放
	}
}

// ===初期化関数===
void CollOBB::Init()
{
	// ---初期化---
	m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);		// 拡縮初期化
	m_coltype = COL_OBB;						// 当たり判定の型をセット

	if (m_nCnt == 0)
	{
		m_pBox = new CBox();						// デバッグ表示生成
	}
	m_nCnt++;									// OBB当たり判定総数増加
}

// ===ヒット確認===
bool CollOBB::CheckHit(CollisionBase* col)
{
	// ---除外処理---
	if (col->GetFollowCollision() == GetFollowCollision())		// 同じ当たり判定を参照している場合
		return false;			// 当たっていないと判定
	if (!col->GetEnable())										// 更新描画なしの場合
		return false;			// 当たっていないと判定

	m_bHit = false;									// 判定初期化
	XMFLOAT3 pos = col->GetCenter();				// 相手当たり判定中心座標

	// ---当たり判定---
	switch (col->GetColType())						// 当たり判定の型で分岐
	{
	case COL_AABB:
	case COL_OBB: {									// OBB同士
		XMFLOAT3 size = ((CollOBB*)col)->GetSize();					// 相手サイズ

		// ---ワールド行列取得---
		XMFLOAT4X4 mW1 = GetWorld();			// ワールド行列取得
		XMFLOAT4X4 mW2 = col->GetWorld();		// 判定相手のワールド行列取得

		// ---中心座標間のベクトルを求める---
		XMVECTOR vD = XMVectorZero();			// 初期化
		vD = XMVectorSet(pos.x - m_Pos.x, pos.y - m_Pos.y, pos.z - m_Pos.z, 0.0f);		// 中心座標間ベクトル

		// ---モデル座標軸を求める---
		XMVECTOR vN[6];
		vN[0] = XMVectorSet(mW1._11, mW1._12, mW1._13, 0.0f);
		vN[1] = XMVectorSet(mW1._21, mW1._22, mW1._23, 0.0f);
		vN[2] = XMVectorSet(mW1._31, mW1._32, mW1._33, 0.0f);
		vN[3] = XMVectorSet(mW2._11, mW2._12, mW2._13, 0.0f);
		vN[4] = XMVectorSet(mW2._21, mW2._22, mW2._23, 0.0f);
		vN[5] = XMVectorSet(mW2._31, mW2._32, mW2._33, 0.0f);

		// ---OBBの大きさ(半分)を掛けたベクトルを求める---
		XMFLOAT3 vBB1 = m_Scale;
		XMFLOAT3 vBB2 = size;
		XMVECTOR vL[6];
		vL[0] = XMVectorSet(mW1._11 * vBB1.x, mW1._12 * vBB1.x, mW1._13 * vBB1.x, 0.0f);
		vL[1] = XMVectorSet(mW1._21 * vBB1.y, mW1._22 * vBB1.y, mW1._23 * vBB1.y, 0.0f);
		vL[2] = XMVectorSet(mW1._31 * vBB1.z, mW1._32 * vBB1.z, mW1._33 * vBB1.z, 0.0f);
		vL[3] = XMVectorSet(mW2._11 * vBB2.x, mW2._12 * vBB2.x, mW2._13 * vBB2.x, 0.0f);
		vL[4] = XMVectorSet(mW2._21 * vBB2.y, mW2._22 * vBB2.y, mW2._23 * vBB2.y, 0.0f);
		vL[5] = XMVectorSet(mW2._31 * vBB2.z, mW2._32 * vBB2.z, mW2._33 * vBB2.z, 0.0f);

		// ---分離軸候補が面の法線ベクトル(モデル座標軸)---
		float fL, f, fD;
		for (int i = 0; i < 6; ++i) {
			XMVECTOR& vS = vN[i];			//分離軸候補

			// ---OBBの影(の半分)の合計---
			fL = 0.0f;
			for (int j = 0; j < 6; ++j) {
				XMStoreFloat(&f, XMVector3Dot(vS, vL[j]));			// 射影
				fL += fabsf(f);										// 合計する
			}

			// ---中心座標間の距離と比較---
			XMStoreFloat(&fD, XMVector3Dot(vS, vD));
			fD = fabsf(fD);
			if (fL < fD) return false;			// 当たっていない
		}

		// ---分離軸候補がそれぞれの辺に垂直なベクトル---
		XMVECTOR vS = XMVectorZero();
		for (int i = 0; i < 3; ++i) {
			for (int j = 3; j < 6; ++j) {

				// ---分離軸候補を求める---
				vS = XMVector3Normalize(XMVector3Cross(vN[i], vN[j]));

				// ---OBBの影(の半分)の合計---
				fL = 0.0f;
				for (int k = 0; k < 6; ++k) {
					XMStoreFloat(&f, XMVector3Dot(vS, vL[k]));		// 射影
					fL += fabsf(f);									// 合計する
				}

				// ---中心座標間の距離と比較---
				XMStoreFloat(&fD, XMVector3Dot(vS, vD));
				fD = fabsf(fD);
				if (fL < fD) return false;		// 当たっていない
			}
		}
		m_bHit = true;

		// --- 当たっていた場合の戻し量計算---
		if (!col->GetFollowCollision()->GetCollider())
		{
			XMFLOAT3 move = { 0.0f,0.0f,0.0f };
			XMFLOAT3 normal = { 0.0f,0.0f,0.0f };	// めり込みを戻す方向
			XMVECTOR work = XMVectorZero();			// 作業用
			float length = 0.0f;					// 長さ
			float r = 0.0f;
			float s = 0.0f;

			// ---最近接点を求める---
			CalcOBBtoPoint(&move, m_Pos, ((CollOBB*)col));

			// ---めり込んだOBB上の点から戻す方向を求める(最近点から中心座標へのベクトルを平面の法線とする---
			normal = XMFLOAT3(m_Pos.x - move.x, m_Pos.y - move.y, m_Pos.z - move.z);		// 最近接点から中心座標へのベクトルを平面の法線とする
			Normalize(&normal);							// 正規化
			work = XMLoadFloat3(&normal);

			for (int i = 0; i < 3; i++)
			{
				XMStoreFloat(&length, XMVector3Dot(vL[i], work));
				r += fabs(length);
			}

			// ---最近接点をヒットした座標として値をセット---
			m_pParent->SetHitPos(&move);

			// ---相手OBBの最近点と当たったOBB間の距離を求める---
			move = XMFLOAT3(m_Pos.x - move.x, m_Pos.y - move.y, m_Pos.z - move.z);
			Dot(&s, move, normal);

			// ---戻し距離を算出---
			if (s > 0)
				length = r - fabs(s);
			else
				length = r + fabs(s);

			// ---法線方向に戻し距離の分を乗算---
			move.x = normal.x * length;
			move.y = normal.y * length;
			move.z = normal.z * length;
			m_pParent->CalcMovement(&move);			// 移動量を設定
		}
		return true;			// 当たっている
		break;
	}
	case COL_SPHERE:

		float len;
		CalcLenOBBtoPoint(&len,pos);
		if (len <= (((CollSphere*)col)->GetRadius()))
		{
			// ---戻し量の計算---
			if (!col->GetFollowCollision()->GetCollider())
			{
				XMFLOAT3 work;
				CalcOBBtoPoint(&work, pos, this);									// 最近接点を求める
				XMFLOAT3 normal(work.x - pos.x, work.y - pos.y, work.z - pos.z);	// 最近接点からオブジェクトへのベクトルを求める
				Normalize(&normal);			// 正規化
				float length;				// 長さ算出用
				Length(&length, work, pos);	// 最近接点からの距離を求める
				length = ((CollSphere*)col)->GetRadius() - length;		// 境界球の半径から距離を減算することでめり込んだ量を算出

				normal.x *= length;			// --ベクトル方向に長さをかける
				normal.y *= length;
				normal.z *= length;
				// ---座標補正---
				m_pParent->CalcMovement(&normal);

				// ---最近接点をヒットした座標として値をセット
				m_pParent->SetHitPos(&work);
			}

			m_bHit = true;
			return true;
		}
	case COL_NONE:
		break;
	default:
		break;
	}
	return false;
}

// ===デバッグ表示===
void CollOBB::Draw()
{
	// ---描画判定---
	if (!GetVisible())
		return;

	// ---描画設定---
	m_pBox->SetMaterial(&m_Material);

	// ---変換行列計算(要改善)---
	XMMATRIX mtxworld, mtx;
	XMFLOAT4X4 mW;
	mtxworld = XMMatrixIdentity();								// 初期化
	mtx = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);		// スケール行列
	mtxworld = XMMatrixMultiply(mtxworld, mtx);					// 反映させる

	mtx = XMLoadFloat4x4(&GetWorld());							// 
	mtxworld = XMMatrixMultiply(mtxworld, mtx);					// スケールを反映

	XMStoreFloat4x4(&mW, mtxworld);								// XMFLOAT4X4型に変換
	m_pBox->SetWorld(&mW);										// メッシュに反映

	// ---描画---
	m_pBox->Draw();
}

// ===OBBサイズ取得===
XMFLOAT3 CollOBB::GetSize()
{
	return m_Scale;
}

// ===最短距離を求める===
// 引数2:基準点
void CollOBB::CalcLenOBBtoPoint(float* ans,const XMFLOAT3& point)
{
	XMVECTOR vD;
	vD = XMVectorSet(point.x - m_Pos.x, point.y - m_Pos.y, point.z - m_Pos.z, 0.0f);	// 中心座標間ベクトル
	XMFLOAT4X4 mW1 = GetWorld();														// ワールド行列取得
	XMVECTOR mRot[3];

	// ---サイズ取得---
	XMFLOAT3 Scale = m_pParent->GetSize();
	mRot[0] = XMVectorSet(mW1._11, mW1._12, mW1._13, 0.0f);
	mRot[1] = XMVectorSet(mW1._21, mW1._22, mW1._23, 0.0f);
	mRot[2] = XMVectorSet(mW1._31, mW1._32, mW1._33, 0.0f);

	// ---値補正(要修正)---
	mRot[0] /= Scale.x;
	mRot[1] /= Scale.y;
	mRot[2] /= Scale.z;

	// ---距離算出---
	float distance = 0.0f;																// 距離
	float size[3] = { m_Scale.x * Scale.x,m_Scale.y * Scale.y,m_Scale.z * Scale.z };
	XMVECTOR calc = XMVectorZero();

	for (int i = 0; i < 3; i++)
	{
		if (size[i] <= 0)																// -の値は加算しない
			continue;
		XMStoreFloat(&distance, XMVector3Dot(vD, mRot[i]) / size[i]);					// 射影計算
		distance = fabs(distance);														// 距離を知りたいので絶対値とする
		if(distance > 1)
			calc += (1 - distance) * size[i] * mRot[i];
	}
	XMStoreFloat(ans, XMVector3Length(calc));											// 計算結果
}

// ===OBB上の最近接点座標を求める===
// 引数3:計算したい相手OBBクラスのポインタ
void CollOBB::CalcOBBtoPoint(XMFLOAT3* ans, const XMFLOAT3& point,CollOBB* obb)
{
	XMVECTOR vD;
	XMFLOAT3 pos = obb->GetCenter();
	XMFLOAT3 vsize = obb->GetSize();
	XMFLOAT3 Scale = obb->GetFollowCollision()->GetSize();
	vD = XMVectorSet(point.x - pos.x, point.y - pos.y, point.z - pos.z, 0.0f);		// 中心座標間ベクトル
	XMFLOAT4X4 mW1 = obb->GetWorld();												// ワールド行列取得
	XMVECTOR mRot[3];

	// ---サイズ取得---
	mRot[0] = XMVectorSet(mW1._11, mW1._12, mW1._13, 0.0f);
	mRot[1] = XMVectorSet(mW1._21, mW1._22, mW1._23, 0.0f);
	mRot[2] = XMVectorSet(mW1._31, mW1._32, mW1._33, 0.0f);

	// ---値補正(要修正)---
	mRot[0] /= Scale.x;
	mRot[1] /= Scale.y;
	mRot[2] /= Scale.z;

	// ---距離算出---
	float distance;
	float size[3] = { vsize.x * Scale.x, vsize.y * Scale.y, vsize.z * Scale.z};
	XMVECTOR calc = XMVectorZero();

	for (int i = 0; i < 3; i++)
	{
		XMStoreFloat(&distance, XMVector3Dot(vD, mRot[i]));			// 射影計算

		// ---その軸の長さによって位置を求める---
		if (distance > size[i])
		{
			distance = size[i];
		}
		if (distance < -size[i])
		{
			distance = -size[i];
		}
		calc += distance * mRot[i];
	}
	XMStoreFloat3(ans, calc);

	// ---求められる最近点はローカルなので、ワールド座標になるように中心座標からオフセットする---
	ans->x = ans->x + pos.x;
	ans->y = ans->y + pos.y;
	ans->z = ans->z + pos.z;

}