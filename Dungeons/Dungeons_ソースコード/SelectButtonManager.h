#pragma once
// ===�{�^���I���Ǘ��N���X��`===
// �{�^���I���Ǘ��N���X���`
// Author kimura
// ======================

// ===�C���N���[�h===
#include "main.h"
#include "UIButton.h"
#include <vector>

// ===�萔�E�}�N����`===
#define BUTTON (SelectButtonManager::Get())			// �C���X�^���X�擾

// ===�N���X��`===
class SelectButtonManager
{
	// ---�����o�֐�---
public:
	SelectButtonManager();					// �R���X�g���N�^
	~SelectButtonManager();					// �f�X�g���N�^

	static void Create();					// ����
	static void Destroy();					// �j��
	static SelectButtonManager* Get();		// �擾

	void Update();							// ���͓ǂݎ��
	void SetSelect(int);					// �I���X�V

	int AddButtonManager(UIButton*);		// �Ǘ����ɒǉ�
	void Release(UIButton*);				// �Ǘ����珜�O

	// ---�����o�ϐ�---
private:
	static SelectButtonManager* m_pInstance; // �C���X�^���X
	std::vector<UIButton*> m_pButtonList;	// �{�^���ꗗ

	UIButton* m_pCurrentSelect;				// ���ݑI�𒆂̃{�^��
	int m_nButtonCnt;						// ���݃{�^����
};