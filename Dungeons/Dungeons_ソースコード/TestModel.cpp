#include "TestModel.h"
#include "ConstantBuffer.h"
#include "Camera.h"
#include "ShaderList.h"
#include "ModelManager.h"
#include "Input.h"
#include "DebugProc.h"
#include "CameraManager.h"

TestModel::TestModel()
{
	m_vPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vScale = XMFLOAT3(0.01f, 0.01f, 0.01f);
	m_vRotate = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_qRot = Quaternion(XMFLOAT3(0.0f, 1.0f, 0.0f), 0.0f);
	m_vFront = XMFLOAT3(0.0f, 0.0f, 1.0f);

	mTime = 0;

	XMStoreFloat4x4(&m_mWorld, XMMatrixIdentity());

	m_pModel = MODEL->Load("data/model/humananime4.fbx");
	m_pModel->AddAnime("data/model/humananime4.fbx");
	m_pModel->AddAnime("data/model/humantest.fbx");
	m_pCBuffer = new ConstantBuffer;
	m_pCBuffer->Create(sizeof(XMFLOAT4X4) * 3);

	m_pCBlight = new ConstantBuffer;
	m_pCBlight->Create(sizeof(XMFLOAT4));

}

TestModel::~TestModel()
{
	delete m_pCBlight;
	delete m_pCBuffer;
}

void TestModel::Update()
{
	float time = (float)mTime;
	++mTime;
	//m_pModel->Anime(time);

	if (CInput::GetKeyPress(VK_D))
	{
		m_pModel->PlayAnime(time, 0);
		m_vFront.x = 1.0f;
		m_vFront.z = 0.0f;

		m_vPos.x += m_vFront.x * 0.02f;
		m_vPos.z += m_vFront.z * 0.02f;
	}

	if (CInput::GetKeyPress(VK_W))
	{
		m_pModel->PlayAnime(time, 0);
		m_vFront.x = 0.0f;
		m_vFront.z = 1.0f;

		m_vPos.x += m_vFront.x * 0.02f;
		m_vPos.z += m_vFront.z * 0.02f;
	}

	if (CInput::GetKeyPress(VK_A))
	{
		m_pModel->PlayAnime(time, 0);
		m_vFront.x = -1.0f;
		m_vFront.z = 0.0f;

		m_vPos.x += m_vFront.x * 0.02f;
		m_vPos.z += m_vFront.z * 0.02f;
	}

	if (CInput::GetKeyPress(VK_S))
	{
		m_pModel->PlayAnime(time, 0);
		m_vFront.x = 0.0f;
		m_vFront.z = -1.0f;

		m_vPos.x += m_vFront.x * 0.02f;
		m_vPos.z += m_vFront.z * 0.02f;
	}

	//Quaternion q, a;
	//q = Quaternion(m_vFront);

	Quaternion::LookRotation(&m_qRot, m_vFront, XMFLOAT3(0.0f, 1.0f, 0.0f));

	XMFLOAT3 vZ = XMFLOAT3(m_mWorld._31, m_mWorld._32, m_mWorld._33);
	XMFLOAT3 vY = XMFLOAT3(m_mWorld._21, m_mWorld._22, m_mWorld._23);


	CDebugProc::Print("[vZ]%f,%f,%f\n", vZ.x, vZ.y, vZ.z);
	CDebugProc::Print("[UP]%f,%f,%f\n", vY.x, vY.y, vY.z);
	//a = Quaternion(vZ);

	//Quaternion::SLerp(&m_qRot, a, q, 1.0f);
	//CDebugProc::Print("[Q]%f,%f,%f\n", m_qRot.x, m_qRot.y, m_qRot.z);

	XMMATRIX mtxWorld, mtxScl, mtxRot, mtxQuaternion, mtxTranslate;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// スケールを反映
	mtxScl = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// 回転を反映
	//mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_vRotate.x),
	//	XMConvertToRadians(m_vRotate.y), XMConvertToRadians(m_vRotate.z));

	Quaternion::GetRotateMatrix(&mtxQuaternion, m_qRot);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxQuaternion);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックス設定
	XMStoreFloat4x4(&m_mWorld, mtxWorld);
}

void TestModel::Draw()
{
	//SetZWrite(false);
	// 変換行列作成
	DirectX::XMMATRIX mat[3];
	DirectX::XMFLOAT4X4 fMat[3];

	mat[0] = XMLoadFloat4x4(&m_mWorld);								// ワールド変換行列
	mat[1] = XMLoadFloat4x4(&CAMERA->GetCurrentCamera()->GetViewMatrix());		// ビュー行列
	mat[2] = XMLoadFloat4x4(&CAMERA->GetCurrentCamera()->GetProjMatrix());		// プロジェクション行列

	DirectX::XMStoreFloat4x4(&fMat[0], XMMatrixTranspose(mat[0]));
	DirectX::XMStoreFloat4x4(&fMat[1], XMMatrixTranspose(mat[1]));
	DirectX::XMStoreFloat4x4(&fMat[2], XMMatrixTranspose(mat[2]));

	m_pCBuffer->Write(fMat);		// 定数バッファにセット

	XMFLOAT4 lightdir = { -1.0f,-1.0f,0.0f,0.0f };
	m_pCBlight->Write(&lightdir);

	m_pCBlight->BindPS(0);
	m_pCBuffer->BindVS(0);

	m_pModel->Draw();
}