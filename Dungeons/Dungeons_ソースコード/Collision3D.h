#pragma once

// ===当たり判定クラス定義===
// 当たり判定クラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "main.h"
#include "Object3DBase.h"
#include "Object2DBase.h"
#include "CollisionBase.h"
#include <vector>

class CollisionBase;				// クラスがあることを定義

// ===定数・マクロ定義===
using COLDIT = std::vector<CollisionBase*>::iterator;

// ===クラス定義===
class Collision3D
{
	// ---メンバ関数---
public:
	Collision3D(Object*);												// コンストラクタ
	~Collision3D();														// デストラクタ

	void Update();														// 更新処理
	void UpdateMatrix();												// 変換行列更新
	void Draw();														// デバッグ表示
	virtual void OnCollision(Object*) = 0;								// 当たった時の処理(当たり判定が必要なオブジェクトごとで継承し、関数を作成する)
	virtual void ReflectMovement() { ; }								// 戻しの計算

	// ---当たり判定セット関数---
	void AddCollisionAABB();												// 当たり判定リストにAABB追加
	void AddCollisionAABB(const XMFLOAT3& size, const XMFLOAT3& offset);	// 当たり判定リストにAABB追加(引数でサイズ、中心座標からのずれを指定)
	void AddCollisionLine();												// 当たり判定線分リストに当たり判定線分を追加
	void AddCollisionLine(const XMFLOAT3&, const XMFLOAT3&);				// 当たり判定線分リストに当たり判定線分を追加(引数で線分の終点、中心座標からのずれを指定)
	void AddCollisionSphere();												// 当たり判定球リストに当たり判定球を追加
	void AddCollisionSphere(const XMFLOAT3&, const float&);					// 当たり判定球リストに当たり判定球を追加(引数で中心座標からのずれ、半径を指定)
	void AddCollisionOBB();													// 当たり判定リストにOBB追加
	void AddCollisionOBB(const XMFLOAT3& offset, const XMFLOAT3& size);		// 当たり判定リストにOBB追加(引数でサイズ、中心座標からのずれを指定)

	void SwitchVisible();											// デバッグ表示有無切り替え
	void SetCollision(bool);										// 当たり判定チェック有無をセット
	void SetCollider(bool);											// めり込み量を戻すかどうかを設定

//	void SetNode(DWORD* pnode) { m_pNodeindex = *pnode; }			// 所属空間ノードをセット

	// ---取得関数---
	Object* GetFollowObject() { return m_pObj; }								// 参照オブジェクト取得
	std::vector<CollisionBase*> GetCollisionData() { return m_pColList; }		// 当たり判定リストを取得
	bool CheckCollision(Collision3D*);								// ヒット確認

//	DWORD* GetCurrentNode() { return &m_pNodeindex; }				// 現在の所属空間ノードを取得

	XMFLOAT3& GetCenter() { return m_vCenter; }							// 中心座標
	XMFLOAT3& GetSize() { return m_vSize; }
	XMFLOAT4X4& GetWorld() { return m_mWorld; }							// ワールド変換行列
	bool& GetEnable() { return m_bEnableflg; }							// 当たり判定を取るかどうかのフラグ取得
	bool& GetCollider() { return m_bCollider; }							// 戻しの計算をするかどうか取得

	void CalcMovement(XMFLOAT3* movement);								// めり込みを戻す量を計算
	XMFLOAT3& GetMovement();											// 戻し量を取得
	void SetHitPos(XMFLOAT3* hitpos);									// 衝突した座標を設定
	XMFLOAT3& GetHitPos();												// 衝突した座標を取得
protected:

	// ---メンバ変数---
private:
	std::vector<CollisionBase*> m_pColList;								// 当たり判定リスト(ベースから派生させて生成する)
	XMFLOAT3 m_vCenter;													// 中心座標
	XMFLOAT3 m_vSize;													// 当たり判定サイズ
	XMFLOAT3 m_vRot;													// 回転量
	XMFLOAT4X4 m_mWorld;												// ワールドマトリックス
	XMFLOAT3 m_Movement;												// 衝突してめり込んだ際に戻す量
	XMFLOAT3 m_HitPos;													// 衝突した位置

	bool m_bEnableflg;													// 当たり判定を取るか取らないかのフラグ(m_bEnableにしようと思っていたら継承してオブジェクトを生成する関係でObject3Dbaseで同じものを定義していたためm_bEnableflgに
	bool m_bCollider;													// 戻し計算を行わないものはtrue

	Object* m_pObj;														// 参照しているオブジェクト
	//DWORD m_pNodeindex;												// 所属しているノード(オクツリー更新のためにどのノードに所属しているかを表す)
};