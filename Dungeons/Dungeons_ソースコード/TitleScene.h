#pragma once
// ===�^�C�g���V�[���N���X��`===
// �^�C�g���V�[���N���X���`
// Author kimura
// =================================

// ===�C���N���[�h===
#include "main.h"
#include "SceneBase.h"

// ===�N���X��`===
class TitleScene : public SceneBase
{
public:
	TitleScene();		// �R���X�g���N�^
	~TitleScene();		// �f�X�g���N�^
	void Update();		// �X�V����
	void Draw();		// �`�揈��
};