#pragma once
// ===�V�[���Ǘ��N���X��`===
// �V�[���Ǘ��N���X���`
// Author kimura
// ======================

// ===�C���N���[�h===
#include "main.h"
#include "SceneBase.h"
#include <memory>

// ===�V�[����ޒ�`===
enum Scene
{
	SCENE_NONE,
	SCENE_TITLE,
	SCENE_GAME,
	SCENE_DEBUG,
	SCENE_GAMEOVER,
	SCENE_GAMECLEAR,
	SCENE_LOAD,
	SCENE_END,
};

// ===�萔�E�}�N����`===
#define SCENE (SceneManager::Get())			// SceneManager�̃C���X�^���X�擾

// ===�N���X��`===
class SceneManager
{
	// ---�����o�֐�---
public:
	static void Create();					// ����
	static void Destroy();					// �j��
	static SceneManager* Get();				// �擾

	void Update();							// �X�V
	void Draw();							// �`��

	void SetScene(Scene);										// �V�[�����Z�b�g
	SceneBase* GetCurrentScene() { return m_pScene.get(); }		// ���݃V�[�����擾
	void LoadScene(Scene);										// �V�[���Ǎ�

private:
	void ReleaseScene();						// �V�[�����
	SceneManager();								// �R���X�g���N�^
	~SceneManager();							// �f�X�g���N�^

	// ---�����o�ϐ�
private:
	static SceneManager* m_pInstance;			// �}�l�[�W���̃C���X�^���X

	std::unique_ptr<SceneBase> m_pScene;		// ���݃V�[���̃|�C���^
	Scene m_eCurrentScene;						// ���݃V�[���^
	Scene m_eChangeScene;						// �J�ڐ�V�[���^
};