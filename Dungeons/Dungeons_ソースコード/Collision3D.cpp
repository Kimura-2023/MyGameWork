// ===当たり判定クラス実装===
// 当たり判定クラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "Collision3D.h"
#include "ColSphere.h"
#include "ColAABB.h"
#include "ColOBB.h"
#include "ColLine.h"
#include "CollisionManager.h"
#include "DebugProc.h"
#include "Calc.h"

// ===コンストラクタ===
Collision3D::Collision3D(Object* obj)
{
	// ---初期化処理---
	m_pObj = obj;										// 参照オブジェクト設定
	XMStoreFloat4x4(&m_mWorld, XMMatrixIdentity());		// ワールド行列初期化
	m_vCenter = m_pObj->GetTransform().Pos;				// 中心座標設定
	m_vSize = m_pObj->GetTransform().Scale;				// スケール設定
	m_bCollider = false;								// めり込み量を戻すかどうか
	COLLISION->AddManager(this);						// 当たり判定管理下に追加
}

// ===デストラクタ===
Collision3D::~Collision3D()
{
	// ---解放処理---
	COLDIT colit = m_pColList.begin();					// 当たり判定リストの先頭イテレータ取得
	while (colit != m_pColList.end())					// 終端まで繰り返し
	{
		delete *colit;									// 解放
		*colit = nullptr;								//
		colit = m_pColList.erase(colit);				// リストから削除
	}
	COLLISION->Release(this);							// 管理下からも解放
}

// ===AABB当たり判定追加===
void Collision3D::AddCollisionAABB()
{
	m_vCenter = m_pObj->GetTransform().Pos;									// 中心座標設定
	CollAABB* box = new CollAABB(this,m_pObj->GetTransform().Scale);		// AABB生成
	m_pColList.push_back(box);												// 当たり判定リストに追加
}

// ===AABB当たり判定追加(オフセットあり)
void Collision3D::AddCollisionAABB(const XMFLOAT3& offset , const XMFLOAT3& size)
{
	m_vCenter = m_pObj->GetTransform().Pos;									// 中心座標設定
	CollAABB* box = new CollAABB(this, offset,size);						// AABB生成
	m_pColList.push_back(box);												// 当たり判定リストに追加
}

// ===線分当たり判定追加===
void Collision3D::AddCollisionLine()
{
	m_vCenter = m_pObj->GetTransform().Pos;									// 中心座標設定
	CollLine* line = new CollLine(this,XMFLOAT3(0.0f,0.0f,0.0f));			// 線分生成
	m_pColList.push_back(line);												// 当たり判定リストに追加
}

// ===線分当たり判定追加(オフセットあり)===
void Collision3D::AddCollisionLine(const XMFLOAT3& offset, const XMFLOAT3& end)
{
	m_vCenter = m_pObj->GetTransform().Pos;									// 中心座標設定
	CollLine* line = new CollLine(this, offset, end);						// 線分生成
	m_pColList.push_back(line);												// 当たり判定リストに追加
}

// ===境界球当たり判定追加===
void Collision3D::AddCollisionSphere()
{
	m_vCenter = m_pObj->GetTransform().Pos;									// 中心座標設定
	CollSphere* sphere = new CollSphere(this,m_pObj->GetRad());				// 境界球生成
	m_pColList.push_back(sphere);											// 当たり判定リストに追加
}

// ===境界球当たり判定追加(オフセットあり)===
void Collision3D::AddCollisionSphere(const XMFLOAT3& offset, const float& rad)
{
	m_vCenter = m_pObj->GetTransform().Pos;									// 中心座標設定
	CollSphere* sphere = new CollSphere(this,offset,rad);					// 境界球生成
	m_pColList.push_back(sphere);											// 当たり判定リストに追加
}

// ===OBB当たり判定追加===
void Collision3D::AddCollisionOBB()
{
	m_vCenter = m_pObj->GetTransform().Pos;									// 中心座標設定
	CollOBB* obb = new CollOBB(this, m_pObj->GetTransform().Scale);			// OBB生成
	m_pColList.push_back(obb);												// 当たり判定リストに追加
}

// ===OBB当たり判定追加(オフセットあり)===
void Collision3D::AddCollisionOBB(const XMFLOAT3& offset, const XMFLOAT3& size)
{
	m_vCenter = m_pObj->GetTransform().Pos;									// 中心座標設定
	CollOBB* obb = new CollOBB(this, offset, size);							// OBB生成
	m_pColList.push_back(obb);												// 当たり判定リストに追加
}

// ===更新処理===
void Collision3D::Update()
{
	m_vCenter = m_pObj->GetPos();				// 参照するオブジェクトの中心座標に合わせる
	m_vSize = m_pObj->GetScale();
	m_vRot = m_pObj->GetRotate();				// 回転も同様に反映させる

	m_mWorld = m_pObj->GetWorld();				// 行列更新
	m_bEnableflg = m_pObj->GetCol();			// 当たり判定の可否更新
	m_Movement = XMFLOAT3(0.0f, 0.0f, 0.0f);	// めり込んだ量をリセット

	if (!m_pObj->GetEnable())
		m_bEnableflg = false;					// 当たり判定をとるかどうか

	COLDIT it = m_pColList.begin();				// 当たり判定リストの先頭イテレータ取得
	while (it != m_pColList.end())				// 当たり判定リストの終端まで繰り返し
	{
		// ---例外処理---
		if ((*it) == nullptr)
		{
			it = m_pColList.erase(it);			// 当たり判定リストから削除
			continue;
		}

		// ---リスト内の当たり判定それぞれの更新---
		(*it)->Update();						// 更新処理
		++it;									// 次の当たり判定を探索
	}
}

// ===行列更新===
void Collision3D::UpdateMatrix()
{
	XMMATRIX mtxWorld,mtxTranslate,mtxRot,mtxScale;
	mtxWorld = XMMatrixIdentity();						// 行列初期化

	// ---拡縮反映---
	mtxScale = XMMatrixScaling(m_vSize.x, m_vSize.y, m_vSize.z);			// スケールを基に行列を生成
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScale);						// ワールド変換行列に反映させる

	// ---回転を反映---
	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_vRot.x), 
		XMConvertToRadians(m_vRot.y), XMConvertToRadians(m_vRot.z));		// 回転行列を生成
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);							// ワールド変換行列に反映

	Quaternion q;										// クォータニオン回転の反映
	q = m_pObj->GetQuaternion();						// 参照オブジェクトの姿勢を取得
	Quaternion::GetRotateMatrix(&mtxRot, q);			// クォータニオンから回転行列を作成
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);		// ワールド変換行列に反映
	
	// ---移動を反映---
	mtxTranslate = XMMatrixTranslation(m_vCenter.x, m_vCenter.y, m_vCenter.z);		// 移動行列を生成
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);							// ワールド変換行列に反映

	// ---ワールドマトリックス設定---
	XMStoreFloat4x4(&m_mWorld, mtxWorld);				// メンバ変数に反映
}

// ---当たり判定デバッグ表示---
void Collision3D::Draw()
{
	// ---除外処理---
	if (!m_pObj->GetVisible())
		return;
	if (!m_pObj->GetEnable())
		return;

	// ---描画---
	SetZWrite(false);							// ZWriteをオフ
	SetBlendState(BS_ALPHABLEND);				// ブレンドステート設定
	COLDIT it = m_pColList.begin();				// 当たり判定リストの先頭イテレータ取得
	while (it != m_pColList.end())				// 終端まで繰り返し
	{
		// ---例外処理---
		if ((*it) == nullptr)
		{
			m_pColList.erase(it);				// リストから除外
			continue;
		}

		// ---描画処理---
		(*it)->Draw();							// デバッグ表示
		++it;									// 次の当たり判定を探索
	}

	// ---描画設定を元に戻す---
	SetBlendState(BS_NONE);						// ブレンド設定
	SetZWrite(true);							// 
}

// ===ヒット確認===
bool Collision3D::CheckCollision(Collision3D* col)
{
	std::vector<CollisionBase*> other = col->GetCollisionData();		// 他オブジェクトの当たり判定

	COLDIT it, otherit;
	it = m_pColList.begin();							// 自当たり判定リストの先頭イテレータ取得
	otherit = other.begin();							// 相手の当たり判定リストの先頭イテレータ取得
	bool bHit = false;									// 当たったかどうか

	// ---判定---
	while (it != m_pColList.end())						// ---自当たり判定リストの終端まで繰り返し
	{
		while (otherit != other.end())						// ---相手当たり判定リストの終端まで繰り返し
		{
			bHit = (*it)->CheckHit(*otherit);				// ヒット確認
			++otherit;										// 次の相手当たり判定を探索

			// ---衝突時---
			if (bHit == true)
				return bHit;									// 他のオブジェクトのどこかの当たり判定にぶつかっていたらtrueを返す
		}
		++it;											// 次の当たり判定探索
	}

	return bHit;										// 結果を返す
}

// ===当たり判定可否を設定===
void Collision3D::SetCollision(bool flg)
{
	m_bEnableflg = flg;
}

// ===めり込み量を戻すかどうかを設定===
void Collision3D::SetCollider(bool flg)
{
	m_bCollider = flg;
}

// ===デバッグ表示有無の切替====
void Collision3D::SwitchVisible()
{
	COLDIT it = m_pColList.begin();					// 当たり判定リストの先頭イテレータ取得
	while (it != m_pColList.end())					// 当たり判定リストの終端まで繰り返し
	{
		// ---例外処理---
		if ((*it) == nullptr)
		{
			m_pColList.erase(it);					// リストから除外
			continue;
		}

		// ---切替---
		(*it)->SetVisible(!(*it)->GetVisible());	// 可視状態の切替
		++it;										// 次の当たり判定を探索
	}
}

// ===戻し量の更新===
void Collision3D::CalcMovement(XMFLOAT3* movement)
{
	// ---めり込んだ距離を絶対値で比較し、大きい方で計算する---
	if (fabs(m_Movement.x) < fabs(movement->x))
		m_Movement.x += movement->x;				// x方向
	if (fabs(m_Movement.y) < fabs(movement->y))
		m_Movement.y += movement->y;				// y方向
	if (fabs(m_Movement.z) < fabs(movement->z))
		m_Movement.z += movement->z;				// z方向
}

// ===戻し量を取得===
XMFLOAT3& Collision3D::GetMovement()
{
	// ---例外処理---
	if (std::isnan(m_Movement.x) || std::isnan(m_Movement.y) || std::isnan(m_Movement.z))
		return m_Movement = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// ---値を返す---
	return m_Movement;
}

// ===衝突点を設定===
void Collision3D::SetHitPos(XMFLOAT3* hitpos)
{
	m_HitPos = *hitpos;
}

// ===衝突店を取得===
XMFLOAT3& Collision3D::GetHitPos()
{
	// ---例外処理---
	if (std::isnan(m_HitPos.x) || std::isnan(m_HitPos.y) || std::isnan(m_HitPos.z))
		return m_HitPos = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// ---値を返す---
	return m_HitPos;
}