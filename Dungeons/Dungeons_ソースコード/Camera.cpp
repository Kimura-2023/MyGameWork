// ===�J�����x�[�X�N���X===
#include "Camera.h"
#include "CameraManager.h"
#include "Light.h"

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
namespace {
	const float CAM_POS_P_X = 0.0f;					// �J�����̎��_�����ʒu(X���W)
	const float CAM_POS_P_Y = 10.0f;				// �J�����̎��_�����ʒu(Y���W)
	const float CAM_POS_P_Z = -10.0f;				// �J�����̎��_�����ʒu(Z���W)
	const float CAM_POS_R_X = 0.0f;					// �J�����̒����_�����ʒu(X���W)
	const float CAM_POS_R_Y = 0.0f;					// �J�����̒����_�����ʒu(Y���W)
	const float CAM_POS_R_Z = 0.0f;					// �J�����̒����_�����ʒu(Z���W)
	const float VIEW_ANGLE = 45.0f;					// ����p
	const float VIEW_ASPECT = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;	// �A�X�y�N�g��
	const float VIEW_NEAR_Z = 0.3f;					// NearZ�l
	const float VIEW_FAR_Z = 1000.0f;				// FarZ�l

}

// �R���X�g���N�^
CameraBase::CameraBase()
{
	Init();
}

// ������
void CameraBase::Init()
{
	m_pSky = nullptr;

	m_vPos = XMFLOAT3(CAM_POS_P_X, CAM_POS_P_Y, CAM_POS_P_Z);		// ���_
	m_vTarget = XMFLOAT3(CAM_POS_R_X, CAM_POS_R_Y, CAM_POS_R_Z);	// �����_
	m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);		// ����x�N�g��
	m_vPosGap = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_fAspectRatio = VIEW_ASPECT;			// �c����
	m_fFovY = VIEW_ANGLE;					// ����p(Degree)
	m_fNearZ = VIEW_NEAR_Z;					// �O���N���b�v����
	m_fFarZ = VIEW_FAR_Z;					// ����N���b�v����

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
	fTan *= m_fAspectRatio;			//�A�X�y�N�g����|����
	m_frus[2].x = 1.0f;		m_frus[2].y = 0.0f;
	m_frus[2].z = fTan;	m_frus[2].w = 0.0f;
	m_frus[3].x = -1.0f;	m_frus[3].y = 0.0f;
	m_frus[3].z = fTan;	m_frus[3].w = 0.0f;
	m_frus[4].x = 0.0f;		m_frus[4].y = 0.0f;
	m_frus[4].z = 1.0f;		m_frus[4].w = -m_fNearZ;
	m_frus[5].x = 0.0f;		m_frus[5].y = 0.0f;
	m_frus[5].z = -1.0f;	m_frus[5].w = m_fFarZ;

	//���K��
	for (int i = 0; i < 4; ++i)
		XMStoreFloat4(&m_frus[i],XMPlaneNormalize(XMLoadFloat4(&m_frus[i])));

	CalcWorldMatrix();
}

// �X�V
void CameraBase::Update()
{
	m_vFront = { m_vTarget.x - m_vPos.x, m_vTarget.y - m_vPos.y, m_vTarget.z - m_vPos.z };		// ���_���璍���_�ւ̃x�N�g�����X�V
	CalcWorldMatrix();
	XMMATRIX mW = XMLoadFloat4x4(&m_mtxWorld);

	// ���ʂ����[���h��Ԃɔz�u
	mW = XMMatrixInverse(nullptr, mW);
	mW = XMMatrixTranspose(mW);
	for (int i = 0; i < 6; ++i)
		XMStoreFloat4(&m_frusw[i], XMPlaneTransform(XMLoadFloat4(&m_frus[i]), mW));

	// �}�g���b�N�X�X�V
	UpdateMatrix();
}

// ��ʃN���A
void CameraBase::Clear()
{
	// main.cpp�̃��\�[�X�Q��
	extern ID3D11RenderTargetView* g_pRenderTargetView;
	extern ID3D11DepthStencilView* g_pDepthStencilView;

	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	ID3D11DeviceContext* pDC = GetDeviceContext();
	pDC->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
	pDC->ClearDepthStencilView(g_pDepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	if (m_pSky) {
		SetZBuffer(false);		// Z�o�b�t�@����
		CLight* pLight = CLight::Get();
		pLight->SetDisable();	// ���C�e�B���O����
		XMFLOAT4X4 mW;
		XMStoreFloat4x4 (&mW, XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z));
		m_pSky->Draw(mW);			// �X�J�C�h�[���`��
		pLight->SetEnable();	// ���C�e�B���O�L��
	}

	SetZBuffer(true);			// Z�o�b�t�@�L��
	SetBlendState(BS_NONE);
}

// �r���[/�v���W�F�N�V���� �}�g���b�N�X�X�V
void CameraBase::UpdateMatrix()
{
	XMFLOAT3 pos = XMFLOAT3(m_vPos.x + m_vPosGap.x, m_vPos.y + m_vPosGap.y, m_vPos.z + m_vPosGap.z);
	XMFLOAT3 target = XMFLOAT3(m_vTarget.x + m_vPosGap.x, m_vTarget.y + m_vPosGap.y, m_vTarget.z + m_vPosGap.z);
	XMStoreFloat4x4(&m_mtxView, XMMatrixLookAtLH(
		XMLoadFloat3(&pos), XMLoadFloat3(&target), XMLoadFloat3(&m_vUp)));
	XMStoreFloat4x4(&m_mtxProj, XMMatrixPerspectiveFovLH(
		XMConvertToRadians(m_fFovY), m_fAspectRatio, m_fNearZ, m_fFarZ));
}

// ���[���h �}�g���b�N�X�ݒ�
void CameraBase::SetWorldMatrix(XMFLOAT4X4& mtxWorld)
{
	m_mtxWorld = mtxWorld;
	m_vPos = XMFLOAT3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
	m_vTarget = XMFLOAT3(mtxWorld._41 + mtxWorld._31, mtxWorld._42 + mtxWorld._32, mtxWorld._43 + mtxWorld._33);
	m_vUp = XMFLOAT3(mtxWorld._21, mtxWorld._22, mtxWorld._23);
}

// ���_/�����_/����x�N�g�����烏�[���h �}�g���b�N�X�Z�o
XMFLOAT4X4& CameraBase::CalcWorldMatrix()
{
	// ---�J�����̎p�������߂�---
	XMVECTOR vecZ = XMVectorSet(m_vTarget.x - m_vPos.x, m_vTarget.y - m_vPos.y, m_vTarget.z - m_vPos.z, 0.0f);		// Z���������Z�o
	XMFLOAT3 vZ;
	XMStoreFloat3(&vZ, XMVector3Normalize(vecZ));						// XMFLOAT3�^�ɕϊ�
	XMVECTOR vecY = XMLoadFloat3(&m_vUp);								// ����x�N�g�����擾
	XMVECTOR vecX = XMVector3Normalize(XMVector3Cross(vecY, vecZ));		// X���������Z�o
	XMFLOAT3 vX;
	XMStoreFloat3(&vX, vecX);											// XMFLOAT3�^�ɕϊ�
	vecY = XMVector3Normalize(XMVector3Cross(vecZ, vecX));				// Y���������Z�o
	XMFLOAT3 vY;
	XMStoreFloat3(&vY, vecY);											// XMFLOAT3�^�ɕϊ�

	// ---���[���h�}�g���b�N�X�Z�o---
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

// ������N���b�v
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
		if (fDot < -fRadius) return 0;//���S�ɊO
		if (fDot <= fRadius) bHit = true;
	}
	if (bHit) return -1;	//�ʂ��ׂ�
	return 1;	//���S�ɓ���
}

Quaternion CameraBase::GetXZQuaternion()
{
	XMFLOAT3 front = { 0.0f,1.0f,0.0f};		// ��̐���
	XMFLOAT3 target = m_vTarget;
	Normalize(&target);						// ���݂̒����_�ւ̃x�N�g���𐳋K��
	target.y = 0;
	Quaternion q;
	Quaternion::LookAt(&q, front, target);
	return q;
}