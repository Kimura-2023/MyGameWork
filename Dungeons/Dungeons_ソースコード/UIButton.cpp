// ===�{�^��UI�x�[�X�N���X����===
// �{�^��UI�x�[�X�N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "UIButton.h"
#include "Input.h"
#include "SelectButtonManager.h"

// ===�R���X�g���N�^===
UIButton::UIButton()
{
	// ---����������---
	m_bSelected = false;						// ��I�����
	m_nIndex = BUTTON->AddButtonManager(this);	// �{�^���Ǘ����ɒǉ�
}

// ===�f�X�g���N�^===
UIButton::~UIButton()
{
	BUTTON->Release(this);						// �{�^���Ǘ���������
}

// ===�X�V����===
void UIButton::Update()
{
	// ---�X�V����---
	if (!m_bEnable)
		return;

	// ---�X�N���[����ł̃}�E�X�J�[�\���Ƃ̏Փ˔���(AABB)---
	XMFLOAT2 min, max;
	min = XMFLOAT2(m_transform.Pos.x - m_transform.Scale.x / 2, m_transform.Pos.y - m_transform.Scale.y / 2);			// ������W
	max = XMFLOAT2(m_transform.Pos.x + m_transform.Scale.x / 2, m_transform.Pos.y + m_transform.Scale.y / 2);			// �E�����W

	POINT mouse = *CInput::GetMousePosition();																			// �}�E�X���W�擾
	XMFLOAT2 MousePos = XMFLOAT2((float)mouse.x, (float)mouse.y);														// �v�Z�����邽�߂ɕϊ�
	MousePos = XMFLOAT2(MousePos.x - SCREEN_WIDTH / 2, -MousePos.y + SCREEN_HEIGHT / 2);								// �X�N���[���Ɏ��܂�悤�ɕ␳
	if (min.x <= MousePos.x && max.x >= MousePos.x && min.y <= MousePos.y && max.y >= MousePos.y)						// �X�N���[����ŃJ�[�\�����d�Ȃ��Ă���ꍇ
	{
		BUTTON->SetSelect(m_nIndex);		// �I����Ԃɂ���
	}

	// ---�I����Ԃ��Ƃ̏���---
	if (m_bSelected)
		m_color.w = 1.0f;				// �W��
	else
		m_color.w = 0.6f;				// �I������Ă���{�^���̓��l�𔖂�����
}