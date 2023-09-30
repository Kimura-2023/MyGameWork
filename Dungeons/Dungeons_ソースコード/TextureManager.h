#pragma once

// ===�e�N�X�`���Ǘ��N���X��`===
// �e�N�X�`���Ǘ��N���X���`
// Author kimura
// ======================

// ===�C���N���[�h===
#include "main.h"
#include "Object2DBase.h"
#include <vector>

// ===�萔�E�}�N����`===
#define TEX (TextureManager::Get())			// �C���X�^���X�擾

// ===�e�N�X�`����ޒ�`===
enum TEX_TYPE
{
	WALL_TEX1 = 0,		// �ǃe�N�X�`��1
	WALL_TEX2,			// �ǃe�N�X�`��2
	WALL_TEX3,			// �ǃe�N�X�`��3
	EFF_TEX1,			// �~�p�[�e�B�N��
	TITLE_TEX1,			// �^�C�g��1
	TITLE_TEX2,			// �^�C�g��2
	TITLE_TEX3,			// �^�C�g��3
	GAMEOVER_TEX1,		// �Q�[���I�[�o�[1
	GAMECLEAR_TEX1,		// �Q�[���I�[�o�[2
	RAMP_TEX,			// �g�D�[���V�F�[�_�[�p
	STAGE_TEX1,			// �X�e�[�W
	WALL_TEX4,			// �ǃe�N�X�`��4

	MAX_TEX
};

// ===�N���X��`===
class TextureManager
{
	// ---�����o�֐�
public:
	TextureManager();
	~TextureManager();

	static TextureManager* Get();			// �C���X�^���X�擾
	static void Destroy();					// �C���X�^���X�j��
	static void Create();					// �C���X�^���X����

	void Release();							// �e�N�X�`�����

	ID3D11ShaderResourceView* Load(std::string);		// �Ǎ�
	void Draw();										// �`�揈��

	// ---�����o�ϐ�
private:
	static TextureManager* m_pInstance;						// �C���X�^���X
	std::map<std::string, Texture*> m_TextureResource;		// ���\�[�X���X�g
};

// ===�֐���`===
LPCSTR GetTexturePass(TEX_TYPE);	// imgui��Ŏg�p����e�N�X�`���f�[�^�̃t�@�C���p�X