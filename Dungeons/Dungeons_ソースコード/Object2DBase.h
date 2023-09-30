// ===�z�u2D�I�u�W�F�N�g�x�[�X�N���X Object2DBase.h
// ---Authot kimura
#pragma once

// ===�C���N���[�h===
#include "main.h"
#include "Object.h"

// ===�N���X��`===
class Object2DBase : public Object
{
	// ---�����o�֐�---
public:
	Object2DBase();				// �R���X�g���N�^
	virtual ~Object2DBase();	// �f�X�g���N�^

	void Init();				// ������
	virtual void Update();		// �X�V
	virtual void Draw();		// �`��

	void SetResource(std::string);		// �f�ރp�X�ݒ�

	void SetUseBlendState(EBlendState);	// �u�����h�X�e�[�g���Z�b�g
	void SetColor(XMFLOAT4 c) { m_color = c; }		// �F�Z�b�g

	XMFLOAT3 GetRotate() { return m_transform.Rot; }		// ��]�擾
	XMFLOAT4 GetColor() { return m_color; }					// �F�擾
	XMFLOAT2 GetScale() { return m_Scale; }					// �g�k�擾
	XMFLOAT2 GetUV() { return m_uv; }						// ����UV�擾
	XMFLOAT2 GetTexSize();									// �E��UV�擾

	float GetCamVecSize();							// �J�����Ƃ̋���

	bool GetUI() { return m_bui; }					// UI���ǂ���
	bool GetBill() { return m_bBillboard; }			// �r���{�[�h�L��
	EBlendState GetBlendState() { return m_Blend; }	// �u�����h�X�e�[�g�擾

	void UpdateMatrix();		// ���[���h�}�g���N�X�X�V

	virtual void ImGuiWindow() { ; }

	// ---�����o�ϐ�---
protected:
	XMFLOAT2 m_Scale;		// �g�k
	XMFLOAT2 m_uv;			// ����uv���W
	XMFLOAT2 m_TexCut;		// �e�N�X�`��������
	XMFLOAT4 m_color;		// �F

	bool m_bBillboard;		// �r���{�[�h�L��
	bool m_bui;

	EBlendState m_Blend;	// �u�����h�X�e�[�g
	ID3D11ShaderResourceView* m_Texture;		// �e�N�X�`��
private:
	float CalcObjSize(XMFLOAT2*);
};