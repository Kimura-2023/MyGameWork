#pragma once

// ===�Q�[���I�[�o�[�V�[���N���X��`===
// �Q�[���I�[�o�[�V�[���N���X���`
// Author kimura
// =================================

// ===�C���N���[�h===
#include "main.h"
#include "SceneBase.h"

// ===�N���X��`===
class GameOverScene : public SceneBase
{
	// ---�����o�֐�---
public:
	GameOverScene();		// �R���X�g���N�^
	~GameOverScene();		// �f�X�g���N�^
	void Update();			// �X�V����
	void Draw();			// �`�揈��
};