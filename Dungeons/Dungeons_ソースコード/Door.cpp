// ===�h�A�N���X����===
// �h�A�N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "Door.h"
#include "DebugProc.h"
#include "ModelManager.h"
#include "ObjectManager.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "ShaderList.h"
#include "TextureManager.h"
#include "MapManager.h"

// ===�R���X�g���N�^===
Door::Door() : Collision3D(this)
{
	// ---����������---
	m_transform.Pos = { 0,0,0 };								// �������W
	m_transform.Rot = { 0,0,0 };								// ������]
	m_transform.Scale = { 1,1,1 };								// �����g�k
	m_id = OT_DOOR;												// �I�u�W�F�N�g�̌^�ݒ�
	m_bEnable = true;											// �X�V�E�`���
	m_col = true;												// �����蔻��L��
	m_bVisible = true;											// ���F��
	m_Model = MODEL->Load("data/model/door.fbx");				// ���f���Ǎ�
	AddCollisionOBB(XMFLOAT3(0, 0, 0), XMFLOAT3(1.0f, 1.0f, 0.6f));	// �����蔻��ǉ�
	m_bStatic = true;							// �ÓI�I�u�W�F�N�g

	m_pPS = GetPS(PS_MASKOBJECT);				// �s�N�Z���V�F�[�_�[�Z�b�g
	m_pVS = GetVS(VS_OBJECT);					// ���_�V�F�[�_�[�Z�b�g
	m_ResourcePass = "data/model/door.fbx";		// �f�ނւ̃p�X

	m_nDirection = 0;							// ����������
	m_Entrance = XMFLOAT3(0.0f, 0.0f, 0.0f);	// �J�n�ʒu������
}

// ===�f�X�g���N�^===
Door::~Door()
{
}

// ===�X�V����===
void Door::Update()
{
	Object3DBase::Update();					// �I�u�W�F�N�g�X�V
}

// ===�`�揈��===
void Door::Draw()
{
	SetCullMode(CULLMODE_CCW);								// �w�ʃJ�����O
	SetTexturePS(TEX->Load("data/texture/ramp.png"), 1);	// �~���e�N�X�`���Z�b�g
	Object3DBase::Draw();									// �`�揈��

	// ---�֊s���`��---
	SetCullMode(CULLMODE_CW);		// �O�ʃJ�����O
	m_pPS = GetPS(PS_OUTLINE);	// �s�N�Z���V�F�[�_�[�Z�b�g
	m_pVS = GetVS(VS_OUTLINE);	// ���_�V�F�[�_�[�Z�b�g
	Object3DBase::Draw();			// �`�揈��

	// ---�`��ݒ�����ɖ߂�---
	SetCullMode(CULLMODE_CCW);		// �w�ʃJ�����O
	m_pPS = GetPS(PS_MASKOBJECT);	// �s�N�Z���V�F�[�_�[�Z�b�g
	m_pVS = GetVS(VS_OBJECT);		// ���_�V�F�[�_�[�Z�b�g
}

// ===�X�V�L��===
bool Door::IsUpdate()
{
	return true;
}

// ===�Փˏ���===
void Door::OnCollision(Object* obj)
{

}

// ===�����ݒ�===
void Door::SetDirection()
{
	m_nDirection = (int)(m_transform.Rot.y / 90) % 4;			// ���ۂ̌���������p���Z�o����

	// ---�v���C���[�̏o���ʒu���Z�b�g---
	float rad = m_transform.Rot.y * (float)M_PI / 180.0f;		// �␳�������
	m_Entrance.x = 3.0f * -sinf(rad) + m_transform.Pos.x;		// �␳����lx����
	m_Entrance.z = 3.0f * -cosf(rad) + m_transform.Pos.z;		// �␳����lz����
	m_Entrance.y = m_transform.Pos.y;							// �����͂��̂܂�
}

// ===�h�A�J��===
void Door::Open()
{
	m_Model = MODEL->Load("data/model/OpenDoor.fbx");			// ���f�����h�A�J����̃��m�ɒu��������
}