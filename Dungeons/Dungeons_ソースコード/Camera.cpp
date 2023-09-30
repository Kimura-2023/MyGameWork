// ===カメラベースクラス===
#include "Camera.h"
#include "CameraManager.h"
#include "Light.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
namespace {
	const float CAM_POS_P_X = 0.0f;					// カメラの視点初期位置(X座標)
	const float CAM_POS_P_Y = 10.0f;				// カメラの視点初期位置(Y座標)
	const float CAM_POS_P_Z = -10.0f;				// カメラの視点初期位置(Z座標)
	const float CAM_POS_R_X = 0.0f;					// カメラの注視点初期位置(X座標)
	const float CAM_POS_R_Y = 0.0f;					// カメラの注視点初期位置(Y座標)
	const float CAM_POS_R_Z = 0.0f;					// カメラの注視点初期位置(Z座標)
	const float VIEW_ANGLE = 45.0f;					// 視野角
	const float VIEW_ASPECT = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;	// アスペクト比
	const float VIEW_NEAR_Z = 0.3f;					// NearZ値
	const float VIEW_FAR_Z = 1000.0f;				// FarZ値

}

// コンストラクタ
CameraBase::CameraBase()
{
	Init();
}

// 初期化
void CameraBase::Init()
{
	m_pSky = nullptr;

	m_vPos = XMFLOAT3(CAM_POS_P_X, CAM_POS_P_Y, CAM_POS_P_Z);		// 視点
	m_vTarget = XMFLOAT3(CAM_POS_R_X, CAM_POS_R_Y, CAM_POS_R_Z);	// 注視点
	m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);		// 上方ベクトル
	m_vPosGap = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_fAspectRatio = VIEW_ASPECT;			// 縦横比
	m_fFovY = VIEW_ANGLE;					// 視野角(Degree)
	m_fNearZ = VIEW_NEAR_Z;					// 前方クリップ距離
	m_fFarZ = VIEW_FAR_Z;					// 後方クリップ距離

	m_vAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);
	float fVecX, fVecZ;
	fVecX = m_vPos.x - m_vTarget.x;
	fVecZ = m_vPos.z - m_vTarget.z;
	m_fLengthInterval = sqrtf(fVecX * fVecX + fVecZ * fVecZ);

	m_frus[0].x = 0.0f;		m_frus[0].y = -1.0f;
	float fTan = tanf(XMConvertToRadians(m_fFovY * 0.5f));
	m_frus[0].z = fTan;	m_frus[0].w = 0.0f;
	m_frus[1].x = 0.0f;		m_frus[1].y = 1.0f;
	m_frus[1].z = fTan;	m_frus[1].w = 0.0f;
	fTan *= m_fAspectRatio;			//アスペクト比を掛ける
	m_frus[2].x = 1.0f;		m_frus[2].y = 0.0f;
	m_frus[2].z = fTan;	m_frus[2].w = 0.0f;
	m_frus[3].x = -1.0f;	m_frus[3].y = 0.0f;
	m_frus[3].z = fTan;	m_frus[3].w = 0.0f;
	m_frus[4].x = 0.0f;		m_frus[4].y = 0.0f;
	m_frus[4].z = 1.0f;		m_frus[4].w = -m_fNearZ;
	m_frus[5].x = 0.0f;		m_frus[5].y = 0.0f;
	m_frus[5].z = -1.0f;	m_frus[5].w = m_fFarZ;

	//正規化
	for (int i = 0; i < 4; ++i)
		XMStoreFloat4(&m_frus[i],XMPlaneNormalize(XMLoadFloat4(&m_frus[i])));

	CalcWorldMatrix();
}

// 更新
void CameraBase::Update()
{
	m_vFront = { m_vTarget.x - m_vPos.x, m_vTarget.y - m_vPos.y, m_vTarget.z - m_vPos.z };		// 視点から注視点へのベクトルを更新
	CalcWorldMatrix();
	XMMATRIX mW = XMLoadFloat4x4(&m_mtxWorld);

	// 平面をワールド空間に配置
	mW = XMMatrixInverse(nullptr, mW);
	mW = XMMatrixTranspose(mW);
	for (int i = 0; i < 6; ++i)
		XMStoreFloat4(&m_frusw[i], XMPlaneTransform(XMLoadFloat4(&m_frus[i]), mW));

	// マトリックス更新
	UpdateMatrix();
}

// 画面クリア
void CameraBase::Clear()
{
	// main.cppのリソース参照
	extern ID3D11RenderTargetView* g_pRenderTargetView;
	extern ID3D11DepthStencilView* g_pDepthStencilView;

	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	ID3D11DeviceContext* pDC = GetDeviceContext();
	pDC->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
	pDC->ClearDepthStencilView(g_pDepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	if (m_pSky) {
		SetZBuffer(false);		// Zバッファ無効
		CLight* pLight = CLight::Get();
		pLight->SetDisable();	// ライティング無効
		XMFLOAT4X4 mW;
		XMStoreFloat4x4 (&mW, XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z));
		m_pSky->Draw(mW);			// スカイドーム描画
		pLight->SetEnable();	// ライティング有効
	}

	SetZBuffer(true);			// Zバッファ有効
	SetBlendState(BS_NONE);
}

// ビュー/プロジェクション マトリックス更新
void CameraBase::UpdateMatrix()
{
	XMFLOAT3 pos = XMFLOAT3(m_vPos.x + m_vPosGap.x, m_vPos.y + m_vPosGap.y, m_vPos.z + m_vPosGap.z);
	XMFLOAT3 target = XMFLOAT3(m_vTarget.x + m_vPosGap.x, m_vTarget.y + m_vPosGap.y, m_vTarget.z + m_vPosGap.z);
	XMStoreFloat4x4(&m_mtxView, XMMatrixLookAtLH(
		XMLoadFloat3(&pos), XMLoadFloat3(&target), XMLoadFloat3(&m_vUp)));
	XMStoreFloat4x4(&m_mtxProj, XMMatrixPerspectiveFovLH(
		XMConvertToRadians(m_fFovY), m_fAspectRatio, m_fNearZ, m_fFarZ));
}

// ワールド マトリックス設定
void CameraBase::SetWorldMatrix(XMFLOAT4X4& mtxWorld)
{
	m_mtxWorld = mtxWorld;
	m_vPos = XMFLOAT3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
	m_vTarget = XMFLOAT3(mtxWorld._41 + mtxWorld._31, mtxWorld._42 + mtxWorld._32, mtxWorld._43 + mtxWorld._33);
	m_vUp = XMFLOAT3(mtxWorld._21, mtxWorld._22, mtxWorld._23);
}

// 視点/注視点/上方ベクトルからワールド マトリックス算出
XMFLOAT4X4& CameraBase::CalcWorldMatrix()
{
	// ---カメラの姿勢を求める---
	XMVECTOR vecZ = XMVectorSet(m_vTarget.x - m_vPos.x, m_vTarget.y - m_vPos.y, m_vTarget.z - m_vPos.z, 0.0f);		// Z軸方向を算出
	XMFLOAT3 vZ;
	XMStoreFloat3(&vZ, XMVector3Normalize(vecZ));						// XMFLOAT3型に変換
	XMVECTOR vecY = XMLoadFloat3(&m_vUp);								// 上方ベクトルを取得
	XMVECTOR vecX = XMVector3Normalize(XMVector3Cross(vecY, vecZ));		// X軸方向を算出
	XMFLOAT3 vX;
	XMStoreFloat3(&vX, vecX);											// XMFLOAT3型に変換
	vecY = XMVector3Normalize(XMVector3Cross(vecZ, vecX));				// Y軸方向を算出
	XMFLOAT3 vY;
	XMStoreFloat3(&vY, vecY);											// XMFLOAT3型に変換

	// ---ワールドマトリックス算出---
	m_mtxWorld._11 = vX.x;
	m_mtxWorld._12 = vX.y;
	m_mtxWorld._13 = vX.z;
	m_mtxWorld._14 = 0.0f;
	m_mtxWorld._21 = vY.x;
	m_mtxWorld._22 = vY.y;
	m_mtxWorld._23 = vY.z;
	m_mtxWorld._24 = 0.0f;
	m_mtxWorld._31 = vZ.x;
	m_mtxWorld._32 = vZ.y;
	m_mtxWorld._33 = vZ.z;
	m_mtxWorld._34 = 0.0f;
	m_mtxWorld._41 = m_vPos.x + m_vPosGap.x;
	m_mtxWorld._42 = m_vPos.y + m_vPosGap.y;
	m_mtxWorld._43 = m_vPos.z + m_vPosGap.z;
	m_mtxWorld._44 = 1.0f;

	return m_mtxWorld;
}

// 視錐台クリップ
int CameraBase::CollisionViewFrustum(XMFLOAT3* pCenter, float fRadius)
{
	bool bHit = false;
	XMVECTOR frusw, center, dot;
	float fDot;
	center = XMLoadFloat3(pCenter);
	for (int i = 0; i < 6; ++i) {
		frusw = XMLoadFloat4(&m_frusw[i]);
		dot = XMPlaneDotCoord(frusw, center);
		XMStoreFloat(&fDot, dot);
		if (fDot < -fRadius) return 0;//完全に外
		if (fDot <= fRadius) bHit = true;
	}
	if (bHit) return -1;	//面を跨ぐ
	return 1;	//完全に内側
}

Quaternion CameraBase::GetXZQuaternion()
{
	XMFLOAT3 front = { 0.0f,1.0f,0.0f};		// 基準の正面
	XMFLOAT3 target = m_vTarget;
	Normalize(&target);						// 現在の注視点へのベクトルを正規化
	target.y = 0;
	Quaternion q;
	Quaternion::LookAt(&q, front, target);
	return q;
}