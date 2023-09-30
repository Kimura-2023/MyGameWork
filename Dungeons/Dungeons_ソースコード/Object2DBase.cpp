// ===2Dオブジェクトベース
// ---Author kimura

// ====インクルード===
#include "Object2DBase.h"
#include "ObjectManager.h"
#include "Camera.h"
#include "DefineData.h"
#include "DebugProc.h"
#include "GameManager.h"
#include "CameraManager.h"
#include "Polygon.h"
#include "TextureManager.h"

// ===コンストラクタ===
Object2DBase::Object2DBase()
{
	OBJECT->AddObjectManager(this);	// オブジェクト追加
	Init();							// 初期化
}

// ===デストラクタ===
Object2DBase::~Object2DBase()
{
	OBJECT->Release(this);			// オブジェクト破棄
}

// ===更新処理===
void Object2DBase::Update()
{
	m_mWorld._41 = m_transform.Pos.x;
	m_mWorld._42 = m_transform.Pos.y;
	m_mWorld._43 = m_transform.Pos.z;

	// ---視錐台判定---
	CameraBase* pCamera = CAMERA->GetCurrentCamera();										// カメラ取得
	if (pCamera->CollisionViewFrustum(&m_transform.Pos, CalcObjSize(&m_Scale)) != 0)
	{
		m_bVisible = true;
	}
	else if (pCamera->CollisionViewFrustum(&m_transform.Pos, CalcObjSize(&m_Scale)) == 0)
	{
		m_bVisible = false;
	}

	// ---更新判定---
	if (!m_bEnable)
		return;

	// ---重力---
	if (m_bGravity)
		m_vAcc.y = -GRAVITY;

	// ---加速度反映---
	m_vVel.x += m_vAcc.x;
	m_vVel.y += m_vAcc.y;
	m_vVel.z += m_vAcc.z;

	// ---速度反映---
	m_transform.Pos.x += m_vVel.x;
	m_transform.Pos.y += m_vVel.y;
	m_transform.Pos.z += m_vVel.z;
}

// ===描画===
void Object2DBase::Draw()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();			// デバイスコンテキスト取得

	// ---描画判定---
	if (!m_bEnable)
		return;
	if (m_Texture == nullptr)
		return;

	// ---描画処理(パラメータセット)---
	SetBlendState(m_Blend);
	CPolygon::SetColor(m_color);
	CPolygon::SetTexture(m_Texture);
	CPolygon::SetSize(m_Scale.x * m_transform.Scale.x, m_Scale.y * m_transform.Scale.y);
	CPolygon::SetUV(m_uv.x, m_uv.y);
	CPolygon::SetFrameSize(m_TexCut.x, m_TexCut.y);
	CPolygon::SetPos(m_transform.Pos.x, m_transform.Pos.y);
	CPolygon::SetRotate(m_transform.Rot);
	CPolygon::SetDraw(m_bui);
	CPolygon::Draw(pDC);					// 描画

	SetZBuffer(true);
}

// ===素材パス設定===
void Object2DBase::SetResource(std::string pass)
{
	m_Texture = TEX->Load(pass);			// テクスチャ読込
	if(m_Texture)
		m_ResourcePass = pass;				// パス更新
}

// ===ブレンドステート設定===
void Object2DBase::SetUseBlendState(EBlendState blend)
{
	m_Blend = blend;
}

// ===マトリックス更新===
void Object2DBase::UpdateMatrix()
{
	XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// スケールを反映
	mtxScl = XMMatrixScaling(m_Scale.x * m_transform.Scale.x, m_Scale.y * m_transform.Scale.y,1.0f);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_transform.Rot.x),
		XMConvertToRadians(m_transform.Rot.y), XMConvertToRadians(m_transform.Rot.z));
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(m_transform.Pos.x, m_transform.Pos.y, m_transform.Pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックス設定
	XMStoreFloat4x4(&m_mWorld, mtxWorld);
}

// ===右下UV取得===
XMFLOAT2 Object2DBase::GetTexSize()
{
	XMFLOAT2 texsize;
	texsize.x = 1.0 / m_TexCut.x;
	texsize.y = 1.0 / m_TexCut.y;
	return texsize;
}

// ===カメラ距離算出===
float Object2DBase::GetCamVecSize()
{
	XMFLOAT3 cpos = CAMERA->GetCurrentCamera()->GetPos();		// カメラ座標取得
	float size;			// 距離
	Length(&size, m_transform.Pos, cpos);		// 距離算出
	return size;
}

// ===サイズ算出===
float Object2DBase::CalcObjSize(XMFLOAT2* f)
{
	return sqrt((f->x * f->x) + (f->y * f->y));
}

void Object2DBase::Init()
{
	// ---初期化処理---
	m_transform = Transform();
	m_vAcc = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vVel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Scale = XMFLOAT2(1.0f, 1.0f);

	m_uv = XMFLOAT2(0.0f, 0.0f);
	m_TexCut = XMFLOAT2(1.0f, 1.0f);

	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	XMStoreFloat4x4(&m_mWorld, XMMatrixIdentity());

	m_Blend = BS_ALPHABLEND;

	m_Texture = nullptr;

	m_bGravity = false;
	m_bEnable = false;
	m_bVisible = false;
	m_bBillboard = true;
	m_bui = true;
}