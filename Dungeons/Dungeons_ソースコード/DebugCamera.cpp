#include "DebugCamera.h"
#include "Input.h"
#include "ModelManager.h"

enum DebugCameraKind
{
	CAM_DCC_NONE,
	CAM_DCC_ORBIT,
	CAM_DCC_TRACK,
	CAM_DCC_DOLLY,
	CAM_DCC_FLIGHT,
};

DebugCamera::DebugCamera()
	: m_state(CAM_DCC_NONE)
	, m_oldPos{ 0, 0 }
{
	SetSky(MODEL->Load("data/model/sky.fbx"));
	CameraBase::Update();		// 初期情報でカメラの行列を更新
}
DebugCamera::~DebugCamera()
{
}
void DebugCamera::Update()
{
	UpdateState();
	if (m_state == CAM_DCC_NONE) return;

	Argument arg;
	// マウス移動量
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	arg.mouseMove = DirectX::XMFLOAT2((float)cursorPos.x - m_oldPos.x, (float)cursorPos.y - m_oldPos.y);
	m_oldPos = cursorPos;
	// カメラ情報
	arg.vCamPos = DirectX::XMLoadFloat3(&m_vPos);
	arg.vCamLook = DirectX::XMLoadFloat3(&m_vTarget);
	DirectX::XMVECTOR vCamUp = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&m_vUp));
	DirectX::XMVECTOR vFront = DirectX::XMVectorSubtract(arg.vCamLook, arg.vCamPos);
	// カメラ姿勢
	arg.vCamFront = DirectX::XMVector3Normalize(vFront);
	arg.vCamSide = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vCamUp, arg.vCamFront));
	arg.vCamUp = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(arg.vCamFront, arg.vCamSide));
	// フォーカス距離
	DirectX::XMStoreFloat(&arg.focus, DirectX::XMVector3Length(vFront));

	switch (m_state)
	{
	case CAM_DCC_ORBIT:		UpdateOrbit(arg);	break;
	case CAM_DCC_TRACK:		UpdateTrack(arg);	break;
	case CAM_DCC_DOLLY:		UpdateDolly(arg);	break;
	case CAM_DCC_FLIGHT:	UpdateFlight(arg);	break;
	}

	CameraBase::Update();
}
void DebugCamera::UpdateState()
{
	DebugCameraKind prev = (DebugCameraKind)m_state;
	if (CInput::GetKeyPress(VK_MENU))
	{
		m_state = CAM_DCC_NONE;
		if (CInput::GetKeyPress(VK_L)) m_state = CAM_DCC_ORBIT;
		if (CInput::GetKeyPress(VK_M)) m_state = CAM_DCC_TRACK;
		if (CInput::GetKeyPress(VK_R)) m_state = CAM_DCC_DOLLY;
	}
	else if (CInput::GetMouseButton(MOUSEBUTTON_R))
	{
		m_state = CAM_DCC_FLIGHT;
	}
	else
	{
		m_state = CAM_DCC_NONE;
	}
	if (prev != m_state)
	{
		GetCursorPos(&m_oldPos);
	}
}
void DebugCamera::UpdateOrbit(Argument& arg)
{
	// マウスの移動量 / 画面サイズ の比率から、画面全体でどれだけ回転するか指定する。
	float angleX = 360.0f * arg.mouseMove.x / 1280.0f;
	float angleY = 180.0f * arg.mouseMove.y / 720.0f;

	// Y軸回転でカメラの横軸を回転
	DirectX::XMMATRIX matRotY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angleX));
	DirectX::XMVECTOR vAxis = DirectX::XMVector3Normalize(DirectX::XMVector3TransformCoord(arg.vCamSide, matRotY));

	// 回転後の軸で縦回転
	DirectX::XMMATRIX matRotSide = DirectX::XMMatrixRotationAxis(vAxis, DirectX::XMConvertToRadians(angleY));
	DirectX::XMVECTOR vRelative = DirectX::XMVectorScale(arg.vCamFront, arg.focus);
	vRelative = DirectX::XMVector3TransformCoord(vRelative, matRotY * matRotSide);

	// カメラ座標更新
	DirectX::XMStoreFloat3(&m_vPos, DirectX::XMVectorSubtract(arg.vCamLook, vRelative));

	// アップベクトル更新
	DirectX::XMStoreFloat3(&m_vUp, DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMVector3Normalize(vRelative), vAxis)));

}
void DebugCamera::UpdateTrack(Argument& arg)
{
	// 高さA、底辺Bとする三角形について tanΘ = A / Bが成り立つ
	// 上記式をもとに割り出した遠景の高さを、移動量 / 画面サイズ の比率から、移動量として求める
	float farScreenHeight = tanf(m_fFovY * 0.5f) * m_fFarZ;
	float screenRateW = arg.mouseMove.x / 640.0f;
	float screenRateH = arg.mouseMove.y / 360.0f;
	float farMoveX = -farScreenHeight * screenRateW * m_fAspectRatio;
	float farMoveY = farScreenHeight * screenRateH;

	// カメラの姿勢をもとに移動
	float rate = arg.focus / m_fFarZ;
	DirectX::XMVECTOR vCamMove = DirectX::XMVectorZero();
	vCamMove = DirectX::XMVectorAdd(vCamMove, DirectX::XMVectorScale(arg.vCamSide, farMoveX * rate));
	vCamMove = DirectX::XMVectorAdd(vCamMove, DirectX::XMVectorScale(arg.vCamUp, farMoveY * rate));
	DirectX::XMStoreFloat3(&m_vPos, DirectX::XMVectorAdd(arg.vCamPos, vCamMove));
	DirectX::XMStoreFloat3(&m_vTarget, DirectX::XMVectorAdd(arg.vCamLook, vCamMove));
}
void DebugCamera::UpdateDolly(Argument& arg)
{
	float rate = arg.focus / m_fFarZ;
	DirectX::XMVECTOR vMove = DirectX::XMVectorScale(arg.vCamFront, m_fFarZ * 0.01f * rate * (arg.mouseMove.x + arg.mouseMove.y));
	DirectX::XMStoreFloat3(&m_vPos, DirectX::XMVectorAdd(arg.vCamPos, vMove));
}
void DebugCamera::UpdateFlight(Argument& arg)
{
	// マウスの移動量 / 画面サイズ の比率から、画面全体でどれだけ回転するか指定する。
	float angleX = 360.0f * arg.mouseMove.x / 1280.0f;
	float angleY = 180.0f * arg.mouseMove.y / 720.0f;

	// 横回転
	DirectX::XMMATRIX matUpRot = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angleX));
	DirectX::XMVECTOR vSideAxis = DirectX::XMVector3Normalize(DirectX::XMVector3TransformCoord(arg.vCamSide, matUpRot));

	// 縦回転
	DirectX::XMMATRIX matSideRot = DirectX::XMMatrixRotationAxis(vSideAxis, DirectX::XMConvertToRadians(angleY));
	DirectX::XMVECTOR vFrontAxis = DirectX::XMVector3Normalize(DirectX::XMVector3TransformCoord(arg.vCamFront, matUpRot * matSideRot));

	// キー入力で移動
	DirectX::XMVECTOR vCamMove = DirectX::XMVectorZero();
	if (CInput::GetKeyPress(VK_W))
		vCamMove = DirectX::XMVectorAdd(vCamMove, vFrontAxis);
	if (CInput::GetKeyPress(VK_S))
		vCamMove = DirectX::XMVectorSubtract(vCamMove, vFrontAxis);
	if (CInput::GetKeyPress(VK_A))
		vCamMove = DirectX::XMVectorSubtract(vCamMove, vSideAxis);
	if (CInput::GetKeyPress(VK_D))
		vCamMove = DirectX::XMVectorAdd(vCamMove, vSideAxis);
	if (CInput::GetKeyPress(VK_Q))
		vCamMove = DirectX::XMVectorAdd(vCamMove, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	if (CInput::GetKeyPress(VK_E))
		vCamMove = DirectX::XMVectorAdd(vCamMove, DirectX::XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f));

	float rate = 0.1f;
	if (CInput::GetKeyPress(VK_SHIFT))
		rate = 0.3f;
	vCamMove = DirectX::XMVectorScale(vCamMove, rate);

	// 更新
	DirectX::XMVECTOR vCamPos = DirectX::XMVectorAdd(arg.vCamPos, vCamMove);
	DirectX::XMStoreFloat3(&m_vPos, vCamPos);
	DirectX::XMStoreFloat3(&m_vTarget, DirectX::XMVectorAdd(vCamPos, DirectX::XMVectorScale(vFrontAxis, arg.focus)));
	DirectX::XMStoreFloat3(&m_vUp, DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vFrontAxis, vSideAxis)));
}