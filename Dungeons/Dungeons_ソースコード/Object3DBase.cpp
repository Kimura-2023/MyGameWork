// ===3D�I�u�W�F�N�g�x�[�X�N���X===
// Author kimura
//==============================

// ===�C���N���[�h===
#include "Object3DBase.h"
#include "ObjectManager.h"
#include "DefineData.h"
#include "ModelManager.h"
#include "DebugProc.h"
#include "CameraManager.h"
#include "ShaderList.h"
#include "SceneManager.h"

// ===�O���[�o��===
std::shared_ptr<ConstantBuffer>Object3DBase::m_pWVPBuffer = nullptr;
int Object3DBase::m_nObjCnt = 0;

// ===�R���X�g���N�^===
Object3DBase::Object3DBase()
{
	// ---�o�b�t�@������---
	if (m_nObjCnt == 0)
	{
		m_pWVPBuffer = std::make_shared<ConstantBuffer>();		// �o�b�t�@����
		m_pWVPBuffer->Create(sizeof(XMFLOAT4X4) * 3);
	}
	m_nObjCnt++;		// �I�u�W�F�N�g�����X�V

	// ---����������---
	OBJECT->AddObjectManager(this);							// �I�u�W�F�N�g�Ǘ����ɒǉ�
	m_Model = nullptr;										// ���f��
	m_pPS = GetPS(PS_LAMBERT);								// �s�N�Z���V�F�[�_�[
	m_pVS = GetVS(VS_OBJECT);								// ���_�V�F�[�_�[

	m_transform = Transform();								// ���W�E��]�E�g�k
	m_qRot = Quaternion(XMFLOAT3(0.0f, 1.0f, 0.0f), 0.0f);	// ��]
	m_vFront = XMFLOAT3(0.0f, 0.0f, 1.0f);					// ���ʕ���

	m_vVel = XMFLOAT3(0.0f, 0.0f, 0.0f);					// ���x
	m_vAcc = XMFLOAT3(0.0f, 0.0f, 0.0f);					// �����x

	XMStoreFloat4x4(&m_mWorld, XMMatrixIdentity());			// �s�񏉊���
	m_bGravity = false;										// �d�͐ݒ�
	m_bEnable = false;										// �X�V�E�`���
	m_bVisible = false;										// ���F��
}

// ===�f�X�g���N�^===
Object3DBase::~Object3DBase()
{
	OBJECT->Release(this);
	m_nObjCnt--;
	if (m_nObjCnt <= 0)
	{
		m_pWVPBuffer.reset();
	}
}

// ===�X�V===
void Object3DBase::Update()
{
	m_mWorld._41 = m_transform.Pos.x;
	m_mWorld._42 = m_transform.Pos.y;
	m_mWorld._43 = m_transform.Pos.z;

	// ---�����䔻��---
	CameraBase* pCamera = CAMERA->GetCurrentCamera();			// �J�����擾
	if (pCamera->CollisionViewFrustum(&m_transform.Pos, CalcObjSize(&m_transform.Scale)) != 0)
		m_bVisible = true;
	else if(pCamera->CollisionViewFrustum(&m_transform.Pos, CalcObjSize(&m_transform.Scale)) == 0)
		m_bVisible = false;

	// ---�X�V����---
	if (!m_bEnable)
		return;
	if (!m_bVisible)
		return;

	// ---�d��---
	if (m_bGravity)
		m_vAcc.y = -GRAVITY;

	// ---�����x�𑬓x�ɔ��f������---
	m_vVel.x += m_vAcc.x;				// x������
	m_vVel.y += m_vAcc.y;				// y������
	m_vVel.z += m_vAcc.z;				// z������

	// ---���x�����W�ɉ��Z---
	m_transform.Pos.x += m_vVel.x;		// x������
	m_transform.Pos.y += m_vVel.y;		// y������
	m_transform.Pos.z += m_vVel.z;		// z������

	// ---�i�s�����ɏ��X�ɉ�]������---
	Quaternion q = Quaternion::Identity();									// ������
	Quaternion::LookRotation(&q, m_vFront, XMFLOAT3(0.0f, 1.0f, 0.0f));		// ���ʕ����������N�H�[�^�j�I�������߂�
	Quaternion::SLerp(&m_qRot, m_qRot, q, 0.25f);							// ���

	// ---�s��X�V---
	UpdateMatrix();

	// ---���G���ԍX�V---
	m_nInvincibleTime--;
	if (m_nInvincibleTime <= 0)
	{
		m_nInvincibleTime = 0;
	}
}

// ===�I�u�W�F�N�g�`��===
void Object3DBase::Draw()
{
	// ---�V�F�[�_�[�Z�b�g---
	m_pPS->Bind();
	m_pVS->Bind();

	// ---�`�攻��---
	if (!m_bEnable)
		return;
	if (!m_bVisible)
		return;

	// ---�ϊ��s��쐬---
	DirectX::XMMATRIX mat[3];
	DirectX::XMFLOAT4X4 fMat[3];

	mat[0] = XMLoadFloat4x4(&m_mWorld);											// ���[���h�ϊ��s��
	mat[1] = XMLoadFloat4x4(&CAMERA->GetCurrentCamera()->GetViewMatrix());		// �r���[�s��
	mat[2] = XMLoadFloat4x4(&CAMERA->GetCurrentCamera()->GetProjMatrix());		// �v���W�F�N�V�����s��

	// ---�s��]�u(�V�F�[�_�[�ɒl��n������)---
	DirectX::XMStoreFloat4x4(&fMat[0], XMMatrixTranspose(mat[0]));			// ���[���h�ϊ��s��
	DirectX::XMStoreFloat4x4(&fMat[1], XMMatrixTranspose(mat[1]));			// �r���[�s��
	DirectX::XMStoreFloat4x4(&fMat[2], XMMatrixTranspose(mat[2]));			// �v���W�F�N�V�����s��

	// ---�l���Z�b�g---
	m_pWVPBuffer->Write(fMat);		// �萔�o�b�t�@�ɃZ�b�g
	m_pWVPBuffer->BindVS(0);

	if(m_Model != nullptr)
		m_Model->Draw();			// �`��

	// ---�G�f�B�^�`��(�֊s��)---
	if (m_bSelect)			// �I����Ԕ���
	{
		SetCullMode(CULLMODE_CW);
		// ---�֊s���`��---
		XMFLOAT4 linecolor = XMFLOAT4(1.0f, 0.7f, 0.0f, 0.0f);		// ���̐F
		SceneBase* scene = SCENE->GetCurrentScene();				// �V�[���擾
		scene->SetColorBuffer((&linecolor));						// �F�ݒ�
		GetPS(PS_OUTLINE)->Bind();		// �s�N�Z���V�F�[�_�[�ݒ�
		GetVS(VS_OUTLINE)->Bind();		// ���_�V�F�[�_�\�ݒ�
		m_Model->Draw();				// �`��

		// ---�`��ݒ�����ɖ߂�---
		SetCullMode(CULLMODE_CCW);
		linecolor = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		scene->SetColorBuffer((&linecolor));
	}

	SetBlendState(BS_NONE);
}

// ===�V���h�E�}�b�v�ɕ`��===
void Object3DBase::DrawShadow()
{
	if (!m_bEnable)
		return;
	if (m_Model != nullptr)
		m_Model->Draw();
}

// ===�}�g���b�N�X�X�V===
void Object3DBase::UpdateMatrix()
{
	XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScl = XMMatrixScaling(m_transform.Scale.x, m_transform.Scale.y, m_transform.Scale.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_transform.Rot.x),XMConvertToRadians(m_transform.Rot.y), XMConvertToRadians(m_transform.Rot.z));
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);
	Quaternion::GetRotateMatrix(&mtxRot, m_qRot);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(m_transform.Pos.x, m_transform.Pos.y, m_transform.Pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�ݒ�
	XMStoreFloat4x4(&m_mWorld, mtxWorld);
}

// ---�I�u�W�F�N�g�̃T�C�Y�v�Z
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