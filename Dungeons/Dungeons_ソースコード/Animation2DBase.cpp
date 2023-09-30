// ===2D�A�j���[�V�����x�[�X�N���X����===
// 2D�A�j���[�V�����x�[�X�N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "Animation2DBase.h"
#include "ObjectManager.h"
#include "DebugProc.h"

// ===�R���X�g���N�^===
Animation2DBase::Animation2DBase()
{
	// ---����������---
	m_fdefault = 30.0f;
	m_fSpd = m_fdefault;
	m_nFrame = 0;
	m_nOld = m_nFrame;
	m_nStart = 0;
	m_nAnimCnt = m_TexCut.x * m_TexCut.y;

	m_Loop = false;
	m_bDestroy = false;

	m_nFrame = 0;								// �Đ����t���[����
	m_nOld = m_nFrame;							// �O�t���[��
	m_nStart = 0;								// �A�j���[�V�����̊J�nuv
	m_nAnimCnt = m_TexCut.x * m_TexCut.y;		// �A�j���[�V�������閇��
	m_fdefault = 30.0f;
	m_fSpd = m_fdefault;		// �Đ��X�s�[�h(�v�Z�p)

	m_Loop = false;				// �Đ���������
	m_bEnd = false;				// �Đ��I���t���O
	m_bDestroy = false;			// �Đ��I���Ŕj�����邩�ǂ���
}

// ===�f�X�g���N�^===
Animation2DBase::~Animation2DBase()
{
}

//===�X�V����===
void Animation2DBase::Update()
{
	m_bEnd = false;
	m_fSpd--;				// �Đ��X�s�[�h�X�V

	// ---�Đ�����---
	if (m_fSpd < 0)
	{
		// ---uv�X�V---
		if (m_nFrame != m_nOld)
		{
			m_uv.x = (float)(m_nFrame % (int)m_TexCut.x) / m_TexCut.x;
			m_uv.y = (float)(m_nFrame / (int)m_TexCut.x) / m_TexCut.y;
			m_nOld = m_nFrame;
		}
		m_nFrame++;															// �A�j���[�V�����t���[�����Z
		m_fSpd = m_fdefault;												// �Đ��b�����Z�b�g

		// ---�Đ��I��---
		if (m_nFrame % (m_nStart + m_nAnimCnt) == 0)
		{
			m_bEnd = true;					// �I���t���O
			if (m_Loop)						// ���[�v����
				m_nFrame = m_nStart;
			else if (m_bDestroy)			// �j������
				OBJECT->Release(this);		// �I�u�W�F�N�g��j��
			else
			{
				m_nFrame = m_nStart;		// �ŏ��̃t���[���Ɉړ�
				m_bEnable = false;			// �`��E�X�V��~
			}
		}
	}
}

// ===�Đ��X�s�[�h�Z�b�g===
void Animation2DBase::SetSpeed(float f)
{
	m_fdefault = f;
}

// ===�A�j���[�V���������Z�b�g===
void Animation2DBase::SetMaxAnim(int n)
{
	m_nAnimCnt = n;
}

// ===�J�n�t���[���Z�b�g===
void Animation2DBase::SetStartFrame(int n)
{
	m_nStart = n;
	m_nFrame = m_nStart;
}

// ===�A�j���[�V�����I������===
bool Animation2DBase::GetEnd()
{
	return m_bEnd;
}

// ===���ݍĐ��t���[���擾===
int Animation2DBase::GetFrame()
{
	return m_nFrame;
}

// ===�A�j���[�V�����I���Z�b�g===
void Animation2DBase::SetnEnd()
{
	m_bEnd = false;
}