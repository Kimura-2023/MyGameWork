#pragma once
// ===�Q�[���V�[���N���X��`===
// �Q�[���V�[���N���X���`
// Author kimura
// =================================

// ===�C���N���[�h===
#include "main.h"
#include "SceneBase.h"

// ===�N���X��`===
class GameScene : public SceneBase
{
	// ---�����o�֐�---
public:
	GameScene();		// �R���X�g���N�^
	~GameScene();		// �f�X�g���N�^
	void Update();		// �X�V����
	void Draw();		// �`�揈��
};