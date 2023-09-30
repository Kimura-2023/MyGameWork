// ===3Dオブジェクトベースクラス===
// Author kimura
//==============================

// ===インクルード===
#include "Object3DBase.h"
#include "ObjectManager.h"
#include "DefineData.h"
#include "ModelManager.h"
#include "DebugProc.h"
#include "CameraManager.h"
#include "ShaderList.h"
#include "SceneManager.h"

// ===グローバル===
std::shared_ptr<ConstantBuffer>Object3DBase::m_pWVPBuffer = nullptr;
int Object3DBase::m_nObjCnt = 0;

// ===コンストラクタ===
Object3DBase::Object3DBase()
{
	// ---バッファ初期化---
	if (m_nObjCnt == 0)
	{
		m_pWVPBuffer = std::make_shared<ConstantBuffer>();		// バッファ生成
		m_pWVPBuffer->Create(sizeof(XMFLOAT4X4) * 3);
	}
	m_nObjCnt++;		// オブジェクト総数更新

	// ---初期化処理---
	OBJECT->AddObjectManager(this);							// オブジェクト管理下に追加
	m_Model = nullptr;										// モデル
	m_pPS = GetPS(PS_LAMBERT);								// ピクセルシェーダー
	m_pVS = GetVS(VS_OBJECT);								// 頂点シェーダー

	m_transform = Transform();								// 座標・回転・拡縮
	m_qRot = Quaternion(XMFLOAT3(0.0f, 1.0f, 0.0f), 0.0f);	// 回転
	m_vFront = XMFLOAT3(0.0f, 0.0f, 1.0f);					// 正面方向

	m_vVel = XMFLOAT3(0.0f, 0.0f, 0.0f);					// 速度
	m_vAcc = XMFLOAT3(0.0f, 0.0f, 0.0f);					// 加速度

	XMStoreFloat4x4(&m_mWorld, XMMatrixIdentity());			// 行列初期化
	m_bGravity = false;										// 重力設定
	m_bEnable = false;										// 更新・描画可否
	m_bVisible = false;										// 視認可否
}

// ===デストラクタ===
Object3DBase::~Object3DBase()
{
	OBJECT->Release(this);
	m_nObjCnt--;
	if (m_nObjCnt <= 0)
	{
		m_pWVPBuffer.reset();
	}
}

// ===更新===
void Object3DBase::Update()
{
	m_mWorld._41 = m_transform.Pos.x;
	m_mWorld._42 = m_transform.Pos.y;
	m_mWorld._43 = m_transform.Pos.z;

	// ---視錐台判定---
	CameraBase* pCamera = CAMERA->GetCurrentCamera();			// カメラ取得
	if (pCamera->CollisionViewFrustum(&m_transform.Pos, CalcObjSize(&m_transform.Scale)) != 0)
		m_bVisible = true;
	else if(pCamera->CollisionViewFrustum(&m_transform.Pos, CalcObjSize(&m_transform.Scale)) == 0)
		m_bVisible = false;

	// ---更新判定---
	if (!m_bEnable)
		return;
	if (!m_bVisible)
		return;

	// ---重力---
	if (m_bGravity)
		m_vAcc.y = -GRAVITY;

	// ---加速度を速度に反映させる---
	m_vVel.x += m_vAcc.x;				// x軸方向
	m_vVel.y += m_vAcc.y;				// y軸方向
	m_vVel.z += m_vAcc.z;				// z軸方向

	// ---速度を座標に加算---
	m_transform.Pos.x += m_vVel.x;		// x軸方向
	m_transform.Pos.y += m_vVel.y;		// y軸方向
	m_transform.Pos.z += m_vVel.z;		// z軸方向

	// ---進行方向に徐々に回転させる---
	Quaternion q = Quaternion::Identity();									// 初期化
	Quaternion::LookRotation(&q, m_vFront, XMFLOAT3(0.0f, 1.0f, 0.0f));		// 正面方向を向くクォータニオンを求める
	Quaternion::SLerp(&m_qRot, m_qRot, q, 0.25f);							// 補間

	// ---行列更新---
	UpdateMatrix();

	// ---無敵時間更新---
	m_nInvincibleTime--;
	if (m_nInvincibleTime <= 0)
	{
		m_nInvincibleTime = 0;
	}
}

// ===オブジェクト描画===
void Object3DBase::Draw()
{
	// ---シェーダーセット---
	m_pPS->Bind();
	m_pVS->Bind();

	// ---描画判定---
	if (!m_bEnable)
		return;
	if (!m_bVisible)
		return;

	// ---変換行列作成---
	DirectX::XMMATRIX mat[3];
	DirectX::XMFLOAT4X4 fMat[3];

	mat[0] = XMLoadFloat4x4(&m_mWorld);											// ワールド変換行列
	mat[1] = XMLoadFloat4x4(&CAMERA->GetCurrentCamera()->GetViewMatrix());		// ビュー行列
	mat[2] = XMLoadFloat4x4(&CAMERA->GetCurrentCamera()->GetProjMatrix());		// プロジェクション行列

	// ---行列転置(シェーダーに値を渡すため)---
	DirectX::XMStoreFloat4x4(&fMat[0], XMMatrixTranspose(mat[0]));			// ワールド変換行列
	DirectX::XMStoreFloat4x4(&fMat[1], XMMatrixTranspose(mat[1]));			// ビュー行列
	DirectX::XMStoreFloat4x4(&fMat[2], XMMatrixTranspose(mat[2]));			// プロジェクション行列

	// ---値をセット---
	m_pWVPBuffer->Write(fMat);		// 定数バッファにセット
	m_pWVPBuffer->BindVS(0);

	if(m_Model != nullptr)
		m_Model->Draw();			// 描画

	// ---エディタ描画(輪郭線)---
	if (m_bSelect)			// 選択状態判定
	{
		SetCullMode(CULLMODE_CW);
		// ---輪郭線描画---
		XMFLOAT4 linecolor = XMFLOAT4(1.0f, 0.7f, 0.0f, 0.0f);		// 線の色
		SceneBase* scene = SCENE->GetCurrentScene();				// シーン取得
		scene->SetColorBuffer((&linecolor));						// 色設定
		GetPS(PS_OUTLINE)->Bind();		// ピクセルシェーダー設定
		GetVS(VS_OUTLINE)->Bind();		// 頂点シェーダ―設定
		m_Model->Draw();				// 描画

		// ---描画設定を元に戻す---
		SetCullMode(CULLMODE_CCW);
		linecolor = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		scene->SetColorBuffer((&linecolor));
	}

	SetBlendState(BS_NONE);
}

// ===シャドウマップに描画===
void Object3DBase::DrawShadow()
{
	if (!m_bEnable)
		return;
	if (m_Model != nullptr)
		m_Model->Draw();
}

// ===マトリックス更新===
void Object3DBase::UpdateMatrix()
{
	XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// スケールを反映
	mtxScl = XMMatrixScaling(m_transform.Scale.x, m_transform.Scale.y, m_transform.Scale.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_transform.Rot.x),XMConvertToRadians(m_transform.Rot.y), XMConvertToRadians(m_transform.Rot.z));
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);
	Quaternion::GetRotateMatrix(&mtxRot, m_qRot);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(m_transform.Pos.x, m_transform.Pos.y, m_transform.Pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックス設定
	XMStoreFloat4x4(&m_mWorld, mtxWorld);
}

// ---オブジェクトのサイズ計算
float Object3DBase::CalcObjSize(XMFLOAT3* v)
{
	return sqrt((v->x * v->x) + (v->y * v->y) + (v->z * v->z));
}

void Object3DBase::SetRot(const XMFLOAT3& vRot)
{
	m_transform.Rot = vRot;
}

void Object3DBase::SetRot(const Quaternion& qRot)
{
	m_qRot = qRot;
}

void Object3DBase::SetFront(const XMFLOAT3& front)
{
	m_vFront = front;
}

XMFLOAT3 Object3DBase::GetFront()
{
	return m_vFront;
}

void Object3DBase::SetResource(std::string pass)
{
	m_ResourcePass = pass;
	m_Model = MODEL->Load(pass);
}