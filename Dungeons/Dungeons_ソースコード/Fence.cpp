// ===��N���X����===
// ��N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "Fence.h"
#include "DebugProc.h"
#include "ModelManager.h"
#include "ObjectManager.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "ShaderList.h"
#include "TextureManager.h"

// ===�R���X�g���N�^===
Fence::Fence() : Collision3D(this)
{
	// ---����������---
	m_transform.Pos = { 0,0,0 };				// �������W
	m_transform.Rot = { 0,0,0 };				// ������]
	m_transform.Scale = { 1,1,1 };				// �����g�k
	m_id = OT_FENCE;							// �I�u�W�F�N�g�̌^�ݒ�
	m_bEnable = true;							// �X�V�E�`���
	m_col = true;								// �����蔻��L��
	m_bVisible = true;							// ���F��
	m_ResourcePass = "data/model/Fence.fbx";	// �f�ނւ̃p�X
	m_Model = MODEL->Load(m_ResourcePass);		// ���f���Ǎ�
	AddCollisionOBB(XMFLOAT3(0, 1.0f, 0), XMFLOAT3(1.5f, 2.0f, 0.2f));		// �����蔻��ǉ�
	m_bStatic = true;							// �ÓI�I�u�W�F�N�g

	m_pPS = GetPS(PS_TOON);						// �s�N�Z���V�F�[�_�[�ݒ�
	m_pVS = GetVS(VS_OBJECT);					// ���_�V�F�[�_�ݒ�
}

// ===�f�X�g���N�^===
Fence::~Fence()
{
}

// ===�X�V����===
void Fence::Update()
{
	Object3DBase::Update();			// �I�u�W�F�N�g�X�V
}

// ===�`�揈��===
void Fence::Draw()
{
	// ---�ʏ�`��---
	SetCullMode(CULLMODE_CCW);		// �w�ʃJ�����O
	SetTexturePS(TEX->Load("data/texture/ramp.png"), 1);		// �~���e�N�X�`���Z�b�g
	Object3DBase::Draw();			// �I�u�W�F�N�g�`��

	// ---�֊s���`��---
	SetCullMode(CULLMODE_CW);		// �O�ʃJ�����O
	m_pPS = GetPS(PS_OUTLINE);		// �s�N�Z���V�F�[�_�[�Z�b�g
	m_pVS = GetVS(VS_OUTLINE);		// ���_�V�F�[�_�[�Z�b�g
	Object3DBase::Draw();			// �`�揈��

	// ---�`��ݒ�����ɖ߂�---
	SetCullMode(CULLMODE_CCW);		// �w�ʃJ�����O
	m_pPS = GetPS(PS_TOON);			// �s�N�Z���V�F�[�_�[�Z�b�g
	m_pVS = GetVS(VS_OBJECT);		// ���_�V�F�[�_�[�Z�b�g

}

// ===�X�V�L��===
bool Fence::IsUpdate()
{
	return true;
}

// ===�Փˏ���===
void Fence::OnCollision(Object* obj)
{
}