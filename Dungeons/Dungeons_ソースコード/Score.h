#pragma once
// ===�X�R�AUI�N���X��`===
// �X�R�AUI�N���X���`
// Author: Kimura
// ======================

// ===�C���N���[�h===
#include "main.h"
#include "UIBase.h"

// ===�N���X��`===
class Score : public UIBase
{
	// ---�����o�֐�---
public:
	Score();		// �R���X�g���N�^
	~Score();		// �f�X�g���N�^

	void Update() override;		// �X�V����
	void Draw() override;		// �`�揈��

	// ---�����o�ϐ�---
private:
};