// ===�S�[���I�u�W�F�N�g�N���X����===
// �S�[���I�u�W�F�N�g�N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "Goal.h"
#include "DebugProc.h"
#include "ModelManager.h"
#include "ObjectManager.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "ShaderList.h"
#include "TextureManager.h"

// ===�R���X�g���N�^===
Goal::Goal() : Collision3D(this)
{
	// ---����������---
	m_transform.Pos = { 0,0,0 };				// �������W
	m_transform.Rot = { 0,0,0 };				// ������]
	m_transform.Scale = { 1,1,1 };				// �����g�k
	m_id = OT_GOAL;								// �I�u�W�F�N�g�̌^�ݒ�
	m_bEnable = true;							// �X�V�E�`���
	m_col = true;								// �����蔻��L��
	m_bVisible = true;							// ���F��
	m_ResourcePass = "data/model/goal.fbx";		// �f�ނւ̃p�X
	m_Model = MODEL->Load(m_ResourcePass);		// ���f���Ǎ�
	AddCollisionSphere(XMFLOAT3(0.0f, 0.0f, 0.0f), 1.0f);		// �����蔻��ǉ�
	m_bStatic = true;							// �ÓI�I�u�W�F�N�g

	m_pPS = GetPS(PS_PHONG);					// �s�N�Z���V�F�[�_�[�ݒ�
	m_pVS = GetVS(VS_OBJECT);					// ���_�V�F�[�_�ݒ�
	m_Rad = 0.0f;
}

// ===�f�X�g���N�^===
Goal::~Goal()
{
}

// ===�X�V����===
void Goal::Update()
{
	// ---���X�ɏ㉺�ړ�---
	++m_Rad;				// sinf��-1�`1�̒l���擾���邽�߂ɒl���X�V
	if (m_Rad > 360.0f)				// ���ȏ�(360�𒴂����烊�Z�b�g
		m_Rad = 0;					// �l�����Z�b�g
	m_transform.Pos.y += sinf(m_Rad * M_PI / 180.0f) * 0.005f;			// ���W�ɔ��f������

	Object3DBase::Update();			// �I�u�W�F�N�g�X�V
}

// ===�`�揈��===
void Goal::Draw()
{
	// ---�ʏ�`��---
	SetCullMode(CULLMODE_CCW);								// �w�ʃJ�����O
	SetTexturePS(TEX->Load("data/texture/ramp.png"), 1);	// �~���e�N�X�`���Z�b�g
	Object3DBase::Draw();									// �I�u�W�F�N�g�`��

	// ---�֊s���`��---
	SetCullMode(CULLMODE_CW);								// �O�ʃJ�����O
	m_pPS = GetPS(PS_OUTLINE);								// �s�N�Z���V�F�[�_�[�Z�b�g
	m_pVS = GetVS(VS_OUTLINE);								// ���_�V�F�[�_�[�Z�b�g
	XMFLOAT4 color = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);		// ���F(����������������
	SCENE->GetCurrentScene()->SetColorBuffer(&color);		// �F��萔�o�b�t�@�ɃZ�b�g
	Object3DBase::Draw();									// �`�揈��

	// ---�`��ݒ�����ɖ߂�---
	SetCullMode(CULLMODE_CCW);		// �w�ʃJ�����O
	m_pPS = GetPS(PS_PHONG);		// �s�N�Z���V�F�[�_�[�Z�b�g
	m_pVS = GetVS(VS_OBJECT);		// ���_�V�F�[�_�[�Z�b�g
	color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);				// ���̐F�ɖ߂�
	SCENE->GetCurrentScene()->SetColorBuffer(&color);		// �萔�o�b�t�@�ɃZ�b�g

}

// ===�X�V�L��===
bool Goal::IsUpdate()
{
	return true;
}

// ===�Փˏ���===
void Goal::OnCollision(Object* obj)
{
}