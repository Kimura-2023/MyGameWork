#pragma once
// ===�̗�UI�N���X��`===
// �̗�UI�N���X���`
// Author: Kimura
// ======================

// ===�C���N���[�h===
#include "main.h"
#include "UIBase.h"

// ===�N���X��`===
class PlayerLife : public UIBase
{
	// ---�����o�֐�---
public:
	PlayerLife();		// �R���X�g���N�^
	~PlayerLife();		// �f�X�g���N�^

	void Update() override;		// �X�V����
	void Draw() override;		// �`�揈��

	// ---�����o�ϐ�---
private:
	int m_nLife;				// �̗�
};