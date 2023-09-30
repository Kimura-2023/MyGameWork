// カメラベースクラス [Camera.h]
#pragma once
#include "main.h"
#include "Model.h"
#include "Calc.h"

class CameraBase
{
protected:
	DirectX::XMFLOAT3 m_vPos;		// 視点
	DirectX::XMFLOAT3 m_vTarget;	// 注視点
	DirectX::XMFLOAT3 m_vUp;		// 上方ベクトル
	DirectX::XMFLOAT3 m_vFront;		// 始点から注視点へのベクトル
	DirectX::XMFLOAT3 m_vPosGap;	// カメラ座標のずれ

	DirectX::XMFLOAT3 m_vAngle;		// カメラの角度
	float m_fLengthInterval;		// カメラの視点と注視点の距離
	XMFLOAT4 m_frus[6];				// 視錐台用
	XMFLOAT4 m_frusw[6];			// 視錐台ワールド変換

	float m_fAspectRatio;			// 縦横比
	float m_fFovY;					// 視野角(Degree)
	float m_fNearZ;					// 前方クリップ距離
	float m_fFarZ;					// 後方クリップ距離
private:

	DirectX::XMFLOAT4X4 m_mtxWorld;	// ワールド マトリックス
	DirectX::XMFLOAT4X4 m_mtxView;	// ビュー マトリックス
	DirectX::XMFLOAT4X4 m_mtxProj;	// プロジェクション マトリックス

	Model* m_pSky;			// スカイ ドーム

public:
	CameraBase();

	virtual void Init();
	virtual void Update();
	virtual void Clear();

	void SetPos(DirectX::XMFLOAT3& vPos) { m_vPos = vPos; }
	DirectX::XMFLOAT3& GetPos() { return m_vPos; }
	DirectX::XMFLOAT3& GetLook() { return m_vTarget; }
	DirectX::XMFLOAT3& GetUp() { return m_vUp; }
	DirectX::XMFLOAT3& GetLookVector() { return m_vFront; }
	void SetPos(float x, float y, float z) { m_vPos.x = x, m_vPos.y = y, m_vPos.z = z; }
	void SetTarget(DirectX::XMFLOAT3& vTarget) { m_vTarget = vTarget; }
	void SetTarget(float x, float y, float z) { m_vTarget.x = x, m_vTarget.y = y, m_vTarget.z = z; }
	void SetUpVector(DirectX::XMFLOAT3& vUp) { m_vUp = vUp; }
	void SetUpVector(float x, float y, float z) { m_vUp.x = x, m_vUp.y = y, m_vUp.z = z; }
	void SetWorldMatrix(DirectX::XMFLOAT4X4& mtxWorld);
	DirectX::XMFLOAT4X4& GetWorldMatrix() { return m_mtxWorld; }
	DirectX::XMFLOAT4X4& CalcWorldMatrix();
	void SetFovY(float fFovY) { m_fFovY = fFovY; }
	void SetAspectRatio(float fAspect) { m_fAspectRatio = fAspect; }
	void SetRangeZ(float fNearZ, float fFarZ) { m_fNearZ = fNearZ, m_fFarZ = fFarZ; }
	void UpdateMatrix();
	DirectX::XMFLOAT4X4& GetViewMatrix() { return m_mtxView; }
	DirectX::XMFLOAT4X4& GetProjMatrix() { return m_mtxProj; }
	DirectX::XMFLOAT3& GetAngle() { return m_vAngle; }
	void SetSky(Model* pSky) { m_pSky = pSky; }
	Model* GetSky() { return m_pSky; }
	float& GetLength() { return m_fLengthInterval; }

	int CollisionViewFrustum(XMFLOAT3* pCenter, float fRadius);

	float& GetAspect() { return m_fAspectRatio; }
	float& GetFov() { return m_fFovY; }
	float& GetNearClip() { return m_fNearZ; }
	float& GetFarClip() { return m_fFarZ; }

	Quaternion GetXZQuaternion();
};
