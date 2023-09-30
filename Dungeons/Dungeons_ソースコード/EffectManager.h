#pragma once

// ===�G�t�F�N�g�Ǘ��N���X��`===
// �G�t�F�N�g�Ǘ��N���X���`
// Author Kimura
// ==============================

// ===�C���N���[�h===
#include "main.h"
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>

// ---���C�u�����̃����N
#ifdef _DEBUG
#pragma comment(lib, "Effekseer_2017_d.lib")
#pragma comment(lib, "EffekseerRendererDX11_2017_d.lib")
#else
#pragma comment(lib, "Effekseer_2017.lib")
#pragma comment(lib, "EffekseerRendererDX11_2017.lib")
#endif

// ===�萔�E�}�N����`===
#define EFFECT (EffectManager::Get())				// �C���X�^���X�擾
#define MAX_PARTICLE (1000)							// �p�[�e�B�N���ő吔

// ===�G�t�F�N�g��ޒ�`===
enum EFFECT_TYPE
{
	HIT_EFFECT = 0,
	MOVE_EFFECT,
	SWORD_EFFECT,
	KICK_EFFECT,
	EXPLOSION_EFFECT,
	ENEMY_EFFECT,
	ENEMY_DEAD_EFFECT,
	REPEL_EFFECT,
	AVOID_EFFECT,

	EFFECT_MAX,
};

// ===�N���X��`===
class EffectManager
{
	// ---�����o�֐�---
public:
	EffectManager();					// �R���X�g���N�^
	~EffectManager();					// �f�X�g���N�^

	static EffectManager* Get();		// �C���X�^���X�擾
	static void Destroy();				// �C���X�^���X�j��
	static void Create();				// �C���X�^���X����

	void Init();						// ����������
	void LoadAll();						// �G�t�F�N�g�ꊇ�Ǎ�

	void Update();						// �X�V����
	void Release();						// �G�t�F�N�g�S���
	void Draw();						// �`�揈��

	void Play(EFFECT_TYPE,const XMFLOAT3& pos);			// �G�t�F�N�g�Đ�

	// ---�����o�ϐ�---
private:
	static EffectManager* m_pInstance;						// �C���X�^���X

	EffekseerRendererDX11::RendererRef m_renderer;			// �`��p
	Effekseer::ManagerRef m_manager;						// �Đ��Ǘ�
	Effekseer::Handle m_handle;								// 
	std::vector<Effekseer::EffectRef> m_EffectResource;		// �G�t�F�N�g���\�[�X
};