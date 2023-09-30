#pragma once
// ===�w�i���f����`===
// �w�i���f���N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "main.h"
#include "Object3DBase.h"
#include "ModelManager.h"

// ===�N���X��`===
class ModelObject : public Object3DBase
{
	// ---�����o�֐�---
public:
	ModelObject();			// �R���X�g���N�^
	~ModelObject();			// �f�X�g���N�^

	void Update();			// �X�V����
	void Draw();			// �`�揈��

	bool IsUpdate();		// �X�V�L��

	void SetResource(std::string);		// �f�ނ̃p�X�ݒ�

	void ImGuiWindow();		// �G�f�B�^�\��

	// ---�����o�ϐ�---
private:
};