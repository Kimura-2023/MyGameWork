#pragma once
// ===�G���^�[�L�[UI�N���X��`===
// UI�N���X���`
// Author: Kimura
// ===============================

// ===�C���N���[�h===
#include "Object2DBase.h"

// ===�N���X��`===
class EnterButton : public Object2DBase
{
	// ---�����o�֐�---
public:
	EnterButton();		// �R���X�g���N�^
	~EnterButton();		// �f�X�g���N�^

	void Update();		// �X�V����
	void Draw();		// �`�揈��
	bool IsUpdate();	// �X�V�L��

	// ---�����o�ϐ�---
private:
};
