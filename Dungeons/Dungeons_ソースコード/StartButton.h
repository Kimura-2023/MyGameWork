#pragma once
// ===�X�^�[�g�{�^���N���X��`===
// �Q�[���J�n�{�^���N���X���`
// Author: Kimura

// ===�C���N���[�h===
#include "UIButton.h"
#include "main.h"

// ===�N���X��`===
class StartButton : public UIButton
{
	// ---�����o�֐�---
public:
	StartButton();		// �R���X�g���N�^
	~StartButton();		// �f�X�g���N�^

	void OnPushed() override;	// �{�^������

	// ---�����o�ϐ�---
private:
};