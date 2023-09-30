#pragma once
// ===�Q�[���i�s�Ǘ��N���X��`===
// �Q�[���Ǘ��N���X���`
// Author Kimura
// ==============================

// ===�C���N���[�h===
#include "main.h"
#include "GameCamera.h"
#include "Input.h"
#include "SceneManager.h"

// ===�萔�E�}�N����`===
#define GAME (GameManager::Get())					// �C���X�^���X�擾

// ===�N���X��`===
class GameManager
{
	// ---�����o�֐�---
public:
	GameManager();									// �R���X�g���N�^
	~GameManager();									// �f�X�g���N�^

	static GameManager* Get();						// �C���X�^���X�擾
	static void Destroy();							// �C���X�^���X�j��
	static void Create();							// �C���X�^���X����

	void Update();									// �X�V����
	void Draw();									// �`�揈��

	XMFLOAT3 MousePos();							// �}�E�X���W(XZ���ʏ�)�擾
	POINT GetMousePoint() { return m_Point; }		// �}�E�X�X�N���[�����W�擾

	// ---�����o�ϐ�---
private:
	static GameManager* m_pInstance;				// �C���X�^���X

	POINT m_Point;									// �X�N���[���}�E�X���W
	XMFLOAT3 m_MouseWorldPos;						// XZ���ʏ�}�E�X���W
};
