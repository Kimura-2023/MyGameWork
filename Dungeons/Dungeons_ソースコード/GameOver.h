#pragma once
// ===�Q�[���I�[�o�[UI�N���X��`===
// �Q�[���I�[�o�[UI�N���X���`
// Author: Kimura
// ===============================

// ===�C���N���[�h===
#include "Object2DBase.h"

// ===�R���X�g���N�^===
class GameOver : public Object2DBase
{
public:
	GameOver();			// �R���X�g���N�^
	~GameOver();		// �f�X�g���N�^

	void Update();		// �X�V����
	void Draw();		// �`�揈��
	bool IsUpdate();	// �X�V�L��
private:
};
