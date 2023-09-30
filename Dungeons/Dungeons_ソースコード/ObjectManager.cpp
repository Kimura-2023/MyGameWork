// ===�I�u�W�F�N�g�Ǘ��N���X���� ================
// �I�u�W�F�N�g���ꌳ�Ǘ�����N���X������
// Author kimura
// ===============================================

// ===�C���N���[�h===
#include "ObjectManager.h"
#include "DebugProc.h"

// ===�ÓI�����o===
ObjectManager* ObjectManager::m_pInstance = nullptr;	// �C���X�^���X

// ===�R���X�g���N�^===
ObjectManager::ObjectManager()
{
	m_3dObject.resize(0);	// 3D�I�u�W�F�N�g���X�g������
	m_2dObject.resize(0);	// 2D�I�u�W�F�N�g���X�g������
	m_UiObject.resize(0);	// UI�I�u�W�F�N�g���X�g������
	m_nCount = 0;			// �I�u�W�F�N�g����
}

// ===�f�X�g���N�^===
ObjectManager::~ObjectManager()
{
	Release();				// �������Ă���I�u�W�F�N�g��S�Ĕj������
}

// ===����===
void ObjectManager::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new ObjectManager;	// �C���X�^���X�𐶐�
	}
	else
	{
		return;
	}
}

// ===�j��===
void ObjectManager::Destroy()
{
	delete m_pInstance;						// �C���X�^���X���폜
	m_pInstance = nullptr;					// null�i�[
}

// ===�擾===
ObjectManager* ObjectManager::Get()
{
	return m_pInstance;						// ObjectManager�̃C���X�^���X��Ԃ�
}

// ===�X�V===
void ObjectManager::Update()
{
	O3DIT it3dobj;				// 3D�I�u�W�F�N�g���X�g(�ȉ�3Dlist)�̃C�e���[�^�[�錾
	it3dobj = m_3dObject.begin();		// 3Dlist�C�e���[�^�[�����X�g�̐擪�Ɉړ�

	// ===�������[�v===
	while (it3dobj != m_3dObject.end())	// --3Dlist�̏I�[�܂�
	{
		// null�`�F�b�N
		if ((*it3dobj) == nullptr)
		{
			it3dobj = m_3dObject.erase(it3dobj);	// 2Dlist����v�f���폜
			m_nCount--;								// �I�u�W�F�N�g��������
			continue;
		}
		(*it3dobj)->Update();		// 3D�I�u�W�F�N�g�X�V
		// �I�u�W�F�N�g�j�������ꍇ�ɃC�e���[�^�[�̎w���ꏊ���Ȃ��Ȃ�̂ŕ␳
		if ((*it3dobj) == nullptr)
		{
			it3dobj = m_3dObject.erase(it3dobj);	// 2Dlist����v�f���폜
			m_nCount--;								// �I�u�W�F�N�g��������
			continue;
		}
		// ===�����I�u�W�F�N�g�̃��[���h�}�g���N�X�X�V===
		//(*it3dobj)->UpdateMatrix();		// �}�g���N�X�X�V
		++it3dobj;						// �C�e���[�^�[�����炷
	}

	O2DIT it2dobj;				// 2D�I�u�W�F�N�g���X�g(�ȉ�2Dlist)�̃C�e���[�^�[�錾
	it2dobj = m_2dObject.begin();		// 2Dlist�C�e���[�^�[�����X�g�̐擪�Ɉړ�

	// ===�������[�v===
	while (it2dobj != m_2dObject.end())
	{
		// null�`�F�b�N
		if ((*it2dobj) == nullptr)
		{
			it2dobj = m_2dObject.erase(it2dobj);	// 2Dlist����v�f���폜
			m_nCount--;								// �I�u�W�F�N�g��������
			continue;
		}
		(*it2dobj)->Update();		// 2D�I�u�W�F�N�g�X�V

		// �I�u�W�F�N�g�j�������ꍇ�ɃC�e���[�^�[�̎w���ꏊ���Ȃ��Ȃ�̂ŕ␳
		if ((*it2dobj) == nullptr)
		{
			it2dobj = m_2dObject.erase(it2dobj);	// 2Dlist����v�f���폜
			m_nCount--;								// �I�u�W�F�N�g��������
			continue;
		}
		// ===�����I�u�W�F�N�g�̃��[���h�}�g���N�X�X�V===
		(*it2dobj)->UpdateMatrix();		// �}�g���N�X�X�V
		++it2dobj;						// �C�e���[�^�[�����炷
	}

	auto ituiobj = m_UiObject.begin();
	while (ituiobj != m_UiObject.end())
	{
		// null�`�F�b�N
		if ((*ituiobj) == nullptr)
		{
			ituiobj = m_UiObject.erase(ituiobj);	// ���X�g����v�f���폜
			m_nCount--;
			continue;
		}
		(*ituiobj)->Update();
		// �I�u�W�F�N�g�j�������ꍇ�ɃC�e���[�^�[�̎w���ꏊ���Ȃ��Ȃ�̂ŕ␳
		if ((*ituiobj) == nullptr)
		{
			ituiobj = m_UiObject.erase(ituiobj);	// 2Dlist����v�f���폜
			m_nCount--;								// �I�u�W�F�N�g��������
			continue;
		}

		++ituiobj;
	}
}

// ---�`��---
void ObjectManager::Draw()
{
	O3DIT it3dobj;				// 3D�I�u�W�F�N�g���X�g(�ȉ�3Dlist)�̃C�e���[�^�[�錾
	it3dobj = m_3dObject.begin();		// 3Dlist�C�e���[�^�[�����X�g�̐擪�Ɉړ�

	// ===�������[�v===
	while (it3dobj != m_3dObject.end())	// --3Dlist�̏I�[�܂�
	{
		// null�`�F�b�N
		if ((*it3dobj) == nullptr)
		{
			it3dobj = m_3dObject.erase(it3dobj);	// 2Dlist����v�f���폜
			m_nCount--;								// �I�u�W�F�N�g��������
			continue;
		}
		(*it3dobj)->Draw();		// 3D�I�u�W�F�N�g�X�V
		// �I�u�W�F�N�g�j�������ꍇ�ɃC�e���[�^�[�̎w���ꏊ���Ȃ��Ȃ�̂ŕ␳
		if ((*it3dobj) == nullptr)
		{
			it3dobj = m_3dObject.erase(it3dobj);	// 2Dlist����v�f���폜
			m_nCount--;								// �I�u�W�F�N�g��������
			continue;
		}
		++it3dobj;						// �C�e���[�^�[�����炷
	}

	O2DIT it2dobj;				// 2D�I�u�W�F�N�g���X�g(�ȉ�2Dlist)�̃C�e���[�^�[�錾
	it2dobj = m_2dObject.begin();		// 2Dlist�C�e���[�^�[�����X�g�̐擪�Ɉړ�
	SetZBuffer(false);
	// ===�������[�v===
	while (it2dobj != m_2dObject.end())
	{
		// null�`�F�b�N
		if ((*it2dobj) == nullptr)
		{
			it2dobj = m_2dObject.erase(it2dobj);	// 2Dlist����v�f���폜
			m_nCount--;								// �I�u�W�F�N�g��������
			continue;
		}
		(*it2dobj)->Draw();		// 2D�I�u�W�F�N�g�X�V

		// �I�u�W�F�N�g�j�������ꍇ�ɃC�e���[�^�[�̎w���ꏊ���Ȃ��Ȃ�̂ŕ␳
		if ((*it2dobj) == nullptr)
		{
			it2dobj = m_2dObject.erase(it2dobj);	// 2Dlist����v�f���폜
			m_nCount--;								// �I�u�W�F�N�g��������
			continue;
		}
		++it2dobj;						// �C�e���[�^�[�����炷
	}

	auto ituiobj = m_UiObject.begin();
	while (ituiobj != m_UiObject.end())
	{
		// null�`�F�b�N
		if ((*ituiobj) == nullptr)
		{
			ituiobj = m_UiObject.erase(ituiobj);	// ���X�g����v�f���폜
			m_nCount--;
			continue;
		}
		(*ituiobj)->Draw();
		++ituiobj;
	}
	SetZBuffer(true);
}

// ===3D�I�u�W�F�N�g��ǉ�===
void ObjectManager::AddObjectManager(Object3DBase* obj3d)
{
	m_3dObject.push_back(obj3d);		// 3D�I�u�W�F�N�g�̃��X�g�ɒǉ�
	++m_nCount;							// �I�u�W�F�N�g�������Z
}

// ===2D�I�u�W�F�N�g��ǉ�===
void ObjectManager::AddObjectManager(Object2DBase* obj2d)
{
	m_2dObject.push_back(obj2d);		// 2D�I�u�W�F�N�g�̃��X�g�ɒǉ�
	++m_nCount;							// �I�u�W�F�N�g�������Z
}

void ObjectManager::AddObjectManager(UIBase* objui)
{
	m_UiObject.push_back(objui);		// UI�I�u�W�F�N�g���X�g�ɒǉ�
	++m_nCount;							// �I�u�W�F�N�g�������Z
}

// ===�S�I�u�W�F�N�g���===
void ObjectManager::Release()
{
	// ---3D�I�u�W�F�N�g���
	O3DIT it3dobj;				// 3D�I�u�W�F�N�g���X�g(�ȉ�3Dlist)�̃C�e���[�^�[�錾
	it3dobj = m_3dObject.begin();				// 3Dlist�C�e���[�^�[�����X�g�̐擪�Ɉړ�

		// ===�������[�v===
	while (it3dobj != m_3dObject.end())	// --3Dlist�̏I�[�܂�
	{
		delete *it3dobj;						// �I�u�W�F�N�g�폜
		++it3dobj;
	}

	// ---2D�I�u�W�F�N�g���
	O2DIT it2dobj;				// 2D�I�u�W�F�N�g���X�g(�ȉ�2Dlist)�̃C�e���[�^�[�錾
	it2dobj = m_2dObject.begin();				// 2Dlist�C�e���[�^�[�����X�g�̐擪�Ɉړ�

		// ===�������[�v===
	while (it2dobj != m_2dObject.end())	// --2Dlist�̏I�[�܂�
	{
		delete *it2dobj;						// �I�u�W�F�N�g�폜
		++it2dobj;
	}

	// ---UI�I�u�W�F�N�g���
	auto ituiobj = m_UiObject.begin();

	while (ituiobj != m_UiObject.end())
	{
		delete *ituiobj;
		++ituiobj;
	}
}

// ===�w�肵��3D�I�u�W�F�N�g�����===
void ObjectManager::Release(Object3DBase* obj3d)
{
	if (m_3dObject.size() <= 0)
		return;
	O3DIT it3dobj;							// 3D�I�u�W�F�N�g���X�g(�ȉ�3Dlist)�̃C�e���[�^�[�錾
	it3dobj = m_3dObject.begin();			// 3Dlist�C�e���[�^�[�����X�g�̐擪�Ɉړ�

	// ===�������[�v===
	while(*it3dobj != obj3d)	// --3Dlist�̃I�u�W�F�N�g�Ǝw��̃I�u�W�F�N�g����v����܂Ŏ��s
	{
		++it3dobj;							// �C�e���[�^�[�����炷	
	}

	it3dobj = m_3dObject.erase(it3dobj);	// 3Dlist����v�f���폜
	m_nCount--;							// �I�u�W�F�N�g��������
}

// ===�w�肵��2D�I�u�W�F�N�g�����===
void ObjectManager::Release(Object2DBase* obj2d)
{
	if (m_2dObject.size() <= 0)
		return;
	O2DIT it2dobj;							// 2D�I�u�W�F�N�g���X�g(�ȉ�2Dlist)�̃C�e���[�^�[�錾
	it2dobj = m_2dObject.begin();			// 2Dlist�C�e���[�^�[�����X�g�̐擪�Ɉړ�

	// ===�������[�v===
	while (*it2dobj != obj2d)	// --2Dlist�̃I�u�W�F�N�g�Ǝw��̃I�u�W�F�N�g����v����܂Ŏ��s
	{
		++it2dobj;							// �C�e���[�^�[�����炷	
	}

	it2dobj = m_2dObject.erase(it2dobj);	// 2Dlist����v�f���폜
	m_nCount--;								// �I�u�W�F�N�g��������
}

// ===�w�肵��UI�I�u�W�F�N�g���===
void ObjectManager::Release(UIBase* objui)
{
	// ---��O����---
	if (m_UiObject.size() <= 0)
		return;

	// ---�������---
	auto ituiobj = m_UiObject.begin();		// UI�I�u�W�F�N�g���X�g�̐擪�C�e���[�^�擾
	while (*ituiobj != objui)	// --2Dlist�̃I�u�W�F�N�g�Ǝw��̃I�u�W�F�N�g����v����܂Ŏ��s
	{
		++ituiobj;							// �C�e���[�^�[�����炷	
	}
	ituiobj = m_UiObject.erase(ituiobj);	// 2Dlist����v�f���폜
	m_nCount--;								// �I�u�W�F�N�g��������
}