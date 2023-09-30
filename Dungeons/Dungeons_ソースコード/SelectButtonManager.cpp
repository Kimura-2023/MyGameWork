// ===�{�^���I���Ǘ��N���X����===
// �{�^���I���Ǘ��N���X������
// Author kimura
// ======================

// ===�C���N���[�h===
#include "SelectButtonManager.h"
#include "Input.h"

// ===�ÓI�����o===
SelectButtonManager* SelectButtonManager::m_pInstance = nullptr;			// �C���X�^���X

// ===�R���X�g���N�^===
SelectButtonManager::SelectButtonManager()
{
	// ---����������---
	m_nButtonCnt = 0;					// �I��ԍ�
	m_pCurrentSelect = nullptr;			// ���ݑI���{�^��
}

// ===�f�X�g���N�^===
SelectButtonManager::~SelectButtonManager()
{
}

// ===�C���X�^���X����===
void SelectButtonManager::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new SelectButtonManager;		// �C���X�^���X�𐶐�
	}
}

// ===�C���X�^���X�擾===
SelectButtonManager* SelectButtonManager::Get()
{
	return m_pInstance;						// �C���X�^���X���
}

// ===�C���X�^���X�j��===
void SelectButtonManager::Destroy()
{
	delete m_pInstance;						// �C���X�^���X���
}

// ===�X�V����===
void SelectButtonManager::Update()
{
	// ---�I�𒆃{�^����������Ή������Ȃ�---
	if (!m_pCurrentSelect)
		return;

	// ---�L�[���͂ł̑I���X�V---
	if (CInput::GetKeyRepeat(VK_DOWN) || CInput::GetKeyRepeat(VK_S) || CInput::GetStickLTrigger(JOYSTICKID1,DR_DOWN,20))			// ������
	{
		int index = (m_pCurrentSelect->GetIndex() + 1) % m_nButtonCnt;		// �I��ԍ��X�V
		SetSelect(index);													// �I���{�^����ݒ�
	}
	if (CInput::GetKeyRepeat(VK_UP) || CInput::GetKeyRepeat(VK_W) || CInput::GetStickLTrigger(JOYSTICKID1, DR_UP, 20))				// �����
	{
		int index = m_pCurrentSelect->GetIndex() - 1;						// �I��ԍ��X�V
		if (index < 0)
			index = m_nButtonCnt - 1;										// �I��ԍ��␳
		SetSelect(index);													// �I���{�^����ݒ�
	}

	// ---�{�^���X�V�ۂŔ���---
	if (!m_pCurrentSelect->GetEnable())
		return;

	// ---�{�^����������---
	if (CInput::GetKeyTrigger(VK_RETURN) || CInput::GetMouseTrigger(MOUSEBUTTON_L) || CInput::GetJoyTrigger(JOYSTICKID1,VB_A))
		m_pCurrentSelect->OnPushed();				// �{�^����������
}

// ===�I���{�^���ݒ�===
void SelectButtonManager::SetSelect(int index)
{
	m_pCurrentSelect->SetSelected(false);			// �O�̃{�^�����I����Ԃ�
	m_pCurrentSelect = m_pButtonList[index];		// �{�^���X�V
	m_pCurrentSelect->SetSelected(true);			// �{�^���̑I����ԍX�V
}

// ===���X�g�ǉ�===
int SelectButtonManager::AddButtonManager(UIButton* button)
{
	if (button)
	{
		m_pButtonList.push_back(button);	// ���X�g�ɒǉ�
		++m_nButtonCnt;						// ���{�^���������Z
	}

	// ---�{�^�����Z�b�g����Ă��Ȃ��ꍇ---
	if (!m_pCurrentSelect)
	{
		m_pCurrentSelect = button;			// �I����Ԃ̃{�^���ɐݒ�
	}

	// ---�ǉ��㏈��---
	return m_nButtonCnt - 1;				// �C���f�b�N�X��Ԃ�
}

// ===�{�^�����===
void SelectButtonManager::Release(UIButton* button)
{
	// ---�I������---
	if (m_pCurrentSelect == button)
		m_pCurrentSelect = nullptr;					// �I�����Ă��镨�ł���΍X�V

	auto it = m_pButtonList.begin();				// �{�^�����X�g�̐擪�C�e���[�^�擾
	while (*it != button)							// ��v����܂ŌJ��Ԃ�
	{
		++it;										// �C�e���[�^�����炷
	}
	m_pButtonList.erase(it);						// ���X�g���珜�O
	--m_nButtonCnt;									// �{�^��������
}