// ===�w�i�I�u�W�F�N�g�N���X����===
// �w�i�I�u�W�F�N�g�N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "ModelObject.h"
#include "DebugProc.h"
#include "ModelManager.h"
#include "ObjectManager.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "ShaderList.h"
#include "TextureManager.h"
#include "ImGuiBase.h"

// ===�R���X�g���N�^===
ModelObject::ModelObject()
{
	// ---����������---
	m_transform.Pos = { 0,0,0 };					// �������W
	m_transform.Rot = { 0,0,0 };					// ������]
	m_transform.Scale = { 1,1,1 };					// �����g�k
	m_id = BG_3DOBJ;								// �I�u�W�F�N�g�^
	m_bEnable = true;								// �X�V�E�`���
	m_col = true;									// �����蔻��L��
	m_bVisible = true;								// �����
	m_Model = MODEL->Load("data/model/Cube.fbx");	// ���f���Ǎ�
	m_bStatic = true;								// �ÓI�I�u�W�F�N�g

	m_pPS = GetPS(PS_TOON);							// �s�N�Z���V�F�[�_�[�ݒ�
	m_pVS = GetVS(VS_OBJECT);						// ���_�V�F�[�_�[�ݒ�
}

// ===�f�X�g���N�^===
ModelObject::~ModelObject()
{
}

// ===�X�V����===
void ModelObject::Update()
{
	Object3DBase::Update();						// �I�u�W�F�N�g�X�V
}

// ===�`�揈��===
void ModelObject::Draw()
{
	// ---�ʏ�`��---
	SetCullMode(CULLMODE_CCW);			// �w�ʃJ�����O
	ID3D11ShaderResourceView* pTex = TEX->Load("data/texture/ramp.png");	// �~���e�N�X�`���擾
	SetTexturePS(pTex, 1);				// �~���e�N�X�`���Z�b�g
	Object3DBase::Draw();				// �`��

	// ---�֊s���`��---
	SetCullMode(CULLMODE_CW);			// �O�ʃJ�����O
	m_pPS = GetPS(PS_OUTLINE);			// �֊s���`��V�F�[�_�[�ɐؑ�
	m_pVS = GetVS(VS_OUTLINE);			// �֊s���`��V�F�[�_�[�ɐؑ�
	Object3DBase::Draw();				// �`��


	// ---�`��㏈��---
	m_pPS = GetPS(PS_TOON);				// �s�N�Z���V�F�[�_�[�Z�b�g
	m_pVS = GetVS(VS_OBJECT);			// ���_�V�F�[�_�\�Z�b�g
	SetCullMode(CULLMODE_CCW);			// �w�ʃJ�����O
}

// ===�X�V�L��===
bool ModelObject::IsUpdate()
{
	return true;
}

// ===�f�ރp�X�ݒ�===
void ModelObject::SetResource(std::string pass)
{
	m_Model = MODEL->Load(pass);
	if(m_Model)
		m_ResourcePass = pass;
}

// ===�G�f�B�^�\��===
void ModelObject::ImGuiWindow()
{
	// ===���f���I��===
	static int idx = 0;					// ���ݑI��ԍ�

	char combo_preview_value[50];							// �p�X�\���p
	strcpy(combo_preview_value,m_ResourcePass.c_str());		// �I�𒆂̃p�X

	if (ImGui::BeginCombo("Model", combo_preview_value, ImGuiComboFlags_HeightSmall))
	{
		for (int n = 0; n < MAX_MODEL; n++)
		{
			const bool is_selected = (idx == n);								// �I�����
			if (ImGui::Selectable(GetModelPass((MODEL_TYPE)n), is_selected))	// �I����Ԃɂ���Ĕ���
			{
				idx = n;														// �I��ԍ����X�V
				SetResource(GetModelPass((MODEL_TYPE)idx));						// �I���������f���̃p�X��ݒ�
			}


			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();				// �G�f�B�^��̑I���X�V
			}

		}
		ImGui::EndCombo();
	}
}