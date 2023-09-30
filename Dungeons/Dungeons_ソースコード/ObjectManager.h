#pragma once

// ===�I�u�W�F�N�g�Ǘ��N���X��`===
// �I�u�W�F�N�g�Ǘ��N���X���`
// Author kimura
// ===================================

// ===�C���N���[�h===
#include "main.h"
#include "Object3DBase.h"
#include "Object2DBase.h"
#include <list>
#include "UIBase.h"

// ===�萔�}�N����`===
#define OBJECT (ObjectManager::Get())

using O3DIT = std::list<Object3DBase*>::iterator;		// 2D�I�u�W�F�N�g�̃C�e���[�^�[�錾
using O2DIT = std::list<Object2DBase*>::iterator;		// 3D�I�u�W�F�N�g�̃C�e���[�^�[�錾

// ===�N���X��`===
class ObjectManager
{
	// ---�����o�֐�---
private:
	ObjectManager();		// �R���X�g���N�^
	~ObjectManager();		// �f�X�g���N�^
public:
	static void Create();			// ����
	static void Destroy();			// �j��
	static ObjectManager* Get();	// �擾

	void AddObjectManager(Object3DBase*);	// 3D�I�u�W�F�N�g�ǉ�
	void AddObjectManager(Object2DBase*);	// 2D�I�u�W�F�N�g�ǉ�
	void AddObjectManager(UIBase*);			// UI�I�u�W�F�N�g�ǉ�

	void Update();					// �X�V
	void Draw();					// �`��
	void Release();					// �I�u�W�F�N�g���
	void Release(Object3DBase*);	// �w���3D�I�u�W�F�N�g���
	void Release(Object2DBase*);	// �w���2D�I�u�W�F�N�g���
	void Release(UIBase*);			// �w���UI�I�u�W�F�N�g���

	// ---�����o�ϐ�---
private:
	static ObjectManager* m_pInstance;		// ���g�̃C���X�^���X
	std::list<Object3DBase*> m_3dObject;	// 3D�I�u�W�F�N�g���X�g
	std::list<Object2DBase*> m_2dObject;	// 2D�I�u�W�F�N�g���X�g
	std::list<UIBase*> m_UiObject;			// UI�I�u�W�F�N�g���X�g

	int m_nCount;							// ���I�u�W�F�N�g��
};
