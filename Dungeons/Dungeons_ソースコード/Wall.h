#pragma once
// ===�ǃN���X��`===
// �ǃN���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "main.h"
#include "Object3DBase.h"
#include "SceneManager.h"
#include "Collision3D.h"

// ===�N���X��`===
class Wall : public Object3DBase, Collision3D
{
	// ---�����o�֐�---
public:
	Wall();				// �R���X�g���N�^
	~Wall();			// �f�X�g���N�^

	void Update();		// �X�V����
	void Draw();		// �`�揈��

	bool IsUpdate();	// �X�V�L��

	void OnCollision(Object*);	// �Փˏ���

	void SetResource(std::string);		// �f�ރp�X�ݒ�
	void ImGuiWindow();					// �G�f�B�^�\��

	// ---�����o�ϐ�---
private:
	ID3D11ShaderResourceView* m_pTex;	// �e�N�X�`��
};