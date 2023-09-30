// ===�n�ʃN���X����===
// �n�ʃu���b�N�N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "Ground.h"
#include "DebugProc.h"
#include "ModelManager.h"
#include "ObjectManager.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "ShaderList.h"
#include "TextureManager.h"
#include "ImGuiBase.h"

// ===�R���X�g���N�^===
Ground::Ground() : Collision3D(this)
{
	// ---����������---
	m_transform.Pos = { 0,0,0 };										// �������W
	m_transform.Rot = { 0,0,0 };										// ������]
	m_transform.Scale = { 1,1,1 };										// �����g�k
	m_id = OT_GROUND;													// �I�u�W�F�N�g�̌^�ݒ�
	m_bEnable = true;													// �X�V�E�`���
	m_col = true;														// �����蔻��L��
	m_bVisible = true;													// ���F��
	m_Model = MODEL->Load("data/model/Cube.fbx");						// ���f���Ǎ�
	AddCollisionOBB(XMFLOAT3(0, 0, 0), XMFLOAT3(1.0f, 1.0f, 1.0f));		// �����蔻��ǉ�
	m_bStatic = true;													// �ÓI�I�u�W�F�N�g

	m_pPS = GetPS(PS_PHONG);									// �s�N�Z���V�F�[�_�[�ݒ�
	m_pVS = GetVS(VS_OBJECT);									// ���_�V�F�[�_�[�ݒ�

	m_ResourcePass = "data/texture/BlockTex003.png";			// �e�N�X�`���p�X�ݒ�
	m_pTex = TEX->Load("data/texture/BlockTex003.png");			// �e�N�X�`���Ǎ�
}

// ===�f�X�g���N�^===
Ground::~Ground()
{
}

// ===�X�V����===
void Ground::Update()
{
	Object3DBase::Update();				// �I�u�W�F�N�g�X�V
}

// ===�`�揈��===
void Ground::Draw()
{
	// ---�ʏ�`��---
	SetCullMode(CULLMODE_CCW);								// �w�ʃJ�����O
	SetTexturePS(m_pTex, 0);								// �e�N�X�`���Z�b�g
	SetTexturePS(TEX->Load("data/texture/ramp.png"), 1);	// �~���e�N�X�`���Z�b�g
	Object3DBase::Draw();									// �`�揈��

	// ---�֊s���`��---
	SetCullMode(CULLMODE_CW);			// �O�ʃJ�����O
	m_pPS = GetPS(PS_OUTLINE);			// �s�N�Z���V�F�[�_�[�Z�b�g
	m_pVS = GetVS(VS_OUTLINE);			// ���_�V�F�[�_�[�Z�b�g
	Object3DBase::Draw();				// �`�揈��

	// --- �`��ݒ�����ɖ߂�---
	m_pPS = GetPS(PS_TOON);
	m_pVS = GetVS(VS_OBJECT);
	SetCullMode(CULLMODE_CCW);
}

// ===�X�V�L��===
bool Ground::IsUpdate()
{
	return true;
}

// ===�Փˏ���===
void Ground::OnCollision(Object* obj)
{
}

// ===�f�ނ̃p�X�ݒ�===
void Ground::SetResource(std::string pass)
{
	m_pTex = TEX->Load(pass);
	if (m_pTex)
		m_ResourcePass = pass;
}

// ===�G�f�B�^�\��===
void Ground::ImGuiWindow()
{
	// ===�e�N�X�`���I��===
	static int idx = 0;				// �I�𒆔ԍ�

	char combo_preview_value[50];		// �I�𒆂̃p�X�\���p
	strcpy(combo_preview_value, m_ResourcePass.c_str());		// �I�𒆂̃p�X��\��

	if (ImGui::BeginCombo("Texture", combo_preview_value, ImGuiComboFlags_HeightSmall))
	{
		for (int n = 0; n < MAX_TEX; n++)
		{
			const bool is_selected = (idx == n);								// �I�����
			if (ImGui::Selectable(GetTexturePass((TEX_TYPE)n), is_selected))	// �ύX������ꍇ
				idx = n;
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
				SetResource(GetTexturePass((TEX_TYPE)idx));
			}

		}
		ImGui::EndCombo();
	}
}