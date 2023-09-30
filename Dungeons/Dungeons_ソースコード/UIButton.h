#pragma once
// ===�{�^��UI�x�[�X�N���X��`===
// �{�^��UI�x�[�X�N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "UIBase.h"
#include "main.h"

// ===�N���X��`===
class UIButton : public UIBase
{
	// ---�����o�֐�---
public:
	UIButton();					// �R���X�g���N�^
	virtual ~UIButton();		// �f�X�g���N�^

	void Update();					// �X�V����
	virtual void OnPushed() = 0;	// �{�^����������
	void SetSelected(bool flg) { m_bSelected = flg; }		// �I������Ă��邩�ǂ���

	int& GetIndex() { return m_nIndex; }		// �{�^���ԍ��擾

	// ---�����o�ϐ�---
private:
	bool m_bSelected;									// �I������Ă��邩�ǂ���
	int m_nIndex;										// �{�^���ԍ�
};