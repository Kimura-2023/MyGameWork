#pragma once

// ===�����蔻��Ǘ��N���X��`===
// �����蔻��Ǘ��N���X���`
// Author Kimura
// ==============================

// ===�C���N���[�h===
#include "main.h"
#include "Collision3D.h"
#include <list>

// ===�萔�E�}�N����`===
#define COLLISION (CollisionManager::Get())			// �����蔻��}�l�[�W���̃C���X�^���X�擾
using COLIT = std::list<Collision3D*>::iterator;	// �����蔻�胊�X�g�̃C�e���[�^�[

// ===�N���X��`===
class CollisionManager
{
	// ---�����o�֐�---
private:
	CollisionManager();								// �R���X�g���N�^
	~CollisionManager();							// �f�X�g���N�^
public:
	static CollisionManager* Get();					// �擾
	static void Destroy();							// �j��
	static void Create();							// ����

	void Update();									// �X�V����
	void Draw();									// �`�揈��

	void AddManager(Collision3D*);					// �Ǘ����ɒǉ�
	void Release();									// �S�����蔻����
	void Release(Collision3D*);						// �����蔻����
	void SwitchVisibleCollision(Object*);			// ����Ԃ�ؑ�

	// ---�����o�ϐ�---
private:
	static CollisionManager* m_pInstance;			// �C���X�^���X
	std::list<Collision3D*> m_CollisionList;		// �����蔻��S���X�g
};
