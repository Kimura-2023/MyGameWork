// ===�����蔻��Ǘ��N���X����===
// �����蔻��Ǘ��N���X������
// Author Kimura
// ==============================

// ===�C���N���[�h===
#include "CollisionManager.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "DebugProc.h"


// ===�ÓI�����o===
CollisionManager* CollisionManager::m_pInstance = nullptr;

// ===�R���X�g���N�^===
CollisionManager::CollisionManager()
{
}

// ===�f�X�g���N�^===
CollisionManager::~CollisionManager()
{
	Release();									// �S�����蔻����
}

// ===����===
void CollisionManager::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new CollisionManager;		// �C���X�^���X�𐶐�
	}
}

// ===�j��===
void CollisionManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;					// �C���X�^���X��j��
		m_pInstance = nullptr;				// null���i�[
	}
}

// ===�擾===
CollisionManager* CollisionManager::Get()
{
	return m_pInstance;						// �C���X�^���X��Ԃ�
}

// ===�Ǘ����ɒǉ�===
void CollisionManager::AddManager(Collision3D* col)
{
	m_CollisionList.push_back(col);			// �����蔻�胊�X�g�ɒǉ�
}

// ===���===
void CollisionManager::Release()
{
	COLIT colit = m_CollisionList.begin();			// �����蔻�胊�X�g�̐擪�C�e���[�^�擾
	while (colit != m_CollisionList.end())			// �I�[�܂ŌJ��Ԃ�
	{
		colit = m_CollisionList.erase(colit);		// ���X�g�������
	}
}

// ===�w�肵�������蔻��̉��===
void CollisionManager::Release(Collision3D* col)
{
	COLIT colit = m_CollisionList.begin();			// ���X�g�̐擪�C�e���[�^�擾
	while ((*colit) != col)							// �Ώۂ̓����蔻��܂ŌJ��Ԃ�
	{
		++colit;									// �C�e���[�^�����炷
	}
	colit = m_CollisionList.erase(colit);			// ���X�g����Ώۂ̓����蔻�������
}

// ===�X�V����===
void CollisionManager::Update()
{
	// ---��O����---
	if (m_CollisionList.size() <= 0)
		return;

	// ---�X�V����---
	COLIT colit = m_CollisionList.begin();	// �����蔻�胊�X�g�̐擪�ɃC�e���[�^�[���擾
	std::list<Collision3D*> ColVec(0);		// �����蔻����s�����X�g

	while (colit != m_CollisionList.end()) // �I�[�܂ŌJ��Ԃ�
	{
		// ---�����蔻��v�Z�̏��O---
		if (!(*colit)->GetFollowObject()->GetEnable())				// �I�u�W�F�N�g�̓����蔻��enable�t���O��false�̏ꍇ�͏������Ȃ�
		{
			(*colit)->SetCollision(false);			// �����蔻����Ƃ�Ȃ�
			++colit;								// ���̓����蔻���T��
			continue;
		}

		// ---�����蔻����W�̍X�V---
		(*colit)->Update();
		if ((*colit)->GetEnable() && (*colit)->GetFollowObject()->GetVisible() && !(*colit)->GetFollowObject()->GetStatic())		// �����蔻����Ƃ�I�u�W�F�N�g�̓��X�g�ɒǉ�
																																	// (�X�V�E�`����s���Ă���I�u�W�F�N�g���A�ÓI�I�u�W�F�N�g����Ȃ�����)
			ColVec.push_back((*colit));				// ���X�g�ɒǉ�
		++colit;									// �C�e���[�^�����炷
	}

	// ---�Փ˔���---
	colit = ColVec.begin();					// �����蔻�胊�X�g�̐擪�ɃC�e���[�^�[���擾
	while (colit != ColVec.end())			// �I�[�܂ŌJ��Ԃ�
	{
		// ---��O����---
		if (m_CollisionList.size() <= 1)
			return;

		// ---����---
		COLIT other = m_CollisionList.begin();
		while (other != m_CollisionList.end())
		{
			// ---���O�ݒ�---
			if ((*other) == (*colit))
			{
				++other;		// ���̓����蔻���T��
				continue;
			}
			if (!(*other)->GetEnable())
			{
				++other;		// ���̓����蔻���T��
				continue;
			}
			if (!(*other)->GetFollowObject()->GetVisible())
			{
				++other;		// ���̓����蔻���T��
				continue;
			}

			// ---�����蔻��`�F�b�N(�Փˎ�true)---
			if ((*colit)->CheckCollision(*other))
			{
				(*colit)->OnCollision((*other)->GetFollowObject());		// ���������Ƃ��̏���
			}
			++other;			// �C�e���[�^�����炷
		}
		(*colit)->ReflectMovement();		// �߂荞�ݗʂ�߂�����
		++colit;				// �C�e���[�^�����炷
	}
}

// ===�f�o�b�O�\��===
void CollisionManager::Draw()
{
	auto it = m_CollisionList.begin();			// �����蔻�胊�X�g�̐擪�C�e���[�^�擾
	while (it != m_CollisionList.end())			// �I�[�܂ŌJ��Ԃ�
	{
		(*it)->Draw();				// �f�o�b�O�\������
		++it;						// �C�e���[�^�����炷
	}
}

// ===�f�o�b�O�\���L���̐ؑ�===
void CollisionManager::SwitchVisibleCollision(Object* obj)
{
	auto it = m_CollisionList.begin();			// �����蔻�胊�X�g�̐擪�C�e���[�^�擾
	while (it != m_CollisionList.end())			// �I�[�܂ŌJ��Ԃ�
	{
		if ((*it)->GetFollowObject() == obj)	// �����̃I�u�W�F�N�g�ƎQ�Ƃ��Ă���I�u�W�F�N�g����v���Ă����烋�[�v���甲����
			break;
		++it;						// �C�e���[�^�����炷
	}
	if(it != m_CollisionList.end())	// �I�u�W�F�N�g�����������ꍇ
		(*it)->SwitchVisible();		// �f�o�b�O�\���̐ؑ�
}