#pragma once
// ===�^�C�g�����SUI�N���X��`===
// �^�C�g�����SUI�N���X���`
// Author: Kimura
// ===============================

// ===�C���N���[�h===
#include "Object2DBase.h"

// ===�N���X��`===
class Title : public Object2DBase
{
public:
	Title();			// �R���X�g���N�^
	~Title();			// �f�X�g���N�^

	void Update();		// �X�V����
	void Draw();		// �`�揈��
	bool IsUpdate();	// �X�V�L��
private:
}; 
