#pragma once

// ===�N���A�V�[���N���X��`===
// �N���A�V�[���N���X���`
// Author kimura
// =================================

// ===�C���N���[�h===
#include "main.h"
#include "SceneBase.h"

// ===�N���X��`===
class GameClearScene : public SceneBase
{
	// ---�����o�֐�---
public:
	GameClearScene();		// �R���X�g���N�^
	~GameClearScene();		// �f�X�g���N�^
	void Update();			// �X�V����
	void Draw();			// �`�揈��
};