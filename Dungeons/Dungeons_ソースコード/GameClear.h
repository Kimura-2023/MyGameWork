#pragma once
// ===�Q�[���N���AUI�N���X��`===
// �Q�[���N���AUI�N���X���`
// Author: Kimura
// ===============================

// ===�C���N���[�h===
#include "Object2DBase.h"

// ===�N���X��`===
class GameClear : public Object2DBase
{
	// ---�����o�֐�---
public:
	GameClear();		// �R���X�g���N�^
	~GameClear();		// �f�X�g���N�^

	void Update();		// �X�V����
	void Draw();		// �`�揈��
	bool IsUpdate();	// �X�V�L��
private:
};
