#pragma once
// ===UI�x�[�X�N���X��`===
// UI�x�[�X�N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "Object.h"
#include "main.h"

// ===�N���X��`===
class UIBase : public Object
{
	// ---�����o�֐�---
public:
	UIBase();				// �R���X�g���N�^
	virtual ~UIBase();		// �f�X�g���N�^

	void virtual Update();	// �X�V����
	void virtual Draw();	// �`�揈��

	void SetResource(std::string);		// �f�ރp�X�ݒ�

	bool IsUpdate();		// �X�V����
	void UpdateMatrix();	//

	// ---�����o�ϐ�---
protected:
	EBlendState m_Blend;	// �u�����h�X�e�[�g
	ID3D11ShaderResourceView* m_Texture;		// �e�N�X�`��

	XMFLOAT2 m_uv;			// ����uv���W
	XMFLOAT2 m_TexCut;		// �e�N�X�`��������
	XMFLOAT4 m_color;		// �F
};