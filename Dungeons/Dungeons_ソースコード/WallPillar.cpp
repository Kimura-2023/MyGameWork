// ===�ǒ��N���X����===
// �ǒ��I�u�W�F�N�g�N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "WallPillar.h"
#include "ModelManager.h"
#include "ShaderList.h"
#include "CollisionManager.h"
#include "TextureManager.h"
#include "ImGuiBase.h"

// ===�R���X�g���N�^===
WallPillar::WallPillar() : Collision3D(this)
{
	m_transform.Pos = { 0,0,0 };										// �������W
	m_transform.Rot = { 0,0,0 };										// ������]
	m_transform.Scale = { 1,1,1 };										// �����g�k
	m_id = OT_WALLPILLAR;												// �I�u�W�F�N�g�̌^�ݒ�
	m_bEnable = true;													// �X�V�E�`���
	m_col = true;														// �����蔻��L��
	m_bVisible = true;													// ���F��
	m_Model = MODEL->Load("data/model/WallPillar.fbx");					// ���f���Ǎ�
	AddCollisionOBB(XMFLOAT3(0, 2.5f, 0), XMFLOAT3(0.7f, 2.5f, 0.7f));	// �����蔻��ǉ�
	m_bStatic = true;													// �ÓI�I�u�W�F�N�g

	m_pPS = GetPS(PS_MASKOBJECT);										// �s�N�Z���V�F�[�_�[�ݒ�
	m_pVS = GetVS(VS_OBJECT);											// ���_�V�F�[�_�[�ݒ�

	m_ResourcePass = "data/texture/BlockTex004.png";					// �e�N�X�`���p�X
	m_pTex = TEX->Load("data/texture/BlockTex004.png");					// �e�N�X�`���Ǎ�
}

// ===�f�X�g���N�^===
WallPillar::~WallPillar()
{

}

// ===�X�V����===
void WallPillar::Update()
{
	Object3DBase::Update();			// �I�u�W�F�N�g�X�V
}

void WallPillar::Draw()
{
	// ---�ʏ�`��---
	SetCullMode(CULLMODE_CCW);								// �w�ʃJ�����O
	SetTexturePS(m_pTex, 0);								// �e�N�X�`���Z�b�g
	SetTexturePS(TEX->Load("data/texture/ramp.png"), 1);	// �~���e�N�X�`���Z�b�g
	Object3DBase::Draw();									// �`�揈��

	// ---�֊s���`��---
	SetCullMode(CULLMODE_CW);			// �O�ʃJ�����O
	m_pPS = GetPS(PS_BEHIND);			// �s�N�Z���V�F�[�_�[�Z�b�g
	m_pVS = GetVS(VS_OUTLINE);			// ���_�V�F�[�_�[�Z�b�g
	Object3DBase::Draw();				// �`�揈��

	// --- �`��ݒ�����ɖ߂�---
	m_pPS = GetPS(PS_MASKOBJECT);
	m_pVS = GetVS(VS_OBJECT);
	SetCullMode(CULLMODE_CCW);
}

// ===�X�V�L��===
bool WallPillar::IsUpdate()
{
	return true;
}

// ===�Փˏ���===
void WallPillar::OnCollision(Object* obj)
{
}

// ===�f�ނ̃p�X�ݒ�===
void WallPillar::SetResource(std::string pass)
{
	m_pTex = TEX->Load(pass);		// �e�N�X�`���Ǎ�
	if (m_pTex)
		m_ResourcePass = pass;		// �p�X��ݒ�
}

// ===�G�f�B�^�\��===
void WallPillar::ImGuiWindow()
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